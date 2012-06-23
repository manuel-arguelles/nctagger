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

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "tag_window.h"
#include "utils.h"

static void tag_window_draw_tags(TagWin *win);
static int write_tag(TagLib_File *file, const int field, const char *data);
static int tag_type_by_field(TagWin *win, FIELD *field);


TagWin *tag_window_new(void)
{
    TagWin *tag_win = NULL;

    tag_win = calloc(1, sizeof(TagWin));

    return tag_win;
}

void tag_window_draw(TagWin *win, 
                     int startx, int starty, 
                     int stopx, int stopy)
{
    if (win != NULL) {
        if (win->win != NULL) {
            /* Clean and redraw */
            wclear(win->win);
            wrefresh(win->win);
            delwin(win->win);
        } 
        win->win = newwin(stopy, stopx, starty, startx);
        win->startx = startx;
        win->starty = starty;
        win->stopx = stopx;
        win->stopy = stopy;
        box(win->win, 0, 0);

        wrefresh(win->win);

        tag_window_draw_tags(win);
    }
}

static void tag_window_draw_tags(TagWin *win)
{
    int current_x;
    int current_y;

    assert(win != NULL);
    
    if (win->data_win != NULL) {
        /* Clean and remove */
        wclear(win->data_win);
        wrefresh(win->data_win);
        delwin(win->data_win);
    }

    win->data_win = newwin(win->stopy - 2,
                           win->stopx - 2,
                           win->starty + 1,
                           win->startx + 1);

    scrollok(win->data_win, TRUE);

    win->attribute[TAG_TEXT_TITLE] =            \
        new_field(1, 5, 2, 2, 0, 0);
    field_opts_off(win->attribute[TAG_TEXT_TITLE],
                   O_ACTIVE);
    field_opts_off(win->attribute[TAG_TEXT_TITLE],
                   O_STATIC);
    set_field_buffer(win->attribute[TAG_TEXT_TITLE],
                     0, "Title:");
    
    win->attribute[TAG_ATTR_TITLE] =            \
        new_field(1, 75, 2, 10, 0, 0);
    field_opts_off(win->attribute[TAG_ATTR_TITLE],
                   O_AUTOSKIP);
    field_opts_off(win->attribute[TAG_ATTR_TITLE],
                   O_STATIC);
    set_field_back(win->attribute[TAG_ATTR_TITLE], 
                   A_UNDERLINE);


    win->attribute[TAG_TEXT_ARTIST] =            \
        new_field(1, 5, 3, 2, 0, 0);
    field_opts_off(win->attribute[TAG_TEXT_ARTIST],
                   O_ACTIVE);
    field_opts_off(win->attribute[TAG_TEXT_ARTIST],
                   O_STATIC);
    set_field_buffer(win->attribute[TAG_TEXT_ARTIST],
                     0, "Artist:");
    
    win->attribute[TAG_ATTR_ARTIST] =            \
        new_field(1, 75, 3, 10, 0, 0);
    field_opts_off(win->attribute[TAG_ATTR_ARTIST],
                   O_AUTOSKIP);
    field_opts_off(win->attribute[TAG_ATTR_ARTIST],
                   O_STATIC);
    set_field_back(win->attribute[TAG_ATTR_ARTIST], 
                   A_UNDERLINE);


    win->attribute[TAG_TEXT_ALBUM] =            \
        new_field(1, 5, 4, 2, 0, 0);
    field_opts_off(win->attribute[TAG_TEXT_ALBUM],
                   O_ACTIVE);
    field_opts_off(win->attribute[TAG_TEXT_ALBUM],
                   O_STATIC);
    set_field_buffer(win->attribute[TAG_TEXT_ALBUM],
                     0, "Album:");
    
    win->attribute[TAG_ATTR_ALBUM] =            \
        new_field(1, 75, 4, 10, 0, 0);
    field_opts_off(win->attribute[TAG_ATTR_ALBUM],
                   O_AUTOSKIP);
    field_opts_off(win->attribute[TAG_ATTR_ALBUM],
                   O_STATIC);
    set_field_back(win->attribute[TAG_ATTR_ALBUM], 
                   A_UNDERLINE);



    win->attribute[TAG_TEXT_TRACK] =            \
        new_field(1, 5, 5, 2, 0, 0);
    field_opts_off(win->attribute[TAG_TEXT_TRACK],
                   O_ACTIVE);
    field_opts_off(win->attribute[TAG_TEXT_TRACK],
                   O_STATIC);
    set_field_buffer(win->attribute[TAG_TEXT_TRACK],
                     0, "Track:");
    
    win->attribute[TAG_ATTR_TRACK] =            \
        new_field(1, 75, 5, 10, 0, 0);
    field_opts_off(win->attribute[TAG_ATTR_TRACK],
                   O_AUTOSKIP);
    field_opts_off(win->attribute[TAG_ATTR_TRACK],
                   O_STATIC);
    set_field_back(win->attribute[TAG_ATTR_TRACK], 
                   A_UNDERLINE);
    set_field_type(win->attribute[TAG_ATTR_TRACK],
                   TYPE_INTEGER,
                   0, 0, 99);


    win->attribute[TAG_TEXT_YEAR] =             \
        new_field(1, 5, 6, 2, 0, 0);
    field_opts_off(win->attribute[TAG_TEXT_YEAR],
                   O_ACTIVE);
    field_opts_off(win->attribute[TAG_TEXT_YEAR],
                   O_STATIC);
    set_field_buffer(win->attribute[TAG_TEXT_YEAR],
                     0, "Year:");
    
    win->attribute[TAG_ATTR_YEAR] =            \
        new_field(1, 75, 6, 10, 0, 0);
    field_opts_off(win->attribute[TAG_ATTR_YEAR],
                   O_AUTOSKIP);
    field_opts_off(win->attribute[TAG_ATTR_YEAR],
                   O_STATIC);
    set_field_back(win->attribute[TAG_ATTR_YEAR], 
                   A_UNDERLINE);
    set_field_type(win->attribute[TAG_ATTR_YEAR],
                   TYPE_INTEGER,
                   0, 0, 9999);



    win->attribute[TAG_TEXT_GENRE] =            \
        new_field(1, 5, 7, 2, 0, 0);
    field_opts_off(win->attribute[TAG_TEXT_GENRE],
                   O_ACTIVE);
    field_opts_off(win->attribute[TAG_TEXT_GENRE],
                   O_STATIC);
    set_field_buffer(win->attribute[TAG_TEXT_GENRE],
                     0, "Genre:");
    
    win->attribute[TAG_ATTR_GENRE] =            \
        new_field(1, 75, 7, 10, 0, 0);
    field_opts_off(win->attribute[TAG_ATTR_GENRE],
                   O_AUTOSKIP);
    field_opts_off(win->attribute[TAG_ATTR_GENRE],
                   O_STATIC);
    set_field_back(win->attribute[TAG_ATTR_GENRE], 
                   A_UNDERLINE);



    win->attribute[TAG_TEXT_COMMENT] =            \
        new_field(1, 5, 8, 2, 0, 0);
    field_opts_off(win->attribute[TAG_TEXT_COMMENT],
                   O_ACTIVE);
    field_opts_off(win->attribute[TAG_TEXT_COMMENT],
                   O_STATIC);
    set_field_buffer(win->attribute[TAG_TEXT_COMMENT],
                     0, "Comment:");
    
    win->attribute[TAG_ATTR_COMMENT] =            \
        new_field(5, 75, 8, 10, 0, 0);
    field_opts_off(win->attribute[TAG_ATTR_COMMENT],
                   O_AUTOSKIP);
    field_opts_off(win->attribute[TAG_ATTR_COMMENT],
                   O_STATIC);
    set_field_back(win->attribute[TAG_ATTR_COMMENT], 
                   A_UNDERLINE);

    win->attribute[TAG_NULL] = NULL;

    /* set_new_page(win->attribute[TAG_TEXT_TRACK], TRUE); */

    win->form = new_form(win->attribute);
    if (win->form == NULL)
        abort();

    set_form_win(win->form, win->win);
    set_form_sub(win->form, win->data_win);
    

    post_form(win->form);

    form_driver(win->form, REQ_FIRST_FIELD);

    wrefresh(win->data_win);

}

