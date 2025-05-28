#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_TRAINEES 4
#define MAX_QUESTIONS 10
#define QUIZ_QUESTIONS 5
#define MAX_NAME 50
#define MAX_ANSWER 100
#define PASS_SCORE 80

typedef struct {
    int id;
    char question[MAX_ANSWER];
    char correct_answer[MAX_ANSWER];
} Question;

typedef struct {
    char name[MAX_NAME];
    char nickname[MAX_NAME];
    char nationality[MAX_NAME];
    int score;
    char pass[5];
} Trainee;

typedef struct {
    int question_id;
    char correctness;
} AnswerResult;

Trainee trainees[MAX_TRAINEES] = {
    {"Park Ji-yeon", "Jiyeon", "Korean", 0, "No"},
    {"Alex Carter", "Lex", "American", 0, "No"},
    {"Sophie Dubois", "Soso", "French", 0, "No"},
    {"Takumi Sato", "Taku", "Japanese", 0, "No"}
};

Question questions[MAX_QUESTIONS] = {
    {1, "Hello", "안녕하세요"},
    {2, "Thank you", "감사합니다"},
    {3, "Yes", "네"},
    {4, "No", "아니요"},
    {5, "I love you", "사랑해요"},
    {6, "Goodbye", "안녕히 가세요"},
    {7, "Sorry", "죄송합니다"},
    {8, "Please", "부탁합니다"},
    {9, "What is your name?", "이름이 뭐예요?"},
    {10, "How are you?", "잘 지내세요?"}
};

AnswerResult answers[QUIZ_QUESTIONS];
int selected_indices[QUIZ_QUESTIONS];

int randExclude(int *used, int used_count) {
    int x, found;
    do {
        x = rand() % MAX_QUESTIONS;
        found = 0;
        for (int i = 0; i < used_count; i++) {
            if (used[i] == x) {
                found = 1;
                break;
            }
        }
    } while (found);
    return x;
}

int selectRandomTakers() {
    int i;
    srand(time(NULL));
    do {
        i = rand() % MAX_TRAINEES;
    } while (strcmp(trainees[i].nationality, "Korean") == 0);
    printf("Welcome, %s, to the Korean quiz session!\n", trainees[i].name);
    return i;
}

void isAnswer(int trainee_index) {
    int total = 0;
    for (int i = 0; i < QUIZ_QUESTIONS; i++) {
        if (answers[i].correctness == 'O') total += 20;
    }
    trainees[trainee_index].score = total;
    strcpy(trainees[trainee_index].pass, total >= PASS_SCORE ? "Yes" : "No");
    printf("\n===== Answer Sheet =====\n");
    for (int i = 0; i < QUIZ_QUESTIONS; i++) {
        printf("Q%d: %c\n", answers[i].question_id, answers[i].correctness);
    }
    printf("Total Score: %d\n", trainees[trainee_index].score);
    printf("Pass: %s\n", trainees[trainee_index].pass);
}

void serveRandomQuiz(int trainee_index) {
    printf("Quiz will begin in 30 seconds...\n");
    sleep(30);
    for (int i = 0; i < QUIZ_QUESTIONS; i++) {
        int idx = randExclude(selected_indices, i);
        selected_indices[i] = idx;
        printf("\nQ%d: %s\n", questions[idx].id, questions[idx].question);
        char answer[MAX_ANSWER];
        printf("Your answer: ");
        fgets(answer, MAX_ANSWER, stdin);
        answer[strcspn(answer, "\n")] = 0;
        answers[i].question_id = questions[idx].id;
        answers[i].correctness = strcmp(answer, questions[idx].correct_answer) == 0 ? 'O' : 'X';
    }
    isAnswer(trainee_index);
}

void testKoreanLang() {
    int index = selectRandomTakers();
    serveRandomQuiz(index);
}

int main() {
    char input;
    while (1) {
        printf("\n[II. Training > 3. Language and Pronunciation Training]\n");
        printf("A. Korean Quiz\n");
        printf("Q. Quit\n");
        printf("Choose option: ");
        input = getchar();
        getchar();
        if (input == 'A' || input == 'a') {
            testKoreanLang();
        } else if (input == 'Q' || input == 'q') {
            break;
        } else {
            printf("Invalid input\n");
        }
    }
    return 0;
}
