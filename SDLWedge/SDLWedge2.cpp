/**
 * SDLWedge2.cpp
 *
 * This is a GUI for Board2 simulator
 *
 * Masoud Ataei
 *
 */

#include <iostream>
#include <fstream>
#include <algorithm>

#include <sys/time.h>
#include <stdio.h>
//#include <unistd.h>
#include "SDLWedge.hpp"
#include <stdio.h>


SDL_Texture *ledOnTex;
SDL_Texture *ledOffTex;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;
SDL_Surface *textSurface;
SDL_Texture * textTexture;
SDL_Rect rect;
TTF_Font *gFont = NULL;
SDLText *txtTitle;
SDLText *txtFPS;
SDLText *txtValue;
SDLText *txtEdit;

bool isRunning;
char strTmp[70];
Uint32 frameStart;
int frameTime = 0;
int frameNumb = 0;
int FPSrate = 0;
int alphaled [8];
const int ledcount = 8;
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int led = 0;
int go=0,op=0,ia=0,ib=0;
int ival = 0;
int menu = stEmpty;
int valu = 0;
bool changed = false;
bool inpwrite = false;
void workonfiles();
void read_from_file();
void write_to_file();
std::string to_lower(std::string my_str);

void draw_rectangle(SDL_Surface* surface, int x, int y, int width, int height, int r, int g, int b);
void draw_circle(SDL_Surface* surface, int x, int y, int width, int height, int r, int g, int b);


int main(int argc, const char * argv[])
{
    init("Board2 GUI", SCREEN_WIDTH, SCREEN_HEIGHT);
    read_from_file();

    while(isRunning)
    {
        handleEvents();  //read keybourd inputs
        render();        //draw leds and texts
        workonfiles();   //read and write on files
    }
    clean();
    return 0;
}

void workonfiles()
{
    if(inpwrite )
    {
        if(menu == stIA) ia = ival;
        if(menu == stIB) ib = ival;
        if(menu == stOp) op = ival;
        if(!((menu == stGoPress) || (menu == stGoRelease))) menu = stEmpty;
        write_to_file();
        inpwrite  = false;
    }

    if(changed)
    {
        changed = false;
    }
    read_from_file();
}

//take out the hex value from the string
int value_line(std::string line)
{
    int st = line.find("0x");
    if(st < 0) return -1;
    std::string str = line.substr(st+2,line.size()-st);
    int res = std::strtoul(str.c_str(), NULL, 16);

    return res;
}

void read_from_file()
{
    std::ifstream myfile ("i_o_directory/b2_inputs.txt"); 
    std::string line = "";
    //read the input file line by line
    while ( std::getline(myfile, line) )
    {
        if(line.length()<3) break;
        line = to_lower(line); // modifies str to lower case
        int n = value_line(line); //get the value
        if (n < 0) break;//the value is invalid
        // check the proper parameter
        if(line.find("go:")!=std::string::npos) go = (n == 0) ? 1 : 0;
        if(line.find("op:")!=std::string::npos) op = n;
        if(line.find("ia:")!=std::string::npos) ia = ~n & 0x0FF;
        if(line.find("ib:")!=std::string::npos) ib = ~n & 0x0FF;
    }
    myfile.close();

    myfile.open("i_o_directory/b2_outputs.txt");
    while ( std::getline(myfile, line) )
    {
        if(line.length()<3) break;
        line = to_lower(line); // modifies str to lower case
        int n = value_line(line); //get the value
        if (n < 0) break;//the value is invalid
        // check the proper parameter
        if(line.find("leds")!=std::string::npos) led = ~n & 0x0FF;
    }
    myfile.close();
}

void write_to_file()
{
   // Create and open a text file
  std::ofstream MyFile("i_o_directory/b2_inputs.txt");
  char str[50];
  // Write to the file
  std::sprintf(str,"Go:0x%x\n", (go == 0) ? 1 : 0);
  std::sprintf(str + strlen(str),"Op:0x%x\n",op);
  std::sprintf(str + strlen(str),"IA:0x%hhx\n",~ia & 0x0FF);
  std::sprintf(str + strlen(str),"IB:0x%hhx",~ib & 0x0FF);
  MyFile <<  std::string( str )<< std::endl;
  // Close the file
  MyFile.close();
}

