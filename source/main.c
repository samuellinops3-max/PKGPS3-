#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include <sys/process.h>
#include <sysutil/sysutil.h>
#include <sysutil/video.h>
#include <io/pad.h>
#include <rsx/rsx.h>
#include <sys/memory.h>

#define MAX_SNAKE 200
#define GRID_W 40
#define GRID_H 22
#define CELL 16

typedef struct {
    int x, y;
} Point;

Point snake[MAX_SNAKE];
int snake_len = 5;
int dir_x = 1, dir_y = 0;
int food_x, food_y;
int score = 0;
int game_over = 0;
int paused = 0;

padInfo padinfo;
padData paddata;

void place_food() {
    food_x = rand() % GRID_W;
    food_y = rand() % GRID_H;
}

void reset_game() {
    snake_len = 5;
    for(int i = 0; i < snake_len; i++) {
        snake[i].x = 10 - i;
        snake[i].y = 10;
    }
    dir_x = 1;
    dir_y = 0;
    score = 0;
    game_over = 0;
    place_food();
}

void update_game() {
    if(game_over || paused) return;

    // Move body
    for(int i = snake_len - 1; i > 0; i--) {
        snake[i] = snake[i-1];
    }
    snake[0].x += dir_x;
    snake[0].y += dir_y;

    // Wall wrap (estilo Slither)
    if(snake[0].x < 0) snake[0].x = GRID_W - 1;
    if(snake[0].x >= GRID_W) snake[0].x = 0;
    if(snake[0].y < 0) snake[0].y = GRID_H - 1;
    if(snake[0].y >= GRID_H) snake[0].y = 0;

    // Eat food
    if(snake[0].x == food_x && snake[0].y == food_y) {
        if(snake_len < MAX_SNAKE - 1) snake_len++;
        score += 10;
        place_food();
    }

    // Self collision
    for(int i = 1; i < snake_len; i++) {
        if(snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            game_over = 1;
        }
    }
}

void handle_input() {
    ioPadGetInfo(&padinfo);
    if(padinfo.status[0]) {
        ioPadGetData(0, &paddata);

        if(paddata.BTN_UP && dir_y == 0) { dir_x = 0; dir_y = -1; }
        if(paddata.BTN_DOWN && dir_y == 0) { dir_x = 0; dir_y = 1; }
        if(paddata.BTN_LEFT && dir_x == 0) { dir_x = -1; dir_y = 0; }
        if(paddata.BTN_RIGHT && dir_x == 0) { dir_x = 1; dir_y = 0; }

        if(paddata.BTN_START) paused = !paused;
        if(paddata.BTN_CROSS && game_over) reset_game();
    }
}

// Callback pra sair pelo botão PS
void sysutil_exit_callback(u64 status, u64 param, void *userdata) {
    if(status == SYSUTIL_EXIT_GAME) {
        // sair
    }
}

int main(int argc, const char* argv[]) {
    // Inicialização básica
    ioPadInit(7);
    sysUtilRegisterCallback(SYSUTIL_EVENT_SLOT0, sysutil_exit_callback, NULL);

    srand(12345);
    reset_game();

    printf("SlitherPS3 iniciado!\n");
    printf("Controles: D-Pad = mover | Start = pause | X = reiniciar\n");

    int frame = 0;
    while(1) {
        sysUtilCheckCallback();

        handle_input();

        frame++;
        if(frame % 6 == 0) {   // velocidade
            update_game();
        }

        // Aqui no futuro a gente desenha com Tiny3D / RSX
        // Por enquanto só roda a lógica (você vê pelo TTY ou depois adiciona gráfico)

        usleep(16000); // \~60fps
    }

    return 0;
}