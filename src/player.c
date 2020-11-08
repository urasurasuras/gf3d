#include "player.h"
#include "simple_logger.h"

void player_weapon_swap(Character * c) {


}
void player_hitscan_attack(Entity* self) {
	Character* character = (Character*)self->data;
	character->check_for_raycast = 1;
	self->rotation.x += .01;
	self->rotation.y += gfc_crandom() * .01;
	slog("PAP");
}
void player_projectile_attack(Entity* self) {

	projectile_spawn(self);
}
void player_think(Entity* self, float deltaTime) {	
	Character* character = (Character*)self->data;

	// Set facing direction to rotation
	self->facingDirection.x = cos(self->rotation.y) /** cos(self->rotation.x)*/;
	self->facingDirection.y = cos(self->rotation.y) * sin(self->rotation.x);
	self->facingDirection.z = sin(self->rotation.y);
	//slog("Rotation:");
	//vector3d_slog(self->rotation);
	//slog("Direction:");
	//vector3d_slog(character->facingDirection);

	// Set velocity vector with speed
	self->rigidbody.velocity.x = 0;
	self->rigidbody.velocity.y = 0;
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
			self->position.y = 10;
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
			swapped = 1;
		}
		if (keys[SDL_SCANCODE_4]) {
			swapped = 1;
		}
		if (keys[SDL_SCANCODE_5]) {
			swapped = 1;

		}

		if (swapped == 1)
			character->last_CLDN3 < SDL_GetTicks();
		//slog("%d", character->last_CLDN3);

	}
	entity_move(self);

}

void entity_move(Entity * self) {
	Character * character = self->data;

	// Increment position from velocity
	self->position.x += (self->rigidbody.velocity.x * self->rigidbody.speed * gameManager()->deltaTime);
	self->position.z += (self->rigidbody.velocity.z * self->rigidbody.speed * gameManager()->deltaTime);

}

void dino_think(Entity * self, float deltaTime) {
	Character * character = (Character*)self->data;

	Vector2D distance;
	distance.x = gameManager()->player->position.x - self->position.x;
	distance.y = gameManager()->player->position.z - self->position.z;


	// Set velocity vector with speed
	self->rigidbody.velocity.x = 0;
	self->rigidbody.velocity.y = 0;
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
	//self->position.z += 0.01;
	//vector3d_slog(self->position);vector3d_slog(gameManager()->player->position);
	if (character->health < 0) {
		entity_free(self);
	}	

}

void entity_touch(Entity * self, Entity * other) {
	//slog("%s colliding with %s", self->name, other->name);
}
void dino_touch(Entity* self, Entity* other) {
	if (other->type == ent_CHAR) {
		Character* c = (Character*)other->data;
		if (c->type == char_PLAYER) {
			c->health -= 0.1;
		}
	}
}

