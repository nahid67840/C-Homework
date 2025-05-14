#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMBERS 10
#define FITNESS_TESTS 7
#define MAX_NAME_LENGTH 50
#define MAX_NICKNAME_LENGTH 50

typedef struct {
    char name[MAX_NAME_LENGTH];
    char nickname[MAX_NICKNAME_LENGTH];
} Member;

typedef struct {
    char nickname[MAX_NICKNAME_LENGTH];
    float health_scores[FITNESS_TESTS];
} HealthData;

Member milliways_members[MAX_MEMBERS];
HealthData health_scores[MAX_MEMBERS];

int member_count = 0;

void displayMainMenu();
void displayTrainingMenu();
void enterFitnessData();
void viewFitnessData();
void setHealth(int member_index);
void getHealth(int member_index);
void parseFitnessData(int member_index, char *input);
int findMemberIndexByNickname(char *nickname);

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
                enterFitnessData();
                break;
            case '3':
                viewFitnessData();
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
    printf("2. Enter Fitness Data\n");
    printf("3. View Fitness Data\n");
    printf("0. Exit\n");
    printf("Q. Quit\n");
}

void displayTrainingMenu() {
    printf("\n=== Training Menu ===\n");
    printf("1. Physical Strength & Knowledge\n");
    printf("0. Go Back\n");
}

void enterFitnessData() {
    char input[256];
    if (member_count < MAX_MEMBERS) {
        printf("\nEnter member name: ");
        fgets(milliways_members[member_count].name, MAX_NAME_LENGTH, stdin);
        milliways_members[member_count].name[strcspn(milliways_members[member_count].name, "\n")] = 0;  // Remove newline
        
        printf("Enter member nickname: ");
        fgets(milliways_members[member_count].nickname, MAX_NICKNAME_LENGTH, stdin);
        milliways_members[member_count].nickname[strcspn(milliways_members[member_count].nickname, "\n")] = 0;  // Remove newline
        
        printf("Enter fitness data for %s (%s):\n", milliways_members[member_count].name, milliways_members[member_count].nickname);
        printf("Please enter comma-separated values for the following tests:\n");
        printf("1-Mile Running Test (minutes), Speed Sprint (seconds), Push-ups (minutes), Squats (minutes), Arm Strength (Push-ups) (minutes), Swimming (minutes), Weightlifting (bodyweight multiplier)\n");
        
        fgets(input, 256, stdin);
        
        parseFitnessData(member_count, input);
        
        member_count++;
    } else {
        printf("Max member limit reached.\n");
    }
}

void parseFitnessData(int member_index, char *input) {
    char *token = strtok(input, ",");
    int i = 0;
    
    while (token != NULL && i < FITNESS_TESTS) {
        health_scores[member_index].health_scores[i] = atof(token);
        token = strtok(NULL, ",");
        i++;
    }
}

void viewFitnessData() {
    char nickname[MAX_NICKNAME_LENGTH];
    
    printf("\nEnter member nickname to view fitness data: ");
    fgets(nickname, MAX_NICKNAME_LENGTH, stdin);
    nickname[strcspn(nickname, "\n")] = 0;  // Remove newline
    
    int member_index = findMemberIndexByNickname(nickname);
    
    if (member_index != -1) {
        getHealth(member_index);
    } else {
        printf("Member with nickname '%s' not found.\n", nickname);
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

void getHealth(int member_index) {
    printf("\nFitness data for %s (%s):\n", milliways_members[member_index].name, milliways_members[member_index].nickname);
    printf("1-Mile Running Test: %.2f minutes\n", health_scores[member_index].health_scores[0]);
    printf("Speed Sprint: %.2f seconds\n", health_scores[member_index].health_scores[1]);
    printf("Push-ups: %.2f minutes\n", health_scores[member_index].health_scores[2]);
    printf("Squats: %.2f minutes\n", health_scores[member_index].health_scores[3]);
    printf("Arm Strength (Push-ups): %.2f minutes\n", health_scores[member_index].health_scores[4]);
    printf("Swimming: %.2f minutes\n", health_scores[member_index].health_scores[5]);
    printf("Weightlifting: %.2f\n", health_scores[member_index].health_scores[6]);
}
