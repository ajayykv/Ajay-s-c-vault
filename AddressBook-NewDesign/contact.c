#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"
#include "file.h"
#include "populate.h"

int indx = -1;             //this variable will carry the resultant index value of the search
int exit_var = 0;          //variable to carry exit value.

void listContacts(AddressBook *addressBook, char sortCriteria) 
{
    // Sort contacts based on the chosen criteria
    switch(sortCriteria)
    {
        Contact temp;
        case '1' :
        for(int i = 0; i < addressBook -> contactCount - 1; i++)
        {
            for(int j = 0; j < addressBook -> contactCount - i - 1; j++)
            {
                if(strcasecmp(addressBook -> contacts[j].name, addressBook -> contacts[j+1].name) > 0)
                {
                    temp = addressBook -> contacts[j];
                    addressBook -> contacts[j] = addressBook -> contacts[j+1];
                    addressBook -> contacts[j+1] = temp;
                }
            }
        }
        break;

        case '2' :
        for(int i = 0; i < addressBook -> contactCount - 1; i++)
        {
            for(int j = 0; j < addressBook -> contactCount - i - 1; j++)
            {
                if(strcmp(addressBook -> contacts[j].phone, addressBook -> contacts[j+1].phone) > 0)
                {
                    temp = addressBook -> contacts[j];
                    addressBook -> contacts[j] = addressBook -> contacts[j+1];
                    addressBook -> contacts[j+1] = temp;
                }
            }
        }
        break;

        case '3' :
        for(int i = 0; i < addressBook -> contactCount - 1; i++)
        {
            for(int j = 0; j < addressBook -> contactCount - i - 1; j++)
            {
                if(strcmp(addressBook -> contacts[j].email, addressBook -> contacts[j+1].email) > 0)
                {
                    temp = addressBook -> contacts[j];
                    addressBook -> contacts[j] = addressBook -> contacts[j+1];
                    addressBook -> contacts[j+1] = temp;
                }
            }
        }
        break;

        default :
        printf(RED"Invalid choice..⚠️\n"RESET);
        return;
    }

    //display the sorted address book contacts
    printf("sl no. Name           Phone          email          \n");
    for(int i = 0; i < addressBook -> contactCount; i++)
    {
        printf("%2.1d.    %-15s", i+1, addressBook -> contacts[i].name);
        printf("%-15s", addressBook -> contacts[i].phone);
        printf("%-15s\n",  addressBook -> contacts[i].email);
    }
    
}

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    //populateAddressBook(addressBook);
    
    // Load contacts from file during initialization (After implementing files)
    loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}

int validate_name(char *name)
{
    int i = 0;

    //first character should be an alphabet
    if(isalpha(name[i]))
    {
        i++;
    }
    else
    {
        return 1;
    }

    while(name[i])
    {
        if( isalpha(name[i]) || name[i] == 32 || name[i] == 46)
            i++;
        else
            return 1;
    }

    //if name is valid, return 0.
    return 0; 
}

int validate_mobile_no(char *mobile_no)
{
    //validations on the user given mobile number
    int i=0;
    while(mobile_no[i])
    {
        if(isdigit(mobile_no[i]))
            i++;
        else
            return 1;
    }
    if(strlen(mobile_no) != 10)
        return 1;

    return 0;
}

int validate_mail_id(char *mail_id)
{
    //validations on the user given mail id.

    char *res1 = strstr(mail_id, ".com");
    char *res2 = strchr(mail_id, '@');
    if(res1 == NULL || res2 == NULL)
    return 1;
    if(*(res1 + 4) != '\0' || *(res2 + 1) == *res1)
    return 1;
    if(!isalpha(mail_id[0]) || mail_id[0] == '@' || strchr(res2+1, '@'))
    return 1;

    for(int i = 0; mail_id[i]; i++)
    {
        if(isupper(mail_id[i]))
        return 1;
    }

    return 0;
}

