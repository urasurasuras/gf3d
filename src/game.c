#include <SDL.h>            

#include "simple_logger.h"

#include "game.h"

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    Uint8 validate = 1;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;


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
        1200,                   //screen width
        700,                    //screen height
        vector4d(0.51,0.75,1,1),//background color
        0,                      //fullscreen
        validate                //validation
    );
	slog_sync();

    entity_init(32);

    Vector2D mousePos;
    int mx, my;
    int lastMx, lastMy;
    lastMx = 0;
    lastMy = 0;
    Matrix4 camMatrix;

    // Create ent
    Entity* playerDino = entity_new();
    playerDino->model = gf3d_model_load("dino");
    gfc_matrix_identity(playerDino->modelMatrix);
    gfc_matrix_make_translation(
        playerDino->modelMatrix,
        vector3d(-20, 0, 0)
    );
    playerDino->think = player_think;
    playerDino->position.y = -20;

        // Create ent
    Entity* playerDino2 = entity_new();
    playerDino2->model = gf3d_model_load("dino");
    gfc_matrix_identity(playerDino2->modelMatrix);
    gfc_matrix_make_translation(
        playerDino2->modelMatrix,
        vector3d(50, 0, 0)
    );
    playerDino2->position.y = 50;

        // Create ent
    Entity* playerDino3 = entity_new();
    playerDino3->model = gf3d_model_load("dino");
    gfc_matrix_identity(playerDino3->modelMatrix);
    gfc_matrix_make_translation(
        playerDino3->modelMatrix,
        vector3d(-20, 0, 0)
    );
    playerDino3->position.x = -20;
    playerDino3->position.y = 20;
    //// Create ent
    //Entity* playerDino4 = entity_new();
    //playerDino4->model = gf3d_model_load("dino");
    //gfc_matrix_identity(playerDino4->modelMatrix);
    //gfc_matrix_make_translation(
    //    playerDino4->modelMatrix,
    //    vector3d(-20, 0, 0)
    //);
    //playerDino4->position.x = 20;
    //playerDino4->position.y = 20;

    // Create ent
    /*Entity* floor = entity_new();
    floor->model = gf3d_model_load("floor");    
    gfc_matrix_identity(floor->modelMatrix);
    gfc_matrix_make_translation(
        floor->modelMatrix,
        vector3d(10, 10, 10)
    );

    gfc_matrix_rotate(
        floor->modelMatrix,
        floor->modelMatrix,
        40,
        vector3d(1, 0, 0)
    );
    gfc_matrix_rotate(
        floor->modelMatrix,
        floor->modelMatrix,
        40,
        vector3d(0, 1, 0)
    );
    gfc_matrix_rotate(
        floor->modelMatrix,
        floor->modelMatrix,
        40,
        vector3d(0, 0, 1)
    );*/
    //floor->position.x = 20;
    

    // main game loop
    slog("gf3d main loop begin");
	slog_sync();

    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        SDL_GetRelativeMouseState(&lastMx, &lastMy);
        SDL_GetMouseState(&mx, &my);

        //update game things here
     
        //gfc_matrix_make_translation(gf3d_vgraphics_get_uniform_buffer_object().view, vector3d(1, 0, 0));
        /*gf3d_vgraphics_rotate_camera(vector3d(0,0,1), (lastMx)*0.01);
        gf3d_vgraphics_rotate_camera(vector3d(1,0,0), (lastMy)*0.01);*/
        gf3d_get_cam()->rotation.x += lastMx * 0.01;
        gf3d_get_cam()->rotation.y += lastMy * 0.01;

        //vector3d_normalize(&gf3d_get_cam()->rotation);

        slog("Camera rotation: %.2f,%.2f", gf3d_get_cam()->rotation.x, gf3d_get_cam()->rotation.y);
        Vector3D axis_X;
        axis_X = vector3d(
            cos(gf3d_get_cam()->rotation.x + M_PI * 0.5) ,
            sin(gf3d_get_cam()->rotation.x + M_PI * 0.5),
            0);
        slog("X axis: %.2f,%.2f", axis_X.x, axis_X.y);

        FPSViewRH(
            gf3d_get_cam(),
            vector3d(0, 0, 0),
            -gf3d_get_cam()->rotation.y,
            -gf3d_get_cam()->rotation.x
        );

        /*
        gf3d_vgraphics_rotate_camera(
           vector3d(                             
               cos(gf3d_get_cam()->rotation.x),
               sin(gf3d_get_cam()->rotation.x),
               0
               ),
            lastMy * 0.01
            );
        gf3d_vgraphics_rotate_camera(
            vector3d(
                0, 
                0, 
                1
            ),
            -lastMx * 0.01
        );      */
        //gf3d_vgraphics_rotate_camera(
        //    vector3d(1, 0 ,0),
        //    -lastMx * 0.01
        //);      


        /*gf3d_vgraphics_rotate_camera(vector3d(0,0,1), (-lastMy)*0.01);
        gf3d_vgraphics_rotate_camera(vector3d(0,0,1), (-lastMx)*0.01);*/

        /*gf3d_camera_get_view(camMatrix);*/
        //gfc_matrix_slog(gf3d_vgraphics_get_uniform_buffer_object().view);

        /*mx = 0;
        my = 0;*/
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

                // Print a report once per second
                if (SDL_GetTicks() > lastUpdate + 20) {
                    entity_think_all();
                    lastUpdate = SDL_GetTicks();
                }
     /*           gfc_matrix_rotate(
                    modelMat,
                    modelMat,
                    0.002,
                    vector3d(1, 0, 0));*/
                entity_draw_all(bufferFrame, commandBuffer);
                
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
