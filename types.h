#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_POVOS 100
#define MAX_CAMINHOS 1000
#define INF INT_MAX

// Estrutura para representar um povo
typedef struct {
    int id;
    int peso;
    int habilidade;
} Povo;

// Estrutura para representar um caminho entre povos
typedef struct {
    int origem;
    int destino;
    int distancia;
} Caminho;

// Estrutura para representar uma instância do problema
typedef struct {
    int num_povos;
    int distancia_maxima;
    int peso_maximo;
    int num_caminhos;
    Povo povos[MAX_POVOS];
    Caminho caminhos[MAX_CAMINHOS];
    int matriz_distancia[MAX_POVOS][MAX_POVOS];
} Instancia;

// Estrutura para representar um estado na programação dinâmica
typedef struct {
    int povo_atual;
    int distancia_restante;
    int peso_restante;
    int habilidade_total;
    int caminho[MAX_POVOS];
    int tamanho_caminho;
    int soldados_recrutados[MAX_POVOS];
} Estado;

// Estrutura para a solução
typedef struct {
    int habilidade_maxima;
    int caminho_otimo[MAX_POVOS];
    int tamanho_caminho_otimo;
    int soldados_no_caminho[MAX_POVOS];
} Solucao;

#endif