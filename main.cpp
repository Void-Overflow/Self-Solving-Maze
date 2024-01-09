#define SDL_MAIN_HANDLED

#define APPLICATION_WIDTH_IN_PIXELS 800
#define APPLICATION_HEIGHT_IN_PIXELS 600

#include "texture.h"
#include "aStarPathFinding.h"

int main() {
	SDL_Event event;
	SDL_Window* window = SDL_CreateWindow("Self-Solving Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, APPLICATION_WIDTH_IN_PIXELS, APPLICATION_HEIGHT_IN_PIXELS, SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	SDL_SetWindowResizable(window, SDL_FALSE);

	texture menuTexts(renderer, { // ({txt, x, y, h, w, size, clr}, detectMouse, decrementAmt(})
		std::make_tuple(ui("Maze Solver", 200, 0, 125, 350, 50, {0,0,0}), false, NULL), //title
		std::make_tuple(ui("Self-Solve", 75, 200, 100, 300, 50, { 155, 75, 20 }), true, 25), //selfSolving
		std::make_tuple(ui("Manuel-Solve", 425, 200, 100, 300, 50, { 20,75,155 }), true, 25), //manuelSolving
		std::make_tuple(ui("Select Difficulty: ",25, 375, 55, 400, 50, { 85,85,85 }), false, NULL), //difficultyText
		std::make_tuple(ui("Easy", 75, 450, 70, 150, 50, { 20, 200, 75, 255 }), true, 18), //easyDifficultyOption
		std::make_tuple(ui("Medium", 300, 450, 70, 200, 50, {200 ,80,55, 100 }), true, 18), //mediumDifficultyOption
		std::make_tuple(ui("Hard", 575, 450, 70, 150, 50, {200 ,80,55, 100 }), true, 18) }); //hardDifficultyOption


	maze::difficulty hardness = maze::easy;
	maze map(renderer, APPLICATION_HEIGHT_IN_PIXELS, APPLICATION_WIDTH_IN_PIXELS);

	aStarPathFinding a_star;

	short screen = 0;
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	int previousBtnPress = 0;

	while (true) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);   
		switch (screen) {
		default:
		case 0: { //menu
			menuTexts.initialUpdate();
			int usrInterface = menuTexts.checkInterfaces();

			if (usrInterface == 1)
				screen = 1;
			else if (usrInterface == 2)
				screen = 2;
			else if (usrInterface > 3) {
				menuTexts.changeAttributes(4, texture::attributes::color, NULL, { 200 ,80,55, 100 });
				menuTexts.changeAttributes(5, texture::attributes::color, NULL, { 200 ,80,55, 100 });
				menuTexts.changeAttributes(6, texture::attributes::color, NULL, { 200 ,80,55, 100 });
				if (usrInterface == 4) {
					hardness = maze::easy;
					menuTexts.changeAttributes(4, texture::attributes::color, NULL, { 20, 200, 75, 255 });
				}
				else if (usrInterface == 5) {
					hardness = maze::medium;
					menuTexts.changeAttributes(5, texture::attributes::color, NULL, { 20, 200, 75, 255 });
				}
				else if (usrInterface == 6) {
					hardness = maze::hard;
					menuTexts.changeAttributes(6, texture::attributes::color, NULL, { 20, 200, 75, 255 });
				}
			}

			break;
			}
		case 1: //self-solving maze
			if(map.generateRandomMaze(hardness, std::make_pair(APPLICATION_WIDTH_IN_PIXELS / 20, APPLICATION_HEIGHT_IN_PIXELS / 20)))
				a_star = aStarPathFinding(map, std::make_pair(0, 0));
			a_star.nextFeasableNode();
			break;
		case 2: //user-solving maze
			map.generateRandomMaze(hardness, std::make_pair(APPLICATION_WIDTH_IN_PIXELS/20, APPLICATION_HEIGHT_IN_PIXELS/20));

			if (SDL_GetTicks() - previousBtnPress >= 250) {
				if (keyState[SDL_SCANCODE_DOWN])
					map.moveUsrCursor(NULL, -1);
				else if (keyState[SDL_SCANCODE_RIGHT])
					map.moveUsrCursor(1, NULL);
				else if (keyState[SDL_SCANCODE_UP])
					map.moveUsrCursor(NULL, 1);
				else if (keyState[SDL_SCANCODE_LEFT])
					map.moveUsrCursor(-1, NULL);
				else if(keyState[SDL_SCANCODE_BACKSPACE])
					map.moveUsrCursor(-1, -1);

				previousBtnPress = SDL_GetTicks();
			}
			break;
		}

		SDL_RenderPresent(renderer);
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}