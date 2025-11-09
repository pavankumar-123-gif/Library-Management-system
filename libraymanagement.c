#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 50
#define MAX_AUTHOR 50
#define PASSWORD "admin123"

typedef struct {
    int id;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int isIssued;
} Book;

FILE *fp;

// Function declarations
void addBook();
void displayBooks();
void searchBookByID();
void issueBook();
void returnBook();
void deleteBook();
void updateBook();
void searchByText();
void libraryStats();
void backupData();
int login();
void menu();

// Main function
int main() {
    if (!login()) {
        printf("\nAccess Denied\n");
        return 0;
    }

    int choice;
    do {
        menu();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBookByID(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: deleteBook(); break;
            case 7: updateBook(); break;
            case 8: searchByText(); break;
            case 9: libraryStats(); break;
            case 10: backupData(); break;
            case 11: printf("\nThank you for using Library Management System!\n"); break;
            default: printf("\nInvalid choice! Try again.\n");
        }
    } while (choice != 11);

    return 0;
}

// Login function
int login() {
    char pass[20];
    char uname[30];
    printf("enter your name:");
    scanf("%s",&uname);
    printf("Enter Admin Password: ");
    scanf("%s", pass);
    if (strcmp(pass, PASSWORD) == 0) {
        printf("\nAccess Granted\n");
        return 1;
    }
    return 0;
}

// Menu display
void menu() {
    printf("\n==== LIBRARY MANAGEMENT SYSTEM ====\n");
    printf("1. Add Book\n");
    printf("2. Display All Books\n");
    printf("3. Search Book (by ID)\n");
    printf("4. Issue Book\n");
    printf("5. Return Book\n");
    printf("6. Delete Book\n");
    printf("7. Update Book\n");
    printf("8. Search by Title/Author\n");
    printf("9. Library Stats\n");
    printf("10. Backup Library Data\n");
    printf("11. Exit\n");
}

// Add Book
void addBook() {
    Book b;
    fp = fopen("library.dat", "ab+");
    if (!fp) { printf("Error opening file!\n"); return; }

    printf("\nEnter Book ID: ");
    scanf("%d", &b.id);
    getchar();
    printf("Enter Title: ");
    fgets(b.title, MAX_TITLE, stdin);
    b.title[strcspn(b.title, "\n")] = '\0';
    printf("Enter Author: ");
    fgets(b.author, MAX_AUTHOR, stdin);
    b.author[strcspn(b.author, "\n")] = '\0';
    b.isIssued = 0;

    fwrite(&b, sizeof(Book), 1, fp);
    fclose(fp);
    printf("Book added successfully\n");
}

// Display all books
void displayBooks() {
    Book b;
    int count = 0;
    fp = fopen("library.dat", "rb");
    if (!fp) { printf("No records found!\n"); return; }

    printf("\n==== LIBRARY RECORDS ====\n");
    while (fread(&b, sizeof(Book), 1, fp)) {
        printf("ID: %d | Title: %s | Author: %s | Status: %s\n",
               b.id, b.title, b.author, b.isIssued ? "Issued" : "Available");
        count++;
    }
    printf("-----------------------------------------------\nTotal Books: %d\n", count);
    fclose(fp);
}

// Search by ID
void searchBookByID() {
    int id, found = 0;
    Book b;
    printf("Enter Book ID to search: ");
    scanf("%d", &id);
    fp = fopen("library.dat", "rb");
    if (!fp) { printf("Error opening file!\n"); return; }

    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.id == id) {
            printf("Book found\nTitle: %s\nAuthor: %s\nStatus: %s\n",
                   b.title, b.author, b.isIssued ? "Issued" : "Available");
            found = 1;
            break;
        }
    }
    if (!found) printf("Book not found\n");
    fclose(fp);
}

// Issue Book
void issueBook() {
    int id, found = 0;
    Book b;
    fp = fopen("library.dat", "rb+");
    if (!fp) { printf("Error opening file!\n"); return; }

    printf("Enter Book ID to issue: ");
    scanf("%d", &id);
    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.id == id) {
            found = 1;
            if (b.isIssued) {
                printf("Book already issued\n");
                break;
            }
            b.isIssued = 1;
            fseek(fp, -sizeof(Book), SEEK_CUR);
            fwrite(&b, sizeof(Book), 1, fp);
            printf("Book issued successfully\n");
            break;
        }
    }
    if (!found) printf("Book not found\n");
    fclose(fp);
}

