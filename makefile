CXX         := g++

BIN         := bin
SRC         := src
INCLUDE     := include
LIB         := lib
LIBRARIES   := -l ncurses -l jsoncpp -l pthread
EXECUTABLE  := a.out


all: $(BIN)/$(EXECUTABLE)

run: clean all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) -std=c++17 -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*
