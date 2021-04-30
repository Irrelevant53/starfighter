#include "ui.h"
#include "files.h"

int selected = -1;
int vehicle_price[3] = { 1000,2000,3000};
extern int vehicle_unlocked[3];

//Implementing basic menu system
extern Texture2D main_menu_button;
float button_pos[4] = {-40.0f,-40.0f, -40.0f, -40.0f};
Sound button_click,button_click2;
extern Font myFont;

int CheckUIOption(int num) {
	static int last_checked=-1;
	
	//Main menu logic
		if (IsKeyPressed(KEY_DOWN)) {
			if (selected < num - 1) {
				PlaySound(button_click2);
			}
			selected++;
		}
		else if (IsKeyPressed(KEY_UP)) {
			if (selected > 0) {
				PlaySound(button_click2);
			}
			selected--;
		}
		if (selected >= num-1) {
			selected = num-1 ;
		}
		else if (selected < 0) {
			selected = 0;
		}

		for (int i = 0; i < num; i++) {
			if (CheckCollisionPointRec((Vector2) { GetMousePosition().x , GetMousePosition().y  }, (Rectangle) { -25,525+i*125,384,96 })) {
				if (i != last_checked) {
					PlaySound(button_click2);
					last_checked = i;
				}
				selected = i;
				if (button_pos[i] <=-10.0f) {
					button_pos[i] += 2;
				}
				
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					ClearWindowState(FLAG_VSYNC_HINT);
					SetTargetFPS(INFINITY);
					return i;
				}
			}
			else if(i!=selected) {
				if (button_pos[i] > -40.0f) {
					button_pos[i]-=2;
				}
			}

		}

		for (int i = 0; i < num; i++) {
			if (selected == i) {
				if (button_pos[i] <= -10.0f) {
					button_pos[i] += 2;
				}
				if (IsKeyPressed(KEY_ENTER)) {
					ClearWindowState(FLAG_VSYNC_HINT);
					SetTargetFPS(INFINITY);
					return i;
				}
			}
		}

		DrawTextureV(main_menu_button, (Vector2) { button_pos[0], 520 }, WHITE);
		DrawTextureV(main_menu_button, (Vector2) { button_pos[1], 640 }, WHITE);
		DrawTextureV(main_menu_button, (Vector2) { button_pos[2], 760 }, WHITE);
		DrawTextureV(main_menu_button, (Vector2) { button_pos[3], 880 }, WHITE);
	
	return -1;
}

void DrawFps() {
	DrawText(FormatText("%d", GetFPS()),  1860, 5 ,24, WHITE);
}

int button_pressed_counter=0;
extern Texture2D unlock_button, selected_button,message,spaceship_scaled[3];
float moveY[2] = { 0,0 };

