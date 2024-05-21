#ifndef HEADER_H_
#define HEADER_H_
#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <fstream>	
#include <map>
#include <utility>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <random>
#include <set>
#include <queue>
#include <stack>


//Screen dimension constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 675;
//Key press surfaces constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
//Tile dimension contants
const int TILE_LY1 = 64;
const int TILE_LY2 = 32;
const int M_W_1 = 20;
const int M_H_1 = 11;
const int M_W_2 = 70;
const int M_H_2 = 35;
const int WalkLeft = -5;
const int WalkRight = 5;
const int WalkUp = -5;
const int WalkDown = 5;
const int monster_move = 6;
const int Char_WIDTH = 60;
const int Char_HEIGHT = 64;
static int RowNum = 0;
enum Screen_
{
	MENU,
	MAIN,
	LEVEL,
	GAME_OVER,
	TOTAL_SCREEN_
};
enum Buttons
{
	play_,
	quit_,
	back_,
	level_,
	easy_,
	normal_,
	hard_,
	TOTAL_BUTTONS

};
class MyTexture
{
public:
	//Initializes variables
	MyTexture( int x = 0, int y = 0);

	//Deallocates memory
	~MyTexture();

	//Deallocates texture
	void free();

	//set renderer
	void setRenderer(SDL_Renderer* _renderer);
	//Loads image at specified path
	bool loadFromFile(std::string path = "", int value = 0);

	//Renders texture at given point
	void TShow(double x = -1, double y = -1, SDL_Rect* clip = NULL);

	//Gets image dimensions
	int getWidth();
	int getHeight();
	void setAlpha(Uint8 alpha);
	//Get type of texture
	int getType() { return type; }
	//return texture
	SDL_Texture* getTexture() { return Texture; }
protected:
	//The actual hardware texture
	SDL_Texture* Texture;
	SDL_Renderer* renderer;
	//Image dimensions
	double pos_x;
	double pos_y;
	int Width;
	int Height;
	int type;
	

};

static void CreateMap(const std::string &Path, MyTexture **Layer, int MAP_H, int MAP_W, char _map);
static Mix_Music* gMusic = NULL;
static Mix_Chunk* click = NULL;
static Mix_Chunk* move = NULL;
static Mix_Chunk* win_sound = NULL;
static Mix_Chunk* lose_sound = NULL;
#endif