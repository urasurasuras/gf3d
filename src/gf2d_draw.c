//#include "gf2d_draw.h"
////#include "gf2d_graphics.h"
//#include "simple_logger.h"
//
///*
// * credit circle drawing algorith:
// * http://groups.csail.mit.edu/graphics/classes/6.837/F98/Lecture6/circle.html
// */
//
//static int gf2d_draw_circle_points(SDL_Point* p, Vector2D center, Vector2D point)
//{
//    if (point.x == 0)
//    {
//        vector2d_set(p[0], center.x, center.y + point.y);
//        vector2d_set(p[1], center.x, center.y - point.y);
//        vector2d_set(p[2], center.x + point.y, center.y);
//        vector2d_set(p[3], center.x - point.y, center.y);
//        return 4;
//    }
//    if (point.x == point.y)
//    {
//        vector2d_set(p[0], center.x + point.x, center.y + point.y);
//        vector2d_set(p[1], center.x - point.x, center.y + point.y);
//        vector2d_set(p[2], center.x + point.x, center.y - point.y);
//        vector2d_set(p[3], center.x - point.x, center.y - point.y);
//        return 4;
//    }
//    if (point.x < point.y)
//    {
//        vector2d_set(p[0], center.x + point.x, center.y + point.y);
//        vector2d_set(p[1], center.x - point.x, center.y + point.y);
//        vector2d_set(p[2], center.x + point.x, center.y - point.y);
//        vector2d_set(p[3], center.x - point.x, center.y - point.y);
//        vector2d_set(p[4], center.x + point.y, center.y + point.x);
//        vector2d_set(p[5], center.x - point.y, center.y + point.x);
//        vector2d_set(p[6], center.x + point.y, center.y - point.x);
//        vector2d_set(p[7], center.x - point.y, center.y - point.x);
//        return 8;
//    }
//    return 0;
//}
//
//void gf2d_draw_circle(Vector2D center, int radius, Vector4D color)
//{
//    SDL_Point* pointArray;
//    int i = 0;
//    Vector2D point = { 0,0 };
//    int p = (5 - radius * 4) / 4;
//    point.y = radius;
//    pointArray = (SDL_Point*)malloc(sizeof(SDL_Point) * radius * 8);
//    if (!pointArray)
//    {
//        slog("gf2d_draw_circle: failed to allocate points for circle drawing");
//        return;
//    }
//    i = gf2d_draw_circle_points(&pointArray[i], center, point);
//    while (point.x < point.y)
//    {
//        point.x++;
//        if (p < 0)
//        {
//            p += 2 * point.x + 1;
//        }
//        else
//        {
//            point.y--;
//            p += 2 * (point.x - point.y) + 1;
//        }
//        i += gf2d_draw_circle_points(&pointArray[i], center, point);
//        if (i + 8 >= radius * 8)
//        {
//            slog("too many points for circle");
//            break;
//        }
//    }
//    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
//        color.x,
//        color.y,
//        color.z,
//        color.w);
//    SDL_RenderDrawPoints(gf2d_graphics_get_renderer(), pointArray, i);
//    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
//        255,
//        255,
//        255,
//        255);
//    free(pointArray);
//}
//
//
///*eol@eof*/
