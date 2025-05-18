#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "Texture.h"
#include "Window.h"
#include "Timer.h"
#include "Button.h"
#include "characters.h"
#include "rangedM.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>
#include <SDL_mixer.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;

enum Scenes
{
    START_MENU = 0,
    MENU = 1,
    SETTING = 2,
    SETTING_KEY = 3,
    NEW_GAME = 4,
    LOAD_GAME = 5,
    MAP_T1 = 6,
    MAP_T2 = 7,
    MAP_A1 = 8,
    MAP_A2 = 9,
    MAP_A3 = 10,
    MAP_A4 = 11,
    MAP_GARDEN = 12,
    CHALLENGE_1 = 13,
    CHALLENGE_2 = 14,
    CHALLENGE_3 = 15,
    CHALLENGE_4 = 16,
    GAME_OVER = 17,
};

class Scene
{
    public:
        Scene();
        ~Scene();
        void free();
        void init();
        void handleEvent(SDL_Event& e);
        void logicScene();
        void renderScene();
        bool fps_max;
        bool vsync;
        bool fadein(Texture* tex); //hiện dần
        bool fadeout(Texture* tex); //mờ dần
        void shoot(int special, int x, int y, int v, double angle, vector<vector<int>> bm, bool ally, bool anglelockk, bool dis, int dam);
        Scenes scene;
    private:
        int step, stepf;
        Uint8 alpha;
        vector<Button> buttons;
        int music_vollume;
        int SFX_vollume;
        bool fps_show;
        bool vn;
        int xsp, ysp;
        Direction d;
        bool pixelmotionn;
        vector<Bullet> bullets1;
        vector<Bullet> bullets2;
        Bullet b3;
        bool cha1, cha2, cha3, cha4;
};

int SCREEN_WIDTH = 720;
int SCREEN_HEIGHT = 480;

void loadMedia();
void close();

extern SDL_Renderer* gRenderer;
TTF_Font* gTimes = nullptr;
TTF_Font* gArial = nullptr;
Texture text;
Texture logo;
Texture menu;
Texture menu2;
Texture setting;
Texture setting2;
Texture background;
Texture black;
Texture white;
Texture transparent;
Texture transparent2;
Texture tick;
Texture newgame;
Texture hero2;
Texture mapt1;
Texture mapt2;
Texture mapa1;
Texture mapa2;
Texture mapa3;
Texture mapa4;
Texture mapg1;
Texture mapg2;
Texture challenge1;
Texture challenge3;
Texture dark;
Texture dimension2;
Texture gate2;
Texture bullettexture[9];
Texture crystalr;
Texture crystalg;
Texture crystalb;
Texture crystaly;
Texture skvn;
Texture ske;
Texture gameovervn;
Texture gameovereng;
Characters hero;
Characters dimension;
Characters gate;
SDL_Rect menuclips[ 4 ];
Timer time1;
Timer bullett;
Timer fpscc;
Window gWindow;
Mix_Chunk* sfxsound[7];

int msv;
int sfx;
bool vs;
bool fpss;
bool fpm;
bool vnm;
int frame = 0;
int frame2 = 0;
string fpscustom = "100";
string ff = "100";
bool a = false;
double newgamey = 1;
bool smooth_camera = false;

extern bool quit;
extern bool vsync2;

//special; tọa độ đạn (x,y); vector; angle; map; đạn đồng minh?; khóa góc; xóa đạn luôn khi vào tường; sát thương
void Scene::shoot(int special, int x, int y, int v, double angle, vector<vector<int>> bm, bool allyy, bool anglelockk, bool dis, int dam)                
{
    Bullet newBullet;
    newBullet.addbullet(special, x, y, v, angle, bm, allyy, anglelockk, dis, dam);
    if(allyy) bullets1.push_back(newBullet);
    else bullets2.push_back(newBullet);
}

Scene::Scene()
{
    scene = START_MENU;
    step = 0;
    stepf = 0;
    alpha = 0;
    music_vollume = 64;
    SFX_vollume = 32;
    vsync = true;
    fps_show = false;
    fps_max = false;
    vn = true; 
    cha1 = false;
    cha2 = false;
    cha3 = false;
    cha4 = false;

    xsp = 41; //sảnh 1
    ysp = 43;

    //xsp = 4; //map1
    //ysp = 28;

    //xsp = 64; //maze-cha1
    //ysp = 160;

    //xsp = 58; //maze-cha1.2
    //ysp = 10;

    //xsp = 19; //garden
    //ysp = 35;

    //xsp = 5; //cha3
    //ysp = 83;

    //xsp = 30; //cha3.2
    //ysp = 45;

    d = FRONT;
    pixelmotionn = true;
    //srand(time(0));
    bullett.setstarttime();
}

Scene::~Scene()
{
    free();
}

