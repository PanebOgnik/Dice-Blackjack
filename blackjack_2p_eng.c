#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

// Function to roll a dice value between 1 and 6
int dice() {
    int roll = rand() % 6 + 1;
    return roll;
}

// Function to ask for player names
void ask_names(char *player1_name, char *player2_name) {
    system("cls");
    printf("\nDealer: \"Please enter the name of Player 1\" ---> ");
    scanf("%s", player1_name);
    getchar();
    printf("\nDealer: \"Please enter the name of Player 2\" ---> ");
    scanf("%s", player2_name);
    getchar();
    return;
}

// Function to determine the order of players
void determine_order(int *player1_dealer, int *player2_dealer, char player1_name[20], char player2_name[20]) {
    int w1 = 0;
    int w2 = 0;
    do {
        system("cls");
        printf("\nDealer: \"The player with the higher value starts as the dealer...\"");
        printf("\nDealer: \"%s, please roll the dice...\" (enter) ", player1_name);
        getchar();
        w1 = dice();
        printf("\n%d\n", w1);
        printf("\nDealer: \"%s, please roll the dice...\" (enter) ", player2_name);
        getchar();
        w2 = dice();
        printf("\n%d", w2);
        sleep(2);
        if (w1 > w2) {
            printf("\nDealer: \"%s, you start as the dealer!\"", player1_name);
            sleep(2);
            *player1_dealer = 1;
        }
        if (w2 > w1) {
            printf("\nDealer: \"%s, you start as the dealer!\"", player2_name);
            sleep(2);
            *player2_dealer = 1;
        }
    } while (w1 == w2);
    return;
}

// Function to place the bet
int place_bet(int *player1_stack, int *player2_stack, char player_name[20]) {
    int input_ok = 0;
    int bet;
    do {
        system("cls");
        printf("\n\nDealer: \"%s, you have %d chips.\"", player_name, *player1_stack);
        printf("\nDealer: \"Please place your bet...\" ");
        scanf("%d", &bet);
        if (bet <= *player1_stack) {
            *player1_stack = *player1_stack - bet;
            *player2_stack = *player2_stack - bet;
            input_ok = 1;
        } else {
            printf("\nDealer: \"You can bet a maximum of %d chips...\"", *player1_stack);
            sleep(1);
        }
    } while (input_ok == 0);
    printf("\nDealer: \"You have bet %d chips. Your stack is now %d.\"", bet, *player1_stack);
    getchar();
    sleep(2);
    return bet;
}

// Function to play a round for the player
int player_round(int *blackjack, char player_name[20]) {
    int w1;
    int w2;
    int game_value;
    system("cls");
    printf("\nDealer: \"%s, please roll the dice...\" (enter) ", player_name);
    getchar();
    w1 = dice();
    w2 = dice();
    game_value = (w1 + w2) * 2;
    printf("\n%d %d", w1, w2);
    if (game_value > 21) {
        return game_value;
    } else {
        if (game_value < 21 && w1 == w2) {
            printf("\nDealer: \"You have a chance for a BLACKJACK!\"\n");
            *blackjack = 1;
        }
    }
    return game_value;
}

// Function to play the second round for the player
int player_second_round(int game_value, int *blackjack, int *round_end, int *player_loss, int *player_win) {
    char choice;
    int input_ok = 0;
    system("cls");
    printf("Dealer: \"Please roll the dice...\" (enter)");
    getchar();
    int w1 = dice();
    int w2 = dice();
    system("cls");
    printf("\n%d %d", w1, w2);
    if (w1 == w2) {
        *blackjack = *blackjack + 1;
    }
    if (*blackjack == 2) {
        printf("\n\nDealer: \"BLACKJACK!\"");
        *round_end = 1;
        *player_win = 1;
        return game_value;
    }
    while (input_ok == 0) {
        system("cls");
        printf("\n%d %d", w1, w2);
        if (game_value + w1 > 21 && game_value + w2 > 21) {
            printf("\n\nDealer: \"Exceeded 21 points! You lost...\"\n");
            game_value = game_value + w1 + w2;
            *round_end = 1;
            *player_loss = 1;
            return game_value;
        }
        printf("\n\nDealer: \"%d points. Which value would you like to add?\"", game_value);
        printf("\n%d (1) ", w1);
        printf("\n%d (2) ", w2);
        printf("\n%d (3)\n", w1 + w2);
        scanf("%c", &choice);
        getchar();
        switch (choice) {
        case '1':
            game_value = game_value + w1;
            input_ok = 1;
            break;
        case '2':
            game_value = game_value + w2;
            input_ok = 1;
            break;
        case '3':
            game_value = game_value + w1 + w2;
            input_ok = 1;
            break;
        default:
            printf("\nInvalid input!\n");
            sleep(1);
            break;
        }
    }
    return game_value;
}

