#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

const int NUMBER_OF_OPTIONS = 3;
enum Options {
    ROCK,
    PAPER,
    SCISSORS
};

enum Results {
    FAIL,
    TIE,
    WIN
};

enum Results fight(enum Options playerChoice, enum Options computerChoice) {
    int beatPlayerAt = (playerChoice + 1) % NUMBER_OF_OPTIONS;
    if (computerChoice == beatPlayerAt) {
        return FAIL;
    }
    if (computerChoice == playerChoice) {
        return TIE;
    }
    return WIN;
}

enum Options computerChoose(int hash) {
    srand((unsigned)time(NULL) * hash);
    unsigned randomNumber = rand();
    printf("\n----- %i\n", randomNumber);
    enum Options choice = randomNumber % NUMBER_OF_OPTIONS;
    return choice;
}

enum Options choiceToEnum(char *choice) {
    if (strlen(choice) == 0) {
        return ROCK;
    }
    char first = choice[0];
    if (first == 'r' || first == 'R') {
        return ROCK;
    }
    if (first == 'p' || first == 'P') {
        return PAPER;
    }
    if (first == 's' || first == 'S') {
        return SCISSORS;
    }

    return ROCK;
}

char* resultToString(enum Results result) {
    switch (result) {
    case WIN:
        return "WIN";
    case TIE:
        return "TIE";
    case FAIL:
        return "LOSE";
    }
    return "TIE";
}

char* choiceToString(enum Options choice) {
    switch (choice) {
    case ROCK:
        return "rock";
    case PAPER:
        return "paper";
    case SCISSORS:
        return "scissors";
    }
    return "rock";
}

void addChoiceToList(enum Options choice, int** previousChoices, size_t* lenPrevChoice) {
    size_t newLengthElements = *lenPrevChoice + 1;
    size_t newLengthBytes = newLengthElements * sizeof(**previousChoices);
    int* newArray = malloc(newLengthBytes);
    if (newArray == NULL) {
        return;
    }

    for (size_t i = 0; i < *lenPrevChoice; i++) {
        newArray[i] = (*previousChoices)[i];
    }

    free(*previousChoices);
    *previousChoices = newArray;

    (*previousChoices)[*lenPrevChoice] = choice;
    *lenPrevChoice = newLengthElements;
}

void doRound(int** previousChoices, size_t* lenPrevChoice) {
    char input[15];

    printf("(R)ock, (P)aper, or (S)cissors? Pick one.\n");
    if (fgets(input, sizeof input, stdin) == NULL) {
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';

    int sum = 0;
    for (int i = 0; i < *lenPrevChoice; i++) {
        sum += (*previousChoices)[i];
    }

    enum Options computerChoice = computerChoose(sum);
    enum Options playerChoice = choiceToEnum(&input);
    addChoiceToList(playerChoice, previousChoices, lenPrevChoice);

    enum Results result = fight(playerChoice, computerChoice);
    printf(
        "You chose %s and the enemy chose %s. You %s!",
        choiceToString(playerChoice),
        choiceToString(computerChoice),
        resultToString(result)
    );
}

int main(void) {
    size_t lenPrevChoice = 0;
    int* previousChoices = malloc(0);
    if (previousChoices == NULL) {
        printf("Your computer doesn't work.");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        doRound(&previousChoices, &lenPrevChoice);
        printf("\n");
    }

    free(previousChoices);
    return 0;
}