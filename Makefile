all:
	gcc -o main src/main.c src/ecs.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11