@BOTTOM@

#ifdef HAVE_FLOCK
  #include <sys/file.h>
  #define lockfd(f,nb) flock(f, LOCK_EX | (nb ? LOCK_NB : 0))
  #define unlockfd(f, nb) flock(f, 0)
#else
  #include <unistd.h>
  #define lockfd(f,nb)  lockf(f, (nb ? F_TLOCK : F_LOCK), 0)
  #define unlockfd(f, nb) lockf(f, 0, 0)
#endif

#ifdef HAVE_MRAND48
  #define random() mrand48()
  #define srandom(seed) srand48((long)seed)
#endif

