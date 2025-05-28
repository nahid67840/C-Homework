#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORDS 10
#define MAX_TRAINEES 10
#define MAX_SENTENCES 10
#define MAX_NAME 30

typedef struct {
    char eng[100];
    char kor[200];
} SentencePair;

typedef struct {
    char nickname[MAX_NAME];
    int score;
} GrammarScore;

SentencePair sentencePairs[MAX_SENTENCES] = {
    {"I love Korean food.", "나는 한국 음식을 사랑해."},
    {"She is reading a book.", "그녀는 책을 읽고 있다."},
    {"We go to school every day.", "우리는 매일 학교에 간다."},
    {"They play soccer on Sundays.", "그들은 일요일마다 축구를 한다."},
    {"He is a good singer.", "그는 좋은 가수이다."},
    {"Do you like k-pop?", "너는 케이팝을 좋아하니?"},
    {"The weather is very nice.", "날씨가 매우 좋다."},
    {"I want to travel to Korea.", "나는 한국에 여행 가고 싶다."},
    {"She dances very well.", "그녀는 춤을 매우 잘 춘다."},
    {"We studied Korean grammar.", "우리는 한국어 문법을 공부했다."}
};

GrammarScore grammarScores[MAX_TRAINEES] = {
    {"Jiyeon", 0}, {"Liam", 0}, {"Emma", 0}, {"Noah", 0}
};

void selectionSort(char* arr[], int n) {
    int i, j, min_idx;
    for (i = 0; i < n-1; i++) {
        min_idx = i;
        for (j = i+1; j < n; j++) {
            if (strcmp(arr[j], arr[min_idx]) < 0) {
                min_idx = j;
            }
        }
        char* temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }
}

int splitWords(char* sentence, char* words[]) {
    int count = 0;
    char* token = strtok(sentence, " ");
    while (token != NULL) {
        words[count++] = token;
        token = strtok(NULL, " ");
    }
    return count;
}

void shuffleWords(char* words[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char* temp = words[i];
        words[i] = words[j];
        words[j] = temp;
    }
}

int getScoreIndex(char* nickname) {
    for (int i = 0; i < MAX_TRAINEES; i++) {
        if (strcmp(grammarScores[i].nickname, nickname) == 0)
            return i;
    }
    return -1;
}

void composeSentences(char* nickname) {
    int idx = getScoreIndex(nickname);
    if (idx == -1) return;

    int used[10] = {0};
    int totalScore = 0;
    char input[200];

    for (int i = 0; i < 5; i++) {
        int q;
        do {
            q = rand() % 10;
        } while (used[q]);
        used[q] = 1;

        printf("\nEnglish: %s\n", sentencePairs[q].eng);

        char temp[200];
        strcpy(temp, sentencePairs[q].kor);
        char* words[MAX_WORDS];
        int count = splitWords(temp, words);

        shuffleWords(words, count);
        selectionSort(words, count);

        printf("Arrange Korean: ");
        for (int j = 0; j < count; j++) printf("%s ", words[j]);
        printf("\nAnswer: ");

        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, sentencePairs[q].kor) == 0) {
            totalScore += 20;
            printf("Correct!\n");
        } else {
            printf("Wrong. Correct: %s\n", sentencePairs[q].kor);
        }
    }

    grammarScores[idx].score += totalScore;
    printf("%s's Total Grammar Score: %d\n", nickname, grammarScores[idx].score);
}

void printScore(char* nickname) {
    int idx = getScoreIndex(nickname);
    if (idx == -1 || grammarScores[idx].score == 0) {
        printf("No scores available.\n");
        return;
    }
    printf("\n[%s's Score: %d]\n", grammarScores[idx].nickname, grammarScores[idx].score);
    printf("\n=== All Scores Ranking ===\n");
    for (int i = 0; i < MAX_TRAINEES; i++) {
        printf("%s - %d\n", grammarScores[i].nickname, grammarScores[i].score);
    }
}

void learnKoreanGrammar() {
    char nickname[MAX_NAME];
    int tries = 0, index;

    while (tries < 3) {
        printf("Enter your nickname: ");
        scanf("%s", nickname);
        getchar();
        index = getScoreIndex(nickname);
        if (index != -1) break;
        printf("Nickname not found. Try again.\n");
        tries++;
    }
    if (index == -1) return;

    while (1) {
        printf("\n1. Compose Korean Sentences\n2. Check Scores\n0. Exit\nSelect: ");
        char choice;
        scanf(" %c", &choice);
        getchar();

        if (choice == '1') composeSentences(nickname);
        else if (choice == '2') printScore(nickname);
        else if (choice == '0') break;
    }
}

int main() {
    srand(time(NULL));
    learnKoreanGrammar();
    return 0;
}
