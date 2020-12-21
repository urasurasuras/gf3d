#include "game.h"
#include "gfc_matrix.h"
#include "gfc_types.h"
#include "gfc_input.h"
#include "simple_json.h"
#include "menu.h"
#include "entity.h"

static GameManager game_manager = { 0 };

void game_exit(){
    game_manager.done = true; // exit condition

    // Save if in editor mode
    if (game_manager.editorMode){
        SJson *allData = NULL;
        allData = sj_object_new();
        if (!allData)return;

        SJson *data_array = NULL;
        data_array = sj_array_new();

        for (Uint32 i = 0; i < get_entity_manager()->entity_count; i++) {

            Entity * ent = NULL;
            ent = &get_entity_manager()->entity_list[i];

            if (!ent)continue;
		    if (!ent->_inuse)continue;
            if (ent->type != ent_EDITOR)continue;

            // Name
            SJson * ent_sj = sj_object_new();

                sj_object_insert(ent_sj, "Name", sj_new_str(ent->name));

            // Pos
            SJson * pos_sj = sj_array_new();
 
                sj_array_append(pos_sj,sj_new_float(ent->position.x));
                sj_array_append(pos_sj,sj_new_float(ent->position.y));
                sj_array_append(pos_sj,sj_new_float(ent->position.z));

                sj_object_insert(ent_sj, "Position", pos_sj);

            // Rot
            SJson * rot_sj = sj_array_new();
 
                sj_array_append(rot_sj,sj_new_float(ent->rotation.x));
                sj_array_append(rot_sj,sj_new_float(ent->rotation.y));
                sj_array_append(rot_sj,sj_new_float(ent->rotation.z));

                sj_object_insert(ent_sj, "Rotation", rot_sj);

            sj_array_append(data_array, ent_sj);

        }

        sj_object_insert(allData, "Ents", data_array);
        
        sj_echo(allData);
        sj_save(allData, "cfg/editor.save");
        sj_free(allData);
    }
}

void toggle_pause(){
    game_manager.paused = !game_manager.paused;
    // test_hud->_active = !test_hud->_active;
}

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

void dino_rotate_x(Entity *self){

    self->rotation.x++;

}

void dino_rotate_y(Entity *self){

    self->rotation.y++;

}
void game_main_load(){

    // Init Game Manager
    // game_manager.lastMx = 0;
    // game_manager.lastMy = 0;
    game_manager.deltaTime = 0;
    game_manager.gf3d_camera = gf3d_get_cam();


    Character* player_data = NULL;
    Entity* player = NULL;

    // Create PLAYER
    game_manager.player = player_spawn();
    
    UI_Element * hud = HUD_element_new();
    hud->sprite = gf3d_sprite_load("images/ak_sprite.png", -1, -1, 0);
    hud->position.x = 1400;
    hud->position.y = 700;

    game_manager.player_data->HUD = hud;

    // Create demo
    Entity * dino1 = NULL;
    Entity * dino2 = NULL;

    dino1 = entity_new();
    dino2 = entity_new();

    if ( !(dino1) || !(dino2)){
        slog("dino who?");
        exit(1);
    }

    dino1->think = dino_rotate_x;
    dino2->think = dino_rotate_y;

    UI_Element * bg = UI_element_new();
    bg->sprite = gf3d_sprite_load("images/bg_flat.png",-1,-1,0);
    // float bg_scale_X;
    // bg_scale_X = (float)bg->sprite->frameWidth;
    // bg->scale.x = (float)bg->sprite->frameWidth/900.0f;
    // bg->scale.y = (float)bg->sprite->frameHeight/1600.0f;
    // bg->position = vector2d(200,100);


    UI_Element * test_hud = UI_element_new();
    test_hud->sprite = gf3d_sprite_load("images/button.png",-1,-1,0);
    test_hud->position = vector2d(800,450);
    //test_hud->drawOffset = vector2d(-test_hud->sprite->frameWidth*.5, -test_hud->sprite->frameHeight*.5);
    test_hud->box.x = test_hud->position.x;
    test_hud->box.y = test_hud->position.y;
    test_hud->box.w = test_hud->sprite->frameWidth*.5;
    test_hud->box.h = test_hud->sprite->frameHeight*.5;
    test_hud->_active = 0;
    test_hud->onClick = game_exit;



    toggle_pause();
}
UI_Element * test_hud;

