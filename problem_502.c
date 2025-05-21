// stage4.c
// Author: Md Nahid Hasan
// Description: Magrathea - Vocal Training (My Voice + GRBAS Scale)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 30

// === [1] Structure Definitions ===
typedef struct {
    char name[MAX_NAME];
    float frequency;
    float amplitude;
    float jitter;
    int grbas[5]; // G, R, B, A, S
} VoiceInfo;

typedef enum { MALE, FEMALE } Gender;

typedef struct {
    char nickname[MAX_NAME];
    Gender gender;
} Member;

// === [2] Utility Function: Random Float Generator ===
float randomInRange(float min, float max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

// === [3] Random Voice Generator Based on Gender ===
void getMyVoiceFromHeaven(VoiceInfo *v, Gender g) {
    if (g == MALE) {
        v->frequency = randomInRange(85, 180);
        v->amplitude = randomInRange(2.5, 3.6);
        v->jitter = randomInRange(0.2, 0.6);
    } else {
        v->frequency = randomInRange(165, 255);
        v->amplitude = randomInRange(3.5, 4.7);
        v->jitter = randomInRange(0.3, 1.0);
    }

    for (int i = 0; i < 5; i++)
        v->grbas[i] = rand() % 4; // GRBAS values: 0-3
}

// === [4] Float Up-Down Guessing Game ===
int upDownGameFloat(float answer, float min, float max) {
    float guess;
    for (int i = 0; i < 3; i++) {
        printf("Enter your guess (%.1f ~ %.1f): ", min, max);
        scanf("%f", &guess);
        if (guess == answer) {
            printf("✅ Correct!\n");
            return 1;
        } else if (guess < answer) {
            printf("🔼 Up!\n");
        } else {
            printf("🔽 Down!\n");
        }
    }
    printf("❌ The correct value was: %.2f\n", answer);
    return 0;
}

// === [5] Integer Up-Down Guessing Game for GRBAS ===
int guessGRBAS(const char *label, int correctValue) {
    int guess;
    for (int i = 0; i < 3; i++) {
        printf("Guess %s (0-3): ", label);
        scanf("%d", &guess);
        if (guess == correctValue) {
            printf("✅ Correct!\n");
            return 1;
        } else if (guess < correctValue) {
            printf("🔼 Up!\n");
        } else {
            printf("🔽 Down!\n");
        }
    }
    printf("❌ The correct %s value was: %d\n", label, correctValue);
    return 0;
}

// === [6] Main Feature Function: Find My Voice ===
void findMyVoice() {
    Member members[] = {
        {"Ariel", FEMALE},
        {"Simba", MALE},
        {"Belle", FEMALE},
        {"Aladdin", MALE}
    };

    char input[MAX_NAME];
    printf("Enter your nickname: ");
    scanf("%s", input);

    Member *m = NULL;
    for (int i = 0; i < 4; i++) {
        if (strcmp(input, members[i].nickname) == 0) {
            m = &members[i];
            break;
        }
    }

    if (m == NULL) {
        printf("❌ Member not found.\n");
        return;
    }

    VoiceInfo voice;
    strcpy(voice.name, m->nickname);
    getMyVoiceFromHeaven(&voice, m->gender);

    printf("\n🎙️ Guess your Frequency:\n");
    int f_found = upDownGameFloat(
        voice.frequency,
        m->gender == MALE ? 85 : 165,
        m->gender == MALE ? 180 : 255
    );

    printf("\n📢 Guess your Amplitude:\n");
    int a_found = upDownGameFloat(
        voice.amplitude,
        m->gender == MALE ? 2.5 : 3.6,
        m->gender == MALE ? 3.6 : 4.7
    );

    printf("\n💓 Guess your Voice Pulse (Jitter):\n");
    int j_found = upDownGameFloat(
        voice.jitter,
        m->gender == MALE ? 0.2 : 0.3,
        m->gender == MALE ? 0.6 : 1.0
    );

    printf("\n🎵 Bonus: GRBAS Scale Evaluation 🎵\n");
    const char *labels[] = {"Grade", "Roughness", "Breathiness", "Asthenia", "Strain"};
    for (int i = 0; i < 5; i++) {
        guessGRBAS(labels[i], voice.grbas[i]);
    }

    if (f_found && a_found && j_found)
        printf("\n🎉 Congratulations %s, you've found your voice!\n", voice.name);
    else
        printf("\n🔁 Try again to find your perfect voice, %s.\n", voice.name);
}

// === [7] Main Menu System Integration ===
int main() {
    srand((unsigned int)time(NULL));
    int choice;
    do {
        printf("\n===========================\n");
        printf("[II. Training > 4. Vocal Training]\n");
        printf("1. Back\n");
        printf("2. B. My Voice\n");
        printf("===========================\n");
        printf("Select: ");
        scanf("%d", &choice);

        if (choice == 2) {
            findMyVoice();
        }
    } while (choice != 1);

    printf("👋 Exiting vocal training...\n");
    return 0;
}
