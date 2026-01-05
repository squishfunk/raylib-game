CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRCDIR = src
SOURCES = $(SRCDIR)/main.cpp \
          $(SRCDIR)/ecs/ecs.cpp \
          $(SRCDIR)/map/map.cpp \
          $(SRCDIR)/systems/movement_system.cpp \
          $(SRCDIR)/systems/render_system.cpp \
          $(SRCDIR)/systems/health_system.cpp \
          $(SRCDIR)/systems/bullet_system.cpp \
          $(SRCDIR)/systems/collision_system.cpp \
          $(SRCDIR)/systems/player_system.cpp \
          $(SRCDIR)/systems/enemy_system.cpp \
          $(SRCDIR)/game/game.cpp \
          $(SRCDIR)/utils/helpers.cpp

OBJECTS = $(SOURCES:.cpp=.o)
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
