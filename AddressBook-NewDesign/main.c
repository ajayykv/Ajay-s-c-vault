/*
    Author      : Ajay Krishnan V
    Date        : 24/ October/ 2025
    Description : This is the code base for addressbook project. The addressbook contains contact details 
    of a person specifically, name, phone and email. This project gives the user the facilities of creating,
    searching, editing, deleting and listing the contacts inside the addressbook.
*/


#include <stdio.h>
#include <stdio_ext.h>
#include <ctype.h>
#include "contact.h"    //including user built header file.

int main() {
    char choice;
    AddressBook addressBook;
    addressBook.contactCount = 0;
    initialize(&addressBook); // Initialize the address book

    do {
        printf(CYAN"\nAddress Book Menu:\n"RESET);
        printf("1. Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
        printf("6. Exit\n");

        __fpurge(stdin);
        printf(YELLOW"Enter your choice: "RESET);
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                createContact(&addressBook);    //pass by reference of the structure variable
                break;
            case '2':
                searchContact(&addressBook);
                break;
            case '3':
                editContact(&addressBook);
                break;
            case '4':
                deleteContact(&addressBook);
                break;
            case '5':
                printf(CYAN"Select sort criteria:\n"RESET);
                printf("1. Sort by name\n");
                printf("2. Sort by phone\n");
                printf("3. Sort by email\n");
                __fpurge(stdin);
                printf(YELLOW"Enter your choice: "RESET);
                char sortChoice;
                scanf(" %c", &sortChoice);
                listContacts(&addressBook, sortChoice);
                break;
            case '6':
                printf(CYAN"\nSaving and Exiting...\n"RESET);
                saveContactsToFile(&addressBook);
                break;
            default:
                printf(RED"Invalid choice. Please try again.\n"RESET);
        }
    } while (choice != '6');
    
       return 0;
}
