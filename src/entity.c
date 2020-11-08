#include "simple_logger.h"
#include "collision.h"
#include "entity.h"
#include "game.h"

typedef struct {
	Entity* entity_list;
	Uint32 entity_count;
}EntityManager;

static EntityManager entity_manager = { 0 };

void entity_free(Entity* self) {
	if (!self)return;
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
			//entity_manager.entity_list[i].speed = 10;// Init speed
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
	if (!self->model)return;

	Vector3D drawPos;


	Vector3D drawOffSet;
	drawOffSet.x = self->facingDirection.x * self->modelPosOffset.x;
	drawOffSet.y = self->facingDirection.y * self->modelPosOffset.y;
	drawOffSet.z = self->facingDirection.z * self->modelPosOffset.z;

	Vector3D rayEnd;
	rayEnd = vector3d(0, 0, 0);

	if (self->type == ent_CHAR) {
		//Character* c = (Character*)self->data;
		//if (c->type == char_PLAYER) {
		//	Vector3D rayScale;

		//	vector3d_scale(rayScale, self->facingDirection, 1);//TODO: not hardcode
		//	vector3d_add(rayEnd, self->position, rayScale);

		//}
	}
	vector3d_add(self->modelPosOffset, self->modelPosOffset, rayEnd);
	vector3d_add(drawPos, self->position, self->modelPosOffset);

	gfc_matrix_make_translation(
		self->modelMatrix,
		drawPos
	);

	Vector3D drawRot;
	vector3d_add(drawRot, self->rotation, self->modelRotOffset);
	setRotation(self->modelMatrix, drawRot);

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
		if (!entity_manager.entity_list[i]._inuse)continue;
		TextWord name;
		strcpy(name, entity_manager.entity_list[i].name);
		if (entity_manager.entity_list[i].think) {
			entity_manager.entity_list[i].think(&entity_manager.entity_list[i], deltaTime);
		}
		entity_collision_check(&entity_manager.entity_list[i]);
	}
}

void entity_entity_collide(Entity* e1, Entity* e2) {
	Character* thisChar;
	Character* otherChar;

	Level* thisLevel;
	Level* otherLevel;

	RectPrism levelRect;
	switch (e1->type)
	{
	case ent_LEVEL:

		thisLevel = (Level*)e1->data;
		if (!thisLevel) {
			slog("This ent is not a leve!");
			break;
		}
		levelRect = thisLevel->bounds;
		switch (e2->type)
		{
		case ent_PROJECTILE:
			if (e2->position.x - e2->rigidbody.collider_radius < levelRect.x ||
				e2->position.x + e2->rigidbody.collider_radius > levelRect.w ||
				e2->position.z - e2->rigidbody.collider_radius < levelRect.z ||
				e2->position.z + e2->rigidbody.collider_radius > levelRect.d
				) {
				entity_free(e2);
			}
			break;
		default:
			break;
		}
		// Level does not need to do anything if it collides with something else
		// Every entity is responsible for their own collision behavior
		break;
	case ent_CHAR:

		thisChar = (Character*)e1->data;

		switch (e2->type)
		{
		case ent_LEVEL:
			otherLevel = (Level*)e2->data;
			levelRect = otherLevel->bounds;
			//e1->position.y -= gameManager()->deltaTime * GRAVITATIONAL_ACCELERATION;

			// If ent is below ground, pull them up
			if (e1->position.y - e1->rigidbody.collider_radius < levelRect.y) {
				e1->position.y = levelRect.y + e1->rigidbody.collider_radius;
			}
			else {// Apply gravity
			}
			//slog("%s at %.2f,%.2f,%.2f", e1->name, e1->position.x, e1->position.y, e1->position.z);

			if (e1->position.x - e1->rigidbody.collider_radius < levelRect.x) {
				e1->position.x = levelRect.x + (e1->rigidbody.collider_radius);
			}
			else if (e1->position.x + e1->rigidbody.collider_radius > levelRect.w) {
				e1->position.x = levelRect.w - (e1->rigidbody.collider_radius);
			}

			if (e1->position.z - e1->rigidbody.collider_radius < levelRect.z) {
				e1->position.z = levelRect.z + (e1->rigidbody.collider_radius);
			}
			else if (e1->position.z + e1->rigidbody.collider_radius > levelRect.d) {
				e1->position.z = levelRect.d - (e1->rigidbody.collider_radius);
			}
			

			break;
		case ent_CHAR:

			otherChar = (Character*)e2->data;

			if (!thisChar) {
				slog("%s is not a character! (has null data pointer)", e1->name);
				break;
			}
			if (!otherChar) {
				slog("%s is not a character! (has null data pointer)", e2->name);
				break;
			}

			// if this ent is in circle collision with the other ent
			if (collide_sphere(e1->position, e1->rigidbody.collider_radius, e2->position, e2->rigidbody.collider_radius))
			{// Sphere-to-sphere
				sphere_to_sphere_pushback(e1, e2);

				if (e1->touch)
				{
					e1->touch(e1, e2);
				}
			}

			if (thisChar->check_for_raycast) {// If this entity is meant to raycast
				Vector3D rayScale;
				Vector3D rayEnd;

				vector3d_scale(rayScale, e1->facingDirection, 1000);//TODO: not hardcode
				vector3d_add(rayEnd, e1->position, rayScale);

				//vector3d_slog(rayEnd);
				int yes = lineCircle(
					e1->position,
					rayEnd,
					e2->position,
					e2->rigidbody.collider_radius
				);
				if (yes) {
					slog("%s casting a ray on %s", e1->name, e2->name);
					otherChar->health -= 30;
				}
			}			
			break;
		case ent_PROJECTILE:
			break;

		default:
			//slog("Other ent %s has no type", e2->name);
			break;
		}

	case ent_PROJECTILE:

		// if this ent is in circle collision with the other ent
		if (collide_sphere(e1->position, e1->rigidbody.collider_radius, e2->position, e2->rigidbody.collider_radius))
		{// Sphere-to-sphere

			if (e1->touch)
			{
				e1->touch(e1, e2);
			}
		}
		break;

	default:
		//slog("This entity %s has no type!", e1->name);
		break;
	}

}
void entity_collision_check(Entity* entity)
{
	int i;
	if (!entity)return;
	if (!entity->_inuse)return;
	for (i = 0; i < entity_manager.entity_count; i++)
	{
		if (!entity_manager.entity_list[i]._inuse)continue;
		if (&entity_manager.entity_list[i] == entity)continue;

		Entity* other = &entity_manager.entity_list[i];

		entity_entity_collide(entity, other);
		
	}
}

void sphere_to_sphere_pushback(Entity * e1,Entity * e2 ) {

	float diffX = e1->position.x - e2->position.x;
	float diffY = e1->position.y - e2->position.y;
	float diffZ = e1->position.z - e2->position.z;
	float radii_sum = e1->rigidbody.collider_radius + e2->rigidbody.collider_radius;
	float length = vector3d_magnitude(vector3d(diffX, diffY, diffZ));
	float unitX = diffX / length;
	float unitY = diffY / length;
	float unitZ = diffZ / length;

	//slog("Diff: %.2f,%.2f,%.2f", diffX, diffY, diffZ);

	e1->position.x = e2->position.x + (radii_sum)*unitX;
	e1->position.y = e2->position.y + (radii_sum)*unitY;
	e1->position.z = e2->position.z + (radii_sum)*unitZ;
}

EntityManager *get_entity_manager(){
	return &entity_manager;
}