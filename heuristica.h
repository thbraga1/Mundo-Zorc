#ifndef HEURISTICA_H
#define HEURISTICA_H

#include "types.h"

Solucao resolver_com_heuristica(const Instancia *instancia);

double calcular_valor_heuristico(const Instancia *instancia, int povo_idx, int peso_disponivel);

int encontrar_melhor_proximo_povo(const Instancia *instancia, int povo_atual, 
                                  int dist_restante, int peso_restante,
                                  int *visitados);

int calcular_distancia_minima_nao_visitado(const Instancia *instancia, int povo_atual, 
                                           int *visitados);

#endif