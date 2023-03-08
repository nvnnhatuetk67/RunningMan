#include <SDL.h>
#include<windows.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
#include <bits/stdc++.h>
#include<SDL_mixer.h>
using namespace std;

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 512;
const string WINDOW_TITLE = "Running Man";
Mix_Music* music = nullptr;
SDL_Color colorPoint = {255,255,255};
SDL_Color colorText = {200,255, 150};
SDL_Color yellow = {230, 250, 10};
SDL_Texture* loadTexture(string path, SDL_Renderer* renderer);
TTF_Font* font = TTF_OpenFont("04B_19__.ttf", 28);


void logSDLError(std::ostream& os, const std::string &msg, bool fatal);

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);


void quitSDL(SDL_Window* window, SDL_Renderer* renderer);

void waitUntilKeyPressed();

void setupImage(string file_path, int x, int y,int width, int height, SDL_Renderer* renderer );

void moveImage(string file_path1, string file_path2, int x, int y,int width, int height, SDL_Renderer* renderer);

void jumping(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int vy, int ay);

void winScreen(SDL_Renderer* renderer);

void startScreen(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color);

bool checkGameOver(SDL_Rect rect1, SDL_Rect rect2);

void loseScreen(SDL_Renderer* renderer, TTF_Font* font);

SDL_Texture* updatePoint(SDL_Renderer* renderer, int point,TTF_Font* font, SDL_Color color);

bool checkPoint(SDL_Rect rect1, SDL_Rect rect2);

SDL_Texture* loadText(SDL_Renderer* renderer, string text, TTF_Font* font, SDL_Color color);

SDL_Texture* updateBackground(SDL_Renderer* renderer, int point);

SDL_Texture* updateObject(SDL_Renderer* renderer, int point);

void loadMusic(string filePath);


