#include "player.h"


 Player player = {
		.position = { 0,0 },
		.CollisionBox = { { 200/5*3,65/5*3+5 },{200/5*3+5,70/5*3+5},{200/5*3,80/5*3+5} },
};
 

 //Initialazing player data based on what is stored in file
 void InitPlayer(int vehicleID) {
	 player.health = vehicle_upgrades[vehicleID][0]*100+100;
	 player.health_regen = vehicle_upgrades[vehicleID][1];
	 player.velocity = (Vector2) { 0, 0 };
	 player.position = (Vector2) { 600, 550 };
 } 

 //Processing player movement
void PlayerMovement(int vehicleID) {
	static int counter = 0;
	player.velocity = (Vector2) { 0, 0 };

	if ( IsKeyDown(KEY_A)) {
		player.velocity.x = -9*GetFrameTime()*70;//r
	}
	else if (IsKeyDown(KEY_D)) {
		player.velocity.x = 9 * GetFrameTime() * 70;//l
	}

	if ( IsKeyDown(KEY_W)) {
		player.velocity.y = -9 * GetFrameTime() * 70;//d
		player.anime[vehicleID].currentFrame = 1;
	}
	else if ( IsKeyDown(KEY_S)) {
		player.velocity.y = 9 * GetFrameTime() * 70;//u
		player.anime[vehicleID].currentFrame = 2;
	}
	else {
		player.anime[vehicleID].currentFrame = 0;
	}

	player.position.x += player.velocity.x;
	player.position.y += player.velocity.y;

	//Ship oscillations make movement feel more real
	if (counter % 10 == 0) {
		player.position.y += GetRandomValue(-1, 1);
	}
	if (counter == 100) {
		counter = 1;
	}
	else {
		counter++;
	}
	LimitMovement();
}

//Limiting player movement so player doesnt get out of screen boundaries
void LimitMovement(void) {
	if (player.position.x < 0) {
		player.position.x = 0;
	}
	if (player.position.y > 1080-60) {
		player.position.y = 1080-60;
	}
	if (player.position.x > 1920-75) {
		player.position.x = 1920-75;
	}
	if (player.position.y < 0) {
		player.position.y = 0;
	}
}

//Putting together animations
Animation lightning_anim;
Animation engine_thrust_anim;
Animation laser_beam_animation[3];
Animation shield_animation;
Animation shield_particles_animation[MAX_ENEMY_BULLETS];
Animation explosion1_animation[MAX_BULLETS];
Animation medium_explosion_animation[MAX_ENEMIES];
Animation player_explosion_animation;
Animation explosion1_player_animation[MAX_ENEMY_BULLETS];
Animation boss_explosion_animation;

void SetPlayerAnimations(void) {
	//Animation of each ship
	player.anime[0] = SetAnimation((Rectangle) { 0, 0, 429/3, 54 }, 3, 0);
	player.anime[1] = SetAnimation((Rectangle) { 0, 0, 732/5, 94/5*3 }, 3, 0);
	player.anime[2] = SetAnimation((Rectangle) { 0, 0, 708 /5, 100 /5*3}, 3, 0);

	lightning_anim = SetAnimation((Rectangle) { 0, 0, 200, 200 }, 3, 200);
	engine_thrust_anim = SetAnimation((Rectangle) { 0, 0, 24*3, 22*3 }, 6, 100);

	laser_beam_animation[0] = SetAnimation((Rectangle) { 0, 0, 25, 51 }, 2, 100);
	laser_beam_animation[1] = SetAnimation((Rectangle) { 0, 0, 1, 54 }, 2, 100);

	shield_animation= SetAnimation((Rectangle) { 0, 0, 162/3, 106 }, 3, 100);
	for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
		shield_particles_animation[i] = SetAnimation((Rectangle) { 0, 0, 64, 64 }, 4, 75);
		explosion1_player_animation[i]= SetAnimation((Rectangle) { 0, 0, 32, 32 }, 5, 50);
	}
	for (int i = 0; i < MAX_BULLETS; i++) {
		explosion1_animation[i] = SetAnimation((Rectangle) { 0, 0,32 ,32}, 5, 50);
	}
	for (int i = 0; i< MAX_ENEMIES; i++) {
		medium_explosion_animation[i] = SetAnimation((Rectangle) { 0, 0, 200, 224 }, 13, 70);
	}

	player_explosion_animation = SetAnimation((Rectangle) { 0, 0, 200, 224 }, 13, 70); //final explosion
	boss_explosion_animation = SetAnimation((Rectangle) { 0, 0, 5200 / 13, 448 }, 13, 100);

}

