#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TITLE 100
#define MAX_DATE 11
#define MAX_FIELD 50
#define MAX_QUA_LEN 100
#define MAX_NAME 20
#define MAX_API 100
#define SOCIAL_COUNT 5

typedef struct {
    int id;
    char title[MAX_TITLE];
    char postingDate[MAX_DATE];
    char deadline[MAX_DATE];
    int numHires;
    char jobField[MAX_FIELD];
    char **qualifications;
    int qualiCount;
} JobAd;

typedef struct {
    char name[MAX_NAME];
    char apiKey[MAX_API];
    char apiUrl[MAX_API];
} SocialNetwork;

typedef struct Node {
    JobAd *job;
    struct Node *next;
} Node;

SocialNetwork socialNetworks[SOCIAL_COUNT] = {
    {"Facebook", "FB123KEY", "https://api.facebook.com/post"},
    {"Instagram", "IG456KEY", "https://api.instagram.com/post"},
    {"Thread", "TH789KEY", "https://api.threads.com/post"},
    {"LinkedIn", "LI101KEY", "https://api.linkedin.com/post"},
    {"X", "X202KEY", "https://api.x.com/post"}
};

int currentId = 1;
Node *jobList = NULL;
Node *expiredList = NULL;

// Helper
int compareDates(const char *date1, const char *date2) {
    return strcmp(date1, date2);
}

char* getCurrentDate() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    static char buffer[11];
    sprintf(buffer, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    return buffer;
}

void freeJob(JobAd *job) {
    for (int i = 0; i < job->qualiCount; i++) {
        free(job->qualifications[i]);
    }
    free(job->qualifications);
    free(job);
}

// Menu: Create job posting
void createJobPosting() {
    JobAd *job = malloc(sizeof(JobAd));
    job->id = currentId++;

    printf("Enter Job Title (max %d chars): ", MAX_TITLE - 1);
    scanf(" %[^\n]", job->title);
    
    printf("Enter Posting Date (YYYY-MM-DD): ");
    scanf(" %s", job->postingDate);

    printf("Enter Deadline (YYYY-MM-DD): ");
    scanf(" %s", job->deadline);

    printf("Enter Number of Hires: ");
    scanf("%d", &job->numHires);

    printf("Enter Job Field (max %d chars): ", MAX_FIELD - 1);
    scanf(" %[^\n]", job->jobField);

    printf("Enter number of qualifications: ");
    scanf("%d", &job->qualiCount);
    getchar();

    job->qualifications = malloc(job->qualiCount * sizeof(char*));
    for (int i = 0; i < job->qualiCount; i++) {
        job->qualifications[i] = malloc(MAX_QUA_LEN);
        printf("Enter qualification %d: ", i + 1);
        fgets(job->qualifications[i], MAX_QUA_LEN, stdin);
        job->qualifications[i][strcspn(job->qualifications[i], "\n")] = 0;
    }

    Node *newNode = malloc(sizeof(Node));
    newNode->job = job;
    newNode->next = jobList;
    jobList = newNode;

    printf("Job Posting Created Successfully!\n\n");
}

// Menu: View job postings
void viewJobPostings(int expired) {
    Node *curr = expired ? expiredList : jobList;
    if (!curr && !expired) {
        printf("No active job postings found. Please create one.\n");
        createJobPosting();
        return;
    }

    char currentDate[MAX_DATE];
    strcpy(currentDate, getCurrentDate());

    Node *ptr = curr;
    while (ptr) {
        if ((expired && compareDates(ptr->job->deadline, currentDate) < 0) ||
            (!expired && compareDates(ptr->job->deadline, currentDate) >= 0)) {
            printf("ID: %d | Title: %s | Deadline: %s\n", ptr->job->id, ptr->job->title, ptr->job->deadline);
        }
        ptr = ptr->next;
    }

    int selectedId;
    printf("Enter ID to view details (0 to cancel): ");
    scanf("%d", &selectedId);

    if (selectedId == 0) return;

    ptr = curr;
    while (ptr) {
        if (ptr->job->id == selectedId) {
            JobAd *j = ptr->job;
            printf("\n--- Job Details ---\n");
            printf("Title: %s\nPosting Date: %s\nDeadline: %s\nHires: %d\nField: %s\n",
                   j->title, j->postingDate, j->deadline, j->numHires, j->jobField);
            for (int i = 0; i < j->qualiCount; i++) {
                printf("Qualification %d: %s\n", i + 1, j->qualifications[i]);
            }
            printf("\n");
            break;
        }
        ptr = ptr->next;
    }
}

// Menu: Post on social media (Stub)
void postOnSocialNetwork() {
    if (!jobList) {
        printf("No job to post. Create one first.\n");
        return;
    }

    printf("Select a social network:\n");
    for (int i = 0; i < SOCIAL_COUNT; i++) {
        printf("%d. %s\n", i + 1, socialNetworks[i].name);
    }

    int choice;
    scanf("%d", &choice);
    if (choice < 1 || choice > SOCIAL_COUNT) return;

    int postId;
    printf("Enter Job ID to post: ");
    scanf("%d", &postId);

    Node *ptr = jobList;
    while (ptr) {
        if (ptr->job->id == postId) {
            printf("Posting to %s using API Key: %s\nURL: %s\n...Posted Successfully!\n\n",
                   socialNetworks[choice - 1].name,
                   socialNetworks[choice - 1].apiKey,
                   socialNetworks[choice - 1].apiUrl);
            return;
        }
        ptr = ptr->next;
    }

    printf("Job ID not found.\n");
}

// Filtering expired jobs (BONUS)
void updateExpiredJobs() {
    char currentDate[MAX_DATE];
    strcpy(currentDate, getCurrentDate());

    Node *prev = NULL;
    Node *curr = jobList;
    while (curr) {
        if (compareDates(curr->job->deadline, currentDate) < 0) {
            // move to expiredList
            Node *expired = curr;
            if (prev)
                prev->next = curr->next;
            else
                jobList = curr->next;

            curr = curr->next;

            expired->next = expiredList;
            expiredList = expired;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}

// Main Function
void findSpecialist() {
    int choice;
    while (1) {
        updateExpiredJobs();
        printf("\n[II. Training > 6. Visual & Image Training > A. Finding People]\n");
        printf("1. Create Job Posting\n2. View Job Postings\n3. Post on Social Networks\n4. View Expired Postings\n5. Exit\n");
        printf("Choose option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createJobPosting(); break;
            case 2: viewJobPostings(0); break;
            case 3: postOnSocialNetwork(); break;
            case 4: viewJobPostings(1); break;
            case 5: return;
            default: printf("Invalid choice.\n");
        }
    }
}

// Entry
int main() {
    findSpecialist();
    // Free memory
    Node *ptr;
    while (jobList) {
        ptr = jobList;
        jobList = jobList->next;
        freeJob(ptr->job);
        free(ptr);
    }
    while (expiredList) {
        ptr = expiredList;
        expiredList = expiredList->next;
        freeJob(ptr->job);
        free(ptr);
    }
    return 0;
}