void tag_window_keypress(void *window, int ch)
{
    TagWin *win = (TagWin *) window;

    assert(win != NULL);

    switch(ch) {
    case KEY_DOWN:
        form_driver(win->form, REQ_NEXT_FIELD);
        form_driver(win->form, REQ_END_LINE);
        break;
    case KEY_UP:
        form_driver(win->form, REQ_PREV_FIELD);
        form_driver(win->form, REQ_END_LINE);
        break;
    case KEY_BACKSPACE:
        form_driver(win->form, REQ_DEL_PREV);
        break;
    case KEY_LEFT:
        form_driver(win->form, REQ_PREV_CHAR);
        break;
    case KEY_RIGHT:
        form_driver(win->form, REQ_NEXT_CHAR);
        break;
    case KEY_NPAGE:
        form_driver(win->form, REQ_NEXT_PAGE);
        break;
    case KEY_F(5):
        /* form_driver(win->form, REQ_NEXT_FIELD);
           form_driver(win->form, REQ_PREV_FIELD); */
        if (form_driver(win->form, REQ_VALIDATION) == E_OK)
        {
            FIELD *cur_f = current_field(win->form);
            char *cur_b = trim(field_buffer(cur_f, 0));
            /* Record the selected field */
            write_tag(win->selected_file, 
                      tag_type_by_field(win, cur_f),
                      cur_b);
            free(cur_b);
        } else {
            form_driver(win->form, REQ_CLR_FIELD);
        }
        break;
    default:
        form_driver(win->form, ch);
        break;
    }
    wrefresh(win->data_win);
}

