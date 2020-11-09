#include "projectile.h"
#include "collision.h"

Entity* projectile_spawn(Entity* owner) {
	Entity* self = entity_new();
	if (!self) { return; }

	Vector3D rayScale;
	Vector3D rayEnd;

	vector3d_scale(rayScale, owner->facingDirection, owner->rigidbody.collider_radius);
	vector3d_add(rayEnd, owner->position, rayScale);
	vector3d_copy(self->position, rayEnd);
	vector3d_copy(self->rigidbody.velocity, owner->facingDirection);

	self->type = ent_PROJECTILE;

	Projectile* p = malloc(sizeof(Projectile));
	memset(p, 0, sizeof(Projectile));
	p->owner = owner;

	self->data = p;

	return self;
}

Entity * projectile_beachball_spawn(Entity* owner){

	Entity* self = projectile_spawn(owner);
	if (!self) { return; }

	Character* ownerChar = (Character*)owner->data;

	self->model = gf3d_model_load("beachball");
	gfc_word_cpy(self->name, "Beachball");

	self->rigidbody.speed = 100;
	self->rigidbody.collider_radius = 1;
	self->rigidbody.gravity_scale = 1;
	self->think = entity_move;
	self->touch_ground = projectile_free_onGround;
	
	Projectile* p = (Projectile*)self->data;
	p->power = ownerChar->power * 50;
	p->time_to_live = 5;

	self->touch = projectile_do_damage;
	
	return self;
}

Entity * projectile_monkeybomb_spawn(Entity* owner){

	Entity* self = projectile_spawn(owner);
	if (!self) { return; }

	Character* ownerChar = (Character*)owner->data;

	self->model = gf3d_model_load("beachball");
	gfc_word_cpy(self->name, "Monkey Bomb");

	self->rigidbody.speed = 100;
	self->rigidbody.collider_radius = 1;
	self->rigidbody.gravity_scale = 1;
	self->think = entity_move;
	self->touch_ground = projectile_stay_onGround;
	
	Projectile* p = (Projectile*)self->data;
	p->power = ownerChar->power * 50;
	p->time_to_live = 5;
	p->explode = projectile_explode;

	//self->touch = projectile_do_damage;



	for (Uint32 i = 0; i < get_entity_manager()->entity_count; i++) {
		Entity* other = &get_entity_manager()->entity_list[i];
		Character* otherChar = (Character*)other->data;
		MOB* otherMOB;

		if (!other->_inuse)continue;
		if (!otherChar)continue;

		switch (other->type)
		{
		case ent_CHAR:
			otherMOB = (MOB*)otherChar->data;
			if (!otherMOB)break;
			if (otherChar->type == char_PLAYER)break;

			otherMOB->target = self;
			break;
		default:
			break;
		}
	}

	return self;
}

Entity * projectile_arrow_spawn(Entity* owner){

	Entity* self = projectile_spawn(owner);
	if (!self) { return; }

	Character* ownerChar = (Character*)owner->data;

	self->model = gf3d_model_load("arrow");
	gfc_word_cpy(self->name, "Arrow");

	self->rigidbody.speed = 200;
	self->rigidbody.collider_radius = .1;
	self->touch_ground = projectile_stay_onGround;
	self->think = entity_move;
	
	Projectile* p = (Projectile*)self->data;
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
		if (p->explode)
			p->explode(self);
		entity_free(self);
		return;
	}
	//entity_free(self);
	vector3d_clear(self->rigidbody.velocity);
	self->rigidbody.gravity_scale = 0;
}
void projectile_stay_onGround_monkeyBomb(Entity* self) {
	
}

void projectile_explode(Entity* self) {

	for (Uint32 i = 0; i < get_entity_manager()->entity_count; i++) {
		Entity* other = &get_entity_manager()->entity_list[i];
		Character* otherChar = (Character*)other->data;

		if (!other->_inuse)continue;
		if (!otherChar)continue;

		switch (other->type)
		{
		case ent_CHAR:
			if (otherChar->type == char_PLAYER)break;
			// Do damage
			if (collide_sphere(self->position, 500, other->position, other->rigidbody.collider_radius))	
				otherChar->health -= 100;

			break;
		default:
			break;
		}
	}
	
	// After explosion, set the targets back
	for (Uint32 i = 0; i < get_entity_manager()->entity_count; i++) {
		Entity* other = &get_entity_manager()->entity_list[i];
		Character* otherChar = (Character*)other->data;
		MOB* otherMOB;

		if (!other->_inuse)continue;
		if (!otherChar)continue;

		switch (other->type)
		{
		case ent_CHAR:
			otherMOB = (MOB*)otherChar->data;
			if (!otherMOB)break;
			if (otherChar->type == char_PLAYER)break;

			otherMOB->target = gameManager()->player;
			break;
		default:
			break;
		}
	}

}