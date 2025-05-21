#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_MEMBERS 10
#define MAX_RESPONSES 3
#define MAX_TRAUMA_LEN 100
#define MAX_RESPONSE_LEN 101

// Member struct
typedef struct {
    char nickname[20];
    int age;
    char trauma[MAX_TRAUMA_LEN];
    int has_trauma;
} Member;

// Counseling Question struct
typedef struct {
    int id;
    char question[100];
} Question;

// Counseling Response struct
typedef struct {
    char nickname[20];
    char question[100];
    char response[MAX_RESPONSE_LEN];
} Response;

// Global arrays
Member members[MAX_MEMBERS] = {
    {"Alex", 20, "", 0}, {"Brian", 21, "", 0}, {"Cody", 19, "", 0},
    {"Dana", 22, "", 0}, {"Ella", 20, "", 0}, {"Finn", 21, "", 0},
    {"Gina", 23, "", 0}, {"Hugo", 24, "", 0}, {"Ivy", 22, "", 0}, {"Jack", 20, "", 0}
};

const Question questions[5] = {
    {1, "In what situations have you experienced this trauma?"},
    {2, "How has this situation affected your daily life and emotions?"},
    {3, "How have you tried to overcome this trauma?"},
    {4, "What emotions do you associate with this trauma?"},
    {5, "What kind of support do you think is necessary to overcome this trauma?"}
};

Response counseling[MAX_MEMBERS * MAX_RESPONSES];
int counseling_count = 0;

void inputTrauma() {
    char nickname[20];
    int found, i;
    do {
        printf("Enter nickname (or type 'exit' to go back): ");
        scanf("%s", nickname);
        if (strcmp(nickname, "exit") == 0) return;

        found = 0;
        for (i = 0; i < MAX_MEMBERS; i++) {
            if (strcmp(members[i].nickname, nickname) == 0) {
                found = 1;
                break;
            }
        }

        found ? printf("Enter trauma description: ") : printf("Nickname not found. Try again.\n");

        if (found) {
            getchar();
            fgets(members[i].trauma, MAX_TRAUMA_LEN, stdin);
            members[i].trauma[strcspn(members[i].trauma, "\n")] = 0;
            members[i].has_trauma = 1;
        }

    } while (!found);
}

void startCounseling() {
    char nickname[20];
    int i, j, found = 0;
    int selected_questions[3] = {-1, -1, -1};

    printf("Members with trauma:\n");
    for (i = 0; i < MAX_MEMBERS; i++) {
        if (members[i].has_trauma) {
            printf("- %s\n", members[i].nickname);
        }
    }

    printf("Enter nickname for counseling: ");
    scanf("%s", nickname);
    for (i = 0; i < MAX_MEMBERS; i++) {
        if (strcmp(members[i].nickname, nickname) == 0 && members[i].has_trauma) {
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Invalid nickname or no trauma data.\n");
        return;
    }

    // Randomly select 3 unique questions
    srand((unsigned int)time(NULL));
    for (j = 0; j < 3; ) {
        int q = rand() % 5;
        int unique = 1;
        for (int k = 0; k < j; k++) {
            if (selected_questions[k] == q) unique = 0;
        }
        if (unique) selected_questions[j++] = q;
    }

    getchar();
    for (j = 0; j < 3; j++) {
        Response r;
        strcpy(r.nickname, nickname);
        strcpy(r.question, questions[selected_questions[j]].question);
        do {
            printf("Q: %s\n> ", r.question);
            fgets(r.response, MAX_RESPONSE_LEN, stdin);
            r.response[strcspn(r.response, "\n")] = 0;
        } while (strlen(r.response) == 0 || strlen(r.response) > 100);
        counseling[counseling_count++] = r;
    }

    printf("Counseling session completed.\n");
}

void viewCounseling() {
    char nickname[20];
    int i;
    printf("Enter nickname to view counseling record: ");
    scanf("%s", nickname);

    for (i = 0; i < MAX_MEMBERS; i++) {
        if (strcmp(members[i].nickname, nickname) == 0 && members[i].has_trauma) {
            printf("Nickname: %s\nTrauma: %s\n", members[i].nickname, members[i].trauma);
            for (int j = 0; j < counseling_count; j++) {
                if (strcmp(counseling[j].nickname, nickname) == 0) {
                    printf("Q: %s\nA: %s\n", counseling[j].question, counseling[j].response);
                }
            }
            return;
        }
    }
    printf("No counseling record found for %s.\n", nickname);
}

void overcomeTrauma() {
    char choice;
    do {
        printf("\n[Trauma Management]\n");
        printf("A. Input Trauma\nB. Start Counseling\nC. View Counseling\nX. Exit\n> ");
        scanf(" %c", &choice);
        choice = toupper(choice);

        switch (choice) {
            case 'A': inputTrauma(); break;
            case 'B': startCounseling(); break;
            case 'C': viewCounseling(); break;
            case 'X': printf("Exiting Trauma Management...\n"); break;
            default: printf("Invalid option.\n");
        }
    } while (choice != 'X');
}

int main() {
    overcomeTrauma();
    return 0;
}