//Enabling player to choose from one of the available spaceships
int SwitchVehicle( ui_text *select_button) {
	static float texture_pos[3] = { 960,960 * 3,960 * 5 };
	static float currentTime = 0,beg_texture_pos=320,sine_counter=0;
	float x;
	static int cntr = 0;
	static bool pres_right = false, pres_left = false;
	static bool increment = true;
	static float my_alpha = 1.0f;

	//keys
	if (IsKeyPressed(KEY_RIGHT) && beg_texture_pos > -3200 && pres_right == false && pres_left == false) {
		PlaySound(button_click);
		pres_left = true;
		cntr++;
		beg_texture_pos -= (960 * 2);
		button_pressed_counter = 0;
	}
	else if (IsKeyPressed(KEY_LEFT)  && beg_texture_pos <0 && pres_left == false && pres_right == false) {
		PlaySound(button_click);
		pres_right = true;
		cntr--;
		beg_texture_pos += (960 * 2);
		button_pressed_counter = 0;
	}
	//arrows (on screen)
	if (CheckCollisionPointTriangle((Vector2) { GetMousePosition().x,GetMousePosition().y }, (Vector2) { 410 * 3 - 40, 210 * 3 }, (Vector2) { 410 * 3 - 40, 230 * 3 }, (Vector2) { 430 * 3 - 40, 220 * 3 }) && beg_texture_pos >-3200 && pres_left == false && pres_right == false) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			PlaySound(button_click);
			pres_left = true;
			cntr++;
			beg_texture_pos -= (960 * 2);
			button_pressed_counter = 0;
		}
	}
	else if (CheckCollisionPointTriangle((Vector2) { GetMousePosition().x , GetMousePosition().y  }, (Vector2) { 250 * 3 - 75, 220 * 3 }, (Vector2) { 270 * 3 - 75, 230 * 3 }, (Vector2) { 270 * 3 - 75, 210 * 3 }) && beg_texture_pos <0 && pres_right == false && pres_left == false) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			PlaySound(button_click);
			pres_right = true;
			cntr--;
			beg_texture_pos += (960 * 2);
			button_pressed_counter = 0;
		}
	}

	
	if (pres_left == true) {
		currentTime += 0.4*GetFrameTime()*1000;
		if (texture_pos[cntr] >= 1920 / 2 ) {
			for (int i = 0; i < 3; i++) {

				texture_pos[i] = EaseLinearOut(currentTime, texture_pos[i], -960 * 2, 7500.0f);
				select_button[i].x = EaseLinearOut(currentTime, select_button[i].x, -960 * 2, 7500.0f);
			}
		}
		else {
			pres_left = false;
			currentTime = 0;
		}
	}
	else if (pres_right == true) {
		currentTime += 0.4*GetFrameTime()*1000;
		if (texture_pos[cntr] <= 1920 / 2) {
			for (int i = 0; i < 3; i++) {

				texture_pos[i] = EaseLinearOut(currentTime, texture_pos[i], 960 * 2, 7500.0f);
				select_button[i].x = EaseLinearOut(currentTime, select_button[i].x, 960 * 2, 7500.0f);
			}
		}
		else {
			pres_right = false;
			currentTime = 0;
		}
	}

	for (int i = 0; i < 3; i++) {
		if (CheckCollisionPointRec((Vector2) { GetMousePosition().x , GetMousePosition().y  }, (Rectangle) { select_button[i].x - 85, select_button[i].y +50, 152, 48 }) ) {
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				PlaySound(button_click2);
				if (vehicle_unlocked[cntr]) {
					button_pressed_counter++;
				}
				else if (!vehicle_unlocked[cntr] && money_data > vehicle_price[cntr]) {
					money_data -= vehicle_price[cntr];
					vehicle_unlocked[cntr] = 1;

				}
			}
		}
		else if (IsKeyPressed(KEY_ENTER)) {
			PlaySound(button_click2);
			if (vehicle_unlocked[cntr]) {
				button_pressed_counter++;
				break;
			}
			else if (!vehicle_unlocked[cntr] && money_data > vehicle_price[cntr]) {
				money_data -= vehicle_price[cntr];
				vehicle_unlocked[cntr] = 1;

			}
		}
		DrawTexture(selected_button, select_button[i].x - 85, select_button[i].y+50, WHITE);
		
	}

	// Drawing spaceship textures
	for (int i = 0; i < 3; i++) {
		if (vehicle_unlocked[i] == 1) {
			DrawTexture(spaceship_scaled[i], texture_pos[i] - 125, 575-sinf(sine_counter)*25, WHITE);
		}
		else {
			DrawTexture(spaceship_scaled[i], texture_pos[i] - 125, 600, LOCKED);

		}
	}
	sine_counter += GetFrameTime();
	DrawTextureRec(coin, (Rectangle) { 60, 0, 19, 24 }, (Vector2) { 890, 25 }, WHITE);



	DrawTriangle((Vector2) { 410 * 3-40, 210 * 3 }, (Vector2) { 410 * 3-40, 230 * 3 }, (Vector2) { 430 * 3-40, 220 * 3 }, MYRED);
	DrawTriangle((Vector2) { 250 * 3-75, 220 * 3 }, (Vector2) { 270 * 3-75, 230 * 3 }, (Vector2) { 270 * 3-75, 210 * 3 }, MYRED);

	DrawRectangleLinesEx((Rectangle) { 15, 60, 315, (175 + moveY[1]) * 3 }, 1, MYRED);

	//Drawing text
	if (language == 0) {
		DrawTextEx(myFont, "Special Abilities", (Vector2) { 40, 85 }, 36, 0, MYRED);
	}
	else {
		DrawTextEx(myFont, "Posebne sposobnosti", (Vector2) { 20, 85 }, 36, 0, MYRED);
	}
	for (int i = 0; i < 3; i++) {
		if (vehicle_unlocked[i] == 1) {
			if (button_pressed_counter == 0) {
				if (language == 0) {
					DrawTextEx(myFont, "Select", (Vector2) { texture_pos[i] -38, 870 }, 36, 0, MYRED);
				}
				else {
					DrawTextEx(myFont, "Odaberi", (Vector2) { texture_pos[i] -38, 870 }, 36, 0, MYRED);
				}
			}
			else if (button_pressed_counter == 1) {
				if (language == 0) {
					DrawTextEx(myFont, "Selected", (Vector2) { texture_pos[i] -55, 870 }, 36, 0, MYRED);
				}
				else {
					DrawTextEx(myFont, "Odabrano", (Vector2) { texture_pos[i] -55, 870 }, 36, 0, MYRED);
				}
			}
		}
		else {
			if (language == 0) {
				DrawTextEx(myFont, "Unlock", (Vector2) { texture_pos[i] -38, 870 }, 36, 0, MYRED);
			}
			else {
				DrawTextEx(myFont, "Otkljucaj", (Vector2) { texture_pos[i] -38, 870 }, 36, 0, MYRED);
			}
		}


	}
	if (button_pressed_counter == 1) {
		if (language == 0) {
			DrawTextEx(myFont, "Press Enter to continue", (Vector2) { GetScreenWidth() / 2 - 175, 1000 }, 36, 1, Fade(MYRED, my_alpha));
		}
		else {
			DrawTextEx(myFont, "Pritisni enter za nastavak", (Vector2) { GetScreenWidth() / 2 - 175, 1000 }, 36, 1, Fade(MYRED, my_alpha));
		}

		if (my_alpha == 1) {
			increment = false;
		}
		else if (my_alpha <= 0.2f) {
			increment = true;
		}

		if (increment) {
			my_alpha += 0.0001f;
		}
		else {
			my_alpha -= 0.0001f;
		}
	}
	else if (button_pressed_counter == 0) {
		if (language == 0) {
			DrawTextEx(myFont, "Choose your vehicle", (Vector2) { GetScreenWidth() / 2 - 150, 1000 }, 36, 1, Fade(MYRED, my_alpha));
		}
		else {
			DrawTextEx(myFont, "Odaberi svoje vozilo", (Vector2) { GetScreenWidth() / 2 - 150, 1000 }, 36, 1, Fade(MYRED, my_alpha));
		}

		if (my_alpha == 1) {
			increment = false;
		}
		else if (my_alpha <= 0.2f) {
			increment = true;
		}

		if (increment) {
			my_alpha += 0.0001f;
		}
		else {
			my_alpha -= 0.0001f;
		}
	}
	return cntr;
}


extern Texture mystery_ability,coin;

extern Texture2D special,abilities[3][2];
extern Texture2D locked_ability;


char *ability_name[3][2] = { {"  Power","Laser rays"}
							,{"Shockwave","Flamethrower"}
							,{" Shield","Laser beam"}
};
char *naziv_sposobnosti[3][2] = { {"  Snaga","Zrake lasera"}
							,{"Udarni val","Bacac plamena"}
							,{"  Stit","  Laser"}
};

char *ability_info[3][2] = { {"\nEnable devastating bullets\nfor a short period of time","\nDisable enemy units with\n two special laser rays"},
							{"\nWipe out enemy units with\n  powerful shockwave","\nUse this ability to easily\nburn your enemies to ashes"},
							{"\nSave your ship from\nincoming projectiles","\nBlast everything with this\n  brutal piece of tech"}
};
char *info_sposobnosti[3][2] = { {"\nLansiraj razorne metke\nna kratak period vremena","\nOnesposobi protivnike pomocu\n dviju snazne laserske zrake"},
							{"\nIzbrisi protivnike pomocu\n  snaznog udarnog vala","\nIskoristi ovu sposobnost za\npretvaranje protivnika u pepeo"},
							{"\nSacuvaj svoje vozilo od\nnadolazecih projektila","\nRazori sve protivnike\ns ovim cudom tehnologije"}
};

Color ability_color[3][2] = { {{255,0,0,255},{102,191,255,255}},{{200,100,8,255},{255,203,0,255}},{{255,100,0,255},{230,41,55,255}} };
char *display_ability_price[3][2] = { {"50000","50000"},{"50000","50000"},{"50000","50000"} };
int abilityPrice[4][2] = { {50000,50000},{50000,50000},{50000,50000},{50000,50000} };