void createContact(AddressBook *addressBook)
{
	/* Define the logic to create a Contacts */
    //STEP 1: read the name and validate it.
    
    int valid = 0;
    char usr_name[50];
    do{
        __fpurge(stdin);
        printf("Enter the Name : ");
        scanf(" %[^\n]", usr_name);

        valid = validate_name(usr_name);
        if(valid != 0)
        printf(RED"ERROR: Invalid name..⚠️ Enter a valid name.\n"RESET);
    }while(valid != 0);

    //STEP 2: If valid -> Goto STEP 3, If not valid -> Print error msg and goto STEP 1.

    //STEP 3: Read the mobile no. and validate it.
    char mobile_no[15];    
    do{
        __fpurge(stdin);
        printf("Enter the phone number : ");
        scanf(" %s", mobile_no);

        valid = validate_mobile_no(mobile_no);
        if(valid != 0)
        printf(RED"ERROR: Invalid mobile number..⚠️ Enter a valid mobile number.\n"RESET);
        else
        {
        for(int i = 0; i< addressBook ->contactCount; i++)
        {
            if(strcmp(mobile_no, addressBook ->contacts[i].phone) == 0)
            {
                printf(MAGENTA"The above phone number is already present..⚠️  Phone number should be unique.\n"RESET);
                valid = 1;
            }
        }        
        }
    }while(valid!=0);    

    //STEP 4: if valid -> Goto sTEP 5, if not valid -> Print error msg and Goto STEP 3.

    //STEP 5: Read the mail ID and validate it.
    char mail_id[50];
    do{
        __fpurge(stdin);
        printf("Enter the mail ID : ");
        scanf(" %s", mail_id);

        valid = validate_mail_id(mail_id);
        if(valid != 0)
        printf(RED"ERROR: Invalid mail ID..⚠️ Enter a valid mail ID.\n "RESET);
        else
        {
        for(int i = 0; i < addressBook -> contactCount; i++)
        {
            if(strcmp(mail_id, addressBook -> contacts[i].email) == 0)
            {
                printf(MAGENTA"The above mail id is already present..⚠️  Mail id should be unique.\n"RESET);
                valid = 1;;
            }
        }            
        }
    }while(valid!=0);
    //STEP 6: if valid -> Goto sTEP 7, if not valid -> Print error msg and Goto STEP 5.

    //STEP 7: Store the details in addressbook
    strcpy(addressBook -> contacts[addressBook -> contactCount].name, usr_name);
    strcpy(addressBook->contacts[addressBook->contactCount].phone, mobile_no);
    strcpy(addressBook->contacts[addressBook->contactCount].email, mail_id);
    printf(GREEN"\nThe contact has been stored suuccessfully..✅\n"RESET);

    addressBook->contactCount++;
    
}


/*
function to seach the user entered name in the address book, prints the result and returns the index of the resultant contact.
*/
void search_by_name(char *usr_name, AddressBook *addressBook, int *indx_value)
{
    indx = -1;
    exit_var = 0;
    int count = 0, array[20];
    printf("sl no. Name           Phone          email          \n");
     for(int c = 0; c < addressBook -> contactCount; c++)
     {
        if(strcasestr(addressBook -> contacts[c].name, usr_name))
        {
            array[count] = c;            
            count++;
            printf("%2.1d.    %-15s", count, addressBook -> contacts[c].name);
            printf("%-15s", addressBook -> contacts[c].phone);
            printf("%-15s\n",  addressBook -> contacts[c].email);
            *indx_value = c;
        }
     }
     if(count == 0)
     {
        printf(CYAN"No matches found..⚠️\n"RESET);
     }
     else if(count > 1)
     {
        int serial = 0;
        do
        {
            __fpurge(stdin);
            printf(YELLOW"Enter serial number of the name you want to search : "RESET);
            scanf(" %d", &serial);
            if(serial > count || serial < 1)
            printf(RED"Incorrect serial number..⚠️  Please enter a valid serial number.\n"RESET);
        }while(serial > count || serial < 1);

        *indx_value = array[serial - 1]; 

        printf("%d. Name : %s\t", 1, addressBook -> contacts[*indx_value].name);
        printf("Mobile number : %s\t", addressBook -> contacts[*indx_value].phone);
        printf("Mail ID : %s\n",  addressBook -> contacts[*indx_value].email);
     }
}


