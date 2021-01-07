
// S.

#include <iomanip>
#include <iostream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"

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
HEADER_D* InsertItem(HEADER_D* d, int PosStructD = 0, int PosStructA = 0, int Pos2Item = 0, char* pNewItemID = NULL);

// Implementations
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
HEADER_D* InsertItem(HEADER_D* d, int PosStructD = 0, int PosStructA = 0, int Pos2Item = 0, char* pNewItemID = NULL)
{
    // TODO: Ensure ID does not exist
    // TODO: Ensure id formatting

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

    ITEM2* item = (ITEM2*)a->pItems;

    for (int i = 0; i < Pos2Item; i++)
    {
        ITEM2* itemNext = a->pNext;

        if (item == NULL)
        {
            PrintError("Reached the limit of items");
            return NULL;
        }

        a = item;
    }

    //ITEM2* pNewItem = (ITEM2*)GetItem(2, pNewItemID);

    return d;
}

int main()
{
    HEADER_D* p = GetStruct4(2, 20);
    PrintDataStructure(p);

    //HEADER_D* pNew = InsertItem(p, 5, 0, 0, NULL);
    //PrintDataStructure(p);

    return 0;
}

// EOF