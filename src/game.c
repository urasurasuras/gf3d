#include "game.h"

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    Uint8 validate = 1;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;

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


    lastMx = 0;
    lastMy = 0;
    Matrix4 camMatrix;
    Model * dinoModel = gf3d_model_load("dino");

    // Create ent
    Entity* playerDino = entity_new();
    playerDino->model = dinoModel;
    gfc_matrix_identity(playerDino->modelMatrix);
    gfc_matrix_make_translation(
        playerDino->modelMatrix,
        vector3d(0, 0, 0)
    );
    playerDino->position.y = -20;

        // Create ent
    Entity* playerDino2 = entity_new();
    playerDino2->model = gf3d_model_load("dino");
    
    gfc_matrix_identity(playerDino2->modelMatrix);
    gfc_matrix_make_translation(
        playerDino2->modelMatrix,
        vector3d(0, 0, 0)
    );
    playerDino2->position.y = 20;

    //    // Create ent
    //Entity* floor = entity_new();
    //floor->model = gf3d_model_load("floor");
    //gfc_matrix_identity(floor->modelMatrix);
    //
    //floor->position.x = 0;
    //floor->position.y = 20;
    //floor->position.z = -10;

    //// Create ent
    //Entity* playerDino4 = entity_new();
    //playerDino4->model = dinoModel;
    //gfc_matrix_identity(playerDino4->modelMatrix);
    //gfc_matrix_make_translation(
    //    playerDino4->modelMatrix,
    //    vector3d(-20, 0, 0)
    //);
    //playerDino4->position.x = 20;
    //playerDino4->position.y = 20;

    // Create player
    Entity* player = entity_new();
    gf3d_get_cam()->player = player;
    player->think = player_think;
    player->type = ent_PLAYER;
    player->speed = 0.01;

    // Create ent
    
    

    // main game loop
    slog("gf3d main loop begin");
	slog_sync();

    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //SDL_GetRelativeMouseState(&lastMx, &lastMy);
        SDL_GetMouseState(&mx, &my);

        //update game things here

        Vector3D camMove = { 1,1,1 };
        //gf3d_camera_move(camMove);
     /*   gfc_matrix_rotate(
            modelMat,
            modelMat,
            0.002,
            vector3d(1,0,0));
        gfc_matrix_rotate(
            modelMat2,
            modelMat2,
            0.002,
            vector3d(0,0,1));*/
        
        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);

            /*Matrix4 testMat;

                gfc_matrix_rotate(
                    &testMat,
                    &testMat,
                    0.1,
                    vector3d(1, 0, 0)
                );

                gfc_matrix_slog(testMat);*/

                gf3d_get_cam()->rotation.x += (mx - half_w) * 0.001;
                gf3d_get_cam()->rotation.y += (my - half_h) * 0.001;

                // Lock rotation
                if (gf3d_get_cam()->rotation.y > (M_PI * .5)) {
                    gf3d_get_cam()->rotation.y = (M_PI * .5);
                }else if (gf3d_get_cam()->rotation.y < -(M_PI * .5)) {
                    gf3d_get_cam()->rotation.y = -(M_PI * .5);
                }
                /*if (gf3d_get_cam()->rotation.x > M_PI) {
                    gf3d_get_cam()->rotation.x = M_PI;
                }else if (gf3d_get_cam()->rotation.x < -M_PI) {
                    gf3d_get_cam()->rotation.x = -M_PI;
                }*/
                //slog("Camera rotation: %.2f,%.2f", gf3d_get_cam()->rotation.x, gf3d_get_cam()->rotation.y);

                gf3d_camera_FPS_rotation(
                    gf3d_get_cam(),
                    player->position,
                    -gf3d_get_cam()->rotation.y,
                    -gf3d_get_cam()->rotation.x
                );

                //slog("player facing: %.2f,%.2f", player->facingDirection.x, player->facingDirection.y);

                //gf3d_camera_set_position(player->position);
                gf3d_vgraphics_update_view();

                //// Print a report once per second
                //if (SDL_GetTicks() > lastUpdate + 0) {
                //    lastUpdate = SDL_GetTicks();
                //}
                entity_think_all();
                /*gfc_matrix_rotate(
                    &floor->modelMatrix,
                    &floor->modelMatrix,
                    0.1,
                    vector3d(0, 1, 0)
                );*/

                //gfc_matrix_slog(floor->modelMatrix);
                entity_draw_all(bufferFrame, commandBuffer);
                //gfc_matrix_slog(floor->modelMatrix);


                SDL_WarpMouseInWindow(NULL, half_w, half_h);
                
            gf3d_command_rendering_end(commandBuffer);
            
        gf3d_vgraphics_render_end(bufferFrame);

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }    
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
