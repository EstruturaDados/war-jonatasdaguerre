#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// --- PROTÓTIPOS ---
void cadastrarTerritorios(Territorio* mapa, int n);
void exibirTerritorios(Territorio* mapa, int n);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);

// ----------------------------------------------------------
int main() {

    srand(time(NULL)); // garante aleatoriedade

    int n;
    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &n);
    getchar(); // limpar buffer

    Territorio* mapa = (Territorio*)calloc(n, sizeof(Territorio));

    if (mapa == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n);

    int opcao;
    do {
        printf("\n--- MENU ---\n");
        printf("1 - Exibir territorios\n");
        printf("2 - Realizar ataque\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            exibirTerritorios(mapa, n);
        }
        else if (opcao == 2) {
            int a, d;
            exibirTerritorios(mapa, n);

            printf("\nEscolha o indice do territorio ATACANTE: ");
            scanf("%d", &a);
            printf("Escolha o indice do territorio DEFENSOR: ");
            scanf("%d", &d);

            if (a < 0 || a >= n || d < 0 || d >= n) {
                printf("Indices invalidos!\n");
                continue;
            }
            if (strcmp(mapa[a].cor, mapa[d].cor) == 0) {
                printf("Nao pode atacar um territorio da mesma cor!\n");
                continue;
            }

            atacar(&mapa[a], &mapa[d]);
        }

    } while (opcao != 0);

    liberarMemoria(mapa);
    return 0;
}
// ----------------------------------------------------------


// FUNÇÃO PARA CADASTRAR OS TERRITÓRIOS
void cadastrarTerritorios(Territorio* mapa, int n) {

    for (int i = 0; i < n; i++) {
        printf("\n--- Cadastro do Territorio %d ---\n", i);

        printf("Nome: ");
        getchar(); 
        fgets(mapa[i].nome, 30, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor (ex: vermelho, azul): ");
        fgets(mapa[i].cor, 10, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

// FUNÇÃO PARA EXIBIR TODOS OS TERRITÓRIOS
void exibirTerritorios(Territorio* mapa, int n) {

    printf("\n--- LISTA DE TERRITORIOS ---\n");

    for (int i = 0; i < n; i++) {
        printf("\nID: %d\n", i);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
    }
}

// FUNÇÃO DE ATAQUE ENTRE TERRITÓRIOS
void atacar(Territorio* atacante, Territorio* defensor) {

    if (atacante->tropas < 1) {
        printf("O territorio atacante nao possui tropas suficientes!\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- BATALHA ---\n");
    printf("Atacante (%s) rolou: %d\n", atacante->nome, dadoAtacante);
    printf("Defensor  (%s) rolou: %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("\nAtacante venceu!\n");

        strcpy(defensor->cor, atacante->cor);

        defensor->tropas = atacante->tropas / 2;
        atacante->tropas /= 2;

        if (defensor->tropas < 1) defensor->tropas = 1;

        printf("\nTerritorio conquistado!\n");
    }
    else {
        printf("\nDefensor venceu!\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }

    printf("\n--- Dados atualizados ---\n");
    printf("Atacante: %s | Tropas: %d | Cor: %s\n",
           atacante->nome, atacante->tropas, atacante->cor);
    printf("Defensor: %s | Tropas: %d | Cor: %s\n",
           defensor->nome, defensor->tropas, defensor->cor);
}

// LIBERA A MEMÓRIA ALOCADA
void liberarMemoria(Territorio* mapa) {
    free(mapa);
    printf("\nMemoria liberada com sucesso!\n");
}