// Return Book
void returnBook() {
    int id, days;
    float fine = 0;
    Book b;
    fp = fopen("library.dat", "rb+");
    if (!fp) { printf("Error opening file!\n"); return; }

    printf("Enter Book ID to return: ");
    scanf("%d", &id);
    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.id == id && b.isIssued) {
            printf("Enter number of late days: ");
            scanf("%d", &days);
            fine = days * 5.0;
            b.isIssued = 0;
            fseek(fp, -sizeof(Book), SEEK_CUR);
            fwrite(&b, sizeof(Book), 1, fp);
            printf("Book returned successfully\nFine = Rs. %.2f\n", fine);
            fclose(fp);
            return;
        }
    }
    printf("Book not found or not issued\n");
    fclose(fp);
}

// Delete Book
void deleteBook() {
    int id, found = 0;
    FILE *temp;
    Book b;

    printf("Enter Book ID to delete: ");
    scanf("%d", &id);
    fp = fopen("library.dat", "rb");
    temp = fopen("temp.dat", "wb");
    if (!fp || !temp) { printf("Error opening file!\n"); return; }

    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.id != id) fwrite(&b, sizeof(Book), 1, temp);
        else found = 1;
    }
    fclose(fp);
    fclose(temp);
    remove("library.dat");
    rename("temp.dat", "library.dat");

    if (found)
        printf("Book deleted successfully\n");
    else
        printf("Book not found\n");
}

// Update Book
void updateBook() {
    int id, found = 0;
    Book b;
    fp = fopen("library.dat", "rb+");
    if (!fp) { printf("Error opening file!\n"); return; }

    printf("Enter Book ID to update: ");
    scanf("%d", &id);
    getchar();
    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.id == id) {
            printf("Enter new Title: ");
            fgets(b.title, MAX_TITLE, stdin);
            b.title[strcspn(b.title, "\n")] = '\0';
            printf("Enter new Author: ");
            fgets(b.author, MAX_AUTHOR, stdin);
            b.author[strcspn(b.author, "\n")] = '\0';
            fseek(fp, -sizeof(Book), SEEK_CUR);
            fwrite(&b, sizeof(Book), 1, fp);
            printf("Book updated successfully\n");
            found = 1;
            break;
        }
    }
    if (!found) printf("Book not found\n");
    fclose(fp);
}

// Search by Title or Author
void searchByText() {
    char text[50];
    Book b;
    int found = 0;
    printf("Enter title or author keyword: ");
    getchar();
    fgets(text, 50, stdin);
    text[strcspn(text, "\n")] = '\0';

    fp = fopen("library.dat", "rb");
    if (!fp) { printf("Error opening file!\n"); return; }

    while (fread(&b, sizeof(Book), 1, fp)) {
        if (strstr(b.title, text) || strstr(b.author, text)) {
            printf("ID: %d | Title: %s | Author: %s | Status: %s\n",
                   b.id, b.title, b.author, b.isIssued ? "Issued" : "Available");
            found = 1;
        }
    }
    if (!found) printf("No matches found\n");
    fclose(fp);
}

// Library Stats
void libraryStats() {
    int total = 0, issued = 0;
    Book b;
    fp = fopen("library.dat", "rb");
    if (!fp) { printf("No records found!\n"); return; }

    while (fread(&b, sizeof(Book), 1, fp)) {
        total++;
        if (b.isIssued) issued++;
    }
    fclose(fp);
    printf("\n==== LIBRARY STATISTICS ====\n");
    printf("Total Books: %d\nIssued Books: %d\nAvailable Books: %d\n",
           total, issued, total - issued);
}

// Backup Library Data
void backupData() {
    FILE *backup;
    Book b;

    fp = fopen("library.dat", "rb");
    backup = fopen("backup.dat", "wb");
    if (!fp || !backup) { printf("Error creating backup!\n"); return; }

    while (fread(&b, sizeof(Book), 1, fp))
        fwrite(&b, sizeof(Book), 1, backup);

    fclose(fp);
    fclose(backup);
    printf("Backup created successfully (File: backup.dat)\n");
}
