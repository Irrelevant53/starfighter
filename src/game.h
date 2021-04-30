#include <raylib.h>
#include "animations.h"


#ifndef GAME_H
#define GAME_H

typedef enum GameScreen {
	MAIN_MENU = 0,
	MAP_CHOICE,
	PLAY,
	SETTINGS,
	VEHICLE_CHOICE,
	ABOUT,
	GAME,
	RAYLIB_LOGO,
}GameScreen;

Texture2D enemy_texture1,enemy_texture2,enemy_texture3,enemy_texture4,enemy_texture5;
Texture2D spaceship[3],boss1,boss2;
Texture2D laser[3],powerful_laser;
Texture2D map_image,level_beam,enemy_bullet_texture[3];
Texture2D star_dust,coin,ellipse,vehicle_ellipse;
Texture2D anim_down,lightning, spaceship_scaled[3];
Texture2D rotated_texture[2], explosion[2],medium_explosion,boss_explosion;
Texture2D end, start, middle,shield,shield_particles;
Texture2D disabling_line,engine_thrust,enemy_exhaust,enemy_exhaust2;
Texture2D upgrades,special,abilities[3][2];
Texture2D main_menu_button,unlock_button,selected_button;
Texture2D message,price[5],max,mystery_ability,open;
Texture2D locked_ability,flame_texture,health,health2;


Font myFont;
Sound machinegun,blaster,bullet1_sound,upgraded_sound,flamethrower,laser_beam,disabling_laser,shockwave_sound,small_explosion,medium_explosion_sound;
Music background_music;
Camera2D camera;
Animation level_beam_animation;

#endif
