#include "programacao_dinamica.h"
#include "heuristica.h"



Solucao resolver_com_heuristica(const Instancia *instancia) {
    Solucao melhor_solucao = {0};

    for (int povo_inicial = 0; povo_inicial < instancia->num_povos; povo_inicial++) {
        Solucao solucao_atual = {0};
        int visitados[MAX_POVOS] = {0};
        
        int povo_atual = povo_inicial;
        int dist_restante = instancia->distancia_maxima;
        int peso_restante = instancia->peso_maximo;
        int habilidade_total = 0;
        int tamanho_caminho = 0;
        
        while (1) {

            int soldados_max = calcular_soldados_recrutaveis(instancia, povo_atual, peso_restante);
            int soldados_a_recrutar = 0;

            if (soldados_max > 0) {
                // Verifica se ainda há povos interessantes acessíveis
                int proximo_promissor = encontrar_melhor_proximo_povo(instancia, povo_atual, 
                                                                    dist_restante, peso_restante, 
                                                                    visitados);
                
                if (proximo_promissor == -1) {
                    // Se não há próximo povo, recruta todos possíveis
                    soldados_a_recrutar = soldados_max;
                } else {
                    // Se há próximo povo, avalia se vale a pena recrutar aqui
                    double valor_atual = instancia->povos[povo_atual].habilidade;
                    double valor_proximo = instancia->povos[proximo_promissor].habilidade;
                    
                    // Se o povo atual tem boa relação habilidade/peso, recruta
                    if (valor_atual >= valor_proximo * 0.7) {
                        soldados_a_recrutar = soldados_max;
                    }
                    // Senão, só recruta se for muito barato
                    else if (instancia->povos[povo_atual].peso <= 30) {
                        soldados_a_recrutar = soldados_max;
                    }
                }
            }
            
            int habilidade_povo = soldados_a_recrutar * instancia->povos[povo_atual].habilidade;
            int peso_usado = soldados_a_recrutar * instancia->povos[povo_atual].peso;
            
            solucao_atual.soldados_no_caminho[tamanho_caminho] = soldados_a_recrutar;
            tamanho_caminho++;
            
            habilidade_total += habilidade_povo;
            peso_restante -= peso_usado;
            visitados[povo_atual] = 1;
            
            // Encontra o melhor próximo povo
            int proximo_povo = encontrar_melhor_proximo_povo(instancia, povo_atual, 
                                                           dist_restante, peso_restante, 
                                                           visitados);
            
            if (proximo_povo == -1) {
                break; // Não há mais povos alcançáveis
            }
            
            // Move para o próximo povo
            int distancia = instancia->matriz_distancia[povo_atual][proximo_povo];
            dist_restante -= distancia;
            povo_atual = proximo_povo;
        }
        
        // Atualiza melhor solução se necessário
        if (habilidade_total > melhor_solucao.habilidade_maxima) {
            melhor_solucao.habilidade_maxima = habilidade_total;
            melhor_solucao.tamanho_caminho_otimo = tamanho_caminho;
            
            for (int i = 0; i < tamanho_caminho; i++) {
                melhor_solucao.caminho_otimo[i] = solucao_atual.caminho_otimo[i];
                melhor_solucao.soldados_no_caminho[i] = solucao_atual.soldados_no_caminho[i];
            }
        }
    }
    
    return melhor_solucao;
}

double calcular_valor_heuristico(const Instancia *instancia, int povo_idx, int peso_disponivel) {
    // Heurística: razão entre habilidade e peso, multiplicada pelos soldados recrutáveis
    if (instancia->povos[povo_idx].peso == 0) return 0.0;
    
    int soldados = calcular_soldados_recrutaveis(instancia, povo_idx, peso_disponivel);
    double razao_habilidade_peso = (double)instancia->povos[povo_idx].habilidade / 
                                   instancia->povos[povo_idx].peso;
    
    return soldados * razao_habilidade_peso;
}

int encontrar_melhor_proximo_povo(const Instancia *instancia, int povo_atual, 
                                  int dist_restante, int peso_restante,
                                  int *visitados) {
    int melhor_povo = -1;
    double melhor_valor = -1.0;
    
    for (int i = 0; i < instancia->num_povos; i++) {
        if (visitados[i] || i == povo_atual) continue;
        
        int distancia = instancia->matriz_distancia[povo_atual][i];
        
        if (distancia == INF || distancia > dist_restante) continue;
        
        // Calcula valor considerando soldados recrutáveis
        double valor = calcular_valor_heuristico(instancia, i, peso_restante);
        
        // Bônus para povos com alta habilidade mesmo que pesados
        if (instancia->povos[i].habilidade >= 7) {
            valor *= 1.5;
        }
        
        // Penalidade leve pela distância
        valor /= (1.0 + distancia * 0.05);
        
        if (valor > melhor_valor) {
            melhor_valor = valor;
            melhor_povo = i;
        }
    }
    
    return melhor_povo;
}

int calcular_distancia_minima_nao_visitado(const Instancia *instancia, int povo_atual, 
                                           int *visitados) {
    int dist_min = INF;
    
    for (int i = 0; i < instancia->num_povos; i++) {
        if (visitados[i] || i == povo_atual) continue;
        
        int dist = instancia->matriz_distancia[povo_atual][i];
        if (dist < dist_min) {
            dist_min = dist;
        }
    }
    
    return dist_min;
}