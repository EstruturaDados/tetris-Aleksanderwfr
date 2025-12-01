#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ==========================================
// DEFINIÇÕES E ESTRUTURAS (Nível Novato)
// ==========================================

#define CAP_FILA 5
#define CAP_PILHA 3

typedef struct {
    char tipo; // 'I', 'O', 'T', 'L', etc.
    int id;    // Identificador único
} Peca;

typedef struct {
    Peca itens[CAP_FILA];
    int inicio;
    int tamanho; // Quantidade atual de itens
} Fila;

typedef struct {
    Peca itens[CAP_PILHA];
    int topo; // Índice do topo (-1 se vazia)
} Pilha;

// Variável global para gerar IDs sequenciais
int contadorGlobalID = 1;

// ==========================================
// FUNÇÕES AUXILIARES
// ==========================================

// Gera uma peça aleatória (Novato)
Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'Z', 'S'};
    p.tipo = tipos[rand() % 7];
    p.id = contadorGlobalID++;
    return p;
}

// Inicialização (Novato & Aventureiro)
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->tamanho = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1; // Indica pilha vazia
}

// ==========================================
// LÓGICA DA FILA CIRCULAR (Nível Novato)
// ==========================================

int filaCheia(Fila *f) {
    return f->tamanho == CAP_FILA;
}

int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

// Insere no final da fila (circular)
void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return; // Segurança básica

    int indiceFinal = (f->inicio + f->tamanho) % CAP_FILA;
    f->itens[indiceFinal] = p;
    f->tamanho++;
}

// Remove do início da fila e retorna a peça
Peca dequeue(Fila *f) {
    Peca p = f->itens[f->inicio];
    
    // Avança o início circularmente
    f->inicio = (f->inicio + 1) % CAP_FILA;
    f->tamanho--;
    
    return p;
}

// ==========================================
// LÓGICA DA PILHA (Nível Aventureiro)
// ==========================================

