#ifndef _DINO_H_
#define _DINO_H_

#include "game.h"
#include "entity.h"
#include "projectile.h"


void spawn_dino_yellow_random();
void spawn_dino_red_random();
void spawn_dino_blue_random();
void dino_touch(Entity* self, Entity* other);
void dino_think(Entity * self);

#endif