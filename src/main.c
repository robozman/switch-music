/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <arpa/inet.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_ttf.h>
#include <switch.h>

#include <Inconsolata_Regular_ttf.h>
#include <Roboto_Medium_ttf.h>
#include <Roboto_Regular_ttf.h>


#define WIDTH 128
#define HEIGHT 72
#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define FPS 60
#define TEXT_SIZE 40

#include "DirectoryItem.h"
#include "DirectoryStruct.h"
#include "BrowseLayout.h"
#include "QueueLayout.h"
#include "Render.h"
#include "Cleanup.h"

// choose which font to use
//#define INCONSOLATA_REGULAR
#define ROBOTO_REGULAR
//#define ROBOTO_MEDIUM

static char default_directory_path[] = "/";

// static bool current_directary_rendered = false;
// static bool current_screen_rendered = false;


int main()
{
    int error = 0;


    /*-------------Setup SDL-------------------*/
    SDL_Init(SDL_INIT_EVERYTHING);

    // Setup window
    SDL_Window* window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0,
        0, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (!window) {
        SDL_Quit();
    }

    // Setup renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        SDL_Quit();
    }


    /*----------------Setup Font---------------*/
    TTF_Init();
    // load TTF storad inside binary based on font define
    #if defined(INCONSOLATA_REGULAR)
    TTF_Font* inconsolata_regular = TTF_OpenFontRW(SDL_RWFromMem((void*)Inconsolata_Regular_ttf,
                                                       Inconsolata_Regular_ttf_size), 1, 40);
    #elif defined(ROBOTO_REGULAR)
    TTF_Font* roboto_regular = TTF_OpenFontRW(SDL_RWFromMem((void*)Roboto_Regular_ttf,
                                                       Roboto_Regular_ttf_size), 1, 40);
    #elif defined(ROBOTO_MEDIUM)
    TTF_Font* roboto_medium = TTF_OpenFontRW(SDL_RWFromMem((void*)Roboto_Medium_ttf,
                                                       Roboto_Medium_ttf_size), 1, 40);
    #endif

    // create color
    SDL_Color white = { 250,  250, 250 };
    SDL_Color material_grey = {  66,  66,   66 };

    // the program will crash without the following, explain to me why plz
    char title_message[] = "Switch Music"; 
    #if defined(INCONSOLATA_REGULAR)
    SDL_Surface* surface_message = TTF_RenderText_Solid(inconsolata_regular, title_message, white);

    #elif defined(ROBOTO_REGULAR)
    SDL_Surface* surface_message = TTF_RenderText_Solid(roboto_regular, title_message, white);

    #elif defined(ROBOTO_MEDIUM)
    SDL_Surface* surface_message = TTF_RenderText_Solid(roboto_medium, title_message, white);

    #endif

    SDL_FreeSurface(surface_message);


    /*--------------Default Screen-------------*/
    DirectoryStruct current_directory;

    memset(current_directory.path, 0, 256);
    snprintf(current_directory.path, 256, "%s", default_directory_path);

    error = create_sorted_DirectoryStruct_without_textures(&current_directory);
    if (error != 0) {
        cleanup_application(renderer, window);
        return error;
    }


    // ifdef based on font choice
    #if defined(INCONSOLATA_REGULAR)
    error = populate_DirectoryStruct_textures(&current_directory, white,
    material_grey, inconsolata_regular, renderer);
    #elif defined(ROBOTO_REGULAR)
    error = populate_DirectoryStruct_textures(&current_directory, white,
    material_grey, roboto_regular, renderer);
    #elif defined(ROBOTO_MEDIUM)
    error = populate_DirectoryStruct_textures(&current_directory, white,
    material_grey, roboto_medium, renderer);
    #endif

    if (error != 0) {
        cleanup_application(renderer, window);
        return error;
    }

    BrowseLayout browse_layout;

    browse_layout.element_height = 50;

    error = create_BrowseLayout(&browse_layout);
    if (error != 0) {
        cleanup_application(renderer, window);
        return error;
    }

    QueueLayout queue_layout;

    queue_layout.element_height = 50;

    error = create_QueueLayout(&queue_layout);
    if (error != 0) {
        cleanup_application(renderer, window);
        return error;
    }


    while (appletMainLoop()) {
        hidScanInput();

        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) break; 

        if (kDown & KEY_DOWN) {
            move_BrowseLayout_selection(&browse_layout, 1);
            move_DirectoryStruct_selection(&current_directory, 1);
        }

        /*
                    static int current_color = 0;
                    uint32_t current_RGBA = RGBA8_MAXALPHA(0, 0, 0);
                    switch (current_color) {
                            case 0:
                                    current_RGBA = RGBA8_MAXALPHA(255, 0, 0);
                                    current_color = 1;
                                    break;
                            case 1:
                                    current_RGBA = RGBA8_MAXALPHA(255, 255, 0);
                                    current_color = 2;
                                    break;
                            case 2:
                                    current_RGBA = RGBA8_MAXALPHA(0, 0, 255);
                                    current_color = 0;
                                    break;
                            default:
                                    break;
                    }

                    for (int i = 0; i < WIDTH * HEIGHT; i++) {
                            framebuf[i] = current_RGBA;//RGBA8_MAXALPHA(255, 0,
       0);
                            //memset((void*)framebuf, 0xFFFF0000, HEIGHT *
       sizeof(uint32_t));
                    }
                    */

        /* Update the texture and render it. */
        // SDL_UpdateTexture(texture, NULL, framebuf, WIDTH * sizeof(framebuf[0]));
        SDL_SetRenderDrawColor(renderer, 66, 66, 66, 1);
        SDL_RenderClear(renderer);
        render_DirectoryStruct_using_BrowseLayout(renderer, &current_directory, &browse_layout);
        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / FPS);
    }

    cleanup_application(renderer, window);

    /*
      SDL_DestroyTexture(texture);
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);

      SDL_Quit();
      */
    return 0;
}