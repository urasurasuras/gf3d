#include "player.h"
#include "simple_logger.h"
void player_think(Entity* self, float deltaTime) {	

	// Set rotaion
	//self->rotation.x = -gf3d_get_cam()->rotation.x;
	//self->rotation.z = my;
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
	//self->position.y += 0.01;
	//self->position.x += 0.01;

	entity_move(self, deltaTime);
}

void entity_move(Entity * self, float deltaTime) {


	// Increment position from velocity
	self->position.x += (self->velocity.x * deltaTime);
	self->position.z += (self->velocity.z * deltaTime);
	//vector3d_slog(self->position);

}

void dino_think(Entity * self, float deltaTime) {


	Vector2D distance;
	distance.x = gameManager()->player->position.x + self->position.x;
	distance.y = gameManager()->player->position.z + self->position.z;

	//vector2d_slog(distance);
	//self->rotation.y = -atan2(
	//	gameManager()->player->position.z + self->position.z, 
	//	) 
	//	/*+ M_PI*/;
	
	if (keys[SDL_SCANCODE_Q]) {
		self->rotation.x -= (deltaTime);
	}
	else if (keys[SDL_SCANCODE_E]) {
		self->rotation.x += (deltaTime);
	}
	//self->rotation.y += (deltaTime);

	// Set facing direction to rotation
	self->facingDirection.x = cos(self->rotation.y);
	self->facingDirection.z = sin(self->rotation.y);	

	// Set velocity to facing dir
	self->velocity.x = self->facingDirection.x;
	self->velocity.z = self->facingDirection.z;

	//vector3d_sub(self->rotation, gameManager()->player->position, self->position);
	entity_move(self, deltaTime);
	//self->position.z += 0.01;
	//vector3d_slog(self->position);vector3d_slog(gameManager()->player->position);

}
