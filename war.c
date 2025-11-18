#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// War estruturado - Missões Estratégicas
// Este arquivo implementa:
// - Estrutura Territorio
// - Alocação dinâmica do mapa
// - Vetor de missões (strings)
// - Atribuição aleatória de missões a jogadores (com malloc)
// - Verificação simples de cumprimento de missão
// - Função de ataque simulada com rand()
// - Modularização e liberação de memória

#define MAX_NOME 30
#define MAX_COR 10
#define MAX_MISSAO 128

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// Prototipos
void atribuirMissao(char **destino, char *missoes[], int totalMissoes);
int verificarMissao(char *missao, Territorio *mapa, int tamanho);
void exibirMissao(const char *missao); // passagem por valor (const)
void atacar(Territorio *atacante, Territorio *defensor);
void exibirMapa(Territorio *mapa, int tamanho);
void liberarMemoria(Territorio *mapa, int tamanho, char **missoesJogadores, int jogadores);

// Implementação

// Sorteia uma missão e aloca memória para 'destino' (passagem por referência)
void atribuirMissao(char **destino, char *missoes[], int totalMissoes) {
    if (totalMissoes <= 0) return;
    int idx = rand() % totalMissoes;
    size_t len = strlen(missoes[idx]) + 1;
    *destino = (char *) malloc(len);
    if (*destino == NULL) {
        fprintf(stderr, "Erro ao alocar memória para missão\n");
        exit(EXIT_FAILURE);
    }
    strcpy(*destino, missoes[idx]);
}

// Exibe missão (passagem por valor / apenas leitura)
void exibirMissao(const char *missao) {
    printf("Sua missão: %s\n", missao);
}

// Verificação simples de missão: procura por palavras-chave nas descrições
// Retorna 1 se cumprida, 0 caso contrário
int verificarMissao(char *missao, Territorio *mapa, int tamanho) {
    if (missao == NULL) return 0;

    // Missão: conquistar N territórios (procura por "Conquistar X")
    if (strstr(missao, "Conquistar 3") != NULL) {
        // Verificar se existe sequência de 3 territórios da mesma cor em qualquer posição
        for (int i = 0; i <= tamanho - 3; ++i) {
            if (strcmp(mapa[i].cor, mapa[i+1].cor) == 0 && strcmp(mapa[i].cor, mapa[i+2].cor) == 0) {
                // assumimos que pertencem ao jogador quando cor != "N" (nulo) - mas em jogo real a cor indica dono
                // Para simplicidade: se cor não for "N", consideramos como conquistado pelo mesmo jogador
                if (strcmp(mapa[i].cor, "N") != 0) return 1;
            }
        }
        return 0;
    }

    if (strstr(missao, "Conquistar 5") != NULL) {
        int count = 0;
        // Conta se existe alguma cor que aparece em pelo menos 5 territórios
        for (int i = 0; i < tamanho; ++i) {
            int c = 1;
            for (int j = i+1; j < tamanho; ++j) {
                if (strcmp(mapa[i].cor, mapa[j].cor) == 0) c++;
            }
            if (c >= 5 && strcmp(mapa[i].cor, "N") != 0) return 1;
        }
        return 0;
    }

    if (strstr(missao, "Eliminar todas as tropas da cor vermelha") != NULL) {
        for (int i = 0; i < tamanho; ++i) {
            if (strcmp(mapa[i].cor, "VERMELHA") == 0 && mapa[i].tropas > 0) return 0;
        }
        return 1; // nenhuma tropa vermelha encontrada
    }

    if (strstr(missao, "Controlar 3 territorios de cor azul") != NULL) {
        int count = 0;
        for (int i = 0; i < tamanho; ++i) if (strcmp(mapa[i].cor, "AZUL") == 0) count++;
        return (count >= 3) ? 1 : 0;
    }

    // Missão padrão: se a string contiver "Conquistar 3 territorios seguidos"
    if (strstr(missao, "seguidos") != NULL || strstr(missao, "seguidos") != NULL) {
        // simplificação: reaproveita o caso de 3
        for (int i = 0; i <= tamanho - 3; ++i) {
            if (strcmp(mapa[i].cor, mapa[i+1].cor) == 0 && strcmp(mapa[i].cor, mapa[i+2].cor) == 0) {
                if (strcmp(mapa[i].cor, "N") != 0) return 1;
            }
        }
        return 0;
    }

    // Se a missão não for reconhecida (futuras expansões), retorna 0
    return 0;
}

// Simula um ataque entre territórios
void atacar(Territorio *atacante, Territorio *defensor) {
    if (atacante == NULL || defensor == NULL) return;
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Ataque inválido: mesma cor.\n");
        return;
    }
    if (atacante->tropas <= 1) {
        printf("Ataque inválido: tropas insuficientes (min 2).\n");
        return;
    }

    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;

    printf("Rolagem: atacante %d x defensor %d\n", dadoA, dadoD);

    if (dadoA > dadoD) {
        // atacante vence: transfere cor e metade das tropas para o defensor
        int transferencia = atacante->tropas / 2; // metade das tropas do atacante
        if (transferencia < 1) transferencia = 1; // ao menos 1

        atacante->tropas -= transferencia;
        defensor->tropas = transferencia;
        strncpy(defensor->cor, atacante->cor, MAX_COR-1);
        defensor->cor[MAX_COR-1] = '\0';

        printf("Atacante vence! %d tropas transferidas. %s agora é %s com %d tropas.\n",
               transferencia, defensor->nome, defensor->cor, defensor->tropas);
    } else {
        // defensor vence: atacante perde 1 tropa
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("Defensor vence! Atacante perde 1 tropa (agora %d).\n", atacante->tropas);
    }
}

