#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TITLE_LEN 50
#define MAX_DATE_LEN 11
#define MAX_FIELD_LEN 30
#define MAX_QUAL_LEN 100
#define MAX_SOCIAL_NAME 20
#define MAX_API_KEY 40
#define MAX_API_URL 100

typedef struct Qualification {
    char *qualification;
    struct Qualification *next;
} Qualification;

typedef struct JobDetail {
    int hires;
    char field[MAX_FIELD_LEN];
    Qualification *qualifications;
} JobDetail;

typedef struct JobPost {
    int id;
    char title[MAX_TITLE_LEN];
    char posting_date[MAX_DATE_LEN];
    char deadline[MAX_DATE_LEN];
    JobDetail detail;
    struct JobPost *next;
} JobPost;

typedef struct SocialNetwork {
    char name[MAX_SOCIAL_NAME];
    char api_key[MAX_API_KEY];
    char api_url[MAX_API_URL];
    struct SocialNetwork *next;
} SocialNetwork;

JobPost *job_list = NULL;
JobPost *expired_job_list = NULL;
SocialNetwork *social_list = NULL;
int job_id_counter = 1;

void free_qualifications(Qualification *head) {
    while (head) {
        Qualification *tmp = head;
        head = head->next;
        free(tmp);
    }
}

void free_jobs(JobPost *head) {
    while (head) {
        free_qualifications(head->detail.qualifications);
        JobPost *tmp = head;
        head = head->next;
        free(tmp);
    }
}

void free_socials(SocialNetwork *head) {
    while (head) {
        SocialNetwork *tmp = head;
        head = head->next;
        free(tmp);
    }
}

int date_compare(const char *d1, const char *d2) {
    return strcmp(d1, d2);
}

void move_expired_jobs(const char *current_date) {
    JobPost **cur = &job_list;
    while (*cur) {
        if (date_compare((*cur)->deadline, current_date) < 0) {
            JobPost *expired = *cur;
            *cur = expired->next;
            expired->next = expired_job_list;
            expired_job_list = expired;
        } else {
            cur = &(*cur)->next;
        }
    }
}

Qualification *input_qualifications() {
    Qualification *head = NULL, *tail = NULL;
    int n;
    printf("Enter number of qualifications: ");
    scanf("%d", &n);
    getchar();
    for (int i = 0; i < n; i++) {
        char buffer[MAX_QUAL_LEN];
        printf("Qualification %d: ", i + 1);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        Qualification *new_qual = malloc(sizeof(Qualification));
        new_qual->qualification = malloc(strlen(buffer) + 1);
        strcpy(new_qual->qualification, buffer);
        new_qual->next = NULL;
        if (!head) head = tail = new_qual;
        else {
            tail->next = new_qual;
            tail = new_qual;
        }
    }
    return head;
}

void print_qualifications(Qualification *head) {
    int i = 1;
    while (head) {
        printf("  %d. %s\n", i++, head->qualification);
        head = head->next;
    }
}

void create_job_posting() {
    JobPost *new_post = malloc(sizeof(JobPost));
    new_post->id = job_id_counter++;
    printf("Enter Job Title (max %d chars): ", MAX_TITLE_LEN - 1);
    fgets(new_post->title, MAX_TITLE_LEN, stdin);
    new_post->title[strcspn(new_post->title, "\n")] = 0;
    printf("Enter Posting Date (YYYY-MM-DD): ");
    fgets(new_post->posting_date, MAX_DATE_LEN, stdin);
    new_post->posting_date[strcspn(new_post->posting_date, "\n")] = 0;
    printf("Enter Deadline (YYYY-MM-DD): ");
    fgets(new_post->deadline, MAX_DATE_LEN, stdin);
    new_post->deadline[strcspn(new_post->deadline, "\n")] = 0;
    printf("Enter Number of Hires: ");
    scanf("%d", &new_post->detail.hires);
    getchar();
    printf("Enter Job Field (max %d chars): ", MAX_FIELD_LEN - 1);
    fgets(new_post->detail.field, MAX_FIELD_LEN, stdin);
    new_post->detail.field[strcspn(new_post->detail.field, "\n")] = 0;
    new_post->detail.qualifications = input_qualifications();
    new_post->next = job_list;
    job_list = new_post;
    printf("Job posting created with ID %d.\n", new_post->id);
}

