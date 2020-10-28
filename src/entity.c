#include "simple_logger.h"
#include "entity.h"

typedef struct {
	Entity* entity_list;
	Uint32 entity_count;

}EntityManager;

static EntityManager entity_manager = { 0 };

void entity_free(Entity* self) {
	if (!self)return;
	//slog("freed ent: %s", self->name);
	//gf2d_sprite_free(self->sprite);
	memset(self, 0, sizeof(Entity));
}
void entity_free_all(){
	
	for (int i = 0; i < entity_manager.entity_count; i++) {
		if (entity_manager.entity_list[i]._inuse) {
			entity_free(&entity_manager.entity_list[i]);
		}
	}
}
EntityManager entity_manager_close() {
	entity_free_all();
	entity_manager.entity_count = 0;
	free(entity_manager.entity_list);
	entity_manager.entity_list = NULL;
	slog("entity manager closed");
}
void entity_init(Uint32 max) {
	if (entity_manager.entity_list != NULL) {
		slog("Ent manager already initialized");
		return;//TODO: return ent manager
	}

	entity_manager.entity_list = gfc_allocate_array(sizeof(Entity), max);

	if (!entity_manager.entity_list) {
		slog("bruh moment for ent list"); 
		return;
	}

	entity_manager.entity_count = max;
}
Entity* entity_new() {
	
	for (int i = 0; i < entity_manager.entity_count; i++) {
		if (!entity_manager.entity_list[i]._inuse) {
			// Mark _inuse
			entity_manager.entity_list[i]._inuse = 1;

			gfc_matrix_identity(entity_manager.entity_list[i].modelMatrix);// Init matrix
			entity_manager.entity_list[i].position = vector3d(0, 0, 0);// Init pos
			entity_manager.entity_list[i].rotation = vector3d(0, 0, 0);// Init rotation
			entity_manager.entity_list[i].speed = 10;// Init speed
			// Set facing direction to rotation
			entity_manager.entity_list[i].facingDirection.x = cos(entity_manager.entity_list[i].rotation.x);
			entity_manager.entity_list[i].facingDirection.y = sin(entity_manager.entity_list[i].rotation.x);
			return &entity_manager.entity_list[i];
		}
	}
}

void entity_draw(Entity* self, Uint32 bufferFrame, VkCommandBuffer commandBuffer) {
	if (!self)return;
	if (self->type == ent_PLAYER)return;

	Vector3D drawpos;

	vector3d_add(drawpos, self->position, self->modelPosOffset);
	gfc_matrix_make_translation(
		&self->modelMatrix,
		drawpos
	);

	setRotation(self->modelMatrix, self->rotation);

	gf3d_model_draw(self->model, bufferFrame, commandBuffer, self->modelMatrix);
}

void entity_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer) {
	
	for (int i = 0; i < entity_manager.entity_count; i++) {
		if (entity_manager.entity_list[i]._inuse) {
			entity_draw(&entity_manager.entity_list[i], bufferFrame, commandBuffer);
		}
	}
}

void entity_think_all(float deltaTime) {
	
	for (int i = 0; i < entity_manager.entity_count; i++) {
		if (entity_manager.entity_list[i]._inuse && entity_manager.entity_list[i].think) {
			entity_manager.entity_list[i].think(&entity_manager.entity_list[i], deltaTime);
		}
	}
}
EntityManager *get_entity_manager(){
	return &entity_manager;
}