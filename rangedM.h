#include <SDL.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Timer.h"
using namespace std;

class Bullet
{
    public:
        Bullet(Texture* texture = nullptr);
        void addbullet(Texture* texture1, Texture* texture2, Texture* texture3, int x, int y, int v, double angle, vector<vector<int>> bm);
        void var();
        void active(int xs, int ys, int x, int y); // xs, ys: tọa độ nhân vật,; x, y: tọa độ camera
        void free();
        double mVelX, mVelY;
        double mPosX, mPosY;
        int mWidth, mHeight;
        int framerate;
        int cframe;
        Uint32 lasttime;
        vector<SDL_Rect> clips;
        vector<SDL_Rect> clipf;
        vector<SDL_Rect> clipe;
        double angle;
        double deltatime;
        Timer speedrender;
        Texture* mTextures;
        Texture* mTexturef;
        Texture* mTexturee;
        int camx, camy;
        int step, step1;
        bool start, startsfx;
        int xx, yy, xa, ya;
        bool v;
        vector<vector<int>> blockmap;
};

Bullet::Bullet(Texture* texture)
{
    mTextures = nullptr;
    mTexturef = nullptr;
    mTexturee = nullptr;
    mPosX = 0;
    mPosY = 0;
    mWidth = 0;
    mHeight = 0;
    framerate = 0;
    cframe = 0;
    deltatime = speedrender.getdeltatime();
    lasttime = 0;
    angle = 0;
    mVelX = 0;
    mVelY = 0;
    camx = 0;
    camy = 0;
    step = 0;
    step1 = 0;
    start = false;
    startsfx = false;
    v = false;
}

void Bullet::addbullet(Texture* texture1, Texture* texture2, Texture* texture3,int x, int y, int v, double angle, vector<vector<int>> bm)
{
    blockmap = bm;
    mTextures = texture1;
    mTexturef = texture2;
    mTexturee = texture3;
    mPosX = x;
    mPosY = y;
    mVelX = v; 
    mVelY = v;
    mWidth = 192;
    mHeight = 192;
    clips.clear();
    for (int i = 0; i < 5; i++)
    {
        SDL_Rect clip;
        clip.x = 0;
        clip.y = i * mHeight;
        clip.w = mWidth;
        clip.h = mHeight;
        clips.push_back(clip);
    }
    clipf.clear();
    for (int i = 0; i < 5; i++)
    {
        SDL_Rect clip;
        clip.x = 0;
        clip.y = i * mHeight;
        clip.w = mWidth;
        clip.h = mHeight;
        clipf.push_back(clip);
    }
    clipe.clear();
    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
    {
        SDL_Rect clip;
        clip.x = j * mWidth;
        clip.y = i * mHeight;
        clip.w = mWidth;
        clip.h = mHeight;
        clipe.push_back(clip);
    }
    this->angle = angle;
    step = 0;
    step1 = 0;
    cframe = 0;
    start = true;
    v = false;
}

void Bullet::active(int xs, int ys, int x, int y)
{
    int xm, ym;
    SDL_GetMouseState( &xm, &ym );
    xx = xm + x;
    yy = ym + y;

    if (start && !v)
    {
        SDL_Rect custom;
        custom.w = 48;
        custom.h = 48;
        SDL_Point center;
        center.x = 48/2;
        center.y = 48/2;
        camx = x;
        camy = y;
        int xs2;
        int ys2;
        xs2 = xs;
        ys2 = ys+2000;
        deltatime = speedrender.getdeltatime();
        if (step == 0)
        {
            framerate = 25;
            step = 1;
        }
        if(step == 1)
        {
            custom.x = xs - 45*cos(atan2(( ( ys) - yy ) , ( ( xs) - xx ))) - camx;
            custom.y = ys - 45*sin(atan2(( ( ys) - yy ) , ( ( xs) - xx ))) - camy;
            if (SDL_GetTicks() - lasttime > 1000/framerate) 
            {
                cframe = (cframe + 1) % (6);
                lasttime = SDL_GetTicks();
            }
            mTextures->render(0, 0, &custom, &clips[cframe-1], false, angle, &center);
            if (cframe == 5) 
            {
                step = 2;
                framerate = 20;
                angle = atan2(( ( ys) - yy ) , ( ( xs) - xx ))/M_PI*180 + 45;
                mVelX *= -cos(atan2(( ( ys) - yy ) , ( ( xs) - xx )));
                mVelY *= -sin(atan2(( ( ys) - yy ) , ( ( xs) - xx )));
                mPosX = custom.x + camx;
                mPosY = custom.y + camy;
            }
        }
        if(step == 2)
        {
            mPosX += mVelX*deltatime/1000;
            mPosY += mVelY*deltatime/1000;
            custom.x = mPosX - camx;
            custom.y = mPosY - camy;

            if (SDL_GetTicks() - lasttime > 1000/framerate) 
            {
                cframe = (cframe + 1) % (6);
                lasttime = SDL_GetTicks();
            }
            mTexturef->render(0, 0, &custom, &clipf[cframe], false, angle, &center);
            if (cframe == 4) 
            {
                step = 3;
                framerate = 10;
            }
        }
        if(step == 3)
        {
            mPosX += mVelX*deltatime/1000;
            mPosY += mVelY*deltatime/1000;
            custom.x = mPosX - camx;
            custom.y = mPosY - camy;
            mTexturef->render(0, 0, &custom, &clipf[4], false, angle, &center);
        }
    }
    var();
}

void Bullet::var()
{
    if (start)
    {
        SDL_Rect custom;
        custom.w = 48;
        custom.h = 48;
        custom.x = mPosX + 24;
        custom.y = mPosY + 24;
        int blockx = floor( custom.x / 48 );
        int blocky = floor( custom.y / 48 );
        if (blockmap[blockx][blocky] == 1) 
        {
            v = true;
            if (step1 == 0)
            {
                startsfx = true;
                framerate = 25;
                step1 = 1;
                cframe = 0;
                xa = mPosX;
                ya = mPosY;
            }
            if(step1 == 1)
            {
                custom.x = xa - camx;
                custom.y = ya - camy;
                if (SDL_GetTicks() - lasttime > 1000/framerate) 
                {
                    cframe = (cframe + 1) % (17);
                    lasttime = SDL_GetTicks();
                }
                mTexturee->render(0, 0, &custom, &clipe[cframe-1]);
                if (cframe == 16) 
                {
                    start = false;
                    v = false;
                }
            }
        }
    }
}

void Bullet::free()
{
    mTextures = nullptr;
    mTexturef = nullptr;
    mTexturee = nullptr;
    mPosX = 0;
    mPosY = 0;
    mWidth = 0;
    mHeight = 0;
    framerate = 0;
    cframe = 0;
    deltatime = speedrender.getdeltatime();
    lasttime = 0;
    angle = 0;
    mVelX = 0;
    mVelY = 0;
    camx = 0;
    camy = 0;
    start = false;
}

