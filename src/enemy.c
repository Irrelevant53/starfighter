#include "enemy.h"

#define MYRED (Color){159,30,30,255}
#define MYRED_TRANSPARENT (Color){159,30,30,37}

Enemy *enemy[MAX_ENEMIES] = { NULL };


//Removing specific enemy from an array
void RemoveEnemy(int index) {
	if (enemy[index]) {
		free(enemy[index]);
		enemy[index] = NULL;
	}
}

//Initializing enemy data
void InitEnemy(Vector2 starting_pos, Vector2 speed,Rectangle colbox,int health, int id) {
	int found = -1;

	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemy[i] == NULL) {
			found = i;
			break;
		}
	}
	if (found >= 0) {
		enemy[found] = malloc(sizeof(Enemy));
		enemy[found]->ID = id;
		enemy[found]->starting_position = starting_pos;
		enemy[found]->speed.x = speed.x;
		enemy[found]->speed.y = speed.y;
		enemy[found]->CollisionBox = colbox;
		enemy[found]->max_health = health;
		enemy[found]->current_health = health;
		enemy[found]->damaged = false;
		enemy[found]->lifetime = 200;

		if(id>=3) {//variable size of exhaust
			enemy[found]->exhaust_animation = SetAnimation((Rectangle) { 0, 0, 420/6, 68 }, 6, 50);
		}
		else {
			enemy[found]->exhaust_animation = SetAnimation((Rectangle) { 0, 0, 35, 34 }, 6, 50);
		}
	}

}