/*
function to seach the user entered phone number in the address book, prints the result and returns the index of the resultant contact.
*/
void search_by_phone(char *mobile_no, AddressBook *addressBook, int *indx_value)
{
    indx = -1;
    for(int c = 0; c < addressBook -> contactCount; c++)
    {
        if(strcmp(addressBook -> contacts[c].phone, mobile_no) == 0)
        {
            printf("--> Name : %s\t", addressBook -> contacts[c].name);
            printf("Mobile number : %s\t", addressBook -> contacts[c].phone);
            printf("Mail ID : %s\n",  addressBook -> contacts[c].email);
            *indx_value = c;
            break;
        }
    }
    
    if(*indx_value == -1)
    printf(CYAN"No matches found..⚠️\n"RESET);
}


/*
function to seach the user entered mail ID in the address book, prints the result and returns the index of the resultant contact.
*/
void search_by_email(char *mail_id, AddressBook *addressBook, int *indx_value)
{
    indx = -1;
    for(int c = 0; c < addressBook -> contactCount; c++)
    {
        if(strcmp(addressBook -> contacts[c].email, mail_id) == 0)
        {
            printf("--> Name : %s\t", addressBook -> contacts[c].name);
            printf("Mobile number : %s\t", addressBook -> contacts[c].phone);
            printf("Mail ID : %s\n",  addressBook -> contacts[c].email);
            *indx_value = c;
            break;
        }
    }

    if(*indx_value == -1)
    printf("No matches found..⚠️\n");
}

void searchContact(AddressBook *addressBook)
{
    /* Define the logic for search */
    exit_var = 0;
    printf(CYAN"Select search field:\n"RESET);
    printf("1. Search by name\n");
    printf("2. Search by phone\n");
    printf("3. Search by email\n");
    printf("4. Exit\n");
    char choice;

    do{
    __fpurge(stdin);
    printf(YELLOW"Enter your choice: "RESET);
    scanf(" %c", &choice);

    switch(choice)
    {
        case '1':       
        int valid;
        char usr_name[50];
        do{
            printf(YELLOW"Enter the Name you want to search : "RESET);
            __fpurge(stdin);
            scanf(" %[^\n]", usr_name);

            valid = validate_name(usr_name);
            if(valid != 0)
            printf(RED"ERROR: Invalid name..⚠️\tEnter a valid name.\n"RESET);
        }while(valid != 0);
        search_by_name(usr_name, addressBook, &indx);
        break;

        case '2':
        char mobile_no[11];    
        do{
            printf(YELLOW"Enter the mobile number you want to search : "RESET);
            __fpurge(stdin);
            scanf(" %s", mobile_no);

            valid = validate_mobile_no(mobile_no);
            if(valid != 0)
            printf(RED"ERROR: Invalid mobile number..⚠️\tEnter a valid mobile number.\n"RESET);
        }while(valid!=0);
        search_by_phone(mobile_no, addressBook, &indx);
        break;

        case '3':
        char mail_id[50];
        do{
            printf(YELLOW"Enter the mail ID you want to search: "RESET);
            __fpurge(stdin);
            scanf(" %s", mail_id);

            valid = validate_mail_id(mail_id);
            if(valid != 0)
            printf(RED"ERROR: Invalid mail ID..⚠️\tEnter a valid mail ID.\n"RESET);
        }while(valid!=0);
        search_by_email(mail_id, addressBook, &indx);
        break;

        case '4' :
        exit_var = 1;
        return;

        default :
        printf(RED"Invalid choice..⚠️\n"RESET);
        break;
    }
    }while(choice < '1' || choice > '4');
              
}

