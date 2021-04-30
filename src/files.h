#define _CRT_SECURE_NO_WARNINGS

#include <raylib.h>
#include <stdio.h>


#ifndef FILES_H
#define FILES_H

FILE *fp,*fp2;

int map_unlocked[10], vehicle_unlocked[3], money_data, vehicle_upgrades[3][4],special_ability[3][2],language;
float vehicle_health, brightness, master_volume , music_volume , sfx_volume ;
bool vSync_state;

void CreateGameFile(const char *name);
void RefreshGameFileData(const char *name);
void CreateSettingsFile(const char *name);
void RefreshSettingsFile(const char *name);


#endif