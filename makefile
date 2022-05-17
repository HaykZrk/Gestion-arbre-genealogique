CC = gcc
CFLAGS = -Iinclude -Wall -g
OREP = obj/
BREP = bin/

vpath %.c src/
vpath %.h include/

prog: $(addprefix $(OREP), main.o genealogie.o file.o)
	$(CC) -o $@ $^
	if [ ! -d $(BREP) ]; then mkdir $(BREP); fi
	mv $@ $(BREP)

$(OREP)main.o: main.c genealogie.h file.h | $(OREP)
	$(CC) -c $(CFLAGS) $<
	mv main.o $(OREP)

$(OREP)genealogie.o: genealogie.c genealogie.h file.h | $(OREP)
	$(CC) -c $(CFLAGS) $<
	mv genealogie.o $(OREP)

$(OREP)file.o: file.c file.h | $(OREP)
	$(CC) -c $(CFLAGS) $<
	mv file.o $(OREP)

$(OREP):
	mkdir $@

clean:
	rm $(OREP)* $(BREP)*
