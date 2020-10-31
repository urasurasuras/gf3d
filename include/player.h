#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "game.h"
#include "entity.h"

void player_think(Entity* self, float deltaTime);
void dino_think(Entity* self, float deltaTime);

void entity_move(Entity* self);
void entity_touch(Entity* self, Entity* other);

#endif // !1
