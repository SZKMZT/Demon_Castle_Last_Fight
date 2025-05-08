#include <SDL.h>
#include <cmath>
#include <vector>
#include <iostream>
#include "Timer.h"
using namespace std;

struct datachar
{
    int health;
    int power;
    int mana;
    int speed; // pixel/s
};

enum Direction
{
    FRONT = 0,
    BEHIND = 1,
    RIGHT = 2,
    LEFT = 3,
};

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern bool smooth_camera;

class Characters
{
    public:
        Characters(Texture* texture = nullptr);
        void addtexture(Texture* texture, int w, int h, int f);
        void motion( SDL_Event* e );
        void checkvar();
        void animated(int mx, int my);
        void getmapxy(Texture* mapp);
        void cameraxy();
        SDL_Rect camxy();
        int mx_camx, my_camy;
    private:
        datachar datas;
        Texture* mTexture;
        int mVelX, mVelY;
        double mPosX, mPosY;
        int mWidth;
		int mHeight;
        vector<SDL_Rect> clipss;
        int framerate;
        int cframe;
        Direction direction;
        SDL_Rect camera;
        double camx, camy;
        double camvx, camvy;
        int mapx, mapy;
        Timer speedrender;
        Uint32 lasttime;
        Uint32 lasttime2;
        int deltatime;
};

Characters::Characters(Texture* texture)
{
    mTexture = nullptr;
    datas.health = 100;
    datas.power = 5;
    datas.mana = 100;
    datas.speed = 6*48;
    mVelX = 0;
    mVelY = 0;
    mPosX = 0;
    mPosY = 0;
    mWidth = 0;
    mHeight = 0;
    framerate = 0;
    cframe = 1;
    direction = FRONT;
    camera.x = 0; 
    camera.y = 0;
    camera.w = SCREEN_WIDTH;
    camera.h = SCREEN_HEIGHT;
    camx = camera.w / 2;
    camy = camera.h / 2;
    camvx = 0;
    camvy = 0;
    mapx = 0;
    mapy = 0;
    lasttime = 0;
    lasttime2 = 0;
    deltatime = speedrender.getdeltatime();
    mx_camx = 0;
    my_camy = 0;
}

void Characters::addtexture(Texture* texture, int w, int h, int f)
{
    mTexture = texture;
    mWidth = texture->mw / w;
    mHeight = texture->mh / h;
    framerate = f;
    clipss.clear();
    for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++)
    {
        SDL_Rect clip;
        clip.x = j * mWidth;
        clip.y = i * mHeight;
        clip.w = mWidth;
        clip.h = mHeight;
        clipss.push_back(clip);
    }
}

void Characters::motion( SDL_Event* e )
{
    if( e->type == SDL_KEYDOWN && e->key.repeat == 0 )
    {
        switch( e->key.keysym.sym )
        {
            case SDLK_w: mVelY = - datas.speed; direction = FRONT; break;
            case SDLK_s: mVelY = datas.speed; direction = BEHIND; break;
            case SDLK_a: mVelX = - datas.speed; direction = LEFT; break;
            case SDLK_d: mVelX = datas.speed; direction = RIGHT; break;
        }
    }

    else if( e->type == SDL_KEYUP && e->key.repeat == 0)
    {
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    
        switch( e->key.keysym.sym )
        {
            case SDLK_w: 
            if ( currentKeyStates[SDL_SCANCODE_S] ) 
            {
                mVelY = datas.speed;
                direction = BEHIND;
            }
            else mVelY =0;
            if ( currentKeyStates[SDL_SCANCODE_D] && currentKeyStates[SDL_SCANCODE_A]) mVelX < 0 ? direction = LEFT : direction = RIGHT;
            else if ( currentKeyStates[SDL_SCANCODE_D] ) direction = RIGHT;
            else if ( currentKeyStates[SDL_SCANCODE_A] ) direction = LEFT;
            break;

            case SDLK_s:
            if ( currentKeyStates[SDL_SCANCODE_W] ) 
            {
                mVelY = - datas.speed;
                direction = FRONT;
            }
            else mVelY =0;
            if ( currentKeyStates[SDL_SCANCODE_D] && currentKeyStates[SDL_SCANCODE_A]) mVelX < 0 ? direction = LEFT : direction = RIGHT;
            else if ( currentKeyStates[SDL_SCANCODE_D] ) direction = RIGHT;
            else if ( currentKeyStates[SDL_SCANCODE_A] ) direction = LEFT;
            break;

            case SDLK_a:
            if ( currentKeyStates[SDL_SCANCODE_D] ) 
            {
                mVelX = datas.speed;
                direction = RIGHT;
            }
            else mVelX =0;
            if ( currentKeyStates[SDL_SCANCODE_S] && currentKeyStates[SDL_SCANCODE_W]) mVelY < 0 ? direction = FRONT : direction = BEHIND;
            else if ( currentKeyStates[SDL_SCANCODE_S] ) direction = BEHIND;
            else if ( currentKeyStates[SDL_SCANCODE_W] ) direction = FRONT;
            break;

            case SDLK_d:
            if ( currentKeyStates[SDL_SCANCODE_A] ) 
            {
                mVelX = - datas.speed;
                direction = LEFT;
            }
            else mVelX =0;
            if ( currentKeyStates[SDL_SCANCODE_S] && currentKeyStates[SDL_SCANCODE_W]) mVelY < 0 ? direction = FRONT : direction = BEHIND;
            else if ( currentKeyStates[SDL_SCANCODE_S] ) direction = BEHIND;
            else if ( currentKeyStates[SDL_SCANCODE_W] ) direction = FRONT;
            break;
        }
    }
}

