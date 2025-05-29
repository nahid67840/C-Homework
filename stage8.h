// stage8.h

#ifndef STAGE8_H
#define STAGE8_H

#define MAX_ARTISTS 100
#define MAX_LEN 100

typedef struct {
    char name[MAX_LEN];
    char nickname[MAX_LEN];
    char dob[MAX_LEN];
    char gender[MAX_LEN];
    char education[MAX_LEN];
    char phone_encrypted[MAX_LEN];   // Encrypted
    char email_encrypted[MAX_LEN];   // Encrypted
    char nationality[MAX_LEN];
    float height;
    float weight;
    char blood_type[MAX_LEN];
    char allergies_encrypted[MAX_LEN]; // Encrypted
    char sns[MAX_LEN];
} Artist;

// Declare global variables
extern Artist artists[MAX_ARTISTS];
extern int artistCount;

// Function declarations
void protectMyData();
void encryptData(char *dest, const char *src, char key, int method);
void decryptData(char *dest, const char *src, char key, int method);
void inputArtistData(int method, char key);
void displayArtistList(int method, char key);
void saveArtistsToFile(const char *filename);
void loadArtistsFromFile(const char *filename);

#endif
