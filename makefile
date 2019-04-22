#makefile to build formfactors library
CC = icpc
CPPFLAGS =
libdir = lib/
incdir = include/
srcdir = src/


OUTFILE = $(libdir)libFF.a
#.PHONY all
all: $(OUTFILE)

FF.o: $(srcdir)FF.cpp $(incdir)FF.h $(srcdir)util.cpp $(incdir)util.h $(incdir)globals.h
	$(CC) $(CPPFLAGS) -c $< -o $@ -I$(incdir) 

util.o: $(srcdir)util.cpp $(incdir)util.h $(incdir)globals.h
	$(CC) $(CPPFLAGS) -c $< -o $@ -I$(incdir) 

$(OUTFILE): FF.o util.o 
	ar r $@ $^


