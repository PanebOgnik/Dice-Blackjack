#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

// Saves the player's balance to a file with their name
int save_balance(int player_stack) {
    char choice;
    char filename[100];
    int input_ok = 0;

    do {
        system("cls");
        printf("\nDealer: \"Would you like to save your balance?\" (y/n) ");
        scanf(" %c", &choice);
        getchar();
        switch (choice) {
            case 'y': {
                FILE *file;
                printf("\nDealer: \"Please enter your name\" ---> ");
                scanf("%s", filename);
                strcat(filename, ".txt");
                file = fopen(filename, "w");
                if (file == NULL) {
                    perror("\nError opening file");
                    return 1;
                }
                fprintf(file, "%d", player_stack);
                fclose(file);
                input_ok = 1;
                break;
            }
            case 'n':
                input_ok = 1;
                break;
            default:
                printf("\nInvalid input!");
                sleep(1);
                break;
        }
    } while (input_ok == 0);

    return 0;
}

// Loads the player's balance from a file with their name
int load_balance(int player_stack) {
    char choice;
    char filename[100];
    int input_ok = 0;

    do {
        system("cls");
        printf("\nDealer: \"Would you like to load your balance?\" (y/n) ");
        scanf(" %c", &choice);
        getchar();
        switch (choice) {
            case 'y': {
                FILE *file;
                printf("\nDealer: \"Enter your name\" ---> ");
                scanf("%[^\n]", filename);
                strcat(filename, ".txt");
                file = fopen(filename, "r");
                if (file == NULL) {
                    perror("\nError opening file");
                    return 1;
                }
                fscanf(file, "%d", &player_stack);
                input_ok = 1;
                fclose(file);
                return player_stack;
                break;
            }
            case 'n':
                input_ok = 1;
                break;
            default:
                printf("\nInvalid input!");
                sleep(1);
                break;
        }
    } while (input_ok == 0);

    return 100;
}

// Allows the player to place a bet and updates their stack accordingly
int bet(int *player_stack) {
    int input_ok = 0;
    int stake;

    do {
        system("cls");
        printf("Dealer: \"You have %d chips.\"", *player_stack);
        printf("\nDealer: \"Place your bet...\" ");
        scanf("%d", &stake);
        if (stake <= *player_stack) {
            *player_stack = *player_stack - stake;
            input_ok = 1;
        } else {
            printf("\nDealer: \"You can only bet %d chips...\"", *player_stack);
            sleep(1);
        }
    } while (input_ok == 0);

    printf("\nDealer: \"You bet %d chips. Your stack is now %d.\"", stake, *player_stack);
    getchar();
    sleep(1);
    return stake;
}

// Simulates a dice roll and returns the result
int roll_dice() {
    int roll = rand() % 6 + 1; // A dice roll
    return roll;
}

// Conducts the player's first round of dice rolls
int player_round(int *blackjack) {
    int d1;
    int d2;
    int play_value;
    system("cls");
    printf("\nDealer: \"Roll the dice...\" (enter) ");
    getchar();
    d1 = roll_dice();
    d2 = roll_dice();
    play_value = (d1 + d2) * 2;
    printf("\n%d %d", d1, d2);

    if (play_value > 21) {
        return play_value;
    } else {
        if (play_value < 21 && d1 == d2) {
            printf("\nDealer: \"You have a chance for BLACKJACK!\"\n");
            *blackjack = 1;
        }
    }
    return play_value;
}

// Handles the player's second round of dice rolls
int player_round2(int play_value, int *blackjack, int *round_end, int *player_loss, int *player_win) {
    char choice;
    int input_ok = 0;
    system("cls");
    printf("Dealer: \"Roll the dice...\" (enter)");
    getchar();
    int d1 = roll_dice();
    int d2 = roll_dice();
    system("cls");
    printf("\n%d %d", d1, d2);

    if (d1 == d2) {
        *blackjack = *blackjack + 1;
    }
    if (*blackjack == 2) {
        printf("\n\nDealer: \"BLACKJACK!\"");
        *round_end = 1, *player_win = 1;
        return play_value;
    }
    while (input_ok == 0) {
        system("cls");
        if (play_value + d1 > 21 && play_value + d2 > 21) {
            printf("\n\nDealer: \"Over 21 points! You lost...\"\n");
            play_value = play_value + d1 + d2;
            *round_end = 1;
            *player_loss = 1;
            return play_value;
        }
        printf("\n\nDealer: \"%d points. Which value would you like to add?\"", play_value);
        printf("\n%d (1) ", d1);
        printf("\n%d (2) ", d2);
        printf("\n%d (3)\n", d1 + d2);
        scanf("%c", &choice);
        getchar();
        switch (choice) {
            case '1':
                play_value = play_value + d1;
                input_ok = 1;
                break;
            case '2':
                play_value = play_value + d2;
                input_ok = 1;
                break;
            case '3':
                play_value = play_value + d1 + d2;
                input_ok = 1;
                break;
            default:
                printf("\nInvalid input!\n");
                sleep(1);
                break;
        }
    }
    return play_value;
}

