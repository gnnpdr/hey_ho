ROOT_DIR := -I/'Рабочий стол'/h/hey_ho/include
#ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

CC      := g++
CFLAGS := -Wall -Wextra -I$(ROOT_DIR)/include -MMD -MP
LDFLAGS  := 
TARGET   := program

SRCS     := main.cpp source/text_preparing.cpp source/lex_analysis.cpp
OBJS     := $(addprefix build/,$(notdir $(SRCS:.cpp=.o)))
DEPS     := $(OBJS:.o=.d)  # Файлы зависимостей (.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

build/%.o: source/%.cpp | build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: %.cpp | build  # Для main.cpp в корне
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p $@

clean:
	rm -rf build $(TARGET)

-include $(DEPS)  # Подключаем сгенерированные зависимости

.PHONY: all clean