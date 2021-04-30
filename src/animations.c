#include "animations.h"


//Setting up animation
Animation SetAnimation(Rectangle frameStart, int numberOfFrames, int frameDelay) {
	Animation anime = {
		.frameStart = frameStart,
		.numberOfFrames = numberOfFrames,
		.frameDelay = frameDelay,
		.currentFrame = 0,
		.timer = 0
	};

	return anime;
}
//Updating each animation
void UpdateAnimation(Animation * animation) {
	animation->timer += (int)(GetFrameTime() * 1000);

	if (animation->timer >= animation->frameDelay) {
		animation->currentFrame =(animation->currentFrame+1) % animation->numberOfFrames;
		animation->timer = 0;
	}
}

//Reseting animation parameters so a new one can come onto previous ones place
void ResetAnimation(Animation *animation) {
	animation->timer = 0;
	animation->currentFrame = 0;
}

int logoPositionX = 1920 / 2 - 128;
int logoPositionY = 1080 / 2 - 128;
int topSideRecWidth = 16;
int leftSideRecHeight = 16;

int bottomSideRecWidth = 16;
int rightSideRecHeight = 16;

//Default raylib animation which plays when starting the game
bool RaylibAnimation(void) { 
	static int framesCounter = 0;
	static int lettersCount = 0;
	static int state = 0;                
	static float alpha = 1.0f;

	if (state == 0)                 // State 0: Small box blinking
	{
		framesCounter++;

		if (framesCounter == 120)
		{
			state = 1;
			framesCounter = 0;      // Reset counter... will be used later...
		}
	}
	else if (state == 1)            // State 1: Top and left bars growing
	{
		topSideRecWidth += 4;
		leftSideRecHeight += 4;

		if (topSideRecWidth == 256) state = 2;
	}
	else if (state == 2)            // State 2: Bottom and right bars growing
	{
		bottomSideRecWidth += 4;
		rightSideRecHeight += 4;

		if (bottomSideRecWidth == 256) state = 3;
	}
	else if (state == 3)            // State 3: Letters appearing (one by one)
	{
		framesCounter++;

		if (framesCounter / 12)       // Every 12 frames, one more letter!
		{
			lettersCount++;
			framesCounter = 0;
		}

		if (lettersCount >= 10)     // When all letters have appeared, just fade out everything
		{
			alpha -= 0.02f;

			if (alpha <= 0.0f)
			{
				alpha = 0.0f;
				state = 4;
			}
		}
	}
	else if (state == 4)            // State 4: Reset and Replay
	{
		if (IsKeyPressed('R'))
		{
			framesCounter = 0;
			lettersCount = 0;

			topSideRecWidth = 16;
			leftSideRecHeight = 16;

			bottomSideRecWidth = 16;
			rightSideRecHeight = 16;

			alpha = 1.0f;
			state = 0;          // Return to State 0
		}
	}
	if (state == 0)
	{
		if ((framesCounter / 15) % 2) DrawRectangle(logoPositionX, logoPositionY, 16, 16, WHITE);
	}
	else if (state == 1)
	{
		DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, WHITE);
		DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, WHITE);
	}
	else if (state == 2)
	{
		DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, WHITE);
		DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, WHITE);

		DrawRectangle(logoPositionX + 240, logoPositionY, 16, rightSideRecHeight, WHITE);
		DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, WHITE);
	}
	else if (state == 3)
	{
		DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, Fade(WHITE, alpha));
		DrawRectangle(logoPositionX, logoPositionY + 16, 16, leftSideRecHeight - 32, Fade(WHITE, alpha));

		DrawRectangle(logoPositionX + 240, logoPositionY + 16, 16, rightSideRecHeight - 32, Fade(WHITE, alpha));
		DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, Fade(WHITE, alpha));

		DrawRectangle(1920 / 2 - 112, 1080 / 2 - 112, 224, 224, Fade(BLACK, alpha));

		DrawText(TextSubtext("raylib", 0, lettersCount), 1920 / 2 - 44, 1080 / 2 + 48, 50, Fade(WHITE, alpha));
	}
	if (state == 4 || IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		return true;
	}

	return false;
}

//Used for smooth fade from one scene to another
bool FadeSceneOut(void) {
	static float alpha=0.0f;
	if (GetFPS()> 60) {
		alpha += GetFrameTime();
	}
	else {
		alpha += 0.02f;
	}

	DrawRectangle(0, -2000, 1920, 3080, Fade(BLACK,alpha));

	if (alpha >= 1.0f) {
		alpha = 0.0f;
		return true;
	}

	return false;
}