
// S.

#include <iomanip>
#include <iostream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
#include <regex>

// Definitions
// Task 1
void PrintDataStructure(HEADER_D* p);
void PrintHeaderDStructure(HEADER_D* p);
void PrintHeaderAStructure(HEADER_A* p);
void PrintItem2Structure(ITEM2* p);
void PrintTimeStructure(TIME* p);
void PrintCounter();
void PrintError(const char* s);

// Task 2
void InsertItemAsFirst(HEADER_A* pHeaderA, ITEM2* item);
void IncertItemInBetween(ITEM2* pItemPrev, ITEM2* pItemNext, ITEM2* pNewItem);
bool StringsAreTheSame(char* a, char* b);
bool VerifyFormattingForItemID(char* ItemID);
HEADER_D* InsertItem(HEADER_D* d, int PosStructD = 0, int PosStructA = 0, int Pos2Item = 0, char* pNewItemID = NULL);

// Task 3
HEADER_D* RemoveItem(HEADER_D* p, char* pRemoveItemID);

int main();

// Implementations
// Task 1
void PrintError(const char* s)
{
    std::cout << "[ERROR]" << s << std::endl;
}

unsigned int counterDStructures = 0;
unsigned int counterAStructures = 0;
unsigned int counter2Items = 0;

void PrintCounter()
{
    // Prints:
    // [ 1| 2| 3] 
    // [98|76|54]
    // etc...
    std::cout << "[";
    std::cout << std::setfill(' ') << std::setw(2) << counterDStructures;
    std::cout << "|";
    std::cout << std::setfill(' ') << std::setw(2) << counterAStructures;
    std::cout << "|";
    std::cout << std::setfill(' ') << std::setw(2) << counter2Items;
    std::cout << "] ";
}

void PrintTimeStructure(TIME* p)
{
    if (p == NULL) return PrintError("Can't print Time structure because it is empty");
    // Print in format: 01:23:45

    std::cout << std::setfill('0') << std::setw(2) << p->Hour;
    std::cout << ":";
    std::cout << std::setfill('0') << std::setw(2) << p->Min;
    std::cout << ":";
    std::cout << std::setfill('0') << std::setw(2) << p->Sec;
}

void PrintItem2Structure(ITEM2* p)
{
    if (p == NULL) return PrintError("Can't print Item 2 structure because it is empty");

    char* pID = p->pID;
    unsigned long int Code = p->Code;
    TIME* pTime = p->pTime;
    struct item2* pNext = p->pNext;

    PrintCounter();
    std::cout << std::setfill(' ') << std::setw(16) << pID;
    std::cout << " ";
    std::cout << std::setfill(' ') << std::setw(10) << Code;
    std::cout << " ";
    PrintTimeStructure(pTime);
    std::cout << std::endl;

    counter2Items++;

    if (pNext != NULL)
        PrintItem2Structure(pNext);
}

void PrintHeaderAStructure(HEADER_A* p)
{
    if (p == NULL) return PrintError("Can't print Header A structure because it is empty");

    ITEM2* pItems = (ITEM2*)p->pItems;
    counter2Items = 0;
    PrintItem2Structure(pItems);
    counter2Items = 0;
    char cBegin = p->cBegin;
    struct headerA* pNext = p->pNext;

    counterAStructures++;

    if (pNext != NULL)
        PrintHeaderAStructure(pNext);
}

void PrintHeaderDStructure(HEADER_D* p)
{
    if (p == NULL) return PrintError("Can't print data structure because it is empty");

    HEADER_A* pHeaderA = p->pHeaderA;
    counterAStructures = 0;
    PrintHeaderAStructure(pHeaderA);
    counterAStructures = 0;

    char cBegin = p->cBegin;
    headerD* pNext = p->pNext;
    headerD* pPrior = p->pPrior;

    counterDStructures++;

    if (pNext != NULL)
        PrintHeaderDStructure(pNext);
}

void PrintDataStructure(HEADER_D* p)
{
    counterDStructures = 0;
    PrintHeaderDStructure(p);
    counterDStructures = 0;
}

