#include "dinos.h"

void spawn_dino_yellow_random() {
    // Declerations
    GameManager * game_manager = NULL;
    Entity* dino = NULL;
    Character* char_data = NULL;
    MOB* mob_data = NULL;

    // Game Manager
    game_manager = gameManager();

    if(!game_manager){
        slog("NULL game manager");
        entity_free(dino);
        return;
    }
    if(     (!game_manager->level) || (!game_manager->player)    ){
        slog("NULL game manager data");
        entity_free(dino);
        return;
    }

    // Entity data
    dino = entity_new();
    if (!dino)return;

    // Character data
    char_data = malloc(sizeof(Character));
    if(!char_data){
        slog("NULL char_data");
        entity_free(dino);
        return;
    }

    dino->entData = (void*)char_data;
    memset(dino->entData, 0, sizeof(Character));

    // MOB data
    mob_data = malloc(sizeof(MOB));
    if(!mob_data){
        slog("NULL mob_data");
        entity_free(dino);
        return;
    }

    char_data->charData = (void*)mob_data;
    memset(char_data->charData, 0, sizeof(MOB));


    // Data assignment
    dino->type = ent_CHAR;
    gfc_word_cpy(dino->name, "Dino");
    dino->model = gf3d_model_load("dino");
    if(!dino->model){
        slog("NULL dino model");
        entity_free(dino);
        return;
    }

    dino->position.y = 6;//height
    dino->position.z = gfc_crandom() * game_manager->level->bounds.d;
    dino->position.x = gfc_crandom() * game_manager->level->bounds.w;
    //dino->think = floor_rotate;

    dino->think = dino_think;
    dino->touch = dino_touch;
    dino->rigidbody.collider_radius = 6;
    dino->rigidbody.gravity_scale = 1;
    dino->rigidbody.speed = 5;
    dino->modelRotOffset = vector3d(-GFC_HALF_PI, -GFC_HALF_PI, 0);

    char_data->check_for_raycast = 0;
    char_data->health = 100;
    char_data->power = .1;
    char_data->CLDN1 = 0;
    char_data->type = char_AI;

    mob_data->target = game_manager->player;
    mob_data->reachedTarget = 0;
    mob_data->mobType = mob_YELLOW;

    slog("Targeting %s", mob_data->target->name);
}

