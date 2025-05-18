#include <SDL.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <SDL_mixer.h>
#include <cstdlib>
#include <ctime>
#include "Timer.h"
using namespace std;

/*
special type:
0.classic
1.fire
2.earth
3.wind
4.thunder
5.ice
6.holy
7.dark arrow
8.black hole

sfx type:
0.fire sfx
1.fire explosion sfx
2.earth explosion sfx
3.thunder sfx
4.thunder explosion sfx
5.ice explosion sfx
6.explosion sfx
*/

class Bullet
{
    public:
        Bullet();
        void addbullet(int special, int x, int y, int v, double angle, vector<vector<int>> bm, bool allyy, bool anglelockk, bool diss, int dam);
        void var();
        void active(int xs, int ys, int x, int y); // xs, ys: tọa độ nhân vật,; x, y: tọa độ camera
        void free();
        double mVelX, mVelY;
        double mPosX, mPosY;
        int mWidth, mHeight;
        int framerate;
        int cframe;
        Uint32 lasttime;
        vector<SDL_Rect> clip;
        double angle; //0: bên trái, 180/-180: bên phải, 90: bên trên, -90: bên dưới
        double deltatime;
        Timer speedrender;
        int camx, camy;
        int step, step1;
        bool start;
        int xx, yy, xa, ya;
        bool v;
        vector<vector<int>> blockmap;
        bool collide; //va chạm
        bool ally; //đồng minh
        bool anglelock;
        int special;
        bool dis;
        int damage;
        bool getdamage;
};

extern Texture bullettexture[9];
extern Mix_Chunk* sfxsound[7];

Bullet::Bullet()
{
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
    v = false;
    collide = false;
    ally = true;
    anglelock = true;
    special = 0;
    srand(time(0));
    dis = false;
    damage = 0;
    getdamage = false;
}

