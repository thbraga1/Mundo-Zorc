#ifndef INPUT_H
#define INPUT_H

#include "types.h"

int ler_instancia(FILE *arquivo, Instancia *instancia);

void inicializar_matriz_distancia(Instancia *instancia);

void imprimir_instancia(const Instancia *instancia);

#endif