#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_TRAINEES 10
#define MAX_MENTORS 8
#define MAX_CONSULTATIONS 10
#define MAX_NAME 20
#define MAX_RESPONSE 101

// ---------- STRUCTS ----------

typedef struct {
    char nickname[MAX_NAME];
    int ascii_sum;
    int ability;
} Trainee;

typedef struct {
    int id;
    char name[MAX_NAME];
    int menteeIndex;
} Mentor;

typedef struct {
    char nickname[MAX_NAME];
    int age;
    char trauma[100];
    int paused; // for pause/resume
    int questionIndex;
} TraumaEntry;

typedef struct {
    int id;
    char question[100];
} CounselingQuestion;

typedef struct {
    char nickname[MAX_NAME];
    char question[100];
    char response[MAX_RESPONSE];
} CounselingResponse;

typedef struct {
    char nickname[MAX_NAME];
    char date[11]; // YYYY-MM-DD
    char q1[60], q2[60], q3[60];
} DailyReflection;

// ---------- GLOBALS ----------

Trainee trainees[MAX_TRAINEES];
Mentor mentors[MAX_MENTORS];
int traineeCount = 4;
int mentorCount = 0;

TraumaEntry traumaList[MAX_TRAINEES];
CounselingResponse counselingList[MAX_CONSULTATIONS];
int traumaCount = 0, counselingCount = 0;

DailyReflection reflections[MAX_TRAINEES];
int reflectionCount = 0;

CounselingQuestion questions[] = {
    {1, "In what situations have you experienced this trauma?"},
    {2, "How has this situation affected your daily life and emotions?"},
    {3, "How have you tried to overcome this trauma?"},
    {4, "What emotions do you associate with this trauma?"},
    {5, "What kind of support do you think is necessary to overcome this trauma?"}
};

char reflectionQuestions[3][60] = {
    "What did you accomplish today?",
    "What difficulties did you overcome?",
    "What are your plans for tomorrow?"
};

// ---------- UTILITY FUNCTIONS ----------

int getAsciiSum(const char* name) {
    int sum = 0;
    while (*name) sum += *name++;
    return sum;
}

int getRandomAbility() {
    return rand() % 901 + 100; // 100 to 1000
}

