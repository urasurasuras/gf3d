#include "player.h"
#include "simple_logger.h"
void player_think(Entity* self, float deltaTime) {	

	// Set facing direction to rotation
	self->facingDirection.x = cos(self->rotation.y);
	self->facingDirection.z = sin(self->rotation.y);
	//vector3d_slog(self->facingDirection);

	// Set velocity vector with speed
	self->velocity.x = 0;
	self->velocity.z = 0;

	Vector3D axisR = vector3d(0, 0, 0);
	vector3d_copy(axisR, self->facingDirection);
	axisR.x = self->facingDirection.z;
	axisR.z = -self->facingDirection.x;

	if (keys[SDL_SCANCODE_W]) {
		// Set velocity to facing dir
		self->velocity.x = self->facingDirection.x;
		self->velocity.z = self->facingDirection.z;
	}
	else if (keys[SDL_SCANCODE_S]) {
		self->velocity.x -= self->facingDirection.x;
		self->velocity.z -= self->facingDirection.z;
	}

	if (keys[SDL_SCANCODE_A]) {

		self->velocity.x += axisR.x;
		self->velocity.z += axisR.z;
	}
	else if (keys[SDL_SCANCODE_D]) {

		self->velocity.x -= axisR.x;
		self->velocity.z -= axisR.z;
	}
	vector3d_slog(self->facingDirection);

	entity_move(self, deltaTime);
}

void entity_raycast();
void entity_move(Entity * self) {


	// Increment position from velocity
	self->position.x += (self->velocity.x * self->speed * gameManager()->deltaTime);
	self->position.z += (self->velocity.z * self->speed * gameManager()->deltaTime);
	//vector3d_slog(self->position);

}

void dino_think(Entity * self, float deltaTime) {


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
	self->facingDirection.x = cos(self->rotation.y);
	self->facingDirection.z = sin(self->rotation.y);	

	// Set velocity to facing dir
	self->velocity.x = self->facingDirection.x;
	self->velocity.z = self->facingDirection.z;

	//vector3d_sub(self->rotation, gameManager()->player->position, self->position);
	entity_move(self);
	//self->position.z += 0.01;
	//vector3d_slog(self->position);vector3d_slog(gameManager()->player->position);

}

void entity_touch(Entity * self, Entity * other) {
	//slog("%s colliding with %s", self->name, other->name);
}