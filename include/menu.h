#ifndef _MENU_H_
#define _MENU_H_

#include <stdlib.h>
#include "gf3d_sprite.h"
//#include "level.h"
#include "collision.h"
#include <SDL_ttf.h>

#define MENU_BUTTON_HALF_WIDTH 165
#define MENU_BUTTON_HALF_HEIGHT 100
typedef struct UI_Element_S
{
    TextWord    name;               /**<Name of this entity (for debugging)*/

    Uint8       _inuse;             /**<Check if entity in memory is active or not*/
    Sprite      *sprite;            /**<A pointer to the sprite*/
    int         collider_shape;     /**<Indicates whether collider is cirlular or rectangular*/
    float       frame;              /**<Current frame of sripte*/
    float       maxFrames;          /**<Maximum number of frames in a sprite*/
    Vector2D    position;           /**<2D position of entity*/
    Vector2D    drawOffset;         /**<Offset of collider*/
    Vector2D    scale;
    SDL_Rect    box;                /**<Bounds of menu*/
    SDL_Texture *Message;

    void        (*onClick)(struct UI_Element_S *self);
    void        (*think)(struct UI_Element_S *self);

}UI_Element;

/**
 * @brief get a pointer to a new UI_element
 * @return NULL on out of memory or error, a pointer to a blank entity otherwise
 */
UI_Element *UI_element_new();

/**
 * @brief initialize entity resource manager
 * @param maxEnts maximum entities
 * */
void UI_manager_init(Uint32 maxEnts);

/**
 * @brief Close entity system
 */
void UI_manager_close();

/**
 * @brief free previously allocated entity
 * @param self a pointer to the entity to free
 */
void UI_free(UI_Element *menu);

/**
 * @brief Update frame info of entity
 * @param self a pointer to the entity to update
 */
void UI_update(UI_Element *self);

/**
 * @brief update every active entity
 */
void UI_update_all();

/**
 * @brief draw every active entity
 */
void UI_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer);

/**
 * @brief For each entity, check all other entities for collision
 * */
void menu_touch_check(UI_Element *menu);

UI_Element *UI_generic(
    SDL_Rect    box,
    Vector2D    drawOffset,
    Sprite      *sprite,
    void        (*onClick)(struct UI_Element_S *self),
    TTF_Font* Sans,
    TextLine     text
);

void button_exit_think (UI_Element *self);

void button_save_think (UI_Element *self);

void button_level_think (UI_Element *self);

#endif