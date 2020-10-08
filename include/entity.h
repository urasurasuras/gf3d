#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "gfc_types.h"

#include "gf3d_model.h"
#include "gfc_matrix.h"

typedef struct Entity_S {
	Uint8 _inuse;
	Vector3D position;
	Model* model;
	Matrix4 modelMatrix;

	void (*think)(struct Entity_S* self);
}Entity;

void entity_init(Uint32 max);
Entity* entity_new();
void entity_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer);
void entity_think_all();
#endif // !_ENTITY_H_
