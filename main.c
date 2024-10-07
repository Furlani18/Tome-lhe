#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_POKEMONS 10
#define NAME_LENGTH 20

typedef enum {
    FOGO,
    AGUA,
    PLANTA,
    TIPO_MAX
} TipoPokemon;

typedef struct {
    char name[NAME_LENGTH];
    int level;
    int health;
    int attack;
    TipoPokemon type; // Tipo do Pokémon
} Pokemon;

typedef struct {
    Pokemon pokemons[MAX_POKEMONS];
    int count;
} Player;

TipoPokemon getTipo() {
    int tipo;
    printf("Escolha o tipo do Pokemon (0 - Fogo, 1 - Agua, 2 - Planta, 3 - Eletrico, 4 - Grama, 5 - Fantasma, 6 - Pedra, 7 - Luta, 8 - Psíquico, 9 - Fada, 10 - Voador, 11 - Inseto): ");
    scanf("%d", &tipo);
    return (TipoPokemon)tipo;
}

void addPokemon(Player* player) {
    if (player->count < MAX_POKEMONS) {
        Pokemon newPokemon;
        printf("Digite o nome do Pokemon: ");
        scanf("%s", newPokemon.name);
        printf("Digite o nivel do Pokemon: ");
        scanf("%d", &newPokemon.level);
        newPokemon.health = newPokemon.level * 10;
        newPokemon.attack = newPokemon.level * 2;
        newPokemon.type = getTipo(); // Obtém o tipo do Pokémon
        player->pokemons[player->count] = newPokemon;
        player->count++;
        printf("Pokemon adicionado!\n");
    } else {
        printf("Limite de Pokemon alcançado!\n");
    }
}

void displayPokemons(Player* player) {
    printf("Pokemons do jogador:\n");
    for (int i = 0; i < player->count; i++) {
        printf("Nome: %s, Nivel: %d, Saude: %d, Ataque: %d, Tipo: %d\n",
               player->pokemons[i].name,
               player->pokemons[i].level,
               player->pokemons[i].health,
               player->pokemons[i].attack,
               player->pokemons[i].type);
    }
}

void capturePokemon(Player* player) {
    if (player->count >= MAX_POKEMONS) {
        printf("Voce ja tem o numero maximo de Pokemon!\n");
        return;
    }

    // Pokémon selvagem gerado aleatoriamente
    Pokemon wildPokemon;
    sprintf(wildPokemon.name, "Pokemon Selvagem %d", rand() % 100); // Nome aleatório
    wildPokemon.level = rand() % 10 + 1; // Nível aleatório de 1 a 10
    wildPokemon.health = wildPokemon.level * 10;
    wildPokemon.attack = wildPokemon.level * 2;

    printf("Um %s selvagem apareceu!\n", wildPokemon.name);
    printf("Nivel: %d, Saude: %d, Ataque: %d\n", wildPokemon.level, wildPokemon.health, wildPokemon.attack);

    // Simulação de captura (50% de chance)
    if (rand() % 2 == 0) {
        player->pokemons[player->count] = wildPokemon;
        player->count++;
        printf("Você capturou %s!\n", wildPokemon.name);
    } else {
        printf("A captura falhou!\n");
    }
}

void trainPokemon(Player* player) {
    if (player->count == 0) {
        printf("Voce nao tem Pokemon para treinar!\n");
        return;
    }

    int index;
    printf("Escolha um Pokemon para treinar (0 a %d): ", player->count - 1);
    scanf("%d", &index);

    if (index < 0 || index >= player->count) {
        printf("Escolha invalida!\n");
        return;
    }

    Pokemon* pokemon = &player->pokemons[index];
    pokemon->level++;
    pokemon->health = pokemon->level * 10; // Atualiza saúde com base no nível
    pokemon->attack = pokemon->level * 2; // Atualiza ataque com base no nível
    printf("%s treinou! Novo nivel: %d, Saude: %d, Ataque: %d\n", pokemon->name, pokemon->level, pokemon->health, pokemon->attack);
}

