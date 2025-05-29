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

// ---------- Music Theory Structures ----------
typedef struct {
    char note;     // A-G
    int octave;    // 1-8
} MusicNote;

typedef struct {
    MusicNote root;
    MusicNote third;
    MusicNote fifth;
} Chord;

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

// ---------- My Voice Game ----------
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

    printf("\n--- GRBAS Voice Tone Scale ---\n");
    const char *labels[] = {"Grade", "Roughness", "Breathiness", "Asthenia", "Strain"};
    for (int i = 0; i < 5; i++) {
        upDownGameInt(labels[i], v.grbas[i]);
    }
}

// ---------- Music Theory ----------
int isValidNote(char note) {
    return (note >= 'A' && note <= 'G');
}

int isValidOctave(int octave) {
    return (octave >= 1 && octave <= 8);
}

int sameOctave(Chord *ch) {
    return ch->root.octave == ch->third.octave && ch->root.octave == ch->fifth.octave;
}

int uniqueNotes(Chord *ch) {
    return ch->root.note != ch->third.note &&
           ch->root.note != ch->fifth.note &&
           ch->third.note != ch->fifth.note;
}

int noteToIndex(char note) {
    return note - 'A'; // A=0, B=1, ..., G=6
}

const char* harmonyLevel(int interval13, int interval15) {
    if (interval13 <= 2 && interval15 >= 4) return "Good Harmony";
    if (interval13 <= 3 && interval15 >= 3) return "Moderate Harmony";
    return "Poor Harmony";
}

void learnMusicTheory() {
    Chord chord;

    printf("Enter root note (A-G): ");
    scanf(" %c", &chord.root.note);
    printf("Enter root octave (1-8): ");
    scanf("%d", &chord.root.octave);

    printf("Enter third note (A-G): ");
    scanf(" %c", &chord.third.note);
    printf("Enter third octave (1-8): ");
    scanf("%d", &chord.third.octave);

    printf("Enter fifth note (A-G): ");
    scanf(" %c", &chord.fifth.note);
    printf("Enter fifth octave (1-8): ");
    scanf("%d", &chord.fifth.octave);

    if (!isValidNote(chord.root.note) || !isValidNote(chord.third.note) || !isValidNote(chord.fifth.note) ||
        !isValidOctave(chord.root.octave) || !isValidOctave(chord.third.octave) || !isValidOctave(chord.fifth.octave)) {
        printf("Invalid note or octave input.\n");
        return;
    }

    printf("\nChord Info:\nRoot: %c%d  Third: %c%d  Fifth: %c%d\n",
           chord.root.note, chord.root.octave,
           chord.third.note, chord.third.octave,
           chord.fifth.note, chord.fifth.octave);

    if (!sameOctave(chord) || !uniqueNotes(chord)) {
        printf("❌ Invalid chord: Notes must be unique and in same octave.\n");
        return;
    }

    int rootIdx = noteToIndex(chord.root.note);
    int thirdIdx = noteToIndex(chord.third.note);
    int fifthIdx = noteToIndex(chord.fifth.note);

    int interval13 = abs(thirdIdx - rootIdx);
    int interval15 = abs(fifthIdx - rootIdx);

    printf("✅ Valid chord.\n🎵 Harmony Level: %s\n", harmonyLevel(interval13, interval15));
}

// ---------- Menu ----------
void stage4Menu() {
    int choice;
    while (1) {
        printf("\n[Stage 4 Menu] 1. Evaluate Vocal  2. Show Results  3. Find My Voice  4. Music Theory  0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: evalVocal(); break;
            case 2: printVocalInfo(); break;
            case 3: findMyVoice(); break;
            case 4: learnMusicTheory(); break;
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
