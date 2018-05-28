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

#include "DirectoryStruct.h"

int create_sorted_DirectoryStruct_without_textures(DirectoryStruct* directory)
{
    directory->posititon_selected = 0;

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
    for (int i = 0; i < directory->size; i++) {
        // legibility variable
        DirectoryItem* current_element = (directory->contents + i);
        struct dirent* current_file_handle = readdir(directory_handle);
        
        if (current_file_handle == NULL) {
            return -1;
        }
        
        current_element->dirent_entry = *current_file_handle;
        //current_element->dirent_entry.d_name = current_element->dirent_entry.d_name;

        current_element->name_length = strlen(current_element->dirent_entry.d_name);
    }

    // cleanup
    closedir(directory_handle);
    //qsort(void *__base, size_t __nmemb, size_t __size, __compar_fn_t __compar)
    qsort(directory->contents, directory->size, sizeof(DirectoryItem), compare_DirectoryItem);

    return 0;
}


int populate_DirectoryStruct_textures(DirectoryStruct* directory,
    SDL_Color fg_color, SDL_Color bg_color, TTF_Font* font,
    SDL_Renderer* renderer)
{
    for (int i = 0; i < directory->size; i++) {
        // legibility variable
        DirectoryItem* current_element = (directory->contents + i);

        // as TTF_RenderText_Solid could only be used on SDL_Surface then you have
        // to create the surface first
        SDL_Surface* name_surface = TTF_RenderText_Shaded(font, 
            current_element->dirent_entry.d_name, fg_color, bg_color);
        //SDL_Surface* name_surface = TTF_RenderText_Solid(font, current_element->dirent_entry.d_name, color);

        // now you can convert it into a texture
        current_element->name_texture = SDL_CreateTextureFromSurface(renderer, name_surface);

        SDL_FreeSurface(name_surface);
    }

    return 0;
}

int move_DirectoryStruct_selection(DirectoryStruct* directory, int movement)
{
    if (movement < 0) { // if moving up
        
        // move if above element 0
        if (directory->posititon_selected > 0) {
            directory->posititon_selected--;
        }

    }
    else if (movement > 0) { // if moving down 

        // move if above element 0
        if (directory->posititon_selected  < (directory->size - 1)) {
            directory->posititon_selected++;
        }
    }

    return 0;
}