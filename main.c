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
    int nivel;
} Pokemon;

// Função para exibir o status de um Pokemon
void exibirStatus(Pokemon p) {
    printf("\n=== Status do Pokemon ===\n");
    printf("Nome: %s\n", p.nome);
    printf("Tipo: %s\n", p.tipo);
    printf("Nivel: %d\n", p.nivel);
    printf("HP: %d\n", p.hp);
    printf("Ataque: %d\n", p.ataque);
    printf("Defesa: %d\n", p.defesa);
    printf("=========================\n");
}

// Função para calcular o dano causado
int calcularDano(int ataque, int defesa) {
    int dano = ataque - defesa;
    if (dano < 0) {
        dano = 0;
    }
    return dano;
}

// Função para a batalha
void batalha(Pokemon *player, Pokemon *inimigo) {
    int turno = 1;
    while (player->hp > 0 && inimigo->hp > 0) {
        printf("\n=== Turno %d ===\n", turno);
        exibirStatus(*player);
        exibirStatus(*inimigo);

        // Escolha do jogador
        int escolha;
        printf("Escolha sua ação:\n");
        printf("1. Atacar\n");
        printf("2. Usar ataque especial\n");
        printf("3. Defender\n");
        printf("4. Usar item\n");
        printf("Escolha: ");
        scanf("%d", &escolha);

        // Ações do jogador
        int dano;
        switch (escolha) {
            case 1: // Jogador ataca
                dano = calcularDano(player->ataque, inimigo->defesa);
                inimigo->hp -= dano;
                printf("Você atacou! Causou %d de dano ao %s.\n", dano, inimigo->nome);
                break;
            case 2: // Jogador usa ataque especial
                dano = calcularDano(player->ataque * 1.5, inimigo->defesa);
                inimigo->hp -= dano;
                printf("Você usou um ataque especial! Causou %d de dano ao %s.\n", dano, inimigo->nome);
                break;
            case 3: // Jogador defende
                printf("Você escolheu se defender!\n");
                player->defesa += 3;  // Aumenta temporariamente a defesa
                break;
            case 4: // Jogador usa item
                printf("Você usou um item!\n");
                player->hp += 20; // Recupera HP
                printf("Recuperou 20 HP!\n");
                break;
            default:
                printf("Opção inválida!\n");
                continue;  // Retorna ao início do loop
        }

        // Movimento do inimigo (aleatório)
        int movimentoInimigo = rand() % 2 + 1; // Inimigo só ataca ou defende
        if (movimentoInimigo == 1) {  // Inimigo ataca
            dano = calcularDano(inimigo->ataque, player->defesa);
            player->hp -= dano;
            printf("%s atacou! Causou %d de dano a você.\n", inimigo->nome, dano);
        } else {  // Inimigo defende
            printf("%s se defendeu!\n", inimigo->nome);
            inimigo->defesa += 3; // Aumenta a defesa do inimigo
        }

        // Nível e evolução
        if (inimigo->hp <= 0) {
            printf("\nVocê venceu a batalha!\n");
            player->nivel++; // Aumenta o nível do jogador após vencer
            printf("Seu Pokémon subiu para o nível %d!\n", player->nivel);
            // Evolução: exemplo básico, você pode expandir isso
            if (player->nivel == 5) {
                printf("%s evoluiu!\n", player->nome);
                player->ataque += 5; // Exemplo de evolução
                player->defesa += 3; // Exemplo de evolução
                player->hp += 10; // Exemplo de evolução
            }
        }

        turno++;
    }

    // Resultado da batalha
    if (player->hp <= 0) {
        printf("\nVocê perdeu a batalha!\n");
    }
}

// Função para mostrar informações sobre os Pokemons disponíveis
void mostrarPokemons(Pokemon listaPokemons[], int tamanho) {
    printf("\n=== Lista de Pokemons ===\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%d. %s | Nivel: %d | HP: %d | Ataque: %d | Defesa: %d | Tipo: %s\n", 
               i + 1, 
               listaPokemons[i].nome, 
               listaPokemons[i].nivel, 
               listaPokemons[i].hp, 
               listaPokemons[i].ataque, 
               listaPokemons[i].defesa, 
               listaPokemons[i].tipo);
    }
    printf("=========================\n");
}

// Menu principal do jogo
void menu(Pokemon listaPokemons[], int tamanho) {
    int opcao;
    do {
        printf("\n=== Menu Principal ===\n");
        printf("| 1. Iniciar Batalha    |\n");
        printf("| 2. Ver Pokemons       |\n");
        printf("| 3. Sair               |\n");
        printf("=========================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                int escolhaPlayer;

                // Jogador escolhe seu Pokemon
                printf("Escolha seu Pokemon:\n");
                mostrarPokemons(listaPokemons, tamanho);
                scanf("%d", &escolhaPlayer);
                Pokemon player = listaPokemons[escolhaPlayer - 1];

                // Inimigo escolhe aleatoriamente
                int escolhaInimigo = rand() % tamanho;
                Pokemon inimigo = listaPokemons[escolhaInimigo];

                printf("\nUma batalha entre %s e %s vai começar!\n", player.nome, inimigo.nome);
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
    srand(time(NULL));  // Inicializa o gerador de números aleatórios

    // Lista de Pokemons para o jogador e inimigo escolher
    Pokemon listaPokemons[25] = {
        {"Pikachu", 35, 10, 5, "eletrico", 1},
        {"Charmander", 30, 8, 6, "fogo", 1},
        {"Squirtle", 40, 7, 8, "agua", 1},
        {"Bulbasaur", 45, 9, 7, "grama", 1},
        {"Jigglypuff", 115, 5, 5, "normal", 1},
        {"Gengar", 60, 14, 5, "fantasma", 1},
        {"Onix", 35, 10, 20, "pedra", 1},
        {"Gyarados", 95, 20, 10, "agua", 1},
        {"Charizard", 78, 14, 9, "fogo", 1},
        {"Snorlax", 160, 12, 8, "normal", 1},
        {"Mewtwo", 106, 14, 9, "psíquico", 1},
        {"Eevee", 55, 10, 6, "normal", 1},
        {"Machamp", 90, 16, 8, "luta", 1},
        {"Alakazam", 55, 14, 7, "psíquico", 1},
        {"Lapras", 130, 12, 10, "agua", 1},
        {"Ditto", 48, 8, 5, "normal", 1},
        {"Scyther", 70, 12, 8, "inseto", 1},
        {"Kabutops", 60, 15, 7, "pedra", 1},
        {"Tyranitar", 100, 18, 10, "pedra", 1},
        {"Zapdos", 90, 15, 10, "eletrico", 1},
        {"Lucario", 70, 14, 8, "luta", 1},
        {"Greninja", 72, 14, 8, "agua", 1},
        {"Mimikyu", 55, 12, 9, "fantasma", 1},
        {"Incineroar", 95, 16, 9, "fogo", 1},
        {"Togekiss", 85, 10, 8, "fada", 1},
        {"Cinderace", 80, 17, 7, "fogo", 1},
        {"Rillaboom", 90, 15, 10, "grama", 1}
    };

    menu(listaPokemons, 25);
    return 0;
}
