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
#define DAYS_IN_WEEK 7
#define MEALS_PER_DAY 3

// ---------- Global Data ----------
const char *fitness_categories[FITNESS_TESTS] = {
    "1-Mile Run (min)", "Sprint (sec)", "Push-ups (min)", 
    "Squats (min)", "Arm Strength (min)", "Swimming (min)", 
    "Weightlifting (xBW)"
};

const char *weekdays[DAYS] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char *days[DAYS_IN_WEEK] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
const char *meals[MEALS_PER_DAY] = {"Breakfast", "Lunch", "Dinner"};

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
char member_routine[MAX_MEMBERS][DAYS][MAX_ROUTINE_LEN];

// DietPlan Struct
typedef struct {
    char allergies[100];
    char allowedFoods[200];
    char dietType[30];
    char meals[DAYS_IN_WEEK][MEALS_PER_DAY][100];
} DietPlan;

DietPlan diet_plans[MAX_MEMBERS];

// -------- Utility Functions --------
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

// -------- Fitness Functions --------
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
        for (int j = 0; j < FITNESS_TESTS; ++j)
            printf("- %s\n", fitness_categories[j]);

        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        parseFitnessData(input, health_scores[i]);
        ++health_count;
    }

    printf("Fitness data successfully recorded.\n");
}

void getHealth() {
    char choice, nickname[MAX_NAME_LEN];
    int found = -1;

    printf("\nView Options: A. All  B. By Member  C. Member + Category\nSelect: ");
    scanf(" %c", &choice);
    clearInput();

    if (toupper(choice) == 'A') {
        for (int i = 0; i < health_count; ++i) {
            printf("\n%s (%s)\n", milliways_members[i][0], health_nicknames[i]);
            for (int j = 0; j < FITNESS_TESTS; ++j)
                printf("%s: %.2f\n", fitness_categories[j], health_scores[i][j]);
        }
    } else {
        printf("Enter nickname: ");
        fgets(nickname, MAX_NAME_LEN, stdin);
        nickname[strcspn(nickname, "\n")] = '\0';

        for (int i = 0; i < health_count; ++i)
            if (strcmp(health_nicknames[i], nickname) == 0) found = i;

        if (found == -1) {
            printf("Member not found.\n"); return;
        }

        if (toupper(choice) == 'B') {
            for (int j = 0; j < FITNESS_TESTS; ++j)
                printf("%s: %.2f\n", fitness_categories[j], health_scores[found][j]);
        } else {
            int cat;
            for (int j = 0; j < FITNESS_TESTS; ++j)
                printf("%d. %s\n", j + 1, fitness_categories[j]);
            printf("Choose test number: ");
            scanf("%d", &cat); clearInput();
            if (cat >= 1 && cat <= FITNESS_TESTS)
                printf("%s: %.2f\n", fitness_categories[cat - 1], health_scores[found][cat - 1]);
        }
    }
}

// -------- Workout Routine --------
void setExerciseRoutine() {
    char name[MAX_NAME_LEN];
    int memberIndex = -1;

    displayMembers();
    printf("Enter member name: ");
    fgets(name, MAX_NAME_LEN, stdin);
    name[strcspn(name, "\n")] = '\0';

    for (int i = 0; i < MAX_MEMBERS; ++i)
        if (strcmp(milliways_members[i][0], name) == 0) memberIndex = i;

    if (memberIndex == -1) { printf("Member not found.\n"); return; }

    int core_used = 0;
    for (int d = 0; d < DAYS; ++d) {
        int has_cardio = 0, has_strength = 0;
        printf("\n[%s]\n", weekdays[d]);
        while (!has_cardio || !has_strength) {
            for (int e = 0; e < EXERCISE_COUNT; ++e)
                printf("%d. %s (%s / %s)\n", e+1, exercise_types[e][0], exercise_types[e][1], exercise_types[e][2]);

            int e1, e2;
            printf("Choose 2 exercise groups (1–5): ");
            scanf("%d %d", &e1, &e2); clearInput();

            if ((e1 < 1 || e1 > 5) || (e2 < 1 || e2 > 5)) continue;
            if ((e1 == 5 || e2 == 5) && core_used) {
                printf("Only one core session per week.\n"); continue;
            }

            has_cardio = (e1 == 1 || e2 == 1);
            has_strength = (e1 != 1 || e2 != 1);
            if (e1 == 5 || e2 == 5) core_used = 1;

            snprintf(member_routine[memberIndex][d], MAX_ROUTINE_LEN, "%s, %s", exercise_types[e1 - 1][1], exercise_types[e2 - 1][2]);
        }
    }

    printf("Workout routine set for %s.\n", milliways_members[memberIndex][0]);
}

void getExerciseRoutine() {
    char name[MAX_NAME_LEN];
    int idx = -1;
    displayMembers();
    printf("Enter member name: ");
    fgets(name, MAX_NAME_LEN, stdin); name[strcspn(name, "\n")] = '\0';

    for (int i = 0; i < MAX_MEMBERS; ++i)
        if (strcmp(milliways_members[i][0], name) == 0) idx = i;
    if (idx == -1) { printf("Not found.\n"); return; }

    printf("\nWorkout Routine for %s (%s):\n", milliways_members[idx][0], milliways_members[idx][1]);
    for (int d = 0; d < DAYS; ++d)
        printf("%s: %s\n", weekdays[d], member_routine[idx][d]);
}

