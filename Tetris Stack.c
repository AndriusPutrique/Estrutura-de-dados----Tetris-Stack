#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ---------------------------
// STRUCT DAS PEÇAS
// ---------------------------
typedef struct {
    char nome;   // Tipo da peça (I, O, T, L)
    int id;      // Identificador único
} Peca;

// ---------------------------
// CONSTANTES DO SISTEMA
// ---------------------------
#define TAM_FILA 5
#define TAM_PILHA 3

// ---------------------------
// VARIÁVEIS GLOBAIS
// ---------------------------
Peca fila[TAM_FILA];       // Fila circular
Peca pilha[TAM_PILHA];     // Pilha de reserva
int inicio = 0, fim = 0;   // Controle da fila circular
int qtdFila = 0;           // Contador da fila
int topo = -1;             // Topo da pilha
int geradorID = 0;         // ID global de peças

// ---------------------------
// FUNÇÃO DE GERAR NOVA PEÇA
// ---------------------------
Peca gerarPeca() {
    char tipos[4] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = geradorID++;
    return nova;
}

// ---------------------------
// FUNÇÕES DA FILA
// ---------------------------
int filaCheia() { return qtdFila == TAM_FILA; }
int filaVazia() { return qtdFila == 0; }

// Enqueue automático
void enfileirar(Peca p) {
    if (!filaCheia()) {
        fila[fim] = p;
        fim = (fim + 1) % TAM_FILA;
        qtdFila++;
    }
}

// Remove da fila
Peca desenfileirar() {
    Peca removida = fila[inicio];
    inicio = (inicio + 1) % TAM_FILA;
    qtdFila--;
    return removida;
}

// ---------------------------
// FUNÇÕES DA PILHA
// ---------------------------
int pilhaCheia() { return topo == TAM_PILHA - 1; }
int pilhaVazia() { return topo == -1; }

void push(Peca p) {
    if (!pilhaCheia()) {
        pilha[++topo] = p;
    }
}

Peca pop() {
    return pilha[topo--];
}

// ---------------------------
// EXIBIR ESTRUTURAS
// ---------------------------
void mostrarFila() {
    printf("Fila de peças\t");
    int pos = inicio;
    for (int i = 0; i < qtdFila; i++) {
        printf("[%c %d] ", fila[pos].nome, fila[pos].id);
        pos = (pos + 1) % TAM_FILA;
    }
    printf("\n");
}

void mostrarPilha() {
    printf("Pilha de reserva\t(Topo -> Base): ");
    for (int i = topo; i >= 0; i--) {
        printf("[%c %d] ", pilha[i].nome, pilha[i].id);
    }
    printf("\n");
}

void mostrarEstado() {
    printf("\n========== ESTADO ATUAL ==========\n");
    mostrarFila();
    mostrarPilha();
    printf("==================================\n");
}

// ---------------------------
// AÇÕES DO SISTEMA
// ---------------------------

// 1. Jogar peça (remove da fila)
void jogarPeca() {
    if (filaVazia()) {
        printf("A fila está vazia!\n");
        return;
    }
    Peca jogada = desenfileirar();
    printf("\nPeça jogada: [%c %d]\n", jogada.nome, jogada.id);

    // Sempre gerar nova peça
    enfileirar(gerarPeca());
}

// 2. Reservar peça (fila → pilha)
void reservarPeca() {
    if (filaVazia()) {
        printf("Fila vazia!\n");
        return;
    }
    if (pilhaCheia()) {
        printf("Pilha cheia! Não é possível reservar.\n");
        return;
    }

    Peca p = desenfileirar();
    push(p);
    printf("\nPeça reservada: [%c %d]\n", p.nome, p.id);

    enfileirar(gerarPeca());
}

// 3. Usar peça reservada (pop)
void usarReservada() {
    if (pilhaVazia()) {
        printf("A pilha está vazia!\n");
        return;
    }

    Peca usada = pop();
    printf("\nPeça usada da reserva: [%c %d]\n", usada.nome, usada.id);
}

// 4. Trocar frente da fila ↔ topo da pilha
void trocarSimples() {
    if (filaVazia()) {
        printf("Fila vazia! Impossível trocar.\n");
        return;
    }
    if (pilhaVazia()) {
        printf("Pilha vazia! Impossível trocar.\n");
        return;
    }

    int frente = inicio;
    Peca temp = fila[frente];
    fila[frente] = pilha[topo];
    pilha[topo] = temp;

    printf("\nTroca realizada entre frente da fila e topo da pilha.\n");
}

// 5. Troca múltipla (3 da fila ↔ 3 da pilha)
void trocarMultipla() {
    if (qtdFila < 3 || topo + 1 < 3) {
        printf("Não há peças suficientes para a troca múltipla!\n");
        return;
    }

    printf("\nRealizando troca múltipla das 3 primeiras peças...\n");

    for (int i = 0; i < 3; i++) {
        int pos = (inicio + i) % TAM_FILA;
        Peca temp = fila[pos];
        fila[pos] = pilha[topo - i];
        pilha[topo - i] = temp;
    }

    printf("Troca múltipla concluída!\n");
}

// ---------------------------
// MAIN
// ---------------------------
int main() {
    srand(time(NULL));

    // Preencher fila inicialmente
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(gerarPeca());
    }

    int opc;

    do {
        mostrarEstado();

        printf("\nOPÇÕES:\n");
        printf("1 - Jogar peça da fila\n");
        printf("2 - Reservar peça (fila → pilha)\n");
        printf("3 - Usar peça reservada (pilha → fora)\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Troca múltipla (3 da fila ↔ 3 da pilha)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);

        switch (opc) {
            case 1: jogarPeca(); break;
            case 2: reservarPeca(); break;
            case 3: usarReservada(); break;
            case 4: trocarSimples(); break;
            case 5: trocarMultipla(); break;
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opção inválida!\n");
        }

    } while (opc != 0);

    return 0;
}
