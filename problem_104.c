#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_JUDGES 100
#define MAX_STRING 512

char judges_array[MAX_JUDGES][MAX_STRING];

int validate_input(char *input) {
    int count = 0;
    for (int i = 0; input[i]; i++) {
        if (input[i] == ',') count++;
    }
    return (count == 6); // 7 fields mean 6 commas
}

void parse_and_display(char *input, int index) {
    char *token;
    char temp[MAX_STRING];
    strcpy(temp, input);
    printf("[Judge %d]\n", index + 1);
    
    token = strtok(temp, ",");
    printf("Name: %s\n", token);

    token = strtok(NULL, ",");
    printf("Gender: %s\n", token);

    token = strtok(NULL, ",");
    printf("Affiliation: %s\n", token);

    token = strtok(NULL, ",");
    printf("Title: %s\n", token);

    token = strtok(NULL, ",");
    printf("Expertise: %s\n", token);

    token = strtok(NULL, ",");
    printf("Email: %s\n", token);

    token = strtok(NULL, ",");
    printf("Phone: %s\n", token);
    printf("-----------------------------------\n");
}

int main() {
    char project_name[100];
    int total_judges = 0;
    int filled = 0;
    char input[MAX_STRING];

    printf("####################################\n");
    printf("#      Judge List Data Entry      #\n");
    printf("####################################\n");

    printf("Participating Project: ");
    fgets(project_name, sizeof(project_name), stdin);
    project_name[strcspn(project_name, "\n")] = 0; // remove newline

    printf("Total Number of Judges: ");
    scanf("%d", &total_judges);
    getchar(); // clear newline

    printf("Number of Selected Members: 4\n");
    printf("++++++++++++++++++++++++++++++++++++\n");
    printf("Starting to input information for %d judges.\n", total_judges);
    printf("++++++++++++++++++++++++++++++++++++\n");

    while (filled < total_judges) {
        printf("Judge %d: ", filled + 1);
        fgets(input, MAX_STRING, stdin);
        input[strcspn(input, "\n")] = 0; // remove newline

        if (!validate_input(input)) {
            printf("The input items are incorrect. Please enter them again.\n");
            continue;
        }
        strcpy(judges_array[filled], input);
        filled++;
    }

    printf("++++++++++++++++++++++++++++++++++++\n");
    printf("Judge information entry is complete.\n");
    printf("++++++++++++++++++++++++++++++++++++\n");
    printf("\"Should we check the judge information?\" ");

    char choice;
    scanf(" %c", &choice);

    if (choice == 'Y') {
        printf("%s Should we check the judge information? Y\n", project_name);
        printf("####################################\n");
        printf("#        Display Judge Data        #\n");
        printf("####################################\n");
        for (int i = 0; i < total_judges; i++) {
            parse_and_display(judges_array[i], i);
        }
    }

    return 0;
}