// -------- Diet Plan --------
void setDietPlan() {
    char name[MAX_NAME_LEN]; int i = -1;
    displayMembers();
    printf("Enter member name for diet: ");
    fgets(name, MAX_NAME_LEN, stdin); name[strcspn(name, "\n")] = '\0';
    for (int m = 0; m < MAX_MEMBERS; ++m)
        if (strcmp(milliways_members[m][0], name) == 0) i = m;
    if (i == -1) { printf("Not found.\n"); return; }

    printf("Allergies: "); fgets(diet_plans[i].allergies, 100, stdin);
    diet_plans[i].allergies[strcspn(diet_plans[i].allergies, "\n")] = '\0';

    printf("Allowed Foods: "); fgets(diet_plans[i].allowedFoods, 200, stdin);
    diet_plans[i].allowedFoods[strcspn(diet_plans[i].allowedFoods, "\n")] = '\0';

    printf("Diet Type: "); fgets(diet_plans[i].dietType, 30, stdin);
    diet_plans[i].dietType[strcspn(diet_plans[i].dietType, "\n")] = '\0';

    for (int d = 0; d < DAYS_IN_WEEK; ++d) {
        printf("[%s]\n", days[d]);
        for (int m = 0; m < MEALS_PER_DAY; ++m) {
            printf("%s: ", meals[m]);
            fgets(diet_plans[i].meals[d][m], 100, stdin);
            diet_plans[i].meals[d][m][strcspn(diet_plans[i].meals[d][m], "\n")] = '\0';
        }
    }

    printf("Diet plan recorded.\n");
}

void getDietPlan() {
    char name[MAX_NAME_LEN]; int i = -1;
    displayMembers();
    printf("Enter member name to view diet: ");
    fgets(name, MAX_NAME_LEN, stdin); name[strcspn(name, "\n")] = '\0';
    for (int m = 0; m < MAX_MEMBERS; ++m)
        if (strcmp(milliways_members[m][0], name) == 0) i = m;
    if (i == -1) { printf("Not found.\n"); return; }

    printf("Diet Plan for %s (%s)\n", milliways_members[i][0], milliways_members[i][1]);
    printf("Allergies: %s\nAllowed: %s\nType: %s\n", diet_plans[i].allergies, diet_plans[i].allowedFoods, diet_plans[i].dietType);
    for (int d = 0; d < DAYS_IN_WEEK; ++d) {
        printf("[%s]\n", days[d]);
        for (int m = 0; m < MEALS_PER_DAY; ++m)
            printf("%s: %s\n", meals[m], diet_plans[i].meals[d][m]);
    }
}

// -------- Training Evaluation --------
void evaluateStage(int stageIndex) {
    if ((stageIndex == 1 && trainingResults[0] != 'P') ||
        (stageIndex > 1 && (trainingResults[0] != 'P' || trainingResults[1] != 'P'))) {
        printf("Previous stages not passed.\n"); return;
    }

    if (trainingResults[stageIndex] != ' ') {
        printf("Stage already evaluated.\n"); return;
    }

    char r;
    printf("Pass this stage? (P/F): ");
    scanf(" %c", &r); clearInput();
    if (toupper(r) == 'P' || toupper(r) == 'F') {
        trainingResults[stageIndex] = toupper(r);
        printf("Recorded.\n");
    } else {
        printf("Invalid.\n");
    }
}

// -------- Menus --------
void handleStage1Menu() {
    char input[10];
    while (1) {
        printf("\n[1. Physical Strength & Knowledge]\n");
        printf("A. Enter Fitness Data\nB. View Fitness Data\nC. Set Workout Routine\n");
        printf("D. View Workout Routine\nE. Set Diet Plan\nF. View Diet Plan\n0/Q to Back\nSelect: ");
        fgets(input, sizeof(input), stdin);
        switch (toupper(input[0])) {
            case 'A': setHealth(); break;
            case 'B': getHealth(); break;
            case 'C': setExerciseRoutine(); break;
            case 'D': getExerciseRoutine(); break;
            case 'E': setDietPlan(); break;
            case 'F': getDietPlan(); break;
            case '0': case 'Q': return;
            default: printf("Invalid.\n"); break;
        }
    }
}

void handleTrainingMenu() {
    char input[10];
    while (1) {
        printf("\n=== Training Menu ===\n");
        for (int i = 0; i < TRAINING_COUNT; ++i)
            printf("%d. %s [%c]\n", i + 1, trainingMenu[i], trainingResults[i]);
        printf("0/Q to back\nSelect: ");
        fgets(input, sizeof(input), stdin);
        if (input[0] == '0' || toupper(input[0]) == 'Q') return;
        int opt = atoi(input);
        if (opt >= 1 && opt <= 8) {
            if (opt == 1) handleStage1Menu();
            else evaluateStage(opt - 1);
        }
    }
}

void displayMainMenu() {
    printf("\n==== Main Menu ====\n1. Audition Management\n2. Training\n3. Debut\n0/Q to Exit\n");
}

void handleMainMenu() {
    char input[10];
    while (1) {
        displayMainMenu();
        printf("Select: ");
        fgets(input, sizeof(input), stdin);
        if (input[0] == '0' || toupper(input[0]) == 'Q') break;
        int opt = atoi(input);
        if (opt == 1 || opt == 3) printf("Not implemented.\n");
        else if (opt == 2) handleTrainingMenu();
    }
}

int main() {
    handleMainMenu();
    return 0;
}