//Purchasing and getting info about special abilities
void ProcessVehicleAbilities(int vehicleID) {
	static bool open_menu1 = false, open_menu2 = false;


	//menu
	if (CheckCollisionPointRec((Vector2) { GetMousePosition().x, GetMousePosition().y }, (Rectangle) { 43 * 3, (85 + moveY[0]) * 3 + 1, 50, 20 })) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			open_menu1 = !open_menu1;

		}
	}
	if (CheckCollisionPointRec((Vector2) { GetMousePosition().x, GetMousePosition().y }, (Rectangle) { 43 * 3, (155 + moveY[1]) * 3 + 1, 50, 20 })) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			open_menu2 = !open_menu2;

		}
	}

	if (open_menu1 && moveY[0] < 43) {
		moveY[0] += 0.04f * 2000 * GetFrameTime();
		moveY[1] += 0.04f * 2000 * GetFrameTime();
	}
	else if (open_menu1 == false && moveY[0] > 0) {
		moveY[0] -= 0.04f * 2000 * GetFrameTime();
		moveY[1] -= 0.04f * 2000 * GetFrameTime();
	}
	if (open_menu2  && moveY[1] < 43 + moveY[0]) {
		moveY[1] += 0.04f * 2000 * GetFrameTime();
	}
	else if (open_menu2 == false && moveY[1] > 0 + moveY[0]) {
		moveY[1] -= 0.04f * 2000 * GetFrameTime();
	}

	DrawRectangleLines(43 * 3, (85 + moveY[0]) * 3 + 1, 50, 20, WHITE);
	DrawRectangleLines(43 * 3, (155 + moveY[1]) * 3 + 1, 50, 20, WHITE);
	DrawTriangleLines((Vector2) { 43 * 3 + 15, (85 + moveY[0]) * 3 + 5 }, (Vector2) { 43 * 3 + 35, (85 + moveY[0]) * 3 + 5 }, (Vector2) { 43 * 3 + 25, (85 + moveY[0]) * 3 + 20 }, WHITE);
	DrawTriangleLines((Vector2) { 43 * 3 + 15, (155 + moveY[1]) * 3 + 5 }, (Vector2) { 43 * 3 + 35, (155 + moveY[1]) * 3 + 5 }, (Vector2) { 43 * 3 + 25, (155 + moveY[1]) * 3 + 20 }, WHITE);

	//Vehicle special abilities
	for (int i = 0; i < 2; i++) {
		if (i == 1) {

			if (special_ability[vehicleID][i] == 1 ) {
				if (moveY[1]-moveY[0] >= 35) {
					if (language == 0) {
						DrawTextEx(myFont, ability_name[vehicleID][i], (Vector2) { 25 * 3 + 40, (42 + moveY[1] + i * 70 + 2) * 3 + 10 }, 18, 0, ability_color[vehicleID][i]);
						DrawTextEx(myFont, ability_info[vehicleID][i], (Vector2) { 25 * 3, (42 + moveY[1] + i * 70 + 2) * 3 + 30 }, 18, 0, WHITE);
					}
					else {
						DrawTextEx(myFont, naziv_sposobnosti[vehicleID][i], (Vector2) { 25 * 3 + 40, (42 + moveY[1] + i * 70 + 2) * 3 + 10 }, 18, 0, ability_color[vehicleID][i]);
						DrawTextEx(myFont, info_sposobnosti[vehicleID][i], (Vector2) { 25 * 3, (42 + moveY[1] + i * 70 + 2) * 3 + 30 }, 18, 0, WHITE);
					}
				}
			}
			else {
				if (moveY[1] - moveY[0] >= 35) {
					if (language == 0) {
						DrawTextEx(myFont, "Unlock", (Vector2) { 40 * 3, (80 + moveY[1] + i * 65) * 3 }, 24, 0, WHITE);
					}
					else {
						DrawTextEx(myFont, "Otkljucaj", (Vector2) { 40 * 3, (80 + moveY[1] + i * 65) * 3 }, 24, 0, WHITE);
					}
				}
				DrawRectangleLines(40 * 3 - 5, (80 + moveY[1] + i * 65) * 3 - 10, 80, 35, MYRED);
				DrawRectangleLines(30 * 3, (41 + moveY[1] + i * 70) * 3, 45 * 3, 44 * 3, MYRED); //texture
				DrawTextEx(myFont, display_ability_price[vehicleID][i], (Vector2) { 40 * 3 - 10, (80 + moveY[1] + i * 65) * 3 - 50 }, 24, 0, WHITE);
				DrawTextureRec(coin, (Rectangle) { 60, 0, 19, 24 }, (Vector2) { 40 * 3 + 60, (80 + moveY[1] + i * 65) * 3 - 55 }, WHITE);

				//button
				if (CheckCollisionPointRec((Vector2) { GetMousePosition().x, GetMousePosition().y }, (Rectangle) { 40 * 3 - 5, (80 + moveY[1] + i * 65) * 3 - 10, 80, 35 }) && vehicle_unlocked[vehicleID]) {
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && money_data >= abilityPrice[vehicleID][i] && moveY[1] >= 43) {
						special_ability[vehicleID][i] = 1;
						money_data -= abilityPrice[vehicleID][i];
					}
				}
			}
		}
		else {
			if (special_ability[vehicleID][i] == 1) {
				if (moveY[0] >= 35) {
					if (language == 0) {
						DrawTextEx(myFont, ability_name[vehicleID][i], (Vector2) { 25 * 3 + 40, (42 + moveY[0] + 2) * 3 + 10 }, 18, 0, ability_color[vehicleID][i]);
						DrawTextEx(myFont, ability_info[vehicleID][i], (Vector2) { 25 * 3, (42 + moveY[0] + 2) * 3 + 30 }, 18, 0, WHITE);
					}
					else {
						DrawTextEx(myFont, naziv_sposobnosti[vehicleID][i], (Vector2) { 25 * 3 + 40, (42 + moveY[0] + 2) * 3 + 10 }, 18, 0, ability_color[vehicleID][i]);
						DrawTextEx(myFont, info_sposobnosti[vehicleID][i], (Vector2) { 25 * 3, (42 + moveY[0] + 2) * 3 + 30 }, 18, 0, WHITE);
					}
				}
			}
			else {
				if (moveY[0] >= 35) {
					if (language == 0) {
						DrawTextEx(myFont, "Unlock", (Vector2) { 40 * 3, (75 + moveY[0]) * 3 }, 24, 0, WHITE);
					}
					else {
						DrawTextEx(myFont, "Otkljucaj", (Vector2) { 40 * 3, (75 + moveY[0]) * 3 }, 24, 0, WHITE);
					}
				}
				DrawRectangleLines(40 * 3 - 5, (75 + moveY[0]) * 3 - 10, 80, 35, MYRED);
				DrawRectangleLines(30 * 3, (41 + moveY[0]) * 3, 45 * 3, 44 * 3, MYRED); //texture
				DrawTextEx(myFont, display_ability_price[vehicleID][i], (Vector2) { 40 * 3 - 10, (75 + moveY[0]) * 3 - 50 }, 24, 0, WHITE);
				DrawTextureRec(coin, (Rectangle) { 60, 0, 19, 24 }, (Vector2) { 40 * 3 + 60, (75 + moveY[0]) * 3 - 55 }, WHITE);


				//button
				if (CheckCollisionPointRec((Vector2) { GetMousePosition().x, GetMousePosition().y }, (Rectangle) { 40 * 3 - 5, (75 + moveY[0]) * 3 - 10, 80, 35 }) && vehicle_unlocked[vehicleID]) {
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && money_data >= abilityPrice[vehicleID][i] && moveY[0] > 0) {
						special_ability[vehicleID][i] = 1;
						money_data -= abilityPrice[vehicleID][i];
					}
				}
			}
			//DrawRectangle(25 * 3, 40 * 3, 70 * 3 + 15, 45 * 3, BLACK);
		}
	}

	for (int i = 0; i < 2; i++) {
		
		if (special_ability[vehicleID][i] == 1) {
			if (i == 1) {
				DrawTextureEx(abilities[vehicleID][i], (Vector2) { 30 * 3, (40 + i * 70 + moveY[0]) * 3 }, 0, 3.0f, WHITE);
				DrawRectangleLines(90, (40 + i * 70+moveY[0]) * 3, 135, 135, MYRED);
			}
			else {
				DrawRectangleLines(90, (40 + i * 70) * 3, 135, 135, MYRED);
				if (vehicleID == 1) {
					DrawTextureEx(abilities[vehicleID][i], (Vector2) { 30 * 3, (40 + i * 70) * 3 }, 0, 3.0f, WHITE);
				}
				else if (vehicleID == 0) {
					DrawTextureEx(abilities[vehicleID][i], (Vector2) { 200, (40 + i * 70) * 4 }, 90, 3.0f, WHITE);
				}
				else {
					DrawTextureEx(abilities[vehicleID][i], (Vector2) { 30 * 4, (40 + i * 70) * 3+15 }, 0, 1.0f, WHITE);
				}
			}
		}
		else {
			if (i == 1) {
				DrawRectangleLines(90, (40 + i * 70 + moveY[0]) * 3, 135, 135, MYRED);
				DrawTextureEx(mystery_ability, (Vector2) { 30 * 3, (40 + i * 70 + moveY[0]) * 3 }, 0, 3.0f, WHITE);
			}
			else {
				DrawRectangleLines(90, (40 + i * 70 ) * 3, 135, 135, MYRED);
				DrawTextureEx(mystery_ability, (Vector2) { 30 * 3, (40 + i * 70) * 3 }, 0, 3.0f, WHITE);
			}
		}
	}
}	

