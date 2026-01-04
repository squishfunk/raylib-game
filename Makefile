all:
	gcc -o main src/main.c src/ecs_core.c src/systems/*.c src/helpers.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11