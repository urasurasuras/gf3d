#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "game.h"
#include "entity.h"
#include "projectile.h"

Entity * player_spawn();

void player_think(Entity* self);

void entity_move(Entity* self);
void entity_touch(Entity* self, Entity* other);
void dino_touch(Entity* self, Entity* other);
void player_hitscan_attack(Entity* self);
void player_projectile_attack(Entity* self);

void pickup_health(Entity* self, Entity* other);
void pickup_speed(Entity* self, Entity* other);
void pickup_damage(Entity* self, Entity* other);
void pickup_kaboom(Entity* self, Entity* other);
void sine_movement(Entity* self);
#endif // !1
