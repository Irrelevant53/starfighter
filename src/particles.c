#include "particles.h"

extern Texture2D star_dust, coin;
extern Player player;
extern bool paused;

StarDust stars[MAX_STARDUST];
Coin *coins[MAX_COINS] = { NULL };
Flame *flame[MAX_FLAME_PARTICLES] = { NULL };
Star blinking_stars[STARS_NUM];
EllipseParticles *ellipse_particles[MAX_ELLIPSE_PARTICLES] = { NULL };

//Drawing star dust
void DrawStarDust(void) {
	for (int i = 0; i < MAX_STARDUST; i++) {
		DrawTexture(star_dust, stars[i].position.x, stars[i].position.y, RAYWHITE);
		if (player.health > 0 && !paused) {
			stars[i].position.x += stars[i].velocity;
		}
		if (stars[i].position.x < -50) {
			stars[i].position.x = 2000;
			stars[i].velocity = GetRandomValue(-25*3, -30*3);
			stars[i].position.y = GetRandomValue(0, 1080);
		}
	}
}

//Initializing star dust properties
void InitStarDust(void) {
	for (int i = 0; i < MAX_STARDUST; i++) {
		stars[i].position = (Vector2) { GetRandomValue(0, 1920), GetRandomValue(0, 1080) };
		stars[i].velocity = GetRandomValue(-25*3, -30*3);
	}
}

//Inizializing stars in the distance
void InitStars(void) {
	for (int i = 0; i < STARS_NUM-100; i++) {
		blinking_stars[i].position = (Vector2) { GetRandomValue(0, 2000), GetRandomValue(0, 1080) };
		blinking_stars[i].id = GetRandomValue(1, 3);
	}
	for (int i = STARS_NUM-100; i < STARS_NUM ; i++) {
		blinking_stars[i].position = (Vector2) { GetRandomValue(0, 2000), GetRandomValue(0, 1080) };
		blinking_stars[i].id = GetRandomValue(1, 4);
	}
}

//Giving smaller stars less velocity since they are further away, and the bigger ones more  NOTE:!!!!!za ovo je potreban poseban thread ako ce se raditi, oduzima previse resursa!!!! u protivom staviti zvijezdice kao sliku
void DrawStars(void) {
	for (int i = 0; i < STARS_NUM; i++) {
		if (blinking_stars[i].id >= 2) {
			DrawCircleGradient(blinking_stars[i].position.x + blinking_stars[i].id / 2, blinking_stars[i].position.y + blinking_stars[i].id / 2, blinking_stars[i].id * 4, (Color) { 255, 255, 255, 60 }, (Color) { 0, 0, 0, 20 });
		}
		DrawRectangleV(blinking_stars[i].position, (Vector2) { blinking_stars[i].id, blinking_stars[i].id }, WHITE);
		/*if (player.health > 0 && !paused) { // Computationally too expensive
			blinking_stars[i].position.x -= blinking_stars[i].id / 100.0f;
		}
		if (blinking_stars[i].position.x < 0) {
			blinking_stars[i].position.x += 2000;
		}*/
	}

}

extern bool disable_shooting,restart_cooldown_timers;
float flame_amount = 200;
extern Sound flamethrower;
//Initializing flame particles (for flamethrower)
void InitFlameParticles(Vector2 position) {
	int found = -1;
	static int counter = 0;

	if (special_ability[1][1] == 1) {
		if (IsKeyDown(KEY_K) && flame_amount>0) {
			if (!IsSoundPlaying(flamethrower)) {
				PlaySound(flamethrower);
			}
			flame_amount-=0.3f;
			disable_shooting = true;
			for (int i = 0; i < MAX_FLAME_PARTICLES; i++) {
				if (flame[i] == NULL && flame_amount>1) {
					found = i;
					break;
				}
			}
			if (found >= 0) {
				flame[found] = malloc(sizeof(Flame));
				flame[found]->position = (Vector2) { position.x + 135, position.y + 35 };
				flame[found]->velocity = (Vector2) { 15, sinf(DEG2RAD*GetRandomValue(30, -30))*7 };
				flame[found]->lifetime = 120;
				flame[found]->damage = 1;
				flame[found]->radius = 15;
				flame[found]->id = GetRandomValue(2, 2);

			}
		}
		else if (flame_amount < 200) {
			flame_amount += 0.3f;
			disable_shooting = false;
		}
		DrawLineEx((Vector2) { 60, 75 }, (Vector2) { 60 + flame_amount, 75 }, 5, (Color) {255-flame_amount/4,flame_amount/2,0,255});

		if (restart_cooldown_timers) {
			flame_amount = 200;
		}
	}
}

extern Texture2D flame_texture;
//Drawing and moving flame particles
void ProcessFlameParticles(void) {
	for (int i = 0; i < MAX_FLAME_PARTICLES; i++) {
		if (flame[i]) {
			if (!paused) {
				flame[i]->lifetime -= 2;
				flame[i]->velocity.x -= (flame[i]->lifetime / 10000);
				flame[i]->velocity.y += GetRandomValue(-10, 10) / 100;
				flame[i]->radius += 1;
				flame[i]->position = Vector2Add(flame[i]->position, flame[i]->velocity);
			}

			
			DrawTextureEx(flame_texture, flame[i]->position, 0,  (flame[i]->radius / 30.0f), (Color) { flame[i]->lifetime * 2, flame[i]->lifetime*2 , flame[i]->lifetime*2, 255-(1/flame[i]->lifetime)*1000 });
			//DrawCircleLines(flame[i]->position.x+flame_texture.width/2* flame[i]->radius / 30.0, flame[i]->position.y + flame_texture.height / 2* flame[i]->radius / 30.0, flame[i]->radius/1.5f, WHITE);
			if (flame[i]->lifetime <= 0) {
				free(flame[i]);
				flame[i] = NULL;
			}
		}
	}
}


