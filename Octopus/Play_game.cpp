#include "Play_game.h"
Game::Game()
{
	window = NULL;
	Renderer = NULL;
	Char = NULL;
	bg_ = MENU, alp = 255, dis = 1, dis_l = 1, cur_point = 0;
	cur_power = 16;
	MapType = '1';
	Layer1 = NULL;
	Layer2 = NULL;
}

Game::~Game()
{
	close();
}
bool Game::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 4);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: \n");
			success = false;
		}
		else
		{
			//Get window surface
			//ScreenSurface = SDL_GetWindowSurface(window);
			Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (Renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}

		}

	}
	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		success = false;
	}
	//Initialize SDL_ttf
	 if( TTF_Init() == -1 )
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	 //Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	return success;
}
bool Game::MediaLoad()
{
	bool success = true;
	//load map
	int n, m;
	std::ifstream fin("side.txt");
	if (!fin.is_open())
	{
		std::cout << "Can't open file side.txt";
		success = false;
	}
	else
	{
		fin >> n >> m;
		generate_layer3(n, m);

	}
	//load background texture
	for (int i = 0; i < TOTAL_SCREEN_; ++i)
		BackgroundTexture[i].setRenderer(Renderer);
	if (BackgroundTexture[MENU].loadFromFile("image/MENU.png") == 0)
	{
		printf("Failed to load background texture image!\n");
		success = false;
	}
	if (BackgroundTexture[MAIN].loadFromFile("image/th.jpg") == 0)
	{
		printf("Failed to load background texture image!\n");
		success = false;
	}
	if (BackgroundTexture[LEVEL].loadFromFile("image/LEVEL.png") == 0)
	{
		printf("Failed to load background texture image!\n");
		success = false;
	}
	//load character texture
	Char = new std::pair<MyCharacter, MyText>;
	Char->first = MyCharacter(500, 120);
	Char->first.setRenderer(Renderer);
	Char->second.setRenderer(Renderer);
	Char->second.setFont(10, "font/lazy.ttf");
	Char->second.loadFromRenderedText(std::to_string(Char->first.getPower()), { 255,255,255 });
	if (Char->first.loadFromFile("image/ch0.png") == 0)
	{
		printf("Failed to load character texture image!\n");
		success = false;
	}
	//Time
	time = new MyText;
	time->setRenderer(Renderer);
	time->setFont(20, "font/lazy.ttf");
	//num
	Num = new MyText;
	Num->setRenderer(Renderer);
	Num->setFont(20, "font/lazy.ttf");
	//load curState
	curState = new MyText;
	curState->setRenderer(Renderer);
	curState->setFont(50, "font/lazy.ttf");

	//load monster
	for (int i = 0; i < 10; i++)
	{
		std::pair<MyCharacter, MyText>* temp = new std::pair<MyCharacter, MyText>;
		temp->first = MyCharacter(70 + rand() % 32, 40 + i * 64);
		temp->first.setRenderer(Renderer);
		temp->first.changePower(rand() % 10 + 1);
		temp->second.setRenderer(Renderer);
		temp->second.setFont(8, "font/lazy.ttf");
		temp->second.loadFromRenderedText(std::to_string(temp->first.getPower()), { 255,255,255 });
		std::string s = "image/ch";
		std::string num = std::to_string(i % 4 + 1);
		//while (num.size() != 2)num = "0" + num;
		s += num;
		s += ".png";
		if (temp->first.loadFromFile(s,atoi(num.c_str())) == 0)
		{
			printf("Failed to load monster texture image!\n");
			success = false;
		}
		if (temp != NULL)
			Monster.push_back(temp);
		else
		{
			printf("Failed to load monster texture image!\n");
		}
	}
	
	//load button texture
	for (int i = 0; i < TOTAL_BUTTONS; ++i)
	{
		std::pair<MyButton, MyText>* temp = new std::pair<MyButton, MyText>;
		temp->first.setRenderer(Renderer);
		temp->second.setRenderer(Renderer);
		temp->second.setFont(25, "font/lazy.ttf");
		switch (i)
		{
		case play_:
			if (temp->second.loadFromRenderedText("Play", { 255,255,255 }) == 0)
			{
				printf("Failed to load text texture image!\n");
				success = false;
			}
			if (temp->first.loadFromFile("image/green_button00.png") == 0)
			{
				printf("1Failed to load button texture image!\n");
				success = false;
			}
			break;
		case level_:
			if (temp->second.loadFromRenderedText("Level", { 255,255,255 }) == 0)
			{
				printf("Failed to load text texture image!\n");
				success = false;
			}
			if (temp->first.loadFromFile("image/green_button00.png") == 0)
			{
				printf("3Failed to load button texture image!\n");
				success = false;
			}
			break;
		case easy_:
			if (temp->second.loadFromRenderedText("Easy", { 255,255,255 }) == 0)
			{
				printf("Failed to load text texture image!\n");
				success = false;
			}
			if (temp->first.loadFromFile("image/green_button00.png") == 0)
			{
				printf("4Failed to load button texture image!\n");
				success = false;
			}
			break;
			case normal_:
				if (temp->second.loadFromRenderedText("Normal", { 255,255,255 }) == 0)
				{
				printf("Failed to load text texture image!\n");
				success = false;
			}
				if (temp->first.loadFromFile("image/green_button00.png") == 0)
				{
				printf("4Failed to load button texture image!\n");
				success = false;
			}
				break;
				case hard_:
					if (temp->second.loadFromRenderedText("Hard", { 255,255,255 }) == 0)
					{
							printf("Failed to load text texture image!\n");
					}
					if (temp->first.loadFromFile("image/green_button00.png") == 0)
					{
							printf("4Failed to load button texture image!\n");
					}
					break;

		case quit_:
			if (temp->second.loadFromRenderedText("Quit", { 255,255,255 }) == 0)
			{


				//"D:\lib\DoP\Octopus\image/green_button00.png"
				printf("Failed to load text texture image!\n");
				success = false;
			}
			if (temp->first.loadFromFile("image/green_button00.png") == 0)
			{
				printf("2Failed to load button texture image!\n");
				success = false;
			}
			break;
		case back_:
			if (temp->first.loadFromFile("image/back1.png") == 0)
			{
				printf("5Failed to load button texture image!\n");
				success = false;
			}
			break;
		default:
			break;
		}

		if (temp != NULL)Button.push_back(temp);
		else
		{
			printf("Failed to load button texture image!\n");
		}
	}
	//load clip
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			Clip[i * 4 + j].x = j * 32;
			Clip[i * 4 + j].y = i * 32;
			Clip[i * 4 + j].w = 32;
			Clip[i * 4 + j].h = 32;
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			Clip_[i * 8 + j].x = j * 32;
			Clip_[i * 8 + j].y = i * 25;
			Clip_[i * 8 + j].w = 32;
			Clip_[i * 8 + j].h = 25;
		}
	}
	
	//Load tile texture
	Layer2 = new MyTexture * [n];
	for (int i = 0; i < n; ++i)
	{
		Layer2[i] = new MyTexture[m];
	}

	//CreateMap("image/map.txt", Layer1, M_H_1, M_W_1, MapType); 
	CreateMap("image/map.txt", Layer2, n, m, MapType);
	
	//Load music
	gMusic = Mix_LoadMUS("mixer/music.wav");
	if (gMusic == NULL)
	{
		printf("Failed to load music music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	
	//Load sound efftects
	move = Mix_LoadWAV("mixer/move.wav");
	if (move == NULL)
	{
		printf("Failed to load move sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	click = Mix_LoadWAV("mixer/click.wav");
	if (click == NULL)
	{
		printf("Failed to load click sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	win_sound = Mix_LoadWAV("mixer/win.wav");
	if (win_sound == NULL)
	{
		printf("Failed to load win sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	lose_sound = Mix_LoadWAV("mixer/lose.wav");
	if (lose_sound == NULL)
	{
		printf("Failed to load lose sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	return success;

}
void Game::CreateMap(const std::string& Path, MyTexture** Layer, int MAP_H, int MAP_W, char map_)
{
	//map_ is type of map
	std::ifstream fin(Path.c_str());
	if (fin.is_open())
	{
		for (int i = 0; i < MAP_H; ++i)
		{
			for (int j = 0; j < MAP_W; ++j)
			{
				Layer[i][j].setRenderer(Renderer);
				int v;
				fin >> v;
				std::string value = std::to_string(v);
				//std::cout << value;
				std::string s = "image/tile_bg/map/tile_00";
				//s += map_;
				//std::cout << s << '\n';
				while (value.size() != 2)value = "0" + value;
				s += value;
				s += ".png";

				if (v != 0 && v != -1)
				{
					//std::cout << s << '\n';
					if (Layer[i][j].loadFromFile(s, v) == 0)
					{
						std::cout << s << '\n';
						printf("Failed to load tile texture image!\n");
					}
					//std::cout << s << '\n';
				}
				else
				{
					Layer[i][j].loadFromFile(s, -1);
				}

			}
		}
	}
	else
	{
		printf("Failed to open map file!\n");
	}
}
void Game::handleInput()
{
	//SDL_Event e;
	int CharFrame = 5;
	//bool quit = false;
	Mix_PlayMusic(gMusic, -1);  //Play the music
	while (!quit)
	{
		if (bg_ == 1)
		{
			Mix_PlayMusic(gMusic, -1);
		}
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				for (int i = 0; i < Button.size(); ++i)
				{
					Mix_PlayChannel(-1, click, 0);
					if (Button[i]->first.handleEvent(e) == true)
					{
						if (bg_ == MENU)
						{
							switch (i)
							{
							case play_:
								if (Char == NULL)
								{
									/*Char->first = MyCharacter(70,120);
									Char->first.setRenderer(Renderer);*/
									MediaLoad();
								}
								bg_ = MAIN;
								break;
							case quit_:
								quit = true;
								break;
							case level_:
								bg_ = LEVEL;
								break;
							}

						}
						if (bg_ == LEVEL)
						{
							switch (i)
							{
								case easy_:
									gapTime = 60;
									break;
								case normal_:
									gapTime = 40;
									break;
								case hard_:
									gapTime = 30;
									break;
							}
						}
						switch (i)
						{
						
						case back_:
							switch (bg_)
							{
							
							case MAIN:
								bg_ = MENU;
								break;
							case GAME_OVER:
								bg_ = MENU;
								break;
							case LEVEL:
								bg_ = MENU;
								break;
							}
							break;
						default:
							break;
						}
					}
				}
			}

		}
		SDL_RenderClear(Renderer);
		SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		bg_ = std::max(0, bg_);
		bg_ %= TOTAL_SCREEN_;
		SDL_Rect clip = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
		BackgroundTexture[bg_].TShow(0, 0, &clip);
		//std::cout << bg_ << '\n';
		if (bg_ == MENU)
		{
			//std::cout << Button.size() << '\n';
			Button[play_]->first.TShow(200, 350);
			/*Button[skill_]->first.TShow(200, 420);
			Button[char_]->first.TShow(200, 490);*/
			Button[level_]->first.TShow(200, 420);
			Button[quit_]->first.TShow(200, 490);
			
			Button[play_]->second.TShow(250, 360);
			/*Button[skill_]->second.TShow(250, 430);
			Button[char_]->second.TShow(250, 500);*/
			Button[quit_]->second.TShow(250, 500);
			
			Button[level_]->second.TShow(250, 430);
		}
		if (bg_ == LEVEL)
		{
			Button[back_]->first.TShow(20, 570);
			Button[back_]->second.TShow(20, 570);
			Button[easy_]->first.TShow(200, 350);
			Button[normal_]->first.TShow(200, 420);
			Button[hard_]->first.TShow(200, 490);
			Button[easy_]->second.TShow(230, 360);
			Button[normal_]->second.TShow(230, 430);
			Button[hard_]->second.TShow(230, 500);

		}
		if (Char != NULL && bg_ == MAIN)
		{
			if (Char != NULL)
			{
				//show map
				for (int i = 0; i < M_H_2; ++i)
				{
					for (int j = 0; j < M_W_2; ++j)
					{
						Layer2[i][j].TShow(j * 16 + 30, i * 16 + 30);
					}
				}
				CharFrame++;
				CharFrame %= 21;
				monsterFrame++;
				monsterFrame %= 16;
				Button[back_]->first.TShow(20, 570);
				int x = 0, y = 0;
				int mx;
				int my;
				SDL_GetMouseState(&mx, &my);
				x = mx - Char->first.getX();
				y = my - Char->first.getY();
				x /= 29;
				y /= 29;
				for (int i = 0; i < Monster.size(); ++i)
				{
					int x_m = Monster[i]->first.getX();
					int y_m = Monster[i]->first.getY();
					if (x_m < 30 || x_m > 16 * M_W_2 || y_m < 3 || y_m > 16 * M_H_2)
					{
						Monster.erase(Monster.begin() + i);
					}
					else
					{
						x_m += rand() % 4;
						y_m += rand() % 2;
						Monster[i]->first.TShow(x_m, y_m, &Clip[monsterFrame]);
						Monster[i]->second.TShow(x_m, y_m);

					}
				}
				//add more Monster
				if (Monster.size() <= 5)
				{
					cnt %= SCREEN_HEIGHT;
					std::pair<MyCharacter, MyText>* temp = new std::pair<MyCharacter, MyText>;
					temp->first = MyCharacter(70, cnt);
					cnt += 100;
					temp->first.setRenderer(Renderer);
					temp->first.changePower(rand() % 10 + 1);
					temp->second.setRenderer(Renderer);
					temp->second.setFont(10, "font/lazy.ttf");
					temp->second.loadFromRenderedText(std::to_string(temp->first.getPower()), { 255,255,255 });
					std::string s = "image/ch";
					std::string num = std::to_string(rand() % 4 + 1);
					//while (num.size() != 2)num = "0" + num;
					s += num;
					s += ".png";
					if (temp->first.loadFromFile(s,atoi(num.c_str())) == 0)
					{
						printf("Failed to load monster texture image!\n");

					}
					if (temp != NULL)
						Monster.push_back(temp);
					else
					{
						printf("Failed to load monster texture image!\n");
					}
				}
				//show character
				Char->first.Update(Layer2, x, y);
				Char->first.TShow(-1, -1, &Clip_[CharFrame]);
				Char->second.loadFromRenderedText(std::to_string(Char->first.getPower()), { 255,255,255 });
				Char->second.TShow(Char->first.getX(), Char->first.getY());
				//show other
				for (int i = 0; i < Monster.size(); i++)
				{
					int x_m = Monster[i]->first.getX();
					int y_m = Monster[i]->first.getY();
					int x_c = Char->first.getX() ;
					int y_c = Char->first.getY() ;
					int Power = Char->first.getPower();
					int monsterPower = Monster[i]->first.getPower();
					int monsterType = Monster[i]->first.getType();	
					switch (monsterType)
					{
						case 1:
							Power += monsterPower;
							break;
						case 2:
							Power *= monsterPower;
							break;
						case 3:
							Power -= monsterPower;
							break;
						case 4:
							Power %= monsterPower;
							Power = abs(Power);
							break;
						default:
							break;
					}
				//Power %= monsterPower;
					//check xm ym
					//std::cout << Monster[i]->first.getPower() << '\n';
					if(x_c <= x_m && x_m <= x_c + 32 && y_c <= y_m && y_m <= y_c + 25)
					{
						Mix_PlayChannel(-1, move, 0);
						Char->first.changePower(Power);Monster.erase(Monster.begin() + i);
					}
					//check xm +32 ym
					else if (x_c <= x_m + 32 && x_m + 32 <= x_c + 32 && y_c <= y_m && y_m <= y_c + 25)
					{
						Mix_PlayChannel(-1, move, 0);
						Char->first.changePower(Power);Monster.erase(Monster.begin() + i);
					}
					//check xm ym + 32
					else if(x_c <= x_m && x_m <= x_c + 32 && y_c <= y_m + 25 && y_m + 25 <= y_c + 25)
					{
						Mix_PlayChannel(-1, move, 0);
						Char->first.changePower(Power);Monster.erase(Monster.begin() + i);
					}
					//check xm + 32 ym + 32
					else if (x_c <= x_m + 32 && x_m + 32 <= x_c + 32 && y_c <= y_m + 25 && y_m + 25 <= y_c + 25)
					{
						Mix_PlayChannel(-1, move, 0);
						Char->first.changePower(Power);Monster.erase(Monster.begin() + i);
					}
				}
				
				//create and render num
				if (curNum == -1)
				{
					curNum = rand() % magicNum + 1;
				}
				Num->loadFromRenderedText(std::to_string(curNum), { 255,255,255 });
				Num->TShow(450, 600);
				
				//show time
				time->loadFromRenderedText(std::to_string(timeLimit - (curTime = SDL_GetTicks() / 1000)), { 255,255,0 });
				time->TShow(250, 600);
				SDL_Delay(50);
				//random num
				if (curTime - passTime >= gapTime)
				{
					passTime = curTime;
					curNum = rand() % magicNum + 1;
					//std::cout << gapTime << '\n';
				}
				else if (timeLimit - curTime <= 0)
				{
					Mix_PlayChannel(-1, lose_sound, 0);
					delete Char;
					Char = NULL;
					curState->loadFromRenderedText("YOU LOOSE", { 255,255,255 });
					//curState->TShow(400, 300);
				}
				else {
					if (Char->first.getPower() == curNum)
					{
						Mix_PlayChannel(-1, win_sound, 0);
						delete Char;
						Char = NULL;
						curState->loadFromRenderedText("YOU WIN", { 255,255,255 });
						
					}
				}
			}
			
		}
		curState->TShow(400, 300);
		SDL_RenderPresent(Renderer);
	}

	//Button[].TShow(200, 420);




}
void Game::close()
{

	//Destroy window	
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	Renderer = NULL;

	//Free the sound effects
	Mix_FreeChunk(click);
    Mix_FreeChunk(move);
	click = NULL;
	move = NULL;

	//Free the music
	Mix_FreeMusic(gMusic);
	gMusic = NULL;

	Char = NULL;
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
}
