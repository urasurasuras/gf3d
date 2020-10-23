#include "player.h"
#include "simple_logger.h"
void player_think(Entity* self) {

	self->direction.x = cos(self->rotation.x);
	self->direction.y = sin(self->rotation.x);


	Vector2D velocity;

	// Move
	if (keys[SDL_SCANCODE_W]) {
		vector2d_copy(velocity, self->direction);
	}
	if (keys[SDL_SCANCODE_S]) {
		vector2d_copy(velocity, -self->direction);
	}
	if (keys[SDL_SCANCODE_A]) {
	}
	if (keys[SDL_SCANCODE_D]) {
		self->position.x -= 0.1;
	}

	//self->position += velocity;
	//self->position.z += 0.1;
	//self->position.x += 0.1;


	self->rotation.x = mx;
	self->rotation.z = my;

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