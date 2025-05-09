#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "Texture.h"
#include "Window.h"
#include "Timer.h"
#include "Button.h"
#include "characters.h"
#include <iostream>
#include <vector>
using namespace std;

enum Scenes
{
    START_MENU = 0,
    MENU = 1,
    SETTING = 2,
    SETTING_KEY = 3,
    NEW_GAME = 4,
    LOAD_GAME = 5,
    MAP_T1,
    MAP_T2,
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
    private:
        Scenes scene;
        int step;
        Uint8 alpha;
        vector<Button> buttons;
        int music_vollume;
        int SFX_vollume;
        bool fps_show;
        bool vn;
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
Characters hero;
SDL_Rect menuclips[ 4 ];
Timer time1;
Timer fpscc;
Window gWindow;

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

Scene::Scene()
{
    scene = START_MENU;
    step = 0;
    alpha = 0;
    music_vollume = 64;
    SFX_vollume = 64;
    vsync = true;
    fps_show = false;
    fps_max = false;
    vn = true; 
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
                    //scene = MAP_T1;
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
                    //step = 0;
                    //scene = LOAD_GAME;
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
        else if(buttons[2].handleEvent(&e) == MOUSE_DOWN ) msv -= 8; 
        else if(buttons[3].handleEvent(&e) == MOUSE_DOWN ) msv += 8;
        else if(buttons[4].handleEvent(&e) == MOUSE_DOWN ) sfx -= 8;
        else if(buttons[5].handleEvent(&e) == MOUSE_DOWN ) sfx += 8;
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
            //step = 0;
            //scene = SETTING_KEY;
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
        //hero.motion(&e);
        hero.motionpixel(&e);
        hero.mousepixel(&e);
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_1)
        {
            scene = MENU;
            step = 0;
        }
        else if (hero.blockevent == 801 && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_2)
        {
            scene = MAP_T2;
            step = 0;
        }
    }
}

void Scene::logicScene()
{
    vsync2 = vsync;
    if ( scene == MAP_T1 && step != 0 )
    {
        //hero.move();
        hero.movepixel();
        if (hero.blockevent == 901) {}
        else if (hero.blockevent == 902) {}
        else if (hero.blockevent == 903) {}
        else if (hero.blockevent == 904) {}
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
        }
    }
    
    else if ( scene == NEW_GAME )
    {
        if ( step == 0 )
        {
            newgamey = 1;
            time1.setstarttime();
            step = 1;
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
            hero.addtexture(&hero2, 3, 4, 6);
            hero.getmapxy(&mapt1, "sanh1.txt");
            hero.startpoint(41,43);
            step = 1;
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
                    string aa = "Nhấn " + to_string(hero.blockevent) + " để mở cửa";
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
                    string aa = "Press " + to_string(hero.blockevent) + " to open the door";
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