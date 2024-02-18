#pragma once
#include "time.h"
struct Storage
{
    int id;
    int numberOfProducts;
    char DateOfPurchase[10];
    char Supplier[50];
    int ingredientCount;
    int ingredientFirstAddress;
};
struct Indexer
{
    int id;
    int address;
    int exists;
};
