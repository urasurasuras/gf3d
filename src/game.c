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
        self->position.z -= (deltaTime *5);
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
    gf3d_get_cam()->player = player;
    player->think = player_think;
    player->type = ent_PLAYER;
    player->speed = 20;
    player->position.y = 10;

    // Create ent
    Entity* playerDino = entity_new();
    playerDino->model = dinoModel;
    playerDino->position.y = -20;
    playerDino->position.z = 6;
    playerDino->position.x = 0;

    playerDino->rotation = vector3d(0, 0, 0);

    // Create ent
    Entity* playerDino2 = entity_new();
    playerDino2->model = gf3d_model_load("dino");
    playerDino2->position.y = 20;
    playerDino2->position.z = 6;
    playerDino2->position.x = 0;
    playerDino2->think = dino_think;
    playerDino->think = dino_think;
    
    // Create FLOOR
    Entity* floor = entity_new();
    floor->model = gf3d_model_load("floor");


    // main game loop
    slog("MAIN LOOP BEGIN");
	slog_sync();

    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //SDL_GetRelativeMouseState(&lastMx, &lastMy);
        SDL_GetMouseState(&gameManager()->mx, &gameManager()->my);

        if (keys[SDL_SCANCODE_TAB]) {
            play = 1;
        }
        //update game things here
        slog("deltaTime: %f", gameManager()->deltaTime);
        // Get mouse input delta
        gf3d_get_cam()->rotation.x += (gameManager()->mx - half_w) * 0.001;
        gf3d_get_cam()->rotation.y += (gameManager()->my - half_h) * 0.001;

        // Lock rotation
        if (gf3d_get_cam()->rotation.y > GFC_HALF_PI) {
            gf3d_get_cam()->rotation.y = GFC_HALF_PI;
        }
        else if (gf3d_get_cam()->rotation.y < -GFC_HALF_PI) {
            gf3d_get_cam()->rotation.y = -GFC_HALF_PI;
        }

        gf3d_camera_FPS_rotation(
            gf3d_get_cam(),
            player->position,
            -gf3d_get_cam()->rotation.y,
            -gf3d_get_cam()->rotation.x
        );

        gf3d_vgraphics_update_view();


        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        gameManager()->deltaTime = ((float)(NOW - LAST) / (float)SDL_GetPerformanceFrequency());

        if (play) {
            entity_think_all(gameManager()->deltaTime);
        }

        SDL_WarpMouseInWindow(NULL, half_w, half_h);

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);

            entity_draw_all(bufferFrame, commandBuffer);

            gf3d_command_rendering_end(commandBuffer);
            
        gf3d_vgraphics_render_end(bufferFrame);

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }    
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d PROGRAM END");
    slog_sync();
    return 0;
}
void startGame() {

}
GameManager* gameManager() {
    return &game_manager;
}
/*eol@eof*/
