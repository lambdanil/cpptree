CC=g++
CPPFLAGS=-O2
DEPS = cpptree.h
OBJ = example.o 

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS)

example: $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS)

clean:
	rm *.o example