// Asks the player if they want to roll again and processes their decision
int query(int play_value, int *blackjack, int *round_end, int *player_win, int *player_loss) {
    char roll_again;
    int input_ok = 0;
    if (*blackjack == 2) {
        printf("\n\n\nDealer: \"YOU HIT BLACKJACK!!!\"\n\n");
        *round_end = 1;
        *player_win = 1;
    }
    if (play_value > 21) {
        printf("\n\nDealer: \"Over 21 points! You lost...\"\n\n");
        *round_end = 1;
        *player_loss = 1;
        return play_value;
    }
    if (play_value == 21) {
        printf("\n\nDealer: \"Congratulations! You have exactly 21 points!\"\n\n");
        *round_end = 1;
    }
    if (play_value < 21) {
        while (input_ok == 0) {
            printf("\n\nDealer: \"%d points. Would you like to roll again?\" (y/n) ", play_value);
            scanf(" %c", &roll_again);
            getchar();
            switch (roll_again) {
                case 'y':
                    play_value = player_round2(play_value, blackjack, round_end, player_loss, player_win);
                    input_ok = 1;
                    break;
                case 'n':
                    *round_end = 1;
                    input_ok = 1;
                    break;
                default:
                    printf("\nInvalid input!\n");
                    break;
            }
        }
    }
    return play_value;
}

// Determines the winner between the player and the bank based on their scores
void winner_check(int *bank_value, int play_value, int *player_win, int *player_loss) {
    if (*bank_value > play_value && *bank_value <= 21) {
        *player_loss = 1;
    }
    if (*bank_value < play_value && play_value <= 21) {
        *player_win = 1;
    }
    if (*bank_value < play_value && play_value > 21) {
        *player_win = 0;
    }
    if (*bank_value == play_value) {
        *player_win = 1;
        *player_loss = 1;
    }
    return;
}

