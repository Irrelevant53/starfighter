#include <raylib.h>

#include "camera.h"
#include "animations.h"
#include "particles.h"
#include "player.h"
#include "camera.h"
#include "game.h"

#ifndef UI_H
#define UI_H

#define MYRED (Color){159,30,30,255}
#define MYRED_TRANSPARENT (Color){159,30,30,37}
#define DARKRED (Color){84,0,0,255}
#define RED2 (Color){141,2,31,255}

typedef struct ui_text {
	float x;
	float y;
}ui_text;

int CheckUIOption(int num);
int SwitchVehicle( ui_text *select_button);
void ProcessVehicleAbilities(int vehicleID);
void DrawFps(void);
bool CheckCollisionPointEllipse(Vector2 point,Vector2 center,int radiusH,int radiusV);
int GetDesiredLevel( float camera_offset);
int ToggleMenu(int state);
void BrightnessSetting(float *screen_brightness);
void MasterVolumeSetting(float *master_volume);
void MusicVolumeSetting(float *music_volume);
void SFXVolumeSetting(float *sfx_volume);
void LanguageSetting(int *lang); //0 for english, 1 for croatian
void vSyncSetting(bool *vSync);
void UndoSceneButton(GameScreen *currentScene);
void UpgradeVehicleStats(int currentVehicle);
void DrawEllipses(void);

#endif