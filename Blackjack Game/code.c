#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct Card_struct {
	int suit;
	int value;
	char suitName[9];
	char valueName[7];
} Card;

Card createCardFunc(int i, int j);
void createDeckFunc(Card deck[]);
void deckShuffle(Card deck[]);
void listPop(int popIndex, int len, Card deck[]);
void cardAllocate(Card deck[], Card p_hand[], Card d_hand[]);
void printHands(Card hand[], int len);
void hitFunc(Card deck[], Card p_hand[], int* p_handSize);
bool bustCheck(Card hand[], int len);
bool compSumCheck(Card hand[], int len);
int handCompare(Card d_hand[], Card p_hand[], int d_len, int p_len);
bool mainGame(); //this will return whether the user wants to play again

int main() {

	while (mainGame());
	printf("Thanks for playing!");

	return 0;
}

Card createCardFunc(int i, int j) {
	Card card;
	card.suit = i;
	card.value = j;
	char valueChar[3];
	switch (card.suit) {
	case 0:
		strcpy(card.suitName, "Hearts");
		break;
	case 1:
		strcpy(card.suitName, "Clubs");
		break;
	case 2:
		strcpy(card.suitName, "Diamonds");
		break;
	case 3:
		strcpy(card.suitName, "Spades");
		break;
	}

	if (card.value == 1 || card.value > 9) {
		switch (card.value) {
		case 1:
			strcpy(card.valueName, "Ace\0");
			card.value = 11;
			break;
		case 11:
			strcpy(card.valueName, "Jack\0");
			card.value = 10;
			break;
		case 12:
			strcpy(card.valueName, "Queen\0");
			card.value = 10;
			break;
		case 13:
			strcpy(card.valueName, "King\0");
			card.value = 10;
			break;
		}
	}
	else {
		sprintf(valueChar, "%d", card.value);
		strcpy(card.valueName, valueChar);
	}
	return card;
}

void createDeckFunc(Card deck[]) {
	int cardIter = 0;

	for (int i = 0; i < 4; i++) {
		for (int j = 1; j < 14; j++) {
			deck[cardIter] = createCardFunc(i, j);
			cardIter++;
		}
	}
}

void deckShuffle(Card deck[]) {
	int randInt;
	Card temp;
	time_t t;

	srand((unsigned)time(&t));
	//info on rand, and time libraries obained from TutorialsPoint -> https://www.tutorialspoint.com

	for (int i = 0; i < 52; i++) {
		randInt = rand() % 52;
		temp = deck[i];
		deck[i] = deck[randInt];
		deck[randInt] = temp;
	}
}

void listPop(int popIndex, int len, Card deck[]) {
	//printf("Popping %s of %s\n", deck[popIndex].valueName, deck[popIndex].suitName);
	for (int i = 0; i < len; i++) {
		deck[i] = deck[i + 1];
	}
}

void cardAllocate(Card deck[], Card p_hand[], Card d_hand[]) {
	for (int i = 0; i < 2; i++) {
		p_hand[i] = deck[0];
		listPop(0, 52, deck);
	}
	for (int i = 0; i < 2; i++) {
		d_hand[i] = deck[0];
		listPop(0, 52, deck);
	}
}

void printHands(Card hand[], int len) {
	for (int i = 0; i < len; i++) {
		printf("| %s of %s | ", hand[i].valueName, hand[i].suitName);
	}
	printf("\n");
}

void hitFunc(Card deck[], Card p_hand[], int* p_handSize) {
	p_hand[(*p_handSize)] = deck[0];
	listPop(0, 52, deck);
}

bool bustCheck(Card hand[], int len) {
	int sum;
	bool restart = true;
	while (restart) {
		sum = 0;
		for (int i = 0; i < len; i++) {
			sum += hand[i].value;
		}
		if (sum > 21) {
			restart = false;
			for (int i = 0; i < len; i++) {
				if (hand[i].value == 11) {
					hand[i].value = 1;
					restart = true;
				}
			}
		}
		else {
			return false;
		}
	}
	return true;
}

bool compSumCheck(Card hand[], int len) {
	int sum = 0;
	for (int i = 0; i < len; i++) {
		sum += hand[i].value;
	}
	printf("Comp sum = %d\n", sum);
	if (sum > 16) {
		return true;
	}
	return false;
}

int handCompare(Card d_hand[], Card p_hand[], int d_len, int p_len) {
	int d_sum = 0, p_sum = 0;
	for (int i = 0; i < d_len; i++) {
		d_sum += d_hand[i].value;
	}
	for (int i = 0; i < p_len; i++) {
		p_sum += p_hand[i].value;
	}
	printf("Dealer sum = %d\n", d_sum);
	printf("You're sum = %d\n", p_sum);
	if (d_sum > p_sum) {
		return 0; //dealer win
	}
	else if (p_sum > d_sum) {
		return 1; //player win
	}
	return 2; //tie
}

bool mainGame() {
	int deckSize = 52, playerHandSize = 2, computerHandSize = 2, result;
	Card deckList[52], playerHand[52], computerHand[52];
	bool done = false, bust = false;
	char playerChoice[6];

	createDeckFunc(deckList);
	deckShuffle(deckList);
	/*for (int i = 0; i < 52; i++) {
		printf("%s of %s\n", deckList[i].valueName, deckList[i].suitName);
	} */
	cardAllocate(deckList, playerHand, computerHand);

	while (!done) {
		printf("Your hand is: ");
		printHands(playerHand, playerHandSize);
		printf("The dealers hand is currently shows as: ");
		printHands(computerHand, 1);
		printf("Would you like to Hit, or Stand?: ");
		scanf("%s", playerChoice);

		if (strcmp("Stand", playerChoice) == 0 || strcmp("stand", playerChoice) == 0) {
			printf("You've Stood\n");
			done = true;
		}
		else if (strcmp("Hit", playerChoice) == 0 || strcmp("hit", playerChoice) == 0) {
			hitFunc(deckList, playerHand, &playerHandSize);
			playerHandSize++;
			printf("You've hit!\n");
			bust = bustCheck(playerHand, playerHandSize);
			if (bust) {
				printf("And you've bust.\n");
				printf("You're hand was: ");
				printHands(playerHand, playerHandSize);
				done = true;
			}
		}
		else {
			printf("Please either Hit or Stand\n");
		}

	}

	if (!bust) {
		while (!compSumCheck(computerHand, computerHandSize)) {
			printf("The dealer reveals the hand: ");
			printHands(computerHand, computerHandSize);
			hitFunc(deckList, computerHand, computerHandSize); //This is not currently working
			computerHandSize++;
			bust = bustCheck(computerHand, computerHandSize);
			if (bust) {
				printf("The computer has bust, their hand was: ");
				printHands(computerHand, computerHandSize);
			}
		}
	}

	if (!bust) {
		printf("The dealers final hand is: \n");
		printHands(computerHand, computerHandSize);
		result = handCompare(computerHand, playerHand, computerHandSize, playerHandSize);
		if (result == 0) {
			printf("The dealer wins!\n");
		}
		else if (result == 1) {
			printf("You win!\n");
		}
		else {
			printf("You tie with the dealer!\n");
		}
	}

	return false;
}