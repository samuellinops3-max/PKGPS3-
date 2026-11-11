#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <io/pad.h>
#include <sysutil/sysutil.h>

#define MAX_SNAKE 300
#define GRID_W 48
#define GRID_H 27

typedef struct {
    int x, y;
} Point;

Point snake[MAX_SNAKE];
int snake_len = 6;
int dir_x = 1, dir_y = 0;
int food_x, food_y;
int score = 0;
int game_over = 0;
int paused = 0;

padInfo padinfo;
padData paddata;

void place_food(void) {
    food_x = rand() % GRID_W;
    food_y = rand() % GRID_H;
}

void reset_game(void) {
    snake_len = 6;
    for(int i = 0; i < snake_len; i++) {
        snake[i].x = 12 - i;
        snake[i].y = 13;
    }
    dir_x = 1;
    dir_y = 0;
    score = 0;
    game_over = 0;
    place_food();
}

void update_game(void) {
    if(game_over || paused) return;

    for(int i = snake_len-1; i > 0; i--)
        snake[i] = snake[i-1];

    snake[0].x += dir_x;
    snake[0].y += dir_y;

    if(snake[0].x < 0) snake[0].x = GRID_W-1;
    if(snake[0].x >= GRID_W) snake[0].x = 0;
    if(snake[0].y < 0) snake[0].y = GRID_H-1;
    if(snake[0].y >= GRID_H) snake[0].y = 0;

    if(snake[0].x == food_x && snake[0].y == food_y) {
        if(snake_len < MAX_SNAKE-1) snake_len++;
        score += 10;
        place_food();
    }

    for(int i = 1; i < snake_len; i++) {
        if(snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            game_over = 1;
    }
}

void handle_input(void) {
    ioPadGetInfo(&padinfo);
    if(padinfo.status[0]) {
        ioPadGetData(0, &paddata);

        if(paddata.BTN_UP && dir_y == 0)    { dir_x = 0; dir_y = -1; }
        if(paddata.BTN_DOWN && dir_y == 0)  { dir_x = 0; dir_y = 1; }
        if(paddata.BTN_LEFT && dir_x == 0)  { dir_x = -1; dir_y = 0; }
        if(paddata.BTN_RIGHT && dir_x == 0) { dir_x = 1; dir_y = 0; }

        if(paddata.BTN_START) paused = !paused;
        if(paddata.BTN_CROSS && game_over) reset_game();
    }
}

int main(int argc, const char* argv[]) {
    ioPadInit(7);
    srand(time(NULL));
    reset_game();

    printf("SlitherPS3 iniciado!\n");

    int frame = 0;
    while(1) {
        handle_input();
        frame++;
        if(frame % 5 == 0) update_game();
        usleep(16000);
    }
    return 0;
}