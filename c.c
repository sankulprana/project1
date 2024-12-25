#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct account {
    int accountNumber;
    char name[100];
    float balance;
    struct account *next;
};

// Function prototypes
struct account* createAccount(int accountNumber, char *name, float initialDeposit);
void addAccount(struct account **head, int accountNumber, char *name, float initialDeposit);
void deposit(struct account *head, int accountNumber, float amount);
void withdraw(struct account *head, int accountNumber, float amount);
void checkBalance(struct account *head, int accountNumber);
void deleteAccount(struct account **head, int accountNumber);
void displayAccounts(struct account *head);
void saveAccountsToFile(struct account *head, const char *filename);
void loadAccountsFromFile(struct account **head, const char *filename);
void freeAccounts(struct account **head);
void menu();

// Function to display the menu
void menu() {
    printf("\n--- Bank Management System ---\n");
    printf("1. Create Account\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Check Balance\n");
    printf("5. Delete Account\n");
    printf("6. Display All Accounts\n");
    printf("7. Save Accounts to File\n");
    printf("8. Load Accounts from File\n");
    printf("9. Exit\n");
    printf("Enter your choice: ");
}

// Function to create a new account node
struct account* createAccount(int accountNumber, char *name, float initialDeposit) {
    struct account *newAccount = (struct account *)malloc(sizeof(struct account));
    if (!newAccount) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    newAccount->accountNumber = accountNumber;
    strcpy(newAccount->name, name);
    newAccount->balance = initialDeposit;
    newAccount->next = NULL;
    return newAccount;
}

// Function to add an account to the bank
void addAccount(struct account **head, int accountNumber, char *name, float initialDeposit) {
    struct account *newAccount = createAccount(accountNumber, name, initialDeposit);
    if (*head == NULL) {
        *head = newAccount;
        printf("Account created successfully.\n");
        return;
    }
    struct account *temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newAccount;
    printf("Account created successfully.\n");
}

// Function to deposit money into an account
void deposit(struct account *head, int accountNumber, float amount) {
    if (head == NULL) {
        printf("No accounts found.\n");
        return;
    }
    struct account *temp = head;
    while (temp != NULL) {
        if (temp->accountNumber == accountNumber) {
            temp->balance += amount;
            printf("Deposited %.2f successfully. New balance: %.2f\n", amount, temp->balance);
            return;
        }
        temp = temp->next;
    }
    printf("Account not found.\n");
}

// Function to withdraw money from an account
void withdraw(struct account *head, int accountNumber, float amount) {
    if (head == NULL) {
        printf("No accounts found.\n");
        return;
    }
    struct account *temp = head;
    while (temp != NULL) {
        if (temp->accountNumber == accountNumber) {
            if (temp->balance >= amount) {
                temp->balance -= amount;
                printf("Withdrawn %.2f successfully. New balance: %.2f\n", amount, temp->balance);
            } else {
                printf("Insufficient funds.\n");
            }
            return;
        }
        temp = temp->next;
    }
    printf("Account not found.\n");
}

// Function to check account balance
void checkBalance(struct account *head, int accountNumber) {
    if (head == NULL) {
        printf("No accounts found.\n");
        return;
    }
    struct account *temp = head;
    while (temp != NULL) {
        if (temp->accountNumber == accountNumber) {
            printf("Account Balance for %d: %.2f\n", temp->accountNumber, temp->balance);
            return;
        }
        temp = temp->next;
    }
    printf("Account not found.\n");
}

// Function to delete an account
void deleteAccount(struct account **head, int accountNumber) {
    if (*head == NULL) {
        printf("No accounts found.\n");
        return;
    }
    struct account *temp = *head, *prev = NULL;

    if (temp != NULL && temp->accountNumber == accountNumber) {
        *head = temp->next;
        free(temp);
        printf("Account deleted successfully.\n");
        return;
    }

    while (temp != NULL && temp->accountNumber != accountNumber) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Account not found.\n");
        return;
    }

    prev->next = temp->next;
    free(temp);
    printf("Account deleted successfully.\n");
}