bool CheckCollisionPointEllipse(Vector2 point, Vector2 center, int radiusH, int radiusV) {
	if (((point.x - center.x)*(point.x - center.x) / (radiusH*radiusH)) + ((point.y - center.y)*(point.y - center.y)) / (radiusV*radiusV) < 1) {
		return true;
	}
	return false;
}

extern int map_unlocked[10];
extern Texture2D level_beam,ellipse;
Color ellipse_color[10];
float add_x_movement[10] = {0.006f,0.007f,0.008f,0.009f,0.010f,0.011f,0.012f,0.013f,0.014f,0.015f};
int reward[10] = { 5000,7500,10000,12000,15000,20000,25000,30000,40000,50000 };
extern Animation level_beam_animation;
const char *levels[10] = { " Battle of Brenn","Skratos playground" ,"Furious Battlefield" ,"  Unstable Space" ,"Battle of Sector 67" ,"Level 6" ,"Level 7" ,"Level 8" ,"Level 9" ,"Level 10" };
bool toggle = false;

//Map selection is implemeted here
int GetDesiredLevel(float camera_offset) { 
	static int times_checked = 0, index = 0;
	static float ellipse_x_movement[10] = { 0 }, ellipse_x_pos[10];
	static bool ellipse_first[10];

	for (int i = 0; i < 10; i++) {
		ellipse_x_movement[i] += add_x_movement[i];
	}

	for (int i = 0; i < 10; i++) {
		ellipse_x_pos[i] = GetScreenWidth() / 2 + sinf(ellipse_x_movement[i]) * 400;

		if (sinf(ellipse_x_movement[i]) >= 0.5f) {
			ellipse_first[i] = false;
		}
		else if (sinf(ellipse_x_movement[i]) <= -0.5f) {
			ellipse_first[i] = true;
		}
		if (map_unlocked[i]) {
			if (CheckCollisionPointEllipse(GetMousePosition(), (Vector2) { ellipse_x_pos[i], 1000 + cosf(ellipse_x_movement[i])*-40 - i * 200 + camera_offset }, 60, 20)) {
				ellipse_color[i] = SKYBLUE;

				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					if (i == index) {
						times_checked++;
					}
					else {
						times_checked = 1;
						index = 0;
					}
					index = i;
					if (times_checked >= 2) {
						times_checked = 0;
						return index;
					}
				}
			}
			else {
				ellipse_color[i] = VIOLET;
			}
			EndMode2D();
			if (times_checked == 1 && !toggle) {
				DrawRectangleLines(10, 55, 300, 450,MYRED);
				DrawTextEx(myFont, levels[index], (Vector2) { 40, 95 }, 30,0, MYRED);
				DrawRectangleLines(20, 125, 280, 150, MYRED);
				DrawTextEx(myFont, "IMAGE...", (Vector2) { 90, 180 }, 48, 1, GRAY);
				if (language == 0) {
					DrawTextEx(myFont, FormatText("After beating this level you \n   will be awarded with:\n\n           %d", reward[index]), (Vector2) { 30, 295 }, 18, 0, WHITE);
				}
				else {
					DrawTextEx(myFont, FormatText("Nagrada nakon prelaska ovog \n         nivoa:\n\n           %d", reward[index]), (Vector2) { 30, 295 }, 18, 0, WHITE);
				}
				DrawTextureRec(coin, (Rectangle) { 60, 0, 19, 24 }, (Vector2) { 100, 365 }, WHITE);
				ellipse_color[index] = WHITE;
				if (IsKeyPressed(KEY_ENTER)) {
					return index;
				}
			}
			BeginMode2D(camera);
		}
		else {
			ellipse_color[i] = RED;
		}
		if (times_checked == 1) {
			InitEllipseParticles((Vector2) { ellipse_x_pos[index] + (GetRandomValue(-60, 60)), 1000 + cosf(ellipse_x_movement[index])*-40 - index * 200 - ellipse.height / 2 + GetRandomValue(-20, 20) });
			DrawEllipseParticles();
		}
	}

		for (int i = 0; i < 10; i++) {
			if (ellipse_first[i]) {
				DrawTexture(ellipse, ellipse_x_pos[i] - ellipse.width / 2, 1000 + cosf(ellipse_x_movement[i])*-40 - i * 200 - ellipse.height / 2, ellipse_color[i]);
				DrawEllipseLines(ellipse_x_pos[i], 1000 + cosf(ellipse_x_movement[i])*-40-i*200, 60, 20, ellipse_color[i]);
			}
		}
		
		for (int i = 0; i< 2000; i++) {
			DrawTextureRec(level_beam, (Rectangle) { level_beam_animation.frameStart.x , level_beam_animation.frameStart.y + (level_beam_animation.currentFrame*level_beam_animation.frameStart.height), level_beam_animation.frameStart.width, level_beam_animation.frameStart.height }, (Vector2) { GetScreenWidth() / 2, 1080 - i }, WHITE);
		}
		UpdateAnimation(&level_beam_animation);
		for (int i = 0; i < 10; i++) {
			if (!ellipse_first[i]) {
				DrawTexture(ellipse, ellipse_x_pos[i]-ellipse.width/2, 1000 + cosf(ellipse_x_movement[i])*-40 - i * 200-ellipse.height/2, ellipse_color[i]);
				DrawEllipseLines(ellipse_x_pos[i],1000 + cosf(ellipse_x_movement[i])*-40 - i * 200, 60, 20, ellipse_color[i]);
			}
		}
		
	
		return -1;

}

