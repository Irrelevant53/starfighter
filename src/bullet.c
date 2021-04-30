#include "bullet.h"


Bullet *bullet[MAX_BULLETS] = { NULL };
Bullet *enemy_bullet[MAX_ENEMY_BULLETS] = { NULL };

//Initalizing bullets which player shoots
void InitPlayerBullet(Vector2 position, Vector2 speed,int radius, int vehicleID,int power) {
	int found = -1;

	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullet[i] == NULL) {
			found = i;
			break;
		}
	}
	if (found >= 0) {
		bullet[found] = malloc(sizeof(Bullet));
		bullet[found]->position.x = position.x;
		bullet[found]->position.y = position.y;
		bullet[found]->speed.y = speed.y;
		bullet[found]->speed.x = speed.x;
		bullet[found]->radius = radius;
		bullet[found]->power = (vehicle_upgrades[vehicleID][2]+1)*power;
		bullet[found]->shooting_speed = vehicle_upgrades[vehicleID][3];
		bullet[found]->acceleration = 1.0f;
	}
}

//Initalizing bullet which enemies shoot
void InitEnemyBullet(Vector2 position, Vector2 speed,int radius,float bullet_power) {
	int found = -1;

	for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
		if (enemy_bullet[i] == NULL) {
			found = i;
			break;
		}
	}
	if (found >= 0) {
		enemy_bullet[found] = malloc(sizeof(Bullet));
		enemy_bullet[found]->position.x = position.x;
		enemy_bullet[found]->position.y = position.y;	
		enemy_bullet[found]->radius = radius;
		enemy_bullet[found]->power = bullet_power;
		enemy_bullet[found]->shooting_speed = 1;

		enemy_bullet[found]->speed.y = speed.y;
		enemy_bullet[found]->speed.x = speed.x;
	
	}
}

//Removing specific player bullet
void RemovePlayerBullet(int index) {
	if (bullet[index]) {
		free(bullet[index]);
		bullet[index] = NULL;
	}
}

//Removing specific enemy bullet
void RemoveEnemyBullet(int index) {
	if (enemy_bullet[index]) {
		free(enemy_bullet[index]);
		enemy_bullet[index] = NULL;
	}
}

extern Player player;
extern Enemy *enemy[MAX_ENEMIES];
extern Camera2D camera;
extern Sound small_explosion;
extern Animation explosion1_animation[MAX_BULLETS];
extern Texture explosion[2];
Vector2 explosion_position[MAX_BULLETS] = { {0} };
int rand_explosion[MAX_BULLETS] = { 0 };

//Checking whether enemy got hit by bullet and processing some related actions
void CheckIfBulletHitsEnemy(int vehicleID) {
	for (int i = 0; i < MAX_BULLETS; i++) {
		for (int j = 0; j < MAX_ENEMIES; j++) {
			if (bullet[i] && enemy[j]) {
				if (CheckCollisionCircleRec((Vector2) { bullet[i]->position.x,bullet[i]->position.y }, bullet[i]->radius, enemy[j]->CollisionBox) && enemy[j]->current_health > 0) {
					PlaySound(small_explosion);
					explosion_position[i] = (Vector2) { bullet[i]->position.x - bullet[i]->radius * 2, bullet[i]->position.y - bullet[i]->radius * 2 };
					rand_explosion[i] = GetRandomValue(0, 1);
					enemy[j]->current_health -= bullet[i]->power;
					enemy[j]->damaged = true;
					enemy[j]->lifetime = 200; //reseting time for which enemy health is shown
					switch (vehicleID) {
					case 0:
						RemovePlayerBullet(i);
						break;
					case 1:
						CameraShake(&camera,1,1); //minigun causes shaking
						RemovePlayerBullet(i);
						break;
						//3rd vehicle's bullets can pierce through limitless number of enemies

					}
						
				}
			}
		}
	}
	//processing small explosion animation
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (explosion_position[i].x != 0 && explosion_position[i].y != 0) {
			UpdateAnimation(&explosion1_animation[i]);
			DrawTextureRec(explosion[rand_explosion[i]], (Rectangle) { explosion1_animation[i].frameStart.x + (explosion1_animation[i].currentFrame*explosion1_animation[i].frameStart.width), explosion1_animation[i].frameStart.y, explosion1_animation[i].frameStart.width, explosion1_animation[i].frameStart.height }, explosion_position[i], WHITE);
			if (explosion1_animation[i].currentFrame == 4) {
				explosion_position[i] = (Vector2) { 0, 0 };
				ResetAnimation(&explosion1_animation[i]);
			}
		}
	}


}