int pilhaCheia(Pilha *p) {
    return p->topo == CAP_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

void push(Pilha *p, Peca peca) {
    if (pilhaCheia(p)) {
        printf("\n[!] Erro: Reserva cheia!\n");
        return;
    }
    p->topo++;
    p->itens[p->topo] = peca;
}

Peca pop(Pilha *p) {
    Peca item = p->itens[p->topo];
    p->topo--;
    return item;
}

// ==========================================
// LÓGICA MESTRE (Integração e Trocas)
// ==========================================

// Opção 4: Troca o topo da pilha com a frente da fila
void trocarFrenteTopo(Fila *f, Pilha *p) {
    if (pilhaVazia(p)) {
        printf("\n[!] Acao invalida: A pilha esta vazia.\n");
        return;
    }
    
    // Acesso direto à memória para troca
    Peca temp = f->itens[f->inicio]; // Frente da fila
    f->itens[f->inicio] = p->itens[p->topo]; // Topo da pilha vai para fila
    p->itens[p->topo] = temp; // Frente da fila vai para pilha
    
    printf("\n[Status] Peca da frente trocada com o topo da reserva!\n");
}

// Opção 5: Troca as 3 peças da pilha com as 3 primeiras da fila
void trocarTudo(Fila *f, Pilha *p) {
    // Regra: Pilha deve estar cheia (3 itens) e Fila ter pelo menos 3 (sempre tem 5 neste jogo)
    if (p->topo < 2) { 
        printf("\n[!] Acao invalida: A reserva precisa ter 3 pecas para essa troca.\n");
        return;
    }

    printf("\n[Status] Trocando as 3 pecas da reserva com as 3 da fila...\n");

    // Loop para trocar 3 elementos
    // A pilha é linear (0, 1, 2), a fila é circular.
    for (int i = 0; i < 3; i++) {
        // Cálculo do índice circular na fila para o elemento i
        int idxFila = (f->inicio + i) % CAP_FILA;
        
        // Troca (swap)
        Peca temp = f->itens[idxFila];
        
        // Atenção: O topo da pilha é o índice 2. Queremos trocar o 1º da fila com o topo?
        // Ou mapear direto 0 com 0? O enunciado não especifica, vamos mapear direto i com i
        // para inverter o conjunto.
        f->itens[idxFila] = p->itens[i];
        p->itens[i] = temp;
    }
}

// ==========================================
// INTERFACE E VISUALIZAÇÃO
// ==========================================

void mostrarEstado(Fila *f, Pilha *p) {
    printf("\n============================================\n");
    printf("           TETRIS STACK - ByteBros          \n");
    printf("============================================\n");
    
    // Mostrar Fila
    printf("FILA (Proximas): ");
    if (filaVazia(f)) {
        printf("[ Vazia ]");
    } else {
        // Percorre a fila circularmente apenas para visualização
        for (int i = 0; i < f->tamanho; i++) {
            int idx = (f->inicio + i) % CAP_FILA;
            if (i == 0) printf(" -> "); // Indica a próxima a sair
            printf("[%c-%02d] ", f->itens[idx].tipo, f->itens[idx].id);
        }
    }
    printf("\n");

    // Mostrar Pilha
    printf("RESERVA (Pilha): ");
    if (pilhaVazia(p)) {
        printf("[ Vazia ]");
    } else {
        for (int i = 0; i <= p->topo; i++) {
            printf("[%c-%02d] ", p->itens[i].tipo, p->itens[i].id);
        }
        printf(" <- Topo");
    }
    printf("\n============================================\n");
}

// ==========================================
// FUNÇÃO PRINCIPAL (MAIN)
// ==========================================

int main() {
    srand(time(NULL)); // Semente para números aleatórios

    Fila fila;
    Pilha pilha;
    
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Setup Inicial (Novato): Preencher a fila com 5 peças
    for (int i = 0; i < CAP_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;
    do {
        mostrarEstado(&fila, &pilha);
        
        printf("\n--- MENU DE ACOES ---\n");
        printf("1 - Jogar peca (Fila -> Jogo)\n");
        printf("2 - Reservar peca (Fila -> Pilha)\n");
        printf("3 - Usar peca reservada (Pilha -> Jogo)\n");
        printf("4 - [MESTRE] Trocar Frente da Fila com Topo da Pilha\n");
        printf("5 - [MESTRE] Inverter 3 primeiros da Fila com Pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        Peca jogada;

        switch (opcao) {
            case 1: // Jogar (Novato)
                jogada = dequeue(&fila);
                printf("\n>>> Voce jogou a peca [%c-%02d]!\n", jogada.tipo, jogada.id);
                // Regra: Sempre repor a peça na fila
                enqueue(&fila, gerarPeca());
                break;

            case 2: // Reservar (Aventureiro)
                if (pilhaCheia(&pilha)) {
                    printf("\n[!] A reserva esta cheia! Jogue a peca ou use a reserva.\n");
                } else {
                    jogada = dequeue(&fila); // Tira da fila
                    push(&pilha, jogada);    // Bota na pilha
                    printf("\n>>> Peca [%c-%02d] enviada para a reserva.\n", jogada.tipo, jogada.id);
                    // Repor a fila
                    enqueue(&fila, gerarPeca());
                }
                break;

            case 3: // Usar Reserva (Aventureiro)
                if (pilhaVazia(&pilha)) {
                    printf("\n[!] A reserva esta vazia!\n");
                } else {
                    jogada = pop(&pilha);
                    printf("\n>>> Voce usou a peca da reserva [%c-%02d]!\n", jogada.tipo, jogada.id);
                }
                break;

            case 4: // Troca Simples (Mestre)
                trocarFrenteTopo(&fila, &pilha);
                break;

            case 5: // Troca Tripla (Mestre)
                trocarTudo(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando Tetris Stack... Ate logo!\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}