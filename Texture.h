#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
using namespace std;

enum ButtonState
{
	MOUSE_OUT = 0,
	MOUSE_OVER_MOTION = 1,      
	MOUSE_OVER = 2,
	MOUSE_DOWN = 3,
	MOUSE_UP = 4,
	MOUSE_DOWN_OUT = 5,
};

extern SDL_Renderer* gRenderer;
extern TTF_Font* gTimes;
extern TTF_Font* gArial;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

class Texture
{
	public:
		Texture();
		~Texture();

		void loadFromFile( string path, bool ColorKey = false, Uint8 r = 0, Uint8 g = 0, Uint8 b = 0 );
		void loadFromRenderedText( string textureText, SDL_Color textColor, TTF_Font* gfont );
		void free();
		void setColor( Uint8 red, Uint8 green, Uint8 blue );
		void setBlendMode( SDL_BlendMode blending );
		void setAlpha( Uint8 alpha );
		void render( int x, int y, SDL_Rect* custom = nullptr, SDL_Rect* clip = nullptr, bool zoom = false, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE );
		void render2( int x, int y, SDL_Rect* custom = nullptr, SDL_Rect* clip = nullptr, bool zoom = false, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE );
		ButtonState handleEvent( SDL_Event* e );
		int getWidth();
		int getHeight();
		SDL_Rect getRect();
		int mx;
		int my;
		int mw;
		int mh;
	private:
		SDL_Texture* mTexture;
		int mWidth;
		int mHeight;
};

Texture::Texture()
{
	mTexture = nullptr;
	mWidth = 0;
	mHeight = 0;
	mx = 0;
	my = 0;
	mw = 0;
	mh = 0;
}

Texture::~Texture()
{
	free();
}

void Texture::loadFromFile( string path, bool ColorKey, Uint8 r, Uint8 g, Uint8 b )
{
    free();

    SDL_Texture* newTexture = nullptr;

    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if (loadedSurface == nullptr) cout << "lỗi loadedSurface " << IMG_GetError() << endl;

    if (ColorKey) SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, r, g, b ));

    newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
	if (newTexture == nullptr) cout << "lỗi newTexture " << IMG_GetError() << endl;

	mWidth = loadedSurface->w;
	mHeight = loadedSurface->h;
	mw = mWidth;
	mh = mHeight;

    SDL_FreeSurface( loadedSurface );
	if(loadedSurface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return;
    }

    mTexture = newTexture;
}

void Texture::loadFromRenderedText( string textureText, SDL_Color textColor, TTF_Font* gfont )
{
    free();

    SDL_Surface* textSurface = TTF_RenderUTF8_Blended( gfont, textureText.c_str(), textColor );

    mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
     
    mWidth = textSurface->w;
	mHeight = textSurface->h;
	mw = mWidth;
	mh = mHeight;

    SDL_FreeSurface( textSurface );
}

void Texture::free()
{
	SDL_DestroyTexture( mTexture );
	mTexture = nullptr;
	mWidth = 0;
	mHeight = 0;
	mx = 0;
	my = 0;
	mw = 0;
	mh = 0;
}

void Texture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void Texture::setBlendMode( SDL_BlendMode blending )
{
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void Texture::setAlpha( Uint8 alpha )
{
	SDL_SetTextureBlendMode( mTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void Texture::render( int x, int y, SDL_Rect* custom, SDL_Rect* clip, bool zoom, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	if (mTexture == nullptr) cout << "lỗi mTexture 2" << endl;
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if( clip != nullptr )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	if ( custom != nullptr )
	{
		renderQuad.x = custom->x;
		renderQuad.y = custom->y;
		renderQuad.w = custom->w;
		renderQuad.h = custom->h;
	}
	if( zoom )
	{
		if (static_cast<double>(renderQuad.w) / renderQuad.h < 
			static_cast<double>(SCREEN_WIDTH) / SCREEN_HEIGHT)
		{
			double rate = static_cast<double>(renderQuad.w) / renderQuad.h;
			renderQuad.h = SCREEN_HEIGHT;
			renderQuad.w = static_cast<int>(renderQuad.h * rate);
			renderQuad.x = (SCREEN_WIDTH - renderQuad.w) / 2;
		}
		else
		{
			double rate = static_cast<double>(renderQuad.h) / renderQuad.w;
			renderQuad.w = SCREEN_WIDTH;
			renderQuad.h = static_cast<int>(renderQuad.w * rate);
			renderQuad.y = (SCREEN_HEIGHT - renderQuad.h) / 2;
		}
	}
	mx = renderQuad.x;
	my = renderQuad.y;
	mw = renderQuad.w;
	mh = renderQuad.h;

	while (angle >= 360) angle -= 360;
	while (angle < 0) angle += 360;

	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

void Texture::render2( int x, int y, SDL_Rect* custom, SDL_Rect* clip, bool zoom, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	if (mTexture == nullptr) cout << "lỗi mTexture 2" << endl;
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if( clip != nullptr )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	if ( custom != nullptr )
	{
		renderQuad.x = custom->x;
		renderQuad.y = custom->y;
		renderQuad.w = custom->w;
		renderQuad.h = custom->h;
	}
	if( zoom )
	{
		if (static_cast<double>(renderQuad.w) / renderQuad.h > 
			static_cast<double>(SCREEN_WIDTH) / SCREEN_HEIGHT)
		{
			double rate = static_cast<double>(renderQuad.w) / renderQuad.h;
			renderQuad.h = SCREEN_HEIGHT;
			renderQuad.w = static_cast<int>(renderQuad.h * rate);
			renderQuad.x = (SCREEN_WIDTH - renderQuad.w) / 2;
		}
		else
		{
			double rate = static_cast<double>(renderQuad.h) / renderQuad.w;
			renderQuad.w = SCREEN_WIDTH;
			renderQuad.h = static_cast<int>(renderQuad.w * rate);
			renderQuad.y = (SCREEN_HEIGHT - renderQuad.h) / 2;
		}
	}
	mx = renderQuad.x;
	my = renderQuad.y;
	mw = renderQuad.w;
	mh = renderQuad.h;

	while (angle >= 360) angle -= 360;
	while (angle < 0) angle += 360;

	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int Texture::getWidth()
{
	return mWidth;
}

int Texture::getHeight()
{
	return mHeight;
}

SDL_Rect Texture::getRect()
{
	return {mx, my, mw, mh};
}

ButtonState Texture::handleEvent( SDL_Event* e )
{
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        int x, y;
        SDL_GetMouseState( &x, &y );
        bool inside = true;

        if( x < mx )
        {
            inside = false;
        }
        else if( x > mx + mw )
        {
            inside = false;
        }
        else if( y < my )
        {
            inside = false;
        }
        else if( y > my + mh )
        {
            inside = false;
        }
        if( !inside )
        {
			return MOUSE_OUT;
        }
        else
        {
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                return MOUSE_OVER_MOTION;
            
                case SDL_MOUSEBUTTONDOWN:
                return MOUSE_DOWN;
                
                case SDL_MOUSEBUTTONUP:
                return MOUSE_UP;

				default:
				return MOUSE_OVER;
            }
        }
    }
	return MOUSE_OUT;
}
