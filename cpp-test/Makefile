# compiler
CC = g++
CXXFLAGS += -Wall -Wextra -pedantic-errors

# the sfml libs
LDLIBS := $(shell pkg-config sfml-all --libs)

BIN_DIR = ./bin
SOURCES = pn_test.cpp

# list of bins
LIST = $(BIN_DIR)/PerlinNoise

all: $(LIST)

# what to do?
$(BIN_DIR)/%: $(SOURCES)
	$(CC) $(CXXFLAGS) $< $(LDLIBS) -o $@

# remove the binary
clean:
	$(RM) $(BIN_DIR)/*