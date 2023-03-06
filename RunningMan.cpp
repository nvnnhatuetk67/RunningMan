#include <SDL.h>
#include<windows.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
#include <bits/stdc++.h>
#include<SDL_mixer.h>
using namespace std;

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 512;
const string WINDOW_TITLE = "SDL test";
Mix_Music* music = nullptr;
SDL_Color color = {255,255,255};
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
void startScreen(SDL_Renderer* renderer);
bool checkGameOver(SDL_Rect rect1, SDL_Rect rect2);
void loseScreen(SDL_Renderer* renderer);
SDL_Texture* updatePoint(SDL_Renderer* renderer, int point,TTF_Font* font);
bool checkPoint(SDL_Rect rect1, SDL_Rect rect2);
SDL_Texture* loadText(SDL_Renderer* renderer, string text, TTF_Font* font);
SDL_Texture* updateBackground(SDL_Renderer* renderer, int point);
SDL_Texture* updateObject(SDL_Renderer* renderer, int point);
void loadMusic();


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
    SDL_Rect rect = {100, 10, 40, 40};
    SDL_Texture* pointText ;
    //
    int highPoint = 0;
    SDL_Rect highrect = {860, 10, 40, 40};
    SDL_Texture* highPointText ;
    //Score
    string scoreText = "Score: ";
    SDL_Rect sc_rect = {10,10,80,50};
    SDL_Texture* score;
    string highScoreText = "High Score: ";
    SDL_Rect hisc_rect = {700,10,150,50};
    SDL_Texture* highscore;
    //
    loadMusic();
    // Play music
    Mix_PlayMusic(music, -1); // -1 means loop indefinitely
    //
    bool isJumping = false;
    bool running = 1;
    bool checkStart = true;
    while(running&&Mix_PlayingMusic()){
    //SDL_Delay(10);
    SDL_RenderCopy(renderer, background, nullptr, &rect_bg);

    SDL_Event event;
    //Moving
    while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT){
                running = 0;
            }
            if(event.type == SDL_KEYDOWN)
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
                       if(ob_rect.x < -200 ){
                            ob_rect.x = x_ob;
                        }
                        if(ob_rect_1.x < -100){
                                ob_rect_1.x = x_ob1;
                        }
                       //destRect.x+=vx/2;
                        if(rect_bg.x < 960- bg_w){
                            rect_bg.x = 960-bg_w;
                            destRect.x+=c_speed;

                            /*if(destRect.x >  940){
                                destRect.x = 940;
                                }*/
                            }
                        break;
                    }
                    case SDLK_UP:{
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
            if (isJumping) {
                    destRect.y += vy;
                      // destRect.x += vx/5;
                    rect_bg.x-=(vx/2);
                    if(destRect.x <= x_1){
                    ob_rect.x -=vx/2;
                    ob_rect_1.x -=vx/2;
                    }
                    if(ob_rect.x < -200 ){
                            ob_rect.x = x_ob;
                    }
                    if(ob_rect_1.x < -100){
                            ob_rect_1.x = x_ob1;
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

        SDL_RenderCopy(renderer, object, nullptr, &ob_rect);
        SDL_RenderCopy(renderer, object_1, nullptr, &ob_rect_1);

        SDL_Delay(10);

          //Handle the scoring
            if(checkPoint(destRect, ob_rect)) point++;
            if(checkPoint(destRect, ob_rect_1)) point++;
            pointText = updatePoint(renderer, point, font);
            SDL_RenderCopy(renderer, pointText, nullptr, &rect);

            if(point >= highPoint) highPoint = point;
            highPointText = updatePoint(renderer, highPoint, font);
            SDL_RenderCopy(renderer, highPointText, nullptr, &highrect);

            score = loadText(renderer, scoreText, font);
            SDL_RenderCopy(renderer,score,nullptr, &sc_rect);

            highscore = loadText(renderer, highScoreText, font);
            SDL_RenderCopy(renderer,highscore,nullptr, &hisc_rect);


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


        //Check collision
        if(checkGameOver(destRect, ob_rect)||checkGameOver(destRect, ob_rect_1)){
                loseScreen(renderer);
                 cout<<"Do you want to play again ?(y/n)\n";

                   do{
                        cin>>choice;
                   }while(choice != 'y' && choice != 'n');

                    if(choice == 'y' ){
                        destRect.x = x_1;
                        destRect.y = y_1;
                        rect_bg.x = 30;
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
                        startScreen(renderer);
                    }
                    else{
                        running = 0;
                        quitSDL(window, renderer);
                    }
            }

        // Check wingame
         if(point >= 35&& destRect.x >=960){
            winScreen(renderer);
            cout<<"Do you want to play again ?(y/n)\n";

                   do{
                        cin>>choice;
                   }while(choice != 'y' && choice != 'n');

                    if(choice == 'y' ){
                        destRect.x = x_1;
                        destRect.y = y_1;
                        rect_bg.x = 30;
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
                        startScreen(renderer);
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
                rect_bg.x = 30;
                rect_bg.y = 0;
                ob_rect.x = x_ob;
                ob_rect.y = y_ob;
                ob_rect_1.x = x_ob1;
                ob_rect_1.y = y_ob;
                background = updateBackground(renderer, point);
                object = updateObject(renderer, point);
                object_1 = updateObject(renderer, point);
                checkStart = 0;
        }
        if(rect_bg.x >= -10 && checkStart){
            startScreen(renderer);
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
    SDL_Rect rect_youwin = {270, -70,  400,620};
    SDL_RenderCopy(renderer, youwin, nullptr, &rect_youwin);
    SDL_RenderPresent(renderer);

}
void startScreen(SDL_Renderer* renderer){
    SDL_Texture* startGame = loadTexture("photo/startGame.png",renderer);
    SDL_Rect rect_startGame = {240, 150, 560,160};
    SDL_RenderCopy(renderer, startGame, nullptr, &rect_startGame);
}

void loseScreen(SDL_Renderer* renderer){
    SDL_Texture* youlose = loadTexture("photo/gameover.png",renderer);
    SDL_Rect rect_youlose = {120, -30,  374*2,268*2};
    SDL_RenderCopy(renderer, youlose, nullptr, &rect_youlose);
    SDL_RenderPresent(renderer);

}
bool checkGameOver(SDL_Rect rect1, SDL_Rect rect2){

    if((rect1.x > rect2.x-40 && rect1.x< rect2.x+ rect2.w-10 )&&( rect1.y >= rect2.y))
       return true;
    return 0;
}
SDL_Texture* updatePoint(SDL_Renderer* renderer, int point, TTF_Font* font){
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
SDL_Texture* loadText(SDL_Renderer* renderer, string text, TTF_Font* font){
    SDL_Surface* surface = TTF_RenderText_Solid(font,text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    return texture;
}
bool checkPoint(SDL_Rect rect1, SDL_Rect rect2){
    return rect1.x >= rect2.x && rect1.x < rect2.x+5 ;
}
SDL_Texture* updateBackground(SDL_Renderer* renderer, int point){
    SDL_Texture* background;
    if(point < 14) background = loadTexture("photo/background1.png", renderer);
    else if( point < 21) background = loadTexture("photo/background2.png", renderer);
    else if (point < 28) background = loadTexture("photo/background3.png", renderer);
    else if(point < 35) background = loadTexture("photo/background4.png", renderer);
    return background;
}
SDL_Texture* updateObject(SDL_Renderer* renderer, int point){
     SDL_Texture* object;
    if(point < 14) object = loadTexture("photo/cactus.png", renderer);
    else if( point < 21) object= loadTexture("photo/chainsaw.png", renderer);
    else if (point < 28) object = loadTexture("photo/headstone.png", renderer);
    else if(point < 35) object = loadTexture("photo/ice.png", renderer);
    return object;
}
void loadMusic(){
    if(SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
    }

    // Initialize SDL_mixer
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;

    }
    // Load MP3 file
     music = Mix_LoadMUS("sound/sound1.mp3");
    if(music == NULL) {
        std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
}
