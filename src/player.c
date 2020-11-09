#include "player.h"
#include "simple_logger.h"

void player_hitscan_attack(Entity* self) {
	Character* character = (Character*)self->entData;
	character->check_for_raycast = 1;
	self->rotation.x += gfc_random() * .02;
	self->rotation.y += gfc_crandom() * .01;
	slog("PAP");
}
void player_projectile_attack(Entity* self) {

	projectile_beachball_spawn(self);
}
void player_projectile_arrow_attack(Entity* self) {

	projectile_arrow_spawn(self);
}
void player_think(Entity* self, float deltaTime) {	
	Character* character = (Character*)self->entData;

	// Set facing direction to rotation
	self->facingDirection.x = cos(self->rotation.y) /** cos(self->rotation.x)*/;
	self->facingDirection.y =/* cos(self->rotation.y) **/ sin(self->rotation.x);
	self->facingDirection.z = sin(self->rotation.y);

	// Set velocity vector with speed
	self->rigidbody.velocity.x = 0;
	//self->rigidbody.velocity.y = 0;
	self->rigidbody.velocity.z = 0;

	Vector3D axisR = vector3d(0, 0, 0);
	vector3d_copy(axisR, self->facingDirection);
	axisR.x = self->facingDirection.z;
	axisR.z = -self->facingDirection.x;

	if (keys[SDL_SCANCODE_W]) {
		// Set velocity to facing dir
		self->rigidbody.velocity.x = self->facingDirection.x;
		self->rigidbody.velocity.z = self->facingDirection.z;
	}
	else if (keys[SDL_SCANCODE_S]) {
		self->rigidbody.velocity.x -= self->facingDirection.x;
		self->rigidbody.velocity.z -= self->facingDirection.z;
	}

	if (keys[SDL_SCANCODE_A]) {

		self->rigidbody.velocity.x += axisR.x;
		self->rigidbody.velocity.z += axisR.z;
	}
	else if (keys[SDL_SCANCODE_D]) {

		self->rigidbody.velocity.x -= axisR.x;
		self->rigidbody.velocity.z -= axisR.z;
	}

	if (keys[SDL_SCANCODE_LSHIFT]) {
		self->rigidbody.speed = 30;
	}
	else {
		self->rigidbody.speed = 20;
	}

	if (character->last_CLDN1 + character->CLDN1 < SDL_GetTicks()) {
		if (keys[SDL_SCANCODE_SPACE]) {
			character->last_CLDN1 = SDL_GetTicks();
			self->position.y += 10;
			//self->rigidbody.velocity.y += 1;
		}
	}
	if (character->last_CLDN2 + character->CLDN2 < SDL_GetTicks()) {		
		if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {

			if (character->primaryAttack) {
				character->primaryAttack(self);
			}
			character->last_CLDN2 = SDL_GetTicks();
		}		
	}
	else
	{
		character->check_for_raycast = 0;
	}		

	if (character->last_CLDN3 + character->CLDN3 < SDL_GetTicks()) {
		//slog("%d", SDL_GetTicks());
		int swapped = 0;

		if (keys[SDL_SCANCODE_1]) {
			character->primaryAttack = player_hitscan_attack;
			character->CLDN2 = 100;
			slog("Weapon 1 selected");
			swapped = 1;
		}
		if (keys[SDL_SCANCODE_2]) {
			character->primaryAttack = player_projectile_attack;
			character->CLDN2 = 500;
			slog("Weapon 2 selected");
			swapped = 1;
		}
		if (keys[SDL_SCANCODE_3]) {
			character->primaryAttack = player_projectile_arrow_attack;
			character->CLDN2 = 1000;
			slog("Weapon 3 selected");
			swapped = 1;
		}
		if (keys[SDL_SCANCODE_4]) {
			character->primaryAttack = projectile_monkeybomb_spawn;
				character->CLDN2 = 10000;
			slog("Weapon 4 selected");
			swapped = 1;
		}
		if (keys[SDL_SCANCODE_5]) {
			swapped = 1;

		}

		if (swapped == 1)
			character->last_CLDN3 < SDL_GetTicks();
		//slog("%d", character->last_CLDN3);

	}
	self->rigidbody.speed *= character->speed_buff;
	entity_move(self);

	if (character->speed_buff < 1) {
		character->speed_buff = 1;
	}
	else {
		character->speed_buff -= deltaTime;
	}
}

void entity_move(Entity * self) {
	Character * character = self->entData;

	// Increment position from velocity
	self->position.x += (self->rigidbody.velocity.x * self->rigidbody.speed * gameManager()->deltaTime);
	self->position.y += (self->rigidbody.velocity.y * self->rigidbody.speed * gameManager()->deltaTime);
	self->position.z += (self->rigidbody.velocity.z * self->rigidbody.speed * gameManager()->deltaTime);

}

void dino_think(Entity * self, float deltaTime) {
	Character * character = (Character*)self->entData;

	Vector2D distance;

	MOB* mob_data = (MOB*)character->charData;
	Entity* target = mob_data->target;
	distance.x = target->position.x - self->position.x;
	distance.y = target->position.z - self->position.z;

	// Set velocity vector with speed
	self->rigidbody.velocity.x = 0;
	//self->rigidbody.velocity.y = 0;
	self->rigidbody.velocity.z = 0;

	self->rotation.y = atan2(distance.y, distance.x);
		/*+ M_PI*/;
		self->facingDirection.x = cos(self->rotation.y) /** cos(self->rotation.x)*/;
		self->facingDirection.z = sin(self->rotation.y);
		//vector3d_slog(self->facingDirection);

	self->rigidbody.velocity.x = self->facingDirection.x;
	self->rigidbody.velocity.z = self->facingDirection.z;
	//vector3d_sub(self->rotation, gameManager()->player->position, self->position);
	entity_move(self);
	//self->position.y -= 0.01;
	/*vector3d_slog(gameManager()->player->position);*/
	if (character->health < 0) {
		entity_free(self);
	}	

}

void entity_touch(Entity * self, Entity * other) {
	//slog("%s colliding with %s", self->name, other->name);
}
void dino_touch(Entity* self, Entity* other) {
	if (other->type == ent_CHAR) {
		Character* c = (Character*)other->entData;
		if (c->type == char_PLAYER) {
			c->health -= 0.1;
		}
	}
}

void pickup_health(Entity * self, Entity * other){

	Character* c = (Character*)other->entData;
	if (c->type == char_PLAYER) {
		c->health += 100;
	}
	slog("%s health now: %.2f", other->name, c->health);

	entity_free(self);
}
void pickup_speed(Entity * self, Entity * other){

	Character* c = (Character*)other->entData;
	if (c->type == char_PLAYER) {
		c->speed_buff = 5;
	}
	slog("%s gave speed buff: x%.2f", other->name, c->speed_buff);
	entity_free(self);

}
void pickup_damage(Entity * self, Entity * other){
	
	for (Uint32 i = 0; i < get_entity_manager()->entity_count; i++) {
		Entity* other = &get_entity_manager()->entity_list[i];
		Character* otherChar = (Character*)other->entData;

		if (!other->_inuse)continue;
		if (!otherChar)continue;

		switch (other->type)
		{
		case ent_CHAR:
			if (otherChar->type == char_PLAYER)break;
			// Do damage
			otherChar->health = 1;
			break;
		default:
			break;
		}
	}
	entity_free(self);
}

void sine_movement(Entity* self) {

	self->position.y = (sin(SDL_GetTicks() * 0.01));
	self->position.y += self->rigidbody.collider_radius;

	//slog("%f", self->position.y);
}
