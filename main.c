#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Definicao de um Pokemon
typedef struct {
    char nome[20];
    int hp;
    int ataque;
    int defesa;
    char tipo[10];
} Pokemon;

// Funcao para exibir o status de um Pokemon
void exibirStatus(Pokemon p) {
    printf("%s (Tipo: %s) | HP: %d | Ataque: %d | Defesa: %d\n", p.nome, p.tipo, p.hp, p.ataque, p.defesa);
}

// Funcao para calcular o dano causado com multiplicador de tipos
int calcularDano(int ataque, int defesa, char *tipoAtacante, char *tipoDefensor) {
    int dano = ataque - defesa;
    if (dano < 0) {
        dano = 0;
    }

    // Multiplicadores de dano com base nos tipos
    if (strcmp(tipoAtacante, "eletrico") == 0 && strcmp(tipoDefensor, "agua") == 0) {
        dano *= 2;  // Eletrico contra Agua
    } else if (strcmp(tipoAtacante, "fogo") == 0 && strcmp(tipoDefensor, "grama") == 0) {
        dano *= 2;  // Fogo contra Grama
    } else if (strcmp(tipoAtacante, "agua") == 0 && strcmp(tipoDefensor, "fogo") == 0) {
        dano *= 2;  // Agua contra Fogo
    } else if (strcmp(tipoAtacante, "grama") == 0 && strcmp(tipoDefensor, "agua") == 0) {
        dano *= 2;  // Grama contra Agua
    }

    return dano;
}

// Funcao para a batalha
void batalha(Pokemon *player, Pokemon *inimigo) {
    int turno = 1;
    while (player->hp > 0 && inimigo->hp > 0) {
        printf("\n=== Turno %d ===\n", turno);
        exibirStatus(*player);
        exibirStatus(*inimigo);

        // Escolha do jogador
        int escolha;
        printf("Escolha sua acao:\n1. Atacar\n2. Ataque Especial\n3. Ataque Critico\n4. Defender\n5. Defesa Especial\nEscolha: ");
        scanf("%d", &escolha);

        // Movimento do inimigo (aleatorio)
        int movimentoInimigo = rand() % 5 + 1;

        if (escolha == 1) {  // Jogador ataca
            int dano = calcularDano(player->ataque, inimigo->defesa, player->tipo, inimigo->tipo);
            inimigo->hp -= dano;
            printf("Voce atacou! Causou %d de dano ao %s.\n", dano, inimigo->nome);
        } else if (escolha == 2) {  // Jogador ataque especial
            int dano = calcularDano(player->ataque * 1.5, inimigo->defesa, player->tipo, inimigo->tipo);
            inimigo->hp -= dano;
            printf("Voce usou um ataque especial! Causou %d de dano ao %s.\n", dano, inimigo->nome);
        } else if (escolha == 3) {  // Jogador ataque critico
            int dano = calcularDano(player->ataque * 2, inimigo->defesa, player->tipo, inimigo->tipo);
            inimigo->hp -= dano;
            printf("Voce usou um ataque critico! Causou %d de dano ao %s.\n", dano, inimigo->nome);
        } else if (escolha == 4) {  // Jogador defende
            printf("Voce escolheu se defender!\n");
            player->defesa += 3;  // Aumenta temporariamente a defesa
        } else if (escolha == 5) {  // Jogador defesa especial
            printf("Voce usou uma defesa especial!\n");
            player->defesa += 5;  // Aumenta mais ainda a defesa
        }

        // Movimento do inimigo
        if (movimentoInimigo == 1) {  // Inimigo ataca
            int dano = calcularDano(inimigo->ataque, player->defesa, inimigo->tipo, player->tipo);
            player->hp -= dano;
            printf("%s atacou! Causou %d de dano a voce.\n", inimigo->nome, dano);
        } else if (movimentoInimigo == 2) {  // Inimigo ataque especial
            int dano = calcularDano(inimigo->ataque * 1.5, player->defesa, inimigo->tipo, player->tipo);
            player->hp -= dano;
            printf("%s usou um ataque especial! Causou %d de dano a voce.\n", inimigo->nome, dano);
        } else if (movimentoInimigo == 3) {  // Inimigo ataque critico
            int dano = calcularDano(inimigo->ataque * 2, player->defesa, inimigo->tipo, player->tipo);
            player->hp -= dano;
            printf("%s usou um ataque critico! Causou %d de dano a voce.\n", inimigo->nome, dano);
        } else if (movimentoInimigo == 4) {  // Inimigo defende
            printf("%s escolheu se defender!\n", inimigo->nome);
            inimigo->defesa += 3;
        } else if (movimentoInimigo == 5) {  // Inimigo defesa especial
            printf("%s usou uma defesa especial!\n", inimigo->nome);
            inimigo->defesa += 5;
        }

        turno++;
    }

    // Resultado da batalha
    if (player->hp <= 0) {
        printf("\nVoce perdeu a batalha!\n");
    } else {
        printf("\nVoce venceu a batalha!\n");
    }
}

