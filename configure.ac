dnl Process this file with autoconf to produce a configure script.
AC_INIT("njudge", 1.3.1)
AM_CONFIG_HEADER(config.h)
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_EXEEXT

dnl set install options
ac_user=""
AC_ARG_WITH(user,
    [  --with-user=NAME        Name of user judge should run as [[judge]] ],
    [  ac_user="$withval"],
    [  ac_user="judge" ]
)
USER="$ac_user"
AC_SUBST(USER)

ac_dir=""
AC_ARG_WITH(dir,
    [  --with-dir=DIR          Directory to install judge in [[/home/judge]] ],
    [  ac_dir="$withval"],
    [  ac_dir="/home/judge" ]
)
DIR="$ac_dir"
AC_SUBST(DIR)

AC_ARG_WITH(domain,
    [  --with-domain=DOMAIN    Domain name judge email comes from [[localhost]] ],
    [ ac_domain="$withval"],
    [ ac_domain="localhost" ]
)
DOMAIN="$ac_domain"
AC_SUBST(DOMAIN)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_AWK
AC_PROG_LN_S
AC_PATH_PROG(HEAD, head)
AC_PATH_PROG(_MAIL_, mail) # MAIL often is already set
AC_PATH_PROG(PERL, perl)

dnl Checks for functions
AC_CHECK_FUNCS(strdup) 

AC_OUTPUT(Makefile smail dipclean)

