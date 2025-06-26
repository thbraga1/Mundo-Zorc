#include "input.h"

int ler_instancia(FILE *arquivo, Instancia *instancia) {
    if (fscanf(arquivo, "%d %d %d %d", 
               &instancia->num_povos, 
               &instancia->distancia_maxima,
               &instancia->peso_maximo,
               &instancia->num_caminhos) != 4) {
        return 0; 
    }

    for (int i = 0; i < instancia->num_povos; i++) {
        if (fscanf(arquivo, "%d %d %d",
                   &instancia->povos[i].id,
                   &instancia->povos[i].peso,
                   &instancia->povos[i].habilidade) != 3) {
            return 0;
        }
    }

    inicializar_matriz_distancia(instancia);

    for (int i = 0; i < instancia->num_caminhos; i++) {
        if (fscanf(arquivo, "%d %d %d",
                   &instancia->caminhos[i].origem,
                   &instancia->caminhos[i].destino,
                   &instancia->caminhos[i].distancia) != 3) {
            return 0;
        }

        int origem = instancia->caminhos[i].origem - 1; 
        int destino = instancia->caminhos[i].destino - 1;
        int dist = instancia->caminhos[i].distancia;
        
        instancia->matriz_distancia[origem][destino] = dist;
        instancia->matriz_distancia[destino][origem] = dist;
    }
    
    return 1;
}

void inicializar_matriz_distancia(Instancia *instancia) {
    for (int i = 0; i < instancia->num_povos; i++) {
        for (int j = 0; j < instancia->num_povos; j++) {
            if (i == j) {
                instancia->matriz_distancia[i][j] = 0;
            } else {
                instancia->matriz_distancia[i][j] = INF;
            }
        }
    }
}

void imprimir_instancia(const Instancia *instancia) {
    printf("Povos: %d, Dist Max: %d, Peso Max: %d, Caminhos: %d\n",
           instancia->num_povos, instancia->distancia_maxima,
           instancia->peso_maximo, instancia->num_caminhos);
           
    printf("Povos:\n");
    for (int i = 0; i < instancia->num_povos; i++) {
        printf("  %d: peso=%d, habilidade=%d\n",
               instancia->povos[i].id,
               instancia->povos[i].peso,
               instancia->povos[i].habilidade);
    }
    
    printf("Matriz de distâncias:\n");
    for (int i = 0; i < instancia->num_povos; i++) {
        for (int j = 0; j < instancia->num_povos; j++) {
            if (instancia->matriz_distancia[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", instancia->matriz_distancia[i][j]);
            }
        }
        printf("\n");
    }
}