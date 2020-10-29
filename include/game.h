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

#include "player.h"

#define window_W 1280
#define window_H 720

typedef struct GameManager_S {
	Entity * player;

	Uint32 lastUpdate;
	float deltaTime;
	Vector2D mousePos;
	int mx, my;
	int lastMx, lastMy;
}GameManager;

const Uint8* keys;

static GameManager game_manager = { 0 };

GameManager* gameManager();
#endif // !1