extern Player player;
extern bool paused;
extern Camera2D camera;
extern Music background_music;
int current_choice = 0;


//Each state represents slightly modified drop menu
int ToggleMenu(int state) { 
	static int num = 0,last_checked=0;
	static float  moveY = 0;
	static bool entered=false;

	if ( IsKeyPressed(KEY_P)) {
		toggle = !toggle;
	}
	else if (IsKeyPressed(KEY_ESCAPE)) {
		toggle = !toggle;
	}
	if (player.health <= 0 && state==2) {
		toggle = true;
	}

	if ((state >= 6) && entered==false) { //After passing each map, enables automatic opening of a menu
		toggle = true;
		entered = true;
	}


	if (moveY < 700 && toggle) {
		moveY += 700;
	}
	else if (moveY > 0 && toggle == false) {
		moveY -= 700;
	}
	if (toggle) {
		camera.offset.y = 0;
		paused = true;
		DrawRectangle(0, 0, 1920, 1080, (Color) { 0, 0, 0, 230 });
	}
	else {
		paused = false;
	}
	if (IsKeyPressed(KEY_DOWN) && toggle) {
		if (current_choice< 3) {
			PlaySound(button_click2);
		}
		current_choice++;
	}
	else if (IsKeyPressed(KEY_UP) && toggle) {
		if (current_choice > 0) {
			PlaySound(button_click2);
		}
		current_choice--;
	}
	if (current_choice >= num-1) {
		current_choice = num-1;
	}
	else if (current_choice < 0) {
		current_choice = 0;
	}
	if (moveY > 0) {
		DrawRectangle(580, -375 + moveY + current_choice * 75, 860, 75, MYRED);
		DrawRectangleLinesEx((Rectangle) { 580, -600 + moveY, 860, 550 },4, MYRED);
		DrawLineEx((Vector2) { 580, -450 + moveY }, (Vector2) { 1440, -450 + moveY },4, MYRED);

		if (state == 2) { //'Game over' state
			if (language == 0) {
				DrawTextEx(myFont, "You got destroyed!", (Vector2) { 825, -500 + moveY }, 48, 0, RED);
				DrawTextEx(myFont, "    Retry", (Vector2) { 890, -350 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "   Settings", (Vector2) { 890, -275 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "  Main Menu", (Vector2) { 900, -200 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "  Exit Game", (Vector2) { 900, -125 + moveY }, 36, 0, WHITE);
			}
			else {
				DrawTextEx(myFont, "Vozilo je unisteno!", (Vector2) { 825, -500 + moveY }, 48, 0, RED);
				DrawTextEx(myFont, "Igraj ponovno", (Vector2) { 890, -350 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "   Postavke", (Vector2) { 890, -275 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, " Glavni menu", (Vector2) { 900, -200 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "    Izlaz", (Vector2) { 900, -125 + moveY }, 36, 0, WHITE);
			}
			num = 4;
		}
		else if (state == 1) {//Game paused state
			PauseMusicStream(background_music);
			if (language == 0) {
				DrawTextEx(myFont, "Game paused", (Vector2) { 890, -500 + moveY }, 48, 0, RED);
				DrawTextEx(myFont, "   Resume", (Vector2) { 900, -350 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "  Main Menu", (Vector2) { 900, -275 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "  Settings", (Vector2) { 910, -200 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "  Exit Game", (Vector2) { 900, -125 + moveY }, 36, 0, WHITE);
			}
			else {
				DrawTextEx(myFont, "Igra pauzirana", (Vector2) { 870, -500 + moveY }, 48, 0, RED);
				DrawTextEx(myFont, "   Nastavi", (Vector2) { 900, -350 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, " Glavni menu", (Vector2) { 900, -275 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "  Postavke", (Vector2) { 910, -200 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "    Izlaz", (Vector2) { 900, -125 + moveY }, 36, 0, WHITE);
			}
			num = 4;
		}
		else if (state == 0) {//About state
			if (language == 0) {
				DrawTextEx(myFont, "  About", (Vector2) { 910, -500 + moveY }, 48, 0, RED);
				DrawTextEx(myFont, "  Main Menu", (Vector2) { 900, -350 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "  Settings", (Vector2) { 910, -275 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "  Exit Game", (Vector2) { 900, -200 + moveY }, 36, 0, WHITE);
			}
			else {
				DrawTextEx(myFont, "  Info", (Vector2) { 910, -500 + moveY }, 48, 0, RED);
				DrawTextEx(myFont, " Glavni Menu", (Vector2) { 900, -350 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "  Postavke", (Vector2) { 910, -275 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "   Izlaz", (Vector2) { 900, -200 + moveY }, 36, 0, WHITE);
			}
			num = 3;
		}
		else if (state == 3) {//Settings state
			if (language == 0) {
				DrawTextEx(myFont, " Settings", (Vector2) { 900, -500 + moveY }, 48, 0, RED);
				DrawTextEx(myFont, "  Main Menu", (Vector2) { 900, -350 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "    About", (Vector2) { 900, -275 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "  Exit Game", (Vector2) { 900, -200 + moveY }, 36, 0, WHITE);
			}
			else {
				DrawTextEx(myFont, " Postavke", (Vector2) { 900, -500 + moveY }, 48, 0, RED);
				DrawTextEx(myFont, " Glavni menu", (Vector2) { 900, -350 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "    Info", (Vector2) { 900, -275 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "    Izlaz", (Vector2) { 900, -200 + moveY }, 36, 0, WHITE);
			}
			num = 3;
		}
		else if (state == 4) {// Vehicle selection state
			if (language == 0) {
				DrawTextEx(myFont, "Vehicle selection", (Vector2) { 800, -500 + moveY }, 48, 8, RED);
				DrawTextEx(myFont, "  Main Menu", (Vector2) { 900, -350 + moveY }, 36, 4, WHITE);
				DrawTextEx(myFont, "    About", (Vector2) { 900, -275 + moveY }, 36, 4, WHITE);
				DrawTextEx(myFont, "  Exit Game", (Vector2) { 900, -200 + moveY }, 36, 4, WHITE);
			}
			else {
				DrawTextEx(myFont, " Odabir vozila", (Vector2) { 800, -500 + moveY }, 48, 8, RED);
				DrawTextEx(myFont, " Glavni menu", (Vector2) { 900, -350 + moveY }, 36, 4, WHITE);
				DrawTextEx(myFont, "    Info", (Vector2) { 900, -275 + moveY }, 36, 4, WHITE);
				DrawTextEx(myFont, "    Izlaz", (Vector2) { 890, -200 + moveY }, 36, 4, WHITE);
			}
			num = 3;
		}
		else if (state == 5) {//Map selection state
			if (language == 0) {
				DrawTextEx(myFont, "Level selection", (Vector2) { 850, -500 + moveY }, 48, 0, RED);
				DrawTextEx(myFont, "  Main Menu", (Vector2) { 900, -350 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "  Settings", (Vector2) { 910, -275 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "  Exit Game", (Vector2) { 900, -200 + moveY }, 36, 0, WHITE);
			}
			else {
				DrawTextEx(myFont, " Odabir nivoa", (Vector2) { 850, -500 + moveY }, 48, 0, RED);
				DrawTextEx(myFont, " Glavni menu", (Vector2) { 900, -350 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "  Postavke", (Vector2) { 910, -275 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "   Izlaz", (Vector2) { 910, -200 + moveY }, 36, 0, WHITE);
			}
			num = 3;
		}
		else if (state >= 6 ) { //level 1 passed ( || state==7....)
			if (language == 0) {
				DrawTextEx(myFont, FormatText("%s passed!", levels[state - 6]), (Vector2) { 780, -500 + moveY }, 48, 0, RED);
				DrawTextEx(myFont, "  Next level", (Vector2) { 900, -350 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "  Main menu", (Vector2) { 900, -275 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "  Settings", (Vector2) { 910, -200 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "  Exit Game", (Vector2) { 900, -125 + moveY }, 36, 0, WHITE);
			}
			else {
				DrawTextEx(myFont, FormatText("%s zavrsen!", levels[state - 6]), (Vector2) { 780, -500 + moveY }, 48, 0, RED);
				DrawTextEx(myFont, "Slijedeci nivo", (Vector2) { 900, -350 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, " Glavni menu", (Vector2) { 900, -275 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "  Postavke", (Vector2) { 910, -200 + moveY }, 36, 0, WHITE);
				DrawTextEx(myFont, "    Izlaz", (Vector2) { 900, -125 + moveY }, 36, 0, WHITE);
			}
			entered = false;
			num = 4;
		}
	}

	if (toggle) {
		for (int i = 0; i < num; i++) {
			if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) { 900, -375 + moveY + i * 75, 172, 64 })) {
				current_choice = i;
				if (last_checked != i) {
					PlaySound(button_click2);
					last_checked = i;
				}
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && i!=0 && state!=1) {
					entered = false;
					toggle = false;
					PlaySound(button_click2);
					UnloadMusicStream(background_music);
					background_music= LoadMusicStream("Assets/Sounds/background_music.mp3"); //reseting music
					return i;
				}
				else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					entered = false;
					toggle = false;
					return current_choice;
				}

			}
			if (IsKeyPressed(KEY_ENTER) && i!=0 && state!=1) {
				entered = false;
				toggle = false;
				UnloadMusicStream(background_music);
				background_music = LoadMusicStream("Assets/Sounds/background_music.mp3");
				return current_choice;
			}
			else if (IsKeyPressed(KEY_ENTER)){
				entered = false;
				toggle = false;
				return current_choice;
			}
		}
	}
	if (toggle == false && state == 1) {
		if (!IsMusicPlaying(background_music)) {
			PlayMusicStream(background_music);
		}
	}
	return -1;

}

//All of the settings are implemented in the rest of this file
void BrightnessSetting(float *screen_brightness) {
	if (language == 0) {
		DrawTextEx(myFont, "Brightness", (Vector2) { 50, 245 }, 36, 2, WHITE);
	}
	else {
		DrawTextEx(myFont, "Svjetlina", (Vector2) { 50, 245 }, 36, 2, WHITE);
	}
	DrawLineEx((Vector2) { 400, 260 }, (Vector2) { 620, 260 }, 10, RED);
	DrawLineEx((Vector2) { *screen_brightness + 370, 245 }, (Vector2) { *screen_brightness + 370, 275 }, 5, RED);

	DrawRectangleLines(380, 240, 300, 40, WHITE);
	if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) { 380, 240, 300, 40 }) && (*screen_brightness <=255 && *screen_brightness >= 30)) {
		*screen_brightness += GetMouseWheelMove()*2.5f;
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			*screen_brightness = GetMouseX() - 370;
		}
	}

	if (*screen_brightness >= 253) {
		*screen_brightness = 253;
	}
	else if (*screen_brightness <= 30) {
		*screen_brightness = 30;
	}

	DrawLine(25, 295, 800, 295, (Color) { 255, 255, 255, 100 });

}

void MasterVolumeSetting(float *master_volume) {
	if (language == 0) {
		DrawTextEx(myFont, "Master volume", (Vector2) { 50, 345 }, 36, 2, WHITE);
	}
	else {
		DrawTextEx(myFont, "Opca glasnoca", (Vector2) { 50, 345 }, 36, 2, WHITE);
	}
	DrawLineEx((Vector2) { 400, 360 }, (Vector2) { 620, 360 }, 10, RED);
	DrawLineEx((Vector2) { *master_volume*110 + 400, 345 }, (Vector2) { *master_volume*110 + 400, 375 }, 5, RED);

	DrawRectangleLines(380, 340, 300, 40, WHITE);
	if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) { 380, 340, 300, 40 }) && (*master_volume <= 2.0f && *master_volume >= 0.0f)) {
		*master_volume += (GetMouseWheelMove()/100.0f);
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			*master_volume = (GetMouseX()- 400.0f)/100.0f;
		}
	}

	if (*master_volume >= 2.0f) {
		*master_volume = 2.0f;
	}
	else if (*master_volume <= 0.0f) {
		*master_volume = 0.0f;
	}
	SetMasterVolume(*master_volume);
	DrawLine(25, 395, 800, 395, (Color) { 255, 255, 255, 100 });
}

void MusicVolumeSetting(float *music_volume) {
	if (language == 0) {
		DrawTextEx(myFont, "Music volume", (Vector2) { 50, 445 }, 36, 2, WHITE);
	}
	else {
		DrawTextEx(myFont, "Glasnoca glazbe", (Vector2) { 50, 445 }, 36, 2, WHITE);
	}
	DrawLineEx((Vector2) { 400, 460 }, (Vector2) { 620, 460 }, 10, RED);
	DrawLineEx((Vector2) { *music_volume * 110 + 400, 445 }, (Vector2) { *music_volume * 110 + 400, 475 }, 5, RED);

	DrawRectangleLines(380, 440, 300, 40, WHITE);
	if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) { 380, 440, 300, 40 }) && (*music_volume <= 2.0f && *music_volume >= 0.0f)) {
		*music_volume += (GetMouseWheelMove() / 100.0f);
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			*music_volume = (GetMouseX() - 400.0f) / 100.0f;
		}
	}

	if (*music_volume >= 2.0f) {
		*music_volume = 2.0f;
	}
	else if (*music_volume <= 0.0f) {
		*music_volume = 0.0f;
	}
	SetMusicVolume(background_music,*music_volume);
	DrawLine(25, 495, 800, 495, (Color) { 255, 255, 255, 100 });
}

extern Sound machinegun, blaster, bullet1_sound, upgraded_sound, flamethrower, laser_beam, disabling_laser, shockwave_sound, small_explosion, medium_explosion_sound,button_click,button_click2;

void SFXVolumeSetting(float *sfx_volume) {
	if (language == 0) {
		DrawTextEx(myFont, "SFX volume", (Vector2) { 50, 545 }, 36, 2, WHITE);
	}
	else {
		DrawTextEx(myFont, "SFX glasnoca", (Vector2) { 50, 545 }, 36, 2, WHITE);
	}
	DrawLineEx((Vector2) { 400, 560 }, (Vector2) { 620, 560 }, 10, RED);
	DrawLineEx((Vector2) { *sfx_volume * 110 + 400, 545 }, (Vector2) { *sfx_volume * 110 + 400, 575 }, 5, RED);

	DrawRectangleLines(380, 540, 300, 40, WHITE);
	if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) { 380, 540, 300, 40 }) && (*sfx_volume <= 2.0f && *sfx_volume >= 0.0f)) {
		*sfx_volume += (GetMouseWheelMove() / 100.0f);
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			*sfx_volume = (GetMouseX() - 400.0f) / 100.0f;
		}
	}

	if (*sfx_volume >= 2.0f) {
		*sfx_volume = 2.0f;
	}
	else if (*sfx_volume <= 0.0f) {
		*sfx_volume = 0.0f;
	}
	//setting volume of every sound
	SetSoundVolume(machinegun, *sfx_volume);
	SetSoundVolume(blaster, *sfx_volume);
	SetSoundVolume(bullet1_sound, *sfx_volume);
	SetSoundVolume(upgraded_sound, *sfx_volume);
	SetSoundVolume(flamethrower, *sfx_volume);
	SetSoundVolume(laser_beam, *sfx_volume);
	SetSoundVolume(disabling_laser, *sfx_volume);
	SetSoundVolume(shockwave_sound, *sfx_volume);
	SetSoundVolume(small_explosion, *sfx_volume);
	SetSoundVolume(medium_explosion_sound, *sfx_volume);
	SetSoundVolume(button_click, *sfx_volume);
	SetSoundVolume(button_click2, *sfx_volume);

	DrawLine(25, 595, 800, 595, (Color) { 255, 255, 255, 100 });
}

void vSyncSetting(bool *vSync) {
	DrawTextEx(myFont, "VSync", (Vector2) { 50, 645 }, 36, 2, WHITE);
	DrawRectangleLines(200, 645, 25, 25, WHITE);
	if (language == 0) {
		DrawTextEx(myFont, "(It is preffered to leave this setting turned on)", (Vector2) { 250, 655 }, 18, 2, WHITE);
	}
	else {
		DrawTextEx(myFont, "(Preferirano je ostaviti ovu postavku upaljenom)", (Vector2) { 250, 655 }, 18, 2, WHITE);
	}

	if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) { 200, 645, 25, 25 })) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			*vSync = !*vSync;
		}
	}

	if (*vSync) {
		DrawRectangle(200, 645, 25, 25, WHITE);
	}
}

void UndoSceneButton(GameScreen *currentScene) {
	static int changeScene = false;

	DrawRectangleLines(15, 15, 30, 30, MYRED);
	if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) { 15, 15, 30, 30 }) && *currentScene!=MAIN_MENU) {
		DrawRectangle(15, 15, 30, 30, MYRED);
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			changeScene = true;
		}
	}
	DrawLineEx((Vector2) {20,30}, (Vector2) {40,30}, 3, WHITE);
	DrawLineEx((Vector2) { 20, 30 }, (Vector2) { 30, 40 }, 2, WHITE);
	DrawLineEx((Vector2) { 20, 30 }, (Vector2) { 30, 20 }, 2, WHITE);
	if (changeScene) {
		if (FadeSceneOut()) {
			if (*currentScene == MAP_CHOICE) {
				button_pressed_counter = 0;
				SetTargetFPS(INFINITY);
				*currentScene = VEHICLE_CHOICE;
				changeScene = false;
			}
			else {
				*currentScene = MAIN_MENU;
				SetTargetFPS(60);
				changeScene = false;
			}
		}
	}
}

