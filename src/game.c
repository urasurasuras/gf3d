#include "game.h"
#include "gfc_matrix.h"
#include "gfc_types.h"
#include "simple_json.h"
#include "menu.h"

static GameManager game_manager = { 0 };

void game_info_load(){
    SJson *cfgFile = sj_load("cfg/spawn.cfg");

    if (!cfgFile){
        slog("no config file, creating BLANK...");

        SJson *allData = NULL;
        allData = sj_object_new();
        if (!allData)return;

        sj_object_insert(allData, "yellow_dino_spawn_cldn", sj_new_int(1000));
        sj_object_insert(allData, "red_dino_spawn_cldn", sj_new_int(1000));
        sj_object_insert(allData, "blue_dino_spawn_cldn", sj_new_int(1000));
        sj_object_insert(allData, "pickup_spawn_cldn", sj_new_int(1000));

        sj_echo(allData);
        sj_save(allData, "cfg/spawn.cfg");
        sj_free(allData);

        exit(0);
        return;
    }

    sj_echo(cfgFile);

    SJson *yellow_dino_spawn_cldn = sj_object_get_value(cfgFile, "yellow_dino_spawn_cldn");;
    SJson *red_dino_spawn_cldn = sj_object_get_value(cfgFile, "red_dino_spawn_cldn");
    SJson *blue_dino_spawn_cldn = sj_object_get_value(cfgFile, "blue_dino_spawn_cldn");
    SJson *pickup_spawn_cldn = sj_object_get_value(cfgFile, "pickup_spawn_cldn");
    sj_get_integer_value(yellow_dino_spawn_cldn, &game_manager.game_info.yellow_dino_spawn_cldn);
    sj_get_integer_value(red_dino_spawn_cldn, &game_manager.game_info.red_dino_spawn_cldn);
    sj_get_integer_value(blue_dino_spawn_cldn, &game_manager.game_info.blue_dino_spawn_cldn);
    sj_get_integer_value(pickup_spawn_cldn, &game_manager.game_info.pickup_spawn_cldn);

    sj_free(cfgFile);

}

void game_level_load() {

    Character* player_data = NULL;
    Entity* player = NULL;

    // Init Game Manager
    game_manager.lastMx = 0;
    game_manager.lastMy = 0;
    game_manager.deltaTime = 0;
    game_manager.gf3d_camera = gf3d_get_cam();

    // Create PLAYER
    game_manager.player = player_spawn();


    // Create FLOOR
    Entity* floor = entity_new();
    floor->entData = malloc(sizeof(Level));
    Level* floorData = (Level*)floor->entData;
    floor->type = ent_LEVEL;
    gfc_word_cpy(floor->name, "Floor");
    floor->model = gf3d_model_load("floor");
    floor->modelRotOffset = vector3d(-GFC_HALF_PI, 0, 0);
    floorData->bounds.x = -160;
    floorData->bounds.y = 0;
    floorData->bounds.z = -160;
    floorData->bounds.w = 160;
    floorData->bounds.h = 160;
    floorData->bounds.d = 160;
    floor->entData = floorData;
    gameManager()->level = floorData;//FIX: level bounds not correct

    Entity* walls = entity_new();
    gfc_word_cpy(walls->name, "Walls");
    walls->model = gf3d_model_load("walls");
    walls->modelRotOffset = vector3d(-GFC_HALF_PI, 0, 0);

    // spawn_dino_yellow_random();
    // spawn_dino_yellow_random();
    // spawn_dino_yellow_random();
    // spawn_dino_yellow_random();
    // spawn_dino_yellow_random();
}
void floor_rotate(Entity* self, float deltaTime) {
    if (keys[SDL_SCANCODE_Q]) {
        self->rotation.x += (deltaTime);
        /*gfc_matrix_rotate(
            self->modelMatrix,
            self->modelMatrix,
            deltaTime,
            vector3d(1, 0, 0)
        );*/
    }
    else if (keys[SDL_SCANCODE_E]) {
        self->rotation.y += (deltaTime);
    }
    else if (keys[SDL_SCANCODE_Z]) {
        self->rotation.z += (deltaTime);
    }
} 

void spawn_pickup_random() {
    
    Entity* self = entity_new();
    if (!self){
        slog("NULL entity pickup");
        return;
    }

    self->type = ent_PICKUP;
    gfc_word_cpy(self->name, "Pickup");
    self->rigidbody.collider_radius = 4;
    self->think = sine_movement;
    self->position.z = gfc_crandom() * gameManager()->level->bounds.d;
    self->position.x = gfc_crandom() * gameManager()->level->bounds.w;
    int index = FLOAT_TO_INT(gfc_random() * 3.5);

    switch (index)
    {
    case 0:
        self->model = gf3d_model_load("pickup_health");
        self->touch = pickup_health;
        break;
    case 1:
        self->model = gf3d_model_load("pickup_damage");
        self->touch = pickup_damage;

        break;
    case 2:
        self->model = gf3d_model_load("pickup_speed");
        self->touch = pickup_speed;

        break;
    case 3:
        self->model = gf3d_model_load("pickup_kaboom");
        self->touch = pickup_kaboom;

        break;
    default:
        break;
    }

}


