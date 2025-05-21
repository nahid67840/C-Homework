#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 50
#define MAX_GROUP_LEN 50
#define MAX_MEMBERS 100
#define MAX_ATTEMPTS 3

typedef struct {
    int tempo;
    char rhythm[5];
} TempoRhythm;

typedef struct {
    char traineeName[MAX_NAME_LEN];
    char testDate[11];
    int targetBeats;
    double elapsedSeconds;
} BeatPracticeRecord;

typedef struct {
    char groupName[MAX_GROUP_LEN];
    char members[MAX_MEMBERS][MAX_NAME_LEN];
    int memberCount;
} TraineeGroup;

static const TempoRhythm tempoRhythms[] = {
    {60, "2/4"},
    {60, "3/4"},
    {60, "4/4"},
    {60, "6/8"},
    {80, "2/4"},
    {80, "3/4"},
    {80, "4/4"},
    {80, "6/8"},
    {120, "2/4"},
    {120, "3/4"},
    {120, "4/4"},
    {120, "6/8"}
};
static const int tempoRhythmCount = sizeof(tempoRhythms) / sizeof(TempoRhythm);

static int attemptsToday = 0;

void getCurrentDateTime(char *buffer, int bufsize) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(buffer, bufsize, "%02d%02d%02d%02d%02d", 
        t->tm_year % 100, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
}

int loadTraineesFromCSV(const char *filename, TraineeGroup *groups, int *groupCount) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    char line[256];
    *groupCount = 0;

    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ",");
        if (!token) continue;
        char *groupName = token;

        int grpIdx = -1;
        for (int i = 0; i < *groupCount; i++) {
            if (strcmp(groups[i].groupName, groupName) == 0) {
                grpIdx = i;
                break;
            }
        }
        if (grpIdx == -1) {
            grpIdx = *groupCount;
            strncpy(groups[grpIdx].groupName, groupName, MAX_GROUP_LEN - 1);
            groups[grpIdx].memberCount = 0;
            (*groupCount)++;
        }
        token = strtok(NULL, ",\r\n");
        if (token && groups[grpIdx].memberCount < MAX_MEMBERS) {
            strncpy(groups[grpIdx].members[groups[grpIdx].memberCount], token, MAX_NAME_LEN - 1);
            groups[grpIdx].memberCount++;
        }
    }

    fclose(fp);
    return 1;
}

int findMemberInGroup(const TraineeGroup *group, const char *name) {
    for (int i = 0; i < group->memberCount; i++) {
        if (strcmp(group->members[i], name) == 0)
            return 1;
    }
    return 0;
}

int inputTrainee(const TraineeGroup *groups, int groupCount, char *traineeName) {
    char groupName[MAX_GROUP_LEN];
    printf("Enter trainee group name: ");
    if (scanf("%49s", groupName) != 1) return 0;

    int grpIdx = -1;
    for (int i = 0; i < groupCount; i++) {
        if (strcmp(groups[i].groupName, groupName) == 0) {
            grpIdx = i;
            break;
        }
    }
    if (grpIdx == -1) {
        printf("Group not found.\n");
        return 0;
    }

    printf("Members in %s:\n", groupName);
    for (int i = 0; i < groups[grpIdx].memberCount; i++) {
        printf("  %s\n", groups[grpIdx].members[i]);
    }

    printf("Enter trainee name: ");
    if (scanf("%49s", traineeName) != 1) return 0;

    if (!findMemberInGroup(&groups[grpIdx], traineeName)) {
        printf("Trainee not found in group.\n");
        return 0;
    }

    return 1;
}

void writeFile(const BeatPracticeRecord *record) {
    FILE *fp = fopen("beat_test_result.txt", "a");
    if (!fp) {
        printf("Error opening file for writing.\n");
        return;
    }
    fprintf(fp, "%s,%s,%d,%.2f\n",
        record->traineeName,
        record->testDate,
        record->targetBeats,
        record->elapsedSeconds);
    fclose(fp);
}

void printResult(const BeatPracticeRecord *record) {
    double expectedTime = 60.0;
    double diff = record->elapsedSeconds - expectedTime;

    printf("Test completed.\n");
    printf("Elapsed time: %.2f seconds\n", record->elapsedSeconds);
    if (diff < 0)
        printf("You were %.2f seconds too fast.\n", -diff);
    else if (diff > 0)
        printf("You were %.2f seconds too slow.\n", diff);
    else
        printf("Perfect timing!\n");
}

void testBeat(const char *traineeName) {
    if (attemptsToday >= MAX_ATTEMPTS) {
        printf("You have reached the maximum number of attempts (3) for today.\n");
        return;
    }

    int idx = rand() % tempoRhythmCount;
    TempoRhythm tr = tempoRhythms[idx];
    int beatsPerMeasure = 0;
    if (strcmp(tr.rhythm, "2/4") == 0) beatsPerMeasure = 2;
    else if (strcmp(tr.rhythm, "3/4") == 0) beatsPerMeasure = 3;
    else if (strcmp(tr.rhythm, "4/4") == 0) beatsPerMeasure = 4;
    else if (strcmp(tr.rhythm, "6/8") == 0) beatsPerMeasure = 6;

    int targetBeats = tr.tempo * 60 / 60;

    printf("Tempo: %d BPM\n", tr.tempo);
    printf("Rhythm: %s\n", tr.rhythm);
    printf("Target beats to count: %d\n", targetBeats);
    printf("Press Enter to start the test...");
    while (getchar() != '\n');
    getchar();

    printf("Start counting beats silently for 60 seconds.\n");
    printf("Press Enter when you finish counting the target beats.\n");
    clock_t start = clock();
    while (getchar() != '\n');
    clock_t end = clock();

    double elapsedSeconds = (double)(end - start) / CLOCKS_PER_SEC;

    BeatPracticeRecord record;
    strncpy(record.traineeName, traineeName, MAX_NAME_LEN - 1);
    getCurrentDateTime(record.testDate, sizeof(record.testDate));
    record.targetBeats = targetBeats;
    record.elapsedSeconds = elapsedSeconds;

    writeFile(&record);
    printResult(&record);

    attemptsToday++;
}

void improveSenseOfBeat() {
    static TraineeGroup groups[MAX_MEMBERS];
    static int groupCount = 0;
    static int groupsLoaded = 0;

    if (!groupsLoaded) {
        if (!loadTraineesFromCSV("newtrainees.csv", groups, &groupCount)) {
            printf("Failed to load trainee groups.\n");
            return;
        }
        groupsLoaded = 1;
    }

    char traineeName[MAX_NAME_LEN];
    if (!inputTrainee(groups, groupCount, traineeName)) {
        printf("Invalid trainee input. Exiting.\n");
        return;
    }

    testBeat(traineeName);
}

int main() {
    srand((unsigned int)time(NULL));
    int choice;
    do {
        printf("=== Magrathea Training ===\n");
        printf("1. Improve Sense of Beat (Team Dance)\n");
        printf("0. Exit\n");
        printf("Select menu: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch(choice) {
            case 1:
                improveSenseOfBeat();
                break;
            case 0:
                printf("Exiting.\n");
                break;
            default:
                printf("Invalid option.\n");
        }
    } while (choice != 0);

    return 0;
}
