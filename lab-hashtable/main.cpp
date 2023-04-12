#include <iostream>
#include "HashTable.h"
#pragma once


int main()
{
    HashTable HT;
    int choice = 0, inputNum;
    cout << "1-add number\n2-search number\n3-delete number from table\n4-print number of elements\n5-print number of buckets\n6-print whole table\n7-print help\n10-clear table and exit\n";

    while (choice != 10) {
        cin >> choice;
        inputNum = 0;
        switch (choice) {
        case 1:
            cout << "Enter the number you need (it should be between 1mln and 1.5mln): ";
            while (inputNum > 150000000 || inputNum < 100000000) cin >> inputNum;
            HT.insertItem(inputNum);
            break;
        case 2:
            cout << "Enter the number you need: ";
            cin >> inputNum;
            HT.searchItem(inputNum);
            break;
        case 3:
            cout << "Enter the number you need: ";
            cin >> inputNum;
            HT.removeItem(inputNum);
            break;
        case 4:
            cout << HT.elementsNum() << " is the number of elements.\n";
            break;
        case 5: 
            cout << HT.tableSize() << " is the number of buckets.\n";
            break;
        case 6:
            HT.out();
            break;
        case 7:
            cout << "1-add number\n2-search number\n3-delete number from table\n4-print number of elements\n5-print number of buckets\n6-print whole table\n7-print help\n10-clear table and exit\n";
            break;
        case 10:
            HT.clearTable();
            cout << "Bye!\n";
            break;
        default:
            cout << "Wrong Input!\n";
            break;
        }
    }
    return (0);
}
