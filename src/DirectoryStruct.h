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

#ifndef DIRECTORYSTRUCT_H
#define DIRECTORYSTRUCT_H

#include <dirent.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "DirectoryItem.h"
#include "BrowseLayout.h"

typedef struct DirectoryStruct {
    char path[256];
    DirectoryItem* contents;
    int size;
    int posititon_selected;
} DirectoryStruct;

/*
        int create_DirectoryStruct(DirectoryStruct*)
        pass in a DirectoryStruct* with the path member already filled out
        struct will be fully filled out upon return
        contents member of struct must be freed
*/
int create_sorted_DirectoryStruct_without_textures(DirectoryStruct*);

int populate_DirectoryStruct_textures(DirectoryStruct*, SDL_Color, SDL_Color,
    TTF_Font*, SDL_Renderer*);

int move_DirectoryStruct_selection(DirectoryStruct*, int);

int cleanup_DirectoryStruct(DirectoryStruct*);

#endif