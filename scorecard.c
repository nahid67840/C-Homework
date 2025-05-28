#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CANDIDATES 6
#define MAX_NAME_LEN 50
#define MAX_JUDGES 5
#define CATEGORY_COUNT 5
#define SCORE_SHEET_SIZE 7

const char *categories[CATEGORY_COUNT] = {
    "Music Proficiency", "Dance", "Vocal", "Visual", "Expression"
};

typedef struct {
    char name[MAX_NAME_LEN];
    int id; // 6-digit unique ID
} Candidate;

typedef struct {
    char judgeName[MAX_NAME_LEN];
    char expertise[MAX_NAME_LEN];
} Judge;

Candidate candidates[MAX_CANDIDATES];
int scores[MAX_CANDIDATES][SCORE_SHEET_SIZE] = {0}; // [candidate][7]
int judgeEntered = 0;

void generateUniqueIDs() {
    srand(time(NULL));
    for (int i = 0; i < MAX_CANDIDATES; ++i) {
        int unique = 0;
        while (!unique) {
            unique = 1;
            int newID = rand() % 900000 + 100000;
            for (int j = 0; j < i; ++j) {
                if (candidates[j].id == newID) {
                    unique = 0;
                    break;
                }
            }
            if (unique) {
                candidates[i].id = newID;
            }
        }
    }
}

void initializeCandidates() {
    const char *names[MAX_CANDIDATES] = {
        "Jiyeon Park", "Ethan Smith", "Helena Silva",
        "Liam Wilson", "Ava Chen", "Noah Lee"
    };
    for (int i = 0; i < MAX_CANDIDATES; ++i) {
        strncpy(candidates[i].name, names[i], MAX_NAME_LEN);
    }
    generateUniqueIDs();
}

void displayHeader() {
    printf("####################################\n");
    printf("#     Audition Evaluation Entry    #\n");
    printf("####################################\n");
}

int inputScore(const char *category) {
    int score;
    do {
        printf("%s (10–100): ", category);
        scanf("%d", &score);
        if (score < 10 || score > 100) {
            printf("Invalid score. Try again.\n");
        }
    } while (score < 10 || score > 100);
    return score;
}

void enterScores(Judge *judge) {
    printf("> Judge Name: ");
    fgets(judge->judgeName, MAX_NAME_LEN, stdin);
    judge->judgeName[strcspn(judge->judgeName, "\n")] = '\0';
    printf("> Expertise: ");
    fgets(judge->expertise, MAX_NAME_LEN, stdin);
    judge->expertise[strcspn(judge->expertise, "\n")] = '\0';
    
    for (int i = 0; i < MAX_CANDIDATES; ++i) {
        printf("++++++++++++++++++++++++++++++++++++\n");
        printf("Candidate: %s\n", candidates[i].name);
        scores[i][0] = candidates[i].id;
        int total = 0;
        for (int j = 0; j < CATEGORY_COUNT; ++j) {
            scores[i][j+1] = inputScore(categories[j]);
            total += scores[i][j+1];
        }
        scores[i][6] = total;
        printf("------------------------------------\n");
    }
    judgeEntered = 1;
}

void reviewScores() {
    printf("\n++++++++++++++++++++++++++++++++++++\n");
    printf("Submission completed.\nPlease review your input!\n");
    printf("------------------------------------\n");
    for (int i = 0; i < MAX_CANDIDATES; ++i) {
        printf("%s (%d): %d\n", candidates[i].name, candidates[i].id, scores[i][6]);
    }
}

int findCandidateIndex(int id) {
    for (int i = 0; i < MAX_CANDIDATES; ++i) {
        if (candidates[i].id == id) {
            return i;
        }
    }
    return -1;
}

void modifyScores() {
    char choice;
    while (1) {
        printf("Modify a score? (Y/N): ");
        scanf(" %c", &choice);
        if (choice == 'N' || choice == 'n') break;

        int id;
        char name[MAX_NAME_LEN];
        printf("Enter candidate name: ");
        getchar(); // consume newline
        fgets(name, MAX_NAME_LEN, stdin);
        name[strcspn(name, "\n")] = '\0';
        printf("Enter candidate ID: ");
        scanf("%d", &id);

        int idx = findCandidateIndex(id);
        if (idx == -1 || strcmp(name, candidates[idx].name) != 0) {
            printf("Candidate not found.\n");
            continue;
        }

        int total = 0;
        for (int j = 0; j < CATEGORY_COUNT; ++j) {
            scores[idx][j+1] = inputScore(categories[j]);
            total += scores[idx][j+1];
        }
        scores[idx][6] = total;
        printf("Score updated for %s.\n", candidates[idx].name);
    }
}

void finalResults() {
    printf("=======================================\n");
    printf("Compiling final selection results...\n");
    printf("=======================================\n");

    // Selection sort to rank top 4
    for (int i = 0; i < MAX_CANDIDATES - 1; ++i) {
        for (int j = i + 1; j < MAX_CANDIDATES; ++j) {
            if (scores[i][6] < scores[j][6]) {
                int temp[SCORE_SHEET_SIZE];
                memcpy(temp, scores[i], sizeof(temp));
                memcpy(scores[i], scores[j], sizeof(temp));
                memcpy(scores[j], temp, sizeof(temp));

                Candidate tmpCand = candidates[i];
                candidates[i] = candidates[j];
                candidates[j] = tmpCand;
            }
        }
    }

    printf("###########################################\n");
    printf("# Congratulations! Welcome to Milliways!  #\n");
    printf("###########################################\n");
    for (int i = 0; i < 4; ++i) {
        printf("%d. %s\n", i + 1, candidates[i].name);
    }
}

int main() {
    initializeCandidates();
    Judge judge;
    char submitChoice;

    while (1) {
        displayHeader();
        enterScores(&judge);
        reviewScores();

        printf("Would you like to submit? (Y/N): ");
        scanf(" %c", &submitChoice);
        if (submitChoice == 'Y' || submitChoice == 'y') {
            printf("***Final submission completed.***\n");
            break;
        } else {
            modifyScores();
        }
    }

    finalResults();
    return 0;
}
