#makefile to build formfactors library
CC = icpc
CPPFLAGS = -Ofast

OUTFILE = libFF.a
#.PHONY all
all: $(OUTFILE)

checking.o: checking.h FF.h util.h globals.h
	$(CC) $(CPPFLAGS) -c $< -o $@  

FF.o: FF.h util.h globals.h
	$(CC) $(CPPFLAGS) -c $< -o $@  

util.o: util.h globals.h
	$(CC) $(CPPFLAGS) -c $< -o $@  

$(OUTFILE): checking.o FF.o util.o 
	ar ru $@ $^
	ranlib $@


