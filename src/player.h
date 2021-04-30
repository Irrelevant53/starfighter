#include <raylib.h>
#include <stdlib.h>
#include <raymath.h>

#include "animations.h"
#include "easings.h"
#include "files.h"

#ifndef PLAYER_H
#define PLAYER_H

#define LOCKED (Color){75,75,75,255}
#define MYRED (Color){159,30,30,255}
#define MYRED_TRANSPARENT (Color){159,30,30,37}
#define DARKRED (Color){84,0,0,255}

#define MAX_ENEMY_BULLETS 500 // check wheter those numbers are equal to one in "bullet.h" file
#define MAX_BULLETS 250
#define MAX_ENEMIES 100


typedef struct Player {
	Vector2 position;
	Vector2 velocity;
	Vector2 CollisionBox[3];
	Animation anime[MAX_ANIMATION];
	int currentAnimation;
	int direction;
	float health;
	int health_regen;
} Player;


void PlayerMovement(int vehicleID);
void LimitMovement(void);
void SetPlayerAnimations(void);
void InitPlayer(int vehicleID);
void PlayerDash(void);
void DrawPlayer(int index);
void DrawPlayerHealth(int vehicleID);
void ShockWaveAbility(void);
bool PowerfulBulletsAbility(void);
void HealthRegen(int vehicleID);



#endif
