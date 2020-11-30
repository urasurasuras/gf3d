#ifndef _GAME_H_
#define _GAME_H_

#include <SDL.h>            

#include "simple_logger.h"

#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_sprite.h"

#include "player.h"
#include "dinos.h"

#define window_W 1600
#define window_H 900

#define GRAVITATIONAL_ACCELERATION 8 // yea, it's 8, deal with it

typedef struct GameManager_S {
	Entity * player;
	Level * level;

	Uint32 lastUpdate;
	float deltaTime;
	Vector2D mousePos;
	int mx, my;
	int lastMx, lastMy;
}GameManager;

const Uint8* keys;


GameManager* gameManager();
//Level* level();
#endif // !1
