#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_MEMBERS 10
#define MAX_ROUTINE 10
#define MAX_NAME 20
#define MAX_SUBJECTS 5
#define MAX_WORDS 10
#define MAX_WORD_LENGTH 6
#define MAX_SENTENCE_LENGTH 101

typedef struct {
    char name[MAX_NAME];
    char nickname[MAX_NAME];
    int age;
    char gender;
    int grade;
} Student;

typedef struct {
    char nickname[MAX_NAME];
    float grades[MAX_SUBJECTS];
    float weights[MAX_SUBJECTS];
    float average;
} SubjectInfo;

Student students[MAX_MEMBERS];
SubjectInfo subjects[MAX_MEMBERS];
int studentCount = 0;

char* subjectsList[MAX_SUBJECTS] = {"English", "Math", "Korean", "Social", "Science"};

void inputCommonLearningInfo() {
    printf("Enter number of students (max %d): ", MAX_MEMBERS);
    scanf("%d", &studentCount);
    for (int i = 0; i < studentCount; i++) {
        printf("Student %d Name: ", i + 1); scanf("%s", students[i].name);
        printf("Nickname: "); scanf("%s", students[i].nickname);
        printf("Age: "); scanf("%d", &students[i].age);
        printf("Gender (M/F): "); scanf(" %c", &students[i].gender);
        students[i].grade = students[i].age - 6;
        strcpy(subjects[i].nickname, students[i].nickname);
        float totalWeightedScore = 0, totalWeight = 0;
        for (int j = 0; j < MAX_SUBJECTS; j++) {
            printf("%s Score: ", subjectsList[j]);
            scanf("%f", &subjects[i].grades[j]);
            printf("%s Weight (0.0 ~ 1.0): ", subjectsList[j]);
            scanf("%f", &subjects[i].weights[j]);
            totalWeightedScore += subjects[i].grades[j] * subjects[i].weights[j];
            totalWeight += subjects[i].weights[j];
        }
        if (totalWeight != 1.0f) {
            printf("Total weights must sum to 1.0. Please re-enter.\n");
            i--; continue;
        }
        subjects[i].average = (float)((int)(totalWeightedScore * 10 + 0.5)) / 10.0;
    }
}

void printStudentInfo() {
    for (int i = studentCount - 1; i >= 0; i--) {
        printf("Name: %s, Nickname: %s, Age: %d, Gender: %c, Grade: %d\n",
            students[i].name, students[i].nickname, students[i].age, students[i].gender, students[i].grade);
    }
}

int compareAvg(const void* a, const void* b) {
    SubjectInfo* s1 = (SubjectInfo*)a;
    SubjectInfo* s2 = (SubjectInfo*)b;
    return (s1->average > s2->average) - (s1->average < s2->average);
}

void printAscendAvgGrade() {
    SubjectInfo temp[MAX_MEMBERS];
    memcpy(temp, subjects, sizeof(subjects));
    qsort(temp, studentCount, sizeof(SubjectInfo), compareAvg);
    for (int i = 0; i < studentCount; i++) {
        printf("Nickname: %s, Average: %.1f\n", temp[i].nickname, temp[i].average);
    }
}

void printDescendHighWeightGrade() {
    for (int i = 0; i < studentCount; i++) {
        int highIndex = 0;
        for (int j = 1; j < MAX_SUBJECTS; j++) {
            if (subjects[i].weights[j] > subjects[i].weights[highIndex])
                highIndex = j;
        }
        printf("Nickname: %s, Subject: %s, Score: %.1f\n",
               subjects[i].nickname, subjectsList[highIndex], subjects[i].grades[highIndex]);
    }
}

void printExcludeMaxMinGrade() {
    char subjectName[MAX_NAME];
    printf("Enter subject name (English, Math, Korean, Social, Science): ");
    scanf("%s", subjectName);
    int subjectIdx = -1;
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        if (strcmp(subjectsList[i], subjectName) == 0) {
            subjectIdx = i; break;
        }
    }
    if (subjectIdx == -1) {
        printf("Invalid subject.\n"); return;
    }
    float max = -1, min = 101;
    for (int i = 0; i < studentCount; i++) {
        if (subjects[i].grades[subjectIdx] > max) max = subjects[i].grades[subjectIdx];
        if (subjects[i].grades[subjectIdx] < min) min = subjects[i].grades[subjectIdx];
    }
    for (int i = 0; i < studentCount; i++) {
        float score = subjects[i].grades[subjectIdx];
        if (score == max || score == min) continue;
        printf("Nickname: %s, %s Score: %.1f\n", subjects[i].nickname, subjectName, score);
    }
}
void obfuscateName(char* name) {
    for (int i = 0; name[i] != '\0'; i++) {
        printf("%d ", name[i]);
    }
    printf("\n");
}

void reverseNickname(char* nickname) {
    int len = strlen(nickname);
    for (int i = len - 1; i >= 0; i--) {
        printf("%c", nickname[i]);
    }
    printf("\n");
}

void inquiryCommonLearningInfo() {
    char option;
    printf("Select option:\n");
    printf("A. Print all student info (desc)\n");
    printf("B. Print students by ascending average\n");
    printf("C. Print subject with highest weight grade\n");
    printf("D. Exclude max/min score for a subject\n");
    printf("E. Obfuscate names\n");
    scanf(" %c", &option);

    switch(option) {
        case 'A':
            printStudentInfo();
            break;
        case 'B':
            printAscendAvgGrade();
            break;
        case 'C':
            printDescendHighWeightGrade();
            break;
        case 'D':
            printExcludeMaxMinGrade();
            break;
        case 'E':
            for (int i = 0; i < studentCount; i++) {
                printf("Name: "); obfuscateName(students[i].name);
                printf("Nickname: "); reverseNickname(students[i].nickname);
            }
            break;
        default:
            printf("Invalid option.\n");
    }
}

int main() {
    char choice;
    while (1) {
        printf("\n== Magrathea Training Program ==\n");
        printf("[K] Input Common Learning Info\n");
        printf("[L] Inquiry Common Learning Info\n");
        printf("[Q] Quit\n");
        printf("Select: ");
        scanf(" %c", &choice);
        switch (choice) {
            case 'K':
                inputCommonLearningInfo();
                break;
            case 'L':
                inquiryCommonLearningInfo();
                break;
            case 'Q':
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid option. Try again.\n");
        }
    }
}
