#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "gfc_types.h"

#include "gf3d_model.h"
#include "gfc_matrix.h"


typedef enum{
	ent_NULL,
	ent_LEVEL,
	ent_CHAR,
	ent_PROJECTILE
}EntType;

typedef enum{
	char_NULL,
	char_PLAYER,
	char_AI
}CharType;

/**
 Types of different entities
*/
typedef struct{
	float x;
	float y;
	float z;
	float w;// Width
	float h;// Height
	float d;// Depth
}RectPrism;

typedef struct{
	RectPrism bounds;
}Level;

typedef struct{
	CharType type;
	void* data;

	Uint32 check_for_raycast;
	float health;

	// Cooldowns
	int CLDN1;
	int last_CLDN1;
	int CLDN2;
	int last_CLDN2;
	int CLDN3;
	int last_CLDN3;

	float power;

	void (*primaryAttack)(struct Entity_S* self);

}Character;

typedef struct{
	Vector3D velocity;	// Moving direction
	float speed;		// Speed multiplier
	Uint32 collider_radius; // Radius of collider sphere
	float gravity_scale;
}Rigidbody;

typedef struct Entity_S {
	Uint8 _inuse;
	TextWord name;
	EntType type;
	void* data;

	// Model
	Model* model;
	Matrix4 modelMatrix;
	Vector3D modelPosOffset;	// Model position offset
	Vector3D modelRotOffset;	// Model rotation offset 

	// Physics
	Vector3D position;	// Position in 3D space
	Vector3D rotation;	// Euler rotation in radians
	Vector3D facingDirection;	// Facing direction vector, offset unit vector from position of ent

	Rigidbody rigidbody;

	// Update
	void (*think)(struct Entity_S* self, float deltaTime);
	void (*touch)(struct Entity_S* self, struct Entity_S* other);
	void (*touch_ground)(struct Entity_S* self);

}Entity;

typedef struct {
	Entity* entity_list;
	Uint32 entity_count;
}EntityManager;

typedef struct {
	Entity* target;
}MOB;

typedef struct {
	Entity* owner;
	float power;
	float time_to_live;
	float time_alive;

	void (*explode)(Entity* self);
}Projectile;

void entity_free(Entity* self);
void entity_init(Uint32 max);
Entity* entity_new();
void entity_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer);
void entity_think_all(float deltaTime);
/**
* @brief Check all other entites for collision
* @param entity Self
*/
void entity_collision_check(Entity* entity);
void entity_entity_collide(Entity* self, Entity* other);

void sphere_to_sphere_pushback(Entity* e1, Entity* e2);

EntityManager* get_entity_manager();
#endif // !_ENTITY_H_
