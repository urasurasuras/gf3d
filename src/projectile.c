#include "projectile.h"
#include "collision.h"

Entity* projectile_spawn(Entity* owner) {

	Projectile* p = NULL;
	
	Entity* self = entity_new();

	if (!owner){
		slog("NULL owner provided");
		return NULL;
	}	

	if (!self) { 
		
		slog("Returned NULL projectile");
		return NULL; 
	}

	p = malloc(sizeof(Projectile));
	if (!p){
		slog("Could not allocate memory for projectile %s owned by %s", self->name, owner->name );
		return NULL;
	}
	memset(p, 0, sizeof(Projectile));
	p->owner = owner;

	Vector3D rayScale;
	Vector3D rayEnd;

	vector3d_scale(rayScale, owner->facingDirection, owner->rigidbody.collider_radius);
	vector3d_add(rayEnd, owner->position, rayScale);
	vector3d_copy(self->position, rayEnd);
	vector3d_copy(self->rigidbody.velocity, owner->facingDirection);
	vector3d_copy(self->rotation, owner->rotation);

	self->type = ent_PROJECTILE;

	
	self->entData = p;

	return self;
}

void projectile_beachball_spawn(Entity* owner){

	Character* owner_char = NULL;
	Projectile* p = NULL;

	if (!owner){
		slog("Owner entity is NULL");
		return;
	}
	Entity* self = projectile_spawn(owner);
	if (!self) { return; }

	owner_char = (Character*)owner->entData;
	if (!owner_char){
		slog("Owner char is NULL");
		return;
	}
	
	p = (Projectile*)self->entData;
	if (!p){
		slog("Projectile of %s is NULL");
		return;
	}

	self->model = gf3d_model_load("beachball");
	gfc_word_cpy(self->name, "Beachball");

	self->rigidbody.speed = 100;
	self->rigidbody.collider_radius = 1;
	self->rigidbody.gravity_scale = 1;
	self->think = entity_move;
	self->touch_ground = projectile_free_onGround;
	self->touch = projectile_do_damage;

	p->power = owner_char->power * 50;
	p->time_to_live = 5;

}

void projectile_monkeybomb_spawn(Entity* owner){

	Character* owner_char = NULL;
	Projectile* p = NULL;
	Entity* other = NULL;
	Character* other_char = NULL;
	MOB* other_MOB = NULL;

	if (!owner){
		slog("Owner entity is NULL");
		return;
	}
	Entity* self = projectile_spawn(owner);
	if (!self) { return; }

	owner_char = (Character*)owner->entData;
	if (!owner_char){
		slog("Owner char is NULL");
		return;
	}
	
	p = (Projectile*)self->entData;
	if (!p){
		slog("Projectile of %s is NULL");
		return;
	}


	self->model = gf3d_model_load("beachball");
	gfc_word_cpy(self->name, "Monkey Bomb");

	self->rigidbody.speed = 100;
	self->rigidbody.collider_radius = 1;
	self->rigidbody.gravity_scale = 1;
	self->think = entity_move;
	self->touch_ground = projectile_stay_onGround;
	
	p->power = owner_char->power * 50;
	p->time_to_live = 5;
	p->explode = projectile_explode;

	//self->touch = projectile_do_damage;

	for (Uint32 i = 0; i < get_entity_manager()->entity_count; i++) {
		other = &get_entity_manager()->entity_list[i];

		if (!other){
			slog("NULL other");
			continue;
		}
		if (!other->_inuse){
			continue;
		}
		if (other->type != ent_CHAR){
			continue;
		}

		other_char = (Character*)other->entData;
		if (!other_char){
			slog("Char data of %s is NULL",other->name);
			continue;
		}

		switch (other->type)
		{
		case ent_CHAR:
			if (other_char->type != char_AI){break;}

			other_MOB = (MOB*)other_char->charData;
			if (!other_MOB){
				slog("NULL other MOB");
				break;
			}			

			other_MOB->target = self;

			break;
		default:
			break;
		}
	}

	//return self;
}

