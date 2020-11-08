#include "projectile.h"

Entity * projectile_beachball_spawn(Entity* owner){

	Entity* self = entity_new();
	if (!self) { return; }

	Vector3D rayScale;
	Vector3D rayEnd;

	Character* ownerChar = (Character*)owner->data;

	vector3d_scale(rayScale, owner->facingDirection, owner->rigidbody.collider_radius);
	vector3d_add(rayEnd, owner->position, rayScale);

	vector3d_copy(self->position, rayEnd);
	self->type = ent_PROJECTILE;
	self->model = gf3d_model_load("beachball");
	gfc_word_cpy(self->name, "Beachball");

	vector3d_copy(self->rigidbody.velocity, owner->facingDirection);
	self->rigidbody.speed = 100;
	self->rigidbody.collider_radius = 1;
	self->rigidbody.gravity_scale = 1;
	self->think = entity_move;
	self->touch_ground = projectile_free_onGround;
	
	Projectile * p = malloc(sizeof(Projectile));
	memset(p, 0, sizeof(Projectile));
	p->owner = owner;
	p->power = ownerChar->power * 50;
	p->time_to_live = 5;
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
	self->rigidbody.collider_radius = .1;
	self->touch_ground = projectile_stay_onGround;
	self->think = entity_move;
	
	Projectile * p = malloc(sizeof(Projectile));
	memset(p, 0, sizeof(Projectile));
	p->owner = owner;
	p->power = ownerChar->power * 200;
	p->time_to_live = 5;
	self->data = p;

	self->touch = projectile_do_damage;

	return self;
}

void projectile_do_damage(Entity* self, Entity* other) {
	Projectile * p = (Projectile*)self->data;
	if (p->owner == other)return;
	Character * otherChar = (Character*)other->data;

	otherChar->health -= p->power;
	
	entity_free(self);
}

void projectile_free_onGround(Entity* self) {
	slog("%s toched the grond", self->name);
	entity_free(self);
	//vector3d_clear(self->rigidbody.velocity);
	//self->rigidbody.gravity_scale = -self->rigidbody.gravity_scale;
}
void projectile_stay_onGround(Entity* self) {
	slog("%s toched the grond", self->name);
	Projectile* p = (Projectile*)self->data;

	p->time_alive += gameManager()->deltaTime;
	//slog("%f", p->time_alive);
	if (p->time_alive > p->time_to_live) {
		entity_free(self);
		return;
	}
	//entity_free(self);
	vector3d_clear(self->rigidbody.velocity);
	self->rigidbody.gravity_scale = -self->rigidbody.gravity_scale;
}