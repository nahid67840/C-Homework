#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TRAINEES 10
#define MAX_MENTORS 8
#define NAME_LEN 20

// Struct for mentor information
typedef struct {
    int id;
    char name[NAME_LEN];
    int mentee_num;
} Mentor;

// Trainee names (can be taken from previous course data)
char *trainees[MAX_TRAINEES] = {
    "Alex", "Brian", "Cody", "Dana", "Ella",
    "Finn", "Gina", "Hugo", "Ivy", "Jack"
};

// Arrays to store ASCII sum and ability values
int trainee_ascii[MAX_TRAINEES];
int trainee_ability[MAX_TRAINEES];

// Mentor list
Mentor mentors[MAX_MENTORS] = {
    {1, "Mentor1", -1},
    {2, "Mentor2", -1},
    {3, "Mentor3", -1},
    {4, "Mentor4", -1},
    {5, "Mentor5", -1},
    {6, "Mentor6", -1},
    {7, "Mentor7", -1},
    {8, "Mentor8", -1}
};

// Convert trainee name to ASCII sum
int parseIntMember(char *name) {
    int sum = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        sum += (int)name[i];
    }
    return sum;
}

// Generate random ability between 100 and 1000
int getRandomAbility() {
    return 100 + rand() % 901;
}

// Initialize trainee data
void initTrainees() {
    for (int i = 0; i < MAX_TRAINEES; i++) {
        trainee_ascii[i] = parseIntMember(trainees[i]);
        trainee_ability[i] = getRandomAbility();
    }
}

// Match trainees to mentors using (index % 8)
void matchMentoring() {
    printf("Trainee No.\tMentor ID\tMentor Name\n");
    for (int i = 0; i < MAX_TRAINEES; i++) {
        int mentor_index = i % MAX_MENTORS;
        mentors[mentor_index].mentee_num = i + 1; // Store the trainee number
        printf("%d\t\t%d\t\t%s\n", i + 1, mentors[mentor_index].id, mentors[mentor_index].name);
    }
}

// Main function
int main() {
    srand(time(NULL)); // Seed for random ability generation

    initTrainees();     // Initialize trainees
    matchMentoring();   // Perform mentor matching

    return 0;
}