// Function to display all accounts
void displayAccounts(struct account *head) {
    if (head == NULL) {
        printf("No accounts found.\n");
        return;
    }
    printf("\n--- List of Accounts ---\n");
    struct account *temp = head;
    while (temp != NULL) {
        printf("Account Number: %d, Name: %s, Balance: %.2f\n", temp->accountNumber, temp->name, temp->balance);
        temp = temp->next;
    }
}

// Function to save accounts to a file
void saveAccountsToFile(struct account *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file.\n");
        return;
    }
    struct account *temp = head;
    while (temp != NULL) {
        fprintf(file, "%d,%s,%.2f\n", temp->accountNumber, temp->name, temp->balance);
        temp = temp->next;
    }
    fclose(file);
    printf("Accounts saved to file successfully.\n");
}

// Function to load accounts from a file
void loadAccountsFromFile(struct account **head, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file.\n");
        return;
    }
    freeAccounts(head);

    int accountNumber;
    char name[100];
    float balance;

    while (fscanf(file, "%d,%99[^,],%f\n", &accountNumber, name, &balance) == 3) {
        addAccount(head, accountNumber, name, balance);
    }
    fclose(file);
    printf("Accounts loaded from file successfully.\n");
}

// Function to free all accounts
void freeAccounts(struct account **head) {
    struct account *temp = *head;
    while (temp != NULL) {
        struct account *next = temp->next;
        free(temp);
        temp = next;
    }
    *head = NULL;
}

// Main function
int main() {
    struct account *bank = NULL;
    int choice, accountNumber;
    char name[100];
    float amount;
    const char *filename = "accounts.txt";

    while (1) {
        menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Account Number: ");
                scanf("%d", &accountNumber);
                printf("Enter Name: ");
                getchar();
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                printf("Enter Initial Deposit: ");
                scanf("%f", &amount);
                addAccount(&bank, accountNumber, name, amount);
                break;

            case 2:
                printf("Enter Account Number: ");
                scanf("%d", &accountNumber);
                printf("Enter Amount to Deposit: ");
                scanf("%f", &amount);
                deposit(bank, accountNumber, amount);
                break;

            case 3:
                printf("Enter Account Number: ");
                scanf("%d", &accountNumber);
                printf("Enter Amount to Withdraw: ");
                scanf("%f", &amount);
                withdraw(bank, accountNumber, amount);
                break;

            case 4:
                printf("Enter Account Number: ");
                scanf("%d", &accountNumber);
                checkBalance(bank, accountNumber);
                break;

            case 5:
                printf("Enter Account Number: ");
                scanf("%d", &accountNumber);
                deleteAccount(&bank, accountNumber);
                break;

            case 6:
                displayAccounts(bank);
                break;

            case 7:
                saveAccountsToFile(bank, filename);
                break;

            case 8:
                loadAccountsFromFile(&bank, filename);
                break;

            case 9:
                freeAccounts(&bank);
                printf("Exiting program. Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

//CODE FOR EXPANDED LIBRARY MANAGEMENT SYSTEM
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct book {
    int id;
    char title[100];
    char author[100];
    struct book *next;
};

// Function prototypes
struct book* createBook(int id, char *title, char *author);
void addBook(struct book **head, int id, char *title, char *author);
void deleteBook(struct book **head, int id);
void searchBook(struct book *head, int id);
void displayBooks(struct book *head);
void sortBooksByTitle(struct book **head);
void saveToFile(struct book *head, const char *filename);
void loadFromFile(struct book **head, const char *filename);
void freeLibrary(struct book **head);
void menu();

// Function to display the main menu
void menu() {
    printf("\n--- Library Management System ---\n");
    printf("1. Add Book\n");
    printf("2. Delete Book\n");
    printf("3. Search Book\n");
    printf("4. Display All Books\n");
    printf("5. Sort Books by Title\n");
    printf("6. Save to File\n");
    printf("7. Load from File\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
}

// Function to create a new book node
struct book* createBook(int id, char *title, char *author) {
    struct book *newBook = (struct book *)malloc(sizeof(struct book));
    if (!newBook) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    newBook->id = id;
    strcpy(newBook->title, title);
    strcpy(newBook->author, author);
    newBook->next = NULL;
    return newBook;
}

// Function to add a book to the library
void addBook(struct book **head, int id, char *title, char *author) {
    struct book *newBook = createBook(id, title, author);
    if (*head == NULL) {
        *head = newBook;
        printf("Book added successfully.\n");
        return;
    }
    struct book *temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newBook;
    printf("Book added successfully.\n");
}

// Function to delete a book by ID
void deleteBook(struct book **head, int id) {
    if (*head == NULL) {
        printf("The library is empty.\n");
        return;
    }
    struct book *temp = *head, *prev = NULL;

    if (temp != NULL && temp->id == id) {
        *head = temp->next;
        free(temp);
        printf("Book deleted successfully.\n");
        return;
    }

    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Book not found.\n");
        return;
    }

    prev->next = temp->next;
    free(temp);
    printf("Book deleted successfully.\n");
}

// Function to search for a book by ID
void searchBook(struct book *head, int id) {
    if (head == NULL) {
        printf("The library is empty.\n");
        return;
    }
    struct book *temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            printf("\nBook Found:\n");
            printf("ID: %d\n", temp->id);
            printf("Title: %s\n", temp->title);
            printf("Author: %s\n", temp->author);
            return;
        }
        temp = temp->next;
    }
    printf("Book not found.\n");
}

// Function to display all books
void displayBooks(struct book *head) {
    if (head == NULL) {
        printf("The library is empty.\n");
        return;
    }
    printf("\n--- List of Books ---\n");
    struct book *temp = head;
    while (temp != NULL) {
        printf("ID: %d, Title: %s, Author: %s\n", temp->id, temp->title, temp->author);
        temp = temp->next;
    }
}

// Function to sort books by title
void sortBooksByTitle(struct book **head) {
    if (*head == NULL || (*head)->next == NULL) {
        printf("Nothing to sort.\n");
        return;
    }

    struct book *i, *j;
    for (i = *head; i->next != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (strcmp(i->title, j->title) > 0) {
                // Swap data
                int tempId = i->id;
                char tempTitle[100], tempAuthor[100];
                strcpy(tempTitle, i->title);
                strcpy(tempAuthor, i->author);

                i->id = j->id;
                strcpy(i->title, j->title);
                strcpy(i->author, j->author);

                j->id = tempId;
                strcpy(j->title, tempTitle);
                strcpy(j->author, tempAuthor);
            }
        }
    }
    printf("Books sorted by title successfully.\n");
}

