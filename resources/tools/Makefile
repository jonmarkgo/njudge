
CC = gcc
CFLAGS = --pedantic -Wall -std=c99
LIBS = `pkg-config --libs glib-2.0`
INCL = `pkg-config --cflags glib-2.0`

SDIR = ../../

VVAPPFILES =  datetime

APPOBJ = $(patsubst %,$(SDIR)/%.o,$(APPFILES))

all: dump_to_sql

datetime.o: ../../datetime.c
	$(CC) -c $(INCL) -o $@ $(CFLAGS) $<

dump_to_sql.o: dump_to_sql.c
	$(CC) -c $(INCL) -o $@ $(CFLAGS) $<

dump_to_sql: datetime.o dump_to_sql.o
	$(CC) -O2 -o $@ $^ $(CFLAGS) $(LIBS)
