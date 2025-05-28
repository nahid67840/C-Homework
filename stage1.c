#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_MEMBERS 6
#define MAX_NAME_LEN 50
#define FITNESS_TESTS 7
#define TRAINING_COUNT 8
#define MAIN_MENU_COUNT 3
#define DAYS 6
#define EXERCISE_COUNT 5
#define MAX_ROUTINE_LEN 100

// ---------- Global Data ----------
const char *fitness_categories[FITNESS_TESTS] = {
    "1-Mile Run (min)", "Sprint (sec)", "Push-ups (min)", 
    "Squats (min)", "Arm Strength (min)", "Swimming (min)", 
    "Weightlifting (xBW)"
};

const char *weekdays[DAYS] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

const char *exercise_types[EXERCISE_COUNT][3] = {
    {"Cardio", "Running", "Cycling"},
    {"Full-body Strength", "Push-ups", "Squats"},
    {"Lower-body Strength", "Leg Press", "Leg Curl"},
    {"Upper-body Strength", "Pull-ups", "Chin-ups"},
    {"Core Strength", "Plank", "Crunches"}
};

char milliways_members[MAX_MEMBERS][2][MAX_NAME_LEN] = {
    {"Jiyeon Park", "Jiyeon"},
    {"Ethan Smith", "Ethan"},
    {"Helena Silva", "Helena"},
    {"Liam Wilson", "Liam"},
    {"Ava Chen", "Ava"},
    {"Noah Lee", "Noah"}
};

char health_nicknames[MAX_MEMBERS][MAX_NAME_LEN];
float health_scores[MAX_MEMBERS][FITNESS_TESTS];
int health_count = 0;

char trainingResults[TRAINING_COUNT] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

const char mainMenu[MAIN_MENU_COUNT][MAX_NAME_LEN] = {
    "I. Audition Management",
    "II. Training",
    "III. Debut"
};

const char trainingMenu[TRAINING_COUNT][MAX_NAME_LEN] = {
    "1. Physical Strength & Knowledge",
    "2. Self-Management & Teamwork",
    "3. Language & Pronunciation",
    "4. Vocal",
    "5. Dance",
    "6. Visual & Image",
    "7. Acting & Stage Performance",
    "8. Fan Communication"
};

char member_routine[MAX_MEMBERS][DAYS][MAX_ROUTINE_LEN];  // [member][day][routine string]

// ---------- Utility ----------
void clearInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void displayMembers() {
    printf("\n[ Milliways Members ]\n");
    for (int i = 0; i < MAX_MEMBERS; ++i) {
        printf("%d. %s (%s)\n", i + 1, milliways_members[i][0], milliways_members[i][1]);
    }
}

// ---------- Fitness Data ----------
void parseFitnessData(char *input, float *scores) {
    char *token = strtok(input, ",");
    int i = 0;
    while (token != NULL && i < FITNESS_TESTS) {
        scores[i++] = atof(token);
        token = strtok(NULL, ",");
    }
}

void setHealth() {
    char nickname[MAX_NAME_LEN];
    char input[256];
    printf("\n[Enter Fitness Data for Each Member]\n");

    for (int i = 0; i < MAX_MEMBERS; ++i) {
        printf("Nickname (%s): ", milliways_members[i][1]);
        fgets(nickname, sizeof(nickname), stdin);
        nickname[strcspn(nickname, "\n")] = '\0';
        strcpy(health_nicknames[i], nickname);

        printf("Enter 7 scores (comma-separated):\n");
        for (int j = 0; j < FITNESS_TESTS; ++j) {
            printf("- %s\n", fitness_categories[j]);
        }
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        parseFitnessData(input, health_scores[i]);
        ++health_count;
    }

    printf("Fitness data successfully recorded.\n");
}

void getHealth() {
    char choice;
    char nickname[MAX_NAME_LEN];
    int found = -1;

    printf("\n[B. View Fitness Data]\n");
    printf("View options:\n");
    printf("A. All data\nB. By member\nC. By member + category\nSelect: ");
    scanf(" %c", &choice);
    clearInput();

    if (toupper(choice) == 'A') {
        for (int i = 0; i < health_count; ++i) {
            printf("\nName: %s | Nickname: %s\n", milliways_members[i][0], health_nicknames[i]);
            for (int j = 0; j < FITNESS_TESTS; ++j) {
                printf("  %s: %.2f\n", fitness_categories[j], health_scores[i][j]);
            }
        }
    } else {
        printf("Enter nickname: ");
        fgets(nickname, sizeof(nickname), stdin);
        nickname[strcspn(nickname, "\n")] = '\0';

        for (int i = 0; i < health_count; ++i) {
            if (strcmp(health_nicknames[i], nickname) == 0) {
                found = i;
                break;
            }
        }

        if (found == -1) {
            printf("Member not found.\n");
            return;
        }

        printf("Name: %s | Nickname: %s\n", milliways_members[found][0], health_nicknames[found]);

        if (toupper(choice) == 'B') {
            for (int j = 0; j < FITNESS_TESTS; ++j) {
                printf("  %s: %.2f\n", fitness_categories[j], health_scores[found][j]);
            }
        } else if (toupper(choice) == 'C') {
            int cat;
            for (int j = 0; j < FITNESS_TESTS; ++j) {
                printf("%d. %s\n", j + 1, fitness_categories[j]);
            }
            printf("Select test number (1–7): ");
            scanf("%d", &cat);
            clearInput();
            if (cat >= 1 && cat <= FITNESS_TESTS) {
                printf("%s: %.2f\n", fitness_categories[cat - 1], health_scores[found][cat - 1]);
            } else {
                printf("Invalid test number.\n");
            }
        } else {
            printf("Invalid option.\n");
        }
    }
}