// Task 2
void InsertItemAsFirst(HEADER_A* pHeaderA, ITEM2* item)
{
    item->pNext = (ITEM2*)pHeaderA->pItems;
    pHeaderA->pItems = item;
}

void IncertItemInBetween(ITEM2* pItemPrev, ITEM2* pItemNext, ITEM2* pNewItem)
{
    pItemPrev->pNext = pNewItem;
    pNewItem->pNext = pItemNext;
}

bool StringsAreTheSame(char* a, char* b)
{
    return strcmp(a, b) == 0;
}

std::regex ItemIDFormat("^[A-Z][a-zA-Z-]* [a-zA-Z-]+$");
bool VerifyFormattingForItemID(char* ItemID)
{
    return std::regex_match(ItemID, ItemIDFormat);
}

bool ItemWithIDExists(HEADER_D* d, char* pItemID)
{
    // First D header is already given in parameters

    // Iterate D headers
    while (d != NULL)
    {
        // First A header
        HEADER_A* a = d->pHeaderA;

        // Itertate A headers
        while (a != NULL)
        {
            // First item
            ITEM2* i = (ITEM2*)a->pItems;

            // Iterate items
            while (i != NULL)
            {
                // Match item's ID to ID we are looking for
                if (StringsAreTheSame(i->pID, pItemID) == true)
                    return true; // FOUND IT

                // Switch to next item
                i = i->pNext;
            }

            // Switch to next header A
            a = a->pNext;
        }

        // Switch to next header D
        d = d->pNext;
    }

    // No matches, means none of the items has this given ID
    return false;
}

HEADER_D* InsertItem(HEADER_D* d, int PosStructD, int PosStructA, int Pos2Item, char* pNewItemID)
{
    if (pNewItemID != NULL && ItemWithIDExists(d, pNewItemID) == true)
    {
        PrintError("Item with such ID already exists!");
        return d;
    }
    if (pNewItemID != NULL && VerifyFormattingForItemID(pNewItemID) != true)
    {
        PrintError("New Item ID has incorrect formatting");
        return d;
    }

    ITEM2* pNewItem = (ITEM2*)GetItem(2, pNewItemID);
    std::cout << "Generated new item:" << std::endl;
    PrintItem2Structure(pNewItem);

    for (int i = 0; i < PosStructD; i++)
    {
        d = d->pNext;

        if (d == NULL)
        {
            PrintError("Reached the limit of D structures");
            return NULL;
        }
    }

    HEADER_A* a = d->pHeaderA;

    for (int i = 0; i < PosStructA; i++)
    {
        a = a->pNext;

        if (a == NULL)
        {
            PrintError("Reached the limit of A structures");
            return NULL;
        }
    }

    if (Pos2Item == 0)
    {
        InsertItemAsFirst(a, pNewItem);
        return d;
    }

    ITEM2* pItemPrev = (ITEM2*)a->pItems;
    ITEM2* pItemNext = pItemPrev->pNext;

    for (int i = 0; i < Pos2Item - 1; i++)
    {
        if (pItemNext == NULL)
        {
            PrintError("Reached the limit of items");
            return NULL;
        }

        pItemPrev = pItemNext;
        pItemNext = pItemPrev->pNext;
    }

    IncertItemInBetween(pItemPrev, pItemNext, pNewItem);

    return d;
}

