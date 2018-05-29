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

#include "Cleanup.h"

void cleanup_application(SDL_Renderer* renderer, SDL_Window* window, DirectoryStruct* directory_struct, BrowseLayout* browse_layout, QueueLayout* queue_layout)
{
    int error = 0;


    if (directory_struct != NULL) {
        error = cleanup_DirectoryStruct(directory_struct);
        if (error != 0) {
            exit(error);
        }
    }
    
    if (directory_struct != NULL) {
        error = cleanup_BrowseLayout(browse_layout);
        if (error != 0) {
            exit(error);
        }
    }
    
    if (directory_struct != NULL) {
        error = cleanup_QueueLayout(queue_layout);
        if (error != 0) {
            exit(error);
        }
    }
    
    // SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}