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

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>

#include "main_window.h"
#include "file_window.h"
#include "tag_window.h"
#include "config.h"

struct _context{
    FileWin *file_window;
    TagWin *tag_window;
    void *active_window;
    void (*key_event)(void *, int ); 
};

static struct _context *context = NULL;

static void focus_to_file_win(void);
static void focus_to_tag_win(void);
static int _get_screen_size(int *x, int *y);


static int _get_screen_size(int *x, int *y)
{
    int ret = -1;

    if (x != NULL && y != NULL) {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        *y = w.ws_row;
        *x = w.ws_col;
        ret = 0;
    }
    return ret;
}

void main_window_init(void)
{
    if (context == NULL) {
        context = calloc(1, sizeof(struct _context));
        context->file_window = file_window_new();
        context->tag_window = tag_window_new();
        file_window_set_tag_window(context->file_window, context->tag_window);
    }

    draw_window();

    focus_to_file_win();
}

void draw_window(void)
{
    int max_x = 0;
    int max_y = 0;
    int fw_max_y = 0;

    if (context != NULL) {
    
        _get_screen_size(&max_x, &max_y);

        mvaddstr(0,0, PACKAGE_STRING);
        attron(A_REVERSE);
        mvaddstr(max_y-1, 1, "F5");
        attroff(A_REVERSE);
        addch(' ');
        addstr("Save");
        addstr("   ");
        attron(A_REVERSE);
        addstr("F10");
        attroff(A_REVERSE);
        addch(' ');
        addstr("Save all");

        refresh();
        
        fw_max_y = max_y / 2;

        if (context->file_window != NULL) {
            file_window_draw(context->file_window,
                             0, 1, max_x, fw_max_y);
        }

        fw_max_y++;
        max_y = ((max_y - fw_max_y) - 1);

        if (context->tag_window != NULL) {
            tag_window_draw(context->tag_window,
                            0, fw_max_y, max_x, max_y);
        }

    }
}

int main_window_event(void)
{
    int ch;
    ch = getch();
    if (ch == KEY_F(1)) {
        /* Change to file window */
        focus_to_file_win();
        return 1;
    } else if (ch == KEY_F(2)) {
        /* Change to tag window */
        focus_to_tag_win();
        return 1;
    } else if (ch == KEY_F(3)) {
        return 0;
    } else {
        context->key_event(context->active_window, ch);
        return 1;
    }
}

static void focus_to_file_win(void)
{
    file_window_focus(context->file_window);
    context->active_window = context->file_window;
    context->key_event = file_window_keypress;
}

static void focus_to_tag_win(void)
{
    tag_window_focus(context->tag_window);
    context->active_window = context->tag_window;
    context->key_event = tag_window_keypress;
}

