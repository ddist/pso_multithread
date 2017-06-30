CC=gcc
CFLAGS=-Wall
ODIR = .
SDIR = src
INC = -Iinc

_OBJS = main.o pso.o shared.o instance.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

_OBJS_MT = main.o pso_mt.o shared.o instance.o
OBJS_MT = $(patsubst %,$(ODIR)/%,$(_OBJS_MT))

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c $(INC) -o $@ $< $(CFLAGS)

pso: $(OBJS) 
	$(CC) -o pso $^ -lm

pso_mt: $(OBJS_MT)
	$(CC) -o pso_mt $^ -lm

all: pso

.PHONY: clean

clean:
	rm -f $(ODIR)/pso
	rm -f $(ODIR)/pso_mt
	rm -f $(ODIR)/*.o