// Function to ask the player for a decision after a round
int ask_player(int game_value, int *blackjack, int *round_end, int *player_win, int *player_loss) {
    char roll_again;
    int input_ok = 0;
    if (*blackjack == 2) {
        printf("\n\n\nDealer: \"YOU HIT THE BLACKJACK!!!\"\n\n");
        *round_end = 1;
        *player_win = 1;
    }
    if (game_value > 21) {
        printf("\n\nDealer: \"Exceeded 21 points! You lost...\"\n\n");
        *round_end = 1;
        *player_loss = 1;
        return game_value;
    }
    if (game_value == 21) {
        printf("\n\nDealer: \"Congratulations! You have exactly 21 points!\"\n\n");
        *round_end = 1;
    }
    if (game_value < 21) {
        while (input_ok == 0) {
            printf("\n\nDealer: \"%d points. Would you like to roll again?\" (y/n) ", game_value);
            scanf(" %c", &roll_again);
            getchar();
            switch (roll_again) {
            case 'y':
                game_value = player_second_round(game_value, blackjack, round_end, player_loss, player_win);
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
    return game_value;
}

// Function to play the second round for the dealer
int dealer_second_round(int game_value, int *bank_value, int *player_win, int *player_loss) {
    int w1 = dice();
    int w2 = dice();
    char choice;
    int input_ok = 0;
    system("cls");
    printf("\nDealer: \"Please roll the dice...\" (enter)");
    getchar();
    printf("\nDealer: \"The bank rolls...\"\n\n");
    sleep(1);
    printf("\n%d %d", w1, w2);
    while (input_ok == 0) {
        printf("\n\nDealer: \"%d points. Which value would you like to add?\" ", *bank_value);
        printf("\n%d (1) ", w1);
        printf("\n%d (2) ", w2);
        printf("\n%d (3) ", w1 + w2);
        scanf(" %c", &choice);
        getchar();
        switch (choice) {
        case '1':
            *bank_value = *bank_value + w1;
            input_ok = 1;
            break;
        case '2':
            *bank_value = *bank_value + w2;
            input_ok = 1;
            break;
        case '3':
            *bank_value = *bank_value + w1 + w2;
            input_ok = 1;
            break;
        default:
            printf("\nInvalid input");
            sleep(1);
            break;
        }
    }
    return *bank_value;
}

// Function to ask the dealer for a decision after a round
int ask_dealer(int game_value, int *bank_value, int *player_win, int *player_loss) {
    int input_ok = 0;
    char choice;
    if (*bank_value > 21) {
        printf("\n\nDealer: \"Exceeded 21 points. The bank loses...\"");
        *player_win = 1;
    }
    if (*bank_value >= 17 && *bank_value > game_value && *bank_value <= 21) {
        *player_loss = 1;
    }
    if (*bank_value >= 17 && *bank_value == game_value) {
        *player_win = 1;
        *player_loss = 1;
    }
    if (*bank_value >= 17 && *bank_value < game_value) {
        *player_win = 1;
    }
    if (*bank_value < 17) {
        while (input_ok == 0) {
            printf("\n\nDealer: \"%d points. Would you like to roll again?\"(y/n) ", *bank_value);
            scanf(" %c", &choice);
            getchar();
            switch (choice) {
            case 'y':
                game_value = dealer_second_round(game_value, bank_value, player_win, player_loss);
                input_ok = 1;
                break;
            case 'n':
                input_ok = 1;
                break;
                return *bank_value;
            default:
                printf("\nInvalid input!");
                sleep(1);
                break;
            }
        }
    }
    return *bank_value;
}

// Function to play a round for the dealer
int dealer_round(int bank_value, char player_name[20]) {
    int w1 = dice();
    int w2 = dice();
    system("cls");
    printf("\nDealer: \"%s, please roll the dice...\" (enter) ", player_name);
    getchar();
    printf("\nDealer: \"The bank rolls...\"\n\n");
    printf("\n%d %d", w1, w2);
    bank_value = (w1 + w2) * 2;
    return bank_value;
}

// Function to check the winner of the round
void check_round_winner(int player_win, int player_loss, int blackjack, int *player1_stack, int *player2_stack, int bet, char player1_name[20], char player2_name[20], int *player1_dealer, int *player2_dealer) {
    if (player_win == 1 && player_loss == 1) {
        *player1_stack = *player1_stack + bet;
        *player2_stack = *player2_stack + bet;
        printf("\nDealer: \"Draw! You get your bet back.");
    }
    if (player_win == 1 && blackjack == 2) {
        *player1_stack = *player1_stack + 3 * bet;
        *player2_stack = *player2_stack - bet;
        *player1_dealer = 1;
        *player2_dealer = 0;
        printf("\nDealer: \"BLACKJACK! %s, you triple your bet...\"", player1_name);
    }
    if (player_win == 1 && player_loss == 0 && blackjack != 2) {
        *player1_stack = *player1_stack + 2 * bet;
        *player1_dealer = 1;
        *player2_dealer = 0;
        printf("\nDealer: \"Congratulations! %s, you double your bet!\"", player1_name);
    }
    if (player_loss == 1 && player_win == 0) {
        *player2_stack = *player2_stack + 2 * bet;
        printf("\nDealer: \"The bank wins! %s, you lose your bet...\"", player1_name);
    }
    return;
}

// Main program
int main() {
    int play_again = 1;
    char player1_name[20];
    char player2_name[20];
    do {
        srand(time(0));
        int player1_dealer = 0;
        int player2_dealer = 0;
        int player1_stack = 100;
        int player2_stack = 100;
        int blackjack;
        int player_win;
        int player_loss;
        int game_value;
        int bank_value;
        int round_end;
        int bet;
        char restart;
        ask_names(player1_name, player2_name);
        determine_order(&player1_dealer, &player2_dealer, player1_name, player2_name);
        do {
            blackjack = 0;
            player_win = 0;
            player_loss = 0;
            game_value = 0;
            bank_value = 0;
            round_end = 0;
            if (player1_dealer == 0) {
                bet = place_bet(&player1_stack, &player2_stack, player1_name);
                game_value = player_round(&blackjack, player1_name);
                game_value = ask_player(game_value, &blackjack, &round_end, &player_win, &player_loss);
                while (round_end == 0) {
                    blackjack = 0;
                    game_value = ask_player(game_value, &blackjack, &round_end, &player_win, &player_loss);
                }
                printf("\nDealer: \"Your round is over. Press any key... \"");
                getchar();
                if (player_win == 0 && player_loss == 0) {
                    bank_value = dealer_round(bank_value, player2_name);
                    bank_value = ask_dealer(game_value, &bank_value, &player_win, &player_loss);
                    while (player_win == 0 && player_loss == 0) {
                        bank_value = ask_dealer(game_value, &bank_value, &player_win, &player_loss);
                    }
                }
                check_round_winner(player_win, player_loss, blackjack, &player1_stack, &player2_stack, bet, player1_name, player2_name, &player1_dealer, &player2_dealer);
            } else {
                bet = place_bet(&player2_stack, &player1_stack, player2_name);
                game_value = player_round(&blackjack, player2_name);
                game_value = ask_player(game_value, &blackjack, &round_end, &player_win, &player_loss);
                while (round_end == 0) {
                    blackjack = 0;
                    game_value = ask_player(game_value, &blackjack, &round_end, &player_win, &player_loss);
                }
                printf("\nDealer: \"Your round is over. Press any key... \"");
                getchar();
                if (player_win == 0 && player_loss == 0) {
                    bank_value = dealer_round(bank_value, player1_name);
                    bank_value = ask_dealer(game_value, &bank_value, &player_win, &player_loss);
                    while (player_win == 0 && player_loss == 0) {
                        bank_value = ask_dealer(game_value, &bank_value, &player_win, &player_loss);
                    }
                }
                check_round_winner(player_win, player_loss, blackjack, &player2_stack, &player1_stack, bet, player2_name, player1_name, &player2_dealer, &player1_dealer);
            }
            printf("\n\nPlayer: %d", game_value);
            printf("\nBank: %d", bank_value);
            printf("\n\n%s has %d chips", player1_name, player1_stack);
            printf("\n\n%s has %d chips", player2_name, player2_stack);
            printf("\n\nNext round... (enter)");
            getchar();
        } while (player1_stack > 0 && player2_stack > 0);
        if (player1_stack <= 0) {
            printf("\n\nDealer: \"%s, you have no more chips. Congratulations, %s!!! You won!\"", player1_name, player2_name);
        }
        if (player2_stack <= 0) {
            printf("\n\nDealer: \"%s, you have no more chips. Congratulations, %s!!! You won!\"", player2_name, player1_name);
        }
        printf("\n\nDo you want to play again? (y/n)");
        scanf(" %c", &restart);
        getchar();
        switch (restart) {
        case 'y':
            break;
        case 'n':
            play_again = 0;
            printf("\nThanks for playing!");
            break;
        default:
            printf("\nInvalid input!");
            sleep(1);
            break;
        }
    } while (play_again == 1);
    printf("\n\n\n");
    return 0;
}
