# MacOS
# CC = clang++

CC = g++
CFLAGS = -std=c++17
TARGET = app

SRCS = src/main.cpp src/wastils/config.cpp src/catan/assets.cpp src/catan/tiles.cpp src/catan/board.cpp\
		src/catan/gamemanager.cpp src/wastils/UI/BaseUIElement.cpp src/wastils/UI/text.cpp\
		src/wastils/geometry.cpp src/catan/turn.cpp src/catan/progress.cpp src/wastils/events.cpp


OBJS = $(SRCS:.cpp=.o)
INCDIR = -Iinclude/ -I/usr/local/include/ 
LIBDIR = 
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lyaml-cpp

$(TARGET): $(OBJS)
	$(CC) -o $@ $(^F) $(LIBDIR) $(LIBS) -g

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(INCDIR) -c $(@:.o=.cpp) -g

all: clean $(OBJS) $(TARGET)

clean:
	-rm -f $(OBJS) $(TARGET) *.d
