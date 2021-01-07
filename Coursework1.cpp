
// S.

#include <iostream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"

int main()
{
    std::cout << "Hello World!\n";

    HEADER_C* p = GetStruct2(3, 100);
    ITEM3* pNewItem = (ITEM3*)GetItem(3);

    return 0;
}

// EOF