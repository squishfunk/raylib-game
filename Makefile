all:
	gcc -o main src/main.c src/ecs.c src/map.c src/systems/*.c src/game/*.c src/helpers.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11