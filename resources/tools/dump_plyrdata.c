
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <netinet/in.h>

#define PLYRDATA_MAGIC          0x504C5952
#define PLYRDATA_VERSION        2

typedef struct {	

    uint32_t magic;
    uint32_t version;
    uint32_t header_size;
    uint32_t record_size;

} plyrhdr_t;

typedef struct {

    uint32_t ontime;
    uint32_t total;
    uint32_t started;
    uint32_t resigned;
    uint32_t tookover;
    uint32_t reserved;

} plyrrec_t;

char sql_str[] = "INSERT INTO player_stats (total, ontime, started, "
                 "resigned, tookover) values (%u, %u, %u, %u, %u);";

int main(int argc, char** argv) {

    int       ret = 0;
    FILE*     infp = NULL;
    uint32_t  count = 0;
    plyrhdr_t hdr;
    plyrrec_t rec;

    if (argc < 2) {
        fprintf(stderr, "usage: dump_plyrdata filename\n");
        goto exit_main;
    }

    if ((infp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "e> file error - %s: %s\n", argv[1], strerror(errno));
        goto exit_main;
    }

    if (fread(&hdr, sizeof hdr, 1, infp) < 1) {
        fprintf(stderr, "e> corrupt header\n");
        goto exit_main;
    }

    if ((hdr.magic != PLYRDATA_MAGIC) || hdr.version != 2) {
        fprintf(stderr, "e> this plyrdata file is not supported\n");
        goto exit_main;
    }

    while (fread(&rec, sizeof rec, 1, infp) > 0) {
        printf("INSERT INTO player_stats (total, ontime, started, resigned, "
               "tookover) VALUES (%u, %u, %u, %u, %u);\n",
               rec.total, rec.ontime, rec.started, rec.resigned, rec.tookover);
        count ++;
    }

    fprintf(stderr, "%u records dumped\n", count);

    ret = 1;

exit_main:

    if (infp) fclose(infp);

    return ret;

}
