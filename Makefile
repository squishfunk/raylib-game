CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O0
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRCDIR = src
BUILDDIR = build
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
          $(SRCDIR)/systems/door_system.cpp \
          $(SRCDIR)/systems/room_system.cpp \
          $(SRCDIR)/game/game.cpp \
          $(SRCDIR)/map/dungeon.cpp \
          $(SRCDIR)/factories/door_factory.cpp \
          $(SRCDIR)/factories/enemy_factory.cpp \
          $(SRCDIR)/factories/player_factory.cpp \
          $(SRCDIR)/utils/helpers.cpp

OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
TARGET = $(BUILDDIR)/main

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BUILDDIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean run