//Upgrading vehicle stats (such as health, power etc.)
void UpgradeVehicleStats(int currentVehicle) {
	int upgradePrice[4][5] = { {1000,2000,4000,8000,16000},{1000,2000,4000,8000,16000},{1000,2000,4000,8000,16000},{1000,2000,4000,8000,16000} };
	char *upgradeName[4] = { "Health","Health\nregen","Bullet\npower","Shooting\nspeed" };
	char *upgradeNaziv[4] = { "Zivot","Regeneracija\nzivota","Jacina\nprojektila","Brzina\npucanja" };

	for (int j = 0; j < 4; j++) {

		if (vehicle_upgrades[currentVehicle][j] == 0) {
			DrawTextEx(myFont, "1000", (Vector2) { (454 + j * 40) * 3 - 5, 152 * 3 + 10 }, 24, 0, MYRED);
		}
		else if (vehicle_upgrades[currentVehicle][j] == 1) {
			DrawTextEx(myFont, "2000", (Vector2) { (454 + j * 40) * 3, 152 * 3 + 10 }, 24, 0, MYRED);
		}
		else if (vehicle_upgrades[currentVehicle][j] == 2) {
			DrawTextEx(myFont, "4000", (Vector2) { (454 + j * 40) * 3, 152 * 3 + 10 }, 24, 0, MYRED);
		}
		else if (vehicle_upgrades[currentVehicle][j] == 3) {
			DrawTextEx(myFont, "8000", (Vector2) { (454 + j * 40) * 3, 152 * 3 + 10 }, 24, 0, MYRED);
		}
		else if (vehicle_upgrades[currentVehicle][j] == 4) {
			DrawTextEx(myFont, "16000", (Vector2) { (454 + j * 40) * 3 - 10, 152 * 3 + 10 }, 24, 0, MYRED);
		}
		else {
			DrawTextEx(myFont, "MAX", (Vector2) { (454 + j * 40) * 3, 152 * 3 + 5 }, 36, 0, MYRED);
		}
		if (language == 0) {
			DrawTextEx(myFont, upgradeName[j], (Vector2) { (454 + j * 40) * 3, 175 * 3 + 5 }, 18, 0, WHITE);
		}
		else {
			DrawTextEx(myFont, upgradeNaziv[j], (Vector2) { (454 + j * 40) * 3, 175 * 3 + 5 }, 18, 0, WHITE);
		}
	}

	DrawRectangleLines(430 * 3, 45 * 3, 185 * 3, 95 * 3, MYRED);
	DrawRectangleLines(1325, 419, 485, 160, MYRED);
	if (language == 0) {
		DrawTextEx(myFont, FormatText("Coins: %d", money_data), (Vector2) { 925, 25 }, 36, 0, WHITE);
	}
	else {
		DrawTextEx(myFont, FormatText("Bodovi: %d", money_data), (Vector2) { 925, 25 }, 36, 0, WHITE);
	}

	//Upgrading vehicle stats
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 4; j++) {
			DrawRectangleLines((450 + j * 40) * 3, (125 - i * 15) * 3, 25 * 3, 8 * 3, MYRED);
		}
	}
	for (int j = 0; j < 4; j++) {
		DrawRectangleLines((450 + j * 40) * 3, 145 * 3, 25 * 3, 25 * 3, WHITE);

		if (CheckCollisionPointRec((Vector2) { GetMousePosition().x, GetMousePosition().y }, (Rectangle) { (450 + j * 40) * 3, 145 * 3, 25 * 3, 25 * 3 })) {
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && money_data > upgradePrice[j][vehicle_upgrades[currentVehicle][j]] && vehicle_upgrades[currentVehicle][j] < 5 && vehicle_unlocked[currentVehicle]) {
				PlaySound(button_click2);
				money_data -= upgradePrice[j][vehicle_upgrades[currentVehicle][j]];
				vehicle_upgrades[currentVehicle][j]++;
			}
		}

		for (int i = 0; i < vehicle_upgrades[currentVehicle][j]; i++) {
			DrawRectangle((450 + j * 40) * 3, (125 - i * 15) * 3, 25 * 3, 8 * 3, MYRED);
		}

	}
}

