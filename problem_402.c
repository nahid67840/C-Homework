#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 50
#define MAX_QUESTIONS 5
#define MAX_ATTEMPTS 3

// Hangul character structure
typedef struct {
    char hangul[MAX_LEN];
    char eng_alpha[MAX_LEN];
    char eng_pron[MAX_LEN];
    char type; // 'C' for consonant, 'V' for vowel
} HangulChar;

// Flashcard game result structure
typedef struct {
    char name[MAX_LEN];
    char challenge_type[3];
    int total_score;
    int attempt_count;
} FlashcardResult;

HangulChar hangul_chars[] = {
    {"ㄱ", "g", "giyeok", 'C'},
    {"ㄴ", "n", "nieun", 'C'},
    {"ㄷ", "d", "digeut", 'C'},
    {"ㄹ", "r", "rieul", 'C'},
    {"ㅁ", "m", "mieum", 'C'},
    {"ㅂ", "b", "bieup", 'C'},
    {"ㅅ", "s", "siot", 'C'},
    {"ㅇ", "", "ieung", 'C'},
    {"ㅈ", "j", "jieut", 'C'},
    {"ㅊ", "ch", "chieut", 'C'},
    {"ㅋ", "k", "kieuk", 'C'},
    {"ㅌ", "t", "tieut", 'C'},
    {"ㅍ", "p", "pieup", 'C'},
    {"ㅎ", "h", "hieut", 'C'},
    {"ㅏ", "a", "a", 'V'},
    {"ㅐ", "ae", "ae", 'V'},
    {"ㅑ", "ya", "ya", 'V'},
    {"ㅒ", "yae", "yae", 'V'},
    {"ㅓ", "eo", "eo", 'V'},
    {"ㅔ", "e", "e", 'V'}
};

FlashcardResult results[100];
int result_count = 0;

void shuffle(int *arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void playKor2EngGame(char *name, char type) {
    int indexes[20];
    int filtered[20];
    int k = 0;
    for (int i = 0; i < 20; i++) if (hangul_chars[i].type == type) filtered[k++] = i;
    shuffle(filtered, k);

    int score = 0;
    for (int i = 0; i < MAX_QUESTIONS; i++) {
        int idx = filtered[i];
        char answer_alpha[MAX_LEN], answer_pron[MAX_LEN];
        printf("Q%d. What is the English alphabet and pronunciation for %s?\n", i + 1, hangul_chars[idx].hangul);
        printf("Format: alphabet,pronunciation\nAnswer: ");
        scanf("%[^,],%s", answer_alpha, answer_pron);

        if (strcmp(answer_alpha, hangul_chars[idx].eng_alpha) == 0 && strcmp(answer_pron, hangul_chars[idx].eng_pron) == 0) {
            printf("Correct!\n");
            score += 10;
        } else {
            printf("Wrong! Correct: %s,%s\n", hangul_chars[idx].eng_alpha, hangul_chars[idx].eng_pron);
        }
    }
    for (int i = 0; i < result_count; i++) {
        if (strcmp(results[i].name, name) == 0 && strcmp(results[i].challenge_type, "KE") == 0) {
            results[i].total_score += score;
            results[i].attempt_count++;
            return;
        }
    }
    strcpy(results[result_count].name, name);
    strcpy(results[result_count].challenge_type, "KE");
    results[result_count].total_score = score;
    results[result_count].attempt_count = 1;
    result_count++;
}

void playEng2KorGame(char *name, char type) {
    int indexes[20];
    int filtered[20];
    int k = 0;
    for (int i = 0; i < 20; i++) if (hangul_chars[i].type == type) filtered[k++] = i;
    shuffle(filtered, k);

    int score = 0;
    for (int i = 0; i < MAX_QUESTIONS; i++) {
        int idx = filtered[i];
        char answer_han[MAX_LEN];
        printf("Q%d. What is the Hangul for %s (%s)?\nAnswer: ", i + 1, hangul_chars[idx].eng_alpha, hangul_chars[idx].eng_pron);
        scanf("%s", answer_han);

        if (strcmp(answer_han, hangul_chars[idx].hangul) == 0) {
            printf("Correct!\n");
            score += 10;
        } else {
            printf("Wrong! Correct: %s\n", hangul_chars[idx].hangul);
        }
    }
    for (int i = 0; i < result_count; i++) {
        if (strcmp(results[i].name, name) == 0 && strcmp(results[i].challenge_type, "EK") == 0) {
            results[i].total_score += score;
            results[i].attempt_count++;
            return;
        }
    }
    strcpy(results[result_count].name, name);
    strcpy(results[result_count].challenge_type, "EK");
    results[result_count].total_score = score;
    results[result_count].attempt_count = 1;
    result_count++;
}

void showResult() {
    printf("\nResults:\n");
    for (int i = 0; i < result_count; i++) {
        double avg = results[i].total_score / (double)results[i].attempt_count;
        printf("%s [%s]: Avg Score = %.2f, Attempts = %d\n",
               results[i].name, results[i].challenge_type, avg, results[i].attempt_count);
    }
}

void learnHangul() {
    char name[MAX_LEN];
    int option;
    srand(time(NULL));

    while (1) {
        printf("\nEnter your name (or type exit to quit): ");
        scanf("%s", name);
        if (strcmp(name, "exit") == 0) break;

        printf("1. Hangul to English\n2. English to Hangul\n3. View Results\nSelect an option (0 to return): ");
        scanf("%d", &option);

        if (option == 0) break;
        else if (option == 1) {
            int sub;
            printf("1. Consonants\n2. Vowels\nSelect: ");
            scanf("%d", &sub);
            playKor2EngGame(name, sub == 1 ? 'C' : 'V');
        }
        else if (option == 2) {
            int sub;
            printf("1. Consonants\n2. Vowels\nSelect: ");
            scanf("%d", &sub);
            playEng2KorGame(name, sub == 1 ? 'C' : 'V');
        }
        else if (option == 3) {
            showResult();
        }
    }
}

int main() {
    learnHangul();
    return 0;
}