extern Animation explosion1_player_animation[MAX_ENEMY_BULLETS];
Vector2 explosion_player_position[MAX_ENEMY_BULLETS] = { {0} };
int rand_player_explosion[MAX_ENEMY_BULLETS] = { 0 };

//Checking wheter player got hit by bullet
void CheckIfBulletHitsPlayer(int vehicleID) {
	for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
		if (enemy_bullet[i]) {
			if (CheckCollisionCircleRec(enemy_bullet[i]->position, enemy_bullet[i]->radius, (Rectangle) { player.position.x, player.position.y+10, player.CollisionBox[vehicleID].x, player.CollisionBox[vehicleID].y })) {
				PlaySound(small_explosion);
				explosion_player_position[i] = (Vector2) { enemy_bullet[i]->position.x - enemy_bullet[i]->radius * 2, enemy_bullet[i]->position.y - enemy_bullet[i]->radius * 2 };
				rand_player_explosion[i] = GetRandomValue(0, 1);
				player.health -= enemy_bullet[i]->power;
				RemoveEnemyBullet(i);
			}

		}
	}
	//processing small explosion animation
	for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
		if (explosion_player_position[i].x != 0 && explosion_player_position[i].y != 0) {
			UpdateAnimation(&explosion1_player_animation[i]);
			DrawTextureRec(explosion[rand_player_explosion[i]], (Rectangle) { explosion1_player_animation[i].frameStart.x + (explosion1_player_animation[i].currentFrame*explosion1_player_animation[i].frameStart.width), explosion1_player_animation[i].frameStart.y, explosion1_player_animation[i].frameStart.width, explosion1_player_animation[i].frameStart.height }, explosion_player_position[i], WHITE);
			if (explosion1_player_animation[i].currentFrame == 4) {
				explosion_player_position[i] = (Vector2) { 0, 0 };
				ResetAnimation(&explosion1_player_animation[i]);
			}
		}
	}
}

//Implementation of player shooting bullets ability
extern bool disable_shooting;
int init_shooting_speed[3] = { 15,8,15 }; //shooting speed, less is faster
extern Sound machinegun,blaster,bullet1_sound,upgraded_sound;

void PlayerShoot(int vehicleID) {
	int randX,randY;
	static int counter = 0;
	bool BulletUpgrade = false;
	static bool play_blaster_sound = false;
	
	if (vehicleID == 0) {
		BulletUpgrade = PowerfulBulletsAbility();
	}
	if (((IsKeyDown(KEY_SPACE) && (counter % (init_shooting_speed[vehicleID] - vehicle_upgrades[vehicleID][3]) == 0)) || IsKeyPressed(KEY_SPACE)) && disable_shooting==false) {
		switch (vehicleID) { 
		case 0:
			
			if (BulletUpgrade) {
				PlaySound(upgraded_sound);
				InitPlayerBullet((Vector2) { player.position.x + 120, player.position.y+35 }, (Vector2) {25, 0 },10, vehicleID,4);
				InitPlayerBullet((Vector2) { player.position.x + 125, player.position.y + 42 }, (Vector2) { 25, 0 }, 10, vehicleID, 4);
			}
			else {
				PlaySound(bullet1_sound);
				InitPlayerBullet((Vector2) { player.position.x + 120, player.position.y+35 }, (Vector2) { 15, 0 }, 10, vehicleID, 1);
				InitPlayerBullet((Vector2) { player.position.x + 125, player.position.y + 42 }, (Vector2) { 15, 0 }, 10, vehicleID, 1);
			}
			break;
		case 1:
			PlaySound(machinegun);
			randX = GetRandomValue(-10, 10);
			randY = GetRandomValue(-10, 10);
			InitPlayerBullet((Vector2) { player.position.x + 145+randX, player.position.y+50+randY }, (Vector2) { 25, 0 },10, vehicleID,2);
			StopSound(blaster);
			break;
		case 2:
			play_blaster_sound = true;
			
			InitPlayerBullet((Vector2) { player.position.x + 130, player.position.y+45 }, (Vector2) { 20, 0 }, 5, vehicleID, 5);
			break;
			
		}
	}
	if (counter == 99999) { counter = 0; } 
	else { counter++; }

	if (play_blaster_sound) {
		PlaySound(blaster);
		play_blaster_sound = false;
	}
}

