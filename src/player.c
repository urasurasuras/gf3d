#include "player.h"
#include "simple_logger.h"
void player_think(Entity* self) {
	
	// Set rotaion
	self->rotation.x = -gf3d_get_cam()->rotation.x;
	//self->rotation.z = my;
	// Set facing direction to rotation
	self->facingDirection.x = cos(self->rotation.x);
	self->facingDirection.y = sin(self->rotation.x);
	//out->y = sin(radians);
	//vector3d_set_angle_by_radians(&self->facingDirection, self->rotation.x);
	//slog("vX DIR: %.2f", self->facingDirection.x);

	// Set velocity vector with speed
	if (keys[SDL_SCANCODE_W]) {
		
		self->velocity.x = -self->facingDirection.x;
		self->velocity.y = -self->facingDirection.y;
		vector2d_slog(self->velocity);
	}
	else if (keys[SDL_SCANCODE_S]) {
		self->velocity.x = self->facingDirection.x;
		self->velocity.y = self->facingDirection.y;
	}
	else {
		self->velocity.x = 0;
		self->velocity.y = 0;
	}
	/*	
	if (keys[SDL_SCANCODE_A]) {
		self->velocity.x = self->facingDirection.x;
		self->velocity.y = self->facingDirection.y;
	}
	else if (keys[SDL_SCANCODE_D]) {
		self->velocity.x = -self->facingDirection.x;
		self->velocity.y = -self->facingDirection.y;
	}
	else {
		self->velocity.x = 0;
		self->velocity.y = 0;
	}*/

	//slog("v: %.2f.%.2f", self->velocity.x, self->velocity.y);

	//velocity.x
	self->position.z += (self->velocity.x * self->speed);
	self->position.x += (self->velocity.y * self->speed);

	//self->position.z += self->facingDirection.x * -0.01;
	//self->position.x += self->facingDirection.y * -0.01;

	//self->position.z += velocity.y;

	//vector3d_copy(self->rotation, )
	
	//gf3d_camera_look_at(vector3d(0, 0, 0), self->position, vector3d(0, 0, 1));
	//gf3d_camera_set_position(self->position);
	//// Rotate
	//if (keys[SDL_SCANCODE_Q]) {
	//	self->position.x += 0.1;
	//}
	//if (keys[SDL_SCANCODE_E]) {
	//	self->position.x -= 0.1;
	//}
}