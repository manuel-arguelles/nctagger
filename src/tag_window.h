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

#ifndef CTAGGER_TAG_WINDOW_H
#define CTAGGER_TAG_WINDOW_H

#include <form.h>
#include <taglib/tag_c.h>

enum tag_attributes_e {
    TAG_TEXT_TITLE = 0,
    TAG_ATTR_TITLE,
    TAG_TEXT_ARTIST,
    TAG_ATTR_ARTIST,
    TAG_TEXT_ALBUM,
    TAG_ATTR_ALBUM,
    TAG_TEXT_TRACK,
    TAG_ATTR_TRACK,
    TAG_TEXT_YEAR,
    TAG_ATTR_YEAR,
    TAG_TEXT_GENRE,
    TAG_ATTR_GENRE,
    TAG_TEXT_COMMENT,
    TAG_ATTR_COMMENT,
    TAG_NULL,
    TAG_ATTR_MAX,
};

typedef struct {
    WINDOW *win;
    WINDOW *data_win;
    int startx;
    int starty;
    int stopx;
    int stopy;
    FIELD *attribute[TAG_ATTR_MAX];
    FORM *form;
    TagLib_File *selected_file;
} TagWin;
 
TagWin *tag_window_new(void);


void tag_window_draw(TagWin *win, 
                     int startx, int starty, 
                     int stopx, int stopy);


void tag_window_keypress(void *window, int ch);

void tag_window_focus(TagWin *win);

void tag_window_load_tags(TagWin *win, TagLib_File *file);

#endif 