extern Texture2D laser[3],powerful_laser;
extern bool paused;

//Updating and drawing players bullets
void UpdatePlayerBullet(int vehicleID) {
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullet[i]) {
			if (!paused) {
				bullet[i]->position.y += bullet[i]->speed.y*bullet[i]->acceleration; // enabling acceleration where needed
				bullet[i]->position.x += bullet[i]->speed.x*bullet[i]->acceleration;
			}
			if (vehicleID == 2) { //3rd vehicles bullets have acceleration
				bullet[i]->acceleration += 0.05f;
			}

			if (vehicleID == 0) {
				if ((vehicle_upgrades[vehicleID][2] + 1)==bullet[i]->power) {
					DrawTexturePro(laser[vehicleID], (Rectangle) { 0, 0, laser[vehicleID].width, laser[vehicleID].height }, (Rectangle) { bullet[i]->position.x, bullet[i]->position.y, laser[vehicleID].width, laser[vehicleID].height }, (Vector2) { laser[1].width / 2, laser[1].height / 2 }, 90 + RAD2DEG * atanf(bullet[i]->speed.y / bullet[i]->speed.x), WHITE);
				}
				else {
					DrawTexturePro(powerful_laser, (Rectangle) { 0, 0, powerful_laser.width, powerful_laser.height }, (Rectangle) { bullet[i]->position.x, bullet[i]->position.y, powerful_laser.width, powerful_laser.height }, (Vector2) { laser[1].width / 2, laser[1].height / 2 }, 90 + RAD2DEG * atanf(bullet[i]->speed.y / bullet[i]->speed.x), WHITE);
				}
			}
			else {
				DrawTexturePro(laser[vehicleID], (Rectangle) { 0, 0, laser[vehicleID].width, laser[vehicleID].height }, (Rectangle) { bullet[i]->position.x, bullet[i]->position.y, laser[vehicleID].width, laser[vehicleID].height }, (Vector2) { laser[1].width / 2, laser[1].height / 2 }, 90+ RAD2DEG * atanf(bullet[i]->speed.y / bullet[i]->speed.x), WHITE);
			//	DrawCircleLines(bullet[i]->position.x-10, bullet[i]->position.y+20, bullet[i]->radius, WHITE);
			}

			if (bullet[i]->position.x > 2000) {
				RemovePlayerBullet(i);
			}
		}

	}
}

//Updating and drawing enemy bullets
extern Texture2D enemy_bullet_texture[3];
extern bool paused;

