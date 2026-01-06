CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O0
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRCDIR = src
BUILDDIR = build
SOURCES = $(SRCDIR)/main.cpp \
          $(SRCDIR)/ecs/Ecs.cpp \
          $(SRCDIR)/map/Map.cpp \
          $(SRCDIR)/systems/MovementSystem.cpp \
          $(SRCDIR)/systems/RenderSystem.cpp \
          $(SRCDIR)/systems/HealthSystem.cpp \
          $(SRCDIR)/systems/BulletSystem.cpp \
          $(SRCDIR)/systems/CollisionSystem.cpp \
          $(SRCDIR)/systems/PlayerSystem.cpp \
          $(SRCDIR)/systems/EnemySystem.cpp \
          $(SRCDIR)/systems/DoorSystem.cpp \
          $(SRCDIR)/systems/RoomSystem.cpp \
          $(SRCDIR)/game/Game.cpp \
          $(SRCDIR)/map/Dungeon.cpp \
          $(SRCDIR)/factories/DoorFactory.cpp \
          $(SRCDIR)/factories/EnemyFactory.cpp \
          $(SRCDIR)/factories/PlayerFactory.cpp \
          $(SRCDIR)/utils/Helpers.cpp \
          $(SRCDIR)/events/EventBus.cpp

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