void tag_window_focus(TagWin *win)
{
    form_driver(win->form, REQ_END_LINE);
    curs_set(1);
    wrefresh(win->data_win);
}

void tag_window_load_tags(TagWin *win, TagLib_File *file)
{
    char *field_data = NULL;
    char num_to_str[10];
    TagLib_Tag *tag = NULL;
    unsigned int n_track = 0;

    assert(win != NULL);
    assert(file != NULL);

    win->selected_file = file;

    tag = taglib_file_tag(file);
    field_data = taglib_tag_title(tag);
    set_field_buffer(win->attribute[TAG_ATTR_TITLE], 
                     0, field_data ? field_data : "");

    field_data = taglib_tag_artist(tag);
    set_field_buffer(win->attribute[TAG_ATTR_ARTIST],
                     0, field_data ? field_data : "");

    field_data = taglib_tag_album(tag);
    set_field_buffer(win->attribute[TAG_ATTR_ALBUM],
                     0, field_data ? field_data : "");

    if ((n_track = taglib_tag_track(tag)) > 0) {
        snprintf(num_to_str, 10, "%d", n_track);
        set_field_buffer(win->attribute[TAG_ATTR_TRACK],
                         0, num_to_str);
    }
    
    field_data = taglib_tag_genre(tag);
    set_field_buffer(win->attribute[TAG_ATTR_GENRE],
                     0, field_data ? field_data : "");

    field_data = taglib_tag_comment(tag);
    set_field_buffer(win->attribute[TAG_ATTR_COMMENT],
                     0, field_data ? field_data : "");

    snprintf(num_to_str, 10, "%d", taglib_tag_year(tag));
    set_field_buffer(win->attribute[TAG_ATTR_YEAR],
                     0, num_to_str);

    taglib_tag_free_strings();

    wrefresh(win->data_win);

}

static int write_tag(TagLib_File *file, const int field, const char *data)
{
    int ret_code = 0;
    TagLib_Tag *tags = NULL;

    assert(file != NULL);
    assert(field > 0);
    assert(data != NULL);

    tags = taglib_file_tag(file);

    switch(field) {
    case TAG_ATTR_TITLE:
        taglib_tag_set_title(tags, data);
        break;

    case TAG_ATTR_ARTIST:
        taglib_tag_set_artist(tags, data);
        break;

    case TAG_ATTR_ALBUM:
        taglib_tag_set_album(tags, data);
        break;

    case TAG_ATTR_TRACK:
        taglib_tag_set_track(tags, atoi(data));
        break;

    case TAG_ATTR_YEAR:
        taglib_tag_set_year(tags, atoi(data));
        break;

    case TAG_ATTR_GENRE:
        taglib_tag_set_genre(tags, data);
        break;

    case TAG_ATTR_COMMENT:
        taglib_tag_set_comment(tags, data);
        break;

    default:
        ret_code = -1;
        break;
    }

    taglib_file_save(file);

    return ret_code;

}

static int tag_type_by_field(TagWin *win, FIELD *field)
{
    int i;
    int field_n = -1;
    for (i = 0; i < TAG_NULL; i++) {
        if (field == win->attribute[i]) {
            field_n = i;
            break;
        }
    }

    return field_n;
}
