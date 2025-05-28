#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_MEMBERS 10
#define MAX_TEXT 150
#define MAX_RESPONSES 200

// ------------------- Structs -------------------

typedef struct {
    char nickname[50];
    int id;
    int ability;
} Trainee;

typedef struct {
    int id;
    char name[50];
    int menteeId;
} Mentor;

typedef struct {
    char nickname[50];
    int age;
    char trauma[200];
} TraumaInfo;

typedef struct {
    int id;
    char content[150];
} CounselingQuestion;

typedef struct {
    char nickname[50];
    char question[150];
    char response[101];
} CounselingResponse;

typedef struct {
    char nickname[50];
    char date[11]; // YYYY-MM-DD
    char achievement[51];
    char struggle[51];
    char plan[51];
} DailyReflection;

// ------------------- Global Data -------------------

char milliewayMembers[5][2][50] = {
    {"Luna", "lu"},
    {"Orion", "or"},
    {"Nova", "nv"},
    {"Vega", "vg"},
    {"Zane", "zn"}
};

CounselingQuestion questions[5] = {
    {1, "In what situations have you experienced this trauma?"},
    {2, "How has this situation affected your daily life and emotions?"},
    {3, "How have you tried to overcome this trauma?"},
    {4, "What emotions do you associate with this trauma?"},
    {5, "What kind of support do you think is necessary to overcome this trauma?"}
};

char reflectionQuestions[3][MAX_TEXT] = {
    "What have you accomplished today?",
    "What difficulties have you overcome?",
    "What plans do you have for tomorrow?"
};

DailyReflection reflections[MAX_MEMBERS];
int reflectionCount = 0;

// ------------------- Utility -------------------