//Moving each kind of enemy specifically
void MoveEnemy(int index,int id) {

	//helper variables with which more complex movement is possible
	static int boss1_next_move = 0, medium2_next_move = 0,medium3_next_move=0,boss2_next_move=0;
	static float sine_counter = 0.0f; 

	switch (id) {
	case 0: //small ship 1
		if (enemy[index]->starting_position.x > 1400) {
			enemy[index]->starting_position.x -= (enemy[index]->speed.x);
			enemy[index]->CollisionBox.x -= enemy[index]->speed.x;
		}
		else if (enemy[index]->starting_position.x == 1400) {
			enemy[index]->starting_position.x -= (enemy[index]->speed.x);
			enemy[index]->CollisionBox.x -= enemy[index]->speed.x;
			enemy[index]->speed.x /= 2;
		}
		else {
			if (enemy[index]->speed.x != 0) {
				enemy[index]->speed.y += 0.02f*GetFrameTime() * 100;
			}
			enemy[index]->starting_position.x -= (enemy[index]->speed.x);
			enemy[index]->CollisionBox.x -= enemy[index]->speed.x;
			enemy[index]->starting_position.y += sinf(enemy[index]->speed.y) * 2;
			enemy[index]->CollisionBox.y += sinf(enemy[index]->speed.y) * 2;
		}
		break;

	case 1: //small ship 2
		enemy[index]->starting_position.x -= (enemy[index]->speed.x);
		enemy[index]->CollisionBox.x -= enemy[index]->speed.x;
		break;

	case 2: //medium ship 1
		enemy[index]->starting_position.x -= (enemy[index]->speed.x);
		enemy[index]->CollisionBox.x -= enemy[index]->speed.x;
		break;
	case 3: //boss 1
		if (enemy[index]->starting_position.x >= 1300 && boss1_next_move==0) {
			enemy[index]->starting_position.x -= enemy[index]->speed.x;
			enemy[index]->CollisionBox.x -= enemy[index]->speed.x;
			if (enemy[index]->starting_position.x <= 1300) {
				boss1_next_move = 1;
			}
		}
		else if(boss1_next_move==1){
			enemy[index]->starting_position.x -= enemy[index]->speed.x;
			enemy[index]->CollisionBox.x -= enemy[index]->speed.x;
			enemy[index]->starting_position.y -= enemy[index]->speed.y;
			enemy[index]->CollisionBox.y -= enemy[index]->speed.y;
			if (enemy[index]->starting_position.x <= 900) {
				boss1_next_move = 2;
			}
		}
		else if (boss1_next_move == 2) {
			enemy[index]->starting_position.x += enemy[index]->speed.x;
			enemy[index]->CollisionBox.x += enemy[index]->speed.x;
			enemy[index]->starting_position.y += enemy[index]->speed.y;
			enemy[index]->CollisionBox.y += enemy[index]->speed.y;
			if (enemy[index]->starting_position.x >= 1300) {
				boss1_next_move = 3;
			}
		}
		else if (boss1_next_move == 3) {
			enemy[index]->starting_position.x -= enemy[index]->speed.x;
			enemy[index]->CollisionBox.x -= enemy[index]->speed.x;
			enemy[index]->starting_position.y += enemy[index]->speed.y;
			enemy[index]->CollisionBox.y += enemy[index]->speed.y;
			if (enemy[index]->starting_position.x <= 900) {
				boss1_next_move = 4;
			}
		}
		else if (boss1_next_move == 4) {
			enemy[index]->starting_position.x += enemy[index]->speed.x;
			enemy[index]->CollisionBox.x += enemy[index]->speed.x;
			enemy[index]->starting_position.y -= enemy[index]->speed.y;
			enemy[index]->CollisionBox.y -= enemy[index]->speed.y;
			if (enemy[index]->starting_position.x >= 1300) {
				boss1_next_move = 1;
			}
		}

		break;
	case 4: //medium ship 2
		if (enemy[index]->starting_position.x > 1450 && medium2_next_move == 0) {
			enemy[index]->starting_position.x -= enemy[index]->speed.x;
			enemy[index]->CollisionBox.x -= enemy[index]->speed.x;
		}
		else if (medium2_next_move==1) {
			sine_counter += 0.8f;

			enemy[index]->starting_position.x += (sinf(DEG2RAD*sine_counter) * 0.8f * enemy[index]->speed.x);
			enemy[index]->CollisionBox.x += (sinf(DEG2RAD*sine_counter) * 0.8f * enemy[index]->speed.x);
			enemy[index]->starting_position.y += (sinf(DEG2RAD*sine_counter/2) * 1.2f * enemy[index]->speed.y);
			enemy[index]->CollisionBox.y += (sinf(DEG2RAD*sine_counter/2) * 1.2f * enemy[index]->speed.y);
		}
		if (medium2_next_move==1 && enemy[index]->starting_position.x > 1950) { //reseting cycle for new spawn
			medium2_next_move = 0;
		}
		if (enemy[index]->starting_position.x <= 1450 && medium2_next_move == 0) {
			medium2_next_move = 1;
		}
		break;
	case 5: // medium ship 3
		if (enemy[index]->starting_position.x >= 1100 && medium3_next_move == 0) {
			enemy[index]->starting_position.x -= enemy[index]->speed.x;
			enemy[index]->CollisionBox.x -= enemy[index]->speed.x;
			if (enemy[index]->starting_position.x <= 1300) {
				medium3_next_move = 1;
			}
		}
		else if (medium3_next_move == 1) {
			enemy[index]->starting_position.y -= enemy[index]->speed.y;
			enemy[index]->CollisionBox.y -= enemy[index]->speed.y;
			if (enemy[index]->starting_position.y <= 400) {
				medium3_next_move = 2;
			}
		}
		else if (medium3_next_move == 2) {
			enemy[index]->starting_position.x += enemy[index]->speed.x;
			enemy[index]->CollisionBox.x += enemy[index]->speed.x;
			if (enemy[index]->starting_position.x >= 1400) {
				medium3_next_move = 3;
			}
		}
		else if (medium3_next_move == 3) {
			enemy[index]->starting_position.y += enemy[index]->speed.y;
			enemy[index]->CollisionBox.y += enemy[index]->speed.y;
			if (enemy[index]->starting_position.y >= 800) {
				medium3_next_move = 4;
			}
		}
		else if (medium3_next_move == 4) {
			enemy[index]->starting_position.x -= enemy[index]->speed.x;
			enemy[index]->CollisionBox.x -= enemy[index]->speed.x;
			if (enemy[index]->starting_position.x <= 1100) {
				medium3_next_move = 1;
			}
		}
		break;
	case 6: //boss 2
		if (enemy[index]->starting_position.x > 1400 && boss2_next_move == 0) {
			enemy[index]->starting_position.x -= enemy[index]->speed.x;
			enemy[index]->CollisionBox.x -= enemy[index]->speed.x;
			if (enemy[index]->starting_position.x <= 1400) {
				boss2_next_move = 1;
			}
		}
		else if (boss2_next_move == 1) {
			sine_counter += 0.8f;
			enemy[index]->starting_position.y += (sinf(DEG2RAD*sine_counter / 2) * 1.2f * enemy[index]->speed.y);
			enemy[index]->CollisionBox.y += (sinf(DEG2RAD*sine_counter / 2) * 1.2f * enemy[index]->speed.y);
		}
		if (boss2_next_move == 1 && enemy[index]->starting_position.x > 1950) { //reseting cycle for new spawn
			boss2_next_move = 0;
		}
		break;
	}

	
}