//Spawing desired number of coins at specific position
void SpawnCoins(Vector2 position, int num) {
	for (int j = 0; j < num; j++) {
		int found = -1;
		for (int i = 0; i < MAX_COINS; i++) {
			if (coins[i] == NULL) {
				found = i;
				break;
			}
		}
		if (found >= 0) {
			coins[found] = malloc(sizeof(Coin));
			coins[found]->position = position;
			coins[found]->velocity = (Vector2) {cosf(j+GetRandomValue(-2,2)) * 2, sinf(j+GetRandomValue(-2, 2)) * 2 };
			coins[found]->lifetime = 120;
			coins[found]->coin_animation= SetAnimation((Rectangle) { 0, 0, 140/7, 23 }, 7, GetRandomValue(50,100));

		}
	}
}

//Particles which are emitted when selecting map
void InitEllipseParticles(Vector2 position) {
	int found = -1;
	for (int i = 0; i < MAX_ELLIPSE_PARTICLES; i++) {
		if (ellipse_particles[i] == NULL) {
			found = i;
			break;
		}
	}
	if (found >= 0) {
		ellipse_particles[found] = malloc(sizeof(EllipseParticles));
		ellipse_particles[found]->position = position;
		ellipse_particles[found]->lifetime = 126;
		ellipse_particles[found]->size = GetRandomValue(2, 4);
		ellipse_particles[found]->rotation = GetRandomValue(2, 4);
		ellipse_particles[found]->velocity = (Vector2) { 0, 0.75f };

	}
}

void DrawEllipseParticles(void) {
	static int rotation[MAX_ELLIPSE_PARTICLES] = { 0 };
	for (int i = 0; i < MAX_ELLIPSE_PARTICLES; i++) {
		if (ellipse_particles[i]) {
			ellipse_particles[i]->position.y -= ellipse_particles[i]->velocity.y;
			ellipse_particles[i]->velocity.y -=0.001f;
			ellipse_particles[i]->lifetime-=2;
			DrawRectanglePro((Rectangle) { ellipse_particles[i]->position.x, ellipse_particles[i]->position.y, ellipse_particles[i]->size, ellipse_particles[i]->size }, (Vector2) { ellipse_particles[i]->size / 2, ellipse_particles[i]->size / 2 }, rotation[i], (Color) {255,255,255,ellipse_particles[i]->lifetime*2});
			rotation[i] += ellipse_particles[i]->rotation;
			if (ellipse_particles[i]->lifetime <= 0) {
				free(ellipse_particles[i]);
				ellipse_particles[i] = NULL;
			}
		}
	}
}


//Setting coin velocity (making coins spread out and follow the player afterwards)
void ProcessCoins(int vehicleID) {

	float startX[MAX_COINS], startY[MAX_COINS], endX, endY;
	float distance[MAX_COINS];
	static float directionX[MAX_COINS];
	static float directionY[MAX_COINS];

	for (int i = 0; i < MAX_COINS; i++) {
		if (coins[i]) {
			if (coins[i]->lifetime >= 70) {
				coins[i]->velocity = coins[i]->velocity;
			}
			else {
				startX[i] = coins[i]->position.x;
				startY[i] = coins[i]->position.y;
				endX = player.position.x+25;
				endY = player.position.y+10;
				distance[i] = sqrt(pow(endX - startX[i], 2) + pow(endY - startY[i], 2));
				directionX[i] = (endX - startX[i]) / distance[i] * 5* ((70-coins[i]->lifetime)/10);
				directionY[i] = (endY - startY[i]) / distance[i] * 5 * ((70 - coins[i]->lifetime) / 10);
				
				coins[i]->velocity = (Vector2) { directionX[i], directionY[i] };
			}
			coins[i]->position = Vector2Add(coins[i]->position, coins[i]->velocity);

			coins[i]->lifetime--;
			
			//Removing coins and adding them into money storage
			if (CheckCollisionPointRec(coins[i]->position, (Rectangle) { player.position.x,player.position.y,player.CollisionBox[vehicleID].x,player.CollisionBox[vehicleID].y }) || coins[i]->lifetime <= -30) {
				money_data ++;
				free(coins[i]);
				coins[i] = NULL;
			}

			
		}
	}
}

//Unloading all coins after exiting the game
void UnloadCoins(void) {
	for (int i = 0; i < MAX_COINS; i++) {
		free(coins[i]);
	}
}


//Drawing coins that exist
void DrawCoins(void) {
	for (int i = 0; i < MAX_COINS; i++) {
		if (coins[i]) {
			DrawTextureRec(coin,
				(Rectangle) {
				coins[i]->coin_animation.frameStart.x + (coins[i]->coin_animation.currentFrame *coins[i]->coin_animation.frameStart.width), coins[i]->coin_animation.frameStart.y, coins[i]->coin_animation.frameStart.width, coins[i]->coin_animation.frameStart.height},
				coins[i]->position,WHITE);

				UpdateAnimation(&coins[i]->coin_animation);
		}
	}

}