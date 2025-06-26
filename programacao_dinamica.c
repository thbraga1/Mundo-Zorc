#include "programacao_dinamica.h" 
#include <string.h>               
#include <stdbool.h>              
#include <stdio.h>               


#define MAX_DIST_MEMO 101  
#define MAX_PESO_MEMO 501  

typedef struct {
    int k_recrutados;     
    int proximo_povo_idx; 
} EscolhaDP;

static int memo_habilidade[MAX_POVOS][MAX_DIST_MEMO][MAX_PESO_MEMO];
static EscolhaDP memo_escolhas[MAX_POVOS][MAX_DIST_MEMO][MAX_PESO_MEMO];
static bool calculado[MAX_POVOS][MAX_DIST_MEMO][MAX_PESO_MEMO];

static int dp_engine_recursive(const Instancia *instancia, int povo_atual_idx,
                               int dist_restante, int peso_restante,
                               int visitados_caminho_atual[]);

int calcular_soldados_recrutaveis(const Instancia *instancia, int povo_idx, int peso_disponivel) {
    if (povo_idx < 0 || povo_idx >= instancia->num_povos) return 0;
    int peso_soldado = instancia->povos[povo_idx].peso;
    if (peso_soldado <= 0) return 0; 
    return peso_disponivel / peso_soldado;
}

Solucao resolver_com_dp(const Instancia *instancia) {
    Solucao melhor_solucao_final = {0}; 
    int povo_inicial_otimo = -1;
    int habilidade_max_geral = 0; 

    if (instancia->distancia_maxima >= MAX_DIST_MEMO || instancia->peso_maximo >= MAX_PESO_MEMO) {
        fprintf(stderr, "ERRO: Limites da instância (D=%d, W=%d) excedem os limites da tabela de memoização (MAX_DIST_MEMO=%d, MAX_PESO_MEMO=%d).\n",
                instancia->distancia_maxima, instancia->peso_maximo, MAX_DIST_MEMO, MAX_PESO_MEMO);
        fprintf(stderr, "Ajuste MAX_DIST_MEMO/MAX_PESO_MEMO ou implemente alocação dinâmica.\n");

        melhor_solucao_final.habilidade_maxima = -1; 
        return melhor_solucao_final;
    }

    for (int i = 0; i < instancia->num_povos; ++i) {
        for (int j = 0; j <= instancia->distancia_maxima; ++j) {
            memset(calculado[i][j], false, (instancia->peso_maximo + 1) * sizeof(bool));
        }
    }


    for (int idx_povo_inicial = 0; idx_povo_inicial < instancia->num_povos; ++idx_povo_inicial) {
        int visitados[MAX_POVOS]; 
        memset(visitados, 0, sizeof(visitados));

        int habilidade_desta_partida = dp_engine_recursive(instancia, idx_povo_inicial,
                                                           instancia->distancia_maxima,
                                                           instancia->peso_maximo,
                                                           visitados);

    
        if (habilidade_desta_partida >= habilidade_max_geral) {
             if (habilidade_desta_partida > habilidade_max_geral) {
                habilidade_max_geral = habilidade_desta_partida;
                povo_inicial_otimo = idx_povo_inicial;
            } else if (habilidade_desta_partida == habilidade_max_geral) {
                if (povo_inicial_otimo == -1 || idx_povo_inicial < povo_inicial_otimo) {
                     povo_inicial_otimo = idx_povo_inicial;
                }
            }
        }
    }
    melhor_solucao_final.habilidade_maxima = habilidade_max_geral;


    if (povo_inicial_otimo != -1 && melhor_solucao_final.habilidade_maxima > 0) { 
        int idx_povo_corrente = povo_inicial_otimo;
        int dist_corrente = instancia->distancia_maxima;
        int peso_corrente = instancia->peso_maximo;
        melhor_solucao_final.tamanho_caminho_otimo = 0;

        while (melhor_solucao_final.tamanho_caminho_otimo < MAX_POVOS && idx_povo_corrente != -1) {

            if (dist_corrente < 0 || peso_corrente < 0 ||
                !calculado[idx_povo_corrente][dist_corrente][peso_corrente]) {
        
                break; 
            }

            EscolhaDP escolha_feita = memo_escolhas[idx_povo_corrente][dist_corrente][peso_corrente];

            melhor_solucao_final.caminho_otimo[melhor_solucao_final.tamanho_caminho_otimo] = idx_povo_corrente;
            melhor_solucao_final.soldados_no_caminho[melhor_solucao_final.tamanho_caminho_otimo] = escolha_feita.k_recrutados;
            melhor_solucao_final.tamanho_caminho_otimo++;

            if (escolha_feita.proximo_povo_idx == -1) { 
                break;
            }

            int peso_gasto_neste_povo = escolha_feita.k_recrutados * instancia->povos[idx_povo_corrente].peso;
            int dist_gasta_proximo = instancia->matriz_distancia[idx_povo_corrente][escolha_feita.proximo_povo_idx];

            if (dist_gasta_proximo == INF) {
                break; 
            }
            
            dist_corrente -= dist_gasta_proximo;
            peso_corrente -= peso_gasto_neste_povo;
            idx_povo_corrente = escolha_feita.proximo_povo_idx;
        }
    } else if (melhor_solucao_final.habilidade_maxima == 0 && povo_inicial_otimo != -1) {
        if (calculado[povo_inicial_otimo][instancia->distancia_maxima][instancia->peso_maximo]) {
             EscolhaDP escolha_inicial = memo_escolhas[povo_inicial_otimo][instancia->distancia_maxima][instancia->peso_maximo];
             if (escolha_inicial.k_recrutados == 0 && escolha_inicial.proximo_povo_idx == -1 && melhor_solucao_final.tamanho_caminho_otimo == 0) {
                 melhor_solucao_final.caminho_otimo[0] = povo_inicial_otimo;
                 melhor_solucao_final.soldados_no_caminho[0] = 0;
                 melhor_solucao_final.tamanho_caminho_otimo = 1;
             }
        }
    }


    return melhor_solucao_final;
}

