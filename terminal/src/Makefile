CC = gcc
CFLAGS = -I. -ludev -lreadline -lpthread

DEPS = main.h common.h strdef.h termutil.h docuproc.h cmdproc.h strdoc.h

OBJ = termutil.o docuproc.o cmdproc.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

i2cterminal: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o i2cterminal