int main(int argc,char *argv[])
{
    int done = 0;
    int play = 0;
    int a;
    Uint8 validate = 0;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;

    //Camera * gf3d_camera = NULL;

    Uint32 LAST =0;
    Uint32 NOW =0;
    
    game_info_load();

    Uint32 yellow_dino_spawn_cldn = game_manager.game_info.yellow_dino_spawn_cldn;
    Uint32 yellow_dino_spawn_last = 0;
    
    Uint32 red_dino_spawn_cldn = game_manager.game_info.red_dino_spawn_cldn;
    Uint32 red_dino_spawn_last = 0;
        
    Uint32 blue_dino_spawn_cldn = game_manager.game_info.blue_dino_spawn_cldn;
    Uint32 blue_dino_spawn_last = 0;

    Uint32 pickup_spawn_cldn = game_manager.game_info.pickup_spawn_cldn;
    Uint32 pickup_spawn_last = 0;

    int half_w = window_W / 2;
    int half_h = window_H / 2;
    SDL_ShowCursor(SDL_DISABLE);
    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"-disable_validate") == 0)
        {
            validate = 0;
        }
    }
    
    init_logger("gf3d.log");    
    slog("gf3d begin");
    gf3d_vgraphics_init(
        "gf3d",                 //program name
        window_W,                   //screen width
        window_H,                    //screen height
        vector4d(0.51,0.75,1,1),//background color
        0,                      //fullscreen
        validate                //validation
    );
	slog_sync();

    entity_init(128);
    UI_manager_init(32);

    // Load all models once
    // gf3d_model_load("dino");
    // gf3d_model_load("dino_blue");
    // gf3d_model_load("dino_red");

    // gf3d_model_load("pickup_health");
    // gf3d_model_load("pickup_damage");
    // gf3d_model_load("pickup_speed");
    // gf3d_model_load("pickup_kaboom");
    /**/
    game_level_load();
    UI_Element * test_hud = UI_element_new();
    test_hud->sprite = gf3d_sprite_load("images/hud.png",-1,-1,0);
    test_hud->position = vector2d(0,0);

    // main game loop
    slog("MAIN LOOP BEGIN");
	slog_sync();

    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //SDL_GetRelativeMouseState(&lastMx, &lastMy);
        SDL_GetMouseState(&game_manager.mx, &game_manager.my);

        //update game things here
        if (!game_manager.paused) {
            // Get mouse input delta
            game_manager.player->rotation.x -= (game_manager.my - half_h) * 0.001;// V look (pitch)
            game_manager.player->rotation.y += (game_manager.mx - half_w) * 0.001;// H look (yaw)

            game_manager.gf3d_camera->rotation.x = game_manager.player->rotation.x;
            game_manager.gf3d_camera->rotation.y = game_manager.player->rotation.y;

            // Clamp rotation
            if (game_manager.player->rotation.x > 1.5) {
                game_manager.player->rotation.x = 1.5;
            }
            else if (game_manager.player->rotation.x < -1.5) {
                game_manager.player->rotation.x = -1.5;
            }

            gf3d_camera_FPS_rotation(
                game_manager.gf3d_camera->view,
                game_manager.player->position,
                game_manager.gf3d_camera->rotation.x,
                -game_manager.gf3d_camera->rotation.y
            );



            SDL_WarpMouseInWindow(NULL, half_w, half_h);

            entity_think_all(gameManager()->deltaTime);

            gf3d_vgraphics_update_view();


            if (yellow_dino_spawn_last + yellow_dino_spawn_cldn < SDL_GetTicks()) {
                yellow_dino_spawn_last = SDL_GetTicks();
                spawn_dino_yellow_random();
            }
            if (red_dino_spawn_last + red_dino_spawn_cldn < SDL_GetTicks()) {
                red_dino_spawn_last = SDL_GetTicks();
                spawn_dino_red_random();
            }
            if (blue_dino_spawn_last + blue_dino_spawn_cldn < SDL_GetTicks()) {
                blue_dino_spawn_last = SDL_GetTicks();
                spawn_dino_blue_random();
            }
            if (pickup_spawn_last + pickup_spawn_cldn < SDL_GetTicks()) {
                pickup_spawn_last = SDL_GetTicks();
                spawn_pickup_random();
            }
        }
        

        LAST = NOW;
        NOW = SDL_GetTicks();

        game_manager.deltaTime = ((float)(NOW - LAST) / 1000);


        // configure render command for graphics command pool
        bufferFrame = gf3d_vgraphics_render_begin();
        // for each mesh, get a command and configure it from the pool
            gf3d_pipeline_reset_frame(gf3d_vgraphics_get_models_pipeline(),bufferFrame);
            gf3d_pipeline_reset_frame(gf3d_vgraphics_get_overlay_pipeline(),bufferFrame);

            commandBuffer = gf3d_command_rendering_begin(bufferFrame, gf3d_vgraphics_get_models_pipeline());

                entity_draw_all(bufferFrame, commandBuffer);

            gf3d_command_rendering_end(commandBuffer);
            
            // 2D overlay rendering
            commandBuffer = gf3d_command_rendering_begin(bufferFrame,gf3d_vgraphics_get_overlay_pipeline());

                UI_draw_all(bufferFrame, commandBuffer);

            gf3d_command_rendering_end(commandBuffer);


        gf3d_vgraphics_render_end(bufferFrame);

        //play = 1; // start game after we are done with the first game loop
        if (game_manager.player_data->health <= 0) {
            //done = 1; 
            slog("Player health %.2f", game_manager.player_data->health);
        }
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        if (keys[SDL_SCANCODE_TAB])game_manager.paused = true;
    }    
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d PROGRAM END");
    slog_sync();
    return 0;
}
//Level * init_level() {
//    return &level;
//}
GameManager* gameManager() {
    return &game_manager;
}
/*eol@eof*/