static int dp_engine_recursive(const Instancia *instancia, int povo_atual_idx,
                               int dist_restante, int peso_restante,
                               int visitados_caminho_atual[]) {
    if (dist_restante < 0) {
        return -1000000000; 
    }
    
    if (calculado[povo_atual_idx][dist_restante][peso_restante]) {
        return memo_habilidade[povo_atual_idx][dist_restante][peso_restante];
    }

    visitados_caminho_atual[povo_atual_idx] = 1; 

    int max_habilidade_para_este_estado = 0; 
    EscolhaDP melhor_escolha_neste_estado = {0, -1}; 

    int peso_soldado_povo_atual = instancia->povos[povo_atual_idx].peso;
    int habilidade_soldado_povo_atual = instancia->povos[povo_atual_idx].habilidade;
    int max_k_a_recrutar = calcular_soldados_recrutaveis(instancia, povo_atual_idx, peso_restante);

    for (int k_recrutados = 0; k_recrutados <= max_k_a_recrutar; ++k_recrutados) {
        int habilidade_neste_povo_com_k = k_recrutados * habilidade_soldado_povo_atual;
        int peso_restante_apos_recrutar_k = peso_restante - (k_recrutados * peso_soldado_povo_atual);

        if (habilidade_neste_povo_com_k >= max_habilidade_para_este_estado) {
            if (habilidade_neste_povo_com_k > max_habilidade_para_este_estado) {
                max_habilidade_para_este_estado = habilidade_neste_povo_com_k;
                melhor_escolha_neste_estado.k_recrutados = k_recrutados;
                melhor_escolha_neste_estado.proximo_povo_idx = -1;
            } else if (habilidade_neste_povo_com_k == max_habilidade_para_este_estado) {
            }
        }

        for (int proximo_p_idx = 0; proximo_p_idx < instancia->num_povos; ++proximo_p_idx) {
            if (visitados_caminho_atual[proximo_p_idx]) {
                continue; 
            }

            int dist_para_proximo = instancia->matriz_distancia[povo_atual_idx][proximo_p_idx];

            if (dist_para_proximo != INF && dist_restante >= dist_para_proximo) {
                int habilidade_caminho_subsequente = dp_engine_recursive(instancia, proximo_p_idx,
                                                                     dist_restante - dist_para_proximo,
                                                                     peso_restante_apos_recrutar_k,
                                                                     visitados_caminho_atual);

                if (habilidade_caminho_subsequente > -1000000000) { 
                    int habilidade_total_com_movimento = habilidade_neste_povo_com_k + habilidade_caminho_subsequente;

                    if (habilidade_total_com_movimento > max_habilidade_para_este_estado) {
                        max_habilidade_para_este_estado = habilidade_total_com_movimento;
                        melhor_escolha_neste_estado.k_recrutados = k_recrutados;
                        melhor_escolha_neste_estado.proximo_povo_idx = proximo_p_idx;
                    } else if (habilidade_total_com_movimento == max_habilidade_para_este_estado) {
                    }
                }
            }
        }
    }

    visitados_caminho_atual[povo_atual_idx] = 0; 


    memo_habilidade[povo_atual_idx][dist_restante][peso_restante] = max_habilidade_para_este_estado;
    memo_escolhas[povo_atual_idx][dist_restante][peso_restante] = melhor_escolha_neste_estado;
    calculado[povo_atual_idx][dist_restante][peso_restante] = true;

    return max_habilidade_para_este_estado;
}

int*** alocar_memo(int num_povos, int dist_max, int peso_max) {
    (void)num_povos; (void)dist_max; (void)peso_max; 
    return NULL;
}

void liberar_memo(int ***memo, int num_povos, int dist_max) {
    (void)memo; (void)num_povos; (void)dist_max; 
}