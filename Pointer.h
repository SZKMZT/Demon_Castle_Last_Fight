
class Pointer
{
    public:
        Pointer(SDL_Renderer* renderer, TTF_Font* font, Window* window);
        ~Pointer();
        SDL_Renderer* getRenderer() { return gRenderer; }
        TTF_Font* getFont() { return gFont; }
        Window* getWindow() { return gWindow; }
        int getScreenWidth() { return screenWidth; }
        int getScreenHeight() { return screenHeight; }
    private:
        Window* gWindow;
        SDL_Renderer* gRenderer;
        TTF_Font* gFont;
        int screenWidth;
        int screenHeight;
};

Pointer::Pointer()
{
    gRenderer = renderer;
    gFont = font;
    gWindow = window;
    screenWidth = 0;
    screenHeight = 0;
}

Pointer::~Pointer()
{
    close();
}