// Handles the bank's second round of dice rolls
int dealer_round2(int *bank_value, int play_value, int *player_win, int *player_loss) {
    int d1;
    int d2;
    d1 = roll_dice();
    d2 = roll_dice();
    system("cls");
    printf("\nDealer: \"Score: %d\"", *bank_value);
    printf("\nDealer: \"The bank rolls...\"\n");
    sleep(2);
    printf("\n%d %d", d1, d2);
    sleep(2);

    if (*bank_value + d1 < 17 && *bank_value + d2 < 17 && *bank_value + d1 + d2 >= play_value) {
        *bank_value = *bank_value + d1 + d2;
        printf("\n\nDealer: \"The bank takes %d points...\"", d1 + d2);
        return *bank_value;
    }
    if (*bank_value + d1 < 17 && *bank_value + d2 >= 17 && *bank_value + d2 <= 21 && *bank_value + d2 < play_value) {
        *bank_value = *bank_value + d1;
        printf("\n\nDealer: \"The bank takes %d points and rolls again...\"", d1);
        sleep(1);
        dealer_round2(bank_value, play_value, player_win, player_loss);
        return *bank_value;
    }
    if (*bank_value + d2 < 17 && *bank_value + d1 >= 17 && *bank_value + d1 <= 21 && *bank_value + d1 < play_value) {
        *bank_value = *bank_value + d2;
        printf("\n\nDealer: \"The bank takes %d points and rolls again...\"", d2);
        sleep(1);
        dealer_round2(bank_value, play_value, player_win, player_loss);
        return *bank_value;
    }
    if (*bank_value + d1 < 17 && *bank_value + d2 > 21) {
        *bank_value = *bank_value + d1;
        printf("\n\nDealer: \"The bank takes %d points and rolls again...\"", d1);
        sleep(1);
        dealer_round2(bank_value, play_value, player_win, player_loss);
        return *bank_value;
    }
    if (*bank_value + d1 > 21 && *bank_value + d2 < 17) {
        *bank_value = *bank_value + d2;
        printf("\n\nDealer: \"The bank takes %d points and rolls again...\"", d2);
        sleep(1);
        dealer_round2(bank_value, play_value, player_win, player_loss);
        return *bank_value;
    }
    if (*bank_value + d1 < 17 && *bank_value + d2 <= 17 && d1 < d2) {
        *bank_value = *bank_value + d1;
        printf("\n\nDealer: \"The bank takes %d points and rolls again...\"", d1);
        sleep(1);
        dealer_round2(bank_value, play_value, player_win, player_loss);
        return *bank_value;
    }
    if (*bank_value + d1 <= 17 && *bank_value + d2 < 17 && d1 >= d2) {
        *bank_value = *bank_value + d2;
        printf("\n\nDealer: \"The bank takes %d points and rolls again...\"", d2);
        sleep(1);
        dealer_round2(bank_value, play_value, player_win, player_loss);
        return *bank_value;
    }
    if (*bank_value + d1 + d2 >= 17 && *bank_value + d1 + d2 <= 21 && *bank_value + d1 + d2 < play_value && *bank_value + d1 < 17) {
        *bank_value = *bank_value + d1;
        printf("\nDealer: \"The bank takes %d points and rolls again...\"", d1);
        sleep(1);
        dealer_round2(bank_value, play_value, player_win, player_loss);
        return *bank_value;
    }
    if (*bank_value + d1 + d2 >= 17 && *bank_value + d1 + d2 <= 21 && *bank_value + d1 + d2 <= play_value && *bank_value + d2 < 17) {
        *bank_value = *bank_value + d2;
        printf("\nDealer: \"The bank takes %d points and rolls again...\"", d2);
        sleep(1);
        dealer_round2(bank_value, play_value, player_win, player_loss);
        return *bank_value;
    }
    if (*bank_value + d1 > 21 && *bank_value + d2 > 21 && *bank_value + d1 >= *bank_value + d2) {
        *bank_value = *bank_value + d2;
        printf("\n\nDealer: \"Over 21 points. The bank loses\"");
        *player_win = 1;
        return *bank_value;
    }
    if (*bank_value + d1 > 21 && *bank_value + d2 > 21 && *bank_value + d1 <= *bank_value + d2) {
        *bank_value = *bank_value + d1;
        printf("\n\nDealer: \"Over 21 points. The bank loses\"");
        *player_win = 1;
        return *bank_value;
    }
    if (*bank_value + d1 + d2 >= 17 && *bank_value + d1 + d2 <= 21 && *bank_value + d1 + d2 >= play_value) {
        *bank_value = *bank_value + d1 + d2;
        printf("\nDealer: \"The bank takes %d points.\"", d1 + d2);
        return *bank_value;
    }
    if (*bank_value + d1 >= 17 && *bank_value + d2 >= 17 && *bank_value + d1 >= *bank_value + d2 && *bank_value + d1 <= 21) {
        *bank_value = *bank_value + d1;
        printf("\n\nDealer: \"The bank takes %d points.\"", d1);
        return *bank_value;
    }
    if (*bank_value + d1 >= 17 && *bank_value + d2 >= 17 && *bank_value + d1 <= *bank_value + d2 && *bank_value + d2 <= 21) {
        *bank_value = *bank_value + d2;
        printf("\n\nDealer: \"The bank takes %d points.\"", d2);
        return *bank_value;
    }
    if (*bank_value + d1 < 17 && *bank_value + d2 >= 17 && *bank_value + d2 <= 21) {
        *bank_value = *bank_value + d2;
        printf("\n\nDealer: \"The bank takes %d points.\"", d2);
        return *bank_value;
    }
    if (*bank_value + d2 < 17 && *bank_value + d1 >= 17 && *bank_value + d1 <= 21) {
        *bank_value = *bank_value + d1;
        printf("\n\nDealer: \"The bank takes %d points.\"", d1);
        return *bank_value;
    }
    if (*bank_value + d2 > 21 && *bank_value + d1 >= play_value) {
        *bank_value = *bank_value + d1;
        printf("\n\nDealer: \"The bank takes %d points.\"", d1);
        return *bank_value;
    }
    if (*bank_value + d1 > 21 && *bank_value + d2 >= play_value) {
        *bank_value = *bank_value + d2;
        printf("\n\nDealer: \"The bank takes %d points.\"", d2);
        return *bank_value;
    }
    return *bank_value;
}

