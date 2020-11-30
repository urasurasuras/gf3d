#include "dinos.h"

void spawn_dino_yellow_random() {
    // Create ent
    Entity* dino = entity_new();
    if (!dino)return;
    dino->entData = malloc(sizeof(Character));
    memset(dino->entData, 0, sizeof(Character));
    Character* sinoData = (Character*)dino->entData;
    dino->type = ent_CHAR;
    gfc_word_cpy(dino->name, "Dino");
    dino->model = gf3d_model_load("dino");


    dino->position.y = 6;//height
    dino->position.z = gfc_crandom() * gameManager()->level->bounds.d;
    dino->position.x = gfc_crandom() * gameManager()->level->bounds.w;
    //dino->think = floor_rotate;

    dino->think = dino_think;
    dino->touch = dino_touch;

    dino->rigidbody.collider_radius = 6;
    dino->rigidbody.gravity_scale = 1;
    sinoData->check_for_raycast = 0;
    dino->rigidbody.speed = 5;
    sinoData->health = 100;
    sinoData->power = .1;
    sinoData->CLDN1 = 0;
    sinoData->type = char_AI;

    dino->modelRotOffset = vector3d(-GFC_HALF_PI, -GFC_HALF_PI, 0);

    sinoData->charData = malloc(sizeof(MOB));
    memset(sinoData->charData, 0, sizeof(Character));
    MOB* mob_data = (MOB*)sinoData->charData;
    mob_data->target = gameManager()->player;
    mob_data->reachedTarget = 0;
    mob_data->mobType = mob_YELLOW;
    slog("Targeting %s", mob_data->target->name);
}

void spawn_dino_red_random() {
    // Create ent
    Entity* dino = entity_new();
    if (!dino)return;
    dino->entData = malloc(sizeof(Character));
    memset(dino->entData, 0, sizeof(Character));
    Character* sinoData = (Character*)dino->entData;
    dino->type = ent_CHAR;
    gfc_word_cpy(dino->name, "Red Dino");
    dino->model = gf3d_model_load("dino_red");


    dino->position.y = 6;//height
    dino->position.z = gfc_crandom() * gameManager()->level->bounds.d;
    dino->position.x = gfc_crandom() * gameManager()->level->bounds.w;

    dino->think = dino_think;
    dino->touch = dino_touch;

    dino->rigidbody.collider_radius = 6;
    dino->rigidbody.gravity_scale = 1;
    sinoData->check_for_raycast = 0;
    dino->rigidbody.speed = 50;
    sinoData->health = 100;
    sinoData->power = 50;
    sinoData->CLDN1 = 5000;
    sinoData->type = char_AI;
    dino->modelRotOffset = vector3d(-GFC_HALF_PI, -GFC_HALF_PI, 0);

    sinoData->charData = malloc(sizeof(MOB));
    memset(sinoData->charData, 0, sizeof(Character));
    MOB* mob_data = (MOB*)sinoData->charData;
    mob_data->target = gameManager()->player;
    mob_data->reachedTarget = 0;
    mob_data->mobType = mob_RED;
    slog("Targeting %s", mob_data->target->name);
}
void spawn_dino_blue_random() {
    // Create ent
    Entity* dino = entity_new();
    if (!dino)return;
    dino->entData = malloc(sizeof(Character));
    memset(dino->entData, 0, sizeof(Character));
    Character* sinoData = (Character*)dino->entData;
    dino->type = ent_CHAR;
    gfc_word_cpy(dino->name, "Blue Dino");
    dino->model = gf3d_model_load("dino_blue");


    dino->position.y = 6;//height
    dino->position.z = gfc_crandom() * gameManager()->level->bounds.d;
    dino->position.x = gfc_crandom() * gameManager()->level->bounds.w;
    //dino->think = floor_rotate;

    dino->think = dino_think;
    dino->touch = dino_touch;

    dino->rigidbody.collider_radius = 6;
    dino->rigidbody.gravity_scale = 1;
    sinoData->check_for_raycast = 0;
    dino->rigidbody.speed = 5;
    sinoData->health = 100;
    sinoData->power = 1;
    sinoData->CLDN1 = 1000;
    sinoData->type = char_AI;
    dino->modelRotOffset = vector3d(-GFC_HALF_PI, -GFC_HALF_PI, 0);

    sinoData->charData = malloc(sizeof(MOB));
    memset(sinoData->charData, 0, sizeof(Character));
    MOB* mob_data = (MOB*)sinoData->charData;
    mob_data->target = gameManager()->player;
    mob_data->reachedTarget = 0;
    mob_data->mobType = mob_BLUE;
    slog("Targeting %s", mob_data->target->name);
}

void dino_think(Entity* self) {
    //float deltaTime =  gameManager()->deltaTime;

    Character* character = (Character*)self->entData;

    Vector2D distance;

    MOB* mob_data = (MOB*)character->charData;
    Entity* target = mob_data->target;
    distance.x = target->position.x - self->position.x;
    distance.y = target->position.z - self->position.z;

    // Set velocity vector with speed
    self->rigidbody.velocity.x = 0;
    //self->rigidbody.velocity.y = 0;
    self->rigidbody.velocity.z = 0;

    self->rotation.y = atan2(distance.y, distance.x);
    /*+ M_PI*/;
    self->facingDirection.x = cos(self->rotation.y) /** cos(self->rotation.x)*/;
    self->facingDirection.z = sin(self->rotation.y);
    //vector3d_slog(self->facingDirection);

    self->rigidbody.velocity.x = self->facingDirection.x;
    self->rigidbody.velocity.z = self->facingDirection.z;


    if (character->last_CLDN1 + character->CLDN1 < SDL_GetTicks()) {

        switch (mob_data->mobType)
        {
        case mob_RED:
            mob_data->reachedTarget = 0;
            break;
        case mob_BLUE:
            projectile_water_spawn(self);
            break;
        default:
            break;
        }
        character->last_CLDN1 = SDL_GetTicks();
    }

    if (!mob_data->reachedTarget) {
        entity_move(self);
    }

    //self->position.y -= 0.01;
    /*vector3d_slog(gameManager()->player->position);*/
    if (character->health <= 0) {
        entity_free(self);
    }

}
void dino_touch(Entity* self, Entity* other) {
    if (other->type == ent_CHAR) {
        Character* c = (Character*)other->entData;
        if (c->type == char_PLAYER) {
            c->health -= c->power;
        }
    }
}