// Function to save books to a file
void saveToFile(struct book *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file.\n");
        return;
    }
    struct book *temp = head;
    while (temp != NULL) {
        fprintf(file, "%d,%s,%s\n", temp->id, temp->title, temp->author);
        temp = temp->next;
    }
    fclose(file);
    printf("Books saved to file successfully.\n");
}

// Function to load books from a file
void loadFromFile(struct book **head, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file.\n");
        return;
    }
    freeLibrary(head);

    int id;
    char title[100], author[100];
    while (fscanf(file, "%d,%99[^,],%99[^\n]\n", &id, title, author) == 3) {
        addBook(head, id, title, author);
    }
    fclose(file);
    printf("Books loaded from file successfully.\n");
}

// Function to free all allocated memory
void freeLibrary(struct book **head) {
    struct book *temp = *head;
    while (temp != NULL) {
        struct book *next = temp->next;
        free(temp);
        temp = next;
    }
    *head = NULL;
}

// Main function
int main() {
    struct book *library = NULL;
    int choice, id;
    char title[100], author[100];
    const char *filename = "library.txt";

    while (1) {
        menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Book ID: ");
                scanf("%d", &id);
                printf("Enter Book Title: ");
                getchar();
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;
                printf("Enter Book Author: ");
                fgets(author, sizeof(author), stdin);
                author[strcspn(author, "\n")] = 0;
                addBook(&library, id, title, author);
                break;

            case 2:
                printf("Enter Book ID to delete: ");
                scanf("%d", &id);
                deleteBook(&library, id);
                break;

            case 3:
                printf("Enter Book ID to search: ");
                scanf("%d", &id);
                searchBook(library, id);
                break;

            case 4:
                displayBooks(library);
                break;

            case 5:
                sortBooksByTitle(&library);
                break;

            case 6:
                saveToFile(library, filename);
                break;

            case 7:
                loadFromFile(&library, filename);
                break;

            case 8:
                freeLibrary(&library);
                printf("Exiting program. Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}