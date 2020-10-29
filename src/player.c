#include "player.h"
#include "simple_logger.h"
void player_think(Entity* self, float deltaTime) {	

	// Set rotaion
	//self->rotation.x = -gf3d_get_cam()->rotation.x;
	//self->rotation.z = my;
	// Set facing direction to rotation
	self->facingDirection.x = cos(self->rotation.y);
	self->facingDirection.y = sin(self->rotation.y);

	// Set velocity vector with speed
	self->velocity.x = 0;
	self->velocity.y = 0;

	Vector3D axisR = vector3d(0, 0, 0);
	vector3d_copy(axisR, self->facingDirection);
	axisR.x = self->facingDirection.y;
	axisR.y = -self->facingDirection.x;

	if (keys[SDL_SCANCODE_W]) {

		self->velocity.x += self->facingDirection.x;
		self->velocity.y += self->facingDirection.y;
	}
	else if (keys[SDL_SCANCODE_S]) {
		self->velocity.x -= self->facingDirection.x;
		self->velocity.y -= self->facingDirection.y;
	}

	if (keys[SDL_SCANCODE_A]) {

		self->velocity.x += axisR.x;
		self->velocity.y += axisR.y;
	}
	else if (keys[SDL_SCANCODE_D]) {

		self->velocity.x -= axisR.x;
		self->velocity.y -= axisR.y;
	}

	self->position.x += (self->velocity.x * self->speed * deltaTime);
	self->position.z += (self->velocity.y * self->speed * deltaTime);
}

void entity_move(Entity * self, float deltaTime) {

	// Set facing direction to rotation
	self->facingDirection.x = cos(self->rotation.y);
	self->facingDirection.y = sin(self->rotation.y);

	// Set velocity to facing dir
	self->velocity.x = -self->facingDirection.x;
	self->velocity.y = -self->facingDirection.y;

	// Increment position from velocity
	self->position.x += (self->velocity.x * deltaTime);
	self->position.z += (self->velocity.y * deltaTime);
}

void dino_think(Entity * self, float deltaTime) {
	self->velocity.x = 1;
	self->velocity.y = 1;

	Vector2D distance;
	distance.x = gameManager()->player->position.x + self->position.x;
	distance.y = gameManager()->player->position.z + self->position.z;

	vector2d_slog(distance);
	//self->rotation.y = -atan2(
	//	gameManager()->player->position.z + self->position.z, 
	//	) 
	//	/*+ M_PI*/;
	
	if (keys[SDL_SCANCODE_Q]) {
		self->rotation.y -= (deltaTime);
	}
	else if (keys[SDL_SCANCODE_E]) {
		self->rotation.y += (deltaTime);
	}
	//vector3d_sub(self->rotation, gameManager()->player->position, self->position);
	entity_move(self, deltaTime);
	//vector3d_slog(self->position);vector3d_slog(gameManager()->player->position);

}