void editContact(AddressBook *addressBook)
{
	/* Define the logic for Editcontact */
    exit_var = 0;
    do{
        searchContact(addressBook);
    }while(indx == -1 && exit_var != 1);

    if(exit_var == 1)
    {
        return;
    }

    printf(CYAN"Select the field you want to edit:\n"RESET);
    printf("1. Edit name\n");
    printf("2. Edit phone\n");
    printf("3. Edit email\n");
    printf("4. Exit\n");
    char choice;

    do{
    __fpurge(stdin);
    printf(YELLOW"Enter your choice: "RESET);
    scanf(" %c", &choice);
    switch(choice)
    {
        case '1' :
        int valid;
        char usr_name[50];
        do{
            printf("Enter the new Name : ");
            __fpurge(stdin);
            scanf("%[^\n]", usr_name);

            valid = validate_name(usr_name);
            if(valid != 0)
            printf(RED"ERROR: Invalid name..⚠️ Enter a valid name.\n"RESET);
        }while(valid != 0);
        strcpy(addressBook -> contacts[indx].name, usr_name);       //replacing the name.

        printf(CYAN"Modified contact details :\n"RESET);
        printf("Name : %s\t", addressBook -> contacts[indx].name);
        printf("Mobile number : %s\t", addressBook -> contacts[indx].phone);
        printf("Mail ID : %s\n",  addressBook -> contacts[indx].email);
        break;

        case '2':
        char mobile_no[11];    
        do{
            printf(YELLOW"Enter the new mobile number : "RESET);
            __fpurge(stdin);
            scanf(" %s", mobile_no);

            valid = validate_mobile_no(mobile_no);
            if(valid != 0)
            printf(RED"ERROR: Invalid mobile number..⚠️ Enter a valid mobile number.\n"RESET);
        }while(valid!=0);
        strcpy(addressBook -> contacts[indx].phone, mobile_no);

        printf(CYAN"Modified contact details :\n"RESET);
        printf("Name : %s\t", addressBook -> contacts[indx].name);
        printf("Mobile number : %s\t", addressBook -> contacts[indx].phone);
        printf("Mail ID : %s\n",  addressBook -> contacts[indx].email);
        break;

        case '3':
        char mail_id[50];
        do{
            printf(YELLOW"Enter the new mail ID : "RESET);
            __fpurge(stdin);
            scanf(" %s", mail_id);

            valid = validate_mail_id(mail_id);
            if(valid != 0)
            printf(RED"ERROR: Invalid mail ID..⚠️⚠️ Enter a valid mail ID.\n "RESET);
        }while(valid!=0);
        strcpy(addressBook -> contacts[indx].email, mail_id);

        printf(CYAN"Modified contact details :\n"RESET);
        printf("Name : %s\t", addressBook -> contacts[indx].name);
        printf("Mobile number : %s\t", addressBook -> contacts[indx].phone);
        printf("Mail ID : %s\n",  addressBook -> contacts[indx].email);
        break;

        case '4' :
        return;

        default :
        printf(RED"Invalid choice..⚠️\n"RESET);
        break;
    }
    }while(choice < '1' || choice > '4');
}

void deleteContact(AddressBook *addressBook)
{
	/* Define the logic for deletecontact */
    do{
    searchContact(addressBook);
    }while(indx == -1 && exit_var != 1);

    if(exit_var == 1)
    {
        return;
    }

    __fpurge(stdin);
    printf(MAGENTA"Do you want to delete the above contact? (Enter Y/N) : "RESET);
    char ch;
    scanf(" %c", &ch);

    switch(ch)
    {
        case 'y' :
        case 'Y' : 
        for(int c = indx; c < addressBook -> contactCount; c++)
        {
            addressBook ->contacts[c] = addressBook -> contacts[c+1];
        }
        addressBook -> contactCount--;
        printf(CYAN"The selected contact has been deleted from the Addressbook Successfully...✅\n"RESET);
        break;

        case 'n' :
        case 'N' :
        printf(CYAN"Deletion of the above contact is aborted.\n"RESET);
        return;

        default :
        printf(RED"Invalid choice..⚠️ Enter valid choice (Y or N).\n"RESET);
        deleteContact(addressBook);
    }
   
}