void projectile_arrow_spawn(Entity* owner){
	
	Character* owner_char = NULL;
	Projectile* p = NULL;

	if (!owner){
		slog("NULL owner");
		return;
	}

	Entity* self = projectile_spawn(owner);
	if (!self) { return; }

	owner_char = (Character*)owner->entData;
	if (!owner_char){
		slog("Owner char is NULL");
		return;
	}	
	
	p = (Projectile*)self->entData;
	if (!p){
		slog("Projectile of %s is NULL");
		return;
	}

	self->model = gf3d_model_load("arrow");
	gfc_word_cpy(self->name, "Arrow");

	self->rigidbody.speed = 200;
	self->rigidbody.collider_radius = .1;
	self->touch_ground = projectile_stay_onGround;
	self->think = entity_move;
	
	p->power = owner_char->power * 200;
	p->time_to_live = 5;
	self->entData = p;

	self->touch = projectile_do_damage;

}

void projectile_water_spawn(Entity* owner){
		
	Character* owner_char = NULL;
	Projectile* p = NULL;

	if (!owner){
		slog("NULL owner");
		return;
	}

	Entity* self = projectile_spawn(owner);
	if (!self) { return; }

	owner_char = (Character*)owner->entData;
	if (!owner_char){
		slog("Owner char is NULL");
		return;
	}	
	
	p = (Projectile*)self->entData;
	if (!p){
		slog("Projectile of %s is NULL");
		return;
	}

	self->model = gf3d_model_load("water");
	gfc_word_cpy(self->name, "Water");

	self->rigidbody.speed = 50;
	self->rigidbody.collider_radius = .1;
	self->touch_ground = projectile_free_onGround;
	self->think = entity_move;
	
	p->power = owner_char->power * 25;
	p->time_to_live = 5;
	self->entData = p;

	self->touch = projectile_do_damage;

}

void projectile_do_damage(Entity* self, Entity* other) {

	Projectile * p = NULL;
	Character * other_char = NULL;

	if (!other){
		slog("NULL other provided");
		return;
	}	
	if (!self) { 		
		slog(" NULL self");
		return; 
	}

	p = (Projectile*)self->entData;
	if (p->owner == other)return;
	other_char = (Character*)other->entData;
	if (!other_char)return;

	other_char->health -= p->power;
	
	entity_free(self);
}

void projectile_free_onGround(Entity* self) {
	if (!self) { 		
		slog(" NULL self");
		return; 
	}
	slog("%s toched the grond", self->name);
	entity_free(self);
	//vector3d_clear(self->rigidbody.velocity);
	//self->rigidbody.gravity_scale = -self->rigidbody.gravity_scale;
}
void projectile_stay_onGround(Entity* self) {
	Projectile* p = NULL;
	
	if (!self) { 		
		slog(" NULL self");
		return; 
	}
	slog("%s toched the grond", self->name);
	p = (Projectile*)self->entData;
	if (!p){
		slog("No projectile data");
		return;
	}

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

	Entity* other = NULL;
	Character* other_char = NULL;
	MOB* other_MOB = NULL;

	if (!self){
		slog("NULL self");
		return;
	}

	for (Uint32 i = 0; i < get_entity_manager()->entity_count; i++) {
		other = &get_entity_manager()->entity_list[i];
		if (!other){
			slog("NULL other");
			continue;
		}
		if (!other->_inuse){
			continue;
		}

		other_char = (Character*)other->entData;
		if(!other_char){
			slog("NULL otherchar");
			continue;
		}

		switch (other->type)
		{
		case ent_CHAR:
			if (other_char->type == char_PLAYER)break;
			// Do damage
			if (collide_sphere(self->position, 500, other->position, other->rigidbody.collider_radius))	
				other_char->health -= 100;

			break;
		default:
			break;
		}
	}
	
	// After explosion, set the targets back
	for (Uint32 i = 0; i < get_entity_manager()->entity_count; i++) {
		other = &get_entity_manager()->entity_list[i];
		if (!other){
			slog("NULL other");
			continue;
		}
		if (!other->_inuse){
			continue;
		}

		other_char = (Character*)other->entData;
		if(!other_char){
			slog("NULL otherchar");
			continue;
		}
		switch (other->type)
		{
		case ent_CHAR:
			other_MOB = (MOB*)other_char->charData;
			if (!other_MOB)break;
			if (other_char->type == char_PLAYER)break;

			other_MOB->target = gameManager()->player;
			break;
		default:
			break;
		}
	}

}