void battle(Player* player) {
    if (player->count < 2) {
        printf("Voce precisa de pelo menos 2 Pokemon para batalhar!\n");
        return;
    }

    int p1, p2;
    printf("Escolha o Pokemon 1 (0 a %d): ", player->count - 1);
    scanf("%d", &p1);
    printf("Escolha o Pokemon 2 (0 a %d): ", player->count - 1);
    scanf("%d", &p2);

    if (p1 < 0 || p1 >= player->count || p2 < 0 || p2 >= player->count) {
        printf("Escolha invalida!\n");
        return;
    }

    Pokemon* pokemon1 = &player->pokemons[p1];
    Pokemon* pokemon2 = &player->pokemons[p2];

    // Considera vantagem de tipo
    float multiplier = 1.0;
    if ((pokemon1->type == FOGO && pokemon2->type == PLANTA) ||
        (pokemon1->type == AGUA && pokemon2->type == FOGO) ||
        (pokemon1->type == PLANTA && pokemon2->type == AGUA)) {
        multiplier = 1.5; // Pokémon 1 tem vantagem
    } else if ((pokemon2->type == FOGO && pokemon1->type == PLANTA) ||
               (pokemon2->type == AGUA && pokemon1->type == FOGO) ||
               (pokemon2->type == PLANTA && pokemon1->type == AGUA)) {
        multiplier = 0.5; // Pokémon 2 tem vantagem
    }

    printf("Batalha entre %s e %s!\n", pokemon1->name, pokemon2->name);

    // Simulação de batalha
    while (pokemon1->health > 0 && pokemon2->health > 0) {
        pokemon2->health -= pokemon1->attack * multiplier;
        printf("%s atacou %s! %s saude: %d\n", pokemon1->name, pokemon2->name, pokemon2->name, pokemon2->health);
        if (pokemon2->health <= 0) {
            printf("%s venceu!\n", pokemon1->name);
            return;
        }

        pokemon1->health -= pokemon2->attack;
        printf("%s atacou %s! %s saude: %d\n", pokemon2->name, pokemon1->name, pokemon1->name, pokemon1->health);
        if (pokemon1->health <= 0) {
            printf("%s venceu!\n", pokemon2->name);
            return;
        }
    }
}

void saveGame(Player* player) {
    FILE* file = fopen("savegame.bin", "wb"); // Abre o arquivo binário
    if (!file) {
        printf("Erro ao salvar o jogo!\n");
        return;
    }
    fwrite(player, sizeof(Player), 1, file); // Escreve os dados do player no arquivo binário
    fclose(file);
    printf("Jogo salvo com sucesso!\n");
}

void loadGame(Player* player) {
    FILE* file = fopen("savegame.bin", "rb"); // Abre o arquivo binário para leitura
    if (!file) {
        printf("Nenhum arquivo salvo encontrado!\n");
        return;
    }
    fread(player, sizeof(Player), 1, file); // Carrega os dados do player do arquivo binário
    fclose(file);
    printf("Jogo carregado com sucesso!\n");
}

int main() {
    Player player;
    player.count = 0;

    int choice;
    do {
        printf("1. Adicionar Pokemon\n");
        printf("2. Exibir Pokemons\n");
        printf("3. Batalhar\n");
        printf("4. Capturar Pokemon\n");
        printf("5. Treinar Pokemon\n");
        printf("6. Salvar Jogo\n");
        printf("7. Carregar Jogo\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addPokemon(&player);
                break;
            case 2:
                displayPokemons(&player);
                break;
            case 3:
                battle(&player);
                break;
            case 4:
                capturePokemon(&player);
                break;
            case 5:
                trainPokemon(&player);
                break;
            case 6:
                saveGame(&player);
                break;
            case 7:
                loadGame(&player);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção invalida!\n");
                break;
        }
    } while (choice != 0);

    return 0;
}
