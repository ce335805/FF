#makefile to build formfactors library
CC = icpc
CPPFLAGS =

OUTFILE = libFF.a
#.PHONY all
all: $(OUTFILE)

FF.o: FF.cpp FF.h util.cpp util.h globals.h
	$(CC) $(CPPFLAGS) -c $< -o $@  

util.o: util.cpp util.h globals.h
	$(CC) $(CPPFLAGS) -c $< -o $@  

$(OUTFILE): FF.o util.o 
	ar r $@ $^
#	ranlib $@


