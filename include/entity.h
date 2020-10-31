#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "gfc_types.h"

#include "gf3d_model.h"
#include "gfc_matrix.h"

#define ent_PLAYER 1

typedef struct Entity_S {
	Uint8 _inuse;
	Uint8 type;
	TextWord name;

	// Model
	Model* model;
	Matrix4 modelMatrix;
	Vector3D modelPosOffset;	// Model position offset
	Vector3D modelRotOffset;	// Model rotation offset 

	// Physics
	Vector3D position;	// Position in 3D space
	Vector3D rotation;	// Euler rotation in radians
	Vector3D facingDirection;	// Facing direction vector
	Vector3D velocity;	// Moving direction
	float speed;		// Speed multiplier

	Uint32 collider_radius; // Radius of collider sphere
	Uint32 check_for_raycast;

	// Update
	void (*think)(struct Entity_S* self, float deltaTime);
	void (*touch)(struct Entity_S* self, struct Entity_S * other);
}Entity;

void entity_init(Uint32 max);
Entity* entity_new();
void entity_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer);
void entity_think_all(float deltaTime);
void entity_collision_check(Entity* entity);
#endif // !_ENTITY_H_
