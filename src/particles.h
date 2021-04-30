#include <raylib.h>

#include "raymath.h"
#include "player.h"
#include "animations.h"

#ifndef PARTICLES_H
#define PARTICLES_H

#define MAX_STARDUST 10
#define MAX_COINS 200
#define MAX_FLAME_PARTICLES 500
#define STARS_NUM 500
#define MAX_ELLIPSE_PARTICLES 50


typedef struct StarDust {
	Vector2 position;
	int velocity;
}StarDust;

typedef struct Coin {
	Vector2 position;
	Vector2 velocity;
	float lifetime;
	Animation coin_animation;
}Coin;

typedef struct Flame {
	Vector2 position;
	Vector2 velocity;
	float lifetime;
	float damage;
	float radius;
	int id;
}Flame;

typedef struct Star {
	int id;
	Vector2 position;
}Star;

typedef struct EllipseParticles{
	Vector2 position;
	float lifetime;
	float rotation;
	int size;
	Vector2 velocity;
}EllipseParticles;

void DrawStarDust(void);
void InitStarDust(void);
void SpawnCoins(Vector2 position,int num);
void ProcessCoins(int vehicleID);
void InitFlameParticles(Vector2 position);
void ProcessFlameParticles(void);
void InitStars(void);
void DrawStars(void);
void InitEllipseParticles(Vector2 position);
void DrawEllipseParticles(void);
void UnloadCoins(void);
void DrawCoins(void);

#endif
