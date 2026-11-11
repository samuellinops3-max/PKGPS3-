#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ppu-types.h>
#include <sysutil/sysutil.h>

// Função de retorno exigida pelo sistema do PlayStation 3
void sysutil_callback(uint64_t status, uint64_t param, void* userdata) {
    (void)status;
    (void)param;
    (void)userdata;
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // Registra os eventos de sistema do console
    sysUtilRegisterCallback(SYSUTIL_EVENT_SLOT0, sysutil_callback, NULL);

    printf("SlitherPS3: Inicializado com sucesso!\n");

    // Loop principal do jogo nativo
    while(1) {
        sysUtilCheckCallback();
    }

    // Desregistra o callback antes de fechar (boa prática de Homebrew)
    sysUtilUnregisterCallback(SYSUTIL_EVENT_SLOT0);
    return 0;
}
