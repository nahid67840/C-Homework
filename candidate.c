#include <stdio.h>
#include <stdlib.h>

#define NUM_CANDIDATES 6
#define NUM_FIELDS 11

// Function to calculate age from DOB
int calculate_age(int year, int month, int day) {
    int current_year = 2025, current_month = 4, current_day = 8;
    int age = current_year - year;
    if (month > current_month || (month == current_month && day > current_day)) {
        age--;
    }
    return age;
}

// Convert TOPIK level to string
const char* topik_level_str(int level) {
    if (level == 0) return "Native";
    static char buf[10];
    sprintf(buf, "Level %d", level);
    return buf;
}

int main() {
    char member_info[NUM_CANDIDATES][NUM_FIELDS][200];

    // Define six one-dimensional candidate arrays
    char candidate01[NUM_FIELDS][200];
    char candidate02[NUM_FIELDS][200];
    char candidate03[NUM_FIELDS][200];
    char candidate04[NUM_FIELDS][200];
    char candidate05[NUM_FIELDS][200];
    char candidate06[NUM_FIELDS][200];

    // Group name input via getchar
    char group_name[100];
    int ch, idx = 0;

    printf("Enter the audition group name: ");
    while ((ch = getchar()) != '\n' && ch != EOF && idx < 99) {
        group_name[idx++] = ch;
    }
    group_name[idx] = '\0';

    // Clear buffer if extra input exists
    if (ch != '\n') while (getchar() != '\n');

    printf("\n####################################\n");
    printf("[%s] Audition Candidate Data Entry\n", group_name);
    printf("####################################\n");

    // Input candidate data
    int i = 0;
    while (i < NUM_CANDIDATES) {
        printf("Entering information for candidate #%d.\n", i + 1);
        printf("---------------------------------\n");

        printf("1. Name: ");
        fgets(member_info[i][0], 200, stdin);

        printf("2. Date of Birth (YYYY/MM/DD format): ");
        fgets(member_info[i][1], 200, stdin);

        printf("3. Gender (F/M): ");
        fgets(member_info[i][2], 200, stdin);

        printf("4. Email: ");
        fgets(member_info[i][3], 200, stdin);

        printf("5. Nationality: ");
        fgets(member_info[i][4], 200, stdin);

        printf("6. BMI: ");
        fgets(member_info[i][5], 200, stdin);

        printf("7. Primary Skill: ");
        fgets(member_info[i][6], 200, stdin);

        printf("8. Secondary Skill: ");
        fgets(member_info[i][7], 200, stdin);

        printf("9. Korean Proficiency Level (TOPIK): ");
        fgets(member_info[i][8], 200, stdin);

        printf("10. MBTI: ");
        fgets(member_info[i][9], 200, stdin);

        printf("11. Introduction: ");
        fgets(member_info[i][10], 200, stdin);

        // Trim newlines
        for (int j = 0; j < NUM_FIELDS; j++) {
            int len = 0;
            while (member_info[i][j][len] != '\0') {
                if (member_info[i][j][len] == '\n') {
                    member_info[i][j][len] = '\0';
                    break;
                }
                len++;
            }
        }

        printf("=================================\n");
        i++;
    }

    // Copy data to the six one-dimensional arrays (as per constraint)
    for (int j = 0; j < NUM_FIELDS; j++) {
        sprintf(candidate01[j], "%s", member_info[0][j]);
        sprintf(candidate02[j], "%s", member_info[1][j]);
        sprintf(candidate03[j], "%s", member_info[2][j]);
        sprintf(candidate04[j], "%s", member_info[3][j]);
        sprintf(candidate05[j], "%s", member_info[4][j]);
        sprintf(candidate06[j], "%s", member_info[5][j]);
    }

    // Display output
    printf("\n####################################\n");
    printf("[%s] Audition Candidate Data Review\n", group_name);
    printf("####################################\n");
    printf("===========================================================================================================\n");
    printf("Name (Age)        | DOB       | Gender | Email                | Nationality  | BMI  | Primary | Secondary       | TOPIK   | MBTI  |\n");
    printf("===========================================================================================================\n");

    for (i = 0; i < NUM_CANDIDATES; i++) {
        int y, m, d;
        sscanf(member_info[i][1], "%d/%d/%d", &y, &m, &d);
        int age = calculate_age(y, m, d);

        printf("%-16s (%2d) | %04d%02d%02d | %-6s | %-20s | %-12s | %-4s | %-8s | %-15s | %-7s | %-5s |\n",
               member_info[i][0], age, y, m, d,
               member_info[i][2],
               member_info[i][3],
               member_info[i][4],
               member_info[i][5],
               member_info[i][6],
               member_info[i][7],
               topik_level_str(atoi(member_info[i][8])),
               member_info[i][9]);

        printf("---------------------------------------------------------------------------------------------\n");
        printf("%s\n", member_info[i][10]);
        printf("---------------------------------------------------------------------------------------------\n");
    }

    return 0;
}
