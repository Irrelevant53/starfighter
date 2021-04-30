#include "camera.h"

//Camera is used when selecting desired map

//Setting up camera properties 
Camera2D SetupCamera(Camera2D *camera) {
	camera->target = (Vector2) { 0,0 };
	camera->offset = (Vector2) { 0,0  };
	camera->rotation = 0.0f;
	camera->zoom = 1.0f;
	return *camera;
}

extern bool paused;
//Updating camera movement
void UpdateMapCamera(Camera2D *camera) {
	if (!paused) {
		camera->offset.y += GetMouseWheelMove() * 75;
	}
	if (camera->offset.y >= 920) {
		camera->offset.y = 920;
	}
	else if (camera->offset.y <= 0) {
		camera->offset.y = 0;
	}

	if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && camera->offset.y<920 && !paused) {
		camera->offset.y += 25;
	}
	else if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && camera->offset.y>0 && !paused) {
		camera->offset.y -= 25;
	}
	
}

void ResetCameraPosition(Camera2D *camera) {
	camera->offset.x = 0;
	camera->offset.y = 0;
}

//Camera shake effect (used when hitting enemies and when doing similar actions)
void CameraShake(Camera2D *camera, int x ,int y) {
	static int timer = 0;
	
	camera->offset.x += GetRandomValue(-x, x);
	camera->offset.y += GetRandomValue(-y, y);
	timer++;
	if (timer %((x+y)/2+1)==0) {
		ResetCameraPosition(camera);
		timer = 0;
	}
}

