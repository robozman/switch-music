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

#define WIDTH 128
#define HEIGHT 72
#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define FPS 60
#define TEXT_SIZE 40

static char default_directory_path[] = "/";

// static bool current_directary_rendered = false;
// static bool current_screen_rendered = false;

typedef struct DirectoryItem {
    char* name; // should point to dirent_entry.d_name
    uint16_t name_length;
    struct dirent dirent_entry;
    SDL_Texture* name_texture;
} DirectoryItem;

typedef struct DirectoryStruct {
    char path[256];
    DirectoryItem* contents;
    uint16_t size;
} DirectoryStruct;

typedef struct BrowseLayout {
    uint16_t size;
    uint16_t element_height;
    SDL_Point* elements;
} BrowseLayout;

void cleanup_application(SDL_Renderer*, SDL_Window*);

int setup_display();

/*
        int create_DirectoryStructure(DirectoryStruct*)
        pass in a DirectoryStruct* with the path member already filled out
        struct will be fully filled out upon return
        contents member of struct must be freed
*/
int create_DirectoryStructure_without_textures(DirectoryStruct*);

int populate_DirectoryStructure_textures(DirectoryStruct*, SDL_Color, TTF_Font*,
    SDL_Renderer*);

int create_BrowseLayout(BrowseLayout*);

int create_queue_layout(SDL_Renderer*);

// fill out element_height member of BrowseLayout struct before calling
int render_DirectoryStructure_using_BrowseLayout(SDL_Renderer*, DirectoryStruct*, BrowseLayout*);

int main()
{
    int8_t error = 0;


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

    /*
      SDL_Texture * texture  = SDL_CreateTexture(renderer,
     SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT); if
     (texture == NULL) { fprintf(stderr, "Failed CreateTexture: %s\n",
     SDL_GetError()); return 1;
      }
      */

    /*----------------Setup Font---------------*/
    TTF_Init();
    // load TTF storad inside binary
    TTF_Font* inconsolata_regular = TTF_OpenFontRW(SDL_RWFromMem((void*)Inconsolata_Regular_ttf,
                                                       Inconsolata_Regular_ttf_size),
        1, 40);
    SDL_Color white = { 255, 255, 255 };
    SDL_Color black = { 0, 0, 0 };

    char title_message[] = "Switch Music";

    // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    SDL_Surface* surface_message = TTF_RenderText_Solid(inconsolata_regular, title_message, white);
    SDL_FreeSurface(surface_message);

    /*
  char title_message[] = "Directory listing for \"/\"";

  // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to
  create the surface first SDL_Surface* surface_message =
  TTF_RenderText_Solid(inconsolata_regular, title_message, White);

  //now you can convert it into a texture
  SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer,
  surface_message);

  SDL_Rect Message_rect; //create a rect
  Message_rect.x = 0; //controls the rect's x coordinate
  Message_rect.y = 0; // controls the rect's y coordinte
  Message_rect.h = TEXT_SIZE; // controls the width of the rect
  Message_rect.w = strlen(title_message) * TEXT_SIZE; // controls the height of
  the rect

      */

    /*--------------Default Screen-------------*/
    DirectoryStruct current_directory;

    memset(current_directory.path, 0, 256);
    snprintf(current_directory.path, 256, "%s", default_directory_path);

    error = create_DirectoryStructure_without_textures(&current_directory);
    if (error != 0) {
        cleanup_application(renderer, window);
        return error;
    }

    error = populate_DirectoryStructure_textures(&current_directory, white,
        inconsolata_regular, renderer);
    if (error != 0) {
        cleanup_application(renderer, window);
        return error;
    }

    BrowseLayout browse_layout;

    browse_layout.element_height = 40;

    error = create_BrowseLayout(&browse_layout);
    if (error != 0) {
        cleanup_application(renderer, window);
        return error;
    }

    while (appletMainLoop()) {
        hidScanInput();

        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) break; 

        /*
                    static uint8_t current_color = 0;
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
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        render_DirectoryStructure_using_BrowseLayout(renderer, &current_directory, &browse_layout);
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

void cleanup_application(SDL_Renderer* renderer, SDL_Window* window)
{
    // SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

int create_DirectoryStructure_without_textures(DirectoryStruct* directory)
{
    DIR* directory_handle;

    // Open directory
    directory_handle = opendir(directory->path);
    if (directory_handle == NULL) {
        // return 1 if opendir() failed
        return -1;
    }

    // Count the amount of elements in the directory
    directory->size = 0;
    for (; readdir(directory_handle); directory->size++)
        ;

    // rewind so we can read the contents again
    // rewinddir doesn't work on libnx
    //rewinddir(directory_handle);
    closedir(directory_handle);
    opendir(directory->path);

    // allocate memory for array based on the amount of items
    directory->contents = malloc(directory->size * sizeof(DirectoryItem));
    if (directory->contents == NULL) {
        return -1;
    }

    // iterate through the items and copy their data into the array
    for (uint16_t i = 0; i < directory->size; i++) {
        // legibility variable
        DirectoryItem* current_element = (directory->contents + i);
        struct dirent* current_file_handle = readdir(directory_handle);
        
        if (current_file_handle == NULL) {
            return -1;
        }
        
        current_element->dirent_entry = *current_file_handle;
        current_element->name = current_element->dirent_entry.d_name;

        current_element->name_length = strlen(current_element->name);
    }

    // cleanup
    closedir(directory_handle);

    return 0;
}

int populate_DirectoryStructure_textures(DirectoryStruct* directory,
    SDL_Color color, TTF_Font* font,
    SDL_Renderer* renderer)
{
    for (uint16_t i = 0; i < directory->size; i++) {
        // legibility variable
        DirectoryItem* current_element = (directory->contents + i);

        // as TTF_RenderText_Solid could only be used on SDL_Surface then you have
        // to create the surface first
        SDL_Surface* name_surface = TTF_RenderText_Solid(font, current_element->name, color);

        // now you can convert it into a texture
        current_element->name_texture = SDL_CreateTextureFromSurface(renderer, name_surface);

        SDL_FreeSurface(name_surface);
    }

    return 0;
}

int create_BrowseLayout(BrowseLayout* browse_layout)
{
    // amount of elements displayed on the screen
    browse_layout->size = WIN_HEIGHT / browse_layout->element_height;

    // allocate array of points corresponding to top-left cornet of elements
    browse_layout->elements = malloc(browse_layout->size * sizeof(SDL_Point));
    if (browse_layout->elements == NULL) {
        return -1;
    }

    for (uint16_t i = 0; i < browse_layout->size; i++) {
        browse_layout->elements[i] = (SDL_Point){ .x = 0, .y = i * browse_layout->element_height };
    }

    return 0;
}

int render_DirectoryStructure_using_BrowseLayout(SDL_Renderer* renderer, 
    DirectoryStruct* directory, BrowseLayout* browse_layout)
{
    // pick the smaller of the two sizes
    uint16_t interations = directory->size < browse_layout->size ? directory->size : browse_layout->size;

    for (uint16_t i = 0; i < interations; i++) {

        SDL_Rect text_destinaton = (SDL_Rect){
            .x=browse_layout->elements[i].x, 
            .y=browse_layout->elements[i].y, 
            .w=browse_layout->element_height * (directory->contents[i].name_length - 1), 
            .h=browse_layout->element_height,
        };
        SDL_RenderCopy(renderer, directory->contents[i].name_texture, NULL, &text_destinaton);
        
    }

    return 0;
}