// Conducts the bank's first round of dice rolls
int dealer_round(int play_value, int *bank_value, int *player_win, int *player_loss) {
    int d1 = roll_dice();
    int d2 = roll_dice();
    system("cls");
    printf("\nDealer: \"The bank rolls...\"\n\n");
    sleep(2);
    printf("\n%d %d", d1, d2);
    *bank_value = (d1 + d2) * 2;
    sleep(1);

    if (*bank_value > 21) {
        printf("\n\nDealer: \"Over 21 points. The bank loses...\"");
        *player_win = 1;
    }
    if (*bank_value > 17 && *bank_value > play_value && *bank_value <= 21) {
        *player_loss = 1;
    }
    if (*bank_value > 17 && *bank_value == play_value) {
        *player_win = 1;
        *player_loss = 1;
    }
    if (*bank_value > 17 && *bank_value < play_value) {
        *player_win = 1;
    }
    if (*bank_value < 17) {
        dealer_round2(bank_value, play_value, player_win, player_loss);
    }

    return *bank_value;
}

// Checks the outcome of the round and updates the player's stack accordingly
void round_winner_check(int player_win, int player_loss, int blackjack, int *player_stack, int stake) {
    if (player_win == 1 && player_loss == 1) {
        *player_stack = *player_stack + stake;
        printf("\nDealer: \"It's a tie! You get your bet back. Your stack is now %d chips.\"", *player_stack);
    }
    if (player_win == 1 && blackjack == 2) {
        *player_stack = *player_stack + 3 * stake;
        printf("\nDealer: \"BLACKJACK! You triple your bet...Your stack is now %d chips.\"", *player_stack);
    }
    if (player_win == 1 && player_loss == 0 && blackjack != 2) {
        *player_stack = *player_stack + 2 * stake;
        printf("\nDealer: \"Congratulations! You double your bet...Your stack is now %d chips.\"", *player_stack);
    }
    if (player_loss == 1 && player_win == 0) {
        printf("\nDealer: \"The bank wins! You lose your bet...Your stack is now %d chips.\"", *player_stack);
    }
    return;
}

// Main function to execute the game loop
int main() {
    int play_again = 1;
    int player_stack = 0;
    player_stack = load_balance(player_stack);

    do {
        srand(time(0));
        char choice;
        int player_win = 0;
        int player_loss = 0;
        int round_end = 0;
        int play_value;
        int bank_value = 0;
        int blackjack = 0;
        int stake = bet(&player_stack);
        sleep(1);
        play_value = player_round(&blackjack);
        play_value = query(play_value, &blackjack, &round_end, &player_win, &player_loss);
        while (round_end == 0) {
            blackjack = 0;
            play_value = query(play_value, &blackjack, &round_end, &player_win, &player_loss);
        }
        printf("\nDealer: \"Your round is over. Press any key... \"");
        getchar();

        if (player_win == 0 && player_loss == 0) {
            dealer_round(play_value, &bank_value, &player_win, &player_loss);
        }
        winner_check(&bank_value, play_value, &player_win, &player_loss);
        round_winner_check(player_win, player_loss, blackjack, &player_stack, stake);

        printf("\n\n\n\nPlayer: %d", play_value);
        printf("\nBank: %d", bank_value);
        printf("\n\nDealer: \"Would you like to play another round?\" (y/n) ");
        scanf(" %c", &choice);
        getchar();
        switch (choice) {
            case 'y':
                break;
            case 'n':
                play_again = 0;
                save_balance(player_stack);
                break;
            default:
                printf("\nInvalid input!");
                sleep(1);
                break;
        }

    } while (player_stack > 0 && play_again == 1);

    if (player_stack == 0) {
        printf("\n\nDealer: \"Unfortunately, you lost everything. Better luck next time!\"");
    } else {
        printf("\n\nDealer: \"Thanks for playing!\"");
    }
    printf("\n\n\n");
    return 0;
}
