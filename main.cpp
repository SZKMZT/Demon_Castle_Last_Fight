#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "Scene.h"
#include <SDL_mixer.h>
using namespace std;

Scene scene;
SDL_Renderer* gRenderer = nullptr;
bool quit = false;
bool vsync2 = true;
extern string fpscustom;
Uint32 t;
int scene2;

int main(int argc, char* argv[])
{
    scene.init();
    SDL_Event e;

    Mix_Music* music = Mix_LoadMUS("assets/sound/music.mp3");
    Mix_VolumeMusic(64);
    Mix_Volume(-1, 32/2);

    while( !quit )
    {
        if ( static_cast<int>(scene.scene) != 0) 
        if (!Mix_PlayingMusic()) 
        Mix_PlayMusic(music, -1);
        if (!scene.fps_max && !scene.vsync) t = SDL_GetTicks();
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type == SDL_QUIT)
            {
                quit = true;
            }
            scene.handleEvent(e);
        }

       scene.logicScene();

        SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF );
        SDL_RenderClear( gRenderer );

        scene.renderScene();

        SDL_RenderPresent(gRenderer);

        if (!scene.fps_max && !scene.vsync) while ( SDL_GetTicks() - t < 1000.f / stoi(fpscustom) ){}
    }

    scene.free();
    close();
    return 0;
}