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
    gf3d_vgraphics_rotate_camera();
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

/*eol@eof*/
