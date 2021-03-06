CC=g++
CFLAGS=-std=c++11 -Isrc/ssg -g
WFLAGS=-Wall
LFLAGS=-lSDL2 -lSDL2_image -lSDL2_ttf
TESTFLAGS=-lgtest -lgtest_main
LINK=g++
ARCHIVE=ar
AFLAGS=rcs

SRC_FOLDER=src/ssg
TEST_FOLDER=test
EXAMPLE_FOLDER=examples
BIN_FOLDER=bin
BUILD_FOLDER=build
EXE=$(BIN_FOLDER)/exe
TEST_EXE=$(BIN_FOLDER)/test/exe

LIBRARY=$(BUILD_FOLDER)/lib/ssg.a
INCLUDE_TARGET=$(BUILD_FOLDER)/include


LIB_SOURCES=$(shell find $(SRC_FOLDER) -type f -iname '*.c' -o -iname '*.cpp')
LIB_HEADERS=$(shell find $(SRC_FOLDER) -type f -iname '*.h')
INCLUDE_HEADERS=$(subst $(SRC_FOLDER),$(BUILD_FOLDER)/include,$(LIB_HEADERS))

INTOBJECTS=$(subst $(SRC_FOLDER),$(BIN_FOLDER)/ssg, $(LIB_SOURCES))
LIB_OBJECTS=$(subst .c,.o,$(subst .cpp,.o,$(INTOBJECTS)))

EXE_OBJECTS=$(BIN_FOLDER)/main.o

TEST_SOURCES=$(shell find $(TEST_FOLDER) -type f -iname '*.c' -o -iname '*.cpp')
TEST_HEADERS=$(shell find $(TEST_FOLDER) -type f -iname '*.h')
TEST_OBJECTS=$(subst .cpp,.o,$(subst $(TEST_FOLDER),$(BIN_FOLDER)/test,$(TEST_SOURCES)))



$(BIN_FOLDER)/ssg/%.o : $(SRC_FOLDER)/%.c* $(LIB_HEADERS)
	$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@ 

$(BIN_FOLDER)/test/%.o : $(TEST_FOLDER)/%.c* $(LIB_HEADERS) $(TEST_HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_FOLDER)/main.o: src/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@


$(BUILD_FOLDER)/include/%.h: $(SRC_FOLDER)/%.h
	cp $< $@


$(LIBRARY): $(LIB_OBJECTS)
	$(ARCHIVE) $(AFLAGS) $@ $^

$(EXE): $(EXE_OBJECTS) $(LIBRARY)
	$(LINK) $^ $(LFLAGS) -o $@

$(TEST_EXE): $(TEST_OBJECTS) $(LIBRARY)
	$(LINK) $^ $(LFLAGS) $(TESTFLAGS) -o $@


.phony: libinclude
libinclude: $(INCLUDE_HEADERS)

.phony: lib
lib: $(LIBRARY) libinclude

.phony: exe
exe: $(EXE)

.phony: run
run: $(EXE)
	./$(EXE)

demo_%:
	make lib
	mkdir -p bin/examples/$@
	$(eval in=$(shell find examples/$@ -type f -iname '*.cpp'))
	$(eval hs=$(shell find examples/$@ -type f -iname '*.h'))
	$(eval out=$(subst examples,bin/examples,$(subst .cpp,.o,$(in))))
	$(CC) $(CFLAGS) -Isrc/ssg -c $(in) -o $(out)
	$(LINK) $(out) $(LIBRARY) $(LFLAGS) -o bin/examples/$@/$@
	./bin/examples/$@/$@

.phony: test
test: $(TEST_EXE)
	./$(TEST_EXE)


.phony: docs
docs: lib
	doxygen Doxyfile


.phony: clobber
clobber:
	rm -f $(shell find $(BIN_FOLDER) -type f -iname '*.o')
	rm -rf $(BIN_FOLDER)/examples/*
	rm -f $(EXE)
	rm -f $(TEST_EXE)
	rm -f $(LIBRARY)
	rm -f $(BUILD_FOLDER)/include/*.h
	rm -rf docs/*


.phony: list
list:
	@echo $(TEST_SOURCES)
	@echo $(TEST_OBJECTS)


#print number of lines
.phony: line_count
line_count:
	find . -name '*.c' -o -name '*.cpp' -o -name '*.h' | xargs wc -l

