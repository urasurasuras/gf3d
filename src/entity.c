#include "simple_logger.h"
#include "collision.h"
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
	
	for (Uint32 i = 0; i < entity_manager.entity_count; i++) {
		if (entity_manager.entity_list[i]._inuse) {
			entity_free(&entity_manager.entity_list[i]);
		}
	}
}
void entity_manager_close() {
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
	
	for (Uint32 i = 0; i < entity_manager.entity_count; i++) {
		if (!entity_manager.entity_list[i]._inuse) {
			// Mark _inuse
			entity_manager.entity_list[i]._inuse = 1;

			gfc_matrix_identity(entity_manager.entity_list[i].modelMatrix);// Init matrix
			entity_manager.entity_list[i].position = vector3d(0, 0, 0);// Init pos
			entity_manager.entity_list[i].rotation = vector3d(0, 0, 0);// Init rotation
			entity_manager.entity_list[i].speed = 10;// Init speed
			//// Set facing direction to rotation
			//entity_manager.entity_list[i].facingDirection.x = cos(entity_manager.entity_list[i].rotation.x);
			//entity_manager.entity_list[i].facingDirection.y = sin(entity_manager.entity_list[i].rotation.x);
			return &entity_manager.entity_list[i];
		}
	}
	slog("No new entity slots!");
	return NULL;
}

void entity_draw(Entity* self, Uint32 bufferFrame, VkCommandBuffer commandBuffer) {
	if (!self)return;
	if (self->type == ent_PLAYER)return;

	Vector3D drawPos;
	vector3d_add(drawPos, self->position, self->modelPosOffset);
	gfc_matrix_make_translation(
		self->modelMatrix,
		drawPos
	);

	Vector3D drawRot;
	vector3d_add(drawRot, self->rotation, self->modelRotOffset);
	setRotation_model(self->modelMatrix, drawRot);

	gf3d_model_draw(self->model, bufferFrame, commandBuffer, self->modelMatrix);
}

void entity_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer) {
	
	for (Uint32 i = 0; i < entity_manager.entity_count; i++) {
		if (entity_manager.entity_list[i]._inuse) {
			entity_draw(&entity_manager.entity_list[i], bufferFrame, commandBuffer);
		}
	}
}

void entity_think_all(float deltaTime) {
	
	for (Uint32 i = 0; i < entity_manager.entity_count; i++) {
		if (entity_manager.entity_list[i]._inuse && entity_manager.entity_list[i].think) {
			entity_manager.entity_list[i].think(&entity_manager.entity_list[i], deltaTime);
		}
		if (entity_manager.entity_list[i].touch) {
			entity_collision_check(&entity_manager.entity_list[i]);
		}
	}
}

void entity_entity_collide(Entity* e1, Entity* e2) {
	if (collide_sphere(e1->position, e1->collider_radius, e2->position, e2->collider_radius))
	{// Sphere-to-sphere
		if (e1->touch)
		{
			e1->touch(e1, e2);
		}
	}
}
void entity_collision_check(Entity* entity)
{
	int i;
	if (!entity)return;
	for (i = 0; i < entity_manager.entity_count; i++)
	{

		if (!entity_manager.entity_list[i]._inuse)continue;
		if (&entity_manager.entity_list[i] == entity)continue;

		entity_entity_collide(entity, &entity_manager.entity_list[i]);

		if (entity->check_for_raycast) {
			Vector3D rayEnd;
			vector3d_scale(rayEnd, entity->facingDirection, 10);
			int yes = lineCircle(
				entity->position,
				rayEnd,
				entity_manager.entity_list[i].position,
				entity_manager.entity_list[i].collider_radius
			);
			if (yes) {
				slog("%s casting a ray on %s", entity->name, entity_manager.entity_list[i].name);
			}

		}
	}
}

EntityManager *get_entity_manager(){
	return &entity_manager;
}