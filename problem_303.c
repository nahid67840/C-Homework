#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_INPUT_LENGTH 100
#define KEYWORD "specter"
#define BINARY_LENGTH 8
#define MAX_KEYWORD_LENGTH 8

typedef struct {
    char keyword[MAX_KEYWORD_LENGTH];
    char message[MAX_INPUT_LENGTH];
} EasterEgg;

EasterEgg arthurEasterEgg = {
    "specter",
    "I confess. After graduating from university, I was blinded by the arrogance of starting a startup and recklessly blocked my friends' paths. I painfully learned that when I am the only one convinced by my idea, it leads to disastrous results. The past Arthur was a ghost of dogmatism and stubbornness."
};

void displayMainMenu();
void displaySelfManagementMenu();
void find_easter_egg();
void convert_to_binary(char c, char *binary_str);
int isRightChar(char *binary_str, char *user_input);
int isEasterEgg(char *user_input);
void shuffleKeywordAndConvertToBinary(char *keyword);

int main() {
    char choice;
    
    while (1) {
        displayMainMenu();
        printf("Enter your choice (0, Q to quit): ");
        choice = getchar();
        getchar(); // consume newline character

        if (choice == '0' || choice == 'Q' || choice == 'q') {
            break;
        }

        switch (choice) {
            case '1':
                displaySelfManagementMenu();
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

void displayMainMenu() {
    printf("\n=== Main Menu ===\n");
    printf("1. Self-Management and Teamwork\n");
    printf("0. Exit\n");
    printf("Q. Quit\n");
}

void displaySelfManagementMenu() {
    printf("\n=== Self-Management and Teamwork ===\n");
    printf("Enter 'Arthur' to find the Easter Egg.\n");
    char input[MAX_INPUT_LENGTH];
    
    printf("Enter your input: ");
    fgets(input, MAX_INPUT_LENGTH, stdin);
    input[strcspn(input, "\n")] = 0; // Remove newline character
    
    if (strcmp(input, "Arthur") == 0) {
        find_easter_egg();
    } else {
        printf("No Easter Egg found.\n");
    }
}

void find_easter_egg() {
    printf("<<Arthur's Easter Egg>>\n");

    // Convert each letter of the keyword to 8-bit binary and reverse order
    char binary_values[KEYWORD_LENGTH][BINARY_LENGTH + 1];
    for (int i = 0; i < MAX_KEYWORD_LENGTH; i++) {
        convert_to_binary(KEYWORD[i], binary_values[i]);
    }

    printf("Please enter the characters corresponding to the reversed binary values:\n");
    
    // Get user input and validate
    char user_input[MAX_KEYWORD_LENGTH + 1];
    int correct = 0;
    while (correct < MAX_KEYWORD_LENGTH) {
        for (int i = MAX_KEYWORD_LENGTH - 1; i >= 0; i--) {
            printf("Enter character for binary %s: ", binary_values[i]);
            fgets(user_input, MAX_KEYWORD_LENGTH + 1, stdin);
            user_input[strcspn(user_input, "\n")] = 0; // Remove newline character

            if (isRightChar(binary_values[i], user_input)) {
                correct++;
            } else {
                printf("Incorrect character, please try again.\n");
                correct = 0;
                break;
            }
        }
    }

    // After the user enters all characters correctly, check the full word
    if (isEasterEgg(user_input)) {
        printf("##Easter Egg Discovered!$$\n");
        printf("%s\n", arthurEasterEgg.message);
    } else {
        printf("Incorrect word. Returning to the menu.\n");
    }
}

void convert_to_binary(char c, char *binary_str) {
    for (int i = BINARY_LENGTH - 1; i >= 0; i--) {
        binary_str[BINARY_LENGTH - 1 - i] = (c & (1 << i)) ? '1' : '0';
    }
    binary_str[BINARY_LENGTH] = '\0'; // Null-terminate the string
}

int isRightChar(char *binary_str, char *user_input) {
    int input_len = strlen(user_input);
    if (input_len != 1) {
        return 0; // Input must be a single character
    }

    // Convert user input character to binary
    char binary_user_input[BINARY_LENGTH + 1];
    convert_to_binary(user_input[0], binary_user_input);

    // Compare with the binary value
    return strcmp(binary_str, binary_user_input) == 0;
}

int isEasterEgg(char *user_input) {
    return strcmp(user_input, KEYWORD) == 0;
}

void shuffleKeywordAndConvertToBinary(char *keyword) {
    // Shuffle the keyword by collecting odd-indexed letters first, then even-indexed
    char shuffled[MAX_KEYWORD_LENGTH + 1];
    int j = 0;

    for (int i = 1; i < MAX_KEYWORD_LENGTH; i += 2) {
        shuffled[j++] = keyword[i];
    }
    for (int i = 0; i < MAX_KEYWORD_LENGTH; i += 2) {
        shuffled[j++] = keyword[i];
    }
    shuffled[MAX_KEYWORD_LENGTH] = '\0';

    // Convert each shuffled letter into 8-bit binary
    char binary_values[MAX_KEYWORD_LENGTH][BINARY_LENGTH + 1];
    for (int i = 0; i < MAX_KEYWORD_LENGTH; i++) {
        convert_to_binary(shuffled[i], binary_values[i]);
    }

    // Display binary values
    printf("Shuffled and converted binary values:\n");
    for (int i = 0; i < MAX_KEYWORD_LENGTH; i++) {
        printf("Character: %c, Binary: %s\n", shuffled[i], binary_values[i]);
    }
}
