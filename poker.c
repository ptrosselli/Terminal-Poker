#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "poker.h"
#define MAXPLAYERS 6
#define MAXCARDS 52

static struct Deck* generate_deck() {
    struct Deck* deck = malloc(sizeof(struct Deck));
    struct Card* cards = malloc(MAXCARDS * sizeof(struct Card));
    int total = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            cards[total].suit = i;
            cards[total].rank = j;
            total++;
        }
    }
    deck->cards = cards;
    deck->count = total;
    return deck;
}

static char* suit_to_char(struct Card card) {
    char* s;
    switch (card.suit) {
        case HEARTS:
            s = "hearts";
            break;
        case SPADES:
            s = "spades";
            break;
        case DIAMONDS:
            s = "diamonds";
            break;
        case CLUBS:
            s = "clubs";
            break;
        default:
            s = "smth went wrong retry";
            break;
    }
    return s;
}

static char* rank_to_char(struct Card card) {
    char* r;
    if (card.rank == 12) {
        r = "Ace";
    }
    else if (card.rank == 11) {
        r = "King";
    }
    else if (card.rank == 10) {
        r = "Queen";
    }
    else if (card.rank == 9) {
        r = "Jack";
    }
    else {
        r = malloc(3 * sizeof(char));
        snprintf(r, 3, "%d", card.rank + 2);
    }
    return r;
}

static void print_deck(struct Deck* deck) {
    for (int i = 0; i < deck->count; i++) {
        printf("C %d    S: %d    R: %d\n", i, deck->cards[i].suit, deck->cards[i].rank);
    }
}

static void shuffle_deck(struct Deck* deck) {
    srand(time(NULL)); // Seed the random number generator
    for (int i = deck->count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Swap array[i] and array[j]
        struct Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
}

static struct Card pop(struct Deck* deck) {
    struct Card c = deck->cards[deck->count - 1];
    deck->count--;
    return c;
}

static void deal_cards(struct Deck* deck, struct Player* players) {
    for (int i = 0; i < MAXPLAYERS; i++) {
        players[i].c1 = pop(deck);
    }
    for (int j = 0; j < MAXPLAYERS; j++) {
        players[j].c2 = pop(deck);
    }
}

static void game_loop(struct Deck* deck, struct Player* players) {
    int dealer = 0;
    bool live = true;

    struct Board* table = malloc(sizeof(struct Board));
    table->pot = 0.00;

    double prev_largest_bet = 0.00;

    while (live) {
        dealer = dealer % 6;
        if (dealer == 0) {
            tryagain:
            printf("----------------------------------------\n"); //40 -'s
            printf("You're Turn!\n There are currently $%.2f in the pot.\n", table->pot);
            printf("You currently have $%.2f in your stack.\n", players[0].money);
            printf("You're hand is %s of %s, %s of %s\n", rank_to_char(players[0].c1), suit_to_char(players[0].c1), rank_to_char(players[0].c2), suit_to_char(players[0].c2));
            printf("Check, Raise, or Fold 'c'/'r'/'f': ");
            
            char ans[100] = "p";
            scanf("%s", ans);
            printf("ANS %s\n", ans);

            if (strcmp(ans, "c") == 0) {
                printf("You check.\n");
            }
            else if (strcmp(ans, "r") == 0) {
                printf("How much would you like to raise? (-1 to go back): ");
                double x = 0.0;
                scanf("%lf", &x);

                if (x == -1) {
                    goto tryagain;
                }
                else if (x >= (2 * prev_largest_bet)) {
                    table->pot += x;
                    prev_largest_bet = x;
                    printf("You raised $%.2f, the pot is now $%.2f\n", x, table->pot);
                }
            }
            else if (strcmp(ans, "f") == 0) {
                printf("You fold.\n");
            }
            else {
                printf("----------------------------------------\n");
                printf("**You must enter either 'c' 'r' or 'f' to check, raise, or fold**\n");
                goto tryagain;
            }
        }
        live = false;
    }

    free(table);
}

int main() {
    /* Generate a new deck and shuffle it */
    struct Deck* deck = generate_deck();
    shuffle_deck(deck);
    //print_deck(deck);

    /* Deal cards to the alloted players */
    struct Player* players = malloc(MAXPLAYERS * sizeof(struct Player));
    deal_cards(deck, players);

    /* Player settings prior to game loop */
    for (int i = 0; i < MAXPLAYERS; i++) {
        //printf("P%d: C1 %d/%d, C2 %d/%d\n", i, players[i].c1.suit, players[i].c1.rank, players[i].c2.suit, players[i].c2.rank);
        players[i].money = 10.00;
        players[i].money_bet_on_hand = 0.00;
        players[i].inHand = true;
    }

    printf("\nWelcome to the Poker table!\n > All players will start with $10.00\n");
    printf(" > You will start as the dealer\n");
    printf(" > The rules are standard No Limit Texas Hold'em\n");
    printf(" > Small blind is 50¢, big blind is $1\n");
    printf(" > The game ends when you go out or win all the money\n");
    printf("*** Type q at any time to quit! ***\n");
    printf("Hit ENTER to start the Game!");
    
    getchar();

    game_loop(deck, players);
}