void Characters::checkvar()
{
    deltatime = speedrender.getdeltatime();
    if (SDL_GetTicks() - lasttime2 <= 4)
    {
        mPosX += mVelX*4/1000;
        mPosY += mVelY*4/1000;
    }
    else
    {
        mPosX += mVelX*deltatime/1000;
        mPosY += mVelY*deltatime/1000;
    }
    if( mPosX < 0 )
    {
        mPosX = 0;
    }
    else if ( mPosX + mWidth> mapx )
    {
        mPosX = mapx - mWidth;
    }

    if( mPosY < 0 )
    {
        mPosY = 0;
    }
    else if ( mPosY + mHeight > mapy )
    {
        mPosY = mapy - mHeight;
    }}  

void Characters::animated(int mx, int my)
{
    if (SDL_GetTicks() - lasttime > 1000/framerate) {
        cframe = (cframe + 1) % 3;
        lasttime = SDL_GetTicks();
    }
    if (mTexture != nullptr)
    {
        if (mVelX == 0 && mVelY == 0)
        {
            cframe = 2;
            switch (direction)
            {
                case FRONT:
                mTexture->render(mx, my, nullptr, &clipss[7]);
                break;

                case BEHIND:
                mTexture->render(mx, my, nullptr, &clipss[1]);
                break;

                case RIGHT:
                mTexture->render(mx, my, nullptr, &clipss[4], false, 0, nullptr, SDL_FLIP_HORIZONTAL);
                break;

                case LEFT:
                mTexture->render(mx, my, nullptr, &clipss[4]);
                break;
            
            default:
                break;
            }
        }
        else
        {
            switch (direction)
            {
                case FRONT:
                mTexture->render(mx, my, nullptr, &clipss[6+cframe]);
                break;

                case BEHIND:
                mTexture->render(mx, my, nullptr, &clipss[0+cframe]);
                break;

                case RIGHT:
                mTexture->render(mx, my, nullptr, &clipss[3+cframe], false, 0, nullptr, SDL_FLIP_HORIZONTAL);
                break;

                case LEFT:
                mTexture->render(mx, my, nullptr, &clipss[3+cframe]);
                break;
            default:
                break;
            }
        }
    }
}

void Characters::getmapxy(Texture* mapp)
{
    mapx = mapp->mw;
    mapy = mapp->mh;
}

void Characters::cameraxy()
{
    if (SDL_GetTicks() - lasttime >= 4) 
    {
        lasttime2 = SDL_GetTicks();
    }

    camera.w = SCREEN_WIDTH;
    camera.h = SCREEN_HEIGHT;
    if (SDL_GetTicks() - lasttime >= 4)
    {
        if (smooth_camera)
        {
            double distance = sqrt( pow( camx - ( mPosX + mWidth / 2 ), 2 ) + pow( camy - ( mPosY + mHeight / 2 ), 2 ) );
            if (distance < double(datas.speed)*deltatime/2000)
            {
                camx = mPosX + mWidth / 2;
                camy = mPosY + mHeight / 2;
            }
            else
            {
                camvx = (distance * (datas.speed * 9 / 10) / 150 + datas.speed / 10) * cos(atan2(( ( mPosY + mHeight / 2 ) - camy ) , ( ( mPosX + mWidth / 2 ) - camx )));
                camvy = (distance * (datas.speed * 9 / 10) / 150 + datas.speed / 10) * sin(atan2(( ( mPosY + mHeight / 2 ) - camy ) , ( ( mPosX + mWidth / 2 ) - camx )));
                /*if (mVelX == 0 && mVelY == 0)
                {
                    camvx = datas.speed * cos(atan2((( mPosY + mHeight / 2 ) - camy ) , ( ( mPosX + mWidth / 2 ) - camx )));
                    camvy = datas.speed * sin(atan2((( mPosY + mHeight / 2 ) - camy ) , ( ( mPosX + mWidth / 2 ) - camx )));
        
                }
                else
                {
                    camvx = (distance * datas.speed / 150 ) * cos(atan2(( ( mPosY + mHeight / 2 ) - camy ) , ( ( mPosX + mWidth / 2 ) - camx )));
                    camvy = (distance * datas.speed / 150 ) * sin(atan2(( ( mPosY + mHeight / 2 ) - camy ) , ( ( mPosX + mWidth / 2 ) - camx )));
        
                }*/
            }   

            camx += camvx*deltatime/1000;
            if( camx < 0 )
            {
                camx = 0;
            }
            else if ( camx > mapx )
            {
                camx = mapx;
            }

            camy += camvy*deltatime/1000;
            if( camy < 0 )
            {
                camy = 0;
            }
            else if ( camy > mapy )
            {
                camy = mapy;
            }

            cout << camx << " " << camy << endl;
        }
        else
        {
            camx = mPosX + mWidth / 2;
            camy = mPosY + mHeight / 2;
        }

        camera.x = int(camx - SCREEN_WIDTH / 2);
        camera.y = int(camy - SCREEN_HEIGHT / 2);

        if( camera.x < 0 ) camera.x = 0; 
        if( camera.y < 0 ) camera.y = 0;
        if( camera.x > mapx - camera.w ) camera.x = mapx - camera.w;
        if( camera.y > mapy - camera.h ) camera.y = mapy - camera.h;

        mx_camx = mPosX - camera.x;
        my_camy = mPosY - camera.y;
    }
}

SDL_Rect Characters::camxy()
{
    return camera;
}
