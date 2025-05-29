// menu.c

#include <stdio.h>
#include "stage8.h"

void showMenu() {
    int choice;
    while (1) {
        printf("\n=== MAIN MENU ===\n");
        printf("1. Training\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                showTrainingMenu();
                break;
            case 0:
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}

void showTrainingMenu() {
    int choice;
    while (1) {
        printf("\n--- TRAINING MENU ---\n");
        printf("8. Fan Communication\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 8:
                showFanCommunicationMenu();
                break;
            case 0:
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}

void showFanCommunicationMenu() {
    int choice;
    while (1) {
        printf("\n--- FAN COMMUNICATION MENU ---\n");
        printf("1. Data Protection\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                protectMyData();  // From stage8.c
                break;
            case 0:
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}
