#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MEMBERS 4
#define MAX_QUESTIONS 10
#define MAX_NAME 50
#define MAX_SENTENCE 200
#define MAX_WORDS 20
#define MAX_CHAR 20
#define MAX_FLASHCARDS 24
#define MAX_RESULTS 20
#define MAX_DICTIONARY 30
#define MAX_TEAM 5

// ---------- Common Data Structures ----------
typedef struct {
    char name[MAX_NAME];
    char nickname[MAX_NAME];
    char nationality[MAX_NAME];
    int quizScore;
    char passStatus[10];
} Trainee;

typedef struct {
    int questionID;
    char question[MAX_SENTENCE];
    char answer[MAX_SENTENCE];
} KoreanQuestion;

typedef struct {
    int questionID;
    char isCorrect; // 'O' or 'X'
} QuizAnswer;

typedef struct {
    char hangul[MAX_CHAR];
    char eng[MAX_CHAR];
    char pronunciation[MAX_CHAR];
} HangulCard;

typedef struct {
    char name[MAX_NAME];
    char challengeType[5];
    int totalScore;
    int attemptCount;
} FlashcardResult;

typedef struct {
    char english[MAX_SENTENCE];
    char korean[MAX_SENTENCE];
} GrammarPair;

typedef struct {
    char nickname[MAX_NAME];
    int score;
} GrammarScore;

typedef struct {
    char word[MAX_CHAR];
    char region[MAX_CHAR];
    char standard[MAX_CHAR];
} DialectWord;

typedef struct {
    char group[MAX_NAME];
    char learner[MAX_NAME];
    int score;
    char stdLevel;
    char diaLevel;
} DialectResult;

typedef struct {
    char rom[MAX_SENTENCE];
    char kor[MAX_SENTENCE];
    char author[MAX_NAME];
} DictationData;

typedef struct {
    char group[MAX_NAME];
    char name[MAX_NAME];
    int totalScore;
    int maxMatch;
    int minMatch;
} DictationResult;

typedef struct {
    int avgScore;
    int minScore;
    int timeLimit;
} TeamBaseline;

typedef struct {
    char teamName[MAX_NAME];
    int avgScore;
    int minScore;
    int timeUsed;
    char result[10];
} TeamPerformance;

// ---------- Global Variables ----------
Trainee trainees[MAX_MEMBERS] = {
    {"Park Ji-yeon", "ariel", "Korean", 0, "Fail"},
    {"Ali", "ali", "Bangladeshi", 0, "Fail"},
    {"Nahid", "nahid", "Bangladeshi", 0, "Fail"},
    {"Vinh", "vinh", "Vietnamese", 0, "Fail"}
};

KoreanQuestion questionBank[MAX_QUESTIONS] = {
    {1, "What is 'Hello' in Korean?", "안녕하세요"},
    {2, "What is 'Thank you' in Korean?", "감사합니다"},
    {3, "What is 'Sorry' in Korean?", "죄송합니다"},
    {4, "What is 'Goodbye' in Korean?", "안녕히 가세요"},
    {5, "What is 'Yes' in Korean?", "네"},
    {6, "What is 'No' in Korean?", "아니요"},
    {7, "What is 'I am hungry' in Korean?", "배고파요"},
    {8, "What is 'I love you' in Korean?", "사랑해요"},
    {9, "What is 'Where is the bathroom?' in Korean?", "화장실 어디에요?"},
    {10, "What is 'Nice to meet you' in Korean?", "반갑습니다"}
};

// ---------- Function Prototypes (Problem-Specific) ----------
void testKoreanLang();
void learnHangul();
void learnKoreanGrammar();
void doWordRelay();
void doDictionaryGame();
void learnDialect();
void doDictation();
void inputBaseLine(TeamBaseline *base, int maxScore, int minTime, int maxTime);
void evalTeamPerformance(TeamPerformance *team, int scores[], int count, int timeUsed, TeamBaseline base);

// ---------- Main Menu ----------
void showMenu() {
    int choice;
    while (1) {
        printf("\n====== II. Training > 3. Language and Pronunciation Training ======\n");
        printf("1. Korean Quiz\n");
        printf("2. Hangul Jamo Learning\n");
        printf("3. Learning Korean Grammar\n");
        printf("4. Korean Word Relay\n");
        printf("5. Dictionary Game\n");
        printf("6. Dialect Learning\n");
        printf("7. Dictation Test\n");
        printf("8. Exit\n");
        printf("Select a menu option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                testKoreanLang(); break;
            case 2:
                learnHangul(); break;
            case 3:
                learnKoreanGrammar(); break;
            case 4:
                doWordRelay(); break;
            case 5:
                doDictionaryGame(); break;
            case 6:
                learnDialect(); break;
            case 7:
                doDictation(); break;
            case 8:
                printf("Exiting program.\n"); return;
            default:
                printf("Invalid option. Try again.\n");
        }
    }
}

int main() {
    srand(time(NULL));
    showMenu();
    return 0;
}