void UpdateEnemyBullet(void) {
	for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
		if (enemy_bullet[i]) {
			if (!paused) {
				enemy_bullet[i]->position.y += enemy_bullet[i]->speed.y;
				enemy_bullet[i]->position.x += enemy_bullet[i]->speed.x;
			}

			//Rotation calculations of each bullet is here
			switch (enemy_bullet[i]->power) {
			case 2:
				DrawTexturePro(enemy_bullet_texture[2], (Rectangle) { 0, 0, laser[1].width, laser[1].height }, (Rectangle) { enemy_bullet[i]->position.x, enemy_bullet[i]->position.y, laser[1].width, laser[1].height }, (Vector2) { laser[1].width / 2, laser[1].height / 2 }, 90 + RAD2DEG * (float)atanf(enemy_bullet[i]->speed.y / enemy_bullet[i]->speed.x), WHITE);
				break;
			case 4:
			case 5:
				DrawTexturePro(enemy_bullet_texture[1], (Rectangle) { 0, 0, laser[1].width, laser[1].height }, (Rectangle) { enemy_bullet[i]->position.x, enemy_bullet[i]->position.y, laser[1].width, laser[1].height }, (Vector2) { laser[1].width / 2, laser[1].height / 2 }, 90 + RAD2DEG * (float)atanf(enemy_bullet[i]->speed.y / enemy_bullet[i]->speed.x), WHITE);
				break;
			default:
				DrawTexturePro(enemy_bullet_texture[0], (Rectangle) { 0, 0, laser[1].width, laser[1].height }, (Rectangle) { enemy_bullet[i]->position.x, enemy_bullet[i]->position.y, laser[1].width, laser[1].height }, (Vector2) { laser[1].width / 2, laser[1].height / 2 }, 90 + RAD2DEG * (float)atanf(enemy_bullet[i]->speed.y / enemy_bullet[i]->speed.x), WHITE);
			}

			if (enemy_bullet[i]->position.y > 1100 || enemy_bullet[i]->position.y<-100 || enemy_bullet[i]->position.x>2000 || enemy_bullet[i]->position.x<-100) {
				RemoveEnemyBullet(i);
			}
		}

	}
}

//Unloading all bullets after the game was closed
void UnloadBullets(void) {
	for (int i = 0; i < MAX_BULLETS; i++) {
		free(bullet[i]);
	}
	for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
		free(enemy_bullet[i]);
	}
}

//Implementation of enemies shooting bullets ability - each kind of shooting is represented with different function
void Enemy1Shoot(void) {
	static int counter = 0;
	
	if (counter % 80==0) {
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy[i] && enemy[i]->ID==0 && enemy[i]->speed.x!=0) {
				InitEnemyBullet((Vector2) { enemy[i]->starting_position.x-5 , enemy[i]->starting_position.y + 55 }, (Vector2) {-4,0.5f},10,2);
				InitEnemyBullet((Vector2) { enemy[i]->starting_position.x-5 , enemy[i]->starting_position.y + 55 }, (Vector2) { -4, 0 }, 10, 2);
				InitEnemyBullet((Vector2) { enemy[i]->starting_position.x-5 , enemy[i]->starting_position.y + 55 }, (Vector2) { -4, -0.5f }, 10, 2);
				
			}
		}
	}
	
	if (counter == 80) {
		counter = 1;
	}
	else {
		counter++;
	}
}

void Enemy2Shoot(void) {
	static int counter = 0;

	if (counter % 60 == 0) {
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy[i] && enemy[i]->ID == 1 && enemy[i]->speed.x != 0) {
				InitEnemyBullet((Vector2) { enemy[i]->starting_position.x - 5, enemy[i]->starting_position.y + 50 }, (Vector2) { -4, 0 }, 10, 2);
				InitEnemyBullet((Vector2) { enemy[i]->starting_position.x - 5, enemy[i]->starting_position.y + 55 }, (Vector2) { -4, 0 }, 10, 2);
				InitEnemyBullet((Vector2) { enemy[i]->starting_position.x - 5, enemy[i]->starting_position.y + 60 }, (Vector2) { -4, 0}, 10, 2);

			}
		}
	}

	if (counter == 80) {
		counter = 1;
	}
	else {
		counter++;
	}
}