//Implementation of player dashing effect
void PlayerDash(void) {
	static bool pressed = false, trigger_delay = false;
	static int counter = 0,delay=0;
	int direction[4] = {0,0,0,0};
	int buttons_pressed = 0;

	if (IsKeyDown(KEY_S)) {
		direction[0]=1;
		buttons_pressed++;
	}
	else {
		direction[0] = 0;
	}
	if (IsKeyDown(KEY_D)) {
		direction[1]=1;
		buttons_pressed++;
	}
	else {
		direction[1] = 0;
	}
	if (IsKeyDown(KEY_A)) {
		direction[2]=1;
		buttons_pressed++;
	}
	else {
		direction[2] = 0;
	}
	if (IsKeyDown(KEY_W)) {
		direction[3]=1;
		buttons_pressed++;
	}
	else {
		direction[3] = 0;
	}


	if (IsKeyPressed(KEY_LEFT_SHIFT) && delay==0) {
		trigger_delay = true;
		pressed = true;
		counter = 0;
	}
	if (trigger_delay) {
		delay++;
	}
	if (delay == 50) {
		delay = 0;
		trigger_delay = false;
	}
	if (pressed ) {
		if (buttons_pressed == 2) {
			if (direction[0] == 1 && direction[1] == 1) {
				player.position.x += 20;
				player.position.y += 20;
				counter++;
			}
			else if (direction[0] == 1 && direction[2] == 1) {
				player.position.x -= 20;
				player.position.y += 20;
				counter++;
			}
			else if (direction[3] == 1 && direction[2] == 1) {
				player.position.x -= 20;
				player.position.y -= 20;
				counter++;
			}
			else if (direction[3] == 1 && direction[1] == 1) {
				player.position.x += 20;
				player.position.y -= 20;
				counter++;
			}

		}
		else {
			if (direction[0] == 1) {
				player.position.y += 40;
				counter++;
			}
			if (direction[1] == 1) {
				player.position.x += 40;
				counter++;
			}
			if (direction[2] == 1) {
				player.position.x -= 40;
				counter++;
			}
			if (direction[3] == 1) {
				player.position.y -= 40;
				counter++;
			}
		}
	}
	if (counter % 5 == 0) {
		counter = 0;
		pressed = false;
	}
	
	return pressed;
	
}


extern Texture2D spaceship[4];
extern Texture2D engine_thrust;
extern Texture2D medium_explosion;
extern bool paused;

Vector2 exhaust_pos[3] = {{ 13*3, 0}, {  13*3, 0}, {  13*3, 0} };
Color exhaust_color[3] = { { 255,255,255,255 },{255,65,0,255},{255,0,0,200} };

//Drawing player on screen
void DrawPlayer(int vehicleID) {
	static bool draw_box = false;

	if (player_explosion_animation.currentFrame <= 6 || player.health>0) {
		DrawTextureRec(engine_thrust, (Rectangle) { engine_thrust_anim.frameStart.x + (engine_thrust_anim.currentFrame*engine_thrust_anim.frameStart.width), engine_thrust_anim.frameStart.y, engine_thrust_anim.frameStart.width, engine_thrust_anim.frameStart.height }, Vector2Subtract((Vector2) { player.position.x, player.position.y }, exhaust_pos[vehicleID]), exhaust_color[vehicleID]);
		if (!paused) {
			UpdateAnimation(&engine_thrust_anim);
		}
		DrawTextureRec(spaceship[vehicleID], (Rectangle) { player.anime[vehicleID].frameStart.x + (player.anime[vehicleID].currentFrame*player.anime[vehicleID].frameStart.width), player.anime->frameStart.y, player.anime[vehicleID].frameStart.width, player.anime[vehicleID].frameStart.height }, player.position, WHITE);
	}
	//death (explosion) animation
	if (player.health <= 0 && player_explosion_animation.currentFrame < 12) {
		UpdateAnimation(&player_explosion_animation);
		DrawTextureRec(medium_explosion, (Rectangle) { player_explosion_animation.frameStart.x + (player_explosion_animation.currentFrame*player_explosion_animation.frameStart.width), player_explosion_animation.frameStart.y, player_explosion_animation.frameStart.width, player_explosion_animation.frameStart.height }, (Vector2) { player.position.x-50,player.position.y-50 }, WHITE);
	}
	if (player_explosion_animation.currentFrame == 12 && player.health>0) {
		player_explosion_animation.currentFrame = 0;
	}


	if (draw_box) {
		DrawRectangleLines(player.position.x, player.position.y, player.CollisionBox[vehicleID].x, player.CollisionBox[vehicleID].y, BLUE);
	}
	
	if (IsKeyPressed(KEY_TAB)) {
		draw_box = !draw_box;
	}
}

extern float vehicle_health;
extern Font myFont;
extern Texture2D health,health2;

//Drawing player health
void DrawPlayerHealth(int vehicleID) {
	static bool init_vehicle_health = false;

	if (!init_vehicle_health) {
		vehicle_health = vehicle_upgrades[vehicleID][0] * 100 + 100;
		init_vehicle_health = true;
	}
	Rectangle rec = { 20*3, 20*3, 500,10*3 };
	float current_health = ceilf((player.health / vehicle_health)*15);
	for (int i = 0; i < (int)current_health; i++) {
		DrawTexture(health2,60 + i * 30, 20, WHITE);
	}
	for (int i = 0; i < 15; i++) {
		DrawTexture(health, 60 + i * 30, 20, WHITE);
	}
	DrawLine(65, 15, 55, 61, DARKRED);
	DrawLine(60+460, 15, 50+460, 61, DARKRED);
	DrawLine(65, 15, 60 + 460, 15, DARKRED);
	DrawLine(55, 61, 510, 61, DARKRED);

}

