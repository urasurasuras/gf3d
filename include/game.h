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
//
//typedef struct Level_S {
//	Model* model;
//	Matrix4 modelMatrix;
//	Vector3D modelPosOffset;	// Model position offset
//	Vector3D modelRotOffset;	// Model rotation offset 
//
//}Level;

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

static GameManager game_manager = { 0 };
static Level* level = { 0 };

GameManager* gameManager();
//Level* level();
#endif // !1