//initialize SDL 
bool init(const char* title, int w, int h)
{
    //int flags = 0;
    rect.h = SCREEN_WIDTH / 8;
    rect.w = rect.h;
    rect.y = 150;
    isRunning = false;
    if(SDL_Init(SDL_INIT_VIDEO) == 0)//SDL_INIT_EVERYTHING
    {
        std::cout << "Subsystem Initialised!..." << std::endl;
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w , h, SDL_WINDOW_SHOWN);
        if(window)
        {
            std::cout << "Windos Created." << std::endl;
        }
        else
        {
            std::cout << "Couldn't creat window:..." << std::endl;//SLD_GetError() << std::endl;
            return false;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if(renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer Created." << std::endl;
        }
        else
        {
            std::cout << "Couldn't creat renderer:..." << std::endl;//SLD_GetError() << std::endl;
            return false;
        }

         //Initialize SDL_ttf
        if( TTF_Init() == -1 )
        {
            std::cout << "Couldn't  initialize SDL_ttf." << TTF_GetError() << std::endl;
            return false;
        }
        else
        {
            gFont = TTF_OpenFont( "OpenSans-Regular.ttf", 28 );

            if( gFont == NULL )
            {
                std::cout << "Couldn't  open font: " << TTF_GetError() << std::endl;
                return false;
            }
        }

    }
    else
    {
        std::cout << "Unable to initialise SDL:..." << std::endl;//SLD_GetError() << std::endl;
        return false;
    }

    //initialize leds surfaces
    SDL_Surface *tmpSurface = IMG_Load("ledon.png");
    ledOnTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    tmpSurface = IMG_Load("ledoff.png");
    ledOffTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);

    if(!ledOnTex)
    {
        tmpSurface = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0);
        draw_circle(tmpSurface,0,0,100,100, 255,0,0);
        ledOnTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    }
    if(!ledOffTex)
    {
        tmpSurface = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0);
        draw_circle(tmpSurface,0,0,100,100, 99, 10, 10);
        ledOffTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    }
    SDL_SetTextureBlendMode(ledOnTex, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(tmpSurface);

    // initialize text location, colors, and where they are
    SDL_Color color = setRGBA(0,0,0,0);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    txtTitle = new SDLText(textSurface,renderer,gFont,textTexture, color);
    txtTitle->setRect(150, 50,100,200);
    txtFPS = new SDLText(textSurface,renderer,gFont,textTexture, color);
    txtFPS->setRect(10, 0,100,200);
    color = setRGBA(0,0,255,0);
    txtValue = new SDLText(textSurface,renderer,gFont,textTexture, color);
    txtValue ->setRect(50, 400,100,200);
    color = setRGBA(255,0,0,0);
    txtEdit  = new SDLText(textSurface,renderer,gFont,textTexture, color);
    txtEdit  ->setRect(50, 440,100,200);

    for(int i = 0; i < 8; i++)
        alphaled[i] = 0;

    isRunning = true;
    return true;
}

//draw shapes
void render()
{
    int tmpled = led & 0x0FF;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    for(int i=0; i<8; i++)
    {
        rect.x = i * rect.w;

        if(tmpled & 0x080)
        {
            alphaled[i] += 5;
            if(alphaled[i] > 255) alphaled[i] = 255;
        }
        else
        {
            alphaled[i] -= 1;
            if(alphaled[i] < 0) alphaled[i] = 0;
        }

        SDL_SetTextureAlphaMod(ledOnTex, (Uint8) alphaled[i]);
        SDL_RenderCopy(renderer, ledOffTex, NULL, &rect);
        SDL_RenderCopy(renderer, ledOnTex, NULL, &rect);

        tmpled = tmpled << 1;
    }
    sprintf(strTmp,"Board2 GUI with SDL");
    txtTitle->render(strTmp);

    sprintf(strTmp,"FPS: %d", FPSrate);
    txtFPS->render(strTmp);
    sprintf(strTmp,"Go=%s  Op=%d  IA=%d  IB=%d  LED=%d", (go == 0 ) ? "Released" : "Pressed", op, ia, ib, led);
    txtValue->render(strTmp);

    if( menu == stEmpty)
    {
        sprintf(strTmp,"  ");
    }
    else if( menu == stGoPress)
    {
        sprintf(strTmp,"Go Pressed");
    }
    else if( menu == stGoRelease)
    {
        sprintf(strTmp,"Go Released");
    }
    else if( menu == stIA)
    {
        sprintf(strTmp,"IA = %d",ival );
    }
    else if( menu == stIB)
    {
        sprintf(strTmp,"IB = %d",ival );
    }
    else if( menu == stOp)
    {
        sprintf(strTmp,"Op = %d",ival );
    }
    else if( menu == stExit )
    {
        sprintf(strTmp,"Quit ? (y / n)");
    }
    else
    {
        sprintf(strTmp,"  ");
    }
    txtEdit->render(strTmp);

    SDL_RenderPresent(renderer);

    if(frameNumb++>=99)
    {
        frameTime = (SDL_GetTicks() - frameStart);
        frameStart = SDL_GetTicks();
        FPSrate = frameTime;
        std::cout << ":> " << FPSrate << "   , takes " << frameTime  << "ms for " << frameNumb <<" frames"<< std::endl;
        frameNumb = 0;
    }
}

