#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX_USERS 100
#define USERNAME_LEN 50
#define PASSWORD_LEN 50
#define PHONE_LEN 15
#define ADDRESS_LEN 100
#define PIN_LEN 5  // 4 digits + null terminator
#define CARD_NUM_LEN 20
#define ADMIN_PIN "9999"  // Fixed admin PIN for admin menu access

typedef struct
{
    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
    char transaction_pin[PIN_LEN];
    char phone[PHONE_LEN];
    char address[ADDRESS_LEN];
    int age;
    double balance;
    char card_number[CARD_NUM_LEN];
    char atm_pin[PIN_LEN];
} User;

User users[MAX_USERS];
int userCount = 0;

// Function prototypes
int login();
void userMenu(int userIndex);
void adminMenu();
void deposit(int userIndex);
void withdraw(int userIndex);
void sendMoney(int senderIndex);
void createAccount();
void generateCardDetails(User *user);
int isValidPin(const char *pin);
int isDigits(const char *str);

int main()
{
    srand(time(NULL));  // Seed RNG once

    int choice;

    while (1)
    {
        printf("\n--- Bank Management System ---\n");
        printf("1. Login\n");
        printf("2. Admin Menu (PIN Required)\n");
        printf("3. Create New Account\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            // Clear invalid input
            while(getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        while(getchar() != '\n'); // Clear newline left in input buffer

        if (choice == 1)
        {
            int index = login();
            if (index != -1) userMenu(index);
        }
        else if (choice == 2)
        {
            adminMenu();
        }
        else if (choice == 3)
        {
            createAccount();
        }
        else if (choice == 4)
        {
            printf("Exiting. Thank you!\n");
            break;
        }
        else
        {
            printf("Invalid choice. Try again.\n");
        }
    }

    printf("\n\n--- Developed by Hridoyaja ---\n");
    printf("Contact: hridoyaja@example.com\n");

    return 0;
}

void generateCardDetails(User *user)
{
    sprintf(user->card_number, "4312%04d%04d", rand() % 10000, rand() % 10000);
    sprintf(user->atm_pin, "%04d", rand() % 10000);
}

void createAccount()
{
    if (userCount >= MAX_USERS)
    {
        printf("User limit reached.\n");
        return;
    }

    User newUser;

    printf("\n--- Create Account ---\n");

    printf("Enter full name (username, max %d chars): ", USERNAME_LEN - 1);
    if (fgets(newUser.username, USERNAME_LEN, stdin) == NULL) {
        printf("Error reading username.\n");
        return;
    }
    newUser.username[strcspn(newUser.username, "\n")] = 0; // remove newline

    printf("Enter phone number (max %d chars): ", PHONE_LEN - 1);
    if (fgets(newUser.phone, PHONE_LEN, stdin) == NULL) {
        printf("Error reading phone number.\n");
        return;
    }
    newUser.phone[strcspn(newUser.phone, "\n")] = 0;

    printf("Enter address (max %d chars): ", ADDRESS_LEN - 1);
    if (fgets(newUser.address, ADDRESS_LEN, stdin) == NULL) {
        printf("Error reading address.\n");
        return;
    }
    newUser.address[strcspn(newUser.address, "\n")] = 0;

    printf("Enter age (positive integer): ");
    if (scanf("%d", &newUser.age) != 1 || newUser.age <= 0) {
        printf("Invalid age.\n");
        while(getchar() != '\n'); // Clear input buffer
        return;
    }
    while(getchar() != '\n'); // Clear newline

    printf("Set login password (max %d chars): ", PASSWORD_LEN - 1);
    if (fgets(newUser.password, PASSWORD_LEN, stdin) == NULL) {
        printf("Error reading password.\n");
        return;
    }
    newUser.password[strcspn(newUser.password, "\n")] = 0;

    while (1)
    {
        printf("Set 4-digit transaction PIN (numbers only): ");
        if (fgets(newUser.transaction_pin, PIN_LEN + 10, stdin) == NULL) {
            printf("Error reading PIN.\n");
            return;
        }
        newUser.transaction_pin[strcspn(newUser.transaction_pin, "\n")] = 0;

        if (isValidPin(newUser.transaction_pin)) break;
        printf("Invalid PIN. It must be exactly 4 digits.\n");
    }

    newUser.balance = 1000.0;
    generateCardDetails(&newUser);
    users[userCount++] = newUser;

    printf("Account created successfully with Tk. 1000 balance!\n");
    printf("Generated Card Number: %s\n", newUser.card_number);
    printf("Generated ATM PIN: %s\n", newUser.atm_pin);
}

int login()
{
    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];

    printf("\n--- Login ---\n");

    printf("Username: ");
    if (fgets(username, USERNAME_LEN, stdin) == NULL) {
        printf("Error reading username.\n");
        return -1;
    }
    username[strcspn(username, "\n")] = 0;

    printf("Password: ");
    if (fgets(password, PASSWORD_LEN, stdin) == NULL) {
        printf("Error reading password.\n");
        return -1;
    }
    password[strcspn(password, "\n")] = 0;

    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0)
        {
            printf("Login successful.\n");
            return i;
        }
    }

    printf("Invalid username or password.\n");
    return -1;
}

