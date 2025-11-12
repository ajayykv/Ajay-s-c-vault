#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook) 
{
    FILE *fptr = fopen("SavedAddressBook.csv", "w");

    fprintf(fptr, "%d\n", addressBook -> contactCount);
    //fprintf(fptr, "Name, Phone, email\n");

    for(int c = 0; c < addressBook -> contactCount; c++)
    {
    fprintf(fptr, "%s,%s,%s\n", addressBook -> contacts[c].name, addressBook -> contacts[c].phone, addressBook -> contacts[c].email);
    }

    fclose(fptr);
}

void loadContactsFromFile(AddressBook *addressBook) 
{
    FILE *fptr = fopen("SavedAddressBook.csv", "r");
    
    fscanf(fptr, "%d\n", &addressBook ->contactCount);
    //printf("contact count : %d\n", addressBook ->contactCount);
    
    for(int c = 0; c < addressBook ->contactCount && addressBook ->contactCount < MAX_CONTACTS; c++)
    {
        fscanf(fptr, "%[^,],%[^,],%s\n", addressBook -> contacts[c].name, addressBook -> contacts[c].phone, addressBook -> contacts[c].email);
    }

    fclose(fptr);
}
