#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "game.h"
#include "entity.h"
Entity* projectile_spawn(Entity* owner);
Entity * projectile_beachball_spawn(Entity * owner);
Entity* projectile_monkeybomb_spawn(Entity* owner);
Entity* projectile_arrow_spawn(Entity* owner);
void projectile_do_damage(Entity* self, Entity* other);
void projectile_free_onGround(Entity* self);
void projectile_stay_onGround(Entity* self);
void projectile_stay_onGround_monkeyBomb(Entity* self);
void projectile_explode(Entity* self);
#endif // !1