//Updating all enemies that are alive
void UpdateEnemy(void) {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemy[i]) {
			MoveEnemy(i,enemy[i]->ID);		

			if (enemy[i]->starting_position.x < -100) {
				enemy[i]->starting_position.x = 2000;
				enemy[i]->CollisionBox.x = 2000;
			}
		}
	}
}

int GetNumOfEnemies(void) {
	int num = 0;

	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemy[i]) {
			num++;
		}
	}
	return num;
}

extern Player player;
extern bool paused;

void DrawEnemy(void) {
	static bool draw_boxes;

	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemy[i]) {
			switch(enemy[i]->ID){
			case 0:
				DrawTextureRec(enemy_exhaust, (Rectangle) { enemy[i]->exhaust_animation.frameStart.x + (enemy[i]->exhaust_animation.currentFrame*enemy[i]->exhaust_animation.frameStart.width), enemy[i]->exhaust_animation.frameStart.y, enemy[i]->exhaust_animation.frameStart.width, enemy[i]->exhaust_animation.frameStart.height }, (Vector2) { enemy[i]->starting_position.x + 50, enemy[i]->starting_position.y + 45 }, WHITE);
				if (player.health > 0 && !paused) {
					UpdateAnimation(&enemy[i]->exhaust_animation);
				}
				DrawTextureV(enemy_texture1, enemy[i]->starting_position, WHITE);
				break;
			case 1:
				DrawTextureRec(enemy_exhaust, (Rectangle) { enemy[i]->exhaust_animation.frameStart.x + (enemy[i]->exhaust_animation.currentFrame*enemy[i]->exhaust_animation.frameStart.width), enemy[i]->exhaust_animation.frameStart.y, enemy[i]->exhaust_animation.frameStart.width, enemy[i]->exhaust_animation.frameStart.height }, (Vector2) { enemy[i]->starting_position.x + 50, enemy[i]->starting_position.y + 45 }, WHITE);
				if (player.health > 0 && !paused) {
					UpdateAnimation(&enemy[i]->exhaust_animation);
				}
				DrawTextureV(enemy_texture2, enemy[i]->starting_position, WHITE);
				break;
			case 2:
				DrawTextureRec(enemy_exhaust, (Rectangle) { enemy[i]->exhaust_animation.frameStart.x + (enemy[i]->exhaust_animation.currentFrame*enemy[i]->exhaust_animation.frameStart.width), enemy[i]->exhaust_animation.frameStart.y, enemy[i]->exhaust_animation.frameStart.width, enemy[i]->exhaust_animation.frameStart.height }, (Vector2) { enemy[i]->starting_position.x + 70, enemy[i]->starting_position.y + 65 }, WHITE);
				if (player.health > 0 && !paused) {
					UpdateAnimation(&enemy[i]->exhaust_animation);
				}
				DrawTextureV(enemy_texture3, enemy[i]->starting_position, WHITE);
				break;
			case 3:
				DrawTextureRec(enemy_exhaust2, (Rectangle) { enemy[i]->exhaust_animation.frameStart.x + (enemy[i]->exhaust_animation.currentFrame*enemy[i]->exhaust_animation.frameStart.width), enemy[i]->exhaust_animation.frameStart.y, enemy[i]->exhaust_animation.frameStart.width, enemy[i]->exhaust_animation.frameStart.height }, (Vector2) { enemy[i]->starting_position.x + 190, enemy[i]->starting_position.y + 90 }, WHITE);
				if (player.health > 0 && !paused) {
					UpdateAnimation(&enemy[i]->exhaust_animation);
				}
				DrawTextureV(boss1, enemy[i]->starting_position, WHITE);
				break;
			case 4:
				DrawTextureRec(enemy_exhaust2, (Rectangle) { enemy[i]->exhaust_animation.frameStart.x + (enemy[i]->exhaust_animation.currentFrame*enemy[i]->exhaust_animation.frameStart.width), enemy[i]->exhaust_animation.frameStart.y, enemy[i]->exhaust_animation.frameStart.width, enemy[i]->exhaust_animation.frameStart.height }, (Vector2) { enemy[i]->starting_position.x + 115, enemy[i]->starting_position.y + 50 }, YELLOW);
				if (player.health > 0 && !paused) {
					UpdateAnimation(&enemy[i]->exhaust_animation);
				}
				DrawTextureV(enemy_texture4, enemy[i]->starting_position, WHITE);
				break;
			case 5:
				DrawTextureRec(enemy_exhaust2, (Rectangle) { enemy[i]->exhaust_animation.frameStart.x + (enemy[i]->exhaust_animation.currentFrame*enemy[i]->exhaust_animation.frameStart.width), enemy[i]->exhaust_animation.frameStart.y, enemy[i]->exhaust_animation.frameStart.width, enemy[i]->exhaust_animation.frameStart.height }, (Vector2) { enemy[i]->starting_position.x + 115, enemy[i]->starting_position.y + 50 }, YELLOW);
				if (player.health > 0 && !paused) {
					UpdateAnimation(&enemy[i]->exhaust_animation);
				}
				DrawTextureV(enemy_texture5, enemy[i]->starting_position, WHITE);
				break;
			case 6:
				DrawTextureRec(enemy_exhaust2, (Rectangle) { enemy[i]->exhaust_animation.frameStart.x + (enemy[i]->exhaust_animation.currentFrame*enemy[i]->exhaust_animation.frameStart.width), enemy[i]->exhaust_animation.frameStart.y, enemy[i]->exhaust_animation.frameStart.width, enemy[i]->exhaust_animation.frameStart.height }, (Vector2) { enemy[i]->starting_position.x + 210, enemy[i]->starting_position.y + 120 }, YELLOW);
				if (player.health > 0 && !paused) {
					UpdateAnimation(&enemy[i]->exhaust_animation);
				}
				DrawTextureV(boss2, enemy[i]->starting_position, WHITE);
				break;

			}

			if (draw_boxes) {
				DrawRectangleLines(enemy[i]->CollisionBox.x, enemy[i]->CollisionBox.y, enemy[i]->CollisionBox.width, enemy[i]->CollisionBox.height, GREEN);
			}
		}
	}
	if (IsKeyPressed(KEY_TAB)) {
		draw_boxes = !draw_boxes;
	}
}

