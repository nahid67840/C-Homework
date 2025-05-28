#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_MENU_LEN 30
#define TRAINING_COUNT 8
#define MAIN_MENU_COUNT 3

// Training stage result states: ' ' = not attempted, 'P' = passed, 'F' = failed
char trainingResults[TRAINING_COUNT] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

const char mainMenu[MAIN_MENU_COUNT][MAX_MENU_LEN] = {
    "I. Audition Management",
    "II. Training",
    "III. Debut"
};

const char trainingMenu[TRAINING_COUNT][MAX_MENU_LEN] = {
    "1. Physical Strength & Knowledge",
    "2. Self-Management & Teamwork",
    "3. Language & Pronunciation",
    "4. Vocal",
    "5. Dance",
    "6. Visual & Image",
    "7. Acting & Stage Performance",
    "8. Fan Communication"
};

void clearInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void displayMainMenu() {
    printf("\n========= Main Menu =========\n");
    for (int i = 0; i < MAIN_MENU_COUNT; ++i) {
        printf("%d. %s\n", i + 1, mainMenu[i]);
    }
    printf("0 / Q / q to exit\n");
    printf("=============================\n");
}

void displayTrainingMenu() {
    printf("\n===== Training Stage Menu =====\n");
    for (int i = 0; i < TRAINING_COUNT; ++i) {
        printf("%d. %s [%c]\n", i + 1, trainingMenu[i], trainingResults[i]);
    }
    printf("0 / Q / q to go back\n");
    printf("===============================\n");
}

void evaluateStage(int stageIndex) {
    char response;
    printf("Would you like to enter the evaluation result? (Y/N): ");
    scanf(" %c", &response);
    response = toupper(response);
    clearInput();

    if (response == 'Y') {
        if (trainingResults[stageIndex] != ' ') {
            printf("This stage has already been evaluated.\n");
            return;
        }

        // Enforce stage 1 and 2 sequencing
        if ((stageIndex == 1 && trainingResults[0] != 'P') ||
            (stageIndex > 1 && (trainingResults[0] != 'P' || trainingResults[1] != 'P'))) {
            printf("You must pass required earlier stage(s) first.\n");
            return;
        }

        printf("Did you complete the training and pass the certification? (P/F): ");
        char result;
        scanf(" %c", &result);
        result = toupper(result);
        clearInput();

        if (result == 'P' || result == 'F') {
            trainingResults[stageIndex] = result;
            printf("Result recorded as '%c'.\n", result);
        } else {
            printf("Invalid input. Use 'P' or 'F'.\n");
        }
    }
}

void handleTrainingMenu() {
    char input[10];
    while (1) {
        displayTrainingMenu();
        printf("Select a training stage: ");
        fgets(input, sizeof(input), stdin);
        if (input[0] == '0' || toupper(input[0]) == 'Q') break;

        int option = atoi(input);
        if (option >= 1 && option <= TRAINING_COUNT) {
            evaluateStage(option - 1);
        } else {
            printf("Invalid stage. Choose between 1 and %d.\n", TRAINING_COUNT);
        }
    }
}

void handleMainMenu() {
    char input[10];
    while (1) {
        displayMainMenu();
        printf("Select a menu: ");
        fgets(input, sizeof(input), stdin);
        if (input[0] == '0' || toupper(input[0]) == 'Q') break;

        int option = atoi(input);
        switch (option) {
            case 1:
                printf("Audition Management - (functionality not implemented).\n");
                break;
            case 2:
                handleTrainingMenu();
                break;
            case 3:
                printf("Debut - (functionality not implemented).\n");
                break;
            default:
                printf("Invalid menu. Please select 1 to 3.\n");
        }
    }
}

int main() {
    handleMainMenu();
    printf("Program exited.\n");
    return 0;
}
