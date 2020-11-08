#include "projectile.h"

Entity * projectile_spawn(Entity* owner){

	Entity* self = entity_new();
	if (!self) { return; }

	Vector3D rayScale;
	Vector3D rayEnd;

	Character* ownerChar = (Character*)owner->data;

	vector3d_scale(rayScale, owner->facingDirection, owner->rigidbody.collider_radius);
	vector3d_add(rayEnd, owner->position, rayScale);

	vector3d_copy(self->position, rayEnd);
	self->type = ent_PROJECTILE;
	self->model = gf3d_model_load("sphere");
	gfc_word_cpy(self->name, "Projectile");

	vector3d_copy(self->rigidbody.velocity, owner->facingDirection);
	self->rigidbody.speed = 50;
	self->think = entity_move;
	
	Projectile * p = malloc(sizeof(Projectile));
	p->owner = owner;
	p->power = ownerChar->power * 50;
	self->data = p;

	self->touch = projectile_do_damage;
	
	return self;
}

Entity * projectile_arrow_spawn(Entity* owner){

	Entity* self = entity_new();
	if (!self) { return; }

	Vector3D rayScale;
	Vector3D rayEnd;

	Character* ownerChar = (Character*)owner->data;

	vector3d_scale(rayScale, owner->facingDirection, owner->rigidbody.collider_radius);
	vector3d_add(rayEnd, owner->position, rayScale);

	vector3d_copy(self->position, rayEnd);
	self->type = ent_PROJECTILE;
	self->model = gf3d_model_load("arrow");
	gfc_word_cpy(self->name, "Arrow");

	vector3d_copy(self->rigidbody.velocity, owner->facingDirection);
	self->rigidbody.speed = 200;
	self->think = entity_move;
	
	Projectile * p = malloc(sizeof(Projectile));
	p->owner = owner;
	p->power = ownerChar->power * 200;
	self->data = p;

	self->touch = projectile_do_damage;
	
	return self;
}

void projectile_do_damage(Entity* self, Entity* other) {
	Projectile * p = (Projectile*)self->data;
	Character * otherChar = (Character*)other->data;

	otherChar->health -= p->power;
	
	entity_free(self);
}