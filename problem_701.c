#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 60
#define MAX_DETAIL_LEN 1024
#define CONTENT_FILENAME "perform_stage.txt"

typedef struct Subjects {
    char name[MAX_NAME_LEN];
    char *detail;
    struct Subjects *next;
} Subjects;

Subjects *head = NULL;

Subjects *create_subject_node(const char *name) {
    Subjects *node = malloc(sizeof(Subjects));
    if (!node) return NULL;
    strncpy(node->name, name, MAX_NAME_LEN - 1);
    node->name[MAX_NAME_LEN - 1] = '\0';
    node->detail = NULL;
    node->next = NULL;
    return node;
}

void free_subjects(Subjects *head) {
    while (head) {
        Subjects *tmp = head;
        head = head->next;
        if (tmp->detail) free(tmp->detail);
        free(tmp);
    }
}

void init_subject_list() {
    const char *subjects[] = {
        "Acting Theory",
        "Stage Theory",
        "Script Analysis",
        "Dialogue Interpretation and Emotional Expression",
        "Character Development",
        "Storytelling Theory",
        "Stage Movement and Poses"
    };
    int n = sizeof(subjects) / sizeof(subjects[0]);
    Subjects *prev = NULL;
    for (int i = 0; i < n; i++) {
        Subjects *node = create_subject_node(subjects[i]);
        if (!node) {
            free_subjects(head);
            head = NULL;
            return;
        }
        if (!head) head = node;
        else prev->next = node;
        prev = node;
    }
}

void append_detail(Subjects *subj, const char *line) {
    size_t old_len = subj->detail ? strlen(subj->detail) : 0;
    size_t add_len = strlen(line);
    char *new_detail = realloc(subj->detail, old_len + add_len + 2);
    if (!new_detail) return;
    subj->detail = new_detail;
    if (old_len == 0) subj->detail[0] = '\0';
    strcat(subj->detail, line);
    strcat(subj->detail, " ");
}

void UpdateSubjectInfo(const char *line) {
    Subjects *cur = head;
    while (cur) {
        if (strstr(line, cur->name)) {
            append_detail(cur, line);
            break;
        }
        cur = cur->next;
    }
}

int LoadContentFile() {
    FILE *fp = fopen(CONTENT_FILENAME, "r");
    if (!fp) {
        printf("Error: Cannot open file %s\n", CONTENT_FILENAME);
        return 0;
    }
    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        UpdateSubjectInfo(line);
    }
    fclose(fp);
    return 1;
}

void PrintSubjectList() {
    Subjects *cur = head;
    int idx = 1;
    while (cur) {
        printf("%d. %s\n", idx++, cur->name);
        if (cur->detail) {
            const char *p = cur->detail;
            while (*p) {
                putchar(*p);
                if (*p == '.') putchar('\n');
                p++;
            }
            putchar('\n');
        } else {
            printf("(No details available)\n");
        }
        cur = cur->next;
    }
}

void insertSubject(const char *name, int pos) {
    Subjects *new_node = create_subject_node(name);
    if (!new_node) return;
    if (pos <= 1 || !head) {
        new_node->next = head;
        head = new_node;
        return;
    }
    Subjects *cur = head;
    int i = 1;
    while (cur->next && i < pos - 1) {
        cur = cur->next;
        i++;
    }
    new_node->next = cur->next;
    cur->next = new_node;
}

void removeSubject(int pos) {
    if (!head || pos < 1) return;
    if (pos == 1) {
        Subjects *tmp = head;
        head = head->next;
        if (tmp->detail) free(tmp->detail);
        free(tmp);
        return;
    }
    Subjects *cur = head;
    int i = 1;
    while (cur->next && i < pos - 1) {
        cur = cur->next;
        i++;
    }
    if (!cur->next) return;
    Subjects *to_remove = cur->next;
    cur->next = to_remove->next;
    if (to_remove->detail) free(to_remove->detail);
    free(to_remove);
}

void learnPerformanceStageTheory() {
    init_subject_list();
    if (!LoadContentFile()) return;
    PrintSubjectList();
}
