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
	//if (!(keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_A])) {
		self->velocity.x = 0;
		self->velocity.y = 0;
	//}
	//else {
		Vector3D axisR = vector3d(0, 0, 0);
		vector3d_copy(axisR, self->facingDirection);
		axisR.x = self->facingDirection.y;
		axisR.y = -self->facingDirection.x;
		vector3d_slog(axisR);

		if (keys[SDL_SCANCODE_W]) {

			self->velocity.x += -self->facingDirection.x;
			self->velocity.y += -self->facingDirection.y;
		}
		else if (keys[SDL_SCANCODE_S]) {
			self->velocity.x += self->facingDirection.x;
			self->velocity.y += self->facingDirection.y;
		}

		if (keys[SDL_SCANCODE_A]) {

			self->velocity.x += axisR.x;
			self->velocity.y += axisR.y;
		}
		else if (keys[SDL_SCANCODE_D]) {

			self->velocity.x += -axisR.x;
			self->velocity.y += -axisR.y;
		}
		vector2d_slog(self->velocity);
	//}

	//slog("v: %.2f.%.2f", self->velocity.x, self->velocity.y);

	//velocity.x
	self->position.z += (self->velocity.x * self->speed);
	self->position.x += (self->velocity.y * self->speed);

}