
enum Suit {
    HEARTS,
    SPADES,
    DIAMONDS,
    CLUBS
};

struct Card {
    enum Suit suit;
    int rank; /* 0-12 for 2-Ace */
};

struct Deck {
    int count;
    struct Card* cards;
};

struct Player {
    /* Two cards in a players hand */
    struct Card c1;
    struct Card c2;

    /* Players chip value */
    double money;
    double money_bet_on_hand;

    /* If the player is playing in the hand */
    bool inHand;
};

struct Board {
    /* Amount to be won */
    double pot;

    /* Flop */
    struct Card* flop1;
    struct Card* flop2;
    struct Card* flop3;

    /* Turn */
    struct Card* turn;

    /* River */
    struct Card* river;
};

static struct Deck* generate_deck();
static void print_deck(struct Deck* deck);
static void shuffle_deck(struct Deck* deck);
static void deal_cards(struct Deck* deck, struct Player* players);
static void game_loop(struct Deck* deck, struct Player* players);
static char* suit_to_char(struct Card card);
static char* rank_to_char(struct Card card);