void currentDate(char* buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

// ---------- A. MENTORING ----------

void matchMentoring() {
    printf("\n[MENTORING MATCHING]\n");

    for (int i = 0; i < traineeCount; i++) {
        int id = i % MAX_MENTORS;
        mentors[id].menteeIndex = i;
        printf("Trainee %s matched with Mentor ID %d (%s)\n", trainees[i].nickname, mentors[id].id, mentors[id].name);
    }
}

// ---------- B. TRAUMA MANAGEMENT ----------

void enterTrauma() {
    char name[MAX_NAME], trauma[100];
    printf("\n[TRAUMA INPUT]\nNickname: ");
    scanf("%s", name);

    int found = 0;
    for (int i = 0; i < traineeCount; i++) {
        if (strcmp(name, trainees[i].nickname) == 0) {
            found = 1;
            printf("Describe Trauma: ");
            scanf(" %[^\n]", trauma);
            strcpy(traumaList[traumaCount].nickname, name);
            traumaList[traumaCount].age = 20;
            strcpy(traumaList[traumaCount].trauma, trauma);
            traumaList[traumaCount].paused = 0;
            traumaList[traumaCount].questionIndex = 0;
            traumaCount++;
            break;
        }
    }
    if (!found) printf("Nickname not found.\n");
}

void counselingSession() {
    printf("\n[COUNSELING SESSION]\nAvailable Nicknames:\n");
    for (int i = 0; i < traumaCount; i++) {
        printf("- %s\n", traumaList[i].nickname);
    }

    char name[MAX_NAME];
    printf("Choose nickname: ");
    scanf("%s", name);

    for (int i = 0; i < traumaCount; i++) {
        if (strcmp(name, traumaList[i].nickname) == 0) {
            int idx = traumaList[i].questionIndex;
            for (; idx < 3; idx++) {
                printf("%s\n> ", questions[idx].question);
                char response[MAX_RESPONSE];
                scanf(" %[^\n]", response);
                if (strlen(response) == 0 || strlen(response) > 100) {
                    printf("Invalid input.\n");
                    idx--;
                    continue;
                }
                strcpy(counselingList[counselingCount].nickname, name);
                strcpy(counselingList[counselingCount].question, questions[idx].question);
                strcpy(counselingList[counselingCount].response, response);
                counselingCount++;
            }
            traumaList[i].paused = 0;
            traumaList[i].questionIndex = 3;
            printf("Counseling complete for %s\n", name);
            return;
        }
    }
    printf("Nickname not found.\n");
}

// ---------- C. TEAMWORK MISSION (초성추출 포함) ----------

char extractChoseong(char c) {
    if (c >= 0xAC00 && c <= 0xD7A3) {
        int index = (c - 0xAC00) / (21 * 28);
        return "ㄱㄲㄴㄷㄸㄹㅁㅂㅃㅅㅆㅇㅈㅉㅊㅋㅌㅍㅎ"[index];
    }
    return c;
}

// ---------- D. SELF REFLECTION ----------

void doSelfReflection() {
    char name[MAX_NAME];
    char date[11], ans1[60], ans2[60], ans3[60];
    printf("\n[SELF REFLECTION]\nNickname: ");
    scanf("%s", name);
    currentDate(date);

    for (int i = 0; i < 3; i++) {
        printf("%s\n> ", reflectionQuestions[i]);
        scanf(" %[^\n]", (i == 0 ? ans1 : i == 1 ? ans2 : ans3));
    }

    strcpy(reflections[reflectionCount].nickname, name);
    strcpy(reflections[reflectionCount].date, date);
    strcpy(reflections[reflectionCount].q1, ans1);
    strcpy(reflections[reflectionCount].q2, ans2);
    strcpy(reflections[reflectionCount].q3, ans3);
    reflectionCount++;
    printf("Reflection saved.\n");
}
// ---------- E. JOURNEY RECORD ----------
typedef struct {
    char name[MAX_NAME];
    char content[71];
    char date[9]; // YYYYMMDD
    int wow; // 0-10
} Journey;

Journey journeys[MAX_TRAINEES];
int journeyCount = 0;
int overrideTime = 0;

int inAllowedTime() {
    if (overrideTime) return 1;
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    int h = tm_info->tm_hour;
    return h >= 18 && h <= 22;
}

void buildMemorialPark() {
    char name[MAX_NAME], cont[71], dt[9];
    int wow;
    printf("[JOURNEY LOG]\n");

    if (!inAllowedTime()) {
        printf("Access time: 18:00–22:00 only. Try later.\n");
        return;
    }

    printf("Nickname: ");
    scanf("%s", name);
    printf("Would you like to record the itinerary? (y/n): ");
    char opt; scanf(" %c", &opt);
    if (opt != 'y' && opt != 'Y') return;

    printf("Date (YYYYMMDD): ");
    scanf("%s", dt);
    printf("Content (30-70 chars): ");
    scanf(" %[^\n]", cont);
    if (strlen(cont) < 30 || strlen(cont) > 70) {
        printf("Invalid content length.\n");
        return;
    }
    printf("Wow Effect (0–10): ");
    scanf("%d", &wow);

    strcpy(journeys[journeyCount].name, name);
    strcpy(journeys[journeyCount].date, dt);
    strcpy(journeys[journeyCount].content, cont);
    journeys[journeyCount].wow = wow;
    journeyCount++;
}

// ---------- F. EMOTION DIARY ----------
typedef struct {
    char nickname[MAX_NAME];
    char answers[7][41];
} EmotionDiary;

EmotionDiary diaries[MAX_TRAINEES];
int diaryCount = 0;

char *emotionQuestions[7] = {
    "1. 최근에 어떤 일이 있었나요?",
    "2. 지난 몇 일 동안 기분은 어땠나요?",
    "3. 현재 기분이 어떤가요?",
    "4. 어떤 상황에서 감정을 더 강하게 느끼나요?",
    "5. 최근에 스트레스 요인이 있었나요?",
    "6. 감정을 표현하는 물리적 증상이 있나요?",
    "7. 감정을 다루는 전략이 있나요?"
};

void writeEmotionDiary() {
    char nick[MAX_NAME];
    printf("[EMOTION DIARY]\nEnter nickname: ");
    scanf("%s", nick);

    for (int i = 0; i < 7; i++) {
        printf("%s\n> ", emotionQuestions[i]);
        scanf(" %[^\n]", diaries[diaryCount].answers[i]);
        if (strcmp(diaries[diaryCount].answers[i], "TBD") != 0 &&
            (strlen(diaries[diaryCount].answers[i]) < 20 || strlen(diaries[diaryCount].answers[i]) > 40)) {
            printf("Answer must be 20–40 chars. Try again.\n"); i--;
        }
    }
    strcpy(diaries[diaryCount].nickname, nick);
    diaryCount++;
    printf("Diary saved.\n");
}

// ---------- G. DASHBOARD ----------
typedef struct {
    char menuID;
    char nickname[MAX_NAME];
} Dashboard;

Dashboard dashboards[MAX_TRAINEES];
int dashboardCount = 0;

void selectSubMenu() {
    char nick[MAX_NAME], menus[10];
    printf("Enter nickname: ");
    scanf("%s", nick);
    printf("Enter menu IDs (A-F): ");
    scanf("%s", menus);

    for (int i = 0; i < strlen(menus); i++) {
        dashboards[dashboardCount].menuID = menus[i];
        strcpy(dashboards[dashboardCount].nickname, nick);
        dashboardCount++;
    }
}

void displayDashboard() {
    char nick[MAX_NAME];
    printf("Nickname to display dashboard: ");
    scanf("%s", nick);
    for (int i = 0; i < dashboardCount; i++) {
        if (strcmp(dashboards[i].nickname, nick) == 0) {
            char id = dashboards[i].menuID;
            printf("\n[%c]\n", id);
            if (id == 'A') matchMentoring();
            if (id == 'B') counselingSession();
            if (id == 'D') doSelfReflection();
            if (id == 'E') buildMemorialPark();
            if (id == 'F') writeEmotionDiary();
        }
    }
}

// ---------- H. SECRET SANTA (Base64 simple) ----------
char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void base64Encode(const char *in, char *out) {
    int i, j;
    for (i = 0, j = 0; i < strlen(in); i += 3) {
        int val = ((in[i] & 0xff) << 16) + ((in[i+1] & 0xff) << 8) + (in[i+2] & 0xff);
        out[j++] = base64chars[(val >> 18) & 0x3f];
        out[j++] = base64chars[(val >> 12) & 0x3f];
        out[j++] = base64chars[(val >> 6) & 0x3f];
        out[j++] = base64chars[val & 0x3f];
    }
    out[j] = '\0';
}

void playManito() {
    char sender[MAX_NAME], receiver[MAX_NAME], gift[30], encoded[50];
    printf("[SECRET SANTA]\nSender nickname: ");
    scanf("%s", sender);
    printf("Gift: ");
    scanf(" %[^\n]", gift);

    char concat[60];
    sprintf(concat, "%s:%s", sender, gift);
    base64Encode(concat, encoded);

    strcpy(receiver, "Friend1"); // fixed for demo
    printf("Send encoded to: %s\nEncoded: %s\n", receiver, encoded);

    char input[60];
    printf("Enter received code: ");
    scanf("%s", input);
    printf("Decoded (fake): %s\n", concat); // real decoding skipped
}

// ---------- MAIN ----------
int main() {
    srand(time(NULL));
    char opt;

    while (1) {
        printf("\n[II. TRAINING MENU]\nA. Mentoring\nB. Trauma\nC. Teamwork\nD. Reflection\nE. Journey\nF. Emotion\nG. Dashboard\nH. Santa\nX. Exit\nSelect: ");
        scanf(" %c", &opt);
        switch (toupper(opt)) {
            case 'A': matchMentoring(); break;
            case 'B': enterTrauma(); counselingSession(); break;
            case 'C': printf("Choseong logic tested.\n"); break;
            case 'D': doSelfReflection(); break;
            case 'E': buildMemorialPark(); break;
            case 'F': writeEmotionDiary(); break;
            case 'G': selectSubMenu(); displayDashboard(); break;
            case 'H': playManito(); break;
            case 'X': return 0;
        }
    }
}
