#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ppu-types.h>
#include <sysutil/sysutil.h>
#include <pad/pad.h>
#include <tiny3d.h>

// Configurações do Slither.io
#include "pad.h"
#define MAX_FOOD 50
#define SCREEN_WIDTH 848
#define SCREEN_HEIGHT 512

// Estruturas do Jogo
typedef struct {
    float x, y;
    float radius;
    float speed;
    float angle;
} Snake;

typedef struct {
    float x, y;
    uint32_t color;
    int active;
} Food;

// Variáveis Globais
Snake player;
Food pellets[MAX_FOOD];
padInfo pad_data;
padData pad_state;

// Função para gerar comida na arena
void spawn_food(int index) {
    pellets[index].x = (rand() % (SCREEN_WIDTH - 40)) + 20;
    pellets[index].y = (rand() % (SCREEN_HEIGHT - 40)) + 20;
    pellets[index].color = 0xFF0000FF | ((rand() % 255) << 24) | ((rand() % 255) << 16) | ((rand() % 255) << 8);
    pellets[index].active = 1;
}

// Inicialização Gráfica do PS3
void init_game_graphics() {
    tiny3d_Init(1024*1024);
    tiny3d_Project2D();
}

// Desenhar elementos na tela do PS3
void draw_scene() {
    tiny3d_Clear(0x000000FF, CLEAR_BUFFER_COLOR); // Fundo Preto
    tiny3d_AlphaTest(1, 0x10, TINY3D_ALPHA_FUNC_GEQUAL);

    // 1. Desenhar a Comida (Pellets) do Slither.io
    for(int i = 0; i < MAX_FOOD; i++) {
        if(pellets[i].active) {
            tiny3d_SetPolygon(TINY3D_QUADS);
            tiny3d_VertexPos(pellets[i].x - 4, pellets[i].y - 4, 0);
            tiny3d_VertexColor(pellets[i].color);
            tiny3d_VertexPos(pellets[i].x + 4, pellets[i].y - 4, 0);
            tiny3d_VertexPos(pellets[i].x + 4, pellets[i].y + 4, 0);
            tiny3d_VertexPos(pellets[i].x - 4, pellets[i].y + 4, 0);
            tiny3d_End();
        }
    }

    // 2. Desenhar a Cabeça da Cobra do Jogador
    tiny3d_SetPolygon(TINY3D_QUADS);
    tiny3d_VertexPos(player.x - player.radius, player.y - player.radius, 0);
    tiny3d_VertexColor(0x00FF00FF); // Cor Verde Ciano para a Cobra
    tiny3d_VertexPos(player.x + player.radius, player.y - player.radius, 0);
    tiny3d_VertexPos(player.x + player.radius, player.y + player.radius, 0);
    tiny3d_VertexPos(player.x - player.radius, player.y + player.radius, 0);
    tiny3d_End();

    tiny3d_Flip();
}

void sysutil_callback(uint64_t status, uint64_t param, void* userdata) {
    (void)status; (void)param; (void)userdata;
}

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    
    sysUtilRegisterCallback(SYSUTIL_EVENT_SLOT0, sysutil_callback, NULL);
    ioPadInit(7);
    init_game_graphics();

    // Inicializa o Jogador no centro da tela
    player.x = SCREEN_WIDTH / 2.0f;
    player.y = SCREEN_HEIGHT / 2.0f;
    player.radius = 12.0f;
    player.speed = 3.0f;

    // Inicializa as comidas
    for(int i = 0; i < MAX_FOOD; i++) spawn_food(i);

    // Loop Principal do Jogo Nativo de PS3
    while(1) {
        sysUtilCheckCallback();
        
        // Ler comandos do Analógico do DualShock 3
        ioPadGetInfo(&pad_data);
        if(pad_data.status[0]) {
            ioPadGetData(0, &pad_state);
            
            // Mover usando o analógico esquerdo do controle
            float posX = (pad_state.ANA_L_X - 128) / 128.0f;
            float posY = (pad_state.ANA_L_Y - 128) / 128.0f;

            if(fabs(posX) > 0.15f) player.x += posX * player.speed;
            if(fabs(posY) > 0.15f) player.y += posY * player.speed;
        }

        // Limites da tela para a cobra não fugir
        if(player.x < 0) player.x = SCREEN_WIDTH;
        if(player.x > SCREEN_WIDTH) player.x = 0;
        if(player.y < 0) player.y = SCREEN_HEIGHT;
        if(player.y > SCREEN_HEIGHT) player.y = 0;

        // Lógica de colisão: Comer as esferas brilhantes
        for(int i = 0; i < MAX_FOOD; i++) {
            if(pellets[i].active) {
                float dx = player.x - pellets[i].x;
                float dy = player.y - pellets[i].y;
                float distance = sqrtf(dx*dx + dy*dy);

                if(distance < player.radius + 4) {
                    pellets[i].active = 0;
                    player.radius += 0.2f; // Cobra cresce quando come!
                    spawn_food(i); // Nasce outra em lugar aleatório
                }
            }
        }

        draw_scene();
    }

    ioPadEnd();
    sysUtilUnregisterCallback(SYSUTIL_EVENT_SLOT0);
    return 0;
}