void DrawEnemyHealth(void) {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemy[i]) {
			if (enemy[i]->damaged) {
				if (enemy[i]->current_health > 0) {
					DrawRectangleRoundedLines((Rectangle) { enemy[i]->starting_position.x, enemy[i]->starting_position.y - 20, 100, 10 }, 0.5f, 50, 2, (Color) { 250, 240, 0, enemy[i]->lifetime });
					DrawRectangleRounded((Rectangle) { enemy[i]->starting_position.x, enemy[i]->starting_position.y - 20, 100.0f * ((float)enemy[i]->current_health / enemy[i]->max_health), 10 }, 0.5f, 50, (Color) { 250, 240, 0, enemy[i]->lifetime });
					enemy[i]->lifetime--;
					if (enemy[i]->lifetime <= 0) {
						enemy[i]->damaged = false;
					}
				}
			}
		}
	}
}
void RemoveAllEnemies(void) {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemy[i]) {
			RemoveEnemy(i);
		}
	}
}

extern float ShockWaveRadius;
extern int posX, posY;

extern Texture2D lightning;
extern Animation lightning_anim;
bool enemy_got_hit_by_shockwave[MAX_ENEMIES] = { false };

void CheckCollisionWithShockWave(void) {
	static bool shake_camera=false;
	static int timer = 0;

	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemy[i]) {
			if (CheckCollisionCircleRec((Vector2) { posX, posY }, ShockWaveRadius, enemy[i]->CollisionBox) && enemy_got_hit_by_shockwave[i]==false) { // one shockwave can hit each enemy only once
				enemy[i]->current_health -=100; 
				enemy_got_hit_by_shockwave[i] = true;
				enemy[i]->damaged = true;
				enemy[i]->lifetime = 200;
				shake_camera = true;
				
			}
		}
	}
	if (shake_camera) {
		CameraShake(&camera, 4, 4);
		timer++;
		if (timer == 50) {
			shake_camera = false;
			timer = 0;
		}
	}
}



