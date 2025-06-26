#ifndef PROGRAMACAO_DINAMICA_H
#define PROGRAMACAO_DINAMICA_H

#include "types.h"

Solucao resolver_com_dp(const Instancia *instancia);

int calcular_soldados_recrutaveis(const Instancia *instancia, int povo_idx, int peso_disponivel);

void busca_exaustiva(const Instancia *instancia, int povo_atual, int dist_restante,
                    int peso_restante, int habilidade_atual, Solucao *solucao_atual,
                    Solucao *melhor_solucao, int nivel, int *visitados);

int calcular_soldados_recrutaveis(const Instancia *instancia, int povo_idx, int peso_disponivel);

#endif