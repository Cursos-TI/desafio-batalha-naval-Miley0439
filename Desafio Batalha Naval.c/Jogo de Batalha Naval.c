#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_TABULEIRO 10
#define TAMANHO_NAVIO 3
#define TAMANHO_HABILIDADE 5

#define AGUA 0
#define NAVIO 3
#define HABILIDADE 5

typedef int Tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
typedef int Habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];

// ------------------- INICIALIZAÇÃO -------------------

void inicializarTabuleiro(Tabuleiro tab) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tab[i][j] = AGUA;
        }
    }
}

// ------------------- EXIBIÇÃO NUMÉRICA -------------------

void exibirTabuleiro(Tabuleiro tab) {
    printf("\n   ");
    for (int col = 0; col < TAMANHO_TABULEIRO; col++) {
        printf("%2d ", col);
    }
    printf("\n");

    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%2d ", i);
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            printf("%2d ", tab[i][j]);
        }
        printf("\n");
    }
}

// ------------------- POSICIONAMENTO DE NAVIOS -------------------

int posicaoLivre(Tabuleiro tab, int linha, int coluna) {
    return linha >= 0 && linha < TAMANHO_TABULEIRO &&
           coluna >= 0 && coluna < TAMANHO_TABULEIRO &&
           tab[linha][coluna] == AGUA;
}

int posicionarNavio(Tabuleiro tab, int linha, int coluna, int dLinha, int dColuna) {
    for (int i = 0; i < TAMANHO_NAVIO; i++) {
        int l = linha + i * dLinha;
        int c = coluna + i * dColuna;
        if (!posicaoLivre(tab, l, c)) return 0;
    }
    for (int i = 0; i < TAMANHO_NAVIO; i++) {
        tab[linha + i * dLinha][coluna + i * dColuna] = NAVIO;
    }
    return 1;
}

int posicionarHorizontal(Tabuleiro tab, int l, int c) {
    return posicionarNavio(tab, l, c, 0, 1);
}
int posicionarVertical(Tabuleiro tab, int l, int c) {
    return posicionarNavio(tab, l, c, 1, 0);
}
int posicionarDiagonalPrincipal(Tabuleiro tab, int l, int c) {
    return posicionarNavio(tab, l, c, 1, 1);
}
int posicionarDiagonalSecundaria(Tabuleiro tab, int l, int c) {
    return posicionarNavio(tab, l, c, 1, -1);
}

// ------------------- MATRIZES DE HABILIDADE -------------------

void construirCone(Habilidade mat) {
    int centro = TAMANHO_HABILIDADE / 2;
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            mat[i][j] = (i >= abs(j - centro)) ? 1 : 0;
        }
    }
}

void construirCruz(Habilidade mat) {
    int centro = TAMANHO_HABILIDADE / 2;
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            mat[i][j] = (i == centro || j == centro) ? 1 : 0;
        }
    }
}

void construirOctaedro(Habilidade mat) {
    int centro = TAMANHO_HABILIDADE / 2;
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            mat[i][j] = (abs(i - centro) + abs(j - centro) <= centro) ? 1 : 0;
        }
    }
}

// ------------------- APLICAÇÃO DAS HABILIDADES -------------------

void aplicarHabilidade(Tabuleiro tab, Habilidade hab, int linhaCentro, int colunaCentro) {
    int offset = TAMANHO_HABILIDADE / 2;

    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            if (hab[i][j] == 1) {
                int linhaTab = linhaCentro + i - offset;
                int colunaTab = colunaCentro + j - offset;

                if (linhaTab >= 0 && linhaTab < TAMANHO_TABULEIRO &&
                    colunaTab >= 0 && colunaTab < TAMANHO_TABULEIRO &&
                    tab[linhaTab][colunaTab] == AGUA) {
                    tab[linhaTab][colunaTab] = HABILIDADE;
                }
            }
        }
    }
}

// ------------------- FUNÇÃO PRINCIPAL -------------------

int main() {
    Tabuleiro tab;
    inicializarTabuleiro(tab);

    // Posicionamento de navios
    posicionarHorizontal(tab, 0, 0);
    posicionarVertical(tab, 4, 5);
    posicionarDiagonalPrincipal(tab, 6, 0);
    posicionarDiagonalSecundaria(tab, 0, 9);

    // Matrizes de habilidade
    Habilidade cone, cruz, octaedro;
    construirCone(cone);
    construirCruz(cruz);
    construirOctaedro(octaedro);

    // Aplicação das habilidades
    aplicarHabilidade(tab, cone, 3, 3);       // Cone em (3,3)
    aplicarHabilidade(tab, cruz, 5, 8);       // Cruz em (5,8)
    aplicarHabilidade(tab, octaedro, 8, 5);   // Octaedro em (8,5)

    // Exibir resultado final
    printf("=== TABULEIRO FINAL ===\n");
    printf("0 = Água | 3 = Navio | 5 = Habilidade\n");
    exibirTabuleiro(tab);

    return 0;
}

