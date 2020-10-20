#include "player.h"
#include "simple_logger.h"
void player_think(Entity* self) {

	// Move
	if (keys[SDL_SCANCODE_W]) {
		self->position.y += 0.1;
	}
	if (keys[SDL_SCANCODE_S]) {
		self->position.y -= 0.1;
	}
	if (keys[SDL_SCANCODE_A]) {
		self->position.x += 0.1;
	}
	if (keys[SDL_SCANCODE_D]) {
		self->position.x -= 0.1;
	}
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