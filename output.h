#ifndef OUTPUT_H
#define OUTPUT_H

#include "types.h"

void imprimir_solucao(const Solucao *solucao, const Instancia *instancia);

void imprimir_comparacao(const Solucao *solucao_dp, const Solucao *solucao_heuristica,
                        const Instancia *instancia);

#endif