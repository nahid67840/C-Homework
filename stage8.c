// stage8.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stage8.h"

Artist artists[MAX_ARTISTS];
int artistCount = 0;

// Encryption methods
void encryptData(char *dest, const char *src, char key, int method) {
    int i;
    for (i = 0; src[i] != '\0'; i++) {
        switch (method) {
            case 1: dest[i] = src[i] ^ key; break;        // XOR
            case 2: dest[i] = src[i] | key; break;        // OR
            case 3: dest[i] = src[i] & key; break;        // AND
            case 4: dest[i] = src[i] << 1; break;         // Bit Shift
            default: dest[i] = src[i];
        }
    }
    dest[i] = '\0';
}

void decryptData(char *dest, const char *src, char key, int method) {
    int i;
    for (i = 0; src[i] != '\0'; i++) {
        switch (method) {
            case 1: dest[i] = src[i] ^ key; break;        // XOR
            case 2: dest[i] = src[i] & (~key); break;     // OR reverse
            case 3: dest[i] = src[i] | (~key); break;     // AND reverse
            case 4: dest[i] = src[i] >> 1; break;         // Bit Shift reverse
            default: dest[i] = src[i];
        }
    }
    dest[i] = '\0';
}

// Input data
void inputArtistData(int method, char key) {
    printf("Enter number of artists: ");
    scanf("%d", &artistCount);
    getchar();

    for (int i = 0; i < artistCount; i++) {
        printf("\n-- Artist %d --\n", i + 1);
        printf("Name: "); fgets(artists[i].name, MAX_LEN, stdin);
        printf("Nickname: "); fgets(artists[i].nickname, MAX_LEN, stdin);
        printf("Date of Birth: "); fgets(artists[i].dob, MAX_LEN, stdin);
        printf("Gender: "); fgets(artists[i].gender, MAX_LEN, stdin);
        printf("Education: "); fgets(artists[i].education, MAX_LEN, stdin);

        char temp[MAX_LEN];
        printf("Phone: "); fgets(temp, MAX_LEN, stdin);
        encryptData(artists[i].phone_encrypted, temp, key, method);

        printf("Email: "); fgets(temp, MAX_LEN, stdin);
        encryptData(artists[i].email_encrypted, temp, key, method);

        printf("Nationality: "); fgets(artists[i].nationality, MAX_LEN, stdin);

        printf("Height (in cm): "); scanf("%f", &artists[i].height); getchar();
        printf("Weight (in kg): "); scanf("%f", &artists[i].weight); getchar();

        printf("Blood Type: "); fgets(artists[i].blood_type, MAX_LEN, stdin);

        printf("Allergies: "); fgets(temp, MAX_LEN, stdin);
        encryptData(artists[i].allergies_encrypted, temp, key, method);

        printf("SNS: "); fgets(artists[i].sns, MAX_LEN, stdin);
    }

    saveArtistsToFile("artist_data.txt");
}

// Save and Load
void saveArtistsToFile(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return;

    for (int i = 0; i < artistCount; i++) {
        fprintf(fp, "%s%s%s%s%s%s%s%s%.2f\n%.2f\n%s%s%s%s",
            artists[i].name,
            artists[i].nickname,
            artists[i].dob,
            artists[i].gender,
            artists[i].education,
            artists[i].phone_encrypted,
            artists[i].email_encrypted,
            artists[i].nationality,
            artists[i].height,
            artists[i].weight,
            artists[i].blood_type,
            artists[i].allergies_encrypted,
            artists[i].sns,
            "\n"
        );
    }

    fclose(fp);
}

void displayArtistList(int method, char key) {
    printf("\n--- Artist List ---\n");
    for (int i = 0; i < artistCount; i++) {
        printf("%d. %s (%s)\n", i + 1, artists[i].name, artists[i].nickname);
    }

    printf("Select artist number to view full info: ");
    int idx;
    scanf("%d", &idx); getchar();
    if (idx < 1 || idx > artistCount) return;

    Artist a = artists[idx - 1];
    printf("\nName: %sNickname: %sDOB: %sGender: %sEducation: %s",
           a.name, a.nickname, a.dob, a.gender, a.education);
    printf("Phone (encrypted): %s\n", a.phone_encrypted);
    printf("Email (encrypted): %s\n", a.email_encrypted);
    printf("Nationality: %s", a.nationality);
    printf("Height: %.2f cm\nWeight: %.2f kg\n", a.height, a.weight);
    printf("Blood Type: %s", a.blood_type);
    printf("Allergies (encrypted): %s\n", a.allergies_encrypted);
    printf("SNS: %s", a.sns);

    char choice;
    printf("\nDecrypt sensitive data? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        char newKey;
        printf("Enter encryption key: ");
        scanf(" %c", &newKey);

        char decPhone[MAX_LEN], decEmail[MAX_LEN], decAllergy[MAX_LEN];
        decryptData(decPhone, a.phone_encrypted, newKey, method);
        decryptData(decEmail, a.email_encrypted, newKey, method);
        decryptData(decAllergy, a.allergies_encrypted, newKey, method);

        printf("Phone: %sEmail: %sAllergies: %s\n", decPhone, decEmail, decAllergy);
    }
}

// Main function
void protectMyData() {
    int method;
    char key;

    printf("Select encryption method:\n");
    printf("1. XOR\n2. OR\n3. AND\n4. Bit Shift\nChoose (1-4): ");
    scanf("%d", &method);

    printf("Enter encryption key (char): ");
    scanf(" %c", &key);
    getchar();

    inputArtistData(method, key);
    displayArtistList(method, key);
}
