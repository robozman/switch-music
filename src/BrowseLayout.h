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

#ifndef BROWSELAYOUT_H
#define BROWSELAYOUT_H

#include <SDL2/SDL.h>

#define WIDTH 128
#define HEIGHT 72
#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define FPS 60
#define TEXT_SIZE 40

typedef struct BrowseLayout {
    int size;
    int element_height;
    int posititon_selected;
    SDL_Point* elements;
} BrowseLayout;

int create_BrowseLayout(BrowseLayout*);

int move_BrowseLayout_selection(BrowseLayout*, int);

#endif