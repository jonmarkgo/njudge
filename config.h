/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if you have the `flock' function. */
#define HAVE_FLOCK 1

/* Define to 1 if you have the `mrand48' function. */
#define HAVE_MRAND48 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Name of package */
#define PACKAGE "njudge"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""njudge""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""njudge" 1.7.6"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "-njudge-"

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.7.6"

/* Version number of package */
#define VERSION "1.7.6"

#ifdef HAVE_FLOCK
  #include <sys/file.h>
  #define lockfd(f,nb) flock(f, LOCK_EX | (nb ? LOCK_NB : 0))
#else
  #include <unistd.h>
  #define lockfd(f,nb)  lockf(f, (nb ? F_TLOCK : F_LOCK), 0)
#endif

#ifdef HAVE_MRAND48
  #define random() mrand48()
  #define srandom(seed) srand48((long)seed)
#endif