void Enemy3Shoot(void) {
	static int counter = 0;

	if (counter % 140 == 0) {
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy[i] && enemy[i]->ID == 2 && enemy[i]->speed.x != 0) {
				InitEnemyBullet((Vector2) { enemy[i]->starting_position.x - 10, enemy[i]->starting_position.y + 80 }, (Vector2) { -4, -2 }, 10, 4);
			}
		}
	}
	else if (counter % 142 == 0) {
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy[i] && enemy[i]->ID == 2 && enemy[i]->speed.x != 0) {
				InitEnemyBullet((Vector2) { enemy[i]->starting_position.x - 10, enemy[i]->starting_position.y + 80}, (Vector2) { -5, -1 }, 10, 4);
			}
		}
	}
	else if (counter % 144 == 0) {
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy[i] && enemy[i]->ID == 2 && enemy[i]->speed.x != 0) {
				InitEnemyBullet((Vector2) { enemy[i]->starting_position.x - 10, enemy[i]->starting_position.y + 80 }, (Vector2) { -6, 0 }, 10, 4);
			}
		}
	}
	else if (counter % 146 == 0) {
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy[i] && enemy[i]->ID == 2 && enemy[i]->speed.x != 0) {
				InitEnemyBullet((Vector2) { enemy[i]->starting_position.x - 10, enemy[i]->starting_position.y + 80 }, (Vector2) { -5, 1 }, 10, 4);
			}
		}
	}
	else if (counter % 148 == 0) {
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy[i] && enemy[i]->ID == 2 && enemy[i]->speed.x != 0) {
				InitEnemyBullet((Vector2) { enemy[i]->starting_position.x - 10, enemy[i]->starting_position.y + 80 }, (Vector2) { -4, 2 }, 10, 4);
			}
		}
	}
	else if (counter % 150 == 0) {
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy[i] && enemy[i]->ID == 2 && enemy[i]->speed.x != 0) {
				InitEnemyBullet((Vector2) { enemy[i]->starting_position.x - 10, enemy[i]->starting_position.y + 80 }, (Vector2) { -4, 2 }, 10, 4);
			}
		}
	}
	else if (counter % 152 == 0) {
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy[i] && enemy[i]->ID == 2 && enemy[i]->speed.x != 0) {
				InitEnemyBullet((Vector2) { enemy[i]->starting_position.x - 10, enemy[i]->starting_position.y + 80 }, (Vector2) { -5, 1 }, 10, 4);
			}
		}
	}
	else if (counter % 154 == 0) {
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy[i] && enemy[i]->ID == 2 && enemy[i]->speed.x != 0) {
				InitEnemyBullet((Vector2) { enemy[i]->starting_position.x - 10, enemy[i]->starting_position.y + 80 }, (Vector2) { -6, 0 }, 10, 4);
			}
		}
	}
	else if (counter % 156 == 0) {
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy[i] && enemy[i]->ID == 2 && enemy[i]->speed.x != 0) {
				InitEnemyBullet((Vector2) { enemy[i]->starting_position.x - 10, enemy[i]->starting_position.y + 80 }, (Vector2) { -5, -1 }, 10, 4);
			}
		}
	}
	else if (counter % 158 == 0) {
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy[i] && enemy[i]->ID == 2 && enemy[i]->speed.x != 0) {
				InitEnemyBullet((Vector2) { enemy[i]->starting_position.x - 10, enemy[i]->starting_position.y + 80 }, (Vector2) { -4, -2 }, 10, 4);
			}
		}
	}

	if (counter == 158) {
		counter = 1;
	}
	else {
		counter++;
	}
}

void Boss1Shoot(void) {
	static int counter = 0;
	float startX,startY,endX, endY, distance, directionX, directionY;

	if (counter % 20 == 0) {

		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy[i] && enemy[i]->ID == 3 && enemy[i]->speed.x != 0) {
				startX = enemy[i]->starting_position.x+120;
				startY = enemy[i]->starting_position.y+100;
				endX = player.position.x+60;
				endY = player.position.y+25;
				distance= sqrtf(pow(endX - startX, 2) + pow(endY - startY, 2));
				directionX=(endX - startX) / distance * 10;
				directionY = (endY - startY) / distance * 10;
				InitEnemyBullet((Vector2) { enemy[i]->starting_position.x +120, enemy[i]->starting_position.y + 100 }, (Vector2) {directionX , directionY }, 10, 8);
			}
		}
	}
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemy[i] && enemy[i]->ID == 3 && enemy[i]->speed.x != 0) {
			if (enemy[i]->starting_position.x == 1300 || enemy[i]->starting_position.x == 900) {
				for (int j = 0; j < 360; j += 20) {
					InitEnemyBullet((Vector2) { enemy[i]->starting_position.x + 120, enemy[i]->starting_position.y + 100 }, (Vector2) { cosf(DEG2RAD*j) * 5, sinf(DEG2RAD*j) * 5 }, 10, 8);
				}
			}
		}
	}
	if (counter == 20) {
		counter = 1;
	}
	else {
		counter++;
	}
}

