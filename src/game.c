#include "game.h"
#include "gfc_matrix.h"
#include "gfc_types.h"

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

void spawn_dino_random(Model * model) {
    // Create ent
    Entity* dino = entity_new();
    if (!dino)return;
    dino->entData = malloc(sizeof(Character));
    Character* sinoData = (Character*)dino->entData;
    dino->type = ent_CHAR;
    gfc_word_cpy(dino->name, "Dino");
    dino->model = gf3d_model_load("dino");
    
    
    dino->position.y = 6;//height
    dino->position.z = gfc_crandom() * gameManager()->level->bounds.d;
    dino->position.x = gfc_crandom() * gameManager()->level->bounds.w;
    //dino->think = floor_rotate;

    dino->think = dino_think;
    dino->touch = dino_touch;

    dino->rigidbody.collider_radius = 6;
    dino->rigidbody.gravity_scale = 1;
    sinoData->check_for_raycast = 0;
    dino->rigidbody.speed = 5;
    sinoData->health = 100;
    dino->modelRotOffset = vector3d(-GFC_HALF_PI, -GFC_HALF_PI, 0);
    /*vector3d_slog(dino->position);*/

    sinoData->charData = malloc(sizeof(MOB));
    MOB* mob_data = (MOB*)sinoData->charData;
    mob_data->target = gameManager()->player;
    slog("Targeting %s", mob_data->target->name);
}

void spawn_pickup_random() {
    
    Entity* self = entity_new();
    if (!self)return;

    self->type = ent_PICKUP;
    gfc_word_cpy(self->name, "Pickup");
    self->rigidbody.collider_radius = 4;
    self->think = sine_movement;

    self->position.z = gfc_crandom() * gameManager()->level->bounds.d;
    self->position.x = gfc_crandom() * gameManager()->level->bounds.w;
    int index = FLOAT_TO_INT(gfc_random() * 3);
    //slog("%d", index);
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
    default:
        break;
    }
    //self->model


}


int main(int argc,char *argv[])
{
    int done = 0;
    int play = 0;
    int a;
    Uint8 validate = 1;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;

    Uint32 LAST =0;
    Uint32 NOW =0;

    Uint32 EnemySpawnCldn = 5000;
    Uint32 lastEnemySpawn = 0;

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
    Model* dinoModel = gf3d_model_load("dino");

    gameManager()->lastMx = 0;
    gameManager()->lastMy = 0;
    gameManager()->deltaTime = 0;
    Matrix4 camMatrix;

    // Create PLAYER
    Entity* player = entity_new();
    player->entData = malloc(sizeof(Character));
    Character* playerData = (Character*)player->entData;
    gfc_word_cpy(player->name, "Player");
    gf3d_get_cam()->player = player;
    player->think = player_think;
    //player->think = floor_rotate;
    player->type = ent_CHAR;
    player->rigidbody.speed = 20;
    player->position.x = 5;
    player->position.y = 5;
    player->position.z = 5;
    player->touch = entity_touch;
    player->rigidbody.collider_radius = 4;
    player->rigidbody.gravity_scale = .5;
    playerData->check_for_raycast = 0;
    playerData->CLDN1 = 1000;
    playerData->last_CLDN1 = 0;
    playerData->CLDN2 = 100;
    playerData->last_CLDN2 = 0;
    playerData->CLDN3 = 2000;
    playerData->last_CLDN3 = 0;

    playerData->power = 1;

    playerData->primaryAttack = player_hitscan_attack;

    playerData->health = 100;

    playerData->speed_buff = 1;
    //player->model = gf3d_model_load("USP45");
    playerData->type = char_PLAYER;
    player->modelPosOffset = vector3d(0, 0, 10);
    player->modelRotOffset = vector3d(GFC_HALF_PI, 0, GFC_HALF_PI);

    gameManager()->player = player;

   
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

    spawn_dino_random(dinoModel);
    //spawn_dino_random(dinoModel);
    //spawn_dino_random(dinoModel);
    //spawn_dino_random(dinoModel);
    //spawn_dino_random(dinoModel);
    //spawn_dino_random(dinoModel);

    // main game loop
    slog("MAIN LOOP BEGIN");
	slog_sync();

    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //SDL_GetRelativeMouseState(&lastMx, &lastMy);
        SDL_GetMouseState(&gameManager()->mx, &gameManager()->my);

        //update game things here
        //slog("deltaTime: %f", gameManager()->deltaTime);
        // Get mouse input delta
        player->rotation.x -= (gameManager()->my - half_h) * gameManager()->deltaTime;// V look (pitch)
        player->rotation.y += (gameManager()->mx - half_w) * gameManager()->deltaTime;// H look (yaw)

        gf3d_get_cam()->rotation.x = player->rotation.x;
        gf3d_get_cam()->rotation.y = player->rotation.y;

        // Clamp rotation
        if (player->rotation.x > 1.5) {
            player->rotation.x = 1.5;
        }
        else if (player->rotation.x < -1.5) {
            player->rotation.x = -1.5;
        }

        gf3d_camera_FPS_rotation(
            gf3d_get_cam(),
            player->position,
            gf3d_get_cam()->rotation.x,
            -gf3d_get_cam()->rotation.y
        );



        LAST = NOW;
        NOW = SDL_GetTicks();

        gameManager()->deltaTime = ((float)(NOW - LAST) / 1000);
        /*

        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        gameManager()->deltaTime = ((float)(NOW - LAST) / (float)SDL_GetPerformanceFrequency());*/

        if (play) {
            entity_think_all(gameManager()->deltaTime);
            //slog("FPS: %.0f", 1/ gameManager()->deltaTime);
        }


        if (lastEnemySpawn + EnemySpawnCldn < SDL_GetTicks()) {
            lastEnemySpawn = SDL_GetTicks();
            //spawn_dino_random(dinoModel);
            spawn_pickup_random();
        }

        SDL_WarpMouseInWindow(NULL, half_w, half_h);

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);

            gf3d_vgraphics_update_view();

            entity_draw_all(bufferFrame, commandBuffer);

            gf3d_command_rendering_end(commandBuffer);
            
        gf3d_vgraphics_render_end(bufferFrame);

        play = 1; // start game after we are done with the first game loop
        if (playerData->health < 0)done = 1;
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
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
