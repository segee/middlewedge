#ifndef SDLFuncs_hpp
#define SDLFuncs_hpp

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include "font.h"
#include <dirent.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define CH_Spaace  2
#define Line_Space 3
#define WCH_Pixle  8
#define HCH_Pixle  13
#define W_SPACE_Pixle  2
#define H_SPACE_Pixle  5
//#define Font_Size  1.6





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
extern SDL_Texture *fontTex;


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
    SDL_LockSurface(surface);
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

    SDL_LockSurface(surface);
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

void draw_font(SDL_Surface* surface, int x, int y, int width, int height)
{
    //SDL_LockSurface(surface);
    std::vector<uint8_t> pixels(surface->h * surface->pitch, 0);

    int dy, dx, dc;
    char ch = 0;

    SDL_LockSurface(surface);
    for (dc = 0; dc < 95; dc++) {
        for (dy = 0; dy < HCH_Pixle; dy++) {
            ch = fontArray[dc][HCH_Pixle - 1 - dy];
            //std::cout << (int)ch << std::endl;
            for (dx = 0; dx < WCH_Pixle; dx ++) {
                if(ch & 0x80)
                {
                    // black
                    pixels[(dx + 8 * dc) *4 + (dy * surface->pitch)] = 0;
                    pixels[(dx + 8 * dc) *4 + (dy * surface->pitch) + 1] = 0;
                    pixels[(dx + 8 * dc) *4 + (dy * surface->pitch) + 2] = 0;
                    pixels[(dx + 8 * dc) *4 + (dy * surface->pitch) + 3] = 0;
                    // blue
                    pixels[(dx + 8 * dc) *4 + ((dy + 13) * surface->pitch)] = 255;
                    pixels[(dx + 8 * dc) *4 + ((dy + 13) * surface->pitch) + 1] = 0;
                    pixels[(dx + 8 * dc) *4 + ((dy + 13) * surface->pitch) + 2] = 0;
                    pixels[(dx + 8 * dc) *4 + ((dy + 13) * surface->pitch) + 3] = 0;
                    // red
                    pixels[(dx + 8 * dc) *4 + ((dy + 26) * surface->pitch)] = 0;
                    pixels[(dx + 8 * dc) *4 + ((dy + 26) * surface->pitch) + 1] = 0;
                    pixels[(dx + 8 * dc) *4 + ((dy + 26) * surface->pitch) + 2] = 255;
                    pixels[(dx + 8 * dc) *4 + ((dy + 26) * surface->pitch) + 3] = 0;
                }
                else
                {
                    // black
                    pixels[(dx + 8 * dc) *4 + (dy * surface->pitch)] = 255;
                    pixels[(dx + 8 * dc) *4 + (dy * surface->pitch) + 1] = 255;
                    pixels[(dx + 8 * dc) *4 + (dy * surface->pitch) + 2] = 255;
                    pixels[(dx + 8 * dc) *4 + (dy * surface->pitch) + 3] = 255;
                    // blue
                    pixels[(dx + 8 * dc) *4 + ((dy + 13) * surface->pitch)] = 255;
                    pixels[(dx + 8 * dc) *4 + ((dy + 13) * surface->pitch) + 1] = 255;
                    pixels[(dx + 8 * dc) *4 + ((dy + 13) * surface->pitch) + 2] = 255;
                    pixels[(dx + 8 * dc) *4 + ((dy + 13) * surface->pitch) + 3] = 255;
                    // red
                    pixels[(dx + 8 * dc) *4 + ((dy + 26) * surface->pitch)]     = 255;
                    pixels[(dx + 8 * dc) *4 + ((dy + 26) * surface->pitch) + 1] = 255;
                    pixels[(dx + 8 * dc) *4 + ((dy + 26) * surface->pitch) + 2] = 255;
                    pixels[(dx + 8 * dc) *4 + ((dy + 26) * surface->pitch) + 3] = 255;
                }

                ch = ch << 1;

            }
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

void try_open_ttf_font(void)
{
    DIR *p;
    struct dirent *pp;
    p = opendir ("./");

    if (p != NULL)
    {
        while ((pp = readdir (p))!=NULL)
        {
            int length = strlen(pp->d_name);
            if (strncmp(pp->d_name + length - 4, ".ttf", 4) == 0)
            {
                gFont = TTF_OpenFont( pp->d_name, 28 );
                if( gFont == NULL )
                {
                    std::cout << "Couldn't  open font: " << TTF_GetError() << std::endl;
                    //return false;
                }
                else
                {
                    std::cout << "font: " << pp->d_name << " opened" << std::endl;
                    (void) closedir (p);
                    return;
                }
            }
        }

    (void) closedir (p);
    }
}

void analize_text(const char * text, int * oline, int * wlen)
{
    int line = 1;
    float witdh = 1;
    float temp = 0;
    for(int i = 0; i < strlen(text);i++ )
    {

        if(text[i] == '\n')
        {
            line++;
            temp = 0;
        }
        else
        {
            temp++;
            if(temp > witdh)
            {
                witdh = temp;
            }
        }
    }
    *oline = line;
    *wlen = witdh;
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
            if(gFont == NULL)
            {
                float Font_Size = 1.6;
                int c = 0;
                if(color.b == 255) c = 1;
                else if(color.r == 255) c = 2;

                int wch = Font_Size * WCH_Pixle + W_SPACE_Pixle;
                int hch = Font_Size * HCH_Pixle + Line_Space;

                int line  = 1;
                int wlen = 1;
                analize_text(text, &line, &wlen);

                int wTex = wch * wlen;
                int hTex = hch * line;
                //SDL_Surface *tmpSurface = SDL_CreateRGBSurface(0, w,h, 32, 0, 0, 0, 0);
                //texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

                texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET , wTex, hTex);

                SDL_SetRenderTarget(renderer , texture);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderClear(renderer);
                //SDL_RenderCopy(renderer, fontTex, NULL, NULL);

                //texture = fontTex;

                SDL_Rect rectCH, rectTex;
                rectTex.x = 0;
                rectTex.y = 0;
                rectTex.h = Font_Size * HCH_Pixle;
                rectTex.w = Font_Size * WCH_Pixle;

                rectCH.x = 0;
                rectCH.y = HCH_Pixle * c;
                rectCH.h = HCH_Pixle;
                rectCH.w = WCH_Pixle;

                char ch = 'A';
                for(int i = 0; i < strlen(text); i++)
                {
                    ch = text[i];
                    if( ch == '\n')
                    {
                        rectTex.x = 0;
                        rectTex.y += hch;
                    }
                    else if(ch >= 32 && ch < 127)
                    {
                        rectCH.x = WCH_Pixle * (ch - ' ');
                        SDL_RenderCopy(renderer, fontTex, &rectCH, &rectTex);
                        rectTex.x += rectTex.w + W_SPACE_Pixle;
                    }
                    else
                    {
                        rectTex.x += rectTex.w + W_SPACE_Pixle;
                    }

                }

                rect.w = wTex;
                rect.h = hTex;

                SDL_SetRenderTarget(renderer, NULL);
                //renderer = SDL_CreateRenderer(window, -1, 0);
                SDL_RenderCopy(renderer, texture, NULL, &rect);

                //SDL_FreeSurface(tmpSurface);

            }
            else
            {
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
        }
        void render(char * text)
        {
            if(gFont == NULL)
            {
                float Font_Size = 2;
                int c = 0;
                if(color.b == 255) c = 1;
                else if(color.r == 255) c = 2;

                int wch = Font_Size * WCH_Pixle + W_SPACE_Pixle;
                int hch = Font_Size * HCH_Pixle + Line_Space;

                int wTex = wch * strlen(text);;
                int hTex = hch ;
                //SDL_Surface *tmpSurface = SDL_CreateRGBSurface(0, w,h, 32, 0, 0, 0, 0);
                //texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

                texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET , wTex, hTex);

                SDL_SetRenderTarget(renderer , texture);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderClear(renderer);


                SDL_Rect rectCH, rectTex;
                rectTex.x = 0;
                rectTex.y = 0;
                rectTex.h = Font_Size * HCH_Pixle;
                rectTex.w = Font_Size * WCH_Pixle;

                rectCH.x = 0;
                rectCH.y = HCH_Pixle * c;
                rectCH.h = HCH_Pixle;
                rectCH.w = WCH_Pixle;

                char ch = 'A';
                for(int i = 0; i < strlen(text); i++)
                {
                    ch = text[i];
                    if(ch >= ' ' && ch < 127)
                    {
                        rectCH.x = WCH_Pixle * (ch - ' ');
                        SDL_RenderCopy(renderer, fontTex, &rectCH, &rectTex);
                    }
                    rectTex.x += rectTex.w + W_SPACE_Pixle;
                }

                rect.w = wTex;
                rect.h = hTex;


                SDL_SetRenderTarget(renderer, NULL);
                //renderer = SDL_CreateRenderer(window, -1, 0);
                SDL_RenderCopy(renderer, texture, NULL, &rect);

                //SDL_FreeSurface(tmpSurface);


            }
            else
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
