#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_DANCES 6
#define MAX_STEPS 10
#define MAX_MEMBERS 10
#define MAX_STEP_LEN 30
#define MAX_PATTERNS 6
#define MAX_DESC_LEN 300
#define MAX_SONGS 4
#define MAX_NAME_LEN 50

// Structs
typedef struct {
    char nameKR[50];
    char nameEN[50];
    char steps[MAX_STEPS][MAX_STEP_LEN];
    int stepCount;
} Dance;

typedef struct {
    char name[50];
    char nickname[50];
    int score;
} Member;

typedef struct {
    char name[MAX_NAME_LEN];
    char description[MAX_DESC_LEN];
} Pattern;

typedef struct TreeNode {
    char patternName[MAX_NAME_LEN];
    struct TreeNode* next;
} TreeNode;

typedef struct {
    char songName[MAX_NAME_LEN];
    TreeNode* head;
} ChoreoSong;

// Globals
Dance dances[MAX_DANCES];
Member members[MAX_MEMBERS] = {
    {"Arthur", "arth", 0},
    {"Yongjin", "yj", 0},
    {"Sejin", "sj", 0},
    {"Nahid", "nh", 0},
    {"Vinh", "vn", 0}
};
int memberCount = 5;
Pattern patterns[MAX_PATTERNS];
ChoreoSong songs[MAX_SONGS];

// ===== Problem 1: Learn Basic Dance Steps =====

void shuffle(char array[][MAX_STEP_LEN], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp[MAX_STEP_LEN];
        strcpy(temp, array[i]);
        strcpy(array[i], array[j]);
        strcpy(array[j], temp);
    }
}

int readDanceSteps(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Cannot open %s\n", filename);
        return 0;
    }
    int count = 0;
    while (fscanf(fp, "%[^,],%[^,],", dances[count].nameKR, dances[count].nameEN) == 2) {
        char stepLine[200];
        fgets(stepLine, sizeof(stepLine), fp);
        stepLine[strcspn(stepLine, "\n")] = 0;

        char* token = strtok(stepLine, " ");
        int sCount = 0;
        while (token && sCount < MAX_STEPS) {
            strcpy(dances[count].steps[sCount++], token);
            token = strtok(NULL, " ");
        }
        dances[count].stepCount = sCount;
        count++;
        if (count >= MAX_DANCES) break;
    }
    fclose(fp);
    return count;
}

int findMemberIndex(char* nickname) {
    for (int i = 0; i < memberCount; i++) {
        if (strcmp(members[i].nickname, nickname) == 0) return i;
    }
    return -1;
}

int evaluate(char correct[][MAX_STEP_LEN], char input[][MAX_STEP_LEN], int count) {
    int correctCount = 0, inOrder = 1;
    for (int i = 0; i < count; i++) {
        int match = 0;
        for (int j = 0; j < count; j++) {
            if (strcmp(input[i], correct[j]) == 0) {
                match = 1;
                if (i != j) inOrder = 0;
                break;
            }
        }
        if (match) correctCount++;
    }
    if (correctCount == count && inOrder) return 100;
    else if (correctCount == count) return 50;
    else if (correctCount >= 1) return 20;
    return 0;
}

void learnDanceStep() {
    char nickname[50];
    int attempt = 0, index = -1;

    while (attempt < 3) {
        printf("Enter your nickname: ");
        scanf("%s", nickname);
        index = findMemberIndex(nickname);
        if (index != -1) break;
        printf("Nickname not found. Try again.\n");
        attempt++;
    }
    if (index == -1) {
        printf("Failed 3 times. Returning to menu.\n");
        return;
    }

    if (!readDanceSteps("dance_step.txt")) return;

    srand(time(NULL));
    int d = rand() % MAX_DANCES;
    Dance selected = dances[d];

    printf("\nDance: %s (%s)\nSteps: ", selected.nameKR, selected.nameEN);
    for (int i = 0; i < selected.stepCount; i++) {
        printf("%s ", selected.steps[i]);
    }
    printf("\nMemorize in 10 seconds...\n");
    sleep(10);
    system("clear");

    char shuffled[MAX_STEPS][MAX_STEP_LEN];
    for (int i = 0; i < selected.stepCount; i++)
        strcpy(shuffled[i], selected.steps[i]);
    shuffle(shuffled, selected.stepCount);

    printf("Dance: %s (%s)\n", selected.nameKR, selected.nameEN);
    printf("Shuffled Steps:\n");
    for (int i = 0; i < selected.stepCount; i++)
        printf("%d. %s\n", i + 1, shuffled[i]);

    char inputSteps[MAX_STEPS][MAX_STEP_LEN];
    printf("\nEnter the steps in order:\n");
    for (int i = 0; i < selected.stepCount; i++) {
        printf("Step %d: ", i + 1);
        scanf("%s", inputSteps[i]);
    }

    int score = evaluate(selected.steps, inputSteps, selected.stepCount);
    members[index].score = score;

    printf("\nCorrect Steps: ");
    for (int i = 0; i < selected.stepCount; i++)
        printf("%s ", selected.steps[i]);

    printf("\nYour Input: ");
    for (int i = 0; i < selected.stepCount; i++)
        printf("%s ", inputSteps[i]);

    printf("\nYour Score: %d\n", score);
}

