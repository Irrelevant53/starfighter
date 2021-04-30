#include <raylib.h>

#include "easings.h"

#ifndef CAMERA_H
#define CAMERA_H

#define max(a, b) 		((a) > (b) ? (a) : (b))
#define min(a, b) 		((a) < (b) ? (a) : (b))


Camera2D SetupCamera(Camera2D *camera);
void UpdateMapCamera(Camera2D *camera);
void ResetCameraPosition(Camera2D *camera);
void CameraShake(Camera2D *camera,int x,int y);



#endif