int main(int argv, char** args)
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Texture* texture = NULL;
    initSDL(window, renderer);

    TTF_Init();
    TTF_Font* font = TTF_OpenFont("04B_19__.ttf", 28);

    SDL_SetRenderDrawColor(renderer, 10, 10, 10 ,255);
    SDL_RenderClear(renderer);


    //Setup rect background
    SDL_Texture* background = loadTexture("photo/background.png",renderer);
    int x = 0, y = 0, bg_w = 4248, bg_h = 512;
    SDL_Rect rect_bg = {x, y, bg_w, bg_h};

    //Set up main character
    int width = 45, height = 65;
    int x_1= 30, y_1 = 361;
    SDL_Rect destRect= {x_1, y_1, width, height};
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    // Character speed and background speed
    int c_speed = 10, bg_speed= 10;

    //Set up object
    SDL_Texture* object = loadTexture("photo/fire.png",renderer);
    int x_ob = 850, y_ob = 325;
    SDL_Rect ob_rect = {x_ob , y_ob , 56 ,101};

    //SDL_RenderCopy(renderer, object, nullptr, &ob_rect);
    SDL_Texture* object_1 = loadTexture("photo/fire.png",renderer);
    int x_ob1 = 1200;
    SDL_Rect ob_rect_1 = {x_ob1 , y_ob , 56 ,101};
    int vy = -10, vx = 8;
    int ay = 1;
    //Set up Wingame screeen
    char choice;

    //Set up Point
    int point = 0;
    SDL_Rect rect = {900, 40, 40, 40};
    SDL_Texture* pointText ;
    //
    int highPoint = 0;
    SDL_Rect highrect = {900, 5, 40, 40};
    SDL_Texture* highPointText ;
    //Score
    string scoreText = "Score: ";
    SDL_Rect sc_rect = {760,40,80,30};
    SDL_Texture* score;
    string highScoreText = "High Score: ";
    SDL_Rect hisc_rect = {760,7,130,30};
    SDL_Texture* highscore;
    //
    loadMusic("sound/sound1.mp3");
    // Play music


    Mix_PlayMusic(music, -1);
    Mix_PlayingMusic();
    //
    Mix_Chunk *soundPointing = Mix_LoadWAV("sound/point.wav");
    Mix_Chunk *soundLose = Mix_LoadWAV("sound/death.wav");
    Mix_Chunk *soundJump = Mix_LoadWAV("sound/jump.wav");

    if (!soundPointing || !soundJump || !soundLose) {
        // error handling
        cout<<"Can't load sound\n";
    }
    //
    bool isJumping = false;
    bool running = 1;
    bool checkStart = true;// Start screen appears only one time on the first background
    bool checkLose = 1; // if collided, character will stop moving;
    bool checkPlay = 0;// If mouse click at play button, game will run
    int countBg = 0;// To change background when character move at the end of map
    while(running){

        SDL_RenderCopy(renderer, background, nullptr, &rect_bg);

        SDL_Event event;

        //Moving
        if(!checkPlay){
            while(SDL_PollEvent(&event)) {
                    if(event.type == SDL_MOUSEBUTTONDOWN){
                       if(event.button.button == SDL_BUTTON_LEFT){
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                         //SDL_Rect playRect = {450, 220, 120, 120 };
                        if((x >= 450 && x <= 670)&&( y >= 220 && y <= 440)){
                            checkPlay = true;

                            break;
                            }
                        }
                    }
                }
            }

        while(SDL_PollEvent(&event)) {
                if(event.type == SDL_QUIT){
                    running = 0;
                    quitSDL(window,renderer);
                }
                if(event.type == SDL_KEYDOWN && checkLose)
                     switch(event.key.keysym.sym){
                       /* case SDLK_LEFT:
                            rect_bg.x+=bg_speed;
                             ob_rect.x +=bg_speed;
                            if(rect_bg.x > 0)
                                rect_bg.x = 0;
                                destRect.x-=c_speed;

                           if(destRect.x < x_1){
                                destRect.x = x_1;
                            }*/
                            break;
                        case SDLK_RIGHT:{
                           rect_bg.x-=bg_speed;
                           if(destRect.x <= x_1){
                           ob_rect.x -=bg_speed;
                           ob_rect_1.x -=bg_speed;
                           }
                           if(ob_rect.x < -200 )ob_rect.x = x_ob;

                            if(ob_rect_1.x < -100) ob_rect_1.x = x_ob1;

                           //destRect.x+=vx/2;
                            if(rect_bg.x < 960- bg_w){
                                rect_bg.x = 960-bg_w;
                                destRect.x+=c_speed;
                                }
                            break;
                        }
                        case SDLK_UP:{
                           /* if(Mix_Playing(-1) != 0  ) {
                                SDL_Delay(100);
                            }*/
                               // Mix_PlayChannel(-1, soundJump, 0);
                                if (!isJumping) {
                                    isJumping = true;
                                    vy = -16;
                                    vx = 10;
                                }

                           break;
                        }
                    }
                }

        //Jumping
        if (isJumping&&checkLose) {
            destRect.y += vy;
                      // destRect.x += vx/5;
            rect_bg.x-=(vx/2);
            if(destRect.x <= x_1){
                ob_rect.x -=vx/2;
                ob_rect_1.x -=vx/2;
            }

            if(rect_bg.x < SCREEN_WIDTH- bg_w){
                rect_bg.x = SCREEN_WIDTH-bg_w;
                destRect.x+=vx/2;
                //if(destRect.x >  960){
                //    destRect.x = 960;
                // }
            }
                vy += ay;
                        //vx -= ay;
            if (destRect.y >= y_1) {
                destRect.y = y_1;
                vy = -10;
                isJumping = false;
            }
        }


          //Handle the scoring
            if(checkLose){
            if(checkPoint(destRect, ob_rect)) {

                point++;
                Mix_PlayChannel(-1, soundPointing, 0);
            }
            if(checkPoint(destRect, ob_rect_1)){
                point++;
                Mix_PlayChannel(-1, soundPointing, 0);
            }
            }

            if(point >= highPoint) highPoint = point;
            pointText = updatePoint(renderer, point, font, colorPoint);
            highPointText = updatePoint(renderer, highPoint, font, colorPoint);
            score = loadText(renderer, scoreText, font, colorPoint);
            highscore = loadText(renderer, highScoreText, font, colorPoint);
            if(checkPlay){
            SDL_RenderCopy(renderer, pointText, nullptr, &rect);
            SDL_RenderCopy(renderer, highPointText, nullptr, &highrect);
            SDL_RenderCopy(renderer,score,nullptr, &sc_rect);
            SDL_RenderCopy(renderer,highscore,nullptr, &hisc_rect);

        // Load object
        if(ob_rect.x < -170 ){
                ob_rect.x = x_ob;
        }
        if(ob_rect_1.x < -80){
                ob_rect_1.x = x_ob1;
        }

        SDL_RenderCopy(renderer, object, nullptr, &ob_rect);
        SDL_RenderCopy(renderer, object_1, nullptr, &ob_rect_1);
        //Set up main character

        if(fmod(-rect_bg.x/20,3)==1)
            texture= loadTexture("photo/character1.png", renderer);
        else if((fmod(-rect_bg.x/20,3)==2))
            texture= loadTexture("photo/character2.png", renderer);
        else
            texture= loadTexture("photo/character3.png", renderer);

        if(rect_bg.x < 980- bg_w|| rect_bg.x == 0)
        {
        if(fmod(destRect.x/10,3) == 1)
            texture= loadTexture("photo/character1.png", renderer);
        else if(fmod(destRect.x/10,3) == 2)
            texture= loadTexture("photo/character2.png", renderer);
        else
            texture= loadTexture("photo/character3.png", renderer);
        }
        SDL_RenderCopy(renderer, texture, NULL, &destRect);

    }




        //Check collision
        if(checkGameOver(destRect, ob_rect)||checkGameOver(destRect, ob_rect_1)){
            //Mix_PlayChannel(-1 , soundLose, 1);
            loseScreen(renderer,font);
            checkLose = 0;
                //SDL_Delay(500);

            while(SDL_PollEvent(&event)) {
                if(event.type == SDL_QUIT){
                        running = 0;
                }
                if(event.type == SDL_KEYDOWN){
                    switch(event.key.keysym.sym){
                        case SDLK_SPACE:{
                            checkLose = 1;
                            destRect.x = x_1;
                            destRect.y = y_1;
                            rect_bg.x = 0;
                            rect_bg.y = 0;
                            ob_rect.x = x_ob;
                            ob_rect.y = y_ob;
                            ob_rect_1.x = x_ob1;
                            ob_rect_1.y = y_ob;
                            point = 0;
                            countBg = 0;
                            checkPlay = 0;
                            background = loadTexture("photo/background.png" ,renderer);
                            object = loadTexture("photo/fire.png",renderer);
                            object_1 = loadTexture("photo/fire.png",renderer);
                            checkStart = true;
                            startScreen(renderer,font, yellow);

                            }
                            break;
                        }
                    }
                }

            }

        // Check wingame
         if(countBg  >= 4&& destRect.x >=960){
            winScreen(renderer);
            cout<<"Do you want to play again ?(y/n)\n";

            do{
                cin>>choice;
            }while(choice != 'y' && choice != 'n');

                if(choice == 'y' ){
                    destRect.x = x_1;
                    destRect.y = y_1;
                    rect_bg.x = 0;
                    rect_bg.y = 0;
                    ob_rect.x = x_ob;
                    ob_rect.y = y_ob;
                    ob_rect_1.x = x_ob1;
                    ob_rect_1.y = y_ob;
                    point = 0;
                    background = loadTexture("photo/background.png" ,renderer);
                    object = loadTexture("photo/fire.png",renderer);
                    object_1 = loadTexture("photo/fire.png",renderer);
                    checkStart = true;
                    countBg = 0;
                    checkPlay = 0;
                    startScreen(renderer,font, yellow);
                }
                else{
                    running = 0;
                    quitSDL(window, renderer);
                    }
            }

        //Check to change background
        if(destRect.x >= 960){
                destRect.x = x_1;
                destRect.y = y_1;
                rect_bg.x = 0;
                rect_bg.y = 0;
                ob_rect.x = x_ob;
                ob_rect.y = y_ob;
                ob_rect_1.x = x_ob1;
                ob_rect_1.y = y_ob;
                countBg++;
                background = updateBackground(renderer, countBg);
                object = updateObject(renderer, countBg);
                object_1 = updateObject(renderer, countBg);

                checkStart = 0;
        }
        if(rect_bg.x >= -10 && checkStart && !checkPlay ){
            startScreen(renderer,font, yellow);
         }

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    TTF_Quit();
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    waitUntilKeyPressed();
    quitSDL(window, renderer);

	return 0;
}

