#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>


#include <exception>
#include <iostream>
#include <string>

//Game general information
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


struct position
{
	int x;
	int y;
	position() :x(0), y(0) {};
};

bool checkCursorCollision(position _playerTarget, SDL_Rect textRect)
{

	
	int  textTop,textBottom,textLeft, textRight; 

	
	
	textTop = textRect.y;
	textLeft = textRect.x;
	textBottom = textRect.y + textRect.h; 
	textRight = textRect.x + textRect.w; 

	if (_playerTarget.x <= textLeft) return false;
	if (_playerTarget.x >= textRight) return false;
	if (_playerTarget.y>= textBottom) return false;
	if (_playerTarget.y<= textTop) return false;

	else return true; 
}


int main(int, char*[])
{
	// --- INIT SDL ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		throw "No es pot inicialitzar SDL subsystems";
	
	// --- WINDOW ---
	SDL_Window *m_window{ SDL_CreateWindow("SDL...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) };
	if (m_window == nullptr)
		throw "No es pot inicialitzar SDL_Window";

	// --- RENDERER ---
	SDL_Renderer *m_renderer{ SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
	if (m_renderer == nullptr)
		throw "No es pot inicialitzar SDL_Renderer";

	//-->SDL_Image 
	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };  //??
	if (!(IMG_Init(imgFlags) & imgFlags)) throw "Error: SDL_image init";

	//-->SDL_TTF
	if (TTF_Init() < 0) throw "Error: TTF init.";
	TTF_Font *font = TTF_OpenFont("../../res/ttf/saiyan.ttf", 80);

	
	
	//-->SDL_Mix
	const Uint8 mixFlags{ MIX_INIT_MP3 | MIX_INIT_OGG };
	if (!(Mix_Init(mixFlags) & mixFlags)) throw "Error: SDL_mix not loaded"; 


	// --- SPRITES ---
		//Background
	SDL_Texture* bgTexture{ IMG_LoadTexture(m_renderer, "../../res/img/bg.jpg") };
	if (bgTexture == nullptr) throw "Error: bgTexture init";
	SDL_Rect bgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT }; //??
	//-->Animated Sprite ---

	// --- TEXT ---
	SDL_Color textColor = { 144, 77, 255, 255 };
	SDL_Surface *textTempSurface = TTF_RenderText_Solid(font, "SDL GAME", textColor);
	SDL_Texture *text = SDL_CreateTextureFromSurface(m_renderer, textTempSurface);
	SDL_Rect textRect{ 300, 50, 200, 65 };

	//Boton play
	SDL_Color playButtonColor = { 255, 0 , 0, 255 };
	SDL_Color playButtonHoverColor = { 100, 0, 0, 255 };
	SDL_Color playButtonActiveColor = { 0,255,0,255 }; 
	
	textTempSurface = TTF_RenderText_Solid(font, "PLAY", playButtonColor);							//we give the surface the info wanted         
	SDL_Texture * playButtonTexture = SDL_CreateTextureFromSurface(m_renderer, textTempSurface);    //We create the texture using the surface with the info given
	
	textTempSurface = TTF_RenderText_Solid(font, "PLAY", playButtonHoverColor);
	SDL_Texture * playButtonHoverTexture = SDL_CreateTextureFromSurface(m_renderer, textTempSurface);
	
	textTempSurface = TTF_RenderText_Solid(font, "PLAY", playButtonActiveColor);
	SDL_Texture * playButtonActiveTexture = SDL_CreateTextureFromSurface(m_renderer, textTempSurface);
	
	SDL_Rect playButtonRect{ 300, 250, 200, 65 };
	SDL_Texture *playButtonTexturePrint = playButtonTexture;

	
	//Boton Sound
	SDL_Color soundButtonColor = { 255, 0 , 0, 255 };
	SDL_Color soundButtonHoverColor = { 100, 0 , 0, 255 };
	
	textTempSurface = TTF_RenderText_Solid(font, "Sound", soundButtonColor);
	SDL_Texture *soundButtonTexture = SDL_CreateTextureFromSurface(m_renderer, textTempSurface);
	
	textTempSurface = TTF_RenderText_Solid(font, "Sound", soundButtonHoverColor);
	SDL_Texture *soundButtonHoverTexture = SDL_CreateTextureFromSurface(m_renderer, textTempSurface);
	
	SDL_Rect soundButtonRect{ 250, 320, 300, 70 };
	SDL_Texture *soundButtonTexturePrint = soundButtonTexture; 
	
	//Boton Exit
	SDL_Color exitButtonColor = { 255, 0 , 0, 255 };
	SDL_Color exitButtonHoverColor = { 100, 0 , 0, 255 };
	
	textTempSurface = TTF_RenderText_Solid(font, "Exit", exitButtonColor);
	SDL_Texture *exitButtonTexture = SDL_CreateTextureFromSurface(m_renderer, textTempSurface);
	
	textTempSurface = TTF_RenderText_Solid(font, "Exit", exitButtonHoverColor);
	SDL_Texture *exitButtonHoverTexture = SDL_CreateTextureFromSurface(m_renderer, textTempSurface);
	
	SDL_Rect exitButtonRect{ 300, 390, 200, 65 };
	SDL_Texture *exitButtonTexturePrint = exitButtonTexture; 
	

	//Button States, we need to struct them into pressed and hovers 
	bool mouseClicked = false; 
	bool playHover = false;
	bool playRed = true; 
	bool soundHover = false;
	bool exitHover = false;
	bool soundOff = false;


	SDL_Texture *playerTexture{ IMG_LoadTexture(m_renderer, "../../res/img/kintoun.png") };
	if (playerTexture == nullptr) throw "No s'han pogut crear les textures(cursor)";
	SDL_Rect playerRect{ 0,0,60,32 };
	position playerTarget; 


	// --- AUDIO ---

	 if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) throw "Unable to initialize SDL_mixer audio systems";

	 Mix_Music *soundtrack{ Mix_LoadMUS("../../res/au/mainTheme.mp3") };
	 if (!soundtrack) throw "unable to load the Mix_Music soundtrack";
	 Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
	 Mix_PlayMusic(soundtrack, -1);

	// --- GAME LOOP ---
	SDL_Event event;
	bool isRunning = true;
	while (isRunning) {
		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {

			case SDL_MOUSEMOTION:
				
				playerTarget.x = event.motion.x; 
				playerTarget.y = event.motion.y; 
				break; 
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
				break;
			case SDL_MOUSEBUTTONUP:
				mouseClicked = true; 
				break;
			default:;
			}
		}

		// UPDATE

		playerRect.x += ((playerTarget.x  - playerRect.w / 2)- playerRect.x) / 10;
		playerRect.y += ((playerTarget.y - playerRect.h/ 2)- playerRect.y) / 10;
		
		playHover = checkCursorCollision(playerTarget, playButtonRect); 
		soundHover = checkCursorCollision(playerTarget, soundButtonRect); 
		exitHover = checkCursorCollision(playerTarget, exitButtonRect); 

		if (mouseClicked) {
			if (exitHover) isRunning = false;

			if (playHover) {
				playRed = !playRed; 
			}
			

			if (soundHover && soundOff == false) {
			
				SDL_PauseAudio(1);
				soundOff = true;
			}
			else if (soundHover && soundOff) {
				SDL_PauseAudio(0);
				soundOff = false;
			}
			mouseClicked = false; 
		}
		
		//Texture Updates
		
		//Play
		if (playHover)
		{
			playButtonTexturePrint = playButtonHoverTexture; 
		}
		else {
			if (playRed) playButtonTexturePrint = playButtonTexture; 
			else playButtonTexturePrint = playButtonActiveTexture; 
		}
		
		//Sound
		if (soundHover) soundButtonTexturePrint = soundButtonHoverTexture;
		else soundButtonTexturePrint = soundButtonTexture;
		
		//Exit
		if (exitHover) exitButtonTexturePrint = exitButtonHoverTexture;
		else exitButtonTexturePrint = exitButtonTexture;
		
		// DRAW
		SDL_RenderClear(m_renderer);
		

		//Background
		SDL_RenderCopy(m_renderer, bgTexture, nullptr, &bgRect);
		SDL_RenderCopy(m_renderer, text, nullptr, &textRect);    //Maiin text
		SDL_RenderCopy(m_renderer, playButtonTexturePrint, nullptr, &playButtonRect); 
		SDL_RenderCopy(m_renderer, soundButtonTexturePrint, nullptr, &soundButtonRect); 
		SDL_RenderCopy(m_renderer, exitButtonTexturePrint, nullptr, &exitButtonRect); 
		
		//Cursor
		SDL_RenderCopy(m_renderer, playerTexture, nullptr, &playerRect);
		SDL_RenderPresent(m_renderer); 

	}

	// --- DESTROY ---
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyTexture(playerTexture); 
	SDL_DestroyTexture(text); 
	SDL_DestroyTexture(playButtonTexturePrint); 
	SDL_DestroyTexture(soundButtonTexturePrint); 
	SDL_DestroyTexture(exitButtonTexturePrint); 
	Mix_CloseAudio();
	//SDL_FreeSurface(tmpSurf);
	//TTF_CloseFont(font);
	Mix_Quit(); 
	IMG_Quit();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);

	// --- QUIT ---
	SDL_Quit();

	return 0;
}