// ===== Problem 2: Learn Choreography Patterns =====

int loadPatterns(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Cannot open %s\n", filename);
        return 0;
    }
    int i = 0;
    while (fscanf(fp, "%[^,],%[^\n]\n", patterns[i].name, patterns[i].description) == 2 && i < MAX_PATTERNS) {
        i++;
    }
    fclose(fp);
    return i;
}

void getRandomClue(char* description, char* clue) {
    int len = strlen(description);
    char temp[MAX_DESC_LEN] = "";
    for (int i = 0; i < len; i++) {
        if (description[i] != ' ') {
            int l = strlen(temp);
            temp[l] = description[i];
            temp[l + 1] = '\0';
        }
    }
    len = strlen(temp);
    if (len <= 10) {
        strcpy(clue, temp);
    } else {
        int start = rand() % (len - 10);
        strncpy(clue, &temp[start], 10);
        clue[10] = '\0';
    }
}

int loadChoreoCSV(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return 0;
    int count = 0;
    char line[500];
    while (fgets(line, sizeof(line), fp) && count < MAX_SONGS) {
        line[strcspn(line, "\n")] = 0;
        char* token = strtok(line, ",");
        strcpy(songs[count].songName, token);
        TreeNode* head = NULL;
        TreeNode* tail = NULL;
        while ((token = strtok(NULL, ",")) != NULL) {
            TreeNode* newNode = malloc(sizeof(TreeNode));
            strcpy(newNode->patternName, token);
            newNode->next = NULL;
            if (!head) head = tail = newNode;
            else {
                tail->next = newNode;
                tail = newNode;
            }
        }
        songs[count].head = head;
        count++;
    }
    fclose(fp);
    return count;
}

void displayAllTrees(int songCount) {
    for (int i = 0; i < songCount; i++) {
        printf("\n[%d] %s: ", i + 1, songs[i].songName);
        TreeNode* curr = songs[i].head;
        while (curr) {
            printf("%s -> ", curr->patternName);
            curr = curr->next;
        }
        printf("END\n");
    }
}

void playPatternGame(ChoreoSong song) {
    TreeNode* curr = song.head;
    printf("\nLet's play the choreography pattern game for [%s]!\n", song.songName);
    printf("First Pattern: %s\n", curr->patternName);
    curr = curr->next;

    while (curr) {
        char input[MAX_NAME_LEN];
        printf("Next Pattern? ");
        scanf("%s", input);
        if (strcmp(input, curr->patternName) != 0) {
            printf("Wrong! Returning to menu.\n");
            return;
        }
        printf("Correct!\n");
        curr = curr->next;
    }

    printf("\n🎉 You completed all patterns for %s! Well done!\n", song.songName);
}

void learnDancePattern() {
    srand(time(NULL));

    int patternCount = loadPatterns("dance_pattern.txt");
    if (patternCount == 0) return;

    int correct = 0;
    for (int i = 0; i < 4; i++) {
        int idx = rand() % patternCount;
        char clue[20];
        getRandomClue(patterns[idx].description, clue);

        printf("\nPattern Description: %s\n", patterns[idx].description);
        printf("Clue: %s\n", clue);

        char answer[50];
        printf("Pattern Name: ");
        scanf(" %[^\n]", answer);

        if (strcmp(answer, patterns[idx].name) == 0) {
            printf("Correct!\n");
            correct++;
        } else {
            printf("Wrong! Correct was: %s\n", patterns[idx].name);
        }
    }

    if (correct < 3) {
        printf("\nLess than 3 correct. Returning to menu.\n");
        return;
    }

    int songCount = loadChoreoCSV("analyz_dance-pattern.csv");
    if (songCount == 0) {
        printf("Failed to load choreography analysis.\n");
        return;
    }

    displayAllTrees(songCount);
    sleep(3);
    system("clear");

    printf("\nSelect a song to test (1-%d), or 0 to skip: ", songCount);
    int choice;
    scanf("%d", &choice);
    if (choice >= 1 && choice <= songCount) {
        playPatternGame(songs[choice - 1]);
    } else {
        printf("Skipped bonus game.\n");
    }
}

// ===== Menu System =====

void danceTrainingMenu() {
    int choice;
    while (1) {
        printf("\n[II. Training > 5. Dance Training]\n");
        printf("1. Learn Basic Dance Steps\n");
        printf("2. Choreography Patterns\n");
        printf("0. Back\n");
        printf("Select > ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                learnDanceStep();
                break;
            case 2:
                learnDancePattern();
                break;
            case 0:
                return;
            default:
                printf("Invalid.\n");
        }
    }
}

void trainingMenu() {
    int choice;
    while (1) {
        printf("\n[II. Training Menu]\n");
        printf("1. Health\n");
        printf("2. Self-Management\n");
        printf("3. Mentoring\n");
        printf("4. Vocal Training\n");
        printf("5. Dance Training\n");
        printf("0. Exit\n");
        printf("Select > ");
        scanf("%d", &choice);

        switch (choice) {
            case 5:
                danceTrainingMenu();
                break;
            case 0:
                return;
            default:
                printf("Under development or invalid.\n");
        }
    }
}

int main() {
    trainingMenu();
    return 0;
}
