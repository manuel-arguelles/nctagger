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

#ifndef CTAGGER_UTILS_H
#define CTAGGER_UTILS_H

#include <stdio.h>
#include <taglib/tag_c.h>

struct file_entry {
    char *name;
    char *filename;
    int selected;
    int directory;
    TagLib_File *tagfile;
};

/*
 * get_tag_lib_file: returns a pointer to TagLib_File or null if the
 * file is not supported by TagLib
 */
TagLib_File *get_tag_lib_file(const char *file);

/*
 * trim: remove leading and trailing spaces. Returned string should be
 * freed after use.
 */
char *trim(const char *string);


#endif
