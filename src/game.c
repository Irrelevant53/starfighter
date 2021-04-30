#define _CRT_SECURE_NO_WARNINGS

//Potrebno sve radnje koje ovise o vremenu pomnoziti s GetFrameTime(), te prilagoditi vrijednoti
#include "raylib.h"
#include "player.h"
#include "easings.h"
#include "particles.h"
#include "enemy.h"
#include "ui.h"
#include "camera.h"
#include "files.h"
#include "game.h"
#include "bullet.h"



//Colors used
Color Blue = { 0,50,255,255 };
Color White = { 245,245,245,200 };
Color Black = { 10,10,10,10 };
Color Cyan = { 0,255,255,255 };
Color Paused = { 255,255,255,40 };
Color Magenta = { 255,0,255,200 };


extern Player player;
extern Bullet *bullet[MAX_BULLETS];
extern Enemy *enemy[MAX_ENEMIES];
extern Texture2D enemy_texture1,enemy_texture2,enemy_texture3,enemy_texture4,boss2;
extern Texture2D spaceship[3],enemy_bullet_texture[3],spaceship_scaled[3],enemy_texture5;
extern Texture2D laser[3],powerful_laser,enemy_exhaust,enemy_exhaust2,boss1,menu_arrows;
extern Texture2D map_image,lightning,special,open,locked_ability,ellipse,vehicle_ellipse;
extern Texture2D rotated_texture[2],upgrades,abilities[3][2],engine_thrust;
extern Texture2D middle, start, end,message,max,mystery_ability,level_beam;
extern Texture2D disabling_line,main_menu_button,unlock_button,selected_button,boss_explosion;
extern Texture2D shield,flame_texture,health,health2,shield_particles,explosion[2],medium_explosion;
extern FILE *fp;
extern Animation level_beam_animation;
extern int map_unlocked[10], vehicle_unlocked[3], money_data, vehicle_upgrades[3][4],special_ability[3][2],language;
extern float vehicle_health, moveY[2];
extern float button_pos[4], brightness, master_volume, music_volume , sfx_volume;
extern bool toggle;
extern int button_pressed_counter;

extern Sound button_click,button_click2,machinegun,blaster,bullet1_sound,upgraded_sound,flamethrower,laser_beam,disabling_laser,shockwave_sound;
extern Sound medium_explosion_sound;
extern Music background_music;
extern Font myFont;
bool restart_cooldown_timers = false;
bool paused;

