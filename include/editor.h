#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "entity.h"

Entity * editor_spawn_entity(Vector3D pos, Vector3D rotation, TextWord name);
void editor_think(Entity* self);

#endif