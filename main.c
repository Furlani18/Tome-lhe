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

#define RESET_COLOR "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

TipoPokemon getTipo() {
    int tipo;
    printf("Escolha o tipo do Pokemon (0 - Fogo, 1 - Água, 2 - Planta): ");
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
        printf(GREEN "Pokemon adicionado!\n" RESET_COLOR);
    } else {
        printf(RED "Limite de Pokemon alcançado!\n" RESET_COLOR);
    }
}

void displayPokemons(Player* player) {
    printf(CYAN "Pokemons do jogador:\n" RESET_COLOR);
    for (int i = 0; i < player->count; i++) {
        printf("Nome: %s, Nivel: %d, Saude: %d, Ataque: %d, Tipo: %d\n",
               player->pokemons[i].name,
               player->pokemons[i].level,
               player->pokemons[i].health,
               player->pokemons[i].attack,
               player->pokemons[i].type);
    }
}

void logBattle(const char* winner, const char* loser) {
    FILE* logFile = fopen("battle_log.txt", "a"); // Abre o arquivo para adicionar
    if (!logFile) {
        printf(RED "Erro ao abrir o arquivo de registro de batalhas!\n" RESET_COLOR);
        return;
    }
    fprintf(logFile, "Batalha: %s venceu %s\n", winner, loser); // Registra o vencedor e o perdedor
    fclose(logFile);
}

void capturePokemon(Player* player) {
    if (player->count >= MAX_POKEMONS) {
        printf(RED "Voce ja tem o numero maximo de Pokemon!\n" RESET_COLOR);
        return;
    }

    // Pokémon selvagem gerado aleatoriamente
    Pokemon wildPokemon;
    sprintf(wildPokemon.name, "Pokemon Selvagem %d", rand() % 100); // Nome aleatório
    wildPokemon.level = rand() % 10 + 1; // Nível aleatório de 1 a 10
    wildPokemon.health = wildPokemon.level * 10;
    wildPokemon.attack = wildPokemon.level * 2;

    printf("Um %s selvagem apareceu!\n", wildPokemon.name);
    printf("Nível: %d, Saude: %d, Ataque: %d\n", wildPokemon.level, wildPokemon.health, wildPokemon.attack);

    // Simulação de captura (50% de chance)
    if (rand() % 2 == 0) {
        player->pokemons[player->count] = wildPokemon;
        player->count++;
        printf(GREEN "Voce capturou %s!\n" RESET_COLOR, wildPokemon.name);
    } else {
        printf(RED "A captura falhou!\n" RESET_COLOR);
    }
}

void trainPokemon(Player* player) {
    if (player->count == 0) {
        printf(RED "Voce nao tem Pokemon para treinar!\n" RESET_COLOR);
        return;
    }

    int index;
    printf("Escolha um Pokemon para treinar (0 a %d): ", player->count - 1);
    scanf("%d", &index);

    if (index < 0 || index >= player->count) {
        printf(RED "Escolha invalida!\n" RESET_COLOR);
        return;
    }

    Pokemon* pokemon = &player->pokemons[index];
    pokemon->level++;
    pokemon->health = pokemon->level * 10; // Atualiza saúde com base no nível
    pokemon->attack = pokemon->level * 2; // Atualiza ataque com base no nível
    printf(GREEN "%s treinou! Novo nível: %d, Saúde: %d, Ataque: %d\n" RESET_COLOR, 
           pokemon->name, pokemon->level, pokemon->health, pokemon->attack);
}

void battle(Player* player) {
    if (player->count < 2) {
        printf(RED "Voce precisa de pelo menos 2 Pokemon para batalhar!\n" RESET_COLOR);
        return;
    }

    int p1, p2;
    printf("Escolha o Pokemon 1 (0 a %d): ", player->count - 1);
    scanf("%d", &p1);
    printf("Escolha o Pokemon 2 (0 a %d): ", player->count - 1);
    scanf("%d", &p2);

    if (p1 < 0 || p1 >= player->count || p2 < 0 || p2 >= player->count) {
        printf(RED "Escolha invalida!\n" RESET_COLOR);
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

    printf(CYAN "Batalha entre %s e %s!\n" RESET_COLOR, pokemon1->name, pokemon2->name);

    // Simulação de batalha
    while (pokemon1->health > 0 && pokemon2->health > 0) {
        pokemon2->health -= pokemon1->attack * multiplier;
        printf("%s atacou %s! %s saúde: %d\n", pokemon1->name, pokemon2->name, pokemon2->name, pokemon2->health);
        if (pokemon2->health <= 0) {
            printf(GREEN "%s venceu!\n" RESET_COLOR, pokemon1->name);
            logBattle(pokemon1->name, pokemon2->name); // Registra a batalha
            return;
        }

        pokemon1->health -= pokemon2->attack;
        printf("%s atacou %s! %s saúde: %d\n", pokemon2->name, pokemon1->name, pokemon1->name, pokemon1->health);
        if (pokemon1->health <= 0) {
            printf(GREEN "%s venceu!\n" RESET_COLOR, pokemon2->name);
            logBattle(pokemon2->name, pokemon1->name); // Registra a batalha
            return;
        }
    }
}

void saveGame(Player* player) {
    FILE* file = fopen("savegame.bin", "wb"); // Abre o arquivo binário
    if (!file) {
        printf(RED "Erro ao salvar o jogo!\n" RESET_COLOR);
        return;
    }
    fwrite(player, sizeof(Player), 1, file); // Escreve os dados do player no arquivo binário
    fclose(file);
    printf(GREEN "Jogo salvo com sucesso!\n" RESET_COLOR);
}

void loadGame(Player* player) {
    FILE* file = fopen("savegame.bin", "rb"); // Abre o arquivo binário para leitura
    if (!file) {
        printf(RED "Nenhum arquivo salvo encontrado!\n" RESET_COLOR);
        return;
    }
    fread(player, sizeof(Player), 1, file); // Carrega os dados do player do arquivo binário
    fclose(file);
    printf(GREEN "Jogo carregado com sucesso!\n" RESET_COLOR);
}

int main() {
    Player player;
    player.count = 0;

    int choice;
    do {
        printf(MAGENTA "========== MENU DO JOGO ==========\n" RESET_COLOR);
        printf(YELLOW "1. Adicionar Pokemon\n");
        printf(RED "2. Listar Pokemons\n");
        printf(BLUE "3. Capturar Pokemon\n");
        printf(RED "4. Treinar Pokemon\n");
        printf(MAGENTA "5. Batalhar\n");
        printf(YELLOW "6. Salvar Jogo\n");
        printf(BLUE "7. Carregar Jogo\n");
        printf(RED "0. Sair\n");
        printf(MAGENTA "===================================\n" RESET_COLOR);
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
                capturePokemon(&player);
                break;
            case 4:
                trainPokemon(&player);
                break;
            case 5:
                battle(&player);
                break;
            case 6:
                saveGame(&player);
                break;
            case 7:
                loadGame(&player);
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf(RED "Opção invalida! Tente novamente.\n" RESET_COLOR);
        }
    } while (choice != 0);

    return 0;
}
