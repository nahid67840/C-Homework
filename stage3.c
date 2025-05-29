#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_QUESTIONS 10
#define MAX_QUIZ_QUESTIONS 5
#define MAX_FLASHCARDS 24
#define MAX_ATTEMPTS 3
#define MAX_RESULTS 100
#define MAX_SENTENCES 10
#define MAX_WORDS 10
#define MAX_TRAINEES 10

// Structures
typedef struct {
    int id;
    char question[256];
    char correctAnswer[128];
} KoreanQuestion;

typedef struct {
    char name[50];
    char nickname[50];
    char nationality[30];
    int quizScore;
    char passStatus[10];
} Trainee;

typedef struct {
    int questionID;
    char correctness;
} QuizAnswer;

typedef struct {
    char hangul[10];
    char engLetter[5];
    char engPron[20];
    char type[10];
} HangulJamo;

typedef struct {
    char name[50];
    char challengeType[3];
    int totalScore;
    int attemptCount;
} FlashcardResult;

typedef struct {
    char english[256];
    char korean[256];
} EngKorSentence;

typedef struct {
    char nickname[50];
    int grammarScore;
} GrammarScore;

// Global Data
KoreanQuestion questionBank[MAX_QUESTIONS] = {
    {1, "What is 'Hello' in Korean?", "안녕하세요"},
    {2, "What is 'Thank you' in Korean?", "감사합니다"},
    {3, "What is 'Sorry' in Korean?", "죄송합니다"},
    {4, "What is 'Yes' in Korean?", "네"},
    {5, "What is 'No' in Korean?", "아니요"},
    {6, "What is 'Goodbye' in Korean?", "안녕히 가세요"},
    {7, "What is 'Please' in Korean?", "주세요"},
    {8, "What is 'How are you?' in Korean?", "잘 지냈어요?"},
    {9, "What is 'Delicious' in Korean?", "맛있어요"},
    {10, "What is 'I love you' in Korean?", "사랑해요"}
};

Trainee trainees[MAX_TRAINEES] = {
    {"Alex Smith", "Alex", "USA", 0, ""},
    {"Maria Gomez", "Maria", "Mexico", 0, ""},
    {"Takeshi Tanaka", "Takeshi", "Japan", 0, ""},
    {"Park Ji-yeon", "Jiyeon", "Korea", 0, ""}
};

QuizAnswer quizAnswers[MAX_QUIZ_QUESTIONS];
HangulJamo flashcards[MAX_FLASHCARDS] = {
    {"ㄱ", "g", "giyeok", "consonant"}, {"ㄴ", "n", "nieun", "consonant"},
    {"ㄷ", "d", "digeut", "consonant"}, {"ㄹ", "r", "rieul", "consonant"},
    {"ㅁ", "m", "mieum", "consonant"}, {"ㅂ", "b", "bieup", "consonant"},
    {"ㅅ", "s", "siot", "consonant"}, {"ㅇ", "", "ieung", "consonant"},
    {"ㅈ", "j", "jieut", "consonant"}, {"ㅊ", "ch", "chieut", "consonant"},
    {"ㅋ", "k", "kieuk", "consonant"}, {"ㅌ", "t", "tieut", "consonant"},
    {"ㅍ", "p", "pieup", "consonant"}, {"ㅎ", "h", "hieut", "consonant"},
    {"ㅏ", "a", "a", "vowel"}, {"ㅑ", "ya", "ya", "vowel"},
    {"ㅓ", "eo", "eo", "vowel"}, {"ㅕ", "yeo", "yeo", "vowel"},
    {"ㅗ", "o", "o", "vowel"}, {"ㅛ", "yo", "yo", "vowel"},
    {"ㅜ", "u", "u", "vowel"}, {"ㅠ", "yu", "yu", "vowel"},
    {"ㅡ", "eu", "eu", "vowel"}, {"ㅣ", "i", "i", "vowel"}
};

FlashcardResult results[MAX_RESULTS];
int resultCount = 0;

EngKorSentence sentences[MAX_SENTENCES] = {
    {"I eat rice every day.", "나는 매일 밥을 먹어요"},
    {"She reads a book at night.", "그녀는 밤에 책을 읽어요"},
    {"We go to school together.", "우리는 함께 학교에 가요"},
    {"He drinks coffee in the morning.", "그는 아침에 커피를 마셔요"},
    {"They play soccer in the park.", "그들은 공원에서 축구를 해요"},
    {"You study Korean very hard.", "당신은 한국어를 열심히 공부해요"},
    {"The cat is sleeping on the sofa.", "고양이는 소파에서 자고 있어요"},
    {"My father drives a car.", "아버지는 차를 운전해요"},
    {"The children are playing games.", "아이들은 게임을 하고 있어요"},
    {"I am writing a letter now.", "나는 지금 편지를 쓰고 있어요"}
};

GrammarScore grammarScores[MAX_TRAINEES] = {
    {"Alex", 0}, {"Maria", 0}, {"Takeshi", 0}, {"Jiyeon", 0}
};
// Function declarations
void testKoreanLang();
void learnHangul();
void learnKoreanGrammar();

int main() {
    srand(time(NULL));
    int choice;

    while (1) {
        printf("\n==== Millieways Language Training Menu ====\n");
        printf("1. Korean Quiz\n");
        printf("2. Hangul Jamo Learning\n");
        printf("3. Learn Korean Grammar\n");
        printf("0. Exit\nChoose: ");
        scanf("%d", &choice); getchar();  // To consume the newline

        switch (choice) {
            case 1:
                testKoreanLang();  // Problem 5
                break;
            case 2:
                learnHangul();     // Problem 6
                break;
            case 3:
                learnKoreanGrammar();  // Problem 7
                break;
            case 0:
                printf("Exiting program. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