// Funcao para mostrar informacoes sobre os Pokemons disponiveis
void mostrarPokemons(Pokemon listaPokemons[], int tamanho) {
    printf("\n=== Lista de Pokemons ===\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%d. %s | HP: %d | Ataque: %d | Defesa: %d | Tipo: %s\n", i + 1, listaPokemons[i].nome, listaPokemons[i].hp, listaPokemons[i].ataque, listaPokemons[i].defesa, listaPokemons[i].tipo);
    }
    printf("=========================\n");
}

// Menu principal do jogo
void menu(Pokemon listaPokemons[], int tamanho) {
    int opcao;
    do {
        printf("\n=== Menu Principal ===\n");
        printf("1. Iniciar Batalha\n");
        printf("2. Ver Pokemons\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                int escolhaPlayer, escolhaInimigo;

                // Jogador escolhe seu Pokemon
                printf("Escolha seu Pokemon:\n");
                mostrarPokemons(listaPokemons, tamanho);
                scanf("%d", &escolhaPlayer);
                Pokemon player = listaPokemons[escolhaPlayer - 1];

                // Inimigo escolhe aleatoriamente
                escolhaInimigo = rand() % tamanho;
                Pokemon inimigo = listaPokemons[escolhaInimigo];

                printf("\nUma batalha entre %s e %s vai comecar!\n", player.nome, inimigo.nome);
                batalha(&player, &inimigo);
                break;
            }
            case 2:
                mostrarPokemons(listaPokemons, tamanho);
                break;
            case 3:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
    } while (opcao != 3);
}

int main() {
    srand(time(NULL));  // Inicializa o gerador de numeros aleatorios

    // Lista de Pokemons para o jogador e inimigo escolher
   Pokemon listaPokemons[25] = {
    {"Pikachu", 35, 10, 5, "eletrico"},
    {"Charmander", 30, 8, 6, "fogo"},
    {"Squirtle", 40, 7, 8, "agua"},
    {"Bulbasaur", 38, 9, 7, "grama"},
    {"Jigglypuff", 115, 6, 5, "normal"},
    {"Meowth", 40, 10, 5, "normal"},
    {"Psyduck", 50, 7, 5, "agua"},
    {"Gengar", 60, 11, 6, "fantasma"},
    {"Onix", 55, 8, 12, "pedra"},
    {"Zapdos", 90, 12, 8, "eletrico"},
    {"Snorlax", 160, 12, 10, "normal"},
    {"Machamp", 90, 13, 8, "luta"},
    {"Eevee", 55, 7, 6, "normal"},
    {"Flareon", 65, 11, 7, "fogo"},
    {"Vaporeon", 65, 8, 8, "agua"},
    {"Arcanine", 90, 12, 9, "fogo"},
    {"Lapras", 130, 9, 10, "agua"},
    {"Dragonite", 91, 14, 10, "dragao"},
    {"Alakazam", 55, 13, 5, "psiquico"},
    {"Machoke", 80, 12, 7, "luta"},
    {"Nidoking", 81, 12, 7, "terra"},
    {"Gyarados", 95, 14, 10, "agua"},
    {"Lucario", 70, 12, 8, "luta"},
    {"Mewtwo", 106, 14, 9, "psiquico"},
    {"Tyranitar", 100, 13, 10, "pedra"},
    {"Crobat", 85, 10, 7, "veneno"}
};

    // Chama o menu principal
    menu(listaPokemons, 25);

    return 0;
}