void game_level_load() {


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
    
    if (!game_manager.editorMode){
        gf3d_model_load_animated("dino", 1, 150);
    }
    
    SJson *save_sj = sj_load("cfg/editor.save");

    if (save_sj){
        slog("Loading from editor save");

        sj_echo(save_sj);

            SJson * ent_array = sj_object_get_value(save_sj, "Ents");

            
            for (int i = 0; i < sj_array_get_count(ent_array); i++) {

                Entity * ent = NULL;
                ent = entity_new();

                if (!ent){
                    continue;
                }
                
                SJson * ent_sj = sj_array_get_nth(ent_array, i);

                    // Name
                    SJson * name_sj = sj_object_get_value(ent_sj, "Name");

                    gfc_word_cpy(ent->name, sj_get_string_value(name_sj));

                    // Pos
                    SJson * pos_sj = sj_object_get_value(ent_sj, "Position");

                        SJson * X_pos_sj = sj_array_get_nth(pos_sj, 0);
                        SJson * Y_pos_sj = sj_array_get_nth(pos_sj, 1);
                        SJson * Z_pos_sj = sj_array_get_nth(pos_sj, 2);

                        Vector3D pos;
                        sj_get_float_value(X_pos_sj, &pos.x);
                        sj_get_float_value(Y_pos_sj, &pos.y);
                        sj_get_float_value(Z_pos_sj, &pos.z);
                        
                        vector3d_slog(pos);

                    vector3d_copy(ent->position, pos);

                    // Rot
                    SJson * rot_sj = sj_object_get_value(ent_sj, "Rotation");

                        SJson * X_rot_sj = sj_array_get_nth(rot_sj, 0);
                        SJson * Y_rot_sj = sj_array_get_nth(rot_sj, 1);
                        SJson * Z_rot_sj = sj_array_get_nth(rot_sj, 2);

                        Vector3D rot;
                        sj_get_float_value(X_rot_sj, &rot.x);
                        sj_get_float_value(Y_rot_sj, &rot.y);
                        sj_get_float_value(Z_rot_sj, &rot.z);

                        vector3d_slog(rot);

                    vector3d_copy(ent->rotation, rot);

                ent->type = ent_WALL;
                ent->max_frames = 1;
                ent->model = gf3d_model_load(ent->name);
                ent->rigidbody.collider_radius = 4;
            }
      
        sj_free(save_sj);

    }


    // spawn_dino_yellow_random();
    // spawn_dino_yellow_random();
    // spawn_dino_yellow_random();
    // spawn_dino_yellow_random();
    // spawn_dino_yellow_random();

    // test_hud->onClick = game_exit;
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
        if (strcmp(argv[a],"-editor") == 0)
        {
            slog("Editor mode");
            game_manager.editorMode = 1;
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
    gfc_input_init("");

    // Load all models once
    // gf3d_model_load("dino");
    // gf3d_model_load("dino_blue");
    // gf3d_model_load("dino_red");

    // gf3d_model_load("pickup_health");
    // gf3d_model_load("pickup_damage");
    // gf3d_model_load("pickup_speed");
    // gf3d_model_load("pickup_kaboom");
    /**/
    game_main_load();
    game_level_load();

    
    // test_hud2->_active = 1;
    // main game loop
    slog("MAIN LOOP BEGIN");
	slog_sync();

    while(!game_manager.done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //SDL_GetRelativeMouseState(&lastMx, &lastMy);
        int mx,my;
        SDL_GetMouseState(&mx, &my);
        game_manager.mousePos.x = (float)mx;
        game_manager.mousePos.y = (float)my;


        //update game things here  

        
        if (!game_manager.paused) {
            // Get mouse input delta
            game_manager.player->rotation.x -= (game_manager.mousePos.y - half_h) * 0.001;// V look (pitch)
            game_manager.player->rotation.y += (game_manager.mousePos.x - half_w) * 0.001;// H look (yaw)

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

            if (!game_manager.editorMode){
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

            entity_think_all(gameManager()->deltaTime);

            gf3d_vgraphics_update_view();

        }
        else {
            
            mouse_think();
            //slog("Game paused");
        }
        

        LAST = NOW;
        NOW = SDL_GetTicks();

        game_manager.deltaTime = ((float)(NOW - LAST) / 1000);


        // configure render command for graphics command pool
        bufferFrame = gf3d_vgraphics_render_begin();
        // for each mesh, get a command and configure it from the pool


                gf3d_pipeline_reset_frame(gf3d_vgraphics_get_models_pipeline(),bufferFrame);

                commandBuffer = gf3d_command_rendering_begin(bufferFrame, gf3d_vgraphics_get_models_pipeline());
            if (!game_manager.paused) {

                    entity_draw_all(bufferFrame, commandBuffer);
            }

                gf3d_command_rendering_end(commandBuffer);
                

                // 2D overlay rendering
                gf3d_pipeline_reset_frame(gf3d_vgraphics_get_overlay_pipeline(),bufferFrame);
                commandBuffer = gf3d_command_rendering_begin(bufferFrame,gf3d_vgraphics_get_overlay_pipeline());
            if (!game_manager.paused) {
                HUD_draw_all(bufferFrame, commandBuffer);
            }else{
                MENU_draw_all(bufferFrame, commandBuffer);
            }

                gf3d_command_rendering_end(commandBuffer);

        gf3d_vgraphics_render_end(bufferFrame);

        gfc_input_update();

        //play = 1; // start game after we are done with the first game loop
        if (game_manager.player_data->health <= 0) {
            //done = 1; 
            slog("Player health %.2f", game_manager.player_data->health);
        }

        if(gfc_input_key_pressed("TAB")){
            toggle_pause();
        }

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