// ---------- Workout Routine ----------
void setExerciseRoutine() {
    char name[MAX_NAME_LEN];
    int memberIndex = -1;

    displayMembers();
    printf("Enter member name to set routine: ");
    fgets(name, MAX_NAME_LEN, stdin);
    name[strcspn(name, "\n")] = '\0';

    for (int i = 0; i < MAX_MEMBERS; ++i) {
        if (strcmp(milliways_members[i][0], name) == 0) {
            memberIndex = i;
            break;
        }
    }

    if (memberIndex == -1) {
        printf("Member not found.\n");
        return;
    }

    int core_used = 0;

    for (int d = 0; d < DAYS; ++d) {
        printf("\n--- %s ---\n", weekdays[d]);
        int has_cardio = 0, has_strength_or_core = 0;
        char daily_routine[MAX_ROUTINE_LEN] = "";

        while (!has_cardio || !has_strength_or_core) {
            printf("Select at least one cardio and one strength/core exercise.\n");

            for (int e = 0; e < EXERCISE_COUNT; ++e) {
                printf("%d. %s: %s, %s\n", e + 1, exercise_types[e][0], exercise_types[e][1], exercise_types[e][2]);
            }

            int e1, e2;
            printf("Enter two exercise group numbers (1–5): ");
            scanf("%d %d", &e1, &e2);
            clearInput();

            if (e1 < 1 || e1 > 5 || e2 < 1 || e2 > 5) {
                printf("Invalid selection. Try again.\n");
                continue;
            }

            if ((e1 == 5 || e2 == 5) && core_used) {
                printf("Core exercises already used this week. Choose another.\n");
                continue;
            }

            has_cardio = (e1 == 1 || e2 == 1);
            has_strength_or_core = (e1 != 1 || e2 != 1);

            if ((e1 == 5 || e2 == 5)) core_used = 1;

            snprintf(daily_routine, MAX_ROUTINE_LEN, "%s, %s", exercise_types[e1 - 1][1], exercise_types[e2 - 1][2]);
            strncpy(member_routine[memberIndex][d], daily_routine, MAX_ROUTINE_LEN);
        }

        printf("Routine for %s set to: %s\n", weekdays[d], member_routine[memberIndex][d]);
    }

    printf("\nWorkout routine for %s successfully recorded.\n", milliways_members[memberIndex][0]);
}

void getExerciseRoutine() {
    char name[MAX_NAME_LEN];
    int memberIndex = -1;

    displayMembers();
    printf("Enter member name to view routine: ");
    fgets(name, MAX_NAME_LEN, stdin);
    name[strcspn(name, "\n")] = '\0';

    for (int i = 0; i < MAX_MEMBERS; ++i) {
        if (strcmp(milliways_members[i][0], name) == 0) {
            memberIndex = i;
            break;
        }
    }

    if (memberIndex == -1) {
        printf("Member not found.\n");
        return;
    }

    printf("\nWorkout Routine for %s (%s):\n", 
        milliways_members[memberIndex][0], milliways_members[memberIndex][1]);

    for (int d = 0; d < DAYS; ++d) {
        printf("%s: %s\n", weekdays[d], member_routine[memberIndex][d][0] ? member_routine[memberIndex][d] : "Not set");
    }
}

// ---------- Training Stage Evaluation ----------
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

// ---------- Menus ----------
void handleStage1Menu() {
    char input[10];
    while (1) {
        printf("\n[1. Physical Strength & Knowledge]\n");
        printf("A. Enter Fitness Data\n");
        printf("B. View Fitness Data\n");
        printf("C. Set Basic Workout Routine\n");
        printf("D. View Basic Workout Routine\n");
        printf("0 / Q to go back\nSelect: ");
        fgets(input, sizeof(input), stdin);

        switch (toupper(input[0])) {
            case 'A': setHealth(); break;
            case 'B': getHealth(); break;
            case 'C': setExerciseRoutine(); break;
            case 'D': getExerciseRoutine(); break;
            case '0':
            case 'Q': return;
            default: printf("Invalid selection.\n"); break;
        }
    }
}

void handleTrainingMenu() {
    char input[10];
    while (1) {
        printf("\n===== Training Stage Menu =====\n");
        for (int i = 0; i < TRAINING_COUNT; ++i) {
            printf("%d. %s [%c]\n", i + 1, trainingMenu[i], trainingResults[i]);
        }
        printf("0 / Q / q to go back\nSelect a training stage: ");
        fgets(input, sizeof(input), stdin);
        if (input[0] == '0' || toupper(input[0]) == 'Q') break;

        int option = atoi(input);
        if (option >= 1 && option <= TRAINING_COUNT) {
            if (option == 1) {
                handleStage1Menu();
            } else {
                evaluateStage(option - 1);
            }
        } else {
            printf("Invalid stage.\n");
        }
    }
}

void displayMainMenu() {
    printf("\n========= Main Menu =========\n");
    for (int i = 0; i < MAIN_MENU_COUNT; ++i) {
        printf("%d. %s\n", i + 1, mainMenu[i]);
    }
    printf("0 / Q / q to exit\n");
    printf("=============================\n");
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
            case 1: printf("Audition Management - Not implemented.\n"); break;
            case 2: handleTrainingMenu(); break;
            case 3: printf("Debut - Not implemented.\n"); break;
            default: printf("Invalid menu option.\n");
        }
    }
}

int main() {
    handleMainMenu();
    printf("Program exited.\n");
    return 0;
}