void Bullet::addbullet(int special, int x, int y, int v, double angle, vector<vector<int>> bm, bool allyy, bool anglelockk, bool diss, int dam)
{
    blockmap = bm;
    mPosX = x;
    mPosY = y;
    mVelX = v; 
    mVelY = v;
    mWidth = 192;
    mHeight = 192;
    int heightpixel = bullettexture[special].getHeight() / mHeight;
    clip.clear();
    for (int i = 0; i < heightpixel; i++)
    {
        SDL_Rect clipp;
        clipp.x = 0;
        clipp.y = i * mHeight;
        clipp.w = mWidth;
        clipp.h = mHeight;
        clip.push_back(clipp);
    }
    this->angle = angle+45;
    step = 0;
    step1 = 0;
    cframe = 0;
    start = true;
    v = false;
    collide = false;
    ally = allyy;
    anglelock = anglelockk;
    this->special = special;
    dis = diss;
    damage = dam;
    if(!dis)
    {
        if(special==0) Mix_PlayChannel(-1, sfxsound[0], 0);
        if(special==1) Mix_PlayChannel(-1, sfxsound[0], 0);
        if(special==3) Mix_PlayChannel(-1, sfxsound[0], 0);
        if(special==4) Mix_PlayChannel(-1, sfxsound[3], 0);
        if(special==5) Mix_PlayChannel(-1, sfxsound[0], 0);
        if(special==6) Mix_PlayChannel(-1, sfxsound[0], 0);
        if(special==7) Mix_PlayChannel(-1, sfxsound[0], 0);
        if(special==8) Mix_PlayChannel(-1, sfxsound[0], 0);
    }
}
//mịa mấy cái quay quay ảo đ chịu được :V từ chối hiểu, thôi chạy được là được
void Bullet::active(int xs, int ys, int x, int y) //xs, ys là tọa độ đầu tiên của viên đạn, tạo ra vòng phép và bắn; x, y là tọa độ camera
{
    int xm, ym;
    SDL_GetMouseState( &xm, &ym );
    xx = xm + x - 24;
    yy = ym + y - 24;

    camx = x;
    camy = y;

    SDL_Rect custom;
    custom.w = 48;
    custom.h = 48;
    SDL_Point center;
    center.x = 48/2;
    center.y = 48/2;

    if (start && !v)
    {
        deltatime = speedrender.getdeltatime();
        if(special == 0)
        {
            if (step == 0)
            {
                framerate = 25;
                step = 1;
                cframe = -1;
            }
            if(step == 1)
            {
                if (anglelock)
                {
                    custom.x = mPosX + 45*cos((angle+135)*M_PI/180) - camx;
                    custom.y = mPosY + 45*sin((angle+135)*M_PI/180) - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (5);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe == -1) bullettexture[0].render(0, 0, &custom, &clip[0], false, angle, &center);
                    else bullettexture[0].render(0, 0, &custom, &clip[cframe], false, angle, &center);
                    if (cframe == 4) 
                    {
                        step = 2;
                        framerate = 20;
                        cframe = -1;
                        mVelX *= cos((angle+135)*M_PI/180);
                        mVelY *= sin((angle+135)*M_PI/180);
                        mPosX = custom.x + camx;
                        mPosY = custom.y + camy;
                    }
                }
                else
                {
                    custom.x = xs - 45*cos(atan2(( ys - yy ) , ( xs - xx ))) - camx;
                    custom.y = ys - 45*sin(atan2(( ys - yy ) , ( xs - xx ))) - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (5);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe == -1) bullettexture[0].render(0, 0, &custom, &clip[0], false, angle, &center);
                    else bullettexture[0].render(0, 0, &custom, &clip[cframe], false, angle, &center);
                    if (cframe == 4) 
                    {
                        step = 2;
                        framerate = 20;
                        cframe = -1;
                        angle = atan2(( ys - yy ) , ( xs - xx ))/M_PI*180 + 45;
                        mVelX *= -cos(atan2(( ys - yy ) , ( xs - xx )));
                        mVelY *= -sin(atan2(( ys - yy ) , ( xs - xx )));
                        mPosX = custom.x + camx;
                        mPosY = custom.y + camy;
                    }
                }
            }
            if(step == 2)
            {
                mPosX += mVelX*deltatime/1000;
                mPosY += mVelY*deltatime/1000;
                custom.x = round(mPosX - camx);
                custom.y = round(mPosY - camy);

                if (SDL_GetTicks() - lasttime > 1000/framerate) 
                {
                    cframe = (cframe + 1) % (5);
                    lasttime = SDL_GetTicks();
                }
                if(cframe == -1) bullettexture[0].render(0, 0, &custom, &clip[5], false, angle, &center);
                else bullettexture[0].render(0, 0, &custom, &clip[cframe+5], false, angle, &center);
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
                custom.x = round(mPosX - camx);
                custom.y = round(mPosY - camy);
                bullettexture[0].render(0, 0, &custom, &clip[9], false, angle, &center);
            }
        }
        else if (special == 1)
        {
            if (step == 0)
            {
                framerate = 30;
                step = 1;
                cframe = -1;
            }
            if(step == 1)
            {
                if (anglelock)
                {
                    custom.x = mPosX + 45*cos((angle+135)*M_PI/180) - camx;
                    custom.y = mPosY + 45*sin((angle+135)*M_PI/180) - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (5);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe==-1) bullettexture[1].render(0, 0, &custom, &clip[0], false, angle, &center);
                    else bullettexture[1].render(0, 0, &custom, &clip[cframe], false, angle, &center);
                    if (cframe == 4) 
                    {
                        step = 2;
                        framerate = 20;
                        cframe = -1;
                        mVelX *= cos((angle+135)*M_PI/180);
                        mVelY *= sin((angle+135)*M_PI/180);
                        mPosX = custom.x + camx;
                        mPosY = custom.y + camy;
                    }
                }
                else
                {
                    custom.x = xs - 45*cos(atan2(( ys - yy ) , ( xs - xx ))) - camx;
                    custom.y = ys - 45*sin(atan2(( ys - yy ) , ( xs - xx ))) - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (5);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe==-1) bullettexture[1].render(0, 0, &custom, &clip[0], false, angle, &center);
                    else bullettexture[1].render(0, 0, &custom, &clip[cframe], false, angle, &center);
                    if (cframe == 4) 
                    {
                        step = 2;
                        framerate = 10;
                        cframe = -1;
                        angle = atan2(( ys - yy ) , ( xs - xx ))/M_PI*180 + 45;
                        mVelX *= -cos(atan2(( ys - yy ) , ( xs - xx )));
                        mVelY *= -sin(atan2(( ys - yy ) , ( xs - xx )));
                        mPosX = custom.x + camx;
                        mPosY = custom.y + camy;
                    }
                }
            }
            if(step == 2)
            {
                mPosX += mVelX*deltatime/1000;
                mPosY += mVelY*deltatime/1000;
                custom.x = round(mPosX - camx);
                custom.y = round(mPosY - camy);

                if (SDL_GetTicks() - lasttime > 1000/framerate) 
                {
                    cframe = (cframe + 1) % (5);
                    lasttime = SDL_GetTicks();
                }
                if(cframe==-1) bullettexture[1].render(0, 0, &custom, &clip[5], false, angle, &center);
                else bullettexture[1].render(0, 0, &custom, &clip[cframe+5], false, angle, &center);
                if (cframe == 4) 
                {
                    step = 3;
                    framerate = 8;
                }
            }
            if(step == 3)
            {
                if (SDL_GetTicks() - lasttime > 1000/framerate) 
                {
                    cframe = rand() % 3;
                    lasttime = SDL_GetTicks();
                }
                mPosX += mVelX*deltatime/1000;
                mPosY += mVelY*deltatime/1000;
                custom.x = round(mPosX - camx);
                custom.y = round(mPosY - camy);
                bullettexture[1].render(0, 0, &custom, &clip[9+cframe], false, angle, &center);
            }
        }
        else if (special == 2)
        {
            if (step == 0)
            {
                framerate = 25;
                step = 1;
                cframe = -1;
            }
            if(step == 1)
            {
                if (anglelock)
                {
                    custom.x = mPosX + 45*cos((angle+135)*M_PI/180) - camx;
                    custom.y = mPosY + 45*sin((angle+135)*M_PI/180) - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (14);
                        lasttime = SDL_GetTicks();
                        if (cframe == 6 && !dis)Mix_PlayChannel(-1, sfxsound[0], 0);
                    }
                    if(cframe==-1) bullettexture[2].render(0, 0, &custom, &clip[0], false, angle+45, &center);
                    else bullettexture[2].render(0, 0, &custom, &clip[cframe], false, angle+45, &center);
                    if (cframe == 13) 
                    {
                        step = 2;
                        framerate = 20;
                        cframe = -1;
                        mVelX *= cos((angle+135)*M_PI/180);
                        mVelY *= sin((angle+135)*M_PI/180);
                        mPosX = custom.x + camx;
                        mPosY = custom.y + camy;
                        angle += 45;
                    }
                }
                else
                {
                    angle = atan2(( ys - yy ) , ( xs - xx ))/M_PI*180 + 90;
                    custom.x = xs - 45*cos(atan2(( ys - yy ) , ( xs - xx ))) - camx;
                    custom.y = ys - 45*sin(atan2(( ys - yy ) , ( xs - xx ))) - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (14);
                        lasttime = SDL_GetTicks();
                        if (cframe == 6 )Mix_PlayChannel(-1, sfxsound[0], 0);
                    }
                    if(cframe==-1) bullettexture[2].render(0, 0, &custom, &clip[0], false, angle, &center);
                    else bullettexture[2].render(0, 0, &custom, &clip[cframe], false, angle, &center);
                    if (cframe == 13) 
                    {
                        step = 2;
                        framerate = 10;
                        cframe = -1;
                        angle = atan2(( ys - yy ) , ( xs - xx ))/M_PI*180 + 90;
                        mVelX *= -cos(atan2(( ys - yy ) , ( xs - xx )));
                        mVelY *= -sin(atan2(( ys - yy ) , ( xs - xx )));
                        mPosX = custom.x + camx;
                        mPosY = custom.y + camy;
                    }
                }
            }
            if(step == 2)
            {
                mPosX += mVelX*deltatime/1000;
                mPosY += mVelY*deltatime/1000;
                custom.x = round(mPosX - camx);
                custom.y = round(mPosY - camy);
                bullettexture[2].render(0, 0, &custom, &clip[13], false, angle, &center);
            }
        }
        else if (special == 3)
        {
            if (step == 0)
            {
                framerate = 25;
                step = 1;
                cframe = -1;
            }
            if(step == 1)
            {
                if (anglelock)
                {
                    custom.x = mPosX + 45*cos((angle+135)*M_PI/180) - camx;
                    custom.y = mPosY + 45*sin((angle+135)*M_PI/180) - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (5);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe==-1) bullettexture[3].render(0, 0, &custom, &clip[0], false, angle-23, &center);
                    else bullettexture[3].render(0, 0, &custom, &clip[cframe], false, angle-23, &center);
                    if (cframe == 4) 
                    {
                        step = 2;
                        framerate = 20;
                        cframe = -1;
                        mVelX *= cos((angle+135)*M_PI/180);
                        mVelY *= sin((angle+135)*M_PI/180);
                        mPosX = custom.x + camx;
                        mPosY = custom.y + camy;
                        angle -= 23;
                    }
                }
                else
                {
                    angle = atan2(( ys - yy ) , ( xs - xx ))/M_PI*180 + 22;
                    custom.x = xs - 45*cos(atan2(( ys - yy ) , ( xs - xx ))) - camx;
                    custom.y = ys - 45*sin(atan2(( ys - yy ) , ( xs - xx ))) - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (5);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe==-1) bullettexture[3].render(0, 0, &custom, &clip[0], false, angle, &center);
                    else bullettexture[3].render(0, 0, &custom, &clip[cframe], false, angle, &center);
                    if (cframe == 4) 
                    {
                        step = 2;
                        framerate = 10;
                        cframe = -1;
                        angle = atan2(( ys - yy ) , ( xs - xx ))/M_PI*180 + 22;
                        mVelX *= -cos(atan2(( ys - yy ) , ( xs - xx )));
                        mVelY *= -sin(atan2(( ys - yy ) , ( xs - xx )));
                        mPosX = custom.x + camx;
                        mPosY = custom.y + camy;
                    }
                }
            }
            if(step == 2)
            {
                mPosX += mVelX*deltatime/1000;
                mPosY += mVelY*deltatime/1000;
                custom.x = round(mPosX - camx);
                custom.y = round(mPosY - camy);

                if (SDL_GetTicks() - lasttime > 1000/framerate) 
                {
                    cframe = (cframe + 1) % (5);
                    lasttime = SDL_GetTicks();
                }
                if(cframe==-1) bullettexture[3].render(0, 0, &custom, &clip[5], false, angle, &center);
                else bullettexture[3].render(0, 0, &custom, &clip[cframe+5], false, angle, &center);
                if (cframe == 3) 
                {
                    step = 3;
                }
            }
            if(step == 3)
            {
                mPosX += mVelX*deltatime/1000;
                mPosY += mVelY*deltatime/1000;
                custom.x = round(mPosX - camx);
                custom.y = round(mPosY - camy);
                bullettexture[3].render(0, 0, &custom, &clip[8], false, angle, &center);
            }
        }
        else if (special == 4)
        {
            if (step == 0)
            {
                framerate = 25;
                step = 1;
                cframe = -1;
            }
            if(step == 1)
            {
                if (anglelock)
                {
                    custom.x = mPosX + 45*cos((angle+135)*M_PI/180) - camx;
                    custom.y = mPosY + 45*sin((angle+135)*M_PI/180) - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (5);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe==-1) bullettexture[4].render(0, 0, &custom, &clip[0], false, angle, &center);
                    else bullettexture[4].render(0, 0, &custom, &clip[cframe], false, angle, &center);
                    if (cframe == 4) 
                    {
                        step = 2;
                        framerate = 5;
                        cframe = -1;
                        mVelX *= cos((angle+135)*M_PI/180);
                        mVelY *= sin((angle+135)*M_PI/180);
                        mPosX = custom.x + camx;
                        mPosY = custom.y + camy;
                    }
                }
                else
                {
                    angle = atan2(( ys - yy ) , ( xs - xx ))/M_PI*180 + 45;
                    custom.x = xs - 45*cos(atan2(( ys - yy ) , ( xs - xx ))) - camx;
                    custom.y = ys - 45*sin(atan2(( ys - yy ) , ( xs - xx ))) - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (5);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe==-1) bullettexture[4].render(0, 0, &custom, &clip[0], false, angle, &center);
                    else bullettexture[4].render(0, 0, &custom, &clip[cframe], false, angle, &center);
                    if (cframe == 4) 
                    {
                        step = 2;
                        framerate = 5;
                        cframe = -1;
                        angle = atan2(( ys - yy ) , ( xs - xx ))/M_PI*180 + 45;
                        mVelX *= -cos(atan2(( ys - yy ) , ( xs - xx )));
                        mVelY *= -sin(atan2(( ys - yy ) , ( xs - xx )));
                        mPosX = custom.x + camx;
                        mPosY = custom.y + camy;
                    }
                }
            }
            if(step == 2)
            {
                if (SDL_GetTicks() - lasttime > 1000/framerate) 
                {
                    cframe = rand() % 4;
                    lasttime = SDL_GetTicks();
                }
                mPosX += mVelX*deltatime/1000;
                mPosY += mVelY*deltatime/1000;
                custom.x = round(mPosX - camx);
                custom.y = round(mPosY - camy);
                bullettexture[4].render(0, 0, &custom, &clip[5+cframe], false, angle, &center);
            }
        }
        else if (special == 5)
        {
            if (step == 0)
            {
                framerate = 30;
                step = 1;
                cframe = -1;
            }
            if(step == 1)
            {
                if (anglelock)
                {
                    custom.x = mPosX + 45*cos((angle+135)*M_PI/180) - camx;
                    custom.y = mPosY + 45*sin((angle+135)*M_PI/180) - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (5);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe==-1) bullettexture[5].render(0, 0, &custom, &clip[0], false, angle, &center);
                    else bullettexture[5].render(0, 0, &custom, &clip[cframe], false, angle, &center);
                    if (cframe == 4) 
                    {
                        step = 2;
                        framerate = 20;
                        cframe = -1;
                        mVelX *= cos((angle+135)*M_PI/180);
                        mVelY *= sin((angle+135)*M_PI/180);
                        mPosX = custom.x + camx;
                        mPosY = custom.y + camy;
                    }
                }
                else
                {
                    custom.x = xs - 45*cos(atan2(( ys - yy ) , ( xs - xx ))) - camx;
                    custom.y = ys - 45*sin(atan2(( ys - yy ) , ( xs - xx ))) - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (5);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe==-1) bullettexture[5].render(0, 0, &custom, &clip[0], false, angle, &center);
                    else bullettexture[5].render(0, 0, &custom, &clip[cframe], false, angle, &center);
                    if (cframe == 4) 
                    {
                        step = 2;
                        framerate = 10;
                        cframe = -1;
                        angle = atan2(( ys - yy ) , ( xs - xx ))/M_PI*180 + 45;
                        mVelX *= -cos(atan2(( ys - yy ) , ( xs - xx )));
                        mVelY *= -sin(atan2(( ys - yy ) , ( xs - xx )));
                        mPosX = custom.x + camx;
                        mPosY = custom.y + camy;
                    }
                }
            }
            if(step == 2)
            {
                mPosX += mVelX*deltatime/1000;
                mPosY += mVelY*deltatime/1000;
                custom.x = round(mPosX - camx);
                custom.y = round(mPosY - camy);

                if (SDL_GetTicks() - lasttime > 1000/framerate) 
                {
                    cframe = (cframe + 1) % (5);
                    lasttime = SDL_GetTicks();
                }
                if(cframe==-1) bullettexture[5].render(0, 0, &custom, &clip[5], false, angle, &center);
                else bullettexture[5].render(0, 0, &custom, &clip[cframe+5], false, angle, &center);
                if (cframe == 3) 
                {
                    step = 3;
                }
            }
            if(step == 3)
            {
                mPosX += mVelX*deltatime/1000;
                mPosY += mVelY*deltatime/1000;
                custom.x = round(mPosX - camx);
                custom.y = round(mPosY - camy);
                bullettexture[5].render(0, 0, &custom, &clip[8], false, angle, &center);
            }
        }
        else if (special == 6)
        {
            if (step == 0)
            {
                framerate = 30;
                step = 1;
                cframe = -1;
            }
            if(step == 1)
            {
                if (anglelock)
                {
                    custom.x = mPosX + 45*cos((angle+135)*M_PI/180) - camx;
                    custom.y = mPosY + 45*sin((angle+135)*M_PI/180) - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (5);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe==-1) bullettexture[6].render(0, 0, &custom, &clip[0], false, angle+45, &center);
                    else bullettexture[6].render(0, 0, &custom, &clip[cframe], false, angle+45, &center);
                    if (cframe == 4) 
                    {
                        step = 2;
                        framerate = 20;
                        cframe = -1;
                        mVelX *= cos((angle+135)*M_PI/180);
                        mVelY *= sin((angle+135)*M_PI/180);
                        mPosX = custom.x + camx;
                        mPosY = custom.y + camy;
                        angle += 45;
                    }
                }
                else
                {
                    angle = atan2(( ys - yy ) , ( xs - xx ))/M_PI*180 + 90;
                    custom.x = xs - 45*cos(atan2(( ys - yy ) , ( xs - xx ))) - camx;
                    custom.y = ys - 45*sin(atan2(( ys - yy ) , ( xs - xx ))) - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (5);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe==-1) bullettexture[6].render(0, 0, &custom, &clip[0], false, angle, &center);
                    else bullettexture[6].render(0, 0, &custom, &clip[cframe], false, angle, &center);
                    if (cframe == 4) 
                    {
                        step = 2;
                        framerate = 10;
                        cframe = -1;
                        angle = atan2(( ys - yy ) , ( xs - xx ))/M_PI*180 + 90;
                        mVelX *= -cos(atan2(( ys - yy ) , ( xs - xx )));
                        mVelY *= -sin(atan2(( ys - yy ) , ( xs - xx )));
                        mPosX = custom.x + camx;
                        mPosY = custom.y + camy;
                    }
                }
            }
            if(step == 2)
            {
                mPosX += mVelX*deltatime/1000;
                mPosY += mVelY*deltatime/1000;
                custom.x = round(mPosX - camx);
                custom.y = round(mPosY - camy);

                if (SDL_GetTicks() - lasttime > 1000/framerate) 
                {
                    cframe = (cframe + 1) % (5);
                    lasttime = SDL_GetTicks();
                }
                if(cframe==-1) bullettexture[6].render(0, 0, &custom, &clip[5], false, angle, &center);
                else bullettexture[6].render(0, 0, &custom, &clip[cframe+5], false, angle, &center);
                if (cframe == 4) 
                {
                    step = 3;
                }
            }
            if(step == 3)
            {
                mPosX += mVelX*deltatime/1000;
                mPosY += mVelY*deltatime/1000;
                custom.x = round(mPosX - camx);
                custom.y = round(mPosY - camy);
                bullettexture[6].render(0, 0, &custom, &clip[9], false, angle, &center);
            }
        }
        else if (special == 7)
        {
            if (step == 0)
            {
                framerate = 30;
                step = 1;
                cframe = -1;
            }
            if(step == 1)
            {
                if (anglelock)
                {
                    custom.x = mPosX + 45*cos((angle+135)*M_PI/180) - camx;
                    custom.y = mPosY + 45*sin((angle+135)*M_PI/180) - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (5);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe==-1) bullettexture[7].render(0, 0, &custom, &clip[0], false, angle, &center);
                    else bullettexture[7].render(0, 0, &custom, &clip[cframe], false, angle, &center);
                    if (cframe == 4) 
                    {
                        step = 2;
                        framerate = 20;
                        cframe = -1;
                        mVelX *= cos((angle+135)*M_PI/180);
                        mVelY *= sin((angle+135)*M_PI/180);
                        mPosX = custom.x + camx;
                        mPosY = custom.y + camy;
                    }
                }
                else
                {
                    custom.x = xs - 45*cos(atan2(( ys - yy ) , ( xs - xx ))) - camx;
                    custom.y = ys - 45*sin(atan2(( ys - yy ) , ( xs - xx ))) - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (5);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe==-1) bullettexture[7].render(0, 0, &custom, &clip[0], false, angle, &center);
                    else bullettexture[7].render(0, 0, &custom, &clip[cframe], false, angle, &center);
                    if (cframe == 4) 
                    {
                        step = 2;
                        framerate = 10;
                        cframe = -1;
                        angle = atan2(( ys - yy ) , ( xs - xx ))/M_PI*180 + 45;
                        mVelX *= -cos(atan2(( ys - yy ) , ( xs - xx )));
                        mVelY *= -sin(atan2(( ys - yy ) , ( xs - xx )));
                        mPosX = custom.x + camx;
                        mPosY = custom.y + camy;
                    }
                }
            }
            if(step == 2)
            {
                mPosX += mVelX*deltatime/1000;
                mPosY += mVelY*deltatime/1000;
                custom.x = round(mPosX - camx);
                custom.y = round(mPosY - camy);

                if (SDL_GetTicks() - lasttime > 1000/framerate) 
                {
                    cframe = (cframe + 1) % (5);
                    lasttime = SDL_GetTicks();
                }
                if(cframe==-1) bullettexture[7].render(0, 0, &custom, &clip[5], false, angle, &center);
                else bullettexture[7].render(0, 0, &custom, &clip[cframe+5], false, angle, &center);
                if (cframe == 4) 
                {
                    step = 3;
                }
            }
            if(step == 3)
            {
                mPosX += mVelX*deltatime/1000;
                mPosY += mVelY*deltatime/1000;
                custom.x = round(mPosX - camx);
                custom.y = round(mPosY - camy);
                bullettexture[7].render(0, 0, &custom, &clip[9], false, angle, &center);
            }
        }
        else if (special == 8)
        {
            if (step == 0)
            {
                framerate = 15;
                step = 1;
                cframe = -1;
            }
            if(step == 1)
            {
                if (anglelock)
                {
                    custom.x = mPosX + 45*cos((angle+135)*M_PI/180) - camx;
                    custom.y = mPosY + 45*sin((angle+135)*M_PI/180) - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (5);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe==-1) bullettexture[8].render(0, 0, &custom, &clip[0], false, angle, &center);
                    else bullettexture[8].render(0, 0, &custom, &clip[cframe], false, angle, &center);
                    if (cframe == 4) 
                    {
                        step = 2;
                        framerate = 20;
                        cframe = -1;
                        mVelX *= cos((angle+135)*M_PI/180);
                        mVelY *= sin((angle+135)*M_PI/180);
                        mPosX = custom.x + camx;
                        mPosY = custom.y + camy;
                    }
                }
                else
                {
                    custom.x = xs - 45*cos(atan2(( ys - yy ) , ( xs - xx ))) - camx;
                    custom.y = ys - 45*sin(atan2(( ys - yy ) , ( xs - xx ))) - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (5);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe==-1) bullettexture[8].render(0, 0, &custom, &clip[0], false, angle, &center);
                    else bullettexture[8].render(0, 0, &custom, &clip[cframe], false, angle, &center);
                    if (cframe == 4) 
                    {
                        step = 2;
                        framerate = 8;
                        cframe = -1;
                        angle = atan2(( ys - yy ) , ( xs - xx ))/M_PI*180 + 45;
                        mVelX *= -cos(atan2(( ys - yy ) , ( xs - xx )));
                        mVelY *= -sin(atan2(( ys - yy ) , ( xs - xx )));
                        mPosX = custom.x + camx;
                        mPosY = custom.y + camy;
                    }
                }
            }
            if(step == 2)
            {
                if (SDL_GetTicks() - lasttime > 1000/framerate) 
                {
                    cframe = rand() % 4;
                    lasttime = SDL_GetTicks();
                }
                mPosX += mVelX*deltatime/1000;
                mPosY += mVelY*deltatime/1000;
                custom.x = round(mPosX - camx);
                custom.y = round(mPosY - camy);
                bullettexture[8].render(0, 0, &custom, &clip[5+cframe], false, angle, &center);
            }
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

        if (mPosX < 0) mPosX = 0;
        if (mPosX > blockmap.size()*48-48) mPosX = blockmap.size()*48-48;
        if (mPosY < 0) mPosY = 0;
        if (mPosY > blockmap[0].size()*48-48) mPosY = blockmap[0].size()*48-48;

        int blockx = floor( (mPosX + 24) / 48 );
        int blocky = floor( (mPosY + 24) / 48 );
        
        if(blockmap[blockx][blocky] == 1 && dis) start = false;
        if ((blockmap[blockx][blocky] == 1 || collide) && start ) 
        {
            v = true;
            if(special == 0)
            {
                if (step1 == 0)
                {
                    Mix_PlayChannel(-1, sfxsound[1], 0);
                    framerate = 25;
                    step1 = 1;
                    cframe = -1;
                    xa = mPosX;
                    ya = mPosY;
                }
                if(step1 == 1)
                {
                    custom.x = xa - camx;
                    custom.y = ya - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (15);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe == -1) bullettexture[1].render(0, 0, &custom, &clip[cframe+9]);
                    else bullettexture[1].render(0, 0, &custom, &clip[cframe+10]);
                    if (cframe == 14) 
                    {
                        start = false;
                        v = false;
                        collide = false;
                    }
                }
            }
            else if (special == 1)
            {
                if (step1 == 0)
                {
                    Mix_PlayChannel(-1, sfxsound[1], 0);
                    framerate = 25;
                    step1 = 1;
                    cframe = -1;
                    xa = mPosX;
                    ya = mPosY;
                }
                if(step1 == 1)
                {
                    custom.x = xa - camx;
                    custom.y = ya - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (15);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe == -1) bullettexture[1].render(0, 0, &custom, &clip[12]);
                    else bullettexture[1].render(0, 0, &custom, &clip[cframe+12]);
                    if (cframe == 14) 
                    {
                        start = false;
                        v = false;
                        collide = false;
                    }
                }
            }
            else if (special == 2)
            {
                if (step1 == 0)
                {
                    Mix_PlayChannel(-1, sfxsound[2], 0);
                    framerate = 10;
                    step1 = 1;
                    cframe = -1;
                    xa = mPosX;
                    ya = mPosY;
                }
                if(step1 == 1)
                {
                    custom.x = xa - camx;
                    custom.y = ya - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (5);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe == -1) bullettexture[2].render(0, 0, &custom, &clip[cframe+15]);
                    else bullettexture[2].render(0, 0, &custom, &clip[cframe+14]);
                    if (cframe == 4) 
                    {
                        start = false;
                        v = false;
                        collide = false;
                    }
                }
            }
            else if (special == 3)
            {
                if (step1 == 0)
                {
                    framerate = 25;
                    step1 = 1;
                    cframe = -1;
                    xa = mPosX;
                    ya = mPosY;
                }
                if(step1 == 1)
                {
                    custom.x = xa - camx;
                    custom.y = ya - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        Mix_PlayChannel(-1, sfxsound[0], 0);
                        cframe = (cframe + 1) % (12);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe == -1) bullettexture[3].render(0, 0, &custom, &clip[9]);
                    else bullettexture[3].render(0, 0, &custom, &clip[cframe+9]);
                    if (cframe == 11) 
                    {
                        start = false;
                        v = false;
                        collide = false;
                    }
                }
            }
            else if (special == 4)
            {
                if (step1 == 0)
                {
                    Mix_PlayChannel(-1, sfxsound[4], 0);
                    framerate = 10;
                    step1 = 1;
                    cframe = -1;
                    xa = mPosX;
                    ya = mPosY;
                }
                if(step1 == 1)
                {
                    custom.x = xa - camx;
                    custom.y = ya - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (7);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe == -1) bullettexture[4].render(0, 0, &custom, &clip[9]);
                    else bullettexture[4].render(0, 0, &custom, &clip[cframe+9]);
                    if (cframe == 6) 
                    {
                        start = false;
                        v = false;
                        collide = false;
                    }
                }
            }
            else if (special == 5)
            {
                if (step1 == 0)
                {
                    Mix_PlayChannel(-1, sfxsound[5], 0);
                    framerate = 10;
                    step1 = 1;
                    cframe = -1;
                    xa = mPosX;
                    ya = mPosY;
                }
                if(step1 == 1)
                {
                    custom.x = xa - camx;
                    custom.y = ya - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (6);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe == -1) bullettexture[5].render(0, 0, &custom, &clip[13]);
                    else bullettexture[5].render(0, 0, &custom, &clip[cframe+13]);
                    if (cframe == 5) 
                    {
                        start = false;
                        v = false;
                        collide = false;
                    }
                    SDL_Rect customm;
                    customm.w = 48;
                    customm.h = 48;
                    SDL_Point center;
                    center.x = 48/2;
                    center.y = 48/2;

                    mPosX += mVelX*deltatime/1000;
                    mPosY += mVelY*deltatime/1000;
                    customm.x = round(mPosX - camx);
                    customm.y = round(mPosY - camy);

                    if(cframe==-1) bullettexture[5].render(0, 0, &customm, &clip[9], false, angle, &center);
                    else if (cframe <= 3) bullettexture[5].render(0, 0, &customm, &clip[cframe+9], false, angle, &center);
                }
            }
            else if (special == 6)
            {
                if (step1 == 0)
                {
                    Mix_PlayChannel(-1, sfxsound[6], 0);
                    framerate = 25;
                    step1 = 1;
                    cframe = -1;
                    xa = mPosX;
                    ya = mPosY;
                }
                if(step1 == 1)
                {
                    custom.x = xa - camx;
                    custom.y = ya - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (15);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe == -1) bullettexture[6].render(0, 0, &custom, &clip[10]);
                    else bullettexture[6].render(0, 0, &custom, &clip[cframe+10]);
                    if (cframe == 14) 
                    {
                        start = false;
                        v = false;
                        collide = false;
                    }
                }
            }
            else if (special == 7)
            {
                if (step1 == 0)
                {
                    framerate = 25;
                    step1 = 1;
                    cframe = -1;
                    xa = mPosX;
                    ya = mPosY;
                }
                if(step1 == 1)
                {
                    custom.x = xa - camx;
                    custom.y = ya - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        Mix_PlayChannel(-1, sfxsound[0], 0);
                        cframe = (cframe + 1) % (11);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe == -1) bullettexture[7].render(0, 0, &custom, &clip[10]);
                    else bullettexture[7].render(0, 0, &custom, &clip[cframe+10]);
                    if (cframe == 10) 
                    {
                        start = false;
                        v = false;
                        collide = false;
                    }
                }
            }
            else if (special == 8)
            {
                if (step1 == 0)
                {
                    Mix_PlayChannel(-1, sfxsound[6], 0);
                    framerate = 25;
                    step1 = 1;
                    cframe = -1;
                    xa = mPosX;
                    ya = mPosY;
                }
                if(step1 == 1)
                {
                    custom.x = xa - camx;
                    custom.y = ya - camy;
                    if (SDL_GetTicks() - lasttime > 1000/framerate) 
                    {
                        cframe = (cframe + 1) % (15);
                        lasttime = SDL_GetTicks();
                    }
                    if(cframe == -1) bullettexture[8].render(0, 0, &custom, &clip[9]);
                    else bullettexture[8].render(0, 0, &custom, &clip[cframe+9]);
                    if (cframe == 14) 
                    {
                        start = false;
                        v = false;
                        collide = false;
                    }
                }
            }
        }
    }
}

void Bullet::free()
{
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
    clip.clear();
}

