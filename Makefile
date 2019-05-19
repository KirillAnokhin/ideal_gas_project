CFLAGS = -g -Wall
OBJ = experiment.o main.o
EXE = model

all: $(EXE)

model: $(OBJ)
	$(CXX) $(CFLAGS) $(OBJ) -o $(EXE)
clean:
	$(RM) *.o
