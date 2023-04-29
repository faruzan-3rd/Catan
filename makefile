# MacOS
# CC = clang++

CC = g++
CFLAGS = -std=c++20 -g -Wall
TARGET = app

SRCS = src/main.cpp src/wastils/config.cpp src/catan/assets.cpp src/catan/board.cpp\
		src/catan/gamemanager.cpp src/wastils/UI/UI.cpp src/wastils/UI/UIScheme.cpp\
		src/wastils/geometry.cpp src/wastils/input.cpp src/catan/graphics.cpp\
		src/catan/logic.cpp src/catan/player.cpp src/catan/events.cpp


OBJS = $(SRCS:.cpp=.o)
INCDIR = -Iinclude/ -I/usr/local/include/ 
LIBDIR = 
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lyaml-cpp

$(TARGET): $(OBJS)
	$(CC) -o $@ $(^F) $(LIBDIR) $(LIBS)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(INCDIR) -c $(@:.o=.cpp)

all: clean $(OBJS) $(TARGET)

clean:
	-rm -f $(OBJS) $(TARGET) *.d
