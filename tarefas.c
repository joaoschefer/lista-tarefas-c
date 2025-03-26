#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Tarefa {
    int id;
    char descricao[100];
    struct Tarefa *prox;
} Tarefa;

Tarefa *inicio = NULL;
int proximoId = 1;

// salvar no arquivo
void salvarTarefas() {
    FILE *arquivo = fopen("tarefas.txt", "w");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    Tarefa *atual = inicio;
    while (atual != NULL) {
        fprintf(arquivo, "%d|%s\n", atual->id, atual->descricao);
        atual = atual->prox;
    }

    fclose(arquivo);
}

// carregar tarefas do arquivo
void carregarTarefas() {
    FILE *arquivo = fopen("tarefas.txt", "r");
    if (!arquivo) return;

    char linha[150];
    while (fgets(linha, sizeof(linha), arquivo)) {
        Tarefa *nova = (Tarefa *)malloc(sizeof(Tarefa));
        if (!nova) continue;

        sscanf(linha, "%d|%[^\n]", &nova->id, nova->descricao);
        nova->prox = inicio;
        inicio = nova;

        if (nova->id >= proximoId) {
            proximoId = nova->id + 1;
        }
    }

    fclose(arquivo);
}

// adicionar tarefa
void adicionarTarefa() {
    Tarefa *nova = (Tarefa *)malloc(sizeof(Tarefa));
    if (!nova) {
        printf("Erro ao alocar memória.\n");
        return;
    }

    nova->id = proximoId++;
    printf("Digite a descrição da tarefa: ");
    fflush(stdin);
    fgets(nova->descricao, 100, stdin);
    nova->descricao[strcspn(nova->descricao, "\n")] = '\0';

    nova->prox = inicio;
    inicio = nova;

    printf("Tarefa adicionada com sucesso! (ID: %d)\n", nova->id);
    salvarTarefas();
}

// função para listar as tarefas
void listarTarefas() {
    if (inicio == NULL) {
        printf("Nenhuma tarefa cadastrada.\n");
        return;
    }

    Tarefa *atual = inicio;
    printf("\n=== Lista de Tarefas ===\n");
    while (atual != NULL) {
        printf("ID: %id | Descrição: %s\n", atual->id, atual->descricao);
        atual = atual->prox;
    }
}

// remover tarefa
void removerTarefa() {
    if (inicio == NULL) {
        printf("Nenhuma tarefa para remover.\n");
        return;
    }

    int id;
    printf("Digite o ID da tarefa a remover: ");
    scanf("%d", &id);

    Tarefa *atual = inicio;
    Tarefa *anterior = NULL;

    while (atual != NULL && atual->id != id) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        inicio = atual->prox;
    } else {
        anterior-> prox = atual->prox;
    }

    free(atual);
    printf("Tarefa removida com sucesso");
    salvarTarefas();
}

// liberar memória ao sair
void liberarMemoria() {
    Tarefa *atual = inicio;
    while (atual != NULL) {
        Tarefa *prox = atual->prox;
        free(atual);
        atual = prox;
    }
}

int main() {
    int opcao;

    carregarTarefas();

    do {
        printf("\n=== GERENCIADOR DE TAREFAS ===\n");
        printf("1 - Adicionar tarefa\n");
        printf("2 - Listar tarefas\n");
        printf("3 - Remover tarefa\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                adicionarTarefa();
                break;
            case 2:
                listarTarefas();
                break;
            case 3:
                removerTarefa();
                break;
            case 0:
                liberarMemoria();
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    return 0;
}
