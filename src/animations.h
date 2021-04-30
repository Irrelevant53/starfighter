#include <raylib.h>

#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#define max(a, b) 		((a) > (b) ? (a) : (b))
#define min(a, b) 		((a) < (b) ? (a) : (b))

#define MAX_ANIMATION	10

typedef struct Animation {
	bool enabled;
	Rectangle frameStart;
	int numberOfFrames;
	int frameDelay;
	int currentFrame;
	int timer;
} Animation;

Animation SetAnimation(Rectangle frameStart, int numberOfFrames, int frameDelay);
void UpdateAnimation(Animation* animation);
void ResetAnimation(Animation *animation);
bool RaylibAnimation(void);
bool FadeSceneOut(void);

#endif