
AC_INIT([nctagger], 
        m4_esyscmd([./GIT-VERSION-GEN]),
        [manuel.arguelles@gmail.com])

AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER(config.h)

AM_INIT_AUTOMAKE([foreign])

AC_PROG_CC

AC_HEADER_TIOCGWINSZ
AC_CHECK_HEADERS([ncurses.h])
AC_SEARCH_LIBS([clear], [ncursesw])

AC_CHECK_HEADERS([form.h])
AC_SEARCH_LIBS([new_form], [formw])

PKG_CHECK_MODULES([TAGLIB], [taglib_c]) 
AC_SUBST([TAGLIB_CFLAGS])
AC_SUBST([TAGLIB_LIBS])


AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