int verifyPIN(int userIndex)
{
    char pin[PIN_LEN + 10];

    printf("Enter 4-digit transaction PIN: ");
    if (fgets(pin, sizeof(pin), stdin) == NULL)
    {
        printf("Error reading PIN.\n");
        return 0;
    }
    pin[strcspn(pin, "\n")] = 0;

    if (!isValidPin(pin))
    {
        printf("PIN must be exactly 4 digits.\n");
        return 0;
    }

    return strcmp(pin, users[userIndex].transaction_pin) == 0;
}

void userMenu(int userIndex)
{
    int choice;

    while (1)
    {
        printf("\n--- User Menu ---\n");
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Send Money\n");
        printf("4. View Balance\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        while(getchar() != '\n');

        if (choice == 1)
        {
            if (verifyPIN(userIndex)) deposit(userIndex);
            else printf("Incorrect PIN.\n");
        }
        else if (choice == 2)
        {
            if (verifyPIN(userIndex)) withdraw(userIndex);
            else printf("Incorrect PIN.\n");
        }
        else if (choice == 3)
        {
            if (verifyPIN(userIndex)) sendMoney(userIndex);
            else printf("Incorrect PIN.\n");
        }
        else if (choice == 4)
        {
            if (verifyPIN(userIndex)) printf("Balance: %.2lf\n", users[userIndex].balance);
            else printf("Incorrect PIN.\n");
        }
        else if (choice == 5)
        {
            printf("Logging out...\n");
            break;
        }
        else
        {
            printf("Invalid choice.\n");
        }
    }
}

void deposit(int userIndex)
{
    double amount;
    printf("Enter amount to deposit: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0)
    {
        printf("Invalid amount.\n");
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');

    users[userIndex].balance += amount;
    printf("Deposited successfully. New balance: %.2lf\n", users[userIndex].balance);
}

void withdraw(int userIndex)
{
    double amount;
    printf("Enter amount to withdraw: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0)
    {
        printf("Invalid amount.\n");
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');

    if (amount <= users[userIndex].balance)
    {
        users[userIndex].balance -= amount;
        printf("Withdrawal successful. Remaining balance: %.2lf\n", users[userIndex].balance);
    }
    else
    {
        printf("Insufficient balance.\n");
    }
}

void sendMoney(int senderIndex)
{
    char receiverUsername[USERNAME_LEN];
    double amount;

    printf("Enter receiver username: ");
    if (fgets(receiverUsername, USERNAME_LEN, stdin) == NULL)
    {
        printf("Error reading username.\n");
        return;
    }
    receiverUsername[strcspn(receiverUsername, "\n")] = 0;

    if (strcmp(receiverUsername, users[senderIndex].username) == 0)
    {
        printf("You cannot send money to yourself.\n");
        return;
    }

    printf("Enter amount to send: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0)
    {
        printf("Invalid amount.\n");
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');

    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].username, receiverUsername) == 0)
        {
            if (amount <= users[senderIndex].balance)
            {
                users[senderIndex].balance -= amount;
                users[i].balance += amount;
                printf("Transferred %.2lf to %s\n", amount, receiverUsername);
            }
            else
            {
                printf("Insufficient balance.\n");
            }
            return;
        }
    }
    printf("User not found.\n");
}

void adminMenu()
{
    char pin[PIN_LEN + 10];
    printf("\n--- Admin Menu ---\n");
    printf("Enter admin PIN: ");
    if (fgets(pin, sizeof(pin), stdin) == NULL)
    {
        printf("Error reading PIN.\n");
        return;
    }
    pin[strcspn(pin, "\n")] = 0;

    if (strcmp(pin, ADMIN_PIN) != 0)
    {
        printf("Invalid admin PIN. Access denied.\n");
        return;
    }

    printf("Access granted to admin.\n");
    printf("\n--- All User Details ---\n");
    for (int i = 0; i < userCount; i++)
    {
        printf("User %d:\n", i + 1);
        printf(" Username: %s\n", users[i].username);
        printf(" Phone: %s\n", users[i].phone);
        printf(" Address: %s\n", users[i].address);
        printf(" Age: %d\n", users[i].age);
        printf(" Balance: %.2lf\n", users[i].balance);
        printf(" Card Number: %s\n", users[i].card_number);
        printf(" ATM PIN: %s\n", users[i].atm_pin);
        printf("--------------------------\n");
    }
}

int isValidPin(const char *pin)
{
    // PIN must be exactly 4 digits
    if (strlen(pin) != 4) return 0;
    return isDigits(pin);
}

int isDigits(const char *str)
{
    for (int i = 0; str[i]; i++)
    {
        if (!isdigit((unsigned char)str[i])) return 0;
    }
    return 1;
}
