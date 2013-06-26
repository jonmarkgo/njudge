
CC = gcc
CFLAGS = --pedantic -Wall -std=c99
LIBS = `pkg-config --libs glib-2.0`
INCL = `pkg-config --cflags glib-2.0`

SDIR = ../
TDIR = .

APPFILES = conf datetime user
TESTFILES = test_main test_conf test_datetime test_user

APPOBJ = $(patsubst %,$(SDIR)/%.o,$(APPFILES))
TESTOBJ = $(patsubst %,$(TDIR)/%.o,$(TESTFILES))

$(TDIR)/%.o: $(TDIR)/%.c
	$(CC) -g -c $(INCL) -o $@ $< $(CFLAGS) -DUNITTEST

$(TDIR)/%.o: $(SDIR)/%.c
	$(CC) -g -c $(INCL) -o $@ $< $(CFLAGS) -DUNITTEST

test: $(TESTOBJ) $(patsubst %,$(TDIR)/%.o,$(APPFILES))
	$(CC) -o $(TDIR)/$@ $^ $(CFLAGS) $(LIBS)

test_clean:
	rm -f $(TDIR)/*.o $(TDIR)/test
