#include <raylib.h>

#include "files.h"
#include "enemy.h"
#include "player.h"
#include "game.h"
#include "particles.h"
#include "camera.h"


#ifndef BULLET_H
#define BULLET_H

#define MAX_BULLETS 250
#define MAX_ENEMY_BULLETS 500

typedef struct Bullet {
	Vector2 position;
	Vector2 speed;
	float radius;
	float rotation;
	int power;
	float acceleration;
	int shooting_speed; //frequency
}Bullet;

void CheckIfBulletHitsEnemy(int vehicleID);
void InitPlayerBullet(Vector2 position, Vector2 speed,int radius, int vehicleID,int power);
void InitEnemyBullet(Vector2 position, Vector2 speed,int radius, float bullet_power);
void RemoveEnemyBullet(int index);
void RemoveAllEnemyBullets(void);
void RemoveAllPlayerBullets(void);
void RemovePlayerBullet(int index);
void UpdatePlayerBullet(int vehicleID);
void UpdateEnemyBullet(void);
void UnloadBullets(void);
void PlayerShoot(int vehicleID);
void Enemy1Shoot(void);
void Enemy2Shoot(void);
void Enemy3Shoot(void);
void Medium2Shoot(void);
void Medium3Shoot(void);
void Boss1Shoot(void);
void Boss2Shoot(void);
void CheckIfBulletHitsPlayer(int vehicleID);
void ActivateShield(void);

#endif