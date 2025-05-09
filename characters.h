#include <SDL.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Timer.h"
using namespace std;

struct datachar
{
    int health;
    int power;
    int mana;
    int speed; // pixel/s
};

enum Checkvar
{
    OUTSIDEC = 0,
    ONC = 1,
    UNDERC = 2,
    LEFTC = 3,
    RIGHTC = 4,
    BETWEENC = 5,
    UNKNOWNC = 6, //cho đủ tránh lỗi chứ không sài :V
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
        void move();
        Checkvar checkvar(int x1, int x2, int y1, int y2);
        void animated(int mx, int my);
        void getmapxy(Texture* mapp, string mappp);
        void cameraxy();
        SDL_Rect camxy();
        int mx_camx, my_camy;
        vector<vector<int>> blockmap;
    private:
        datachar datas;
        Texture* mTexture;
        double mVelX, mVelY;
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
        double deltatime;
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
    mWidth = texture->getWidth() / w;
    mHeight = texture->getHeight() / h;
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

void Characters::getmapxy(Texture* mapp, string mappp)
{
    blockmap.clear();
    mapx = mapp->getWidth();
    mapy = mapp->getHeight();
    string m = "assets/texture/map/" + mappp;
    ifstream MapF(m);
    int wp, hp; MapF >> wp >> hp;
    blockmap.resize(wp, vector<int>(hp));
    for (int i = 0; i < hp; i++)
    for (int j = 0; j < wp; j++)
    MapF >>  blockmap[j][i];
    MapF.close();
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

void Characters::move()
{
    deltatime = speedrender.getdeltatime();
    mPosX += mVelX*deltatime/1000;
    mPosY += mVelY*deltatime/1000;
    
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
    }

    int blockx = floor( mPosX / 48 );
    int blocky = floor( mPosY / 48 );
    for (int j = blocky-1; j < blocky+2; j++)
    if (j >= 0 && j < blockmap[0].size() )
    {
        for (int i = blockx-1; i < blockx+2; i++)
        if (i >= 0 && i < blockmap.size() )
        {
            if (blockmap[i][j] == 1)
            {
                if (checkvar(i*48, (i+1)*48, j*48, (j+1)*48 ) != 0) cout << checkvar(i*48, (i+1)*48, j*48, (j+1)*48 ) << endl;
                switch (checkvar(i*48, (i+1)*48, j*48, (j+1)*48 ))
                {
                    case UNDERC:
                    mPosY = (j+1)*48;
                    break;

                    case ONC:
                    mPosY = (j-1)*48;
                    break;

                    case RIGHTC:
                    mPosX = (i+1)*48;
                    break;

                    case LEFTC:
                    mPosX = (i-1)*48;
                    break;
                }
            }
        }
    }
}  

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
            if (distance > 150 * distance / ( 3000 * (1000/deltatime) - 9 * distance ) + 1) 
            //if này để khi distance đến 1 giới hạn nhỏ nhất định thì tọa độ gắn trung tâm luôn tránh tình trạng giật
            //công thức tính để tránh trường hợp khi còn x distance thì bước nhảy trên 1 vòng lặp lớn hơn 2 lần distance giới hạn dẫn đến tọa độ liên tục âm dương mà không tiến tới khoảng cách không đạt tới giới hạn được
            //có thể có cách dễ hơn nhưng phòng trường hợp fps và tốc độ thay đổi dẫn tới sai số thì đây là 1 giải pháp, và thật sự thì nó có ích
            // + 1 để tránh vì double đổi sang int có thể sai số nên +1 để phòng trường hợp đó 
            {
                if (mVelX == 0 && mVelY == 0) 
                if (distance <= datas.speed * deltatime / 1000)
                {
                    camx = mPosX + mWidth / 2;
                    camy = mPosY + mHeight / 2;
                }
                camvx = (distance * (datas.speed * 9 / 10) / 150 + datas.speed / 10) * cos(atan2(( ( mPosY + mHeight / 2 ) - camy ) , ( ( mPosX + mWidth / 2 ) - camx )));
                camvy = (distance * (datas.speed * 9 / 10) / 150 + datas.speed / 10) * sin(atan2(( ( mPosY + mHeight / 2 ) - camy ) , ( ( mPosX + mWidth / 2 ) - camx )));
            }
            else
            {
                camx = mPosX + mWidth / 2;
                camy = mPosY + mHeight / 2;
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
        }
        else
        {
            camx = mPosX + mWidth / 2;
            camy = mPosY + mHeight / 2;
        }

        camera.x = round(camx - SCREEN_WIDTH / 2);
        camera.y = round(camy - SCREEN_HEIGHT / 2);

        if( camera.x < 0 ) camera.x = 0; 
        if( camera.y < 0 ) camera.y = 0;
        if( camera.x > mapx - camera.w ) camera.x = mapx - camera.w;
        if( camera.y > mapy - camera.h ) camera.y = mapy - camera.h;

        mx_camx = round(mPosX - camera.x);
        my_camy = round(mPosY - camera.y);
    }
}

SDL_Rect Characters::camxy()
{
    return camera;
}

Checkvar Characters::checkvar(int x1, int x2, int y1, int y2)
{
    int ix = x2 - mPosX;
    int iy = y2 - mPosY;

    if (mPosX + mWidth <= x1 || mPosX >= x2 || mPosY + mHeight <= y1 || mPosY >= y2) return OUTSIDEC;
    else if (mPosX == x1 && mPosY == y1) return BETWEENC;
    else if((mPosX + mWidth >= (x1+x2)/2 && mPosX + mWidth <= x2 && mPosY >= y1 && mPosY <= (y1+y2)/2) ||
            (mPosX >= x1 && mPosX <= (x1+x2)/2 && mPosY >= y1 && mPosY <= (y1+y2)/2) ||
            (mPosX >= x1 && mPosX <= (x1+x2)/2 && mPosY + mHeight >= (y1+y2)/2 && mPosY + mHeight <= y2) ||
            (mPosX + mWidth >= (x1+x2)/2 && mPosX + mWidth <= x2 && mPosY + mHeight >= (y1+y2)/2 && mPosY + mHeight <= y2)) return BETWEENC;
    else 
    {
        if (mPosY > (y1+y2)/2)
        {
            if (ix >= mWidth * 3 / 2)
            {
                if (mWidth*2 - ix > iy) return UNDERC;
                else return LEFTC;
            }
            else
            {
                if (ix > iy) return UNDERC;
                else return RIGHTC;
            }
        }
        else if (mPosY + mHeight < (y1+y2)/2)
        {
            if (ix >= mWidth * 3 / 2)
            {
                if (mWidth*2 - ix > mHeight*2 - iy) return ONC;
                else return LEFTC;
            }
            else
            {
                if (ix > mHeight*2 - iy) return ONC;
                else return RIGHTC;
            }
        }
        else
        {
            if (iy >= mHeight/2 && iy <= mHeight * 3 / 2)
            {
                if (ix < mWidth/2) return RIGHTC;
                else return LEFTC;
            }
        }
    }
    return UNKNOWNC;
}
