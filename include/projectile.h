#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "game.h"
#include "entity.h"

Entity * projectile_spawn(Entity * owner);
void projectile_do_damage(Entity* self, Entity* other);
#endif // !1
