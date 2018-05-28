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

#include "QueueLayout.h"

int create_QueueLayout(QueueLayout* queue_layout)
{
    queue_layout->posititon_selected = 0;

    // amount of elements displayed on the screen
    // subtract 2 for header bar and bottom bar
    queue_layout->size = (WIN_HEIGHT / queue_layout->element_height) - 2;

    // allocate array of points corresponding to top-left cornet of elements
    queue_layout->elements = malloc(queue_layout->size * sizeof(SDL_Point));
    if (queue_layout->elements == NULL) {
        return -1;
    }

    for (int i = 0; i < queue_layout->size; i++) {
        queue_layout->elements[i] = (SDL_Point){ .x = 40, /* indent by 40*/ .y = (i + 1) * queue_layout->element_height };
    }

    return 0;

}