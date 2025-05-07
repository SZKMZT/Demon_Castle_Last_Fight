#include <SDL.h>
using namespace std;

struct datachar
{
    int health;
    int power;
    int mana;
    int speed; // pixel/s
};

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

class Characters
{
    public:
        Characters(Texture* texture = nullptr);
        void addtexture(Texture* texture);
        void motion( SDL_Event* e );
        void checkvar();
        void animated();
    private:
        datachar datas;
        Texture* mTexture;
        int mVelX, mVelY;
        int mPosX, mPosY;
        int mWidth;
		int mHeight;

};

Characters::Characters(Texture* texture)
{
    mTexture = nullptr;
    datas.health = 100;
    datas.power = 5;
    datas.mana = 100;
    datas.speed = 2;
    mVelX = 0;
    mVelY = 0;
    mPosX = 0;
    mPosY = 0;
    mWidth = 0;
    mHeight = 0;
}

void Characters::addtexture(Texture* texture)
{
    mTexture = texture;
    mWidth = texture->mw;
    mHeight = texture->mh;
}

void Characters::motion( SDL_Event* e )
{
    if( e->type == SDL_KEYDOWN && e->key.repeat == 0 )
    {
        switch( e->key.keysym.sym )
        {
            case SDLK_w: mVelY = - datas.speed; break;
            case SDLK_s: mVelY = datas.speed; break;
            case SDLK_a: mVelX = - datas.speed; break;
            case SDLK_d: mVelX = datas.speed; break;
        }
    }

    else if( e->type == SDL_KEYUP && e->key.repeat == 0)
    {
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    
        switch( e->key.keysym.sym )
        {
            case SDLK_w: 
            {
            if ( currentKeyStates[SDL_SCANCODE_S] ) mVelY = datas.speed;
                else mVelY =0;
                break;
            }

            case SDLK_s:
            {
                if ( currentKeyStates[SDL_SCANCODE_W] ) mVelY = - datas.speed;
                else mVelY =0;
                break;
            }

            case SDLK_a:
            {
                if ( currentKeyStates[SDL_SCANCODE_D] ) mVelX = datas.speed;
                else mVelX =0;
                break;
            }

            case SDLK_d:
            {
                if ( currentKeyStates[SDL_SCANCODE_A] ) mVelX = - datas.speed;
                else mVelX =0;
                break;
            }
        }
    }
}

void Characters::checkvar()
{
    mPosX += mVelX;
    if( mPosX < 0 )
    {
        mPosX = 0;
    }
    else if ( mPosX + mWidth> SCREEN_WIDTH )
    {
        mPosX = SCREEN_WIDTH - mWidth;
    }

    mPosY += mVelY;
    if( mPosY < 0 )
    {
        mPosY = 0;
    }
    else if ( mPosY + mHeight > SCREEN_HEIGHT )
    {
        mPosY = SCREEN_HEIGHT - mHeight;
    }
}  

void Characters::animated()
{
    if (mTexture != nullptr)
    {
        mTexture->render(mPosX, mPosY);
    }
}