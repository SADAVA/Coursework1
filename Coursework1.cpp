
// S.

#include <iomanip>
#include <iostream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"

void PrintDataStructure(HEADER_D* p);
void PrintHeaderAStructure(HEADER_A* p);
void PrintItem2Structure(ITEM2* p);
void PrintTimeStructure(TIME* p);
void PrintCounter();
void PrintError(const char* s);

void PrintError(const char* s)
{
    std::cout << "[ERROR]" << s << std::endl;
}

unsigned int counter = 0;

void PrintCounter()
{
    // Prints:
    //   1) 
    //  23)
    // 999)
    // etc..
    counter++;
    std::cout << std::setfill(' ') << std::setw(3) << counter;
    std::cout << ") ";
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

    if (pNext != NULL)
        PrintItem2Structure(pNext);
}

void PrintHeaderAStructure(HEADER_A* p)
{
    if (p == NULL) return PrintError("Can't print Header A structure because it is empty");

    ITEM2* pItems = (ITEM2*)p->pItems;
    PrintItem2Structure(pItems);
    char cBegin = p->cBegin;
    struct headerA* pNext = p->pNext;

    if (pNext != NULL)
        PrintHeaderAStructure(pNext);
}

void PrintDataStructure(HEADER_D* p)
{
    if (p == NULL) return PrintError("Can't print data structure because it is empty");

    HEADER_A* pHeaderA = p->pHeaderA;
    PrintHeaderAStructure(pHeaderA);

    char cBegin = p->cBegin;
    headerD* pNext = p->pNext;
    headerD* pPrior = p->pPrior;

    if (pNext != NULL)
        PrintDataStructure(pNext);
}

int main()
{
    HEADER_D* p = GetStruct4(2, 20);
    // ITEM2* pNewItem = (ITEM2*)GetItem(2);

    PrintDataStructure(p);

    return 0;
}

// EOF