void Scene::handleEvent(SDL_Event& e)
{
    gWindow.handleEvent(e);

    if (scene == MENU)
    {
        if( e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED && step != 0 )
        {
            buttons[0].mRenderRect.w = SCREEN_WIDTH / 4;
            buttons[0].mRenderRect.h = buttons[0].mRenderRect.w / 3;
            buttons[0].mRenderRect.x = SCREEN_WIDTH * 0.235;
            buttons[0].mRenderRect.y = SCREEN_HEIGHT / 6 * 4;

            buttons[1].mRenderRect.w = SCREEN_WIDTH / 4;
            buttons[1].mRenderRect.h = buttons[1].mRenderRect.w / 3;
            buttons[1].mRenderRect.x = SCREEN_WIDTH * 0.235;
            buttons[1].mRenderRect.y = SCREEN_HEIGHT / 6 * 4 + buttons[1].mRenderRect.h * 1.125;

            buttons[2].mRenderRect.w = SCREEN_WIDTH / 4;
            buttons[2].mRenderRect.h = buttons[2].mRenderRect.w / 3;
            buttons[2].mRenderRect.x = SCREEN_WIDTH * 0.765 - buttons[2].mRenderRect.w;
            buttons[2].mRenderRect.y = SCREEN_HEIGHT / 6 * 4;

            buttons[3].mRenderRect.w = SCREEN_WIDTH / 4;
            buttons[3].mRenderRect.h = buttons[3].mRenderRect.w / 3;
            buttons[3].mRenderRect.x = SCREEN_WIDTH * 0.765 - buttons[3].mRenderRect.w;
            buttons[3].mRenderRect.y = SCREEN_HEIGHT / 6 * 4 + buttons[3].mRenderRect.h * 1.125;

        }
        if (step != 0)
        {
            if (buttons[0].handleEvent(&e) == MOUSE_OUT) buttons[0].mClip.x = 0;
            else
            {
                buttons[0].mClip.x = buttons[0].mClip.w;
                if (buttons[0].handleEvent(&e) == MOUSE_DOWN)
                {
                    step = 0;
                    scene = NEW_GAME;
                }
            }
            if (buttons[1].handleEvent(&e) == MOUSE_OUT) buttons[1].mClip.x = 0;
            else
            {
                buttons[1].mClip.x = buttons[1].mClip.w;
                if (buttons[1].handleEvent(&e) == MOUSE_DOWN)
                {
                    step = 0;
                    scene = SETTING;
                }
            }
            if (buttons[2].handleEvent(&e) == MOUSE_OUT) buttons[2].mClip.x = 0;
            else
            {
                buttons[2].mClip.x = buttons[2].mClip.w;
                if (buttons[2].handleEvent(&e) == MOUSE_DOWN)
                {
                    step = 0;
                    int sceneValue;
                    int di;
                    ifstream file("save/save.txt");
                    if (file >> sceneValue) 
                    {
                        file >> hero.mPosX >> hero.mPosY >> di >> cha1 >> cha2 >> cha3 >> cha4 >> hero.datas.health;
                        xsp = floor((hero.mPosX+48)/48);
                        ysp = floor((hero.mPosY+48)/48);
                        d = static_cast<Direction>(di);
                        scene = static_cast<Scenes>(sceneValue);
                    }
                    file.close();
                }
            }
            if (buttons[3].handleEvent(&e) == MOUSE_OUT) buttons[3].mClip.x = 0;
            else
            {
                buttons[3].mClip.x = buttons[3].mClip.w;
                if (buttons[3].handleEvent(&e) == MOUSE_DOWN) 
                {
                    step = 0;
                    quit = true;
                }
            }
        }
    }

    else if ( scene == SETTING && step != 0 )
    {
        if( e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED ) step = 0;
        if (buttons[0].handleEvent(&e) == MOUSE_DOWN ) 
        {
            SDL_StopTextInput(); 
            step = 0;
            scene = MENU;
        }
        else if(buttons[2].handleEvent(&e) == MOUSE_DOWN ) 
        {
            msv -= 8;
            if(msv<0) msv = 0;
        } 
        else if(buttons[3].handleEvent(&e) == MOUSE_DOWN )
        {
            msv += 8;
            if(msv>128) msv = 128;
        } 
        else if(buttons[4].handleEvent(&e) == MOUSE_DOWN )
        {
            sfx -= 8;
            if(sfx < 0) sfx=0;
        }
        else if(buttons[5].handleEvent(&e) == MOUSE_DOWN )
        {
            sfx += 8;
            if(sfx > 128) sfx=128;
        }
        else if(buttons[6].handleEvent(&e) == MOUSE_DOWN ) 
        {
            vs = !vs;
            if(fpm) fpm = false;
        }
        else if(buttons[7].handleEvent(&e) == MOUSE_DOWN ) fpss = !fpss;
        else if(buttons[8].handleEvent(&e) == MOUSE_DOWN ) 
        {
            fpm = !fpm;
            if(vs) vs = false;
        }
        else if(buttons[9].handleEvent(&e) == MOUSE_DOWN )
        {
            step = 0;
            scene = SETTING_KEY;
        } 
        else if(buttons[11].handleEvent(&e) == MOUSE_DOWN ) vnm = !vnm;
        else if(buttons[12].handleEvent(&e) == MOUSE_DOWN ) vnm = !vnm;
        else if(buttons[1].handleEvent(&e) == MOUSE_DOWN )
        {
            SDL_StopTextInput(); 
            music_vollume = msv;
            SFX_vollume = sfx;
            vsync2 = vs;
            fps_show = fpss;
            fps_max = fpm;
            vn = vnm;
            Mix_VolumeMusic(music_vollume);
            Mix_Volume(-1, SFX_vollume/2);
            
            if (vn)
            {
                newgame.free();
                newgame.loadFromFile( "assets/texture/img/new game vn.png" );
            }
            else
            {
                newgame.free();
                newgame.loadFromFile( "assets/texture/img/new game eng.png" );
            }

            if (stoi(ff)<25)
            {
                ff = "25";
                fpscustom = ff;
            }
            else fpscustom = ff;

            if (vsync != vs)
            {
                vsync = vs;
                gWindow.free();
                SDL_DestroyRenderer( gRenderer );
                gRenderer = nullptr;
                init();
            }
            step = 0;
            scene = MENU;
            if (fps_show)
            {
                frame = 0;
                fpscc.setstarttime();
                SDL_Color textColor = { 255, 255, 255, 255 };
                text.loadFromRenderedText( "[?] FPS ~ wait", textColor, gArial );
            }
        }
        else if(buttons[13].handleEvent(&e) == MOUSE_DOWN ) 
        {
            if (a)
            {
                a = false;
                SDL_StopTextInput(); 
            }
            else
            {
                SDL_StartTextInput();
                a = true;
            }
        }
        else if (a)
        {
            if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE && ff.length() > 0) ff.pop_back();
            else if(e.type == SDL_TEXTINPUT) if (e.text.text[0] >= '0' && e.text.text[0] <= '9' && ff.length() < 5) ff += e.text.text;
        }
    }

    else if ( scene == SETTING_KEY && step != 0 )
    {
        if(buttons[0].handleEvent(&e) == MOUSE_DOWN ) 
        {
            step = 0;
            scene = MENU;
        }
    }

    else if (scene == NEW_GAME && step != 0)
    {
        if (e.type == SDL_KEYDOWN)
        {
            step = 0;
            scene = MAP_T1;
            newgamey = 1;
        }
    }

    else if ( scene == MAP_T1 && step != 0 )
    {
        if (pixelmotionn)
        {
            hero.motionpixel(&e);
            hero.mousepixel(&e);
        }
        else hero.motion(&e);

        if (hero.blockevent == 801 && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_e)
        {
            hero.stop();
            scene = MAP_T2;
            step = 0;
            xsp = 41;
            ysp = 26;
            d = FRONT;
        }
    }

    else if ( scene == MAP_T2 && step != 0 )
    {
        if (pixelmotionn)
        {
            hero.motionpixel(&e);
            hero.mousepixel(&e);
        }
        else hero.motion(&e);
    }

    else if ( scene == MAP_A1 && step != 0 )
    {
        if (pixelmotionn)
        {
            hero.motionpixel(&e);
            hero.mousepixel(&e);
        }
        else hero.motion(&e);
    }

    else if ( scene == MAP_A2 && step != 0 )
    {
        if (pixelmotionn)
        {
            hero.motionpixel(&e);
            hero.mousepixel(&e);
        }
        else hero.motion(&e);
    }

    else if ( scene == MAP_A3 && step != 0 )
    {
        if (pixelmotionn)
        {
            hero.motionpixel(&e);
            hero.mousepixel(&e);
        }
        else hero.motion(&e);
    }

    else if ( scene == MAP_A4 && step != 0 )
    {
        if (pixelmotionn)
        {
            hero.motionpixel(&e);
            hero.mousepixel(&e);
        }
        else hero.motion(&e);
    }

    else if ( scene == MAP_GARDEN && step != 0 )
    {
        if (pixelmotionn)
        {
            hero.motionpixel(&e);
            hero.mousepixel(&e);
        }
        else hero.motion(&e);
    }

    else if ( scene == CHALLENGE_1 && step != 0 )
    {
        if (pixelmotionn)
        {
            hero.motionpixel(&e);
            hero.mousepixel(&e);
        }
        else hero.motion(&e);

        if (hero.blockevent == 801 && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_e)
        {
            hero.stop();
            cha1 = true;
            scene = MAP_GARDEN;
            step = 0;
            xsp = 8;
            ysp = 12;
            d = FRONT;
        }
    }   

    else if ( scene == CHALLENGE_3 && step != 0 )
    {
        if (pixelmotionn)
        {
            hero.motionpixel(&e);
            hero.mousepixel(&e);
        }
        else hero.motion(&e);

        if (hero.blockevent == 800 && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_e)
        {
            hero.stop();
            cha3 = true;
            scene = MAP_GARDEN;
            step = 0;
            xsp = 30;
            ysp = 34;
            d = FRONT;
        }
    }   

    if ((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q && e.key.repeat == 0) || (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && !pixelmotionn) && step != 0) 
    {
        shoot(rand() % 9, hero.mPosX, hero.mPosY, 48*6, 0, hero.blockmap, true, false, false, hero.datas.power);
    }

    else if ( scene == GAME_OVER )
    {
       if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
       {
            step = 0;
            int sceneValue;
            int di;
            ifstream file("save/save.txt");
            if (file >> sceneValue) 
            {
                file >> hero.mPosX >> hero.mPosY >> di >> cha1 >> cha2 >> cha3 >> cha4 >> hero.datas.health;
                xsp = floor((hero.mPosX+48)/48);
                ysp = floor((hero.mPosY+48)/48);
                d = static_cast<Direction>(di);
                scene = static_cast<Scenes>(sceneValue);
            }
            file.close();
       } 
    }

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_m && step != 0) 
    {
        if(pixelmotionn)
        {
            hero.mVelX = 0;
            hero.mVelY = 0;
            pixelmotionn = !pixelmotionn;
            smooth_camera = true;
        }
        else
        {
            hero.mVelX = 0;
            hero.mVelY = 0;
            pixelmotionn = !pixelmotionn;
            smooth_camera = false;
            hero.mPosX = floor(hero.mPosX/48)*48;
            hero.mPosY = floor(hero.mPosY/48)*48;
        }

    }

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
    {
        hero.mVelX = 0;
        hero.mVelY = 0;
        ofstream file("save/save.txt");
        file << scene << " " << floor(hero.mPosX/48)*48 << " " << floor(hero.mPosY/48)*48 << " " << hero.direction << " " << cha1 << " " << cha2<< " " << cha3<< " " << cha4 << " " << hero.datas.health;
        file.close();
        step = 0;
        scene = MENU;
    }
}   

