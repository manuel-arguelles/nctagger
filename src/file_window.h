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

#ifndef CTAGGER_FILE_WINDOW_H
#define CTAGGER_FILE_WINDOW_H

#include <ncurses.h>

#include "list.h"

/* FIXME: separation? */
#include "tag_window.h" 

typedef struct {
    WINDOW *win;
    int startx;
    int starty;
    int stopx;
    int stopy;
    List *dir_entries;
    char *cwd;
    int dir_offset;
    int dir_sel_idx;
    int dir_scr_pos;
    TagWin *tag_win;
} FileWin;
 
FileWin *file_window_new(void);

void file_window_draw(FileWin *win, 
                      int startx, int starty, 
                      int stopx, int stopy);

void file_window_focus(FileWin *win);

void file_window_keypress(void  *window, int ch);

void file_window_set_tag_window(FileWin *filewin, TagWin *tagwin);

#endif