// Task 3
HEADER_D* RemoveItem(HEADER_D* d, char* pRemoveItemID)
{
    if (pRemoveItemID == NULL)
    {
        PrintError("In order to remove an item ID must be specified");
        return d;
    }
    if (ItemWithIDExists(d, pRemoveItemID) != true)
    {
        PrintError("Item with such ID DOES NOT exist!");
        return d;
    }

    HEADER_D* originalD = d;

    // First D header is already given in parameters
    HEADER_D* dPrevious = NULL;

    // Iterate D headers
    while (d != NULL)
    {
        // First A header
        HEADER_A* a = d->pHeaderA;
        HEADER_A* aPrevious = NULL;

        // Itertate A headers
        while (a != NULL)
        {
            // First item
            ITEM2* i = (ITEM2*)a->pItems;
            ITEM2* iPrevious = NULL;

            // Iterate items
            while (i != NULL)
            {
                // Match item's ID to ID we are looking for
                if (StringsAreTheSame(i->pID, pRemoveItemID) == true)
                {
                    // REMOVING ITEM
                    ITEM2* iNext = i->pNext;
                    delete i;

                    if (iPrevious == NULL) // This is the first item in header A
                    {
                        if (iNext != NULL)
                        {
                            a->pItems = iNext;
                        }
                        else // No next and no previous
                        {
                            // REMOVING A HEADER
                            HEADER_A* aNext = a->pNext;
                            delete a;

                            if (aPrevious == NULL) // This is the first A header
                            {
                                if (aNext == NULL) // No next and no previous
                                {
                                    // REMOVING D HEADER
                                    HEADER_D* dNext = d->pNext;
                                    delete d;

                                    if (dPrevious == NULL) // This is the first d header
                                    {
                                        return dNext;
                                    }
                                    else
                                    {
                                        dPrevious->pNext = dNext;
                                    }
                                }
                                else
                                {
                                    d->pHeaderA = aNext;
                                }
                            }
                            else
                            {
                                aPrevious->pNext = aNext;
                            }

                        }
                    }
                    else
                    {
                        iPrevious->pNext = iNext;
                    }

                    // Item deleted, return
                    return originalD;
                }

                // Switch to next item
                iPrevious = i;
                i = i->pNext;
            }

            // Switch to next header A
            aPrevious = a;
            a = a->pNext;
        }

        // Switch to next header D
        dPrevious = d;
        d = d->pNext;
    }

    PrintError("Failed to remove item, ID not found");

    return d;
}

int main()
{
    std::cout << "######### Creating new data #########" << std::endl;
    int numberOfItems = 30;
    HEADER_D* p = GetStruct4(2, numberOfItems);
    std::cout << "Initial data with " << numberOfItems << " items: " << std::endl;
    PrintDataStructure(p);

    std::cout << "######### Inserting new data #########" << std::endl;
    const char* newItemCodes[] = { "Z A", "Z Z", "Z K", "A Z", "A A", "A K", "G Z", "G A", "G K", "M A", "M Ba", "M Bb", "M Z" };
    for (const char* newItemCode : newItemCodes)
        InsertItem(p, 0, 0, 0, (char*)newItemCode);

    std::cout << "######### Inserting invalid data #########" << std::endl;
    InsertItem(p, 0, 0, 0, (char*)"M Ba");
    InsertItem(p, 0, 0, 0, (char*)"Mba");

    std::cout << "######### Data with new items  #########" << std::endl;
    PrintDataStructure(p);

    std::cout << "######### Removing data that we just inserted #########" << std::endl;
    for (const char* newItemCode : newItemCodes)
        RemoveItem(p, (char*)newItemCode);

    std::cout << "######### Removing invalid data #########" << std::endl;
    RemoveItem(p, (char*)"M Ba");
    RemoveItem(p, (char*)"Mba");

    std::cout << "######### FINAL DATA #########" << std::endl;
    PrintDataStructure(p);

    return 0;
}

int mainForDebug()
{
    HEADER_D* p = GetStruct4(2, 20);
    std::cout << "Initial data:" << std::endl;
    PrintDataStructure(p);

    // char* pNewItemID = NULL; // (char*)"Wate-r Mania";
    // HEADER_D* pNew = InsertItem(p, 8, 1, 1, pNewItemID);
    // std::cout << "Incerted item:" << std::endl;
    // PrintDataStructure(pNew);

    char* pRemoveItemID = (char*)"Raw Umber";
    HEADER_D* pRemoved = RemoveItem(p, pRemoveItemID);
    std::cout << "Removed item:" << std::endl;
    PrintDataStructure(pRemoved);

    std::cout << "Finish" << std::endl;
    return 0;
}

// EOF