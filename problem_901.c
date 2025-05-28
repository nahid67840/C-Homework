// stage8.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stage8.h"

Artist *artists = NULL;
int artist_count = 0;

void xor_crypt(char *data, const char *key) {
    size_t key_len = strlen(key);
    for (size_t i = 0; data[i]; i++) {
        data[i] ^= key[i % key_len];
    }
}

void or_crypt(char *data, const char *key) {
    size_t key_len = strlen(key);
    for (size_t i = 0; data[i]; i++) {
        data[i] |= key[i % key_len];
    }
}

void and_crypt(char *data, const char *key) {
    size_t key_len = strlen(key);
    for (size_t i = 0; data[i]; i++) {
        data[i] &= key[i % key_len];
    }
}

void bitshift_crypt(char *data, const char *key) {
    size_t key_len = strlen(key);
    for (size_t i = 0; data[i]; i++) {
        unsigned char shift = key[i % key_len] % 8;
        data[i] = (data[i] << shift) | (data[i] >> (8 - shift));
    }
}

typedef void (*EncryptFunc)(char *, const char *);

void apply_crypt(char *data, const char *key, EncryptFunc func) {
    func(data, key);
}

void input_artist(Artist *a) {
    printf("Name: "); fgets(a->name, MAX_STR, stdin); a->name[strcspn(a->name, "\n")] = 0;
    printf("Nickname: "); fgets(a->nickname, MAX_STR, stdin); a->nickname[strcspn(a->nickname, "\n")] = 0;
    printf("DOB (YYYY-MM-DD): "); fgets(a->dob, 11, stdin); getchar();
    printf("Gender: "); fgets(a->gender, 10, stdin); a->gender[strcspn(a->gender, "\n")] = 0;
    printf("Education: "); fgets(a->education, MAX_STR, stdin); a->education[strcspn(a->education, "\n")] = 0;
    printf("Phone Number: "); fgets(a->phone, MAX_STR, stdin); a->phone[strcspn(a->phone, "\n")] = 0;
    printf("Email: "); fgets(a->email, MAX_STR, stdin); a->email[strcspn(a->email, "\n")] = 0;
    printf("Nationality: "); fgets(a->nationality, MAX_STR, stdin); a->nationality[strcspn(a->nationality, "\n")] = 0;
    printf("Height (cm): "); scanf("%f", &a->height); getchar();
    printf("Weight (kg): "); scanf("%f", &a->weight); getchar();
    printf("Blood Type: "); fgets(a->blood_type, 5, stdin); a->blood_type[strcspn(a->blood_type, "\n")] = 0;
    printf("Allergies: "); fgets(a->allergies, MAX_STR, stdin); a->allergies[strcspn(a->allergies, "\n")] = 0;
    printf("SNS: "); fgets(a->sns, MAX_STR, stdin); a->sns[strcspn(a->sns, "\n")] = 0;
}

void save_artists(const char *filename, const char *key, EncryptFunc func) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("Cannot open file for writing.\n");
        return;
    }
    fwrite(&artist_count, sizeof(int), 1, fp);
    for (int i = 0; i < artist_count; i++) {
        Artist a = artists[i];
        func(a.phone, key);
        func(a.email, key);
        func(a.allergies, key);
        fwrite(&a, sizeof(Artist), 1, fp);
    }
    fclose(fp);
}

void load_artists(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("No saved artist data found.\n");
        return;
    }
    fread(&artist_count, sizeof(int), 1, fp);
    artists = malloc(sizeof(Artist) * artist_count);
    if (!artists) {
        printf("Memory allocation failed.\n");
        fclose(fp);
        return;
    }
    for (int i = 0; i < artist_count; i++) {
        fread(&artists[i], sizeof(Artist), 1, fp);
    }
    fclose(fp);
}

void print_artist_brief() {
    printf("\nArtist List:\n");
    for (int i = 0; i < artist_count; i++) {
        printf("%d. %s (%s)\n", i + 1, artists[i].name, artists[i].nickname);
    }
}

void print_artist_full(Artist *a, int decrypted) {
    printf("\nName: %s\n", a->name);
    printf("Nickname: %s\n", a->nickname);
    printf("DOB: %s\n", a->dob);
    printf("Gender: %s\n", a->gender);
    printf("Education: %s\n", a->education);
    printf("Phone: %s\n", a->phone);
    printf("Email: %s\n", a->email);
    printf("Nationality: %s\n", a->nationality);
    printf("Height: %.2f cm\n", a->height);
    printf("Weight: %.2f kg\n", a->weight);
    printf("Blood Type: %s\n", a->blood_type);
    printf("Allergies: %s\n", a->allergies);
    printf("SNS: %s\n", a->sns);
    if (!decrypted)
        printf("[Encrypted fields shown]\n");
}

EncryptFunc select_encryption_method() {
    printf("Select encryption method:\n");
    printf("1. XOR\n");
    printf("2. OR\n");
    printf("3. AND\n");
    printf("4. Bit Shift\n");
    printf("Choice: ");
    int choice;
    scanf("%d", &choice);
    getchar();

    switch (choice) {
        case 1: return xor_crypt;
        case 2: return or_crypt;
        case 3: return and_crypt;
        case 4: return bitshift_crypt;
        default:
            printf("Invalid choice, defaulting to XOR.\n");
            return xor_crypt;
    }
}

void protectMyData() {
    const char filename[] = "artistdata.dat";
    char key[100];
    EncryptFunc crypt_func;

    crypt_func = select_encryption_method();

    printf("Enter encryption key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0;

    printf("Enter number of artists: ");
    scanf("%d", &artist_count);
    getchar();

    artists = malloc(sizeof(Artist) * artist_count);
    if (!artists) {
        printf("Memory allocation failed.\n");
        return;
    }
    for (int i = 0; i < artist_count; i++) {
        printf("\nArtist #%d\n", i + 1);
        input_artist(&artists[i]);
    }

    save_artists(filename, key, crypt_func);
    load_artists(filename);

    while (1) {
        print_artist_brief();
        printf("Select artist number to view details (0 to exit): ");
        int sel;
        scanf("%d", &sel);
        getchar();
        if (sel == 0) break;
        if (sel < 1 || sel > artist_count) {
            printf("Invalid selection.\n");
            continue;
        }
        Artist *a = &artists[sel - 1];
        print_artist_full(a, 0);

        printf("Decrypt encrypted fields? (y/n): ");
        char ch = getchar();
        getchar();
        if (ch == 'y' || ch == 'Y') {
            crypt_func(a->phone, key);
            crypt_func(a->email, key);
            crypt_func(a->allergies, key);
            print_artist_full(a, 1);
            crypt_func(a->phone, key);
            crypt_func(a->email, key);
            crypt_func(a->allergies, key);
        }
    }

    free(artists);
    artists = NULL;
}
