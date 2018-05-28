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

#include "Render.h"

int render_DirectoryStruct_using_BrowseLayout(SDL_Renderer* renderer, 
    DirectoryStruct* directory, BrowseLayout* browse_layout)
{
    // pick the smaller of the two sizes
    int elements_to_render = directory->size < browse_layout->size ? directory->size : browse_layout->size;
    int starting_element = 0;
    /*  starting DirectoryItem calculation, enables scrolling down to see more items
        if posititon in directory is less then max elements displayed
            start at element 0
        if postiton in directory is greatr then max elements
            start at element posititon - size
    */
    if (directory->posititon_selected <= browse_layout->size) {
        starting_element = 0;
    }
    else if (directory->posititon_selected > browse_layout->size) {
        starting_element = directory->posititon_selected - browse_layout->size;
    }



    // genereate destination rectanges based on string length and render to them
    for (int i = 0; i < elements_to_render; i++) {

        int w, h;

        // get width and height of font texture, add starting offset
        SDL_QueryTexture(directory->contents[i + starting_element].name_texture, NULL, NULL, &w, &h);

        // destination rectangle
        SDL_Rect text_destinaton = (SDL_Rect){
            // x and y values come frome browse layout elements
            // if element is selecetd, indent by 10
            .x = i == browse_layout->posititon_selected ? browse_layout->elements[i].x + 10 : browse_layout->elements[i].x, 
            .y = browse_layout->elements[i].y, 
            // w and h from font texture
            .w = w,
            .h = h,
            /*
            .w=browse_layout->element_height * (directory->contents[i].name_length - 1), 
            .h=browse_layout->element_height,
            */
        };
        SDL_RenderCopy(renderer, directory->contents[i + starting_element].name_texture, NULL, &text_destinaton);
        
    }

    // render selected identifier
    SDL_Rect selected_identifier = (SDL_Rect){
        .x = browse_layout->elements[browse_layout->posititon_selected].x,
        .y = browse_layout->elements[browse_layout->posititon_selected].y + 10,
        .w = 5,
        .h = browse_layout->element_height - 10,
    };
    SDL_SetRenderDrawColor(renderer, 250, 250, 250, 1);
    SDL_RenderFillRect(renderer, &selected_identifier);


    return 0;
}