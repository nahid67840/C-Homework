#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TRAINEES 100
#define MAX_NAME_LEN 50
#define MAX_FEEDBACK_LEN 51  // 50 characters + null terminator

typedef struct {
    char name[MAX_NAME_LEN];
    int vocalRange; // max MIDI value
    double vocalStability;
    char feedback[MAX_FEEDBACK_LEN];
} VocalEval;

VocalEval trainees[MAX_TRAINEES];
int traineeCount = 0;

// Function pointer typedefs
typedef double (*StabilityFunc)(int*, int);
typedef void (*FeedbackFunc)(char*);

// Function to generate unique random MIDI values
void generateMIDINotes(int *arr, int count, int rangeLimit) {
    int used[128] = {0};
    int generated = 0;
    while (generated < count) {
        int val = rand() % (rangeLimit + 1);
        if (!used[val]) {
            arr[generated++] = val;
            used[val] = 1;
        }
    }
}

// Calculate vocal stability
double calculateStability(int *arr, int len) {
    int sum = 0;
    for (int i = 1; i < len; ++i) {
        sum += abs(arr[i] - arr[i - 1]);
    }
    return len > 1 ? (double)sum / (len - 1) : 0.0;
}

// Provide feedback
void provideFeedback(char *feedback) {
    printf("Enter feedback (max 50 characters): ");
    fgets(feedback, MAX_FEEDBACK_LEN, stdin);
    feedback[strcspn(feedback, "\n")] = 0; // Remove newline
}

// Measure function
void measure(VocalEval *eval, StabilityFunc sFunc) {
    int noteCount;
    printf("Enter vocal range (0-127) and number of MIDI values (6-12): ");
    scanf("%d %d", &eval->vocalRange, &noteCount);
    getchar(); // consume newline

    if (noteCount < 6 || noteCount > 12 || noteCount > eval->vocalRange + 1) {
        printf("Invalid input. Please ensure note count is 6-12 and <= vocal range.\n");
        return;
    }

    int notes[12];
    generateMIDINotes(notes, noteCount, eval->vocalRange);
    eval->vocalStability = sFunc(notes, noteCount);
}

// Evaluate vocal info
void evalVocal() {
    if (traineeCount >= MAX_TRAINEES) {
        printf("Trainee limit reached.\n");
        return;
    }

    VocalEval *eval = &trainees[traineeCount];
    printf("Enter trainee name: ");
    fgets(eval->name, MAX_NAME_LEN, stdin);
    eval->name[strcspn(eval->name, "\n")] = 0;

    measure(eval, calculateStability);
    provideFeedback(eval->feedback);
    traineeCount++;
}

// Comparison function for qsort
int compareStability(const void *a, const void *b) {
    double stabA = ((VocalEval*)a)->vocalStability;
    double stabB = ((VocalEval*)b)->vocalStability;
    return (stabA > stabB) - (stabA < stabB);
}

// Print sorted vocal info
void printVocalInfo() {
    qsort(trainees, traineeCount, sizeof(VocalEval), compareStability);
    printf("\n--- Trainee Vocal Evaluations ---\n");
    for (int i = 0; i < traineeCount; ++i) {
        printf("Name: %s | Stability: %.2f | Feedback: %s\n",
               trainees[i].name, trainees[i].vocalStability, trainees[i].feedback);
    }
    printf("----------------------------------\n");
}

// Sample menu system (extend from Course 2 problem)
void showMenu() {
    int choice;
    srand((unsigned int)time(NULL));
    while (1) {
        printf("\n[II. Training > 4. Vocal Training > A. Vocal Evaluation]\n");
        printf("1. Evaluate Vocal\n");
        printf("2. Show All Evaluations\n");
        printf("0. Exit\n");
        printf("Select: ");
        scanf("%d", &choice);
        getchar(); // consume newline
        switch (choice) {
            case 1: evalVocal(); break;
            case 2: printVocalInfo(); break;
            case 0: return;
            default: printf("Invalid option. Try again.\n");
        }
    }
}

// Entry point
int main() {
    showMenu();
    return 0;
}