void DrawElectricity(void) {
	static int rotation = 0;

	if (!paused) {
		rotation++;
	}
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemy[i]) {
			if (enemy[i]->speed.x == 0) {
				switch (enemy[i]->ID) {
				case 0:
				case 1:
					DrawTexturePro(lightning, (Rectangle) { lightning_anim.frameStart.x + (lightning_anim.currentFrame*lightning_anim.frameStart.width), lightning_anim.frameStart.y, lightning_anim.frameStart.width, lightning_anim.frameStart.height }, (Rectangle) { enemy[i]->starting_position.x + 30, enemy[i]->starting_position.y + 30, 200, 200 }, (Vector2) { lightning.width / 6, lightning.height / 2 }, rotation, WHITE);
					break;
				case 2:
				case 4:
				case 5:
					DrawTexturePro(lightning, (Rectangle) { lightning_anim.frameStart.x + (lightning_anim.currentFrame*lightning_anim.frameStart.width), lightning_anim.frameStart.y, lightning_anim.frameStart.width, lightning_anim.frameStart.height }, (Rectangle) { enemy[i]->starting_position.x + 50, enemy[i]->starting_position.y + 40, 200, 200 }, (Vector2) { lightning.width / 6, lightning.height / 2 }, rotation, WHITE);
					break;
				case 3:
					DrawTexturePro(lightning, (Rectangle) { lightning_anim.frameStart.x + (lightning_anim.currentFrame*lightning_anim.frameStart.width), lightning_anim.frameStart.y, lightning_anim.frameStart.width, lightning_anim.frameStart.height }, (Rectangle) { enemy[i]->starting_position.x + 100, enemy[i]->starting_position.y + 75, 200, 200 }, (Vector2) { lightning.width / 6, lightning.height / 2 }, rotation, WHITE);
					break;
				case 6:
					DrawTexturePro(lightning, (Rectangle) { lightning_anim.frameStart.x + (lightning_anim.currentFrame*lightning_anim.frameStart.width), lightning_anim.frameStart.y, lightning_anim.frameStart.width, lightning_anim.frameStart.height }, (Rectangle) { enemy[i]->starting_position.x + 250, enemy[i]->starting_position.y + 200, 200, 200 }, (Vector2) { lightning.width / 6, lightning.height / 2 }, rotation, WHITE);
					break;

				}
			}

		}
	}
	if (!paused) {
		UpdateAnimation(&lightning_anim);
	}
}


extern Texture2D disabling_line;
extern Sound disabling_laser;
extern bool restart_cooldown_timers;

