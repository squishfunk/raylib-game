#ifndef GAME_H
#define GAME_H

#include "ecs.h"

typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_GAME_OVER
} GameState;

typedef struct {
    GameState currentState;
    ECS ecs;
    int playerId;
    float lastShootTime;
} Game;

void game_init(void);
void game_cleanup(void);
void game_run(void);

void game_state_menu_update(Game *game);
void game_state_menu_render(void);
void game_state_playing_update(Game *game);
void game_state_playing_render(Game *game);

#endif