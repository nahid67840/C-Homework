#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMBERS 10
#define MAX_NAME_LENGTH 50
#define MAX_NICKNAME_LENGTH 50
#define WEEK_DAYS 6
#define EXERCISE_TYPES 5

typedef struct {
    char name[MAX_NAME_LENGTH];
    char nickname[MAX_NICKNAME_LENGTH];
} Member;

typedef struct {
    char routine[WEEK_DAYS][3][MAX_NAME_LENGTH];
} ExerciseRoutine;

Member milliways_members[MAX_MEMBERS];
ExerciseRoutine member_routines[MAX_MEMBERS];
int member_count = 0;

void displayMainMenu();
void displayTrainingMenu();
void setExerciseRoutine();
void getExerciseRoutine();
void displayMemberList();
int findMemberIndexByNickname(char *nickname);

const char *exercise_types[EXERCISE_TYPES] = {
    "Cardio: Running", "Cardio: Cycling", "Cardio: Fast Walking",
    "Full-body Strength: Push-ups", "Full-body Strength: Squats",
    "Lower-body Strength: Leg Press", "Lower-body Strength: Leg Curl",
    "Upper-body Strength: Pull-ups", "Upper-body Strength: Chin-ups",
    "Core Strength: Plank", "Core Strength: Crunches"
};

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
                displayTrainingMenu();
                break;
            case '2':
                setExerciseRoutine();
                break;
            case '3':
                getExerciseRoutine();
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

void displayMainMenu() {
    printf("\n=== Main Menu ===\n");
    printf("1. Training\n");
    printf("2. Set Basic Workout Routine\n");
    printf("3. View Basic Workout Routine\n");
    printf("0. Exit\n");
    printf("Q. Quit\n");
}

void displayTrainingMenu() {
    printf("\n=== Training Menu ===\n");
    printf("1. Physical Strength & Knowledge\n");
    printf("0. Go Back\n");
}

void setExerciseRoutine() {
    char name[MAX_NAME_LENGTH];
    char nickname[MAX_NICKNAME_LENGTH];
    int member_index;

    printf("\nEnter member name: ");
    fgets(name, MAX_NAME_LENGTH, stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Enter member nickname: ");
    fgets(nickname, MAX_NICKNAME_LENGTH, stdin);
    nickname[strcspn(nickname, "\n")] = 0;

    member_index = findMemberIndexByNickname(nickname);

    if (member_index == -1) {
        printf("Member not found. Please add the member first.\n");
        return;
    }

    printf("\nSetting exercise routine for %s (%s):\n", name, nickname);

    for (int day = 0; day < WEEK_DAYS; day++) {
        printf("Enter exercise for day %d (choose from the following options):\n", day + 1);
        for (int i = 0; i < EXERCISE_TYPES; i++) {
            printf("%d. %s\n", i + 1, exercise_types[i]);
        }

        for (int exercise = 0; exercise < 3; exercise++) {
            printf("Enter exercise %d for day %d (1-%d): ", exercise + 1, day + 1, EXERCISE_TYPES);
            int choice;
            scanf("%d", &choice);
            getchar();

            if (choice < 1 || choice > EXERCISE_TYPES) {
                printf("Invalid choice, try again.\n");
                exercise--;
                continue;
            }
            strcpy(member_routines[member_index].routine[day][exercise], exercise_types[choice - 1]);
        }
    }

    printf("\nWorkout routine set successfully for %s (%s).\n", name, nickname);
}

void getExerciseRoutine() {
    char nickname[MAX_NICKNAME_LENGTH];
    int member_index;

    printf("\nEnter member nickname to view workout routine: ");
    fgets(nickname, MAX_NICKNAME_LENGTH, stdin);
    nickname[strcspn(nickname, "\n")] = 0;

    member_index = findMemberIndexByNickname(nickname);

    if (member_index == -1) {
        printf("Member with nickname '%s' not found.\n", nickname);
        return;
    }

    printf("\nWorkout routine for %s (%s):\n", milliways_members[member_index].name, milliways_members[member_index].nickname);
    for (int day = 0; day < WEEK_DAYS; day++) {
        printf("Day %d:\n", day + 1);
        for (int exercise = 0; exercise < 3; exercise++) {
            if (strlen(member_routines[member_index].routine[day][exercise]) > 0) {
                printf("%s\n", member_routines[member_index].routine[day][exercise]);
            }
        }
    }
}

int findMemberIndexByNickname(char *nickname) {
    for (int i = 0; i < member_count; i++) {
        if (strcmp(milliways_members[i].nickname, nickname) == 0) {
            return i;
        }
    }
    return -1;
}

void displayMemberList() {
    printf("\nMember List:\n");
    for (int i = 0; i < member_count; i++) {
        printf("%s (%s)\n", milliways_members[i].name, milliways_members[i].nickname);
    }
}
