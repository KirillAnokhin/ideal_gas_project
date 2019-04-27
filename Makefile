CFLAGS = -g -Wall
OBJ = container.cpp main.cpp
EXE = model

all: $(EXE)

model: $(OBJ)
	$(CXX) $(CFLAGS) $(OBJ) -o $(EXE)
clean:
	$(RM) $(OBJ) $(EXE)