void view_job_postings(JobPost *list, int show_expired) {
    if (!list) {
        if (!show_expired) {
            printf("No active job postings found. Redirecting to create job posting...\n");
            create_job_posting();
        } else {
            printf("No expired job postings found.\n");
        }
        return;
    }
    printf("%s Job Postings:\n", show_expired ? "Expired" : "Active");
    JobPost *cur = list;
    while (cur) {
        printf("ID: %d | Title: %s | Posting Date: %s | Deadline: %s\n",
               cur->id, cur->title, cur->posting_date, cur->deadline);
        cur = cur->next;
    }
    printf("Enter job ID to see details or 0 to return: ");
    int id;
    scanf("%d", &id);
    getchar();
    if (id == 0) return;
    cur = list;
    while (cur && cur->id != id) cur = cur->next;
    if (!cur) {
        printf("Job ID not found.\n");
        return;
    }
    printf("Job Details for ID %d:\n", cur->id);
    printf("Title: %s\n", cur->title);
    printf("Posting Date: %s\n", cur->posting_date);
    printf("Deadline: %s\n", cur->deadline);
    printf("Number of Hires: %d\n", cur->detail.hires);
    printf("Job Field: %s\n", cur->detail.field);
    printf("Qualifications:\n");
    print_qualifications(cur->detail.qualifications);
}

void post_on_social_networks() {
    if (!social_list) {
        printf("No social networks registered.\n");
        return;
    }
    printf("Select Social Network:\n");
    SocialNetwork *cur = social_list;
    int idx = 1;
    while (cur) {
        printf("%d. %s\n", idx++, cur->name);
        cur = cur->next;
    }
    int choice;
    scanf("%d", &choice);
    getchar();
    if (choice <= 0) return;
    cur = social_list;
    for (int i = 1; i < choice && cur; i++) cur = cur->next;
    if (!cur) {
        printf("Invalid social network selection.\n");
        return;
    }
    if (!job_list) {
        printf("No active job postings to post.\n");
        return;
    }
    printf("Enter Job ID to post: ");
    int job_id;
    scanf("%d", &job_id);
    getchar();
    JobPost *job = job_list;
    while (job && job->id != job_id) job = job->next;
    if (!job) {
        printf("Job ID not found.\n");
        return;
    }
    printf("Posting job ID %d on %s...\n", job->id, cur->name);
    printf("API Key: %s\nAPI URL: %s\n", cur->api_key, cur->api_url);
    printf("Post successful!\n");
}

void init_social_networks() {
    SocialNetwork *fb = malloc(sizeof(SocialNetwork));
    strcpy(fb->name, "Facebook");
    strcpy(fb->api_key, "FB_API_KEY");
    strcpy(fb->api_url, "https://api.facebook.com/post");
    fb->next = NULL;
    SocialNetwork *insta = malloc(sizeof(SocialNetwork));
    strcpy(insta->name, "Instagram");
    strcpy(insta->api_key, "IG_API_KEY");
    strcpy(insta->api_url, "https://api.instagram.com/post");
    insta->next = NULL;
    fb->next = insta;
    SocialNetwork *thread = malloc(sizeof(SocialNetwork));
    strcpy(thread->name, "Thread");
    strcpy(thread->api_key, "THREAD_API_KEY");
    strcpy(thread->api_url, "https://api.thread.com/post");
    thread->next = NULL;
    insta->next = thread;
    SocialNetwork *linkedin = malloc(sizeof(SocialNetwork));
    strcpy(linkedin->name, "LinkedIn");
    strcpy(linkedin->api_key, "LINKEDIN_API_KEY");
    strcpy(linkedin->api_url, "https://api.linkedin.com/post");
    linkedin->next = NULL;
    thread->next = linkedin;
    SocialNetwork *x = malloc(sizeof(SocialNetwork));
    strcpy(x->name, "X");
    strcpy(x->api_key, "X_API_KEY");
    strcpy(x->api_url, "https://api.x.com/post");
    x->next = NULL;
    linkedin->next = x;
    social_list = fb;
}

void findSpecialist() {
    init_social_networks();
    char current_date[11];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(current_date, sizeof(current_date), "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    move_expired_jobs(current_date);
    while (1) {
        printf("\n[II. Training > 6. Visual & Image Training > A. Finding People]\n");
        printf("1. Create Job Posting\n2. View Job Postings\n3. Post on Social Networks\n4. Exit\nSelect: ");
        int sel;
        scanf("%d", &sel);
        getchar();
        switch (sel) {
            case 1:
                create_job_posting();
                break;
            case 2:
                printf("1. View Active Job Postings\n2. View Expired Job Postings\nSelect: ");
                int subsel;
                scanf("%d", &subsel);
                getchar();
                if (subsel == 1) view_job_postings(job_list, 0);
                else if (subsel == 2) view_job_postings(expired_job_list, 1);
                break;
            case 3:
                post_on_social_networks();
                break;
            case 4:
                free_jobs(job_list);
                free_jobs(expired_job_list);
                free_socials(social_list);
                return;
            default:
                printf("Invalid selection.\n");
        }
    }
}