void Scene::logicScene()
{
    vsync2 = vsync;
    if ( scene == MAP_T1 && step != 0 )
    {
        if (pixelmotionn) hero.movepixel();
        else hero.move();

        if (hero.blockevent == 901) 
        {
            hero.stop();
            scene = MAP_T2;
            step = 0;
            xsp = 3;
            ysp = 4;
            d = BEHIND;
        }
        else if (hero.blockevent == 902) 
        {
            hero.stop();
            scene = MAP_T2;
            step = 0;
            xsp = 78;
            ysp = 4;
            d = BEHIND;
        }
        else if (hero.blockevent == 903)
        {
            hero.stop();
            scene = MAP_T2;
            step = 0;
            xsp = 29;
            ysp = 33;
            d = LEFT;
        }
        else if (hero.blockevent == 904)
        {
            hero.stop();
            scene = MAP_T2;
            step = 0;
            xsp = 54;
            ysp = 33;
            d = RIGHT;
        }
    }

    else if ( scene == MAP_T2 && step != 0 )
    {
        if (pixelmotionn) hero.movepixel();
        else hero.move();

        if (hero.blockevent == 901) 
        {
            hero.stop();
            scene = MAP_T1;
            step = 0;
            xsp = 5;
            ysp = 4;
            d = BEHIND;
        }
        else if (hero.blockevent == 902) 
        {
            hero.stop();
            scene = MAP_T1;
            step = 0;
            xsp = 80;
            ysp = 4;
            d = BEHIND;
        }
        else if (hero.blockevent == 903)
        {
            hero.stop();
            scene = MAP_T1;
            step = 0;
            xsp = 41;
            ysp = 31;
            d = BEHIND;
        }
        else if (hero.blockevent == 904)
        {
            hero.stop();
            scene = MAP_A1;
            step = 0;
            xsp = 4;
            ysp = 28;
            d = FRONT;
        }
        else if (hero.blockevent == 905)
        {
            hero.stop();
            scene = MAP_T1;
            step = 0;
            xsp = 35;
            ysp = 33;
            d = RIGHT;
        }
        else if (hero.blockevent == 906)
        {
            hero.stop();
            scene = MAP_T1;
            step = 0;
            xsp = 48;
            ysp = 33;
            d = LEFT;
        }
    }

    else if ( scene == MAP_A1 && step != 0 )
    {
        if (pixelmotionn) hero.movepixel();
        else hero.move();

        if (hero.blockevent == 901) 
        {
            hero.stop();
            scene = MAP_T2;
            step = 0;
            xsp = 41;
            ysp = 14;
            d = BEHIND;
        }
        else if (hero.blockevent == 902) 
        {
            hero.stop();
            scene = MAP_GARDEN;
            step = 0;
            xsp = 19;
            ysp = 38;
            d = FRONT;
        }
    }

    else if ( scene == MAP_A2 && step != 0 )
    {
        if (pixelmotionn) hero.movepixel();
        else hero.move();

        if (hero.blockevent == 901) 
        {
            hero.stop();
            scene = MAP_A2;
            step = 0;
            xsp = 21;
            ysp = 38;
            d = FRONT;
        }
        else if (hero.blockevent == 902) 
        {
            hero.stop();
            scene = MAP_A2;
            step = 0;
            xsp = 38;
            ysp = 21;
            d = LEFT;
        }
        else if (hero.blockevent == 903) 
        {
            hero.stop();
            scene = MAP_GARDEN;
            step = 0;
            xsp = 4;
            ysp = 23;
            d = RIGHT;
        }
        else if (hero.blockevent == 904) 
        {
            hero.stop();
            scene = MAP_A2;
            step = 0;
            xsp = 4;
            ysp = 21;
            d = RIGHT;
        }
    }

    else if ( scene == MAP_A3 && step != 0 )
    {
        if (pixelmotionn) hero.movepixel();
        else hero.move();

        if (hero.blockevent == 901) 
        {
            hero.stop();
            scene = MAP_A3;
            step = 0;
            xsp = 44;
            ysp = 4;
            d = BEHIND;
        }
        else if (hero.blockevent == 902) 
        {
            hero.stop();
            scene = MAP_GARDEN;
            step = 0;
            xsp = 19;
            ysp = 5;
            d = BEHIND;
        }
        else if (hero.blockevent == 903) 
        {
            hero.stop();
            scene = MAP_A1;
            step = 0;
            xsp = 4;
            ysp = 28;
            d = FRONT;
        }
    }

    else if ( scene == MAP_A4 && step != 0 )
    {
        if (pixelmotionn) hero.movepixel();
        else hero.move();

        if (hero.blockevent == 901) 
        {
            hero.stop();
            scene = MAP_A1;
            step = 0;
            xsp = 4;
            ysp = 28;
            d = FRONT;
        }
        else if (hero.blockevent == 902) 
        {
            hero.stop();
            scene = MAP_GARDEN;
            step = 0;
            xsp = 4;
            ysp = 23;
            d = RIGHT;
        }
        else if (hero.blockevent == 903) 
        {
            hero.stop();
            scene = MAP_A4;
            step = 0;
            xsp = 4;
            ysp = 4;
            d = BEHIND;
        }
        else if (hero.blockevent == 904) 
        {
            hero.stop();
            scene = MAP_GARDEN;
            step = 0;
            xsp = 34;
            ysp = 23;
            d = LEFT;
        }
        else if (hero.blockevent == 905) 
        {
            hero.stop();
            scene = MAP_A4;
            step = 0;
            xsp = 44;
            ysp = 38;
            d = FRONT;
        }
        else if (hero.blockevent == 906) 
        {
            hero.stop();
            scene = MAP_A4;
            step = 0;
            xsp = 24;
            ysp = 4;
            d = BEHIND;
        }
    }

    else if ( scene == MAP_GARDEN && step != 0 )
    {
        if (pixelmotionn) hero.movepixel();
        else hero.move();

        if (hero.blockevent == 901) 
        {
            hero.stop();
            scene = MAP_A2;
            step = 0;
            xsp = 38;
            ysp = 21;
            d = LEFT;
        }
        else if (hero.blockevent == 902)
        {
            hero.stop();
            scene = MAP_A3;
            step = 0;
            xsp = 24;
            ysp = 4;
            d = BEHIND;
        }
        else if (hero.blockevent == 903)
        {
            hero.stop();
            scene = MAP_A4;
            step = 0;
            xsp = 4;
            ysp = 38;
            d = FRONT;
        }
        else if (hero.blockevent == 904)
        {
            hero.stop();
            scene = MAP_A1;
            step = 0;
            xsp = 4;
            ysp = 4;
            d = BEHIND;
        }
        else if (hero.blockevent == 911)
        {
            hero.stop();
            scene = MAP_A2;
            step = 0;
            xsp = 21;
            ysp = 38;
            d = FRONT;
        }
        else if (hero.blockevent == 912)
        {
            hero.stop();
            scene = MAP_A3;
            step = 0;
            xsp = 24;
            ysp = 4;
            d = BEHIND;
        }
        else if (hero.blockevent == 921)
        {
            hero.stop();
            scene = MAP_A3;
            step = 0;
            xsp = 24;
            ysp = 4;
            d = BEHIND;
        }
        else if (hero.blockevent == 922)
        {
            hero.stop();
            scene = MAP_A4;
            step = 0;
            xsp = 24;
            ysp = 38;
            d = FRONT;
        }
        else if (hero.blockevent == 931)
        {
            hero.stop();
            scene = MAP_GARDEN;
            step = 0;
            xsp = 19;
            ysp = 38;
            d = FRONT;
        }
        else if (hero.blockevent == 932)
        {
            hero.stop();
            scene = MAP_A2;
            step = 0;
            xsp = 21;
            ysp = 4;
            d = BEHIND;
        }
        else if (hero.blockevent == 941)
        {
            hero.stop();
            scene = MAP_A3;
            step = 0;
            xsp = 24;
            ysp = 4;
            d = BEHIND;
        }
        else if (hero.blockevent == 942)
        {
            hero.stop();
            scene = MAP_GARDEN;
            step = 0;
            xsp = 19;
            ysp = 35;
            d = FRONT;
        }
        else if (hero.blockevent == 951 && !cha1)
        {
            hero.stop();
            scene = CHALLENGE_1;
            step = 0;
            xsp = 64;
            ysp = 160;
            d = FRONT;
        }
        else if (hero.blockevent == 952 && !cha2)
        {
            hero.stop();
            scene = CHALLENGE_1;
            step = 0;
            xsp = 64;
            ysp = 160;
            d = FRONT;
        }
        else if (hero.blockevent == 953 && !cha3)
        {
            hero.stop();
            scene = CHALLENGE_3;
            step = 0;
            xsp = 5;
            ysp = 83;
            d = FRONT;
        }
        else if (hero.blockevent == 954 && !cha4)
        {
            hero.stop();
            scene = CHALLENGE_1;
            step = 0;
            xsp = 64;
            ysp = 160;
            d = FRONT;
        }
    }

    else if ( scene == CHALLENGE_1 && step != 0 )
    {
        if (pixelmotionn) hero.movepixel();
        else hero.move();

        if (hero.blockevent == 901) 
        {
            hero.stop();
            scene = MAP_GARDEN;
            step = 0;
            xsp = 19;
            ysp = 35;
            d = FRONT;
        }
    }

    else if ( scene == CHALLENGE_3 && step != 0 )
    {
        if (pixelmotionn) hero.movepixel();
        else hero.move();

        if (hero.blockevent == 901) 
        {
            hero.stop();
            scene = MAP_GARDEN;
            step = 0;
            xsp = 19;
            ysp = 35;
            d = FRONT;
        }
        if (hero.blockevent == 801) 
        {
            if(pixelmotionn)
            {
                hero.mVelX = 0;
                hero.mVelY = 0;
                hero.b1 = false;
                hero.m1 = false;
                hero.m2 = false;
                hero.motionp = false;
            } 
            hero.mPosY -= 3;
        }
        if (hero.blockevent == 802) 
        {
            if(pixelmotionn)
            {
                hero.mVelX = 0;
                hero.mVelY = 0;
                hero.b1 = false;
                hero.m1 = false;
                hero.m2 = false;
                hero.motionp = false;
            } 
            hero.mPosX += 3;
        }
        if (hero.blockevent == 803) 
        {
            if(pixelmotionn)
            {
                hero.mVelX = 0;
                hero.mVelY = 0;
                hero.b1 = false;
                hero.m1 = false;
                hero.m2 = false;
                hero.motionp = false;
            } 
            hero.mPosY += 3;
        }
        if (hero.blockevent == 804) 
        {
            if(pixelmotionn)
            {
                hero.mVelX = 0;
                hero.mVelY = 0;
                hero.b1 = false;
                hero.m1 = false;
                hero.m2 = false;
                hero.motionp = false;
            } 
            hero.mPosX -= 3;
        }
    
        if (bullett.gettime() > 100)
        {
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 2 *48-48, 82 *48-48, 48*3, 135, hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 8 *48-48, 82 *48-48, 48*3, ((rand() % 2 == 0) ? 135 : 45), hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 14 *48-48, 82 *48-48, 48*3, ((rand() % 2 == 0) ? 135 : 45), hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 20 *48-48, 82 *48-48, 48*3, ((rand() % 2 == 0) ? 135 : 45), hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 26 *48-48, 82 *48-48, 48*3, ((rand() % 2 == 0) ? 135 : 45), hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 32 *48-48, 82 *48-48, 48*3, ((rand() % 2 == 0) ? 135 : 45), hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 38 *48-48, 82 *48-48, 48*3, ((rand() % 2 == 0) ? 135 : 45), hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 44 *48-48, 82 *48-48, 48*3, ((rand() % 2 == 0) ? 135 : 45), hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 50 *48-48, 82 *48-48, 48*3, ((rand() % 2 == 0) ? 135 : 45), hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 56 *48-48, 82 *48-48, 48*3, ((rand() % 2 == 0) ? 135 : 45), hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 62 *48-48, 82 *48-48, 48*3, ((rand() % 2 == 0) ? 135 : 45), hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 68 *48-48, 82 *48-48, 48*3, 45, hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 74 *48-48, 82 *48-48, 48*3, 45, hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 80 *48-48, 82 *48-48, 48*3, 45, hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 2 *48-48, 65 *48-48, 48*3, -90, hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 8 *48-48, 65 *48-48, 48*3, -90, hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 14 *48-48, 65 *48-48, 48*3, -90, hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 20 *48-48, 65 *48-48, 48*3, -90, hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 26 *48-48, 65 *48-48, 48*3, -90, hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 32 *48-48, 65 *48-48, 48*3, -90, hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 38 *48-48, 65 *48-48, 48*3, -90, hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 44 *48-48, 65 *48-48, 48*3, -90, hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 50 *48-48, 65 *48-48, 48*3, -90, hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 56 *48-48, 65 *48-48, 48*3, -90, hero.blockmap, false, true, true, 25);
            if(rand() % 5 == 0) shoot(rand() % 2 + 7, 62 *48-48, 65 *48-48, 48*3, -90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 63 *48-48, 60 *48-48, 48*5, 180, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 63 *48-48, 54 *48-48, 48*5, 180, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 63 *48-48, 48 *48-48, 48*5, 180, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 63 *48-48, 42 *48-48, 48*5, 180, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 63 *48-48, 36 *48-48, 48*5, 180, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 63 *48-48, 30 *48-48, 48*5, 180, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 63 *48-48, 24 *48-48, 48*5, 180, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 80 *48-48, 63 *48-48, 48*5, 0, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 80 *48-48, 57 *48-48, 48*5, 0, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 80 *48-48, 51 *48-48, 48*5, 0, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 80 *48-48, 45 *48-48, 48*5, 0, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 80 *48-48, 39 *48-48, 48*5, 0, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 80 *48-48, 33 *48-48, 48*5, 0, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 80 *48-48, 27 *48-48, 48*5, 0, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 80 *48-48, 22 *48-48, 48*4, 0, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 80 *48-48, 20 *48-48, 48*4, 0, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 80 *48-48, 18 *48-48, 48*4, 0, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 80 *48-48, 16 *48-48, 48*4, 0, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 80 *48-48, 14 *48-48, 48*4, 0, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 80 *48-48, 12 *48-48, 48*4, 0, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 80 *48-48, 10 *48-48, 48*4, 0, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 80 *48-48, 8 *48-48, 48*4, 0, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 80 *48-48, 6 *48-48, 48*4, 0, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 80 *48-48, 4 *48-48, 48*4, 0, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 61 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 59 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 57 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 55 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 53 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 51 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 49 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 47 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 45 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 43 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 41 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 39 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 37 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 35 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 33 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 31 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 29 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 27 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 25 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 10 == 0) shoot(rand() % 2 + 7, 23 *48-48, 22 *48-48, 48*4, (rand() % 10) * 10 + 45, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 2 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 3 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 4 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 5 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 6 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 7 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 8 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 9 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 10 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 11 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 12 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 13 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 14 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 15 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 16 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 17 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 18 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 19 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 20 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            if(rand() % 15 == 0) shoot(rand() % 2 + 7, 21 *48-48, 52 *48-48, 48*4, 90, hero.blockmap, false, true, true, 25);
            bullett.setstarttime();
        }    
    }

    if (step != 0)
    {
        for (auto& bullet : bullets2) 
        if( sqrt( pow( hero.mPosX - bullet.mPosX , 2 ) + pow( hero.mPosY - bullet.mPosY, 2 ) ) < 24) 
        {
            bullet.collide = true;
            if (!bullet.getdamage)
            {
                hero.datas.health -= bullet.damage;
                bullet.getdamage = true;
                cout << hero.datas.health << endl;
            }
        }

        for (auto& bullet1 : bullets1)
        for (auto& bullet2 : bullets2)
        {
            if (&bullet1 == &bullet2 || bullet1.ally == bullet2.ally) continue; 
            if( sqrt( pow( bullet1.mPosX - bullet2.mPosX , 2 ) + pow( bullet1.mPosY - bullet2.mPosY, 2 ) ) < 24) 
            {
            bullet1.collide = true;
            bullet2.collide = true;
            }
        }
    }
    if(step == 0 && !bullets1.empty()) bullets1.clear();
    if(step == 0 && !bullets2.empty()) bullets2.clear();
    srand(chrono::high_resolution_clock::now().time_since_epoch().count());
    if(hero.datas.health < 0)
    {
        hero.stop();
        scene = GAME_OVER;
        step = 0;
    }
}

