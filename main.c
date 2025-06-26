#include <stdio.h>   
#include <string.h>  
#include "types.h"
#include "input.h"
#include "programacao_dinamica.h"
#include "heuristica.h"
#include "output.h"

int main(int argc, char *argv[]) {
    char *nome_arquivo_entrada = NULL;

    if (argc == 3 && strcmp(argv[1], "-i") == 0) {
        nome_arquivo_entrada = argv[2];
    } else {
        fprintf(stderr, "Uso: %s -i <arquivo_de_entrada>\n", argv[0]);
        fprintf(stderr, "Exemplo: %s -i entrada.txt\n", argv[0]);
        return 1; 
    }

    FILE *arquivo = fopen(nome_arquivo_entrada, "r");
    if (!arquivo) {
        fprintf(stderr, "Erro ao abrir arquivo: %s\n", nome_arquivo_entrada);
        return 1; 
    }

    int num_instancias;
    if (fscanf(arquivo, "%d", &num_instancias) != 1) {
        fprintf(stderr, "Erro ao ler o número de instâncias do arquivo.\n");
        fclose(arquivo);
        return 1; 
    }

    for (int i = 0; i < num_instancias; i++) {
        Instancia instancia;

        if (!ler_instancia(arquivo, &instancia)) {
            fprintf(stderr, "Erro ao ler dados para a instância %d.\n", i + 1);
            fclose(arquivo);
            return 1; 
        }

        Solucao solucao_dp = resolver_com_dp(&instancia);

        imprimir_solucao(&solucao_dp, &instancia);

    }

    fclose(arquivo);
    return 0;
}