void DrawDisablingLine(void) { //!!!!!!NOTE:treba resetirat brzinu enemya s obzirom na brzinu koju je imo prije!!!!!
	static bool init_timer = false, init_cooldown_timer = false;
	static float timer = 0;
	static int counter = 0;
	static bool draw_line = false;
	Vector2 LaserButton = { 600*3,300*3 };

	if (special_ability[0][1] == 1 ) {
		DrawCircleLines(LaserButton.x, LaserButton.y, 60, MYRED);

		if (IsKeyPressed(KEY_K) && draw_line == false && init_timer==false && init_cooldown_timer==false) {
			draw_line = true;
		}
		else if (CheckCollisionPointCircle((Vector2) { GetMousePosition().x,GetMousePosition().y }, LaserButton, 60) && init_timer == false && init_cooldown_timer == false) {
			DrawCircle(LaserButton.x, LaserButton.y, 60,MYRED);
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				draw_line = true;
			}
		}

		if (draw_line && init_timer==false && init_cooldown_timer==false) {
			if (!IsSoundPlaying(disabling_laser) && !paused) {
				PlaySound(disabling_laser);
			}
			DrawTexturePro(disabling_line, (Rectangle) { 0, 0, disabling_line.width, disabling_line.height }, (Rectangle) { player.position.x + 140, player.position.y + 40, disabling_line.width, disabling_line.height }, (Vector2) { 0, disabling_line.height/2 }, -RAD2DEG*tanhf((float)600/2000)+RAD2DEG*tanhf((float)(counter)/( 2000)), WHITE);
			DrawTexturePro(disabling_line, (Rectangle) { 0, 0, disabling_line.width, disabling_line.height }, (Rectangle) { player.position.x + 140, player.position.y + 40, disabling_line.width, disabling_line.height }, (Vector2) { 0, disabling_line.height / 2}, +RAD2DEG*tanhf((float)600 / 2000) -RAD2DEG*tanhf((float)(counter)/(2000)), WHITE);
			//DrawLine(player.position.x + 25, player.position.y + 10, player.position.x  + 2000, player.position.y-600+counter, YELLOW); 
			//DrawLine(player.position.x + 25, player.position.y + 10, player.position.x +2000, player.position.y+600-counter, YELLOW);
		

			for (int i = 0; i < MAX_ENEMIES; i++) {
				if (enemy[i]) {
					if (lineRect(player.position.x + 140, player.position.y + 40, player.position.x  + 2000, player.position.y-600+counter, enemy[i]->starting_position.x, enemy[i]->starting_position.y, enemy[i]->CollisionBox.width, enemy[i]->CollisionBox.height) || lineRect(player.position.x + 140, player.position.y + 40, player.position.x  + 2000, player.position.y+600- counter, enemy[i]->starting_position.x, enemy[i]->starting_position.y, enemy[i]->CollisionBox.width, enemy[i]->CollisionBox.height)) {
						enemy[i]->speed.x = 0;
						enemy[i]->speed.y = 0;
					}
				}
			}
			if (!paused) {
				counter += 15;
			}
		}
		

		if (counter >= 1000) {
			counter = 0;
			init_timer = true;
			draw_line = false;
		}

		if (init_timer && !paused) {
			timer += 1.2f; //5s able to use
			DrawCircleSector(LaserButton, 60, 180, 180 - timer, 100, MYRED);
			if (timer >= 360) {
				for (int i = 0; i < MAX_ENEMIES; i++) {
					if (enemy[i]) {
							if (enemy[i]->speed.x == 0) {
								switch (enemy[i]->ID) {
								case 0:
									enemy[i]->speed.x = 1;
									break;
								case 1:
									enemy[i]->speed.x = 1.5f;
									break;
								case 2:
									enemy[i]->speed.x = 0.8f;
									break;
								case 3:
									enemy[i]->speed.x = 2;
									enemy[i]->speed.y = 1.5f;
									break;
								case 4:
									enemy[i]->speed.x = 2;
									enemy[i]->speed.y = 1.5f;
									break;
								case 5:
									enemy[i]->speed.x = 1.75f;
									enemy[i]->speed.y = 1.75f;
									break;
								case 6:
									enemy[i]->speed.x = 1.75f;
									enemy[i]->speed.y = 1.75f;
									break;
							}
						}
					}
				}
				init_timer = false;
				init_cooldown_timer = true;
				timer = 0;
			}
		}
		if (init_cooldown_timer && !paused) {
			timer += 1.2f;
			DrawCircleSectorLines(LaserButton, 60, 180, 180 - timer, 100, MYRED);
			if (timer >= 360) {
				init_cooldown_timer = false;
				timer = 0;
			}
		}
	}
	if (restart_cooldown_timers) {
		timer = 0;
		init_cooldown_timer = false;
		init_timer = false;
		counter = 0;
		draw_line = false;
	}
	restart_cooldown_timers = false;
	
}

//formula to check wheter two lines are touching
bool lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
	float uA = ((x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));
	float uB = ((x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));

	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
		return true;
	}
	return false;
}

//formula to check wheter two rectangles are overlapping
bool lineRect(float x1, float y1, float x2, float y2, float rx, float ry, float rw, float rh) {
	bool left = lineLine(x1, y1, x2, y2, rx, ry, rx, ry + rh);
	bool right = lineLine(x1, y1, x2, y2, rx + rw, ry, rx + rw, ry + rh);
	bool top = lineLine(x1, y1, x2, y2, rx, ry, rx + rw, ry);
	bool bottom = lineLine(x1, y1, x2, y2, rx, ry + rh, rx + rw, ry + rh);

	if (left || right || top || bottom) {
		return true;
	}
	return false;
}

