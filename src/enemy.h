#include <raylib.h>
#include <stdlib.h>

#include "game.h"
#include "player.h"
#include "particles.h"
#include "camera.h"

#ifndef ENEMY_H
#define ENEMY_H

#define MAX_ENEMIES 100

typedef struct Enemy {
	int ID;
	Vector2 starting_position;
	Vector2 ending_position;
	Vector2 speed;
	int max_health;
	int current_health;
	float bullet_power;
	Rectangle CollisionBox;
	bool damaged;
	float lifetime; //how long will health bar be shown for
	Animation exhaust_animation;
}Enemy;


void RemoveEnemy(int index);
void RemoveAllEnemies(void);
void InitEnemy(Vector2 starting_pos, Vector2 speed,Rectangle colbox,int health,int id);
void MoveEnemy(int index, int id);
void UpdateEnemy(void);
int GetNumOfEnemies(void);
void DrawEnemy(void);
void DrawEnemyHealth(void);
void CheckCollisionWithShockWave(void);
void DrawElectricity(void);
void DrawDisablingLine(void);
void CheckEnemyHealth(void);
void LaserBeam(void);
void CheckCollisionFlameParticles(void);
bool lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
bool lineRect(float x1, float y1, float x2, float y2, float rx, float ry, float rw, float rh);


#endif
