#include "gfc_matrix.h"
#include "gf3d_camera.h"

#include <string.h>


static Camera camera = { 0 };

Camera * gf3d_get_cam() {
    return &camera;
}

void gf3d_camera_get_view(Matrix4 *view)
{
    if (!view)return;
    memcpy(view, camera.view,sizeof(Matrix4));
}

void gf3d_camera_set_view(Matrix4 *view)
{
    if (!view)return;
    memcpy(camera.view,view,sizeof(Matrix4));
}

void gf3d_camera_look_at(
    Vector3D position,
    Vector3D target,
    Vector3D up
)
{
    gfc_matrix_view(
        camera.view,
        position,
        target,
        up
    );
}

void gf3d_rotate_camera(Vector3D axis, float degrees)
{
    gfc_matrix_rotate(
        camera.view,
        camera.view,
        degrees,
        axis);
    
}

void gf3d_camera_set_position(Vector3D position)
{
    camera.view[0][3] = position.x;
    camera.view[1][3] = position.y;
    camera.view[2][3] = position.z;
}

void gf3d_camera_move(Vector3D move)
{
    camera.view[0][3] += move.x;
    camera.view[1][3] += move.y;
    camera.view[2][3] += move.z;
}

// Pitch must be in the range of [-90 ... 90] degrees and 
// yaw must be in the range of [0 ... 360] degrees.
// Pitch and yaw variables must be expressed in radians.
void gf3d_camera_FPS_rotation(Matrix4 out, Vector3D eye, float pitch, float yaw)
{

    // I assume the values are already converted to radians.
    float cosPitch = cos(pitch);
    float sinPitch = sin(pitch);
    float cosYaw = cos(yaw);
    float sinYaw = sin(yaw);

    Vector3D xaxis = { cosYaw, 0, -sinYaw };
    Vector3D yaxis = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
    Vector3D zaxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };

    // Create a 4x4 view matrix from the right, up, forward and eye position vectors
    Matrix4 viewMatrix;

    viewMatrix[0][0] = xaxis.z;
    viewMatrix[0][1] = yaxis.z;
    viewMatrix[0][2] = zaxis.z;
    viewMatrix[0][3] = 0;

    viewMatrix[1][0] = xaxis.x;
    viewMatrix[1][1] = yaxis.x;
    viewMatrix[1][2] = zaxis.x;
    viewMatrix[1][3] = 0;

    viewMatrix[2][0] = xaxis.y;
    viewMatrix[2][1] = yaxis.y;
    viewMatrix[2][2] = zaxis.y;
    viewMatrix[2][3] = 0;

    viewMatrix[3][0] = -vector3d_dot_product(xaxis, eye);
    viewMatrix[3][1] = -vector3d_dot_product(yaxis, eye);
    viewMatrix[3][2] = -vector3d_dot_product(zaxis, eye);
    viewMatrix[3][3] = 1;

    gfc_matrix_copy(out, viewMatrix);
}
/*eol@eof*/
