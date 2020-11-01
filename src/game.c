#include "game.h"
#include "gfc_matrix.h"

void floor_rotate(Entity* self, float deltaTime) {
    if (keys[SDL_SCANCODE_Q]) {
        self->rotation.x += (deltaTime);
    }
    else if (keys[SDL_SCANCODE_E]) {
        self->rotation.y += (deltaTime);
    }
    else if (keys[SDL_SCANCODE_Z]) {
        self->rotation.z += (deltaTime);
    }
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

    entity_init(32);


    gameManager()->lastMx = 0;
    gameManager()->lastMy = 0;
    gameManager()->deltaTime = 0;
    Matrix4 camMatrix;
    Model * dinoModel = gf3d_model_load("dino");

    // Create PLAYER
    Entity* player = entity_new();
    player->data = malloc(sizeof(Character));
    Character* playerData = (Character*)player->data;
    strcpy(player->name, "Player");
    gf3d_get_cam()->player = player;
    player->think = player_think;
    player->type = ent_CHAR;
    playerData->speed = 20;
    player->position.x = 5;
    player->position.y = 5;
    player->position.z = 5;
    player->touch = entity_touch;
    playerData->collider_radius = 10;
    playerData->check_for_raycast = 1;
    gameManager()->player = player;

    // Create ent
    Entity* dino = entity_new();
    dino->data = malloc(sizeof(Character));
    Character* sinoData = (Character*)dino->data;
    dino->type = ent_CHAR;
    strcpy(dino->name, "Dino");
    dino->model = dinoModel;
    dino->position.y = 6;
    dino->position.z = 30;
    dino->position.x = 0;
    //dino->think = dino_think;

    sinoData->collider_radius = 6;
    sinoData->check_for_raycast = 0;
    dino->modelRotOffset = vector3d(GFC_HALF_PI,-GFC_HALF_PI,0);

    //// Create ent
    //Entity* dino2 = entity_new();
    //dino2->model = gf3d_model_load("dino");
    //dino2->position.y = 20;
    //dino2->position.z = 6;
    //dino2->position.x = 0;
    //dino2->think = dino_think;
    //dino2->modelRotOffset = vector3d(0, -GFC_HALF_PI, 0);

    
    // Create FLOOR
    //gameManager()->level = &level;
    //gameManager()->level->model = gf3d_model_load("floor");
    //gameManager()->level->modelRotOffset = vector3d(GFC_HALF_PI, 0, 0);
    Entity* floor = entity_new();
    floor->data = malloc(sizeof(Level));
    Level* floorData = (Level*)floor->data;
    floor->type = ent_LEVEL;
    strcpy(floor->name, "Floor");
    floor->model = gf3d_model_load("floor");
    floor->modelRotOffset = vector3d(GFC_HALF_PI, 0, 0);
    floorData->bounds.x = -160;
    floorData->bounds.y = 0;
    floorData->bounds.z = -160;
    floorData->bounds.w = 160;
    floorData->bounds.h = 160;
    floorData->bounds.d = 160;
    floor->data = floorData;
    gameManager()->level = floor;//FIX: level bounds not correct

    //floor->think = floor_rotate;

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
        player->rotation.x -= (gameManager()->my - half_h) * 0.001;// V look (pitch)
        player->rotation.y += (gameManager()->mx - half_w) * 0.001;// H look (yaw)

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
        NOW = SDL_GetPerformanceCounter();

        gameManager()->deltaTime = ((float)(NOW - LAST) / (float)SDL_GetPerformanceFrequency());

        if (play) {
            entity_think_all(gameManager()->deltaTime);
            slog("Update");
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