void Scene::renderScene()
{
    if( scene == START_MENU )
    {
        if (step == 0)
        {
            alpha = 0;
            logo.setAlpha( 0 );
            time1.setstarttime();
            step = 1;
        }
        else if( step == 1 )
        {
            if ( time1.gettime() >= 30 )
            {
                logo.setAlpha( alpha );
                alpha += 5;
                time1.setstarttime();
            }
            else if (alpha >= 255 )
            {
                step = 2;
                alpha = 255;
            }
            logo.render( 0, 0, nullptr, nullptr, true );
        }
        else if( step == 2 )
        {
            if ( time1.gettime() >= 3000 )
            {
                step = 3;
                time1.setstarttime();
            }
            logo.render( 0, 0, nullptr, nullptr, true );
        }
        else if ( step == 3 )
        {
            if ( time1.gettime() >= 30 )
            {
                logo.setAlpha( alpha );
                alpha -= 5;
                time1.setstarttime();
            }
            else if (alpha <= 0 )
            {
                scene = MENU;
                buttons.clear();
                alpha = 0;
                step = 0;
            }
            logo.render( 0, 0, nullptr, nullptr, true );
        }
    }

    else if ( scene == MENU )
    {
        if ( step == 0)
        {
            buttons.clear();
            menu.render( 0, 0, nullptr, nullptr, true );

            SDL_Rect custom;
            custom.w = SCREEN_WIDTH / 4;
            custom.h = custom.w / 3;
            custom.x = SCREEN_WIDTH * 0.235;
            custom.y = SCREEN_HEIGHT / 6 * 4;
            buttons.emplace_back(&menu2, custom, menuclips[0]);

            custom.x = SCREEN_WIDTH * 0.235;
            custom.y = SCREEN_HEIGHT / 6 * 4 + custom.h * 1.125;
            buttons.emplace_back(&menu2, custom, menuclips[1]);

            custom.x = SCREEN_WIDTH * 0.765 - custom.w;
            custom.y = SCREEN_HEIGHT / 6 * 4;
            buttons.emplace_back(&menu2, custom, menuclips[2]);

            custom.x = SCREEN_WIDTH * 0.765 - custom.w;
            custom.y = SCREEN_HEIGHT / 6 * 4 + custom.h * 1.125;
            buttons.emplace_back(&menu2, custom, menuclips[3]);
            step = 1;
        }
        else if ( step == 1 )
        {
            menu.render( 0, 0, nullptr, nullptr, true );
            buttons[0].render();
            buttons[1].render();
            buttons[2].render();
            buttons[3].render();
        }
    }
    
    else if ( scene == SETTING )
    {
        if (step == 0)
        {
            background.render( 0, 0, nullptr, nullptr, true );
            black.setAlpha(175);
            SDL_Rect custom;
            custom.h = background.mh;
            custom.w = background.mw;
            custom.x = background.mx;
            custom.y = background.my;
            black.render( 0, 0, &custom );

            custom.h = SCREEN_HEIGHT * 0.85;
            custom.w = custom.h;
            custom.x = (SCREEN_WIDTH - custom.w) / 2;
            custom.y = (SCREEN_HEIGHT - custom.h) / 2;
            setting.render( 0, 0, &custom );

            buttons.clear();
            custom.w = setting.mw * 0.38;
            custom.h = setting.mh / 15;
            custom.x = setting.mx + setting.mw * 19 / 35;
            custom.y = setting.my + setting.mh * 92 / 105;
            buttons.emplace_back(&black, custom); //back button 0

            custom.w = setting.mw * 0.38;
            custom.h = setting.mh / 15;
            custom.x = setting.mx + setting.mw * 8 / 105;
            custom.y = setting.my + setting.mh * 92 / 105;
            buttons.emplace_back(&black, custom); //apply button 1

            custom.w = setting.mw * 11 / 210;
            custom.h = setting.mh * 11 / 210;
            custom.x = setting.mx + setting.mw * 101 / 210;
            custom.y = setting.my + setting.mh * 7 / 30;
            buttons.emplace_back(&black, custom); //music- button 2

            custom.w = setting.mw * 11 / 210;
            custom.h = setting.mh * 11 / 210;
            custom.x = setting.mx + setting.mw * 88 / 105;
            custom.y = setting.my + setting.mh * 7 / 30;
            buttons.emplace_back(&black, custom); //music+ button 3

            custom.w = setting.mw * 11 / 210;
            custom.h = setting.mh * 11 / 210;
            custom.x = setting.mx + setting.mw * 101 / 210;
            custom.y = setting.my + setting.mh * 31 / 105;
            buttons.emplace_back(&black, custom); //sfx- button 4

            custom.w = setting.mw * 11 / 210;
            custom.h = setting.mh * 11 / 210;
            custom.x = setting.mx + setting.mw * 88 / 105;
            custom.y = setting.my + setting.mh * 31 / 105;
            buttons.emplace_back(&black, custom); //sfx+ button 5

            custom.w = setting.mw * 11 / 210;
            custom.h = setting.mh * 11 / 210;
            custom.x = setting.mx + setting.mw * 8 / 75;
            custom.y = setting.my + setting.mh * 11 / 25;
            buttons.emplace_back(&black, custom); //vsync button 6

            custom.w = setting.mw * 11 / 210;
            custom.h = setting.mh * 11 / 210;
            custom.x = setting.mx + setting.mw * 8 / 75;
            custom.y = setting.my + setting.mh * 109 / 210;
            buttons.emplace_back(&black, custom); //fps show button 7

            custom.w = setting.mw * 11 / 210;
            custom.h = setting.mh * 11 / 210;
            custom.x = setting.mx + setting.mw * 8 / 75;
            custom.y = setting.my + setting.mh * 127 / 210;
            buttons.emplace_back(&black, custom); //framerate button 8

            custom.w = setting.mw * 34 / 105;
            custom.h = setting.mh * 13 / 210;
            custom.x = setting.mx + setting.mw * 59 / 105;
            custom.y = setting.my + setting.mh * 19 / 42;
            buttons.emplace_back(&black, custom); //rebind key button 9

            custom.w = setting.mw * 34 / 105;
            custom.h = setting.mh * 11 / 105;
            custom.x = setting.mx + setting.mw * 59 / 105;
            custom.y = setting.my + setting.mh * 37 / 70;
            buttons.emplace_back(&black, custom); //unknown button 10

            custom.w = setting.mw * 11 / 210;
            custom.h = setting.mh * 11 / 210;
            custom.x = setting.mx + setting.mw * 58 / 105;
            custom.y = setting.my + setting.mh * 376 / 525;
            buttons.emplace_back(&black, custom); //vn lang button 11

            custom.w = setting.mw * 11 / 210;
            custom.h = setting.mh * 11 / 210;
            custom.x = setting.mx + setting.mw * 58 / 105;
            custom.y = setting.my + setting.mh * 39 / 50;
            buttons.emplace_back(&black, custom); //eng lang button 12

            custom.w = setting.mw * 13 / 42;
            custom.h = setting.mh * 3 / 35;
            custom.x = setting.mx + setting.mw * 4 / 35;
            custom.y = setting.my + setting.mh * 26 / 35;
            buttons.emplace_back(&black, custom); //custom fps button 13

            //max framerate (trung tâm): w = * 47 / 175; h = * 412 / 525
            //music vollume (trung tâm): w = * 52 / 75; h = * 9 / 35
            //sfx (trung tâm): w = * 52 / 75; h = * 67 / 210

            //Uint32 fps;
            msv = music_vollume;
            sfx = SFX_vollume;
            vs = vsync;
            fpss = fps_show;
            fpm = fps_max;
            vnm = vn;
            ff = fpscustom;
            step = 1;
        }
        else if (step == 1)
        {
            background.render( 0, 0, nullptr, nullptr, true );
            black.setAlpha(175);
            SDL_Rect custom;
            custom.h = background.mh;
            custom.w = background.mw;
            custom.x = background.mx;
            custom.y = background.my;
            black.render( 0, 0, &custom );

            custom.h = SCREEN_HEIGHT * 0.85;
            custom.w = custom.h;
            custom.x = (SCREEN_WIDTH - custom.w) / 2;
            custom.y = (SCREEN_HEIGHT - custom.h) / 2;
            setting.render( 0, 0, &custom );

            if(vs)
            {
                custom.w = setting.mw * 11 / 210;
                custom.h = setting.mh * 11 / 210;
                custom.x = setting.mx + setting.mw * 8 / 75;
                custom.y = setting.my + setting.mh * 11 / 25;
                tick.render( 0, 0, &custom );
            }
            if (fpss)
            {
                custom.w = setting.mw * 11 / 210;
                custom.h = setting.mh * 11 / 210;
                custom.x = setting.mx + setting.mw * 8 / 75;
                custom.y = setting.my + setting.mh * 109 / 210;
                tick.render( 0, 0, &custom );
            }
            if(fpm)
            {
                custom.w = setting.mw * 11 / 210;
                custom.h = setting.mh * 11 / 210;
                custom.x = setting.mx + setting.mw * 8 / 75;
                custom.y = setting.my + setting.mh * 127 / 210;
                tick.render( 0, 0, &custom );
            }
            if(vnm)
            {
                custom.w = setting.mw * 11 / 210;
                custom.h = setting.mh * 11 / 210;
                custom.x = setting.mx + setting.mw * 58 / 105;
                custom.y = setting.my + setting.mh * 376 / 525;
                tick.render( 0, 0, &custom );
            }
            else
            {
                custom.w = setting.mw * 11 / 210;
                custom.h = setting.mh * 11 / 210;
                custom.x = setting.mx + setting.mw * 58 / 105;
                custom.y = setting.my + setting.mh * 39 / 50;
                tick.render( 0, 0, &custom );
            }

            if (!ff.empty())
            {
                SDL_Color textColor = { 255, 255, 255, 255 };
                text.loadFromRenderedText( ff, textColor, gArial );
                custom.h = setting.mh * 3 / 35;
                custom.w = static_cast<int>( (float)text.mw / text.mh * custom.h );
                custom.x = setting.mx + setting.mw * 4 / 35 + (setting.mw * 14 / 42 - custom.w) / 2;
                custom.y = setting.my + setting.mh * 26 / 35;
                text.render(0, 0, &custom);
            }
            string a = to_string(msv * 100 / 128) + "%" ;
            SDL_Color textColor = { 255, 255, 255, 255 };
            text.loadFromRenderedText( a, textColor, gArial );
            custom.h = setting.mh * 0.04;
            custom.w = static_cast<int>( (float)text.mw / text.mh * custom.h );
            custom.x = setting.mx + setting.mw * 39 / 70 + (setting.mw * 9 / 35 - custom.w) / 2;
            custom.y = setting.my + setting.mh * 7 / 30;
            text.render(0, 0, &custom);

            a = to_string(sfx * 100 / 128) + "%" ;
            text.loadFromRenderedText( a, textColor, gArial );
            custom.h = setting.mh * 0.04;
            custom.w = static_cast<int>( (float)text.mw / text.mh * custom.h );
            custom.x = setting.mx + setting.mw * 39 / 70 + (setting.mw * 9 / 35 - custom.w) / 2;
            custom.y = setting.my + setting.mh * 3 / 10;
            text.render(0, 0, &custom);
        }
    }

    else if (scene == SETTING_KEY)
    {
        if (step == 0)
        {
            background.render( 0, 0, nullptr, nullptr, true );
            black.setAlpha(175);
            SDL_Rect custom;
            custom.h = background.mh;
            custom.w = background.mw;
            custom.x = background.mx;
            custom.y = background.my;
            black.render( 0, 0, &custom );

            custom.h = SCREEN_HEIGHT * 0.85;
            custom.w = custom.h;
            custom.x = (SCREEN_WIDTH - custom.w) / 2;
            custom.y = (SCREEN_HEIGHT - custom.h) / 2;
            setting2.render( 0, 0, &custom );

            buttons.clear();
            custom.w = setting.mw * 0.57;
            custom.h = setting.mh * 6 / 23;
            custom.x = setting.mx + setting.mw * 3 / 10;
            custom.y = setting.my + setting.mh * 7 / 10;
            buttons.emplace_back(&black, custom); //back button 0

            if (vn)
            {
                custom.w = setting2.mw * 3/4;
                custom.h = custom.w * skvn.mh / skvn.mw;
                custom.x = (setting2.mw - custom.w) / 2 + setting2.mx;
                custom.y = setting2.my * 4;
                skvn.render( 0, 0, &custom );
            }
            else
            {
                custom.w = setting2.mw * 3/4;
                custom.h = custom.w * skvn.mh / skvn.mw;
                custom.x = (setting2.mw - custom.w) / 2 + setting2.mx;
                custom.y = setting2.my * 4;
                ske.render( 0, 0, &custom );
            }

            step = 1;
        }
        else if (step == 1)
        {
            background.render( 0, 0, nullptr, nullptr, true );
            black.setAlpha(175);
            SDL_Rect custom;
            custom.h = background.mh;
            custom.w = background.mw;
            custom.x = background.mx;
            custom.y = background.my;
            black.render( 0, 0, &custom );

            custom.h = SCREEN_HEIGHT * 0.85;
            custom.w = custom.h;
            custom.x = (SCREEN_WIDTH - custom.w) / 2;
            custom.y = (SCREEN_HEIGHT - custom.h) / 2;
            setting2.render( 0, 0, &custom );

            if (vn)
            {
                custom.w = setting2.mw * 3/4;
                custom.h = custom.w * skvn.mh / skvn.mw;
                custom.x = (setting2.mw - custom.w) / 2 + setting2.mx;
                custom.y = setting2.my * 4;
                skvn.render( 0, 0, &custom );
            }
            else
            {
                custom.w = setting2.mw * 3/4;
                custom.h = custom.w * skvn.mh / skvn.mw;
                custom.x = (setting2.mw - custom.w) / 2 + setting2.mx;
                custom.y = setting2.my * 4;
                ske.render( 0, 0, &custom );
            }
        }
    }
    
    else if ( scene == NEW_GAME )
    {
        if ( step == 0 )
        {
            newgamey = 1;
            time1.setstarttime();
            step = 1;
            xsp = 41;
            ysp = 43;
            d = FRONT;
        }
        else if (step == 1)
        {
            if ( time1.gettime() >= 10 )
            {
                newgamey -= 0.001;
                time1.setstarttime();
            }
            SDL_Rect custom;
            custom.w = SCREEN_WIDTH * 2 / 3;
            custom.h = custom.w * newgame.mh / newgame.mw;
            custom.x = SCREEN_WIDTH / 6;
            custom.y = SCREEN_HEIGHT * newgamey;
            newgame.render(0,0, &custom);

            custom.w = SCREEN_HEIGHT / 6;
            custom.h = SCREEN_WIDTH;
            custom.x = SCREEN_WIDTH / 2 - custom.w / 2;
            custom.y = SCREEN_HEIGHT / 6 * 5 - custom.h / 2 + custom.w / 2;
            SDL_Point custom2 = {custom.w / 2, custom.h / 2 };
            transparent2.render(0, 0, &custom, nullptr, false, -90, &custom2 );
            custom.y = - custom.h / 2 + custom.w / 2;
            transparent2.render(0, 0, &custom, nullptr, false, 90, &custom2 );
            if ( SCREEN_HEIGHT * newgamey < - SCREEN_WIDTH * 2 / 3 * newgame.mh / newgame.mw) 
            {
                step = 0;
                scene = MAP_T1;
                newgamey = 1;
            }
        }
    }

    else if ( scene == MAP_T1 )
    {
        if (step==0)
        {
            hero.getmapxy(&mapt1, "sanh1.txt");
            hero.addtexture(&hero2, 3, 4, 6);
            hero.startpoint(xsp,ysp);
            hero.direction = d;

            hero.cameraxy();
            SDL_Rect cameraRect = hero.camxy();
            mapt1.render(0, 0, nullptr, &cameraRect);
            hero.animated(hero.mx_camx, hero.my_camy);

            if (fadeout(&black)) step = 1;
        }
        else if (step == 1)
        {
            hero.cameraxy();
            SDL_Rect cameraRect = hero.camxy();
            mapt1.render(0, 0, nullptr, &cameraRect);
            hero.animated(hero.mx_camx, hero.my_camy);

            if (hero.blockevent == 801)
            {
                if (vn)
                {
                    string aa = "Nhấn E để mở cửa";
                    SDL_Color textColor = { 0, 0, 0, 255 };
                    text.loadFromRenderedText( aa, textColor, gTimes );
                    SDL_Rect custom;
                    custom.h = SCREEN_HEIGHT / 15;
                    custom.w = static_cast<int>( (float)text.mw / text.mh * custom.h );
                    custom.x = (SCREEN_WIDTH - custom.w) / 2;
                    custom.y = (SCREEN_HEIGHT - custom.h) / 2;
                    text.render(0, 0, &custom);
                }
                else
                {
                    string aa = "Press E to open the door";
                    SDL_Color textColor = { 0, 0, 0, 255 };
                    text.loadFromRenderedText( aa, textColor, gTimes );
                    SDL_Rect custom;
                    custom.h = SCREEN_HEIGHT / 15;
                    custom.w = static_cast<int>( (float)text.mw / text.mh * custom.h );
                    custom.x = (SCREEN_WIDTH - custom.w) / 2;
                    custom.y = (SCREEN_HEIGHT - custom.h) / 2;
                    text.render(0, 0, &custom);
                }
            }
        }
    }

    else if ( scene == MAP_T2 )
    {
        if (step==0)
        {
            hero.getmapxy(&mapt2, "sanh2.txt");
            hero.addtexture(&hero2, 3, 4, 6);
            hero.startpoint(xsp,ysp);
            hero.direction = d;

            hero.cameraxy();
            SDL_Rect cameraRect = hero.camxy();
            mapt2.render(0, 0, nullptr, &cameraRect);
            hero.animated(hero.mx_camx, hero.my_camy);

            if (fadeout(&black)) step = 1;
        }
        else if (step == 1)
        {
            hero.cameraxy();
            SDL_Rect cameraRect = hero.camxy();
            mapt2.render(0, 0, nullptr, &cameraRect);
            hero.animated(hero.mx_camx, hero.my_camy);
        }
    }

    else if ( scene == MAP_A1 )
    {
        if (step==0)
        {
            hero.getmapxy(&mapa1, "m1.txt");
            hero.addtexture(&hero2, 3, 4, 6);
            hero.startpoint(xsp,ysp);
            hero.direction = d;

            hero.cameraxy();
            SDL_Rect cameraRect = hero.camxy();
            mapa1.render(0, 0, nullptr, &cameraRect);
            hero.animated(hero.mx_camx, hero.my_camy);

            if (fadeout(&black)) step = 1;
        }
        else if (step == 1)
        {
            hero.cameraxy();
            SDL_Rect cameraRect = hero.camxy();
            mapa1.render(0, 0, nullptr, &cameraRect);
            hero.animated(hero.mx_camx, hero.my_camy);
        }
    }

    else if ( scene == MAP_A2 )
    {
        if (step==0)
        {
            hero.getmapxy(&mapa2, "m2.txt");
            hero.addtexture(&hero2, 3, 4, 6);
            hero.startpoint(xsp,ysp);
            hero.direction = d;

            hero.cameraxy();
            SDL_Rect cameraRect = hero.camxy();
            mapa2.render(0, 0, nullptr, &cameraRect);
            hero.animated(hero.mx_camx, hero.my_camy);

            if (fadeout(&black)) step = 1;
        }
        else if (step == 1)
        {
            hero.cameraxy();
            SDL_Rect cameraRect = hero.camxy();
            mapa2.render(0, 0, nullptr, &cameraRect);
            hero.animated(hero.mx_camx, hero.my_camy);
        }
    }

    else if ( scene == MAP_A3 )
    {
        if (step==0)
        {
            hero.getmapxy(&mapa3, "m3.txt");
            hero.addtexture(&hero2, 3, 4, 6);
            hero.startpoint(xsp,ysp);
            hero.direction = d;

            hero.cameraxy();
            SDL_Rect cameraRect = hero.camxy();
            mapa3.render(0, 0, nullptr, &cameraRect);
            hero.animated(hero.mx_camx, hero.my_camy);

            if (fadeout(&black)) step = 1;
        }
        else if (step == 1)
        {
            hero.cameraxy();
            SDL_Rect cameraRect = hero.camxy();
            mapa3.render(0, 0, nullptr, &cameraRect);
            hero.animated(hero.mx_camx, hero.my_camy);
        }
    }

    else if ( scene == MAP_A4 )
    {
        if (step==0)
        {
            hero.getmapxy(&mapa4, "m4.txt");
            hero.addtexture(&hero2, 3, 4, 6);
            hero.startpoint(xsp,ysp);
            hero.direction = d;

            hero.cameraxy();
            SDL_Rect cameraRect = hero.camxy();
            mapa4.render(0, 0, nullptr, &cameraRect);
            hero.animated(hero.mx_camx, hero.my_camy);

            if (fadeout(&black)) step = 1;
        }
        else if (step == 1)
        {
            hero.cameraxy();
            SDL_Rect cameraRect = hero.camxy();
            mapa4.render(0, 0, nullptr, &cameraRect);
            hero.animated(hero.mx_camx, hero.my_camy);
        }
    }

    else if ( scene == MAP_GARDEN )
    {
        if (step==0)
        {
            hero.getmapxy(&mapg1, "mgarden.txt");
            hero.addtexture(&hero2, 3, 4, 6);
            dimension.addtexture(&dimension2, 8, 8, 4);
            gate.addtexture(&gate2, 4, 1, 5);
            hero.startpoint(xsp,ysp);
            hero.direction = d;

            hero.cameraxy();
            SDL_Rect cameraRect = hero.camxy();
            mapg1.render(0, 0, nullptr, &cameraRect);
            dimension.animatedeffect(4*48-48 - hero.camera.x, 17*48-48 - hero.camera.y, 1, 4, 2);
            dimension.animatedeffect(4*48-48 - hero.camera.x, 28*48-48 - hero.camera.y, 1, 4, 2);
            dimension.animatedeffect(13*48-48 - hero.camera.x, 5*48-48 - hero.camera.y, 1, 4, 2);
            dimension.animatedeffect(24*48-48 - hero.camera.x, 5*48-48 - hero.camera.y, 1, 4, 2);
            dimension.animatedeffect(33*48-48 - hero.camera.x, 17*48-48 - hero.camera.y, 1, 4, 2);
            dimension.animatedeffect(33*48-48 - hero.camera.x, 28*48-48 - hero.camera.y, 1, 4, 2);
            dimension.animatedeffect(13*48-48 - hero.camera.x, 37*48-48 - hero.camera.y, 1, 4, 2);
            dimension.animatedeffect(24*48-48 - hero.camera.x, 37*48-48 - hero.camera.y, 1, 4, 2);
            gate.animatedeffect(18*48-48 - hero.camera.x, 8*48-48 - hero.camera.y, 1, 1);
            if(!cha1)dimension.animatedeffect(8*48-48 - hero.camera.x, 12*48-48 - hero.camera.y, 33, 36);
            if(!cha2)dimension.animatedeffect(30*48-48 - hero.camera.x, 12*48-48 - hero.camera.y, 41, 44);
            if(!cha4)dimension.animatedeffect(8*48-48 - hero.camera.x, 34*48-48 - hero.camera.y, 49, 52);
            if(!cha3)dimension.animatedeffect(30*48-48 - hero.camera.x, 34*48-48 - hero.camera.y, 57, 60);
            hero.animated(hero.mx_camx, hero.my_camy);

            if (fadeout(&black)) step = 1;
        }
        else if (step == 1)
        {
            hero.cameraxy();
            SDL_Rect cameraRect = hero.camxy();
            mapg1.render(0, 0, nullptr, &cameraRect);
            dimension.animatedeffect(4*48-48 - hero.camera.x, 17*48-48 - hero.camera.y, 1, 4, 2);
            dimension.animatedeffect(4*48-48 - hero.camera.x, 28*48-48 - hero.camera.y, 1, 4, 2);
            dimension.animatedeffect(13*48-48 - hero.camera.x, 5*48-48 - hero.camera.y, 1, 4, 2);
            dimension.animatedeffect(24*48-48 - hero.camera.x, 5*48-48 - hero.camera.y, 1, 4, 2);
            dimension.animatedeffect(33*48-48 - hero.camera.x, 17*48-48 - hero.camera.y, 1, 4, 2);
            dimension.animatedeffect(33*48-48 - hero.camera.x, 28*48-48 - hero.camera.y, 1, 4, 2);
            dimension.animatedeffect(13*48-48 - hero.camera.x, 37*48-48 - hero.camera.y, 1, 4, 2);
            dimension.animatedeffect(24*48-48 - hero.camera.x, 37*48-48 - hero.camera.y, 1, 4, 2);
            gate.animatedeffect(18*48-48 - hero.camera.x, 8*48-48 - hero.camera.y, 1, 1);
            if(!cha1)dimension.animatedeffect(8*48-48 - hero.camera.x, 12*48-48 - hero.camera.y, 33, 36);
            if(!cha2)dimension.animatedeffect(30*48-48 - hero.camera.x, 12*48-48 - hero.camera.y, 41, 44);
            if(!cha4)dimension.animatedeffect(8*48-48 - hero.camera.x, 34*48-48 - hero.camera.y, 49, 52);
            if(!cha3)dimension.animatedeffect(30*48-48 - hero.camera.x, 34*48-48 - hero.camera.y, 57, 60);
            hero.animated(hero.mx_camx, hero.my_camy);
        }
    }

    else if ( scene == CHALLENGE_1 )
    {
        if (step==0)
        {
            hero.getmapxy(&challenge1, "maze.txt");
            hero.addtexture(&hero2, 3, 4, 6);
            hero.startpoint(xsp,ysp);
            hero.direction = d;

            hero.cameraxy();
            SDL_Rect cameraRect = hero.camxy();
            challenge1.render(0, 0, nullptr, &cameraRect);
            hero.animated(hero.mx_camx, hero.my_camy);
            dark.mw = hero.mWidth*80;
            dark.mh = hero.mHeight*80;
            SDL_Rect custom;
            custom.w = dark.mw;
            custom.h = dark.mh;
            custom.x = hero.mx_camx - dark.mw/2 + hero.mWidth/2;
            custom.y = hero.my_camy - dark.mh/2 + hero.mHeight/2;
            dark.render( 0, 0, &custom);

            if (fadeout(&black)) step = 1;
        }
        else if (step == 1)
        {
            hero.cameraxy();
            SDL_Rect cameraRect = hero.camxy();
            challenge1.render(0, 0, nullptr, &cameraRect);
            crystalr.render(57*48 - hero.camera.x, 4*48 - hero.camera.y);
            hero.animated(hero.mx_camx, hero.my_camy);
            SDL_Rect custom;
            custom.w = dark.mw;
            custom.h = dark.mh;
            custom.x = hero.mx_camx - dark.mw/2 + hero.mWidth/2;
            custom.y = hero.my_camy - dark.mh/2 + hero.mHeight/2;
            dark.render( 0, 0, &custom);
        }
    }

    else if ( scene == CHALLENGE_3 )
    {
        if (step==0)
        {
            hero.getmapxy(&challenge3, "cha3.txt");
            hero.addtexture(&hero2, 3, 4, 6);
            hero.startpoint(xsp,ysp);
            hero.direction = d;

            hero.cameraxy();
            SDL_Rect cameraRect = hero.camxy();
            challenge3.render(0, 0, nullptr, &cameraRect);
            hero.animated(hero.mx_camx, hero.my_camy);

            if (fadeout(&black)) step = 1;
        }
        else if (step == 1)
        {
            hero.cameraxy();
            SDL_Rect cameraRect = hero.camxy();
            challenge3.render(0, 0, nullptr, &cameraRect);
            crystalb.render(35*48 - hero.camera.x, 42*48 - hero.camera.y);
            hero.animated(hero.mx_camx, hero.my_camy);
        }
    }

    else if ( scene == GAME_OVER )
    {
        if (vn) gameovervn.render2( 0, 0, nullptr, nullptr, true );
        else gameovereng.render2( 0, 0, nullptr, nullptr, true );
    }

    if (step != 0 && !bullets1.empty())
    {
        for (auto& bullet : bullets1) bullet.active(hero.mPosX, hero.mPosY, hero.camera.x, hero.camera.y);
        bullets1.erase (remove_if(bullets1.begin(), bullets1.end(), [](Bullet& b) { return !b.start; }), bullets1.end());
    }
    if (step != 0 && !bullets2.empty())
    {
        for (auto& bullet : bullets2) bullet.active(hero.mPosX, hero.mPosY, hero.camera.x, hero.camera.y);
        bullets2.erase (remove_if(bullets2.begin(), bullets2.end(), [](Bullet& b) { return !b.start; }), bullets2.end());
    }
    
    if (fps_show)
    {
        if ( fpscc.gettime() >= 1000 )
        {
            frame2 = frame;
            SDL_Color textColor = { 255, 255, 255, 255 };
            text.loadFromRenderedText( to_string(frame2) + " FPS", textColor, gArial );
            frame = 0;
            fpscc.setstarttime();
        }
        frame++;
        if (text.mw != 0)
        {
            SDL_Color textColor = { 255, 255, 255, 255 };
            text.loadFromRenderedText( to_string(frame2) + " FPS", textColor, gArial );
            SDL_Rect custom;
            custom.h = SCREEN_HEIGHT / 25;
            custom.w = static_cast<int>( (float)text.mw / text.mh * custom.h );
            custom.x = 0;
            custom.y = 0;
            text.render(0, 0, &custom);
        }
    }

    if (scene >= MAP_T1 && scene <= CHALLENGE_4)
    {
        string a = "Hp: " + to_string(hero.datas.health);
        SDL_Color textColor = { 255, 255, 255, 255 };
        text.loadFromRenderedText( a, textColor, gArial );
        SDL_Rect custom;
        custom.h = SCREEN_HEIGHT / 25;
        custom.w = static_cast<int>( (float)text.mw / text.mh * custom.h );
        custom.x = SCREEN_WIDTH - text.mw;
        custom.y = 0;
        text.render(0, 0, &custom);
    }
}

