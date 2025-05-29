#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_NAME 30
#define MAX_FEEDBACK 51
#define MAX_TRAINEES 100
#define MAX_MIDI_VALUES 12

// ---------- Vocal Evaluation Structures ----------
typedef struct {
    char name[MAX_NAME];
    int vocalRange;
    float vocalStability;
    char feedback[MAX_FEEDBACK];
    int midiValues[MAX_MIDI_VALUES];
    int midiCount;
} VocalEvaluation;

VocalEvaluation trainees[MAX_TRAINEES];
int traineeCount = 0;

// ---------- My Voice Structures ----------
typedef struct {
    char name[MAX_NAME];
    char gender; // 'M' or 'F'
    float frequency;
    float amplitude;
    float jitter;
    int grbas[5]; // G, R, B, A, S
} MyVoice;

typedef struct {
    float freq_min, freq_max;
    float amp_min, amp_max;
    float jitter_min, jitter_max;
} GenderRange;

GenderRange maleRange = {85.0, 180.0, 2.5, 3.6, 0.2, 0.6};
GenderRange femaleRange = {165.0, 255.0, 3.5, 4.7, 0.3, 1.0};

// ---------- Vocal Evaluation Functions ----------
float calculateStability(int *arr, int n) {
    int sumDiff = 0;
    for (int i = 0; i < n - 1; i++) {
        sumDiff += abs(arr[i + 1] - arr[i]);
    }
    return (float)sumDiff / (n - 1);
}

void generateUniqueMIDI(int *arr, int count, int maxRange) {
    int used[128] = {0};
    int val, i = 0;
    while (i < count) {
        val = rand() % (maxRange + 1);
        if (!used[val]) {
            used[val] = 1;
            arr[i++] = val;
        }
    }
}

void evalVocal() {
    if (traineeCount >= MAX_TRAINEES) {
        printf("Maximum number of trainees reached.\n");
        return;
    }

    VocalEvaluation v;
    printf("Enter trainee name: ");
    scanf("%s", v.name);

    printf("Enter vocal range (0-127): ");
    scanf("%d", &v.vocalRange);

    printf("Enter number of MIDI values (6 to 12): ");
    scanf("%d", &v.midiCount);
    if (v.midiCount < 6 || v.midiCount > 12 || v.midiCount > v.vocalRange + 1) {
        printf("Invalid input. Aborting.\n");
        return;
    }

    srand((unsigned int)v.vocalRange);
    generateUniqueMIDI(v.midiValues, v.midiCount, v.vocalRange);
    v.vocalStability = calculateStability(v.midiValues, v.midiCount);

    printf("Enter feedback (max 50 chars): ");
    scanf(" %[^\n]", v.feedback);

    trainees[traineeCount++] = v;
}

int compareStability(const void *a, const void *b) {
    float diff = ((VocalEvaluation *)a)->vocalStability - ((VocalEvaluation *)b)->vocalStability;
    return (diff > 0) - (diff < 0);
}

void printVocalInfo() {
    if (traineeCount == 0) {
        printf("No vocal evaluations available.\n");
        return;
    }

    qsort(trainees, traineeCount, sizeof(VocalEvaluation), compareStability);

    printf("\n--- Vocal Evaluation Results ---\n");
    for (int i = 0; i < traineeCount; i++) {
        printf("Name: %s | Stability: %.2f | Feedback: %s\n",
               trainees[i].name, trainees[i].vocalStability, trainees[i].feedback);
    }
}

// ---------- My Voice Game Functions ----------
float randomInRange(float min, float max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

void getMyVoiceFromHeaven(MyVoice *v) {
    GenderRange range = (v->gender == 'M') ? maleRange : femaleRange;
    v->frequency = randomInRange(range.freq_min, range.freq_max);
    v->amplitude = randomInRange(range.amp_min, range.amp_max);
    v->jitter = randomInRange(range.jitter_min, range.jitter_max);
    for (int i = 0; i < 5; i++) v->grbas[i] = rand() % 4;
}

int upDownGameFloat(const char *label, float target, float min, float max) {
    float guess;
    int tries = 3;
    while (tries--) {
        printf("Guess %s (%.1f ~ %.1f): ", label, min, max);
        scanf("%f", &guess);
        if (fabs(guess - target) < 0.01) {
            printf("Correct!\n");
            return 1;
        }
        printf("%s!\n", (guess < target) ? "Up" : "Down");
    }
    printf("Answer was: %.2f\n", target);
    return 0;
}

int upDownGameInt(const char *label, int target) {
    int guess;
    int tries = 3;
    while (tries--) {
        printf("Guess %s (0 ~ 3): ", label);
        scanf("%d", &guess);
        if (guess == target) {
            printf("Correct!\n");
            return 1;
        }
        printf("%s!\n", (guess < target) ? "Up" : "Down");
    }
    printf("Answer was: %d\n", target);
    return 0;
}

void findMyVoice() {
    MyVoice v;
    printf("Enter your nickname: ");
    scanf("%s", v.name);

    printf("Enter gender (M/F): ");
    scanf(" %c", &v.gender);
    if (v.gender != 'M' && v.gender != 'F') {
        printf("Invalid gender.\n");
        return;
    }

    srand((unsigned int)time(NULL));
    getMyVoiceFromHeaven(&v);

    GenderRange range = (v.gender == 'M') ? maleRange : femaleRange;

    printf("\nLet's discover your voice!\n");

    int f = upDownGameFloat("Frequency", v.frequency, range.freq_min, range.freq_max);
    int a = upDownGameFloat("Amplitude", v.amplitude, range.amp_min, range.amp_max);
    int j = upDownGameFloat("Jitter", v.jitter, range.jitter_min, range.jitter_max);

    if (f && a && j) {
        printf("\n🎉 Congratulations! You found your voice!\n");
    } else {
        printf("\n😅 Try again next time to discover your voice!\n");
    }

    // Bonus: GRBAS voice tone scale game
    printf("\n--- GRBAS Voice Tone Scale ---\n");
    const char *labels[] = {"Grade", "Roughness", "Breathiness", "Asthenia", "Strain"};
    for (int i = 0; i < 5; i++) {
        upDownGameInt(labels[i], v.grbas[i]);
    }
}

// ---------- Menu ----------
void stage4Menu() {
    int choice;
    while (1) {
        printf("\n[Stage 4 Menu] 1. Evaluate Vocal  2. Show Results  3. Find My Voice  0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: evalVocal(); break;
            case 2: printVocalInfo(); break;
            case 3: findMyVoice(); break;
            case 0: return;
            default: printf("Invalid option.\n");
        }
    }
}

// ---------- Main ----------
int main() {
    stage4Menu();
    return 0;
}
