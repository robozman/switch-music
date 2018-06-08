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

#include "BrowseLayout.h"

int create_BrowseLayout(BrowseLayout* browse_layout)
{
    browse_layout->in_focus = true;
    browse_layout->posititon_selected = 0;

    // amount of elements displayed on the screen
    // subtract 2 for header bar and bottom bar
    browse_layout->size = (WIN_HEIGHT / browse_layout->element_height) - 2;

    // allocate array of points corresponding to top-left cornet of elements
    browse_layout->elements = malloc(browse_layout->size * sizeof(SDL_Point));
    if (browse_layout->elements == NULL) {
        return -1;
    }

    for (int i = 0; i < browse_layout->size; i++) {
        browse_layout->elements[i] = (SDL_Point){ .x = 40, /* indent by 40*/ .y = (i + 1) * browse_layout->element_height };
    }

    return 0;
}

int move_BrowseLayout_selection(BrowseLayout* browse_layout, int movement, int total_elements)
{

    if (movement < 0) { // if moving up
        
        // move if below top item
        if (browse_layout->posititon_selected > 0) {
            browse_layout->posititon_selected--;
        }

    }
    else if (movement > 0) { // if moving down 
        // move if above bottom item and above last rendered_element
        if ((browse_layout->posititon_selected < (browse_layout->size - 1)) && (browse_layout->posititon_selected < (total_elements - 1))) {
            browse_layout->posititon_selected++;
        }

    }

    return 0;
}

int cleanup_BrowseLayout(BrowseLayout* browse_layout)
{
    free(browse_layout->elements);

    return 0;
}