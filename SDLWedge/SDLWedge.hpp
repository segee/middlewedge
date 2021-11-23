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
extern SDL_Surface *textSurface;
extern SDL_Texture * textTexture;
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
        SDLText(SDL_Surface * _Surface, SDL_Renderer *_renderer, TTF_Font *_gFont, SDL_Texture * _texture, SDL_Color _color)
        {
            textSurface = _Surface;
            renderer = _renderer;
            gFont = _gFont;
            texture = _texture;
            color = _color;
            texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        }

        ~SDLText();

        SDL_Surface *textSurface;
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

        void render(char * text)
        {
            if( texture != NULL )
            {
                SDL_DestroyTexture( texture );
                texture = NULL;
            }
            if(!(textSurface=TTF_RenderText_Blended(gFont,text,color))) {
            //handle error here, perhaps print TTF_GetError at least
            } else {
            //SDL_BlitSurface(text_surface,NULL,rect,NULL);
            //perhaps we can reuse it, but I assume not for simplicity.
            texture = SDL_CreateTextureFromSurface(renderer, textSurface);
            }
            //SDL_Surface* textSurface = TTF_RenderText_Blended( gFont, textureText.c_str(), textColor );
            setRect(textSurface->h,textSurface->w);
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_FreeSurface( textSurface );
            //SDL_RenderPresent(renderer);
        }
        void render()
        {
            if(!(textSurface=TTF_RenderText_Solid(gFont,text,color))) {
            //handle error here, perhaps print TTF_GetError at least
            } else {
            //SDL_BlitSurface(text_surface,NULL,rect,NULL);
            //perhaps we can reuse it, but I assume not for simplicity.
                texture = SDL_CreateTextureFromSurface(renderer, textSurface);
            }
            //SDL_Surface* textSurface = TTF_RenderText_Blended( gFont, textureText.c_str(), textColor );

            SDL_RenderCopy(renderer, texture, NULL, &rect);

            SDL_RenderPresent(renderer);
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
    stExit
};

#endif
