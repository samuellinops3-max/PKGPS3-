#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ppu-lv2.h>
#include <sysutil/video.h>
#include <rsx/rsx.h>

// Estrutura básica para controle de tela do PS3
typedef struct {
    int width;
    int height;
    uint32_t *ptr;
} DisplayBuffer;

int main(int argc, char* argv[]) {
    // Inicializa os sistemas do PS3
    sysUtilRegisterCallback(SYSUTIL_EVENT_SLOT0, NULL, NULL);

    // TODO: Configurar o buffer de renderização de tela (RSX)
    // Este é o esqueleto inicial para o Slither.io rodar nativo no PS3

    printf("SlitherPS3 Inicializado com sucesso!\n");

    // Loop infinito básico de homebrew (segura o jogo rodando)
    while(1) {
        sysUtilCheckCallback();
        // O jogo vai rodar aqui dentro
    }

    return 0;
}
