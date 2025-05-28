#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STAGES 8

char stages[MAX_STAGES] = {'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N'};

void displayMainMenu();
void displayTrainingMenu();
void handleAuditionManagement();
void handleTraining();
void handleDebut();
void stageEvaluation(int stage);
int checkTrainingCompletion();

int main() {
    char choice;

    while (1) {
        displayMainMenu();
        printf("Enter your choice (0, Q to quit): ");
        choice = getchar();
        getchar();  

        if (choice == '0' || choice == 'Q' || choice == 'q') {
            break;
        }

        switch (choice) {
            case '1':
                handleAuditionManagement();
                break;
            case '2':
                handleTraining();
                break;
            case '3':
                handleDebut();
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

void displayMainMenu() {
    printf("\n=== Main Menu ===\n");
    printf("1. Audition Management\n");
    printf("2. Training\n");
    printf("3. Debut\n");
    printf("0. Exit\n");
    printf("Q. Quit\n");
}

void handleAuditionManagement() {
    printf("\nYou selected Audition Management.\n");
}

void handleTraining() {
    char choice;

    while (1) {
        displayTrainingMenu();
        printf("Enter your choice (0 to go back): ");
        choice = getchar();
        getchar();  

        if (choice == '0') {
            break;
        }

        int stage = choice - '1'; 

        if (stage < 0 || stage >= MAX_STAGES) {
            printf("Invalid choice! Please select a valid stage.\n");
        } else if (stages[stage] == 'P') {
            printf("Stage %d has already been passed.\n", stage + 1);
        } else if (stage == 0 || stage == 1) {
            stageEvaluation(stage);  
        } else if (checkTrainingCompletion() >= 2) {
            stageEvaluation(stage);
        } else {
            printf("Stages 1 and 2 must be completed before selecting other stages.\n");
        }
    }
}

void displayTrainingMenu() {
    printf("\n=== Training Menu ===\n");
    printf("1. Physical Strength & Knowledge\n");
    printf("2. Self-Management & Teamwork\n");
    printf("3. Language & Pronunciation\n");
    printf("4. Vocal\n");
    printf("5. Dance\n");
    printf("6. Visual & Image\n");
    printf("7. Acting & Stage Performance\n");
    printf("8. Fan Communication\n");
    printf("0. Go Back\n");
}

void handleDebut() {
    printf("\nYou selected Debut.\n");
}

void stageEvaluation(int stage) {
    char result;
    printf("\nWould you like to enter the evaluation result? (Y/N): ");
    scanf(" %c", &result);
    if (result == 'Y' || result == 'y') {
        printf("Did you complete the training and pass the certification? (P/F): ");
        scanf(" %c", &result);
        if (result == 'P' || result == 'p') {
            stages[stage] = 'P';
            printf("Stage %d passed.\n", stage + 1);
        } else {
            stages[stage] = 'F';
            printf("Stage %d failed.\n", stage + 1);
        }
    }
}

int checkTrainingCompletion() {
    int completed = 0;
    for (int i = 0; i < MAX_STAGES; i++) {
        if (stages[i] == 'P') {
            completed++;
        }
    }
    return completed;
}
