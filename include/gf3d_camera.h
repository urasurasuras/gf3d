#ifndef __GF3D_CAMERA_H__
#define __GF3D_CAMERA_H__

#include "gfc_matrix.h"
#include "gf3d_vgraphics.h"
#include "entity.h"

typedef struct Camera_S {
    Matrix4 view;
    Vector3D position;
    Vector3D rotation;

    Entity * player;

}Camera;

Camera * gf3d_get_cam();

/**
 * @brief get the current camera view
 * @param view output, the matrix provided will be populated with the current camera information
 */
void gf3d_camera_get_view(Matrix4 *view);

/**
 * @brief set the current camera based on the matrix provided
 */
void gf3d_camera_set_view(Matrix4 *view);

/**
 * @brief set the camera properties based on position and direction that the camera should be looking
 * @param position the location for the camera
 * @param target the point the camera should be looking at
 * @param up the direction considered to be "up"
 */
void gf3d_camera_look_at(
    Vector3D position,
    Vector3D target,
    Vector3D up
);
void gf3d_rotate_camera(Vector3D axis, float degrees);
/**
 * @brief explicitely set the camera positon, holding all other parameters the same
 * @param position the new position for the camera
 */
void gf3d_camera_set_position(Vector3D position);

/**
 * @brief move the camera relatively based on the vector provided
 * @param move the ammount to move the camera
 */
void gf3d_camera_move(Vector3D move);

/*
* @brief rotate camera
* @param out Output for rotation matrix
* @param eye Location of camera eye
* @param pitch X axis by radians
* @param yaw Y axis by radians
*/
void gf3d_camera_FPS_rotation(Matrix4 out, Vector3D eye, float pitch, float yaw);
#endif
