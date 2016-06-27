CC=g++
CFLAGS=-Wall
LFLAGS=-lSDL2 -lSDL2_image
TESTFLAGS=-lgtest -lgtest_main
LINK=g++

SRC_FOLDER=src/jvisu
TEST_FOLDER=test
BIN_FOLDER=bin
EXE=$(BIN_FOLDER)/exe
TEST_EXE=$(BIN_FOLDER)/test/exe

LIB_SOURCES=$(shell find $(SRC_FOLDER) -type f -iname '*.c' -o -iname '*.cpp')
LIB_HEADERS=$(shell find $(SRC_FOLDER) -type f -iname '*.h')

INTOBJECTS=$(subst $(SRC_FOLDER),$(BIN_FOLDER)/jvisu, $(LIB_SOURCES))
LIB_OBJECTS=$(subst .c,.o,$(subst .cpp,.o,$(INTOBJECTS)))

EXE_OBJECTS=$(BIN_FOLDER)/main.o $(LIB_OBJECTS)

TEST_SOURCES=$(shell find $(TEST_FOLDER) -type f -iname '*.c' -o -iname '*.cpp')
TEST_HEADERS=$(shell find $(TEST_FOLDER) -type f -iname '*.h')
TEST_OBJECTS=$(subst .cpp,.o,$(subst $(TEST_FOLDER),$(BIN_FOLDER)/test,$(TEST_SOURCES)))



$(BIN_FOLDER)/jvisu/%.o : $(SRC_FOLDER)/%.c* $(LIB_HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(BIN_FOLDER)/test/%.o : $(TEST_FOLDER)/%.c* $(LIB_HEADERS) $(TEST_HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_FOLDER)/main.o: src/main.cpp $(LIB_HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@



$(EXE): $(EXE_OBJECTS)
	$(LINK) $^ $(LFLAGS) -o $@

$(TEST_EXE): $(TEST_OBJECTS) $(LIB_OBJECTS)
	$(LINK) $^ $(LFLAGS) $(TESTFLAGS) -o $@


.phony: exe
exe: $(EXE)


.phony: run
run: $(EXE)
	./$(EXE)


.phony: test
test: $(TEST_EXE)
	./$(TEST_EXE)


.phony: clobber
clobber:
	rm -f $(shell find $(BIN_FOLDER) -type f -iname '*.o')
	rm -f $(EXE)
	rm -f $(TEST_EXE)


.phony: list
list:
	@echo $(TEST_SOURCES)
	@echo $(TEST_OBJECTS)


#print number of lines
.phony: line_count
line_count:
	find . -name '*.c' -o -name '*.cpp' -o -name '*.h' | xargs wc -l

