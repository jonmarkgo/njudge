
/* File for generating Dip Statistics */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "conf.h"
#include "dipstats.h"
#include <stdarg.h>

/* This function returns complete filename, based on date values */
static char *GenerateStatsFileName()
{
    static char ret_name[256];

    /* filename is of the format YYMMDDHH */

   time_t now = time(NULL);

   /*
    * Use gmtime as Winter/Summer time adjustments would over-write
    * existing logs otherwise
    */

   struct tm *ts = gmtime(&now);

   sprintf(ret_name,
           "%2.2d%2.2d%2.2d%2.2d%s",
           ts->tm_year % 100, /* Already in Y2k! */
           ts->tm_mon+1,
           ts->tm_mday,
           ts->tm_hour,
           ".txt");
    return ret_name;
}

int StatLog( int stat_level, char *stat_text, ...)
{

   char filename[512];
   char subdir[512];
   FILE *fsptr;
   va_list args;



   if (!STATS_FLAG) return 0; /* Not enabled, so exit! */

   sprintf(subdir,"%s/%3.3d",
	   STATS_DIR, stat_level);

   sprintf(filename, "%s/%s",
           subdir, GenerateStatsFileName());

   /* Check if the various components exist, creating if not */

   mkdir(STATS_DIR, 0xFFF);
   mkdir(subdir, 0xFFF);

   fsptr = fopen(filename, "a");
   if (fsptr != NULL) {
      va_start(args, stat_text);
      fprintf(fsptr, stat_text, args);
      va_end(args);
      fclose(fsptr);
      return 0;
   }
   else
   {
      return 1;
   }
}
