#include <SDL.h>
#include <iostream>
using namespace std;

class Button
{
    public:
        Button(Texture* texture = nullptr, SDL_Rect pos = {}, SDL_Rect clip = {});
        ButtonState handleEvent( SDL_Event* e );
        void render();
        void setClip(SDL_Rect clip) { mClip = clip; }
        void setRect(SDL_Rect rect) { mRenderRect = rect; }
        SDL_Rect mRenderRect;
        SDL_Rect mClip;
    private:
        Texture* mTexture;
        SDL_Rect defaultclip;
};

Button::Button(Texture* texture, SDL_Rect pos, SDL_Rect clip)
{
    mTexture = texture;
    mRenderRect = pos;
    mClip = clip;
    defaultclip = mClip;
}

void Button::render()
{
    mTexture->render(0, 0, &mRenderRect, &mClip );
}

ButtonState Button::handleEvent( SDL_Event* e )
{
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        int x, y;
        SDL_GetMouseState( &x, &y );
        bool inside = true;

        if( x < mRenderRect.x )
        {
            inside = false;
        }
        else if( x > mRenderRect.x + mRenderRect.w )
        {
            inside = false;
        }
        else if( y < mRenderRect.y )
        {
            inside = false;
        }
        else if( y > mRenderRect.y + mRenderRect.h )
        {
            inside = false;
        }
        if( !inside )
        {
            if (e->type == SDL_MOUSEBUTTONDOWN) return MOUSE_DOWN_OUT;
			else return MOUSE_OUT;
            
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