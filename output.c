#include "output.h"
#include <stdio.h>

void imprimir_solucao(const Solucao *solucao, const Instancia *instancia) {
    printf("%d", solucao->habilidade_maxima);

    for (int i = 0; i < solucao->tamanho_caminho_otimo; i++) {
        int idx = solucao->caminho_otimo[i]; 
        int povo_id = instancia->povos[idx].id;
        int soldados = solucao->soldados_no_caminho[i]; 
        printf(" %d %d", povo_id, soldados);
    }
    printf("\n");
}

void imprimir_comparacao(const Solucao *solucao_dp, const Solucao *solucao_heuristica,
                         const Instancia *instancia) {
    printf("=== COMPARAÇÃO DAS SOLUÇÕES ===\n");

    printf("\nPROGRAMAÇÃO DINÂMICA:\n");
    imprimir_solucao(solucao_dp, instancia);

    printf("\nHEURÍSTICA GULOSA:\n");
    imprimir_solucao(solucao_heuristica, instancia);

    printf("\nRESUMO:\n");
    printf("DP: %d pontos de habilidade\n", solucao_dp->habilidade_maxima);
    printf("Heurística: %d pontos de habilidade\n", solucao_heuristica->habilidade_maxima);

    if (solucao_dp->habilidade_maxima > solucao_heuristica->habilidade_maxima) {
        printf("Programação Dinâmica encontrou solução melhor!\n");
    } else if (solucao_heuristica->habilidade_maxima > solucao_dp->habilidade_maxima) {
        printf("Heurística encontrou solução melhor!\n");
    } else {
        if (solucao_dp->habilidade_maxima == 0 && solucao_heuristica->habilidade_maxima == 0) {
            printf("Ambas não encontraram solução com habilidade positiva ou não encontraram solução.\n");
        } else {
            printf("Ambas encontraram soluções equivalentes!\n");
        }
    }

    printf("===========================\n\n");
}