//Removing enemy if its health is less or equal to zero, and then also spawning coins
Vector2 medium_explosion_pos[MAX_ENEMIES] = { {0} }, boss_explosion_position = {0,0};
extern Animation medium_explosion_animation[200],boss_explosion_animation;
extern Texture2D medium_explosion,boss_explosion;
int destroy_delay[MAX_ENEMIES] = {0}; //needed in order for enemy not to get deleted imediately after started to play animation of explosion 
extern Sound medium_explosion_sound;

void CheckEnemyHealth(void) {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemy[i]) {
			if (enemy[i]->current_health <= 0) {
				destroy_delay[i]++;
				if (enemy[i]->ID == 3 || enemy[i]->ID==6) {
					boss_explosion_position = (Vector2) { enemy[i]->starting_position.x - 125, enemy[i]->starting_position.y - 75 };	
				}
				else {
					medium_explosion_pos[i] = (Vector2) { enemy[i]->starting_position.x - 75, enemy[i]->starting_position.y - 50 };
				}
				enemy[i]->speed = (Vector2) { 0.00001f, 0 }; // needed to that this animation doesnt interfere with "disabling line" one
				// Emiting specific number of coins based on what kind of enemy was destroyed
				if (destroy_delay[i] == 25) {
					switch (enemy[i]->ID) {
					case 0:
						SpawnCoins(enemy[i]->starting_position, 5);
						break;
					case 1:
						SpawnCoins(enemy[i]->starting_position, 10);
						break;
					case 2:
						SpawnCoins(enemy[i]->starting_position, 25);
						break;
					case 3:
						SpawnCoins(enemy[i]->starting_position, 100);
						break;
					case 4:
						SpawnCoins(enemy[i]->starting_position, 50);
						break;
					case 5:
						SpawnCoins(enemy[i]->starting_position, 50);
						break;
					case 6:
						SpawnCoins(enemy[i]->starting_position, 200);
						break;

					}
					if (enemy[i]->ID == 3 || enemy[i]->ID == 6) {
						SetSoundVolume(medium_explosion_sound, 2.0f);
					}
					else {
						SetSoundVolume(medium_explosion_sound, 1.0f);
					}
					PlaySound(medium_explosion_sound);
				}
				else if (destroy_delay[i] == 50) {
					RemoveEnemy(i);
					enemy_got_hit_by_shockwave[i] = false;
					destroy_delay[i] = 0;
				}
			}
		}
		if (medium_explosion_pos[i].x != 0 && medium_explosion_pos[i].y != 0) { //Explosion is done here
					CameraShake(&camera, 2, 2);
					UpdateAnimation(&medium_explosion_animation[i]);
					DrawTextureRec(medium_explosion, (Rectangle) { medium_explosion_animation[i].frameStart.x + (medium_explosion_animation[i].currentFrame*medium_explosion_animation[i].frameStart.width), medium_explosion_animation[i].frameStart.y, medium_explosion_animation[i].frameStart.width, medium_explosion_animation[i].frameStart.height }, medium_explosion_pos[i], WHITE);
					if (medium_explosion_animation[i].currentFrame == 12) {
						medium_explosion_pos[i] = (Vector2) { 0, 0 };
						ResetAnimation(&medium_explosion_animation[i]);
					}
		}
		
	}
	if (boss_explosion_position.x != 0 && boss_explosion_position.y != 0) {
		CameraShake(&camera, 5, 5);
		UpdateAnimation(&boss_explosion_animation);
		DrawTextureRec(boss_explosion, (Rectangle) { boss_explosion_animation.frameStart.x + (boss_explosion_animation.currentFrame*boss_explosion_animation.frameStart.width), boss_explosion_animation.frameStart.y, boss_explosion_animation.frameStart.width, boss_explosion_animation.frameStart.height }, boss_explosion_position, WHITE);
		if (boss_explosion_animation.currentFrame == 12) {
			boss_explosion_position = (Vector2) { 0, 0 };
			ResetAnimation(&boss_explosion_animation);
		}
	}
}
extern Texture2D start, middle, end;
extern Sound laser_beam;
int ray_length = 0;
int enemy_hit = -1;
bool disable_shooting = false;
extern Animation laser_beam_animation[3];
bool hit_ship = false;