void HealthRegen(int vehicleID) {
	if (player.health+ (vehicle_upgrades[vehicleID][1] + 1)*0.02f <= vehicle_health) {
		player.health += (vehicle_upgrades[vehicleID][1]+1)*0.02f;
	}
}

int posX=0, posY=0;
float ShockWaveRadius=0.0f;
extern Sound shockwave_sound;
extern bool enemy_got_hit_by_shockwave[MAX_ENEMIES],restart_cooldown_timers;

//Implementation of shock wave ability
void ShockWaveAbility(void) {
	static float timer = 0;
	static bool init_timer = false;
	Vector2 ShockwaveButton = { 600*3,300*3 };
	static bool EmitShockWave = false;
	

	if (special_ability[1][0] == 1) {
		DrawCircleLines(ShockwaveButton.x, ShockwaveButton.y, 60, (Color) { 255, 0, 0, 255 });
		if (IsKeyPressed(KEY_J) && init_timer==false) {
			PlaySound(shockwave_sound);
			init_timer = true;
			EmitShockWave = true;
			posX = player.position.x;
			posY = player.position.y;
		}
		else if (CheckCollisionPointCircle((Vector2) { GetMousePosition().x,GetMousePosition().y }, ShockwaveButton, 60) && init_timer == false) {
			DrawCircle(ShockwaveButton.x, ShockwaveButton.y, 60, (Color) { 255, 0, 0, 255 });
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				init_timer = true;
				EmitShockWave = true;
				posX = player.position.x;
				posY = player.position.y;
			}

		}

		if (EmitShockWave) {
			DrawCircleGradient(posX, posY, ShockWaveRadius, (Color) { 0, 0, 0, 0 }, (Color) { 255, 161, 0, 255 - ShockWaveRadius / 5 });
			if (!paused) {
				ShockWaveRadius += 20 * GetFrameTime() * 75;
			}

			if (255 - ShockWaveRadius / 5 <= 0) {
				ShockWaveRadius = 0;
				posX = 0;
				posY = 0;
				EmitShockWave = false;
			}
		}

		if (init_timer && !paused) {
			timer+=0.25f*GetFrameTime()*75; // cooldown
			DrawCircleSectorLines(ShockwaveButton, 60, 180, 180 - timer, 100, (Color) {255,0,0,255});
			if (timer >= 360) {
				init_timer = false;
				timer = 0;
				for (int i = 0; i < MAX_ENEMIES; i++) {
					enemy_got_hit_by_shockwave[i] = false;
				}
				
			}
		}
		if (restart_cooldown_timers) {
			init_timer = false;
			timer = 0;
			ShockWaveRadius = 0;
			EmitShockWave = false;
			restart_cooldown_timers = false;
		}
	}
	
}

//Implementation of an ability that enables player to use more powerful bullets for a short period of time
bool PowerfulBulletsAbility(void) {
	static float timer = 0;
	static bool init_timer = false, init_cooldown_timer = false;
	Vector2 PowerfulBulletsButton = { 600*3,250*3 };

	if (special_ability[0][0] == 1) {
		DrawCircleLines(PowerfulBulletsButton.x, PowerfulBulletsButton.y, 60, MYRED);

		if (IsKeyPressed(KEY_J) && init_timer == false && init_cooldown_timer==false) {
			init_timer = true;
		}
		else if (CheckCollisionPointCircle((Vector2) { GetMousePosition().x , GetMousePosition().y  }, PowerfulBulletsButton, 60) && init_timer == false && init_cooldown_timer == false) {
			DrawCircle(PowerfulBulletsButton.x, PowerfulBulletsButton.y, 60, MYRED);
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				init_timer = true;
			}
		}

		if (init_timer) {
			timer+=1.2f*GetFrameTime()*75; //5s able to use
			DrawCircleSector(PowerfulBulletsButton, 60, 180, 180 - timer, 100, MYRED);
			if (timer >= 360) {
				init_cooldown_timer = true;
				init_timer = false;
				timer = 0;
			}
		}
		if (init_cooldown_timer) {
			timer += 0.6f*GetFrameTime()*75; //10 sec cooldown
			DrawCircleSectorLines(PowerfulBulletsButton, 60, 180, 180 - timer, 100, MYRED);
			if (timer >= 360) {
				init_cooldown_timer = false;
				timer = 0;
			}
		}
		if (restart_cooldown_timers) {
			timer = 0;
			init_cooldown_timer = false;
			init_timer = false;
		}
		return init_timer;
	}
	else {
		return false;
	}
}