void spawn_dino_red_random() {
    // Declerations
    GameManager * game_manager = NULL;
    Entity* dino = NULL;
    Character* char_data = NULL;
    MOB* mob_data = NULL;

    // Game Manager
    game_manager = gameManager();

    if(!game_manager){
        slog("NULL game manager");
        entity_free(dino);
        return;
    }
    if(     (!game_manager->level) || (!game_manager->player)    ){
        slog("NULL game manager data");
        entity_free(dino);
        return;
    }

    // Entity data
    dino = entity_new();
    if (!dino)return;

    // Character data
    char_data = malloc(sizeof(Character));
    if(!char_data){
        slog("NULL char_data");
        entity_free(dino);
        return;
    }

    dino->entData = (void*)char_data;
    memset(dino->entData, 0, sizeof(Character));

    // MOB data
    mob_data = malloc(sizeof(MOB));
    if(!mob_data){
        slog("NULL mob_data");
        entity_free(dino);
        return;
    }

    char_data->charData = (void*)mob_data;
    memset(char_data->charData, 0, sizeof(MOB));

    // Data assignment
    dino->position.y = 6;//height
    dino->position.z = gfc_crandom() * game_manager->level->bounds.d;
    dino->position.x = gfc_crandom() * game_manager->level->bounds.w;

    dino->think = dino_think;
    dino->touch = dino_touch;
    dino->rigidbody.speed = 50;
    dino->rigidbody.collider_radius = 6;
    dino->rigidbody.gravity_scale = 1;
    dino->modelRotOffset = vector3d(-GFC_HALF_PI, -GFC_HALF_PI, 0);

    char_data->check_for_raycast = 0;
    char_data->health = 100;
    char_data->power = 50;
    char_data->CLDN1 = 5000;
    char_data->type = char_AI;

    mob_data->target = game_manager->player;
    mob_data->reachedTarget = 0;
    mob_data->mobType = mob_RED;
    slog("Targeting %s", mob_data->target->name);
}
void spawn_dino_blue_random() {
    // Declerations
    GameManager * game_manager = NULL;
    Entity* dino = NULL;
    Character* char_data = NULL;
    MOB* mob_data = NULL;

    // Game Manager
    game_manager = gameManager();

    if(!game_manager){
        slog("NULL game manager");
        entity_free(dino);
        return;
    }
    if(     (!game_manager->level) || (!game_manager->player)    ){
        slog("NULL game manager data");
        entity_free(dino);
        return;
    }

    // Entity data
    dino = entity_new();
    if (!dino)return;

    // Character data
    char_data = malloc(sizeof(Character));
    if(!char_data){
        slog("NULL char_data");
        entity_free(dino);
        return;
    }

    dino->entData = (void*)char_data;
    memset(dino->entData, 0, sizeof(Character));

    // MOB data
    mob_data = malloc(sizeof(MOB));
    if(!mob_data){
        slog("NULL mob_data");
        entity_free(dino);
        return;
    }

    char_data->charData = (void*)mob_data;
    memset(char_data->charData, 0, sizeof(MOB));

    // Data assignment
    dino->position.y = 6;//height
    dino->position.z = gfc_crandom() * game_manager->level->bounds.d;
    dino->position.x = gfc_crandom() * game_manager->level->bounds.w;
    //dino->think = floor_rotate;

    dino->think = dino_think;
    dino->touch = dino_touch;

    dino->rigidbody.collider_radius = 6;
    dino->rigidbody.gravity_scale = 1;
    char_data->check_for_raycast = 0;
    dino->rigidbody.speed = 5;
    char_data->health = 100;
    char_data->power = 1;
    char_data->CLDN1 = 1000;
    char_data->type = char_AI;
    dino->modelRotOffset = vector3d(-GFC_HALF_PI, -GFC_HALF_PI, 0);

    mob_data->target = game_manager->player;
    mob_data->reachedTarget = 0;
    mob_data->mobType = mob_BLUE;
    slog("Targeting %s", mob_data->target->name);
}

void dino_think(Entity* self) {
    
    if(!self){
        slog("No dino to think");
        return;
    }

    // Declerations
    Character* char_data = NULL;
    MOB* mob_data = NULL;

    char_data = (Character*)self->entData;
    if(!char_data){
        slog("No char data for %s", self->name);
        return;
    }
    mob_data = (MOB*)char_data->charData;
    if(!mob_data){
        slog("No mob data for %s", self->name);
        return;
    }

    Vector2D distance;

    distance.x = mob_data->target->position.x - self->position.x;
    distance.y = mob_data->target->position.z - self->position.z;
    self->rotation.y = atan2(distance.y, distance.x);

    // Set velocity vector with speed
    self->rigidbody.velocity.x = 0;
    //self->rigidbody.velocity.y = 0;
    self->rigidbody.velocity.z = 0;

    /*+ M_PI*/;
    self->facingDirection.x = cos(self->rotation.y) /** cos(self->rotation.x)*/;
    self->facingDirection.z = sin(self->rotation.y);
    //vector3d_slog(self->facingDirection);

    self->rigidbody.velocity.x = self->facingDirection.x;
    self->rigidbody.velocity.z = self->facingDirection.z;


    if (char_data->last_CLDN1 + char_data->CLDN1 < SDL_GetTicks()) {

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
        char_data->last_CLDN1 = SDL_GetTicks();
    }

    if (!mob_data->reachedTarget) {
        entity_move(self);
    }

    if (char_data->health <= 0) {
        entity_free(self);
    }
    slog("DINO THINK");
}
void dino_touch(Entity* self, Entity* other) {

    Character* c = NULL;

    if(!self){
        slog("No self");
        return;
    }
    if(!other){
        slog("No other");
        return;
    }

    if (other->type == ent_CHAR) {
        c = (Character*)other->entData;

        if(!c){
            slog("No other char");
            return;
        }

        if (c->type == char_PLAYER) {
            c->health -= c->power;
        }
    }
}
