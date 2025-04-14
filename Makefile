CC = gcc
CFLAGS = -std=c2x `pkg-config --cflags gtk4 cairo` -fopenmp
LIBS = -lm `pkg-config --libs gtk4 cairo`

BUILD_TYPE = dev
DEBUGGING_FLAGS = -fsanitize=undefined,address,leak -g3 -Og -Wall -Wpedantic -D_FORTIFY_SOURCE=2
PTIMIZATION_FLAGS = -O3

SRC = $(shell find src -name "*.c")
TARGET = main.out

ifeq ($(BUILD_TYPE), dev)
    CFLAGS += $(DEBUGGING_FLAGS)
else ifeq ($(BUILD_TYPE), release)
    CFLAGS += $(OPTIMIZATION_FLAGS)
else
    $(error "Invalid build type. Valid options are: dev, release")
endif

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
