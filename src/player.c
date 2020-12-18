#include "player.h"
#include "simple_logger.h"

Entity * player_spawn(){

	Character* player_data = NULL;

	Entity* player = entity_new();
	if(!player){
		slog("Could not get new ent for player");
		return NULL;
	}

	player_data = malloc(sizeof(Character));
	if(!player_data){
		slog("Player data could not be allocated");
		return player;
	}

    gfc_word_cpy(player->name, "Player");
    gf3d_get_cam()->player = player;
	player->modelPosOffset = vector3d(0, 0, 10);
    player->modelRotOffset = vector3d(GFC_HALF_PI, 0, GFC_HALF_PI);
    player->think = player_think;
    //player->think = floor_rotate;
    player->type = ent_CHAR;
    player->rigidbody.speed = 20;
    player->position.x = 5;
    player->position.y = 5;
    player->position.z = 5;
    player->touch = entity_touch;
    player->rigidbody.collider_radius = 4;
    player->rigidbody.gravity_scale = .5;

    player_data->check_for_raycast = 0;
    player_data->CLDN1 = 1000;
    player_data->last_CLDN1 = 0;
    player_data->CLDN2 = 100;
    player_data->last_CLDN2 = 0;
    player_data->CLDN3 = 2000;
    player_data->last_CLDN3 = 0;
    player_data->power = 1;
    player_data->primaryAttack = player_hitscan_attack;
    player_data->health = 100;
    player_data->speed_buff = 1;
    //player->model = gf3d_model_load("USP45");
    player_data->type = char_PLAYER;
	
	player->entData = player_data;
	gameManager()->player_data = player_data;

	return player;
}

void player_hitscan_attack(Entity* self) {

	Character* character = NULL;

	if (!self){
		slog("NULL attacker provided");
		return;
	}

	character = (Character*)self->entData;
	if (!character){
		slog("No character data provided for %s", self->name);
		return;
	}

	character->check_for_raycast = 1;
	self->rotation.x += gfc_random() * .02;
	self->rotation.y += gfc_crandom() * .01;
	slog("PAP");
}
void player_projectile_attack(Entity* self) {

	if (!self){
		slog("NULL self");
		return;
	}
	
	projectile_beachball_spawn(self);
}
void player_projectile_arrow_attack(Entity* self) {

	if (!self){
		slog("NULL self");
		return;
	}
	
	projectile_arrow_spawn(self);
}

void player_think(Entity* self) {	

	if (!self){
		slog("NULL self");
		return;
	}
	
	float deltaTime =  gameManager()->deltaTime;

	Character* character = (Character*)self->entData;
	if (!character){
		slog("No character data provided for %s", self->name);
		return;
	}

	// Set facing direction to rotation
	self->facingDirection.x = cos(self->rotation.y) /** cos(self->rotation.x)*/;
	self->facingDirection.y =/* cos(self->rotation.y) **/ sin(self->rotation.x);
	self->facingDirection.z = sin(self->rotation.y);

	// Set velocity vector with speed
	self->rigidbody.velocity.x = 0;
	//self->rigidbody.velocity.y = 0;
	self->rigidbody.velocity.z = 0;

	Vector3D axisR = vector3d(0, 0, 0);
	vector3d_copy(axisR, self->facingDirection);
	axisR.x = self->facingDirection.z;
	axisR.z = -self->facingDirection.x;

	if (keys[SDL_SCANCODE_W]) {
		// Set velocity to facing dir
		self->rigidbody.velocity.x = self->facingDirection.x;
		self->rigidbody.velocity.z = self->facingDirection.z;
	}
	else if (keys[SDL_SCANCODE_S]) {
		self->rigidbody.velocity.x -= self->facingDirection.x;
		self->rigidbody.velocity.z -= self->facingDirection.z;
	}

	if (keys[SDL_SCANCODE_A]) {

		self->rigidbody.velocity.x += axisR.x;
		self->rigidbody.velocity.z += axisR.z;
	}
	else if (keys[SDL_SCANCODE_D]) {

		self->rigidbody.velocity.x -= axisR.x;
		self->rigidbody.velocity.z -= axisR.z;
	}

	if (keys[SDL_SCANCODE_LSHIFT]) {
		self->rigidbody.speed = 35;
	}
	else {
		self->rigidbody.speed = 15;
	}

	if (character->last_CLDN1 + character->CLDN1 < SDL_GetTicks()) {
		if (keys[SDL_SCANCODE_SPACE]) {
			character->last_CLDN1 = SDL_GetTicks();
			self->position.y += 10;
			//self->rigidbody.velocity.y += 1;
		}
	}
	if (character->last_CLDN2 + character->CLDN2 < SDL_GetTicks()) {		
		if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {

			if (character->primaryAttack) {
				character->primaryAttack(self);
			}
			character->last_CLDN2 = SDL_GetTicks();
		}		
	}
	else
	{
		character->check_for_raycast = 0;
	}		

	if (character->last_CLDN3 + character->CLDN3 < SDL_GetTicks()) {
		//slog("%d", SDL_GetTicks());
		int swapped = 0;

		if (keys[SDL_SCANCODE_1]) {
			character->primaryAttack = player_hitscan_attack;
			character->CLDN2 = 100;
			slog("Weapon 1 selected");
			swapped = 1;
		}
		if (keys[SDL_SCANCODE_2]) {
			character->primaryAttack = player_projectile_attack;
			character->CLDN2 = 500;
			slog("Weapon 2 selected");
			swapped = 1;
		}
		if (keys[SDL_SCANCODE_3]) {
			character->primaryAttack = player_projectile_arrow_attack;
			character->CLDN2 = 1000;
			slog("Weapon 3 selected");
			swapped = 1;
		}
		// if (keys[SDL_SCANCODE_4]) {
		// 	character->primaryAttack = projectile_monkeybomb_spawn;
		// 		character->CLDN2 = 5000;
		// 	slog("Weapon 4 selected");
		// 	swapped = 1;
		// }
		if (keys[SDL_SCANCODE_5]) {
			swapped = 1;

		}

		if (swapped == 1)
			character->last_CLDN3 < SDL_GetTicks();
		//slog("%d", character->last_CLDN3);

	}
	self->rigidbody.speed *= character->speed_buff;
	entity_move(self);

	if (character->speed_buff < 1) {
		character->speed_buff = 1;
	}
	else {
		character->speed_buff -= deltaTime;
	}
}