void Medium2Shoot(void) {
	static int counter = 0,main_counter=0;
	float startX, startY, endX, endY, distance, directionX, directionY;

	if (main_counter >= 150 && main_counter <= 170) {
		if (counter % 5 == 0) {

			for (int i = 0; i < MAX_ENEMIES; i++) {
				if (enemy[i] && enemy[i]->ID == 4 && enemy[i]->speed.x != 0) {
					startX = enemy[i]->starting_position.x ;
					startY = enemy[i]->starting_position.y + 60;
					endX = player.position.x + 60;
					endY = player.position.y + 25;
					distance = sqrtf(pow(endX - startX, 2) + pow(endY - startY, 2));
					directionX = (endX - startX) / distance * 10;
					directionY = (endY - startY) / distance * 10;
					InitEnemyBullet((Vector2) { enemy[i]->starting_position.x , enemy[i]->starting_position.y + 65 }, (Vector2) { directionX, directionY }, 10, 5);
					InitEnemyBullet((Vector2) { enemy[i]->starting_position.x, enemy[i]->starting_position.y + 85 }, (Vector2) { directionX, directionY }, 10, 5);
				}
			}
		}
		if (counter == 5) {
			counter = 1;
		}
		else {
			counter++;
		}
	}
	if (main_counter >= 170) {
		main_counter = 0;
	}
	else {
		main_counter++;
	}
}

void Medium3Shoot(void) {
	static int counter = 0, main_counter = 0;
	float startX, startY, endX, endY, distance, directionX, directionY;

	if (main_counter >= 100 && main_counter <= 110) {
		if (counter % 5 == 0) {

			for (int i = 0; i < MAX_ENEMIES; i++) {
				if (enemy[i] && enemy[i]->ID == 5 && enemy[i]->speed.x != 0) {
					startX = enemy[i]->starting_position.x;
					startY = enemy[i]->starting_position.y + 60;
					endX = player.position.x + 60;
					endY = player.position.y + 25;
					distance = sqrtf(pow(endX - startX, 2) + pow(endY - startY, 2));
					directionX = (endX - startX) / distance * 10;
					directionY = (endY - startY) / distance * 10;
					InitEnemyBullet((Vector2) { enemy[i]->starting_position.x, enemy[i]->starting_position.y + 85 }, (Vector2) { directionX, directionY-0.5f }, 10, 5);
					InitEnemyBullet((Vector2) { enemy[i]->starting_position.x, enemy[i]->starting_position.y + 85 }, (Vector2) { directionX, directionY }, 10, 5);
					InitEnemyBullet((Vector2) { enemy[i]->starting_position.x, enemy[i]->starting_position.y + 85 }, (Vector2) { directionX, directionY+0.5f }, 10, 5);
				}
			}
		}
		if (counter == 5) {
			counter = 1;
		}
		else {
			counter++;
		}
	}
	if (main_counter >= 110) {
		main_counter = 0;
	}
	else {
		main_counter++;
	}
}

void Boss2Shoot(void) {
	static int counter = 0;

	if (counter % 50 == 0) {
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy[i] && enemy[i]->ID == 6 && enemy[i]->speed.x != 0) {
				for (int j = 90; j < 270; j += 10) {
					InitEnemyBullet((Vector2) { enemy[i]->starting_position.x + 120, enemy[i]->starting_position.y + 100 }, (Vector2) { cosf(DEG2RAD*j) * 5, sinf(DEG2RAD*j) * 5 }, 10, 8);
				}
			
			}
		}
	}
	if (counter % 100 == 0) {
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy[i] && enemy[i]->ID == 6 && enemy[i]->speed.x != 0) {
				for (int j = 0; j < 5; j++) {
					InitEnemyBullet((Vector2) { enemy[i]->starting_position.x, enemy[i]->starting_position.y + 100 + j * 40 }, (Vector2) { -3, 0 }, 10, 10);
				}
			}
		}
	}
	if (counter == 100) {
		counter = 1;
	}
	else {
		counter++;
	}

}
extern Texture2D shield,shield_particles;
extern Animation shield_animation,shield_particles_animation[200];
extern bool restart_cooldown_timers;
bool draw_shield = false;
Vector2 particle_position[MAX_ENEMY_BULLETS] = { {0} }; // position needed to know where to draw particle animation
Vector2 ShieldButton = { 1800,900 };

