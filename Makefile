CC=g++
CFLAGS=-Wall
LINK=g++

SRC_FOLDER=src/jvisu
TEST_FOLDER=test
BIN_FOLDER=bin
EXE=$(BIN_FOLDER)/exe

LIB_SOURCES=$(shell find $(SRC_FOLDER) -type f -iname '*.c' -o -iname '*.cpp')
LIB_HEADERS=$(shell find $(SRC_FOLDER) -type f -iname '*.h')

INTOBJECTS=$(subst $(SRC_FOLDER),$(BIN_FOLDER)/jvisu, $(SOURCES))
LIB_OBJECTS=$(subst .c,.o,$(subst .cpp,.o,$(INTOBJECTS)))

EXE_OBJECTS=$(BIN_FOLDER)/main.o $(LIB_OBJECTS)

TEST_SOURCES=$(shell find $(TEST_FOLDER) -type f -iname '*.c' -o -iname '*.cpp')
TEST_HEADERS=$(shell find $(TEST_FOLDER) -type f -iname '*.h')



$(BIN_FOLDER)/jvisu/%.o : $(SRC_FOLDER)/%.c*
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_FOLDER)/test/%.o : $(TEST_FOLDER)/%.c*
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_FOLDER)/main.o: src/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@



$(EXE): $(EXE_OBJECTS) $(LIB_HEADERS)
	$(LINK) $^ -o $@

.phony: exe
exe: $(EXE)


.phony: run
run: $(EXE)
	./$(EXE)


.phony: clobber
clobber:
	rm -f $(BIN_FOLDER)/*.o
	rm -f $(EXE)


.phony: list
list:
	@echo $(LIB_SOURCES)
	@echo $(LIB_HEADERS)
	@echo $(EXE_OBJECTS)


#print number of lines
.phony: line_count
line_count:
	find . -name '*.c' -o -name '*.cpp' -o -name '*.h' | xargs wc -l

