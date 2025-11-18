#include <stdio.h>
#include <string.h>

/*
    Definição da struct Territorio:
    Armazena nome, cor do exército e número de tropas.
*/
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main() {
    // Vetor de 5 territórios
    Territorio t[5];

    // Cadastro dos territórios
    for (int i = 0; i < 5; i++) {
        printf("\n=== Cadastro do Território %d ===\n", i + 1);

        printf("Nome do território: ");
        scanf("%s", t[i].nome);

        printf("Cor do exército: ");
        scanf("%s", t[i].cor);

        printf("Quantidade de tropas: ");
        scanf("%d", &t[i].tropas);
    }

    // Exibição dos dados cadastrados
    printf("\n\n===== TERRITÓRIOS REGISTRADOS =====\n");

    for (int i = 0; i < 5; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("Nome: %s\n", t[i].nome);
        printf("Cor do exército: %s\n", t[i].cor);
        printf("Tropas: %d\n", t[i].tropas);
    }

    return 0;
}
