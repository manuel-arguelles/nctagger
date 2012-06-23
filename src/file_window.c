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
#include <taglib/tag_c.h>


#include "file_window.h"
#include "utils.h"

struct _file_entry {
    char *name;
    char *filename;
    int selected;
    int directory;
    TagLib_File *tagfile;
};

typedef struct _file_entry file_entry_t;

static List *load_files(const char *path);
static void free_list_entry(void *data);
static void file_window_load_files(FileWin *win, const char *path);
static void file_window_display_files(FileWin *win);

FileWin *file_window_new(void)
{
    FileWin *file_win = NULL;
    
    file_win = calloc(1, sizeof(FileWin));

    file_win->cwd = getcwd(NULL, 0);

    return file_win;
}

void file_window_draw(FileWin *win, 
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

        file_window_load_files(win, ".");
        file_window_display_files(win);
    }
}

static void file_window_load_files(FileWin *win, const char *path)
{
    List *list; 

    if (win != NULL && path != NULL) {
        list = load_files(path);
        if (list != NULL) {
            if (win->dir_entries) {
                /* Free old entries */
                list_free(win->dir_entries, free_list_entry);
            }
            win->dir_entries = list;
        }
    }
}

static void file_window_display_files(FileWin *win)
{
    int dir_pos = 0;
    int k = 0;
    int i = 1;
    const file_entry_t *file = NULL;

    if (win != NULL && win->dir_entries != NULL) {
        k = win->dir_offset;
        while ((file = list_get(win->dir_entries, k++)) != NULL) {
            if (i >= (win->stopy -1) )
                break;
            if (file->selected) {
                wattron(win->win, A_REVERSE);
                mvwaddstr(win->win, i, 1, file->name);
                wattroff(win->win, A_REVERSE);
                win->dir_scr_pos = i;
            } else 
                mvwaddstr(win->win, i, 1, file->name);
            i++;
        }
        wrefresh(win->win);
    }
}

static int is_directory(const struct dirent *dir_entry)
{
    if (dir_entry->d_type == DT_DIR && dir_entry->d_name[0] != '.')
        return 1;
    
    return 0;
}

static int is_file(const struct dirent *dir_entry)
{
    if (dir_entry->d_type == DT_DIR || dir_entry->d_name[0] == '.')
        return 0;
    
    return 1;
}


static List *load_files(const char *path)
{
    List *entries = NULL;
    List *entries_dir = NULL;
    List *entries_file = NULL;;
    DIR *dir;
    struct dirent **ent;
    char buf[5120];
    file_entry_t *file = NULL;
    int num_ent = 0;

    if (path == NULL) {
        return NULL;
    }

    dir = opendir(path);
    if (dir != NULL) {
        entries_dir = list_new();
        file = calloc(1, sizeof(file_entry_t));
        file->name = strdup("[..]");
        file->selected = 1;
        file->directory = 1;
        if (snprintf(buf, sizeof(buf)-1, 
                     "%s%c%s", path, '/', "..") >= sizeof(buf)-1) {
            abort();
        }

        file->filename = strdup(buf);
           
        list_add(entries_dir, file);
        closedir(dir);
    }

    if ((num_ent = scandir(path, &ent, is_directory, alphasort)) > 0) {
        int i;
        for (i = 0; i < num_ent; i++) {
            file = calloc(1, sizeof(file_entry_t));
            if (snprintf(buf, sizeof(buf)-1, 
                         "%s%c%s", path, '/', 
                         ent[i]->d_name) >= sizeof(buf)-1) {
                abort();
            }
            file->filename = strdup(buf);
            if (snprintf(buf, sizeof(buf)-1, "[%s]", 
                         ent[i]->d_name) >= sizeof(buf)-1) {
                abort();
            }
            file->name = strdup(buf);
            file->directory = 1;
            list_add(entries_dir, file);
        }
    }

    if ((num_ent = scandir(path, &ent, is_file, alphasort)) > 0) {
        int i;
        entries_file = list_new();
        for (i = 0; i < num_ent; i++) {
            TagLib_File *tag_file = NULL;
            if (snprintf(buf, sizeof(buf)-1, 
                         "%s%c%s", path, '/', 
                         ent[i]->d_name) >= sizeof(buf)-1) {
                abort();
            }
            tag_file = get_tag_lib_file(buf);
            if (tag_file) {
                file = calloc(1, sizeof(file_entry_t));
                file->filename = strdup(buf);
                file->name = strdup(ent[i]->d_name);
                file->tagfile = tag_file;
                list_add(entries_file, file);
            }
        }
    }

    entries = list_join(entries_dir, entries_file);
    list_free(entries_file, NULL);

    return entries;

}

void free_list_entry(void *data)
{
    file_entry_t *entry = data;
    if (entry) {
        free(entry->name);
        free(entry->filename);
        if (entry->tagfile)
            taglib_file_free(entry->tagfile);
        free(entry);
    }
}

void file_window_keypress(void *window, int ch)
{
    FileWin *win = (FileWin *) window;

    if (win != NULL) {
        if (ch == KEY_UP) {
            if (win->dir_sel_idx != 0) {
                file_entry_t *new_entry =                           \
                    list_get(win->dir_entries, win->dir_sel_idx -1 );
                if (new_entry != NULL) {
                    /* Deselect and select previous one */
                    file_entry_t *old_entry =   \
                        list_get(win->dir_entries, win->dir_sel_idx);
                    old_entry->selected = 0;
                    new_entry->selected = 1;
                    if (!new_entry->directory)
                        tag_window_load_tags(win->tag_win, 
                                             new_entry->tagfile);
                    win->dir_sel_idx--;
                    if (win->dir_scr_pos <= win->starty) {
                        win->dir_offset--;
                        wclear(win->win);
                        box(win->win, 0, 0);
                    }
                }
            }
        } else if (ch == KEY_DOWN) {
            if (win->dir_sel_idx < win->dir_entries->len) {
                file_entry_t *new_entry = \
                    list_get(win->dir_entries, win->dir_sel_idx + 1);
                if (new_entry != NULL) {
                    file_entry_t *old_entry =                       \
                        list_get(win->dir_entries, win->dir_sel_idx);
                    /* Deselect and select next one */
                    old_entry->selected = 0;
                    new_entry->selected = 1;
                    if (!new_entry->directory)
                        tag_window_load_tags(win->tag_win, 
                                             new_entry->tagfile);
                    win->dir_sel_idx++;
                    if (win->dir_scr_pos >= win->stopy - 2) {
                        win->dir_offset++;
                        wclear(win->win);
                        box(win->win, 0, 0);
                    }
                }
            }
        } else if (ch == KEY_ENTER || ch == '\n') {
            char *path = NULL;
            file_entry_t *selected = list_get(win->dir_entries, win->dir_sel_idx);
            path = selected->filename;
            if (selected->directory) {
                file_window_load_files(win, path);
                wclear(win->win);
                box(win->win, 0, 0);
                win->dir_offset = 0;
                win->dir_sel_idx = 0;
            } 
        }
        file_window_display_files(win);
    }
}

void file_window_focus(FileWin *win)
{
    curs_set(0);
}

void file_window_set_tag_window(FileWin *filewin, TagWin *tagwin)
{
    assert(filewin != NULL);

    filewin->tag_win = tagwin;
}
