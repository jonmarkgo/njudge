
CC = gcc
CFLAGS = --pedantic -Wall -std=c99
LIBS = `pkg-config --libs glib-2.0`
INCL = `pkg-config --cflags glib-2.0`

SDIR = ../../

VVAPPFILES =  datetime

APPOBJ = $(patsubst %,$(SDIR)/%.o,$(APPFILES))

all: migrate_db

datetime.o: ../../datetime.c
	$(CC) -g -c $(INCL) -o $@ $(CFLAGS) $<

user.o: ../../user.c
	$(CC) -g -c $(INCL) -o $@ $(CFLAGS) $<

migrate_db.o: migrate_db.c
	$(CC) -g -c $(INCL) -o $@ $(CFLAGS) $<

migrate_db: datetime.o migrate_db.o user.o
	$(CC) -g -O2 -o $@ $^ $(CFLAGS) $(LIBS)
