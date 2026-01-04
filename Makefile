all:
	gcc -o main src/main.c src/ecs_core.c src/ecs_systems.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11