bool Scene::fadein(Texture* tex) //hiện dần
{
    if (stepf == 0)
    {
        alpha = 0;
        tex->setAlpha( 0 );
        time1.setstarttime();
        stepf = 1;
    }
    if( stepf == 1 )
    {
        if ( time1.gettime() >= 10 )
        {
            tex->setAlpha( alpha );
            alpha += 5;
            time1.setstarttime();
        }
        if (alpha >= 255 )
        {
            stepf = 0;
            alpha = 255;
            return true;
        }
        SDL_Rect custom;
        custom.w = SCREEN_WIDTH;
        custom.h = SCREEN_HEIGHT;
        tex->render( 0, 0, &custom);
    }
    return false;
}

bool Scene::fadeout(Texture* tex) //mờ dần
{
    if (stepf == 0)
    {
        alpha = 255;
        tex->setAlpha( 255 );
        time1.setstarttime();
        stepf = 1;
    }
    if ( stepf == 1 )
    {
        if ( time1.gettime() >= 10 )
        {
            tex->setAlpha( alpha );
            alpha -= 5;
            time1.setstarttime();
        }
        else if (alpha <= 0 )
        {
            alpha = 0;
            stepf = 0;
            return true;
        }
        SDL_Rect custom;
        custom.w = SCREEN_WIDTH;
        custom.h = SCREEN_HEIGHT;
        tex->render( 0, 0, &custom);
    }
    return false;
}