void LanguageSetting(int *lang) {
	static bool pressed = false;
	if (*lang == 0) {
		DrawTextEx(myFont, "Language: English", (Vector2) { 1000, 245 }, 36, 0, WHITE);
	}
	else {
		DrawTextEx(myFont, "   Jezik: Hrvatski", (Vector2) { 1000, 245 }, 36, 0, WHITE);
	}

	DrawRectangleLines(1150, 232, 200, 50, WHITE);
	DrawRectangleLines(1306, 236, 40, 40, WHITE);
	if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) { 1306, 236, 40, 40 })) {
		DrawRectangle(1306, 236, 40, 40, WHITE);
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			pressed = !pressed;
		}
	}

	if (pressed) {
		if (*lang == 0) {
			DrawTextEx(myFont, "Croatian", (Vector2) { 1155, 300 }, 36, 0, WHITE);
			DrawRectangleLines(1150, 282, 200, 50, WHITE);
		}
		else {
			DrawTextEx(myFont, "Engleski", (Vector2) { 1155, 300 }, 36, 0, WHITE);
			DrawRectangleLines(1150, 282, 200, 50, WHITE);
		}

		if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) { 1150, 282, 200, 50 })) {
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				*lang = !(*lang);
				pressed = false;
			}
		}
	}
}

//Ellipses that are drawn below vehicle when selecting it
void DrawEllipses(void) {
	static float vertical_movement[3] = { 0,33,66 };
	for (int i = 0; i < 3; i++) {
		vertical_movement[i] += GetFrameTime()*50;
		DrawTexture(vehicle_ellipse, 850, 750 - vertical_movement[i], (Color) { 255, 255, 255, 255 - vertical_movement[i] * 2.5f });

		if (vertical_movement[i] >= 100) {
			vertical_movement[i] = 0;
		}
	}

}