//Implementation of shield for 3rd spaceship
void ActivateShield(void) {
	
	static float timer = 0;
	static bool init_cooldown_timer = false;
	
	if (special_ability[2][0]) {
		DrawCircleLines(ShieldButton.x, ShieldButton.y, 60, MYRED);
		if (IsKeyPressed(KEY_J) && draw_shield == false && init_cooldown_timer == false) {
			draw_shield = true;;
		}
		else if (CheckCollisionPointCircle((Vector2) { GetMousePosition().x, GetMousePosition().y }, ShieldButton, 60) && draw_shield == false && init_cooldown_timer == false) {
			DrawCircle(ShieldButton.x, ShieldButton.y, 60, MYRED);
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				draw_shield= true;
			}
		}
		if (draw_shield) {
			DrawTextureRec(shield, (Rectangle) { shield_animation.frameStart.x + (shield_animation.currentFrame*shield_animation.frameStart.width), shield_animation.frameStart.y, shield_animation.frameStart.width, shield_animation.frameStart.height }, (Vector2) { player.position.x + 110, player.position.y - 20 }, WHITE);
			if (!paused) {
				UpdateAnimation(&shield_animation);
				timer += 0.6f*GetFrameTime() * 75;
			}
			DrawCircleSector(ShieldButton, 60, 180, 180 - timer, 100, MYRED);
			if (timer >= 360) {
				init_cooldown_timer = true;
				draw_shield = false;
				timer = 0;
			}

			for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
				if (enemy_bullet[i]) {
					if (CheckCollisionCircles((Vector2) { player.position.x + 110, player.position.y + 40 }, 50, enemy_bullet[i]->position, enemy_bullet[i]->radius)) {
						particle_position[i] = (Vector2) { enemy_bullet[i]->position.x-enemy_bullet[i]->radius*2,enemy_bullet[i]->position.y-enemy_bullet[i]->radius*2 };
						RemoveEnemyBullet(i);
					}
				}
			}
		}

		//processing particle animation
		for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
			if (particle_position[i].x != 0 && particle_position[i].y != 0) {
				UpdateAnimation(&shield_particles_animation[i]);
				DrawTextureRec(shield_particles, (Rectangle) { shield_particles_animation[i].frameStart.x + (shield_particles_animation[i].currentFrame*shield_particles_animation[i].frameStart.width), shield_particles_animation[i].frameStart.y, shield_particles_animation[i].frameStart.width, shield_particles_animation[i].frameStart.height }, particle_position[i], WHITE);
				if (shield_particles_animation[i].currentFrame == 3) {
					particle_position[i] = (Vector2) { 0, 0 };
					ResetAnimation(&shield_particles_animation[i]);
				}
			}
		}

		if (init_cooldown_timer && !paused) {
			timer += 1.2f*GetFrameTime() * 75; //10 sec cooldown
			DrawCircleSectorLines(ShieldButton, 60, 180, 180 - timer, 100, MYRED);
			if (timer >= 360) {
				init_cooldown_timer = false;
				timer = 0;
				draw_shield = false;
			}
		}
		if (restart_cooldown_timers) {
			timer = 0;
			init_cooldown_timer = 0;
			draw_shield = false;
			restart_cooldown_timers = false;
		}
	}
}

void RemoveAllPlayerBullets(void) {
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullet[i]) {
			RemovePlayerBullet(i);
		}
	}
}

void RemoveAllEnemyBullets(void) {
	for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
		if (enemy_bullet[i]) {
			RemoveEnemyBullet(i);
		}
	}
}