SDL_Texture* loadTexture(string path, SDL_Renderer* renderer){
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == nullptr){
        cout<<"Unable to  load image " << path << " SDL_image Error: "
        << IMG_GetError() << endl;
    }
    else{
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if(newTexture == nullptr)
            cout<<"Unable to create texture from " << path << " SDL Error: "
                << SDL_GetError()<< endl;
                SDL_FreeSurface( loadedSurface);
    }
    return newTexture;
}
void logSDLError(std::ostream& os, const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (window == nullptr)
        logSDLError(std::cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    //SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr)
        logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             ( e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}

void winScreen(SDL_Renderer* renderer){
    SDL_Texture* youwin = loadTexture("photo/wingame.png",renderer);
    SDL_Rect rect_youwin = {270, -70,  460,620};
    SDL_RenderCopy(renderer, youwin, nullptr, &rect_youwin);
    SDL_RenderPresent(renderer);

}
void startScreen(SDL_Renderer* renderer ,TTF_Font* font, SDL_Color color){
    string startText = " PRESS TO START ";
    SDL_Texture* startGame =  loadText( renderer, startText, font, color);
    SDL_Rect rect_startGame = {100, 110, 800,100};
    SDL_RenderCopy(renderer, startGame, nullptr, &rect_startGame);
     //play button
    SDL_Texture* play = loadTexture("photo/playbutton.png", renderer);;
    SDL_Rect playRect = {420, 220, 120, 120 };
    SDL_RenderCopy(renderer, play, nullptr, &playRect);
    SDL_RenderPresent(renderer);
}

void loseScreen(SDL_Renderer* renderer, TTF_Font* font){
    //SDL_Texture* youlose = loadTexture("photo/gameover.png",renderer);
    string losegame = " Game Over ";
    string startText = " Press SPACE to play again! ";
    SDL_Texture* youlose = loadText(renderer, losegame, font, colorText);
    SDL_Texture* start = loadText(renderer, startText, font, colorText);

    SDL_Rect rect_youlose = {330, 100,  300 , 80};
    SDL_Rect startRect  = {100 ,250,800, 80};

    SDL_RenderCopy(renderer, youlose, nullptr, &rect_youlose);
    SDL_RenderCopy(renderer, start, nullptr, &startRect);
    SDL_RenderPresent(renderer);

}
bool checkGameOver(SDL_Rect rect1, SDL_Rect rect2){

    if((rect1.x > rect2.x-40 && rect1.x< rect2.x+ rect2.w - 10 )&&( rect1.y >= rect2.y+5))
       return true;
    return 0;
}
SDL_Texture* updatePoint(SDL_Renderer* renderer, int point, TTF_Font* font, SDL_Color color){
    string textPoint ;
    if(point < 10)
        textPoint ="0" +to_string(point);
    else
        textPoint = to_string(point);
    SDL_Surface* surface = TTF_RenderText_Solid(font,textPoint.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

return texture;
}
SDL_Texture* loadText(SDL_Renderer* renderer, string text, TTF_Font* font, SDL_Color color){
    SDL_Surface* surface = TTF_RenderText_Solid(font,text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    return texture;
}
bool checkPoint(SDL_Rect rect1, SDL_Rect rect2){
    return  rect1.x >= rect2.x+5 && rect1.x <= rect2.x + 10  ;
}
SDL_Texture* updateBackground(SDL_Renderer* renderer, int point){
    SDL_Texture* background;
    if(point == 1) background = loadTexture("photo/background1.png", renderer);
    else if( point == 2) background = loadTexture("photo/background2.png", renderer);
    else if (point == 3) background = loadTexture("photo/background3.png", renderer);
    else if(point == 4) background = loadTexture("photo/background4.png", renderer);
    return background;
}
SDL_Texture* updateObject(SDL_Renderer* renderer, int point){
     SDL_Texture* object;
    if(point == 1) object = loadTexture("photo/cactus.png", renderer);
    else if( point == 2) object= loadTexture("photo/chainsaw.png", renderer);
    else if (point == 3) object = loadTexture("photo/headstone.png", renderer);
    else if(point == 4) object = loadTexture("photo/ice.png", renderer);
    return object;
}
void loadMusic(string filePath){
    if(SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
    }

    // Initialize SDL_mixer
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;

    }
    // Load MP3 file
     music = Mix_LoadMUS(filePath.c_str());//"sound/sound1.mp3"
    if(music == NULL) {
        std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
}
