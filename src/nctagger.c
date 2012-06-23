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
#include <unistd.h>
#include <ncurses.h>
#include <signal.h>
#include <locale.h>

#include "main_window.h"

void signal_handler(int signum)
{
    if (signum == SIGWINCH) {
        /* Resize */
        draw_window();
    }

}

int main(int argc, char **argv)
{
    struct sigaction act;
    act.sa_handler = signal_handler;
    
    sigaction(SIGWINCH, &act, NULL);

    /* TODO: Check that console is UTF-8 */
    if (setlocale(LC_ALL, "") == NULL) {
        printf("Could not set locale, please check your LC_ALL, LC_CTYPE\n");
        return 1;
    }    

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    main_window_init();

    while(main_window_event()) {
        /* This is main loop, nothing really usefull here */
    }
    

    endwin();

    return 0;
}