// Exibe o mapa (lista de territórios)
void exibirMapa(Territorio *mapa, int tamanho) {
    printf("--- MAPA ---\n");
    for (int i = 0; i < tamanho; ++i) {
        printf("%2d) %-15s | Cor: %-8s | Tropas: %2d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("------------\n");
}

// Libera memória alocada (mapa e missões dos jogadores)
void liberarMemoria(Territorio *mapa, int tamanho, char **missoesJogadores, int jogadores) {
    if (mapa != NULL) free(mapa);
    if (missoesJogadores != NULL) {
        for (int i = 0; i < jogadores; ++i) {
            if (missoesJogadores[i] != NULL) free(missoesJogadores[i]);
        }
        free(missoesJogadores);
    }
}

int main() {
    srand((unsigned int) time(NULL));

    // Criando mapa dinamicamente
    int numTerritorios = 8;
    Territorio *mapa = (Territorio *) calloc(numTerritorios, sizeof(Territorio));
    if (!mapa) {
        fprintf(stderr, "Falha ao alocar mapa\n");
        return 1;
    }

    // Inicializa alguns territórios (nomes, cores e tropas)
    // Em um jogo real, as cores indicariam o dono do território
    char *nomes[] = {"Amazônia", "Pampas", "Pantanal", "Caatinga", "MataAtl", "Cerrado", "Serra", "Litoral"};
    char *cores_init[] = {"VERMELHA", "AZUL", "VERDE", "N", "N", "N", "N", "N"};
    int tropas_init[] = {4, 3, 5, 2, 2, 1, 1, 1};

    for (int i = 0; i < numTerritorios; ++i) {
        strncpy(mapa[i].nome, nomes[i], MAX_NOME-1);
        mapa[i].nome[MAX_NOME-1] = '\0';
        strncpy(mapa[i].cor, cores_init[i], MAX_COR-1);
        mapa[i].cor[MAX_COR-1] = '\0';
        mapa[i].tropas = tropas_init[i];
    }

    // Vetor de descrições de missões
    char *missoesDisponiveis[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Conquistar 5 territorios",
        "Controlar 3 territorios de cor azul",
        "Conquistar 3 territorios seguidos e eliminar todas as tropas da cor vermelha"
    };
    int totalMissoes = sizeof(missoesDisponiveis) / sizeof(missoesDisponiveis[0]);

    // Número de jogadores
    int jogadores = 2;
    // Aloca vetor de ponteiros para missões dos jogadores
    char **missoesJogadores = (char **) calloc(jogadores, sizeof(char *));
    if (!missoesJogadores) {
        fprintf(stderr, "Falha ao alocar missões dos jogadores\n");
        free(mapa);
        return 1;
    }
    for (int i = 0; i < jogadores; ++i) missoesJogadores[i] = NULL;

    // Atribui missão a cada jogador (mostrar somente no início)
    for (int p = 0; p < jogadores; ++p) {
        atribuirMissao(&missoesJogadores[p], missoesDisponiveis, totalMissoes);
        printf("Jogador %d - ", p+1);
        exibirMissao(missoesJogadores[p]);
    }

    // Exibe mapa inicial
    exibirMapa(mapa, numTerritorios);

    // Loop simples de turnos (simulação reduzida)
    int turno = 0;
    int maxTurnos = 50; // evita loop infinito
    int vencedor = -1;

    while (turno < maxTurnos) {
        int jogadorAtual = (turno % jogadores);
        printf("\n--- Turno %d: Jogador %d ---\n", turno+1, jogadorAtual+1);

        // Simulação: o jogador tenta atacar um território inimigo aleatório (busca o primeiro diferente)
        int atacanteIdx = -1;
        int defensorIdx = -1;
        // encontra um território do jogadorAtual (cor atribuída artificialmente)
        // Para este exemplo, definimos que jogador 1 -> VERMELHA, jogador 2 -> AZUL
        const char *corJogador = (jogadorAtual == 0) ? "VERMELHA" : "AZUL";
        for (int i = 0; i < numTerritorios; ++i) {
            if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas > 1) { atacanteIdx = i; break; }
        }
        // escolhe um defensor com cor diferente
        for (int j = 0; j < numTerritorios; ++j) {
            if (atacanteIdx != j && strcmp(mapa[j].cor, corJogador) != 0) { defensorIdx = j; break; }
        }

        if (atacanteIdx != -1 && defensorIdx != -1) {
            printf("%s (atacante) ataca %s (defensor)\n", mapa[atacanteIdx].nome, mapa[defensorIdx].nome);
            atacar(&mapa[atacanteIdx], &mapa[defensorIdx]);
        } else {
            printf("Sem ataques possíveis neste turno.\n");
        }

        // Verificar missão silenciosamente ao fim do turno para cada jogador
        for (int p = 0; p < jogadores; ++p) {
            if (verificarMissao(missoesJogadores[p], mapa, numTerritorios)) {
                vencedor = p;
                break;
            }
        }
        if (vencedor != -1) break;

        // exibir mapa ao fim do turno
        exibirMapa(mapa, numTerritorios);

        turno++;
    }

    if (vencedor != -1) {
        printf("\n##### Jogador %d cumpriu a missão e venceu o jogo! #####\n", vencedor+1);
        printf("Missão cumprida: %s\n", missoesJogadores[vencedor]);
    } else {
        printf("\nNenhuma missão foi cumprida ao fim dos %d turnos.\n", maxTurnos);
    }

    // liberar memória
    liberarMemoria(mapa, numTerritorios, missoesJogadores, jogadores);

    return 0;
}