void getTodayDate(char *buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

void clearInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// ------------------- A. Mentoring -------------------

void matchMentoring() {
    printf("[Mentor Matching Logic Here]\n");
}

// ------------------- B. Trauma Management -------------------

void inputTraumaData(TraumaInfo traumaList[], int *traumaCount) {
    char name[50], trauma[200];
    while (1) {
        printf("Enter nickname (or 'exit'): ");
        scanf("%s", name);
        if (strcmp(name, "exit") == 0) break;

        int found = 0;
        for (int i = 0; i < *traumaCount; i++) {
            if (strcmp(name, traumaList[i].nickname) == 0) {
                found = 1;
                printf("Enter trauma description: ");
                getchar(); fgets(trauma, sizeof(trauma), stdin);
                trauma[strcspn(trauma, "\n")] = 0;
                strcpy(traumaList[i].trauma, trauma);
                break;
            }
        }
        found ? printf("Saved.\n") : printf("Nickname not found.\n");
    }
}

void traumaMenu(TraumaInfo traumaList[], int *traumaCount) {
    CounselingResponse responses[MAX_RESPONSES];
    int responseCount = 0;
    char option;

    while (1) {
        printf("\n[B] Input Trauma  [C] Counseling  [V] View Result  [E] Exit\nSelect: ");
        scanf(" %c", &option);

        if (toupper(option) == 'B') {
            inputTraumaData(traumaList, traumaCount);
        } else if (toupper(option) == 'E') {
            return;
        } else {
            printf("Invalid.\n");
        }
    }
}

void overcomeTrauma() {
    static TraumaInfo traumaList[MAX_MEMBERS] = {
        {"lu", 19, ""}, {"or", 20, ""}, {"nv", 18, ""}, {"vg", 21, ""}, {"zn", 22, ""}
    };
    static int traumaCount = 5;
    traumaMenu(traumaList, &traumaCount);
}

// ------------------- C. Teamwork Mission -------------------

void completeMission() {
    printf("[Teamwork Mission Logic Placeholder]\n");
}

// ------------------- D. Self Reflection -------------------

void inputReflection() {
    char name[50], ach[60], strug[60], plan[60];
    printf("Enter your nickname: ");
    scanf("%s", name);
    clearInput();

    printf("%s\n", reflectionQuestions[0]);
    fgets(ach, 60, stdin); ach[strcspn(ach, "\n")] = 0;

    printf("%s\n", reflectionQuestions[1]);
    fgets(strug, 60, stdin); strug[strcspn(strug, "\n")] = 0;

    printf("%s\n", reflectionQuestions[2]);
    fgets(plan, 60, stdin); plan[strcspn(plan, "\n")] = 0;

    if (strlen(ach) < 20 || strlen(ach) > 50 || strlen(strug) < 20 || strlen(strug) > 50 || strlen(plan) < 20 || strlen(plan) > 50) {
        printf("Each answer must be between 20–50 characters.\n");
        return;
    }

    strcpy(reflections[reflectionCount].nickname, name);
    getTodayDate(reflections[reflectionCount].date);
    strcpy(reflections[reflectionCount].achievement, ach);
    strcpy(reflections[reflectionCount].struggle, strug);
    strcpy(reflections[reflectionCount].plan, plan);
    reflectionCount++;

    printf("Reflection saved.\n");
}

void displayReflection() {
    char name[50];
    printf("Enter nickname to view reflection: ");
    scanf("%s", name);

    for (int i = 0; i < reflectionCount; i++) {
        if (strcmp(name, reflections[i].nickname) == 0) {
            printf("Date: %s\n", reflections[i].date);
            printf("Q1: %s\nA: %s\n", reflectionQuestions[0], reflections[i].achievement);
            printf("Q2: %s\nA: %s\n", reflectionQuestions[1], reflections[i].struggle);
            printf("Q3: %s\nA: %s\n", reflectionQuestions[2], reflections[i].plan);
            return;
        }
    }
    printf("No reflection found.\n");
}

void doSelfReflection() {
    char option;
    printf("[R] Record  [V] View: ");
    scanf(" %c", &option);
    if (toupper(option) == 'R') inputReflection();
    else if (toupper(option) == 'V') displayReflection();
    else printf("Invalid option.\n");
}
// ------------------- E. Journey Log -------------------

typedef struct {
    char nickname[50];
    char date[9]; // YYYYMMDD
    char content[71];
    int wow;
} JourneyRecord;

JourneyRecord journeys[MAX_MEMBERS];
int journeyCount = 0;

void buildMemorialPark() {
    time_t raw;
    struct tm *timeinfo;
    time(&raw);
    timeinfo = localtime(&raw);
    int hour = timeinfo->tm_hour;

    if (hour < 18 || hour > 22) {
        printf("You can only use this function from 18:00 to 22:00.\n");
        return;
    }

    char nick[50], content[100];
    int wow;

    printf("Enter your nickname: ");
    scanf("%s", nick);
    clearInput();

    printf("Enter your journey content (30-70 chars): ");
    fgets(content, sizeof(content), stdin); content[strcspn(content, "\n")] = 0;

    if (strlen(content) < 30 || strlen(content) > 70) {
        printf("Content must be between 30-70 characters.\n");
        return;
    }

    printf("Enter wow effect (0–10): ");
    scanf("%d", &wow);

    strcpy(journeys[journeyCount].nickname, nick);
    getTodayDate(journeys[journeyCount].date);
    strcpy(journeys[journeyCount].content, content);
    journeys[journeyCount].wow = wow;
    journeyCount++;

    printf("Journey recorded.\n");
}

// ------------------- F. Emotion Diary -------------------

typedef struct {
    char nickname[50];
    char answers[7][41];
} EmotionDiary;

char emotionQuestions[7][100] = {
    "1. 최근에 어떤 일이 있었나요? 그 일에 대한 감정을 어떻게 느꼈나요?",
    "2. 지난 몇 일 동안 기분은 어땠나요?",
    "3. 현재 기분이 어떤가요? 기쁨, 슬픔, 분노, 불안 등 어떤 감정을 느끼고 있나요?",
    "4. 어떤 상황에서 감정을 더 강하게 느끼나요?",
    "5. 감정적인 변화나 스트레스 요인이 최근에 있었나요?",
    "6. 감정을 표현하는 물리적인 증상이 있나요?",
    "7. 감정을 다루는 방식에 대해 어떤 생각이나 전략이 있나요?"
};

EmotionDiary diaries[MAX_MEMBERS];
int diaryCount = 0;

void writeEmotionDiary() {
    char nick[50];
    int skipped = 0;

    printf("Enter your nickname: ");
    scanf("%s", nick);
    clearInput();

    strcpy(diaries[diaryCount].nickname, nick);
    for (int i = 0; i < 7; i++) {
        printf("%s\n", emotionQuestions[i]);
        fgets(diaries[diaryCount].answers[i], 41, stdin);
        diaries[diaryCount].answers[i][strcspn(diaries[diaryCount].answers[i], "\n")] = 0;

        if (strcmp(diaries[diaryCount].answers[i], "TBD") == 0)
            skipped++;
    }
    diaryCount++;

    printf("Emotion diary saved. Skipped: %d\n", skipped);
}

void printEmotionDiary() {
    char name[50];
    printf("Enter nickname to view emotion diary: ");
    scanf("%s", name);

    for (int i = 0; i < diaryCount; i++) {
        if (strcmp(name, diaries[i].nickname) == 0) {
            for (int j = 0; j < 7; j++) {
                if (strcmp(diaries[i].answers[j], "TBD") != 0) {
                    printf("Q%d: %s\nA: %s\n", j + 1, emotionQuestions[j], diaries[i].answers[j]);
                }
            }
            return;
        }
    }
    printf("No diary found.\n");
}

void logEmotionDiary() {
    char choice;
    printf("[W] Write  [P] Print: ");
    scanf(" %c", &choice);
    if (toupper(choice) == 'W') writeEmotionDiary();
    else if (toupper(choice) == 'P') printEmotionDiary();
    else printf("Invalid.\n");
}

// ------------------- G. Dashboard -------------------

typedef struct {
    char nickname[50];
    char menuIds[10];
    int count;
} Dashboard;

Dashboard dashboards[MAX_MEMBERS];
int dashCount = 0;

void showDashboard() {
    char name[50];
    printf("Enter nickname for dashboard: ");
    scanf("%s", name);

    printf("Choose submenus to display (A-F, e.g., ACD): ");
    char menus[10];
    scanf("%s", menus);

    strcpy(dashboards[dashCount].nickname, name);
    strcpy(dashboards[dashCount].menuIds, menus);
    dashboards[dashCount].count = strlen(menus);
    dashCount++;

    printf("Dashboard setup complete.\n");
}

void displayDashboard() {
    char name[50];
    printf("Enter nickname to view dashboard: ");
    scanf("%s", name);

    for (int i = 0; i < dashCount; i++) {
        if (strcmp(name, dashboards[i].nickname) == 0) {
            printf("Dashboard for %s:\n", name);
            for (int j = 0; j < dashboards[i].count; j++) {
                char m = dashboards[i].menuIds[j];
                if (m == 'D') displayReflection();
                else if (m == 'E') buildMemorialPark();
                else if (m == 'F') printEmotionDiary();
                // Add more submenu displays here as needed
            }
            return;
        }
    }
    printf("No dashboard found.\n");
}

// ------------------- H. Secret Santa -------------------

char *base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void encodeBase64(char *input, char *output) {
    int i, j, len = strlen(input);
    for (i = 0, j = 0; i < len;) {
        unsigned char a = i < len ? input[i++] : 0;
        unsigned char b = i < len ? input[i++] : 0;
        unsigned char c = i < len ? input[i++] : 0;

        output[j++] = base64[a >> 2];
        output[j++] = base64[((a & 3) << 4) | (b >> 4)];
        output[j++] = (i > len + 1) ? '=' : base64[((b & 15) << 2) | (c >> 6)];
        output[j++] = (i > len) ? '=' : base64[c & 63];
    }
    output[j] = '\0';
}

void playManito() {
    char santa[50], gift[50], encoded[100];
    printf("Enter your nickname: ");
    scanf("%s", santa);
    printf("Enter your gift: ");
    scanf("%s", gift);

    char combined[100];
    sprintf(combined, "%s:%s", santa, gift);
    encodeBase64(combined, encoded);
    printf("Encoded: %s\n", encoded);

    srand(time(0));
    int targetIndex;
    do {
        targetIndex = rand() % 5;
    } while (strcmp(milliewayMembers[targetIndex][1], santa) == 0);

    printf("Guess who you are gifting to: ");
    char guess[50];
    scanf("%s", guess);

    if (strcmp(guess, milliewayMembers[targetIndex][1]) == 0) {
        printf("Correct! Secret Santa Info: %s\n", encoded);
    } else {
        printf("Wrong! Try again.\n");
    }
}

// ------------------- Main Menu -------------------

void showMainMenu() {
    printf("\n=== Magratea System ===\n");
    printf("A. Mentoring\n");
    printf("B. Trauma Management\n");
    printf("C. Teamwork Mission\n");
    printf("D. Self-Reflection\n");
    printf("E. Journey Log\n");
    printf("F. Emotion Diary\n");
    printf("G. Dashboard\n");
    printf("H. Secret Santa\n");
    printf("Q. Quit\n");
    printf("Select: ");
}

int main() {
    char option;
    while (1) {
        showMainMenu();
        scanf(" %c", &option);
        switch (toupper(option)) {
            case 'A': matchMentoring(); break;
            case 'B': overcomeTrauma(); break;
            case 'C': completeMission(); break;
            case 'D': doSelfReflection(); break;
            case 'E': buildMemorialPark(); break;
            case 'F': logEmotionDiary(); break;
            case 'G': displayDashboard(); break;
            case 'H': playManito(); break;
            case 'Q': return 0;
            default: printf("Invalid choice.\n"); break;
        }
    }
    return 0;
}