//Implementation of laser beam used  3rd spaceship
void LaserBeam(void) {
	static float charge = 350;

	if (special_ability[2][1] == 1 ) {
		if (IsKeyDown(KEY_K) && charge>0) {
			if (!IsSoundPlaying(laser_beam)) {
				PlaySound(laser_beam);
			}
			disable_shooting = true;
			charge-=0.5f;
			for (int j = MAX_ENEMIES - 1; j >= 0; j--) {
				if (enemy[j] && enemy[j]->current_health>0) {
					if (CheckCollisionRecs((Rectangle) { player.position.x + 126, player.position.y + 17, 2000, 50 }, enemy[j]->CollisionBox)) {
						hit_ship = true;
						if (ray_length > enemy[j]->CollisionBox.x - player.position.x) {
							ray_length = enemy[j]->CollisionBox.x - player.position.x;
							enemy_hit = j;
							
						}

					}
					else if(hit_ship==false) {
						enemy_hit = -1;
					}

				}
			}
			if (charge > 1) {
				if (enemy_hit != -1) {
					if (enemy[enemy_hit]) {
						enemy[enemy_hit]->damaged = true;
						enemy[enemy_hit]->lifetime = 200;
						enemy[enemy_hit]->current_health-=2;
					}
				}
			
				if (ray_length > 100) {
					DrawTextureRec(start, (Rectangle) { laser_beam_animation[0].frameStart.x + (laser_beam_animation[0].currentFrame*laser_beam_animation[0].frameStart.width), laser_beam_animation[0].frameStart.y, laser_beam_animation[0].frameStart.width, laser_beam_animation[0].frameStart.height }, (Vector2) { player.position.x + 126, player.position.y + 18 }, WHITE);
					for (int i = 0; i < ray_length-104; i++) {
						
						DrawTextureRec(middle, (Rectangle) { laser_beam_animation[1].frameStart.x + (laser_beam_animation[1].currentFrame*laser_beam_animation[1].frameStart.width), laser_beam_animation[1].frameStart.y, laser_beam_animation[1].frameStart.width, laser_beam_animation[1].frameStart.height }, (Vector2){player.position.x + 147 + i, player.position.y + 17}, WHITE);
					}

					DrawTexture(end, player.position.x + ray_length+32, player.position.y + 17, WHITE);

				}
				else if(ray_length<100){
					DrawTexture(start, player.position.x + 126, player.position.y + 17, WHITE);
					DrawTexture(end, player.position.x + 135, player.position.y + 17, WHITE);
				}
			}
			ray_length = 2000;
			UpdateAnimation(&laser_beam_animation[0]);
			UpdateAnimation(&laser_beam_animation[1]);
		}
		else if(charge<350){
			disable_shooting = false;
			charge += 0.25f;
		}
		DrawLineEx((Vector2) { 60, 75 }, (Vector2) { 60 + charge, 75 }, 5, MYRED);

		if (restart_cooldown_timers) {
			charge = 350;
		}
	}
	
	
}

extern Flame *flame[MAX_FLAME_PARTICLES];

//Checking if enemies are getting burned by flamethrower, if so deal damage to them
void CheckCollisionFlameParticles(void) {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		for (int j = 0; j < MAX_FLAME_PARTICLES; j++) {
			if (enemy[i] && flame[j]) {
				if (CheckCollisionCircleRec((Vector2) { flame[j]->position.x + flame_texture.width / 2 * flame[j]->radius / 30.0, flame[j]->position.y + flame_texture.height / 2 * flame[j]->radius / 30.0 }, flame[j]->radius / 1.5f, enemy[i]->CollisionBox) && enemy[i]->current_health>0) {
					enemy[i]->current_health-=flame[j]->damage;
					enemy[i]->damaged = true;
					enemy[i]->lifetime = 200;
					free(flame[j]); // makes it more balanced to just remove flame particle after hitting 1 enemy
					flame[j] = NULL;
				}
			}
		}
	}
}