void Scene::free()
{
    logo.free();
    menu.free();
    menu2.free();
    setting.free();
    setting2.free();
    background.free();
    black.free();
    white.free();
    transparent.free();
    transparent2.free();
    tick.free();
    text.free();
    newgame.free();
    hero2.free();
    mapt1.free();
    mapt2.free();
    mapa1.free();
    mapa2.free();
    mapa3.free();
    mapa4.free();
    mapg1.free();
    mapg2.free();
    challenge1.free();
    challenge3.free();
    dark.free();
    dimension2.free();
    gate2.free();
    for (int i=0; i<9; i++) bullettexture[i].free();
    crystalr.free();
    crystalg.free();
    crystalb.free();
    crystaly.free();
    skvn.free();
    ske.free();
    gameovervn.free();
    gameovereng.free();
    buttons.clear();
    gWindow.free();
    SDL_DestroyRenderer( gRenderer );
    TTF_CloseFont( gTimes );
    TTF_CloseFont( gArial );
    gRenderer = nullptr;
    gTimes = nullptr;
    gArial = nullptr;
}

void Scene::init()
{
    gWindow.init();
    loadMedia();
}

void loadMedia()
{
    logo.loadFromFile( "assets/texture/img/logo.jpg" );
    menu.loadFromFile( "assets/texture/img/menu.png" );
    menu2.loadFromFile( "assets/texture/img/menu 2.png" );
    setting.loadFromFile( "assets/texture/img/setting.png", true, 166, 166, 166 );
    setting2.loadFromFile( "assets/texture/img/keys.png", true, 166, 166, 166 );
    background.loadFromFile( "assets/texture/img/background.png" );
    black.loadFromFile( "assets/texture/img/black.png" );
    white.loadFromFile( "assets/texture/img/white.png" );
    transparent.loadFromFile( "assets/texture/img/transparent.png" );
    transparent2.loadFromFile( "assets/texture/img/transparent 2.png" );
    tick.loadFromFile( "assets/texture/img/tick.png" );
    newgame.loadFromFile( "assets/texture/img/new game vn.png" );
    hero2.loadFromFile( "assets/texture/characters/hero.png" );
    mapt1.loadFromFile( "assets/texture/map/sanh1.png" );
    mapt2.loadFromFile( "assets/texture/map/sanh2.png" );
    mapa1.loadFromFile( "assets/texture/map/m1.png" );
    mapa2.loadFromFile( "assets/texture/map/m2.png" );
    mapa3.loadFromFile( "assets/texture/map/m3.png" );
    mapa4.loadFromFile( "assets/texture/map/m4.png" );
    mapg1.loadFromFile( "assets/texture/map/mgarden.png" );
    mapg2.loadFromFile( "assets/texture/map/mgarden2.png" );
    challenge1.loadFromFile( "assets/texture/map/maploz.png" );
    challenge3.loadFromFile( "assets/texture/map/cha3.png" );
    dark.loadFromFile( "assets/texture/img/transparent3.png" );
    dimension2.loadFromFile( "assets/texture/effect/dimension2.png" );
    gate2.loadFromFile( "assets/texture/map/gate.png" );
    bullettexture[0].loadFromFile( "assets/texture/effect/classic.png" );
    bullettexture[1].loadFromFile( "assets/texture/effect/fire.png" );
    bullettexture[2].loadFromFile( "assets/texture/effect/earth.png" );
    bullettexture[3].loadFromFile( "assets/texture/effect/wind.png" );
    bullettexture[4].loadFromFile( "assets/texture/effect/thunder.png" );
    bullettexture[5].loadFromFile( "assets/texture/effect/ice.png" );
    bullettexture[6].loadFromFile( "assets/texture/effect/holy.png" );
    bullettexture[7].loadFromFile( "assets/texture/effect/dark1.png" );
    bullettexture[8].loadFromFile( "assets/texture/effect/dark2.png" );
    crystalr.loadFromFile( "assets/texture/map/crystal red.png" );
    crystalg.loadFromFile( "assets/texture/map/crystal green.png" );
    crystalb.loadFromFile( "assets/texture/map/crystal blue.png" );
    crystaly.loadFromFile( "assets/texture/map/crystal yellow.png" );
    skvn.loadFromFile( "assets/texture/img/k1.png" );
    ske.loadFromFile( "assets/texture/img/k2.png" );
    gameovervn.loadFromFile( "assets/texture/img/gameover1.png" );
    gameovereng.loadFromFile( "assets/texture/img/gameover2.png" );

    sfxsound[0] = Mix_LoadWAV("assets/sound/fire sfx.wav");
    sfxsound[1] = Mix_LoadWAV("assets/sound/fire explosion sfx.wav");
    sfxsound[2] = Mix_LoadWAV("assets/sound/earth explosion sfx.wav");
    sfxsound[3] = Mix_LoadWAV("assets/sound/thunder sfx.wav");
    sfxsound[4] = Mix_LoadWAV("assets/sound/thunder explosion sfx.wav");
    sfxsound[5] = Mix_LoadWAV("assets/sound/ice explosion sfx.wav");
    sfxsound[6] = Mix_LoadWAV("assets/sound/explosion sfx.wav");

    for( int j = 0; j < 4; ++j )
    {
        menuclips[ j ].x = 0;
        menuclips[ j ].y = j * 200;
        menuclips[ j ].w = 600;
        menuclips[ j ].h = 200;
    }
    
    gTimes = TTF_OpenFont( "assets/fonts/times.ttf", 28 );
    gArial = TTF_OpenFont( "assets/fonts/arial.ttf", 28 );
}

void close()
{
    logo.free();
    menu.free();
    menu2.free();
    setting.free();
    setting2.free();
    background.free();
    black.free();
    white.free();
    transparent.free();
    tick.free();
    text.free();
    newgame.free();
    hero2.free();
    mapt1.free();
    mapt2.free();
    mapa1.free();
    mapa2.free();
    mapa3.free();
    mapa4.free();
    mapg1.free();
    mapg2.free();
    challenge1.free();
    challenge3.free();
    dark.free();
    dimension2.free();
    gate2.free();
    for (int i=0; i<9; i++) bullettexture[i].free();
    crystalr.free();
    crystalg.free();
    crystalb.free();
    crystaly.free();
    skvn.free();
    ske.free();
    gameovervn.free();
    gameovereng.free();


    gWindow.free();

    SDL_DestroyRenderer( gRenderer );
    TTF_CloseFont( gTimes );
    TTF_CloseFont( gArial );
    gRenderer = nullptr;
    gTimes = nullptr;
    gArial = nullptr;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}