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

//static bool current_directary_rendered = false;
//static bool current_screen_rendered = false;

typedef struct directory_item
{
    char *name; // should point to dirent_entry.d_name
    struct dirent dirent_entry;
    SDL_Texture *name_texture; 
} directory_item;

typedef struct directory_struct
{
	char path[4096]; 
	directory_item* contents;
	uint16_t size;
} directory_struct;

void cleanup_application(SDL_Renderer*, SDL_Window*);

int setup_display();

/* 
	int create_directory_structure(directory_struct*)
	pass in a directory_struct* with the path member already filled out
	struct will be fully filled out upon return
	contents member of struct must be freed
*/
int create_directory_structure(directory_struct*);

int populate_directory_array_textures(directory_struct*, SDL_Color, TTF_Font*);

void render_directory();



int main()
{

    uint8_t error = 0;
    /*
	//int i;
	uint32_t framebuf[WIDTH * HEIGHT]40;
	//memset((void *)framebuf, 0, sizeof(uint32_t) * WIDTH * HEIGHT);
	
	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		framebuf[i] = 0xFFFF0000;//RGBA8_MAXALPHA(255, 0, 0);
		//memset((void*)framebuf, 0xFFFF0000, HEIGHT * sizeof(uint32_t));
	}
   
	*/

    /*-------------Setup SDL-------------------*/
    SDL_Init(SDL_INIT_EVERYTHING);

    //Setup window
    SDL_Window* window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (!window) {
        SDL_Quit();
    }

    //Setup renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        SDL_Quit();
    }

    /*
	SDL_Texture * texture  = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	if (texture == NULL) {
			fprintf(stderr, "Failed CreateTexture: %s\n", SDL_GetError());
			return 1;
	}
	*/

    /*----------------Setup Font---------------*/
    TTF_Init();
    // load TTF storad inside binary
    TTF_Font* inconsolata_regular = TTF_OpenFontRW(SDL_RWFromMem((void*)Inconsolata_Regular_ttf, Inconsolata_Regular_ttf_size), 1, 14);
    SDL_Color white = { 255, 255, 255 };
    SDL_Color black = { 0, 0, 0 };

    // create string to display
    //size_t needed = snprintf(NULL, 0, "Directory listing for \"%s\"", "/") + 1;
    //char *title_message = malloc(needed);
    //snprintf(title_message, needed, "Directory listing for \"%s\"", "/");

    /*
    char title_message[] = "Directory listing for \"/\"";

    // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    SDL_Surface* surface_message = TTF_RenderText_Solid(inconsolata_regular, title_message, White);

    //now you can convert it into a texture
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surface_message);

    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 0; //controls the rect's x coordinate
    Message_rect.y = 0; // controls the rect's y coordinte
    Message_rect.h = TEXT_SIZE; // controls the width of the rect
    Message_rect.w = strlen(title_message) * TEXT_SIZE; // controls the height of the rect

	*/

	/*--------------Default Screen-------------*/
    directory_struct current_directory;
    
    error = create_directory_structure(&current_directory);
    if (error != 0) {
    	cleanup_application(SDL_Renderer *renderer, SDL_Window *window);
        return error;
    }

    error = populate_directory_array_textures(&current_directory, white, inconsolata_regular);



    while (appletMainLoop()) {
        hidScanInput();

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
				framebuf[i] = current_RGBA;//RGBA8_MAXALPHA(255, 0, 0);
				//memset((void*)framebuf, 0xFFFF0000, HEIGHT * sizeof(uint32_t));
			}	
			*/

        /* Update the texture and render it. */
        //SDL_UpdateTexture(texture, NULL, framebuf, WIDTH * sizeof(framebuf[0]));
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
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
    //SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

int create_directory_array(directory_struct* directory)
{
    DIR* directory_handle;
    struct dirent* file_handle;

    // Open directory 
    directory_handle = opendir(directory->path); 
    if (directory_handle == NULL) {
    	// return 1 if opendir() failed
		return 1;
    }

    // Count the amount of elements in the directory
    for (; readdir(directory_handle); directory->size++)
        ;

    // allocate memory for array based on the amount of items
    directory->contents = malloc(directory->size * sizeof(directory_item));


    // iterate through the items and copy their data into the array
    for (uint16_t i = 0; i < directory->size; i++) {
    	// legibility variable
    	directory_item* current_element = (directory->contents + i);
    	struct dirent *current_file_handle = readdir(directory_handle);

    	current_element->dirent_entry = *current_file_handle;
    	current_element->name = current_element->dirent_entry.d_name;
    }

    // cleanup
    closedir(directory_handle);

    return 0;
}