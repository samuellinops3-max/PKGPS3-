#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ppu-types.h>
#include <sysutil/sysutil.h>

// Função padrão de callback exigida pelo sistema do console
void event_callback(uint64_t status, uint64_t param, void* userdata) {
    (void)status;
    (void)param;
    (void)userdata;
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // Inicializa as chamadas de sistema do PS3
    sysUtilRegisterCallback(SYSUTIL_EVENT_SLOT0, event_callback, NULL);

    printf("SlitherPS3: Base do jogo carregada com sucesso!\n");

    // Loop do jogo nativo
    while(1) {
        sysUtilCheckCallback();
    }

    sysUtilUnregisterCallback(SYSUTIL_EVENT_SLOT0);
    return 0;
}
