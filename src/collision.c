#include "gfc_vector.h"

int collide_circle(Vector2D p1, float r1, Vector2D p2, float r2){
    if (vector2d_magnitude_squared(
        vector2d( p2.x - p1.x , p2.y - p1.y)) < ((r2+r1)*(r2+r1))
        )
    {
        return 1;
    }
    return 0;
}

int collide_sphere(Vector3D p1, float r1, Vector3D p2, float r2){

    Vector3D diff;
    vector3d_sub(diff, p2, p1);
    if (
        vector3d_magnitude_squared(diff) < ((r2+r1)*(r2+r1))
        )
    {
        return 1;
    }
    return 0;
}

int collide_rect(SDL_Rect r1,SDL_Rect r2){
    if (((r1.x + r1.w) < r2.x) ||
       ((r2.x + r2.w) < r1.x) ||
       ((r1.y + r1.h) < r2.y) ||
       ((r2.y + r2.h) < r1.y))
    {
        return 0;
    }
    return 1;
}

int collide_menu(SDL_Rect r1, Vector2D m){
    if (((r1.x + r1.w) <      m.x)  ||
       ((r1.y + r1.h) <       m.y) ||
       (r1.x >                m.x)  ||
       (r1.y >                m.y))
    {
        return 0;
    }
    return 1;
}

// POINT/CIRCLE
int pointCircle(Vector2D p, Vector2D c, float r) {

  // get distance between the point and circle's center
  // using the Pythagorean Theorem
  float distX = p.x - c.x;
  float distY = p.y - c.y;
  float distance = sqrt( (distX*distX) + (distY*distY) );

  // if the distance is less than the circle's
  // radius the point is inside!
  if (distance <= r) {
    return true;
  }
  return false;
}
// POINT/sphere
int pointSphere(Vector3D p, Vector3D c, float r) {

  // get distance between the point and circle's center
  // using the Pythagorean Theorem
    Vector3D diff;
    vector3d_sub(diff, p, c);

  // if the distance is less than the circle's
  // radius the point is inside!
  if (vector3d_magnitude_squared(diff) <= r*r) {
    return true;
  }
  return false;
}

// LINE/POINT
int linePoint(Vector3D start, Vector3D end, Vector3D p){

  // get distance from the point to the two ends of the line
  float d1 = vector3d_magnitude_between(p, start);
  float d2 = vector3d_magnitude_between(p, end);

  // get the length of the line
  float lineLen = vector3d_magnitude_between(start, end);

  // since floats are so minutely accurate, add
  // a little buffer zone that will give collision
  float buffer = 0.1;    // higher # = less accurate

  // if the two distances are equal to the line's
  // length, the point is on the line!
  // note we use the buffer here to give a range,
  // rather than one #
  if (d1+d2 >= lineLen-buffer && d1+d2 <= lineLen+buffer) {
    return true;
  }
  return false;
}


// LINE/CIRCLE
int lineCircle(Vector3D start, Vector3D end, Vector3D c, float r){

  // is either end INSIDE the circle?
  // if so, return true immediately
  int inside1 = pointSphere(start, c, r);
  int inside2 = pointSphere(end, c, r);
  if (inside1 || inside2) return true;

  // get length of the line
  float len = vector3d_magnitude_between(start, end);
  //float distX = start.x - end.x;
  //float distY = start.y - end.y;
  //float len = sqrt( (distX*distX) + (distY*distY) );

  // get dot product of the line and circle
  /*vector3d_dot_product*/
  float dot = ( ((c.x-start.x)*(end.x-start.x)) + ((c.y-start.y)*(end.y-start.y)) ) / pow(len,2);

  // find the closest point on the line
  float closestX = start.x + (dot * (end.x-start.x));
  float closestY = start.y + (dot * (end.y-start.y));
  float closestZ = start.z + (dot * (end.z-start.z));

  // is this point actually on the line segment?
  // if so keep going, but if not, return false
  int onSegment = linePoint(start,end, vector3d(closestX,closestY,closestZ));
  if (!onSegment) return false;

  // optionally, draw a circle at the closest
  // point on the line
//   fill(255,0,0);
//   noStroke();
//   ellipse(closestX, closestY, 20, 20);

  // get distance to closest point
    float distX = closestX - c.x;
    float distY = closestY - c.y;
    float distZ = closestZ - c.z;
    float distance = sqrt( (distX*distX) + (distY*distY) + (distZ*distZ));

  if (distance <= r) {
    return true;
  }
  return false;
}
//http://www.jeffreythompson.org/collision-detection/line-circle.php