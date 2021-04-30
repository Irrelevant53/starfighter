#include "files.h"

//Creating file and writing some essential data in it if file doesnt exist, otherwise we are just opening the file
void CreateGameFile(const char *name) {
	if (!FileExists(name)) {
		fp = fopen(name, "w");
		fprintf(fp, "%d %d %d %d %d\n", 1, 0, 0,0,0);
		fprintf(fp, "%d %d %d %d %d\n",0,0,0,0,0);

		fprintf(fp, "\n");
		fprintf(fp, "%d %d %d\n",1,0,0); //vehicle data
		fprintf(fp, "\n\n");
		fprintf(fp, "%d\n\n",0); //money data
		for (int i = 0; i < 3; i++) { //upgrade data
			fprintf(fp, "%d %d %d %d\n",0,0,0,0);
		}
		fprintf(fp, "\n");

		for (int i = 0; i < 3; i++) {
			fprintf(fp, "%d %d\n",0,0);
		}
		fclose(fp);
		fp = fopen(name, "a+");
		fseek(fp, 0, SEEK_SET);
	}
	else {
		fp = fopen(name, "a+");
		fseek(fp, 0, SEEK_SET);
	}
}

void CreateSettingsFile(const char *name) {
	if (!FileExists(name)) {
		fp2 = fopen(name, "w");
		fprintf("%f %f %f %f\n", 200, 0.5f, 0.5f, 0.5f);
		fprintf("%d %d", 1,0);
		fclose(fp2);
		fp2 = fopen(name, "a+");
		fseek(fp2, 0, SEEK_SET);
	}
	else {
		fp2 = fopen(name, "a+");
		fseek(fp2, 0, SEEK_SET);
	}
}

//Saving current game data (game progress) into file 
void RefreshGameFileData(const char *name) {
	fclose(fp);
	fp = fopen(name, "w+");
	fprintf(fp, "%d %d %d %d %d\n", map_unlocked[0], map_unlocked[1], map_unlocked[2], map_unlocked[3], map_unlocked[4]);
	fprintf(fp, "%d %d %d %d %d\n", map_unlocked[5], map_unlocked[6], map_unlocked[7], map_unlocked[8], map_unlocked[9]);

	fprintf(fp, "\n");
	for (int i = 0; i < 3; i++) {
		fprintf(fp, "%d ", vehicle_unlocked[i]); //vehicle data
	}
	fprintf(fp, "\n\n");
	fprintf(fp, "%d\n\n", money_data); //money data

	for (int i = 0; i < 3; i++) { //upgrade data
		fprintf(fp, "%d %d %d %d\n", vehicle_upgrades[i][0], vehicle_upgrades[i][1], vehicle_upgrades[i][2], vehicle_upgrades[i][3]);
	}
	fprintf(fp,"\n");

	for (int i = 0; i < 3; i++) {
		fprintf(fp, "%d %d\n", special_ability[i][0], special_ability[i][1]);
	}

}

//Refreshing file data when needed
void RefreshSettingsFile(const char *name) {
	fclose(fp2);
	fp2 = fopen(name, "w+");
	fprintf(fp2, "%.2f %.2f %.2f %.2f\n", brightness,master_volume,music_volume,sfx_volume);
	fprintf(fp2, "%d %d", vSync_state,language);
}
