#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <SDL_mixer.h>
using namespace std;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern SDL_Renderer* gRenderer;
extern bool vsync2;

class Window
{
	public:
		Window();
		void init(); 
		void handleEvent( SDL_Event& e );
		void free();
		int getWidth();
		int getHeight();

	private:
		SDL_Window* mWindow;
		int mWidth;
		int mHeight;
        bool mFullScreen;
        bool mMinimized;
};

Window::Window()
{
	mWindow = nullptr;
	mWidth = 0;
	mHeight = 0;
    mFullScreen = false;
    mMinimized = false;
}

void Window::init()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	TTF_Init();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 2048);
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
	mWindow = SDL_CreateWindow( "Demon Castle: Last Fight", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
	mWidth = SCREEN_WIDTH;
	mHeight = SCREEN_HEIGHT;
	if (vsync2) gRenderer = SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	else gRenderer = SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED );
	SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF );
	SDL_StopTextInput(); 
}

void Window::handleEvent( SDL_Event& e )
{
	if( e.type == SDL_WINDOWEVENT )
	{
		switch( e.window.event )
		{
			case SDL_WINDOWEVENT_SIZE_CHANGED:
			mWidth = e.window.data1;
			mHeight = e.window.data2;
			SCREEN_HEIGHT = mHeight;
			SCREEN_WIDTH = mWidth;
			//SDL_RenderPresent( Renderer );
			break;

            case SDL_WINDOWEVENT_EXPOSED:
            SDL_RenderPresent( gRenderer );
            break;

            case SDL_WINDOWEVENT_MINIMIZED:
            mMinimized = true;
            break;

            //Cửa sổ được phóng to
            case SDL_WINDOWEVENT_MAXIMIZED:
            mMinimized = false;
            break;

            case SDL_WINDOWEVENT_RESTORED:
            mMinimized = false;
            break;

			/*bật khi muốn chạy nền, hiện tại không cần
			case SDL_WINDOWEVENT_CLOSE:
			SDL_HideWindow( mWindow );
			break;
            */
		}
	}
    else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F11 && e.key.repeat == 0 )
    {
        if( mFullScreen )
        {
            SDL_SetWindowFullscreen( mWindow, 0 );
            mFullScreen = false;
        }
        else
        {
            SDL_SetWindowFullscreen( mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP );
            mFullScreen = true;
			mMinimized = false;
        }
    }
}

void Window::free()
{
	if( mWindow != nullptr )
	{
		SDL_DestroyWindow( mWindow );
        mWindow = nullptr;
        mWidth = 0;
	    mHeight = 0;
        mFullScreen = false;
        mMinimized = false;
	}
}

int Window::getWidth()
{
	return mWidth;
}

int Window::getHeight()
{
	return mHeight;
}