void entity_move(Entity * self) {

	if (!self){
		slog("NULL self");
		return;
	}
	
	float deltaTime =  gameManager()->deltaTime;

	// Increment position from velocity
	self->position.x += (self->rigidbody.velocity.x * self->rigidbody.speed * deltaTime);
	self->position.y += (self->rigidbody.velocity.y * self->rigidbody.speed * deltaTime);
	self->position.z += (self->rigidbody.velocity.z * self->rigidbody.speed * deltaTime);

}


void entity_touch(Entity * self, Entity * other) {

	if (!self){
		slog("NULL self");
		return;
	}
	
	if (!other){
		slog("NULL other");
		return;
	}
	
	//slog("%s colliding with %s", self->name, other->name);
}

void pickup_health(Entity * self, Entity * other){

	if (!self){
		slog("NULL self");
		return;
	}
	
	if (!other){
		slog("NULL other");
		return;
	}
	
	Character* c = (Character*)other->entData;
	if (!c){
		slog("No character data provided for %s", self->name);
		return;
	}

	if (c->type == char_PLAYER) {
		c->health += 100;
	}
	slog("%s health now: %.2f", other->name, c->health);

	entity_free(self);
}
void pickup_speed(Entity * self, Entity * other){

	if (!self){
		slog("NULL self");
		return;
	}
	
	if (!other){
		slog("NULL other");
		return;
	}
	
	Character* c = (Character*)other->entData;
	if (!c){
		slog("No character data provided for %s", self->name);
		return;
	}
		if (c->type == char_PLAYER) {
		c->speed_buff = 5;
	}
	slog("%s gave speed buff: x%.2f", other->name, c->speed_buff);
	entity_free(self);

}
void pickup_damage(Entity * self, Entity * other){
	
	if (!self){
		slog("NULL self");
		return;
	}
	if (!other){
		slog("NULL other");
		return;
	}

	for (Uint32 i = 0; i < get_entity_manager()->entity_count; i++) {
		Entity * other_in_aoe = NULL;
		Character* other_char = NULL;
		other_in_aoe = &get_entity_manager()->entity_list[i];		
		if (!other_in_aoe){
			slog("NULL other");
			continue;

			if (!other_in_aoe->_inuse){
				continue;
			}
		}
		
		other_char = (Character*)other->entData;
		if (!other_char){
			slog("NULL other char");
			continue;
		}

		switch (other_in_aoe->type)
		{
		case ent_CHAR:
			if (other_char->type == char_AI) {
				// Do damage
				other_char->health = 1;
				slog("can instakill %s on %.2f", other->name, other_char->health);

			}
			break;
		default:
			break;
		}
	}
	entity_free(self);
}
void pickup_kaboom(Entity * self, Entity * other){	
		
	if (!self){
		slog("NULL self");
		return;
	}
	if (!other){
		slog("NULL other");
		return;
	}

	for (Uint32 i = 0; i < get_entity_manager()->entity_count; i++) {
		Entity * other_in_aoe = NULL;
		Character* other_char = NULL;
		other_in_aoe = &get_entity_manager()->entity_list[i];		
		if (!other_in_aoe){
			slog("NULL other");
			continue;

			if (!other_in_aoe->_inuse){
				continue;
			}
		}
		
		other_char = (Character*)other->entData;
		if (!other_char){
			slog("NULL other char");
			continue;
		}

		switch (other->type)
		{
		case ent_CHAR:
			if (other_char->type == char_AI) {
				// Do damage
				slog("kaboom target %s", other->name);

				entity_free(other);
			}
			break;
		default:
			break;
		}

	}
	entity_free(self);
}

void sine_movement(Entity* self) {

	if (!self){
		slog("NULL self");
		return;
	}

	self->position.y = (sin(SDL_GetTicks() * 0.01));
	self->position.y += self->rigidbody.collider_radius;

	//slog("%f", self->position.y);
}
