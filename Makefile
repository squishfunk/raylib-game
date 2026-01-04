all:
	gcc -o main src/main.c src/ecs.c src/systems/*.c src/helpers.c src/game.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11