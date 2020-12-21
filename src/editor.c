#include "entity.h"
#include "player.h"

Entity * editor_spawn_entity(Vector3D pos, Vector3D rotation, TextWord name){

    Entity * self = NULL;
    self = entity_new();

    if (!self){
        return;
    }

    gfc_word_cpy(self->name, name);
    self->type = ent_EDITOR;
    self->model = gf3d_model_load(name);
    vector3d_copy(self->position, pos);
    self->rotation.z = rotation.y;

    return self;
}
void editor_think(Entity* self){
	player_move(self);

	if(gfc_input_key_pressed("1")){
		editor_spawn_entity(self->position, self->rotation, "wall");
	}
	if(gfc_input_key_pressed("2")){
		editor_spawn_entity(self->position, self->rotation, "pillar");
	}
	if(gfc_input_key_pressed("3")){
		slog("qweqweqweq");
	}
	if(gfc_input_key_pressed("4")){
		slog("QWEQWEQWE");
	}
	if(gfc_input_key_pressed("5")){
		slog("five");
	}
}