#ifndef SDLFuncs_hpp
#define SDLFuncs_hpp

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

extern bool isRunning;
extern bool changed;
extern bool inpwrite;
extern const int ledcount;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Event event;
//extern SDL_Surface *textSurface;
//extern SDL_Texture * textTexture;
extern SDL_Rect rect;
extern TTF_Font *gFont;
extern SDL_Texture *ledOnTex;
extern SDL_Texture *ledOffTex;



void check_keyboard(SDL_Keycode key, bool *changed, bool *write);
SDL_Color setRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
bool init(const char* title, int w, int h);

void handleEvents();
void update();
void render();
void renderHelp();
void clean();
void quit();

void draw_circle(SDL_Surface* surface, int x, int y, int width, int height, int r, int g, int b)
{
    //SDL_LockSurface(surface);
    std::vector<uint8_t> pixels(surface->h * surface->pitch, 0);

    int dy, dx;
    int maxwidth = width * 4;
    double dist = 0, cx = width/2 , cy = height/2 ;
    double x0 =0, y0 = 0;
    for (dy = y; dy < height; dy++) {
        for (dx = x; dx < maxwidth; dx += 4) {
            x0 = (double) dx /4 - x - cx;
            y0 = (double) dy - y - cy;
            dist = (double) 4 * x0 * x0 / width / width + (double) 4* y0 * y0 / height / height;
            if(dist > 0.7)
            {
                pixels[dx + (dy * surface->pitch)] = 255;
                pixels[dx + (dy * surface->pitch) + 1] = 255;
                pixels[dx + (dy * surface->pitch) + 2] = 255;
                pixels[dx + (dy * surface->pitch) + 3] = 255;
            }
            else
            {
                pixels[dx + (dy * surface->pitch)] = b;
                pixels[dx + (dy * surface->pitch) + 1] = g;
                pixels[dx + (dy * surface->pitch) + 2] = r;
                pixels[dx + (dy * surface->pitch) + 3] = 0;
            }
        }
    }
    memcpy(surface->pixels, pixels.data(), surface->pitch * surface->h);

    SDL_UnlockSurface(surface);
}

void draw_rectangle(SDL_Surface* surface, int x, int y, int width, int height, int r, int g, int b)
{
    //SDL_LockSurface(surface);
    std::vector<uint8_t> pixels(surface->h * surface->pitch, 0);

    int dy, dx;
    int maxwidth = width * 4;
    for (dy = y; dy < height; dy++) {
        for (dx = x; dx < maxwidth; dx += 4) {
            pixels[dx + (dy * surface->pitch)] = b;
            pixels[dx + (dy * surface->pitch) + 1] = g;
            pixels[dx + (dy * surface->pitch) + 2] = r;
            pixels[dx + (dy * surface->pitch) + 3] = 0;
        }
    }
    memcpy(surface->pixels, pixels.data(), surface->pitch * surface->h);

    SDL_UnlockSurface(surface);
}

void handleEvents()
{
    while(SDL_PollEvent(&event) != 0 )
    {
        switch(event.type)
        {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                check_keyboard(event.key.keysym.sym, &changed, &inpwrite );
            default:
                break;
        }
    }
}

void quit()
{
    isRunning = false;
}

std::string to_lower(std::string my_str)
{
    std::string _str = my_str;
    std::transform(my_str.begin(), my_str.end(), _str.begin(), ::tolower);
    return _str;
}


SDL_Color setRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    return color;
}


class SDLText {
    public:
        SDLText()  ;
        SDLText(SDL_Renderer *_renderer, TTF_Font *_gFont, SDL_Color _color)
        {
            renderer = _renderer;
            gFont = _gFont;
            color = _color;
            texture = SDL_CreateTextureFromSurface(renderer, surface);
        }

        ~SDLText();

        SDL_Surface *surface;
        char * text;
        SDL_Rect rect;
        SDL_Renderer *renderer;
        TTF_Font *gFont;
        SDL_Texture * texture;

        SDL_Color color; //={0,0,255};

        SDL_Color setRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        {
            SDL_Color color;
            color.r = r;
            color.g = g;
            color.b = b;
            color.a = a;
            return color;
        }

        void setRect(int x, int y, int h, int w)
        {
            rect.x = x;
            rect.y = y;
            rect.h = h;
            rect.w = w;
        }
        void setRect(int h, int w)
        {
            rect.h = h;
            rect.w = w;
        }

        void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
        {
            color.r = r;
            color.g = g;
            color.b = b;
            color.a = a;
        }
        void renderWrap(const char * text)
        {
            if( texture != NULL )
            {
                SDL_DestroyTexture( texture );
                texture = NULL;
            }
            if(!(surface=TTF_RenderText_Blended_Wrapped(gFont,text,color,SCREEN_WIDTH - rect.x)))
            {
            //handle error here, perhaps print TTF_GetError at least
            }
            else {
            //SDL_BlitSurface(text_surface,NULL,rect,NULL);
            //perhaps we can reuse it, but I assume not for simplicity.

            texture = SDL_CreateTextureFromSurface(renderer, surface);
            }
            //SDL_Surface* textSurface = TTF_RenderText_Blended( gFont, textureText.c_str(), textColor );
            setRect(surface->h,surface->w);
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_FreeSurface( surface );
            //SDL_RenderPresent(renderer);
        }
        void render(char * text)
        {
            if( texture != NULL )
            {
                SDL_DestroyTexture( texture );
                texture = NULL;
            }
            if(!(surface=TTF_RenderText_Blended(gFont,text,color))) {
            //handle error here, perhaps print TTF_GetError at least
            } else {
            //SDL_BlitSurface(text_surface,NULL,rect,NULL);

            	texture = SDL_CreateTextureFromSurface(renderer, surface);
            }
            //SDL_Surface* textSurface = TTF_RenderText_Blended( gFont, textureText.c_str(), textColor );
            setRect(surface->h,surface->w);
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_FreeSurface( surface );
            //SDL_RenderPresent(renderer);
        }
        void render()
        {
            SDL_RenderCopy(renderer, texture, NULL, &rect);
        }
        void clean()
        {
            //SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }
};

enum StatusLabel
{
    stEmpty,
    stGoPress,
    stGoRelease,
    stIA,
    stIB,
    stOp,
    stHelp,
    stExit
};

#endif
