#makefile to build formfactors library
CC = icpc
CPPFLAGS = -Ofast -Wall -Wextra -pedantic

OUTFILE = libFF.a
#.PHONY all
all: $(OUTFILE)

FF.o: FF.h util.h globals.h
	$(CC) $(CPPFLAGS) -c $< -o $@  

util.o: util.h globals.h
	$(CC) $(CPPFLAGS) -c $< -o $@  

$(OUTFILE): FF.o util.o 
	ar ru $@ $^
	ranlib $@