//get rid of allocated memory 
void clean()
{
    SDL_DestroyTexture(textTexture);
    SDL_DestroyTexture(ledOffTex);
    SDL_DestroyTexture(ledOnTex);
    SDL_FreeSurface(textSurface);
	TTF_CloseFont( gFont );
	gFont = NULL;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Program quit successfully." << std::endl;
}

void check_keyboard(SDL_Keycode key, bool *changed, bool *write)
{
    switch( key )
    {

        case SDLK_g:
            if(menu != stExit)
            {
                if(go == 0)
                {
                    go = 1;
                    menu = stGoPress;
                }
                else
                {
                    go = 0;
                    menu = stGoRelease;
                }
                *changed = true;
                *write = true;
            }
        break;

        case SDLK_a:
            if(menu != stExit) menu = stIA;
            ival = 0;
            *changed = true;
        break;

        case SDLK_b:
            if(menu != stExit) menu = stIB;
            ival = 0;
            *changed = true;
        break;

        case SDLK_o:
            if(menu != stExit) menu = stOp;
            ival = 0;
            *changed = true;
        break;
        case SDLK_l:
            if(menu != stExit) menu = stEmpty;
            ival = 0;
            *changed = true;
        break;


        case SDLK_RETURN:
        case SDLK_RETURN2:
        case SDLK_KP_ENTER:
            if(menu != stExit)
            {
                *changed = true;
                *write = true;
            }
        break;
        case SDLK_ESCAPE:
            if(menu == stIA || menu == stIB || menu == stOp || menu == stExit)
            {
                menu = stEmpty;
            }
            else
            {
                menu = stExit;
            }
        break;
        case SDLK_y:
            if(menu == stExit)
            {
                quit();
            }
        break;
        case SDLK_n:
            menu = stEmpty;
        break;
        case SDLK_0:
        case SDLK_1:
        case SDLK_2:
        case SDLK_3:
        case SDLK_4:
        case SDLK_5:
        case SDLK_6:
        case SDLK_7:
        case SDLK_8:
        case SDLK_9:
            ival = ival * 10 + (key -48);
            if(ival>255 || ival<0) ival = 0;
            if(!(menu == stIA || menu == stIB || menu == stOp)) ival = 0;
            *changed = true;
        break;
        case SDLK_KP_0:
        case SDLK_KP_1:
        case SDLK_KP_2:
        case SDLK_KP_3:
        case SDLK_KP_4:
        case SDLK_KP_5:
        case SDLK_KP_6:
        case SDLK_KP_7:
        case SDLK_KP_8:
        case SDLK_KP_9:

            ival = ival * 10 + (int) ( (key - SDLK_KP_1 + 1) % 10);
            if(ival>255 || ival<0) ival = 0;
            if(!(menu == stIA || menu == stIB || menu == stOp)) ival = 0;
            *changed = true;
        break;
        case SDLK_DELETE:
        case SDLK_BACKSPACE:
            if(menu == stIA || menu == stIB || menu == stOp) ival = ival / 10;
            *changed = true;
        break;

        default:
            if(menu != stExit)
            {
                ival = 0;
                menu = stEmpty;
                *changed = true;
            }
        break;
    }
}
