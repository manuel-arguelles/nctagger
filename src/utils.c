/* 
 * Copyright (C) 2012  Manuel Argüelles <manuel.arguelles@gmail.com>
 *
 * This file is part of nctagger.
 *
 * Nctagger is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "utils.h"
#include <string.h>

TagLib_File *get_tag_lib_file(const char *file)
{
    TagLib_File *tagfile = NULL;

    if (file == NULL) {
        return NULL;
    }

    tagfile = taglib_file_new(file);
    if (tagfile == NULL) {
        return NULL;
    }

    if (taglib_file_is_valid(tagfile)) {
        /* TagLib_Tag *tags = NULL;
         * tags = taglib_file_tag(tagfile);
         */
        taglib_file_save(tagfile);
    } else {
        taglib_file_free(tagfile);
        tagfile = NULL;
    }

    return tagfile;
}

char *trim(const char *string)
{
    int i,j = 0;
    char data[strlen(string)+1];
    
    for (i = 0; string[i] != '\0'; i++) {
        if (string[i] != ' ' && string[i] != '\t') {
            data[j++] = string[i];
        } else if (j > 0) {
            data[j++] = string[i];
        }
    }
    data[j] = '\0';
    
    for (i = j; i > 0; i--) {
        if (data[i] == ' ' || data[i] == '\t' || data[i] == '\0')
            data[i] = '\0';
        else 
            break;
    }

    return strdup(data);
}
