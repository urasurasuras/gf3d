#include "player.h"
#include "simple_logger.h"
void player_think(Entity* self, float deltaTime) {	
	Character* character = (Character*)self->data;

	// Set facing direction to rotation
	character->facingDirection.x = cos(self->rotation.y) * cos(self->rotation.x);
	character->facingDirection.y = cos(self->rotation.y) * sin(self->rotation.x);
	character->facingDirection.z = sin(self->rotation.y);
	//slog("Rotation:");
	//vector3d_slog(self->rotation);
	//slog("Direction:");
	//vector3d_slog(character->facingDirection);

	// Set velocity vector with speed
	character->velocity.x = 0;
	character->velocity.z = 0;

	Vector3D axisR = vector3d(0, 0, 0);
	vector3d_copy(axisR, character->facingDirection);
	axisR.x = character->facingDirection.z;
	axisR.z = -character->facingDirection.x;

	if (keys[SDL_SCANCODE_W]) {
		// Set velocity to facing dir
		character->velocity.x = character->facingDirection.x;
		character->velocity.z = character->facingDirection.z;
	}
	else if (keys[SDL_SCANCODE_S]) {
		character->velocity.x -= character->facingDirection.x;
		character->velocity.z -= character->facingDirection.z;
	}

	if (keys[SDL_SCANCODE_A]) {

		character->velocity.x += axisR.x;
		character->velocity.z += axisR.z;
	}
	else if (keys[SDL_SCANCODE_D]) {

		character->velocity.x -= axisR.x;
		character->velocity.z -= axisR.z;
	}
	//vector3d_slog(character->facingDirection);

	entity_move(self, deltaTime);
}

void entity_move(Entity * self) {
	Character * character = self->data;

	// Increment position from velocity
	self->position.x += (character->velocity.x * character->speed * gameManager()->deltaTime);
	self->position.z += (character->velocity.z * character->speed * gameManager()->deltaTime);
	//vector3d_slog(self->position);

}

void dino_think(Entity * self, float deltaTime) {
	Character * character = (Character*)self->data;

	Vector2D distance;
	distance.x = gameManager()->player->position.x - self->position.x;
	distance.y = gameManager()->player->position.z - self->position.z;

	self->rotation.y = atan2(distance.y, distance.x);
		/*+ M_PI*/;
	
	if (keys[SDL_SCANCODE_Q]) {
		self->rotation.y -= (deltaTime);
	}
	else if (keys[SDL_SCANCODE_E]) {
		self->rotation.y += (deltaTime);
	}
	//self->rotation.y += (deltaTime);

	// Set facing direction to rotation
	character->facingDirection.x = cos(self->rotation.y);
	character->facingDirection.z = sin(self->rotation.y);

	// Set velocity to facing dir
	character->velocity.x = character->facingDirection.x;
	character->velocity.z = character->facingDirection.z;

	//vector3d_sub(self->rotation, gameManager()->player->position, self->position);
	entity_move(self);
	//self->position.z += 0.01;
	//vector3d_slog(self->position);vector3d_slog(gameManager()->player->position);

}

void entity_touch(Entity * self, Entity * other) {
	//slog("%s colliding with %s", self->name, other->name);
}