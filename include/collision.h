#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#include "gfc_vector.h"

/**
 * @brief Circle-to-circle collision
 * @param p1 Center of circle 1
 * @param r1 Radius of circle 1
 * @param p2 Center of circle 2
 * @param r2 Radius of circle 2
 * @return 1 if they are colliding
 * */
int collide_circle(Vector2D p1, float r1, Vector2D p2, float r2);
int collide_sphere(Vector3D p1, float r1, Vector3D p2, float r2);

/**
 * @brief Rectangle-to-rectangle collision
 * @param r1 Rectangle 1
 * @param r2 Rectangle 2
 * @return 1 if they are colliding
 * */
int collide_rect(SDL_Rect r1,SDL_Rect r2);

int collide_menu(SDL_Rect r1, Vector2D m);

int pointCircle(Vector2D p, Vector2D c, float r);

int pointSphere(Vector3D p, Vector3D c, float r);

int linePoint(Vector3D start, Vector3D end, Vector3D p);

int lineCircle(Vector3D start, Vector3D end, Vector3D c, float r);

#endif