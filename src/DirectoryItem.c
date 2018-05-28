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

#include "DirectoryItem.h"

int compare_DirectoryItem(const void *p1, const void *p2)
{
        /*

        char *n1, *n2;

        if (((const DirectoryItem *) p1)->dirent_entry.d_name[0] == '.')
                n1 = ((const DirectoryItem *) p1)->dirent_entry.d_name + 1;
        else
                n1 = ((const DirectoryItem *) p1)->dirent_entry.d_name;

        if (((const DirectoryItem *) p2)->dirent_entry.d_name[0] == '.')
                n2 = ((const DirectoryItem *) p2)->dirent_entry.d_name + 1;
        else
                n2 = ((const DirectoryItem *) p2)->dirent_entry.d_name;
        */

        DirectoryItem *n1, *n2;

        n1 = (DirectoryItem *) p1;
        n2 = (DirectoryItem *) p2;

        int max_size = n1->name_length < n2->name_length ? n1->name_length : n2->name_length;

        //strncmp(const char *__s1, const char *__s2, size_t __n)
        return strncmp(n1->dirent_entry.d_name, n2->dirent_entry.d_name, max_size);
}