int main(void)
{
	
	SetConfigFlags(FLAG_FULLSCREEN_MODE );
	InitWindow(1920,1080, "space shooter");
	InitAudioDevice();


	//Loading textures that are used in game
	spaceship[0] = LoadTexture("Assets/Textures/space_ships/brodic_spritesheet2_2_larger2.png");
	spaceship[1] = LoadTexture("Assets/Textures/space_ships/brodic2_spritesheet_2_larger2.png");
	spaceship[2] = LoadTexture("Assets/Textures/space_ships/brodic4_spritesheet_2_larger.png");

	spaceship_scaled[0] = LoadTexture("Assets/Textures/space_ships/brodic1_x2.png");
	spaceship_scaled[1] = LoadTexture("Assets/Textures/space_ships/brodic2_x2.png");
	spaceship_scaled[2] = LoadTexture("Assets/Textures/space_ships/brodic3_x2.png");

	lightning = LoadTexture("Assets/Textures/lightning2.png");
	enemy_texture1 = LoadTexture("Assets/Textures/gray_enemy1.png");
	enemy_texture2 = LoadTexture("Assets/Textures/gray_enemy2.png");
	enemy_texture3 = LoadTexture("Assets/Textures/gray_enemy3.png");
	enemy_texture4 = LoadTexture("Assets/Textures/gray_enemy5.png");
	enemy_texture5 = LoadTexture("Assets/Textures/gray_enemy7.png");
	boss1= LoadTexture("Assets/Textures/boss1.png");
	boss2 = LoadTexture("Assets/Textures/boss2.png");
	laser[0] = LoadTexture("Assets/Textures/bullet1.png");
	laser[1] = LoadTexture("Assets/Textures/bullet2.png");
	laser[2] = LoadTexture("Assets/Textures/bullet3.png");
	powerful_laser = LoadTexture("Assets/Textures/bullet1_powerup.png");
	flame_texture = LoadTexture("Assets/Textures/flame3.png");
	enemy_bullet_texture[0]= LoadTexture("Assets/Textures/enemy_bullet1.png");
	enemy_bullet_texture[1] = LoadTexture("Assets/Textures/enemy_bullet2.png");
	enemy_bullet_texture[2] = LoadTexture("Assets/Textures/enemy_bullet3.png");


	star_dust = LoadTexture("Assets/Textures/star_dust.png");
	coin = LoadTexture("Assets/Textures/coins_spritesheet2.png");
	rotated_texture[0] = LoadTexture("Assets/Textures/map_rotation1.png");
	rotated_texture[1] = LoadTexture("Assets/Textures/map_rotation2.png");
	disabling_line = LoadTexture("Assets/Textures/disabling_line.png");
	ellipse = LoadTexture("Assets/Textures/ellipse.png");
	enemy_exhaust= LoadTexture("Assets/Textures/enemy_exhaust.png");
	enemy_exhaust2 = LoadTexture("Assets/Textures/enemy_exhaust2.png");

	main_menu_button = LoadTexture("Assets/Textures/main_menu_button.png");

	unlock_button = LoadTexture("Assets/Textures/unlock_button.png");
	selected_button = LoadTexture("Assets/Textures/selected_button.png");
	message = LoadTexture("Assets/Textures/message.png");
	upgrades = LoadTexture("Assets/Textures/upgrades.png");
	engine_thrust = LoadTexture("Assets/Textures/exhaust1-sheet2.png");
	health = LoadTexture("Assets/Textures/health.png");
	health2 = LoadTexture("Assets/Textures/health2.png");
	explosion[0] = LoadTexture("Assets/Textures/explosion1.png");
	explosion[1] = LoadTexture("Assets/Textures/explosion2.png");
	medium_explosion = LoadTexture("Assets/Textures/medium_explosion.png");
	boss_explosion = LoadTexture("Assets/Textures/boss_explosion.png");

	abilities[0][0] = powerful_laser;
	abilities[1][0] = LoadTexture("Assets/Textures/ability3.png");
	abilities[2][0] = LoadTexture("Assets/Textures/shield2.png");


	abilities[0][1] = LoadTexture("Assets/Textures/ability1.png");
	abilities[1][1] = LoadTexture("Assets/Textures/ability2.png");
	abilities[2][1] = LoadTexture("Assets/Textures/ability4.png");
	mystery_ability = LoadTexture("Assets/Textures/mystery_ability.png");
	locked_ability = LoadTexture("Assets/Textures/locked_ability.png");
	shield = LoadTexture("Assets/Textures/shield2-sheet.png");
	shield_particles = LoadTexture("Assets/Textures/shield_particles.png");
	vehicle_ellipse = LoadTexture("Assets/Textures/vehicle_ellipse.png");

	max = LoadTexture("Assets/Textures/max.png");
	open = LoadTexture("Assets/Textures/open.png");

	start = LoadTexture("Assets/Textures/start2-sheet.png");
	middle = LoadTexture("Assets/Textures/middle2-sheet.png");
	end = LoadTexture("Assets/Textures/end2.png");
	level_beam= LoadTexture("Assets/Textures/level_beam.png");

	Texture2D bg = LoadTexture("Assets/Textures/planet_background2_huge.png");
	Texture2D bg2 = LoadTexture("Assets/Textures/planet_background2_huge2.png");
	Texture2D bg3 = LoadTexture("Assets/Textures/planet_background2_huge3.png");
	Texture2D bg4 = LoadTexture("Assets/Textures/planet_background2_huge4.png");
	Texture2D garage_background = LoadTexture("Assets/Textures/garage.png");
	Texture2D title = LoadTexture("Assets/Textures/title.png");
	Texture2D star[5];

	Animation star_animation[5] = { 
		SetAnimation((Rectangle) { 0, 0, 5628 / 14, 411 }, 14, 65),
		SetAnimation((Rectangle) { 0, 0, 1876 / 14, 137 }, 14, 75),
		SetAnimation((Rectangle) { 0, 0, 938 / 14, 68 }, 14, 60),
		SetAnimation((Rectangle) { 0, 0, 703 / 14, 51 }, 14, 90),
		SetAnimation((Rectangle) { 0, 0, 1500 / 14, 109 }, 14, 85)
	};
	Vector2 star_pos[5] = { 
		(Vector2){ 700, 350 } ,
		(Vector2) {100,300},
		(Vector2) {300,900},
		(Vector2) {1700,300},
		(Vector2) {1500,750},
	};
	
	Texture2D planets[16] = { LoadTexture("Assets/Textures/planets/Barren.png"),
							  LoadTexture("Assets/Textures/planets/Desert.png"),
							  LoadTexture("Assets/Textures/planets/GasGiant.png") ,
							  LoadTexture("Assets/Textures/planets/Ice.png") ,
							  LoadTexture("Assets/Textures/planets/Lava.png") ,
							  LoadTexture("Assets/Textures/planets/Ocean.png") ,
							  LoadTexture("Assets/Textures/planets/Asteroid.png") ,
							  LoadTexture("Assets/Textures/planets/Asteroid1.png") ,
							  LoadTexture("Assets/Textures/planets/Asteroid2.png") ,
							  LoadTexture("Assets/Textures/planets/Asteroid3.png") ,
							  LoadTexture("Assets/Textures/planets/Nebula1.png") ,
							  LoadTexture("Assets/Textures/planets/Nebula2.png") ,
							  LoadTexture("Assets/Textures/planets/Nebula3.png") ,
							  LoadTexture("Assets/Textures/planets/Nebula4.png") ,
							  LoadTexture("Assets/Textures/trampoline.png") ,
						      LoadTexture("Assets/Textures/slide.png") ,
	};


	button_click = LoadSound("Assets/Sounds/button-click2.wav");
	button_click2 = LoadSound("Assets/Sounds/button-click.wav");
	machinegun=LoadSound("Assets/Sounds/machinegun_single2.wav");
	blaster= LoadSound("Assets/Sounds/blaster.wav");
	bullet1_sound= LoadSound("Assets/Sounds/laser.wav");
	upgraded_sound=LoadSound("Assets/Sounds/laser.wav");
	flamethrower = LoadSound("Assets/Sounds/flamethrower.wav");
	laser_beam = LoadSound("Assets/Sounds/laser1.wav");
	disabling_laser = LoadSound("Assets/Sounds/disabling_laser.wav");
	shockwave_sound= LoadSound("Assets/Sounds/shockwave.wav");
	small_explosion=LoadSound("Assets/Sounds/small_explosion.wav");
	medium_explosion_sound = LoadSound("Assets/Sounds/medium_explosion.wav");
	background_music=LoadMusicStream("Assets/Sounds/background_music.mp3");

	myFont = LoadFontEx("Assets/Fonts/Pixel4.ttf",72,0,250);
	level_beam_animation = SetAnimation((Rectangle) { 0, 0, 112, 2 }, 4, 100);

	SetExitKey(0);
	int menu_choice = -1;

	CreateGameFile("game_data.txt");
	CreateSettingsFile("settings.txt");

	player.position = (Vector2){ GetScreenWidth() / 2,GetScreenHeight()-GetScreenHeight()/5 };
	int counter = 0,ui_pressed=-1,shipID=0,chosen_map=-1;
	bool entered_map = false,push_wave_out=true;


	brightness = 200.0f,master_volume=0.5f,music_volume=0.5f,sfx_volume=0.5f;

	GameScreen currentScreen = RAYLIB_LOGO;
	
	float radius=0;
	int wave_count = 0,trampoline_rotation=0;
		
	fscanf(fp, "%d %d %d %d %d\n", &map_unlocked[0],&map_unlocked[1],&map_unlocked[2], &map_unlocked[3], &map_unlocked[4]);
	fscanf(fp, "%d %d %d %d %d\n", &map_unlocked[5], &map_unlocked[6], &map_unlocked[7], &map_unlocked[8], &map_unlocked[9]);
		
	 fscanf(fp,"%d %d %d \n",&vehicle_unlocked[0],&vehicle_unlocked[1], &vehicle_unlocked[2]); 
	 fscanf(fp, "%d\n", &money_data);
	 for (int i = 0; i < 3; i++) {
			 fscanf(fp, "%d %d %d %d\n", &vehicle_upgrades[i][0], &vehicle_upgrades[i][1], &vehicle_upgrades[i][2], &vehicle_upgrades[i][3]);
	 }
	 for (int i = 0; i < 3; i++) {
		 fscanf(fp, "%d %d\n", &special_ability[i][0],&special_ability[i][1]);
	 }
	 fscanf(fp2, "%f %f %f %f\n", &brightness, &master_volume, &music_volume, &sfx_volume);
	 fscanf(fp2, "%d %d\n", &vSync_state,&language);

	ui_text UI_main_menu[3] = { {1920 / 2 , 1080 / 2+40},{1920 / 2-10 , 1080 / 2 + 80},{1920 / 2 , 1080 / 2 + 120} };
	ui_text select_button[3] = { {960+10,810},{960 * 3+10,810},{960 * 5+10,810} };


	int randX, randY,map=0,currentVehicle=0;
	bool draw_boxes = true;
	
	camera = SetupCamera(&camera);
	float currentTime = 0;
	bool pres_left = false, pres_right = false,animation_done=false;
	SetPlayerAnimations();
	float bg_pos = 0;

	bool fade_scene_out = false,entered_game=false;
	int next_scene;
	SetTargetFPS(60);
	SetMasterVolume(master_volume);
	SetMusicVolume(background_music, music_volume);

	while (true) //trebat ce promijenit na kraju
	{

		if (IsKeyDown(KEY_LEFT_ALT) && IsKeyDown(KEY_F4)) {
			break;
		}
		if (IsKeyPressed(KEY_P)) {
			TakeScreenshot("Skrin.png");
		}

		BeginDrawing();
		ClearBackground(BLACK);
		switch (currentScreen) {
		case RAYLIB_LOGO:
			animation_done=RaylibAnimation();
			if (animation_done) {
				currentScreen = MAIN_MENU;
			}
			break;
		case MAIN_MENU:
			if (entered_game==false) {
				InitStars();
				star[0]=LoadTexture("Assets/Textures/star_animated.png"),
				star[1]=LoadTexture("Assets/Textures/star_animated2.png"),
				star[2]=LoadTexture("Assets/Textures/star_animated3.png"),
				star[3]=LoadTexture("Assets/Textures/star_animated4.png"),
				star[4]=LoadTexture("Assets/Textures/star_animated5.png"),
				entered_game = true;
			}
			DrawStars();
			for (int i = 0; i < 5; i++) {
				DrawTextureRec(star[i], (Rectangle) { star_animation[i].frameStart.x + (star_animation[i].currentFrame*star_animation[i].frameStart.width), star_animation[i].frameStart.y, star_animation[i].frameStart.width, star_animation[i].frameStart.height }, star_pos[i] , WHITE);
				UpdateAnimation(&star_animation[i]);
			}
			DrawTexture(title, 259, 200, WHITE);
			
			ui_pressed = CheckUIOption(4);
			if (ui_pressed != -1) {
				fade_scene_out = true;
				next_scene = ui_pressed;
			}

		
			if (language == 0) {
				DrawTextEx(myFont, "Start", (Vector2) { button_pos[0] + 150, 560 }, 36, 0, MYRED);
				DrawTextEx(myFont, "Settings", (Vector2) { button_pos[1] + 150, 680 }, 36, 0, MYRED);
				DrawTextEx(myFont, "About", (Vector2) { button_pos[2] + 150, 800 }, 36, 0, MYRED);
				DrawTextEx(myFont, "Exit", (Vector2) { button_pos[3] + 150, 920 }, 36, 0, MYRED);
			}
			else {
				DrawTextEx(myFont, "Start", (Vector2) { button_pos[0] + 150, 560 }, 36, 0, MYRED);
				DrawTextEx(myFont, "Postavke", (Vector2) { button_pos[1] + 150, 680 }, 36, 0, MYRED);
				DrawTextEx(myFont, "Info", (Vector2) { button_pos[2] + 150, 800 }, 36, 0, MYRED);
				DrawTextEx(myFont, "Izlaz", (Vector2) { button_pos[3] + 150, 920 }, 36, 0, MYRED);
			}
			DrawFps();

			if (fade_scene_out) {
				if (FadeSceneOut()) {
					for (int i = 0; i < 5; i++) {
						UnloadTexture(star[i]); // dynamicly loading and unloading star textures because they are pretty large
					}
					entered_game = false;
					switch (next_scene) {
					case 0:
						currentScreen = VEHICLE_CHOICE;
						fade_scene_out = false;
						break;
					case 1:
						currentScreen = SETTINGS;
						fade_scene_out = false;
						break;
					case 2:
						currentScreen = ABOUT;
						fade_scene_out = false;
						break;
					case 3:
						exit(0);
						fade_scene_out = false;
						break;
					}

				}
			}

			break;

		case SETTINGS:
			if (language == 0) {
				DrawTextEx(myFont, "Settings", (Vector2) { 50, 95 }, 72, 5, WHITE);
			}
			else {
				DrawTextEx(myFont, "Postavke", (Vector2) { 50, 95 }, 72, 5, WHITE);
			}
			DrawLine(50, 195, 1000, 195, (Color) { 255, 255, 255, 100 });
			BrightnessSetting(&brightness);
			MasterVolumeSetting(&master_volume);
			MusicVolumeSetting(&music_volume);
			SFXVolumeSetting(&sfx_volume);
			vSyncSetting(&vSync_state );
			LanguageSetting(&language);
			menu_choice = ToggleMenu(3);
			if (menu_choice != -1) {
				fade_scene_out = true;
				next_scene = menu_choice;
			}
			
			if(fade_scene_out){
				if (FadeSceneOut()) {
					switch (next_scene) {
					case 0:
						button_pressed_counter = 0;
						SetTargetFPS(60);
						currentScreen = MAIN_MENU;
						RefreshSettingsFile("settings.txt");
						fade_scene_out = false;
						break;
					case 1:
						currentScreen = ABOUT;
						RefreshSettingsFile("settings.txt");
						fade_scene_out = false;
						break;
					case 2:
						RefreshGameFileData("game_data.txt");
						RefreshSettingsFile("settings.txt");
						fade_scene_out = false;
						exit(0);
						break;
						
					}
				}
			}
			
			DrawFps();
			UndoSceneButton(&currentScreen);
			break;
		case ABOUT:
			if (language == 0) {
				DrawTextEx(myFont, "About", (Vector2) { 50, 95 }, 72, 5, WHITE);
				DrawText("This game was made as a final high school project.\nIt was created using Raylib library and was written in C \nprogramming language to demonstrate that you don't\nneed advanced game engines to create games, but just\nknowledge about language you are using and programming logic!", 50, 245, 36, WHITE);
				DrawText("Copyright 2020-2021	 Dominik Saric	 All rights reserved", 750, 1020, 18, WHITE);
			}
			else {
				DrawTextEx(myFont, "Info", (Vector2) { 50, 95 }, 72, 5, WHITE);
				DrawText("Ova igra je napravljena kao zavrsni srednjoskolski rad.\nKreirana je koristeci Raylib biblioteku, te je napisana u C \nprogramskom jeziku s namjerom da pokaze mogucnost \nkreiranja igara bez naprednih game engine-a, vec samo\nkoristeci znanje odabranog jezika i logiku programiranja!", 50, 245, 36, WHITE);
				DrawText("Autorska prava 2020-2021	 Dominik Saric	 Sva prava pridrzana", 750, 1020, 18, WHITE);
			}
			DrawLine(50, 195, 1000, 195, (Color) { 255, 255, 255, 100 });
			menu_choice = ToggleMenu(0);
			if (menu_choice != -1) {
				fade_scene_out = true;
				next_scene = menu_choice;
			}

			if (fade_scene_out) {
				if (FadeSceneOut()) {
					switch (next_scene) {
					case 0:
						menu_choice = -1;
						button_pressed_counter = 0;
						SetTargetFPS(60);
						currentScreen = MAIN_MENU;
						fade_scene_out = false;
						break;
					case 1:
						menu_choice = -1;
						currentScreen = SETTINGS;
						fade_scene_out = false;
						break;
					case 2:
						menu_choice = -1;
						RefreshGameFileData("game_data.txt");
						fade_scene_out = false;
						exit(0);
						break;
						
					}
				}
			}

			DrawFps();
			UndoSceneButton(&currentScreen);
			break;
		case MAP_CHOICE:
			DrawStars();
			if (entered_map == true) {
				if (vSync_state) {
					SetWindowState(FLAG_VSYNC_HINT | FLAG_FULLSCREEN_MODE);
				}
				entered_map = false;
			}
			BeginMode2D(camera);
			UpdateMapCamera(&camera);
			map = GetDesiredLevel(camera.offset.y);
			if (map != -1) {
				chosen_map = map;
			}

			if (chosen_map != -1 && FadeSceneOut()) {
				camera.offset.y = 0;
				SetMusicVolume(background_music, music_volume);
				PlayMusicStream(background_music);
				currentScreen = GAME;
				InitPlayer(currentVehicle);
				InitStarDust();
				UnloadTexture(map_image);
			}

			//Menu
			menu_choice = ToggleMenu(5);
			if (menu_choice != -1) {
				fade_scene_out = true;
				next_scene = menu_choice;
			}
			if (fade_scene_out) {
				if (FadeSceneOut()) {
					switch (next_scene) {
					case 0:
						menu_choice = -1;
						button_pressed_counter = 0;
						currentScreen = MAIN_MENU;
						fade_scene_out = false;
						break;
					case 1:
						menu_choice = -1;
						currentScreen = SETTINGS;
						fade_scene_out = false;
						break;
					case 2:
						menu_choice = -1;
						RefreshGameFileData("game_data.txt");
						fade_scene_out = false;
						exit(0);
						break;
					}
				}
			}

			EndMode2D();
			UndoSceneButton(&currentScreen);

			break;

		case VEHICLE_CHOICE:
			DrawTexture(garage_background, 0, 0, WHITE);
			DrawEllipses();
			if (!paused) {
				currentVehicle = SwitchVehicle(select_button);
			}
			ProcessVehicleAbilities(currentVehicle);
			UpgradeVehicleStats(currentVehicle);

			//Switching scenes
			if (button_pressed_counter >= 2 && FadeSceneOut()) {
				currentScreen = MAP_CHOICE;
				SetTargetFPS(60);
				entered_map = true;
			}
			DrawFps();
			//Menu options
			menu_choice = ToggleMenu(4);
			if (menu_choice != -1) {
				fade_scene_out = true;
				next_scene = menu_choice;
			}
			UndoSceneButton(&currentScreen);
			if (fade_scene_out) {
				if (FadeSceneOut()) {
					switch (next_scene) {
						case 0:
							menu_choice = -1;
							SetTargetFPS(60);
							currentScreen = MAIN_MENU;
							button_pressed_counter = 0;
							fade_scene_out = false;
							break;
						case 1:
							menu_choice = -1;
							currentScreen = ABOUT;
							fade_scene_out = false;
							break;
						case 2:
							menu_choice = -1;
							RefreshGameFileData("game_data.txt");
							fade_scene_out = false;
							exit(0);
							break;
						}
					}
				}
			entered_map = false;
			break;
			
		case GAME:
			if (toggle == false) {
				UpdateMusicStream(background_music);
			}
			BeginMode2D(camera);

			ClearBackground((Color) { 0, 0, 10, 255 });

			DrawStars();
			DrawStarDust();
			switch(chosen_map){ //drawing background textures first
			case 0:
				DrawTexture(bg, 400 + bg_pos/2, 1080 - bg.height, WHITE);
				DrawTexture(planets[0], 1900 + bg_pos * 2, 600, WHITE);
				if (counter <= 255) {
					DrawTextEx(myFont, "Battle of Brenn", (Vector2) { GetScreenWidth() / 2 - 250, 200 }, 72, 2, (Color) { 255, 255, 255, 255 - counter });
					counter++;
				}
				break;
			case 1:
				DrawTexture(bg3, 400 + bg_pos/3, 0, WHITE);
				DrawTexture(planets[1], 1450 + bg_pos, 150, WHITE);
				DrawTexture(planets[6], 1600 + bg_pos * 3, 600, WHITE);
				DrawTexture(planets[7], 1900 + bg_pos * 2.5f, 800, WHITE);
				if (bg_pos + 2000 < 0) {
					bg_pos = 500;
				}
				if (counter <= 255) {
					DrawTextEx(myFont, "Skratos Playground", (Vector2) { GetScreenWidth() / 2 - 250, 200 }, 72, 2, (Color) { 255, 255, 255, 255 - counter });
					counter++;
				}
				break;
			case 2:
				DrawTexture(planets[2], 1820 + bg_pos * 1.75f, 150, WHITE);
				DrawTexture(planets[4], 2000 + bg_pos * 1.25f, 600, WHITE);
				DrawTexture(planets[10], 1990 + bg_pos * 2.5f, 800, WHITE);
				DrawTexture(planets[11], 2050 + bg_pos * 2.25f, 300, WHITE);
				if (bg_pos + 2000 < 0) {
					bg_pos = 500;
				}
				if (counter <= 255) {
					DrawTextEx(myFont, "Furious Battlefield", (Vector2) { GetScreenWidth() / 2 - 250, 200 }, 72, 2, (Color) { 255, 255, 255, 255 - counter });
					counter++;
				}
				break;
			case 3:
				DrawTexture(bg4, 900 + bg_pos/4, 0, WHITE);
				DrawTexture(planets[8], 1900 + bg_pos * 2.25f, 350, WHITE);
				DrawTexture(planets[9], 1600 + bg_pos * 1.75f, 750, WHITE);
				DrawTexture(planets[13], 2100 + bg_pos * 1.75f, 3300, WHITE);
				if (bg_pos + 2000 < 0) {
					bg_pos = 500;
				}
				if (counter <= 255) {
					DrawTextEx(myFont, "Unstable Space", (Vector2) { GetScreenWidth() / 2 - 250, 200 }, 72, 2, (Color) { 255, 255, 255, 255 - counter });
					counter++;
				}
				break;
			case 4:
				DrawTexture(bg2, 400 + bg_pos/1.5f, 1080 - bg2.height, WHITE);
				DrawTexture(planets[3], 1700 + bg_pos * 2.25f, 925, WHITE);
				DrawTexture(planets[5], 1900 + bg_pos * 1.5f, 450, WHITE);
				DrawTexture(planets[12], 1600 + bg_pos * 1.75f, 755, WHITE);
				if (bg_pos + 2000 < 0) {
					bg_pos = 500;
				}
				if (counter <= 255) {
					DrawTextEx(myFont, "Battle of Sector 67", (Vector2) { GetScreenWidth() / 2 - 250, 200 }, 72, 2, (Color) { 255, 255, 255, 255 - counter });
					counter++;
				}
				break;

			}


			if (player.health > 0 && !paused) {
				bg_pos += -0.2f;
				ProcessCoins(currentVehicle);
				DrawCoins();
				UpdateEnemy();
			}
			UpdatePlayerBullet(currentVehicle);
			DrawEnemy();
			UpdateEnemyBullet();
			if (player.health > 0 && !paused) {
				CheckIfBulletHitsEnemy(currentVehicle);
				Enemy1Shoot();
				Enemy2Shoot();
				Enemy3Shoot();
				Medium2Shoot();
				Medium3Shoot();
				Boss1Shoot();
				Boss2Shoot();
			
				CheckEnemyHealth();


				switch (currentVehicle) {
				case 0:
					PlayerDash();
					break;
				case 1:
					InitFlameParticles((Vector2) { player.position.x, player.position.y });
					break;
				case 2:
					LaserBeam();
					break;
				}
				HealthRegen(currentVehicle);
				PlayerMovement(currentVehicle);
				PlayerShoot(currentVehicle);
				DrawEnemyHealth();
				CheckIfBulletHitsPlayer(currentVehicle);
			}

			switch (currentVehicle) // drawing stuff while game is paused, but not updating it
			{
			case 0:
				DrawElectricity();
				DrawDisablingLine();
				break;
			case 1:
				ProcessFlameParticles();
				CheckCollisionFlameParticles();
				ShockWaveAbility();
				CheckCollisionWithShockWave();
				break;
			case 2:
				ActivateShield();
				break;
			}
			DrawPlayer(currentVehicle);






			switch (chosen_map) { // Enemies are spawned here
			case 0:
				if (wave_count == 0 && counter == 255 && push_wave_out == true) {
					InitEnemy((Vector2) { 2000, 300 }, (Vector2) { 1, 0 }, (Rectangle) { 2000 + 15, 300 + 15, 60, 80 }, 20, 0);
					InitEnemy((Vector2) { 2000, 500 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 2000 + 15, 500 + 15, 60, 80 }, 40, 1);
					InitEnemy((Vector2) { 2000, 700 }, (Vector2) { 1, 0 }, (Rectangle) { 2000 + 15, 700 + 15, 60, 80 }, 20, 0);
					push_wave_out = false;
				}
				else if (wave_count == 1 && push_wave_out == true) {
					InitEnemy((Vector2) { 2000, 400 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 2000 + 15, 400 + 15, 60, 80 }, 40, 1);
					InitEnemy((Vector2) { 1950, 500 }, (Vector2) { 1, 0 }, (Rectangle) { 1950 + 15, 500 + 15, 60, 80 }, 20, 0);
					InitEnemy((Vector2) { 2000, 600 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 2000 + 15, 600 + 15, 60, 80 }, 40, 1);
					push_wave_out = false;
				}
				else if (wave_count == 2 && push_wave_out == true) {
					InitEnemy((Vector2) { 2000, 400 }, (Vector2) { 1, 0 }, (Rectangle) { 2000 + 15, 400 + 15, 60, 80 }, 20, 0);
					InitEnemy((Vector2) { 2000, 600 }, (Vector2) { 1, 0 }, (Rectangle) { 2000 + 15, 600 + 15, 60, 80 }, 20, 0);
					InitEnemy((Vector2) { 1950, 400 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 1950 + 15, 400 + 15, 60, 80 }, 40, 1);
					InitEnemy((Vector2) { 1950, 600 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 1950 + 15, 600 + 15, 60, 80 }, 40, 1);
					push_wave_out = false;
				}
				else if (wave_count == 3 && push_wave_out == true) {
					InitEnemy((Vector2) { 2000, 400 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2000 + 15, 400 + 15, 60, 100 }, 200, 2);
					InitEnemy((Vector2) { 2000, 600 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2000 + 15, 600 + 15, 60, 100 }, 200, 2);
					push_wave_out = false;
				}
				else if (wave_count == 4 && push_wave_out == true) {
					InitEnemy((Vector2) { 2000, 500 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2000 + 15, 500 + 15, 60, 100 }, 200, 2);
					InitEnemy((Vector2) { 2050, 350 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 350 + 15, 60, 80 }, 20, 0);
					InitEnemy((Vector2) { 2050, 500 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 500 + 15, 60, 80 }, 20, 0);
					InitEnemy((Vector2) { 2050, 650 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 650 + 15, 60, 80 }, 20, 0);
					push_wave_out = false;
				}
				else if (wave_count == 5 && push_wave_out == true) {
					InitEnemy((Vector2) { 2000, 300 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2000 + 15, 300 + 15, 60, 100 }, 200, 2);
					InitEnemy((Vector2) { 2050, 450 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 450 + 15, 60, 80 }, 20, 0);
					InitEnemy((Vector2) { 2100, 550 }, (Vector2) { 1, 0 }, (Rectangle) { 2100 + 15, 550 + 15, 60, 80 }, 40, 1);
					InitEnemy((Vector2) { 2050, 650 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 650 + 15, 60, 80 }, 20, 0);
					InitEnemy((Vector2) { 2000, 800 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2000 + 15, 800 + 15, 60, 100 }, 200, 2);
					push_wave_out = false;
				}
				else if (wave_count == 6 && push_wave_out == true) {
					InitEnemy((Vector2) { 2000, 400 }, (Vector2) { 2, 1.5f }, (Rectangle) { 2000 + 15, 400 + 15, 200, 150 }, 1000, 3);
					push_wave_out = false;
				}
				else if (wave_count == 7 && push_wave_out == true) { //Level 1 passed  
					counter++;
					if (counter >= 350) {
						if (counter == 350) {
							money_data += 5000;
						}
						menu_choice = ToggleMenu(6);
					}
					if (menu_choice != -1) {
						fade_scene_out = true;
						next_scene = menu_choice;
					}
					if (fade_scene_out) {
						if (FadeSceneOut()) {
							switch (next_scene) {
							case 0:
								menu_choice = -1;
								counter = 0;
								map_unlocked[1] = 1;
								chosen_map=1; //entering new map
								wave_count = 0;
								counter = 0;
								push_wave_out = false;
								bg_pos = 0;
								button_pressed_counter = 0;
								RemoveAllEnemyBullets();
								RemoveAllPlayerBullets();
								InitPlayer(currentVehicle);
								restart_cooldown_timers = true; //restarting ability cooldowns
								fade_scene_out = false;
								toggle = false;
								break;
							case 1: //main menu
								menu_choice = -1;
								wave_count = 0;
								counter = 0;
								push_wave_out = false;
								bg_pos = 0;
								button_pressed_counter = 0;
								RemoveAllEnemies();
								RemoveAllEnemyBullets();
								RemoveAllPlayerBullets();
								map = -1;
								SetTargetFPS(60);
								restart_cooldown_timers = true;
								chosen_map = -1;
								currentScreen = MAIN_MENU;
								fade_scene_out = false;
								break;
							case 2: //settings
								menu_choice = -1;
								wave_count = 0;
								counter = 0;
								push_wave_out = false;
								bg_pos = 0;
								button_pressed_counter = 0;
								RemoveAllEnemies();
								RemoveAllEnemyBullets();
								RemoveAllPlayerBullets();
								restart_cooldown_timers = true;
								chosen_map = -1;
								currentScreen = SETTINGS;
								fade_scene_out = false;
								break;
							case 3:
								menu_choice = -1; // exit game
								RefreshGameFileData("game_data.txt");
								exit(0);
								break;
							}
						}
					}

				}
				if (GetNumOfEnemies() == 0 && counter >= 255 && wave_count < 7) {
					wave_count++;
					push_wave_out = true;
				}

				break;

				case 1:
					DrawTexturePro(planets[14], (Rectangle) { 0, 0, planets[14].width, planets[14].height }, (Rectangle) {1600 + bg_pos * 3, 750, planets[14].width, planets[14].height}, (Vector2) { planets[14].width/2, planets[14].height/2 },trampoline_rotation, WHITE); // drawing trampoline in front of ship
					DrawTexturePro(planets[15], (Rectangle) { 0, 0, planets[15].width, planets[15].height }, (Rectangle) { 2200 + bg_pos * 1.2f, 350, planets[15].width, planets[15].height }, (Vector2) { planets[15].width / 2, planets[15].height / 2 }, trampoline_rotation/2.0f, WHITE);
					if (!paused) {
						trampoline_rotation++;
					}
					if (wave_count == 0 && counter == 255 && push_wave_out == true) {
						//InitEnemy((Vector2) { 1800, 100 }, (Vector2) { 2, 2 }, (Rectangle) { 1800 + 15, 100 + 15, 60, 80 }, 20, 4);//

						InitEnemy((Vector2) { 2000, 200 }, (Vector2) { 1, 0 }, (Rectangle) { 2000 + 15, 200 + 15, 60, 80 }, 40, 1);
						InitEnemy((Vector2) { 2000, 400 }, (Vector2) { 1, 0 }, (Rectangle) { 2000 + 15, 400 + 15, 60, 80 }, 40, 1);
						InitEnemy((Vector2) { 2000, 600 }, (Vector2) { 1, 0 }, (Rectangle) { 2000 + 15, 600 + 15, 60, 80 }, 40, 1);
						InitEnemy((Vector2) { 2000, 800 }, (Vector2) { 1, 0 }, (Rectangle) { 2000 + 15, 800 + 15, 60, 80 }, 20, 1);
						push_wave_out = false;
					}
					else if (wave_count == 1 && push_wave_out == true) {
						InitEnemy((Vector2) { 1950, 500 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 1950 + 15, 500 + 15, 60, 80 }, 40, 1);
						InitEnemy((Vector2) { 2000, 400 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 2000 + 15, 400 + 15, 60, 80 }, 40, 1);
						InitEnemy((Vector2) { 2000, 600 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 2000 + 15, 600 + 15, 60, 80 }, 40, 1);
						InitEnemy((Vector2) { 2050, 300 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 300 + 15, 60, 80 }, 40, 1);
						InitEnemy((Vector2) { 2050, 500 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 500 + 15, 60, 80 }, 20, 0);
						InitEnemy((Vector2) { 2050, 700 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 700 + 15, 60, 80 }, 40, 1);
						push_wave_out = false;
					}
					else if (wave_count == 2 && push_wave_out == true) {
						InitEnemy((Vector2) { 2000, 200 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 2000 + 15, 200 + 15, 60, 80 }, 40, 1);
						InitEnemy((Vector2) { 1950, 400 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 1950 + 15, 400 + 15, 60, 80 }, 20, 0);
						InitEnemy((Vector2) { 2200, 550 }, (Vector2) { 1, 0 }, (Rectangle) { 2200 + 15, 550 + 15, 60, 100 }, 200, 2);
						InitEnemy((Vector2) { 2000, 550 }, (Vector2) { 1, 0 }, (Rectangle) { 2000 + 15, 550 + 15, 60, 100 }, 200, 2);
						InitEnemy((Vector2) { 1950, 700 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 1950 + 15, 700 + 15, 60, 80 }, 20, 0);
						InitEnemy((Vector2) { 2000, 900 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 2000 + 15, 900 + 15, 60, 80 }, 40, 1);
						push_wave_out = false;
					}
					else if (wave_count == 3 && push_wave_out == true) {
						InitEnemy((Vector2) { 2000, 300 }, (Vector2) { 1.5f, 1.5f }, (Rectangle) { 2000 + 15, 300 + 15, 100, 100 }, 500, 4);
						push_wave_out = false;
					}
					else if (wave_count == 4 && push_wave_out == true) {
						InitEnemy((Vector2) { 2100, 200 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2100 + 15, 200 + 15, 60, 80 }, 40, 1);
						InitEnemy((Vector2) { 2075, 300 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2075 + 15, 300 + 15, 60, 80 }, 40, 1);
						InitEnemy((Vector2) { 2050, 400 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2050 + 15, 400 + 15, 60, 80 }, 20, 0);
						InitEnemy((Vector2) { 2000, 500 }, (Vector2) { 1, 0 }, (Rectangle) { 2000 + 15, 500 + 15, 60, 100 }, 200, 2);
						InitEnemy((Vector2) { 2000, 600 }, (Vector2) { 1, 0 }, (Rectangle) { 2000 + 15, 600 + 15, 60, 100 }, 200, 2);
						InitEnemy((Vector2) { 2050, 700 }, (Vector2) { 0.8f,0 }, (Rectangle) { 2050 + 15, 700 + 15, 60, 80 }, 20, 0);
						InitEnemy((Vector2) { 2075, 800 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2075 + 15, 800 + 15, 60, 80 }, 40, 1);
						InitEnemy((Vector2) { 2100, 900 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2100 + 15, 900 + 15, 60, 80 }, 40, 1);

						push_wave_out = false;
					}
					else if (wave_count == 5 && push_wave_out == true) {
						InitEnemy((Vector2) { 2000, 500 }, (Vector2) { 2, 2 }, (Rectangle) { 2000 + 15, 500 + 15, 100, 100 }, 500, 4);
						InitEnemy((Vector2) { 2000, 600 }, (Vector2) { 2, -2 }, (Rectangle) { 2000 + 15, 600 + 15, 100, 100 }, 500, 4);
						push_wave_out = false;
					}
					else if (wave_count == 6 && push_wave_out == true) {
						InitEnemy((Vector2) { 2050, 550 }, (Vector2) { 2, -1.2f }, (Rectangle) { 2050 + 15, 550 + 15, 100, 100 }, 500, 4);
						InitEnemy((Vector2) { 2000, 350 }, (Vector2) { 1, 0 }, (Rectangle) { 2000 + 15, 350 + 15, 60, 80 }, 200, 2);
						InitEnemy((Vector2) { 2000, 750 }, (Vector2) { 1, 0 }, (Rectangle) { 2000 + 15, 750 + 15, 100, 100 }, 200, 2);
						push_wave_out = false;
					}
					else if (wave_count == 7 && push_wave_out == true) { //Level 2 passed  
						counter++;
						if (counter >= 350) {
							if (counter == 350) {
								money_data += 7500;
							}
							menu_choice = ToggleMenu(7);
						}
						if (menu_choice != -1) {
							fade_scene_out = true;
							next_scene = menu_choice;
						}
						if (fade_scene_out) {
							if (FadeSceneOut()) {
								switch (next_scene) {
								case 0:
									menu_choice = -1;
									counter = 0;
									map_unlocked[2] = 1;
									chosen_map = 2; //entering new map
									wave_count = 0;
									counter = 0;
									push_wave_out = false;
									bg_pos = 0;
									button_pressed_counter = 0;
									RemoveAllEnemyBullets();
									RemoveAllPlayerBullets();
									InitPlayer(currentVehicle);
									restart_cooldown_timers = true; //restarting ability cooldowns
									fade_scene_out = false;
									toggle = false;
									break;
								case 1: //main menu
									menu_choice = -1;
									wave_count = 0;
									counter = 0;
									push_wave_out = false;
									bg_pos = 0;
									button_pressed_counter = 0;
									RemoveAllEnemies();
									RemoveAllEnemyBullets();
									RemoveAllPlayerBullets();
									map = -1;
									SetTargetFPS(60);
									restart_cooldown_timers = true;
									chosen_map = -1;
									currentScreen = MAIN_MENU;
									fade_scene_out = false;
									break;
								case 2: //settings
									menu_choice = -1;
									wave_count = 0;
									counter = 0;
									push_wave_out = false;
									bg_pos = 0;
									button_pressed_counter = 0;
									RemoveAllEnemies();
									RemoveAllEnemyBullets();
									RemoveAllPlayerBullets();
									restart_cooldown_timers = true;
									chosen_map = -1;
									currentScreen = SETTINGS;
									fade_scene_out = false;
									break;
								case 3:
									menu_choice = -1; // exit game
									RefreshGameFileData("game_data.txt");
									exit(0);
									break;
								}
							}
						}

					}
					if (GetNumOfEnemies() == 0 && counter >= 255 && wave_count < 7) {
						wave_count++;
						push_wave_out = true;
					}
					break;
					case 2:
						if (wave_count == 0 && counter == 255 && push_wave_out == true) {
							InitEnemy((Vector2) { 2000, 200 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 2000 + 15, 200 + 15, 60, 80 }, 20, 0);
							InitEnemy((Vector2) { 1950, 400 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 1950 + 15, 400 + 15, 60, 80 }, 20, 0);
							InitEnemy((Vector2) { 2200, 550 }, (Vector2) { 1, 0 }, (Rectangle) { 2200 + 15, 550 + 15, 60, 100 }, 200, 2);
							InitEnemy((Vector2) { 1950, 700 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 1950 + 15, 700 + 15, 60, 80 }, 20, 0);
							InitEnemy((Vector2) { 2000, 900 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 2000 + 15, 900 + 15, 60, 80 }, 20, 0);
							push_wave_out = false;
						}
						else if (wave_count == 1 && push_wave_out == true) {
							InitEnemy((Vector2) { 2075, 300 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2075 + 15, 300 + 15, 60, 80 }, 40, 1);
							InitEnemy((Vector2) { 2050, 400 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2050 + 15, 400 + 15, 60, 80}, 20, 0);
							InitEnemy((Vector2) { 2050, 700 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2050 + 15, 700 + 15, 60, 80 }, 20, 0);
							InitEnemy((Vector2) { 2075, 800 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2075 + 15, 800 + 15, 60, 80 }, 40, 1);
							InitEnemy((Vector2) { 2000, 450 }, (Vector2) { 1.5f, 1.5f }, (Rectangle) { 2000 + 15, 450 + 15, 100, 100 }, 200, 4);
							InitEnemy((Vector2) { 2000, 650 }, (Vector2) { 1.5f, -1.5f }, (Rectangle) { 2000 + 15, 650 + 15, 100, 100 }, 200, 4);
							
							push_wave_out = false;
						}
						else if (wave_count == 2 && push_wave_out == true) {
							InitEnemy((Vector2) { 2050, 550 }, (Vector2) { 2, -1.2f }, (Rectangle) { 2050 + 15, 550 + 15, 60, 100 }, 200, 2);
							InitEnemy((Vector2) { 2000, 350 }, (Vector2) { 1, 1.5f }, (Rectangle) { 2000 + 15, 350 + 15, 100, 100 }, 500, 4);
							InitEnemy((Vector2) { 2000, 750 }, (Vector2) { 1, -1.5f }, (Rectangle) { 2000 + 15, 750 + 15, 100, 100 }, 500, 4);
							push_wave_out = false;
						}
						else if (wave_count == 3 && push_wave_out == true) {
							InitEnemy((Vector2) { 2075, 300 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2075 + 15, 300 + 15, 60, 80 }, 40, 1);
							InitEnemy((Vector2) { 2050, 400 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2050 + 15, 400 + 15, 60, 80 }, 20, 0);
							InitEnemy((Vector2) { 2000, 500 }, (Vector2) { 2, 2 }, (Rectangle) { 2000 + 15, 500 + 15, 100, 100 }, 750, 5);
							InitEnemy((Vector2) { 2050, 700 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2050 + 15, 700 + 15, 60, 80 }, 20, 0);
							InitEnemy((Vector2) { 2075, 800 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2075 + 15, 800 + 15, 60, 80 }, 40, 1);
							push_wave_out = false;
						}
						else if (wave_count == 4 && push_wave_out == true) {
							InitEnemy((Vector2) { 2050, 550 }, (Vector2) { 2, -1.2f }, (Rectangle) { 2050 + 15, 550 + 15, 100, 100 }, 500, 4);
							InitEnemy((Vector2) { 2000, 350 }, (Vector2) { 1, 1 }, (Rectangle) { 2000 + 15, 350 + 15, 60, 80 }, 750, 5);
							InitEnemy((Vector2) { 2000, 750 }, (Vector2) { 1, 1 }, (Rectangle) { 2000 + 15, 750 + 15, 100, 100 }, 750, 5);

							push_wave_out = false;
						}
						else if (wave_count == 5 && push_wave_out == true) {
							InitEnemy((Vector2) { 2050, 450 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 450 + 15, 60, 80 }, 20, 0);
							InitEnemy((Vector2) { 2100, 550 }, (Vector2) { 1, 0 }, (Rectangle) { 2100 + 15, 550 + 15, 60, 80 }, 20, 0);
							InitEnemy((Vector2) { 2050, 650 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 650 + 15, 60, 80 }, 20, 0);
							InitEnemy((Vector2) { 2000, 800 }, (Vector2) { 0.8f, -1.5f }, (Rectangle) { 2000 + 15, 800 + 15, 100, 100 }, 500, 4);
							InitEnemy((Vector2) { 2000, 300 }, (Vector2) { 0.8f, 1.5f }, (Rectangle) { 2000 + 15, 300 + 15, 100, 100 }, 500, 4);
							push_wave_out = false;
						}
						else if (wave_count == 6 && push_wave_out == true) {
							InitEnemy((Vector2) { 2050, 350 }, (Vector2) { 2, 1.2f }, (Rectangle) { 2050 + 15, 350 + 15, 100, 100 }, 500, 4);
							InitEnemy((Vector2) { 2050, 550 }, (Vector2) { 2, -1.2f }, (Rectangle) { 2050 + 15, 550 + 15, 100, 100 }, 500, 4);
							InitEnemy((Vector2) { 2000, 750 }, (Vector2) { 1, 1 }, (Rectangle) { 2000 + 15, 750 + 15, 100, 100 }, 750, 5);
							InitEnemy((Vector2) { 2000, 350 }, (Vector2) { 1, 1 }, (Rectangle) { 2000 + 15, 350 + 15, 100, 100 }, 750, 5);
							
							push_wave_out = false;
						}
						else if (wave_count == 7 && push_wave_out == true) { //Level 3 passed  
							counter++;
							if (counter >= 350) {
								if (counter == 350) {
									money_data += 10000;
								}
								menu_choice = ToggleMenu(8);
							}
							if (menu_choice != -1) {
								fade_scene_out = true;
								next_scene = menu_choice;
							}
							if (fade_scene_out) {
								if (FadeSceneOut()) {
									switch (next_scene) {
									case 0:
										menu_choice = -1;
										counter = 0;
										map_unlocked[3] = 1;
										chosen_map = 3; //entering new map
										wave_count = 0;
										counter = 0;
										push_wave_out = false;
										bg_pos = 0;
										button_pressed_counter = 0;
										RemoveAllEnemyBullets();
										RemoveAllPlayerBullets();
										InitPlayer(currentVehicle);
										restart_cooldown_timers = true; //restarting ability cooldowns
										fade_scene_out = false;
										toggle = false;
										break;
									case 1: //main menu
										menu_choice = -1;
										wave_count = 0;
										counter = 0;
										push_wave_out = false;
										bg_pos = 0;
										button_pressed_counter = 0;
										RemoveAllEnemies();
										RemoveAllEnemyBullets();
										RemoveAllPlayerBullets();
										map = -1;
										SetTargetFPS(60);
										restart_cooldown_timers = true;
										chosen_map = -1;
										currentScreen = MAIN_MENU;
										fade_scene_out = false;
										break;
									case 2: //settings
										menu_choice = -1;
										wave_count = 0;
										counter = 0;
										push_wave_out = false;
										bg_pos = 0;
										button_pressed_counter = 0;
										RemoveAllEnemies();
										RemoveAllEnemyBullets();
										RemoveAllPlayerBullets();
										restart_cooldown_timers = true;
										chosen_map = -1;
										currentScreen = SETTINGS;
										fade_scene_out = false;
										break;
									case 3:
										menu_choice = -1; // exit game
										RefreshGameFileData("game_data.txt");
										exit(0);
										break;
									}
								}
							}

						}
						if (GetNumOfEnemies() == 0 && counter >= 255 && wave_count < 7) {
							wave_count++;
							push_wave_out = true;
						}
						break;
						case 3:
							if (wave_count == 0 && counter == 255 && push_wave_out == true) {
								InitEnemy((Vector2) { 2000, 200 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 2000 + 15, 200 + 15, 60, 80 }, 20, 0);
								InitEnemy((Vector2) { 1950, 400 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 1950 + 15, 400 + 15, 60, 100 }, 200, 2);
								InitEnemy((Vector2) { 2200, 550 }, (Vector2) { 1, 1 }, (Rectangle) { 2200 + 15, 550 + 15, 100, 100 }, 500, 4);
								InitEnemy((Vector2) { 2000, 900 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 2000 + 15, 900 + 15, 60, 100 }, 200, 2);
								InitEnemy((Vector2) { 1950, 700 }, (Vector2) { 1.5f, 0 }, (Rectangle) { 1950 + 15, 700 + 15, 60, 80 }, 20, 0);
								
								push_wave_out = false;
							}
							else if (wave_count == 1 && push_wave_out == true) {
								InitEnemy((Vector2) { 2075, 300 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2075 + 15, 300 + 15, 60, 100 }, 200, 2);
								InitEnemy((Vector2) { 2050, 400 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2050 + 15, 400 + 15, 60, 80 }, 20, 0);
								InitEnemy((Vector2) { 2000, 450 }, (Vector2) { 1.5f, 1.5f }, (Rectangle) { 2000 + 15, 450 + 15, 100, 100 }, 500, 4);
								InitEnemy((Vector2) { 2000, 350 }, (Vector2) { 1.5f, 1.5f }, (Rectangle) { 2000 + 15, 350 + 15, 100, 100 }, 750, 5);
								InitEnemy((Vector2) { 2050, 700 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2050 + 15, 700 + 15, 60, 80 }, 20, 0);
								InitEnemy((Vector2) { 2075, 800 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2075 + 15, 800 + 15, 60, 100 }, 200, 2);
								push_wave_out = false;
							}
							else if (wave_count == 2 && push_wave_out == true) {
								InitEnemy((Vector2) { 2000, 350 }, (Vector2) { 1, 1.5f }, (Rectangle) { 2000 + 15, 350 + 15, 100, 100 }, 500, 4);
								InitEnemy((Vector2) { 2050, 550 }, (Vector2) { 2, -1.2f }, (Rectangle) { 2050 + 15, 550 + 15, 60, 100 }, 200, 2);
								InitEnemy((Vector2) { 2000, 750 }, (Vector2) { 1, -1.5f }, (Rectangle) { 2000 + 15, 750 + 15, 100, 100 }, 500, 4);
								InitEnemy((Vector2) { 2075, 400 }, (Vector2) { 1, 1.5f }, (Rectangle) { 2075 + 15, 400 + 15, 60, 80 }, 20, 0);
								InitEnemy((Vector2) { 2075, 500 }, (Vector2) { 2, -1.2f }, (Rectangle) { 2075 + 15, 500 + 15, 60, 80 }, 40, 1);
								InitEnemy((Vector2) { 2075, 600 }, (Vector2) { 1, -1.5f }, (Rectangle) { 2075 + 15, 600 + 15, 60, 80 }, 20, 0);
								push_wave_out = false;
							}
							else if (wave_count == 3 && push_wave_out == true) {
								InitEnemy((Vector2) { 2075, 300 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2075 + 15, 300 + 15, 60, 80 }, 40, 1);
								InitEnemy((Vector2) { 2050, 400 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2050 + 15, 400 + 15, 60, 80 }, 20, 0);
								InitEnemy((Vector2) { 2000, 500 }, (Vector2) { 2, 2 }, (Rectangle) { 2000 + 15, 500 + 15, 100, 100 }, 750, 5);
								InitEnemy((Vector2) { 2050, 700 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2050 + 15, 700 + 15, 60, 80 }, 20, 0);
								InitEnemy((Vector2) { 2075, 800 }, (Vector2) { 0.8f, 0 }, (Rectangle) { 2075 + 15, 800 + 15, 60, 80 }, 20, 1);
								push_wave_out = false;
							}
							else if (wave_count == 4 && push_wave_out == true) {
								InitEnemy((Vector2) { 2000, 350 }, (Vector2) { 1, 1 }, (Rectangle) { 2000 + 15, 350 + 15, 100, 100 }, 750, 5);
								InitEnemy((Vector2) { 2100, 650 }, (Vector2) { 1, 0 }, (Rectangle) { 2100 + 15, 650 + 15, 60, 80 }, 40, 1);
								InitEnemy((Vector2) { 2050, 550 }, (Vector2) { 2, -1.2f }, (Rectangle) { 2050 + 15, 550 + 15, 100, 100 }, 500, 4);
								InitEnemy((Vector2) { 2100, 650 }, (Vector2) { 1, 0 }, (Rectangle) { 2100 + 15, 650 + 15, 60, 80 }, 40, 1);
								InitEnemy((Vector2) { 2000, 750 }, (Vector2) { 1, 1 }, (Rectangle) { 2000 + 15, 750 + 15, 100, 100 }, 750, 5);

								push_wave_out = false;
							}
							else if (wave_count == 5 && push_wave_out == true) {
								InitEnemy((Vector2) { 2000, 300 }, (Vector2) { 0.8f, 1.5f }, (Rectangle) { 2000 + 15, 300 + 15, 100, 100 }, 500, 4);
								InitEnemy((Vector2) { 2050, 450 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 450 + 15, 60, 80 }, 20, 0);
								InitEnemy((Vector2) { 2050, 550 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 550 + 15, 60, 80 }, 20, 0);
								InitEnemy((Vector2) { 2050, 650 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 650 + 15, 60, 80 }, 20, 0);
								InitEnemy((Vector2) { 2100, 450 }, (Vector2) { 1, 0 }, (Rectangle) { 2100 + 15, 450 + 15, 60, 80 }, 40, 1);
								InitEnemy((Vector2) { 2100, 550 }, (Vector2) { 1, 0 }, (Rectangle) { 2100 + 15, 550 + 15, 60, 80 }, 40, 1);
								InitEnemy((Vector2) { 2100, 650 }, (Vector2) { 1, 0 }, (Rectangle) { 2100 + 15, 650 + 15, 60, 80 }, 40, 1);
								InitEnemy((Vector2) { 2000, 800 }, (Vector2) { 0.8f, -1.5f }, (Rectangle) { 2000 + 15, 800 + 15, 100, 100 }, 500, 4);
								push_wave_out = false;
							}
							else if (wave_count == 6 && push_wave_out == true) {
								InitEnemy((Vector2) { 2050, 550 }, (Vector2) { 2, -1.2f }, (Rectangle) { 2050 + 15, 550 + 15, 200, 150 }, 1000, 3);
								InitEnemy((Vector2) { 2000, 750 }, (Vector2) { 1, 1 }, (Rectangle) { 2000 + 15, 750 + 15, 100, 100 }, 500, 4);
								InitEnemy((Vector2) { 2050, 350 }, (Vector2) { 2, 1.2f }, (Rectangle) { 2050 + 15, 350 + 15, 200, 150 }, 1000, 3);
								push_wave_out = false;
							}
							else if (wave_count == 7 && push_wave_out == true) { //Level 4 passed  
								counter++;
								if (counter >= 350) {
									if (counter == 350) {
										money_data += 10000;
									}
									menu_choice = ToggleMenu(9);
								}
								if (menu_choice != -1) {
									fade_scene_out = true;
									next_scene = menu_choice;
								}
								if (fade_scene_out) {
									if (FadeSceneOut()) {
										switch (next_scene) {
										case 0:
											menu_choice = -1;
											counter = 0;
											map_unlocked[4] = 1;
											chosen_map = 4; //entering new map
											wave_count = 0;
											counter = 0;
											push_wave_out = false;
											bg_pos = 0;
											button_pressed_counter = 0;
											RemoveAllEnemyBullets();
											RemoveAllPlayerBullets();
											InitPlayer(currentVehicle);
											restart_cooldown_timers = true; //restarting ability cooldowns
											fade_scene_out = false;
											toggle = false;
											break;
										case 1: //main menu
											menu_choice = -1;
											wave_count = 0;
											counter = 0;
											push_wave_out = false;
											bg_pos = 0;
											button_pressed_counter = 0;
											RemoveAllEnemies();
											RemoveAllEnemyBullets();
											RemoveAllPlayerBullets();
											map = -1;
											SetTargetFPS(60);
											restart_cooldown_timers = true;
											chosen_map = -1;
											currentScreen = MAIN_MENU;
											fade_scene_out = false;
											break;
										case 2: //settings
											menu_choice = -1;
											wave_count = 0;
											counter = 0;
											push_wave_out = false;
											bg_pos = 0;
											button_pressed_counter = 0;
											RemoveAllEnemies();
											RemoveAllEnemyBullets();
											RemoveAllPlayerBullets();
											restart_cooldown_timers = true;
											chosen_map = -1;
											currentScreen = SETTINGS;
											fade_scene_out = false;
											break;
										case 3:
											menu_choice = -1; // exit game
											RefreshGameFileData("game_data.txt");
											exit(0);
											break;
										}
									}
								}

							}
							if (GetNumOfEnemies() == 0 && counter >= 255 && wave_count < 7) {
								wave_count++;
								push_wave_out = true;
							}
							break;
								case 4:
									if (wave_count == 0 && counter == 255 && push_wave_out == true) {
										InitEnemy((Vector2) { 2000, 450 }, (Vector2) { 1.5f, 1.5f }, (Rectangle) { 2000 + 15, 450 + 15, 100, 100 }, 500, 4);
										InitEnemy((Vector2) { 2050, 350 }, (Vector2) { 2, -1.2f }, (Rectangle) { 2050 + 15, 350 + 15, 60, 100 }, 200, 2);
										InitEnemy((Vector2) { 2050, 550 }, (Vector2) { 2, -1.2f }, (Rectangle) { 2050 + 15, 550 + 15, 60, 100 }, 200, 2);
										push_wave_out = false;
									}
									else if (wave_count == 1 && push_wave_out == true) {
										InitEnemy((Vector2) { 2050, 250 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 250 + 15, 60, 80 }, 20, 0);
										InitEnemy((Vector2) { 2050, 350 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 350 + 15, 60, 80 }, 20, 0);
										InitEnemy((Vector2) { 2050, 750 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 750 + 15, 60, 80 }, 20, 0);
										InitEnemy((Vector2) { 2050, 850 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 850 + 15, 60, 80 }, 20, 0);
										InitEnemy((Vector2) { 2000, 550 }, (Vector2) { 1.5f, 1.5f }, (Rectangle) { 2000 + 15, 550 + 15, 100, 100 }, 750, 5);
										push_wave_out = false;
									}
									else if (wave_count == 2 && push_wave_out == true) {
										InitEnemy((Vector2) { 2000, 400 }, (Vector2) { 1.5f, 1.5f }, (Rectangle) { 2000 + 15, 400 + 15, 100, 100 }, 500, 4);
										InitEnemy((Vector2) { 2050, 550 }, (Vector2) { 2, -1.2f }, (Rectangle) { 2050 + 15, 550 + 15, 60, 100 }, 200, 2);
										InitEnemy((Vector2) { 2000, 650 }, (Vector2) { 1.5f, 1.5f }, (Rectangle) { 2000 + 15, 650 + 15, 100, 100 }, 750, 5);
										push_wave_out = false;
									}
									else if (wave_count == 3 && push_wave_out == true) {
										InitEnemy((Vector2) { 2050, 550 }, (Vector2) { 2, -1.2f }, (Rectangle) { 2050 + 15, 550 + 15, 60, 100 }, 200, 2);
										InitEnemy((Vector2) { 2150, 550 }, (Vector2) { 2.2f, -1.2f }, (Rectangle) { 2150 + 15, 550 + 15, 60, 100 }, 200, 2);
										InitEnemy((Vector2) { 2250, 550 }, (Vector2) { 2.4f, -1.2f }, (Rectangle) { 2250 + 15, 550 + 15, 60, 100 }, 200, 2);
										push_wave_out = false;
									}
									else if (wave_count == 4 && push_wave_out == true) {
										InitEnemy((Vector2) { 2050, 400 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 400 + 15, 60, 80 }, 20, 1);
										InitEnemy((Vector2) { 2100, 450 }, (Vector2) { 1, 0 }, (Rectangle) { 2100 + 15, 450 + 15, 60, 80 }, 20, 0);
										InitEnemy((Vector2) { 2050, 500 }, (Vector2) { 1, 0 }, (Rectangle) { 2050 + 15, 500 + 15, 60, 80 }, 20, 1);
										InitEnemy((Vector2) { 2200, 300 }, (Vector2) { 1.75f, 1.5f }, (Rectangle) { 2200 + 15, 300 + 15, 100, 100 }, 500, 4);
										InitEnemy((Vector2) { 2200, 600 }, (Vector2) { 1.75f, 1.5f }, (Rectangle) { 2200 + 15, 600 + 15, 100, 100 }, 500, 4);

										push_wave_out = false;
									}
									else if (wave_count == 5 && push_wave_out == true) {
										InitEnemy((Vector2) { 2000, 300 }, (Vector2) { 1.75f, 1.5f }, (Rectangle) { 2000 + 15, 300 + 15, 100, 100 }, 750, 5);
										InitEnemy((Vector2) { 2000, 500 }, (Vector2) { 1.75f, 1.5f }, (Rectangle) { 2000 + 15, 500 + 15, 100, 100 }, 750, 5);
										InitEnemy((Vector2) { 2100, 400 }, (Vector2) { 1.75f, 1.5f }, (Rectangle) { 2100 + 15, 400 + 15, 100, 100 }, 500, 4);
										InitEnemy((Vector2) { 2100, 600 }, (Vector2) { 2, -1.5f }, (Rectangle) { 2100 + 15, 600 + 15, 100, 100 }, 500, 4);
										push_wave_out = false;
									}
									else if (wave_count == 6 && push_wave_out == true) {
										InitEnemy((Vector2) { 2000, 550 }, (Vector2) { 1.5f, 1.5f }, (Rectangle) { 2000 + 15, 550 + 15, 200, 300 }, 2000, 6);
										InitEnemy((Vector2) { 2150, 350 }, (Vector2) { 1, 0 }, (Rectangle) { 2150 + 15, 350 + 15, 60, 80 }, 20, 0);
										InitEnemy((Vector2) { 2150, 750 }, (Vector2) { 1, 0 }, (Rectangle) { 2150 + 15, 750 + 15, 60, 80 }, 20, 0);
										InitEnemy((Vector2) { 2150, 850 }, (Vector2) { 1, 0 }, (Rectangle) { 2150 + 15, 850 + 15, 60, 80 }, 20, 0);
										push_wave_out = false;
									}
									else if (wave_count == 7 && push_wave_out == true) { //Level 4 passed  
										counter++;
										if (counter >= 350) {
											if (counter == 350) {
												money_data += 10000;
											}
											menu_choice = ToggleMenu(10);
										}
										if (menu_choice != -1) {
											fade_scene_out = true;
											next_scene = menu_choice;
										}
										if (fade_scene_out) {
											if (FadeSceneOut()) {
												switch (next_scene) {
												case 0:
													menu_choice = -1;
													counter = 0;
													map_unlocked[5] = 1;
													chosen_map = 4; //entering new map
													wave_count = 0;
													counter = 0;
													push_wave_out = false;
													bg_pos = 0;
													button_pressed_counter = 0;
													RemoveAllEnemyBullets();
													RemoveAllPlayerBullets();
													InitPlayer(currentVehicle);
													restart_cooldown_timers = true; //restarting ability cooldowns
													fade_scene_out = false;
													toggle = false;
													break;
												case 1: //main menu
													menu_choice = -1;
													wave_count = 0;
													counter = 0;
													push_wave_out = false;
													bg_pos = 0;
													button_pressed_counter = 0;
													RemoveAllEnemies();
													RemoveAllEnemyBullets();
													RemoveAllPlayerBullets();
													map = -1;
													SetTargetFPS(60);
													restart_cooldown_timers = true;
													chosen_map = -1;
													currentScreen = MAIN_MENU;
													fade_scene_out = false;
													break;
												case 2: //settings
													menu_choice = -1;
													wave_count = 0;
													counter = 0;
													push_wave_out = false;
													bg_pos = 0;
													button_pressed_counter = 0;
													RemoveAllEnemies();
													RemoveAllEnemyBullets();
													RemoveAllPlayerBullets();
													restart_cooldown_timers = true;
													chosen_map = -1;
													currentScreen = SETTINGS;
													fade_scene_out = false;
													break;
												case 3:
													menu_choice = -1; // exit game
													RefreshGameFileData("game_data.txt");
													exit(0);
													break;
												}
											}
										}

									}
									if (GetNumOfEnemies() == 0 && counter >= 255 && wave_count < 7) {
										wave_count++;
										push_wave_out = true;
									}
									break;
			}
				





			DrawPlayerHealth(currentVehicle);
			DrawTextureRec(coin, (Rectangle) { 60, 0, 19, 24 }, (Vector2) { 20 * 3, 90 }, WHITE);
			if (language == 0) {
				DrawTextEx(myFont, FormatText("Coins: %d", money_data), (Vector2) { 40 * 3 - 30, 100 }, 24, 0, (Color) { 6, 254, 140, 255 });
			}
			else {
				DrawTextEx(myFont, FormatText("Bodovi: %d", money_data), (Vector2) { 40 * 3 - 30, 100 }, 24, 0, (Color) { 6, 254, 140, 255 });
			}


			DrawFps();

			if (player.health > 0 && wave_count!=7 ) {//this menu mustn't interfere with one after the level was passed
				menu_choice = ToggleMenu(1);
				if (menu_choice != -1 ) { 
					fade_scene_out = true;
					next_scene = menu_choice;
				}
				if (next_scene == 0 && fade_scene_out) { //Resume option must be outside switch-case to avoid fade-out animation
					ResumeMusicStream(background_music);
					menu_choice = -1;
					fade_scene_out = false;
				}
				if (fade_scene_out) {
					if (FadeSceneOut()) {
						switch (next_scene) {
						case 1: //reseting game variables
							menu_choice = -1;
							wave_count = 0;
							counter = 0;
							push_wave_out = false;
							bg_pos = 0;
							button_pressed_counter = 0;
							RemoveAllEnemies();
							RemoveAllEnemyBullets();
							RemoveAllPlayerBullets();
							chosen_map = -1;
							SetTargetFPS(60);
							restart_cooldown_timers = true;
							currentScreen = MAIN_MENU;
							fade_scene_out = false;
							break;
						case 2:
							menu_choice = -1;
							wave_count = 0;
							counter = 0;
							push_wave_out = false;
							bg_pos = 0;
							button_pressed_counter = 0;
							RemoveAllEnemies();
							RemoveAllEnemyBullets();
							RemoveAllPlayerBullets();
							restart_cooldown_timers = true;
							currentScreen = SETTINGS;
							chosen_map = -1;
							fade_scene_out = false;
							break;
						case 3:
							menu_choice = -1;
							RefreshGameFileData("game_data.txt");
							exit(0);
							break;
						}
					}
				}
			}
			else if(player.health<0){ 
				menu_choice = ToggleMenu(2);
				if (menu_choice != -1) {
					fade_scene_out = true;
					next_scene = menu_choice;
				}
				if (fade_scene_out) {
					if (FadeSceneOut()) {
						switch (next_scene) {
						case 0: //reseting all variables
							RemoveAllEnemies();
							RemoveAllEnemyBullets();
							RemoveAllPlayerBullets();
							InitPlayer(currentVehicle);
							restart_cooldown_timers = true;
							wave_count = 0;
							push_wave_out = true;
							counter = 0;
							bg_pos = 0;
							fade_scene_out = false;
							toggle = false;
							break;
						case 1:
							menu_choice = -1;
							wave_count = 0;
							counter = 0;
							push_wave_out = false;
							bg_pos = 0;
							button_pressed_counter = 0;
							RemoveAllEnemies();
							RemoveAllEnemyBullets();
							RemoveAllPlayerBullets();
							restart_cooldown_timers = true;
							currentScreen = SETTINGS;
							chosen_map = -1;
							fade_scene_out = false;
							break;
						case 2:
							menu_choice = -1;
							wave_count = 0;
							counter = 0;
							push_wave_out = false;
							bg_pos = 0;
							button_pressed_counter = 0;
							RemoveAllEnemies();
							RemoveAllEnemyBullets();
							RemoveAllPlayerBullets();
							button_pressed_counter = 0;
							restart_cooldown_timers = true;
							SetTargetFPS(60);
							currentScreen = MAIN_MENU;
							chosen_map = -1;
							fade_scene_out = false;
							break;
						case 3:
							RefreshGameFileData("game_data.txt"); // save collected coins
							exit(0);
							break;
						}
					}
				}
			}

			EndMode2D();
			
		}
		//Brightness setting
		DrawRectangle(0, 0, 1920, 1080, (Color) { 0,0,0, 255-brightness});
		EndDrawing();
	}

	UnloadBullets();
	UnloadCoins();

	//Unloading all of the sounds, music and textures previously loaded into GPU
	UnloadTexture(coin);
	UnloadTexture(lightning);
	UnloadTexture(enemy_texture1);
	UnloadTexture(enemy_texture2);
	UnloadTexture(enemy_texture3);
	UnloadTexture(enemy_texture4);
	UnloadTexture(enemy_texture5);
	UnloadTexture(star_dust);
	UnloadTexture(mystery_ability);
	UnloadTexture(message);
	UnloadTexture(flame_texture);
	UnloadTexture(bg);
	UnloadTexture(ellipse);
	UnloadTexture(powerful_laser);
	UnloadTexture(rotated_texture[0]);
	UnloadTexture(rotated_texture[1]);
	UnloadTexture(disabling_line);
	UnloadTexture(unlock_button);
	UnloadTexture(selected_button);
	UnloadTexture(unlock_button);
	UnloadTexture(message);
	UnloadTexture(upgrades);
	UnloadTexture(engine_thrust);
	UnloadTexture(locked_ability);
	UnloadTexture(max);
	UnloadTexture(open);
	UnloadTexture(start);
	UnloadTexture(middle);
	UnloadTexture(end);
	UnloadTexture(enemy_exhaust);
	UnloadTexture(main_menu_button);
	UnloadTexture(health);
	UnloadTexture(health2);
	UnloadTexture(explosion[0]);
	UnloadTexture(explosion[1]);
	UnloadTexture(medium_explosion);
	UnloadTexture(shield);
	UnloadTexture(shield_particles);
	UnloadTexture(level_beam);
	UnloadMusicStream(background_music);
	UnloadSound(button_click);
	UnloadSound(button_click2);
	UnloadSound(blaster);
	UnloadSound(machinegun);
	UnloadSound(bullet1_sound);
	UnloadSound(upgraded_sound);
	UnloadSound(flamethrower);
	UnloadSound(laser_beam);
	UnloadSound(disabling_laser);
	UnloadSound(shockwave_sound);

	for (int i = 0; i < 3; i++) {;
		UnloadTexture(enemy_bullet_texture[i]);
		UnloadTexture(spaceship[i]);
		UnloadTexture(laser[i]);
		UnloadTexture(spaceship_scaled[i]);
		for (int j = 0; j < 2; j++) {
			UnloadTexture(abilities[i][j]);
		}
	}
	for (int i = 0; i < 17; i++) {
		UnloadTexture(planets[i]);
	}
	UnloadFont(myFont);
	RefreshGameFileData("game_data.txt");
	fclose(fp);
	CloseWindow();
	return 0;
}

