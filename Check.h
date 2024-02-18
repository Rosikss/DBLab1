#pragma once


#include "Master.h"
#include "Storage.h"
#include "Check.h"

#define INDEXER_SIZE sizeof(struct Indexer)
#define Storage_DATA "storage.fl"
#define Storage_SIZE sizeof(struct Storage)
#define INGREDIENT_DATA "ingredient.fl"
#define INGREDIENT_SIZE sizeof(struct Ingredient)
#define Storage_IND "storage.ind"

int getStorage(struct Storage* storage, int id, char* error);
int checkFileExists(FILE* indexTable, FILE* database, char* error)
{
    if (indexTable == NULL || database == NULL) {
        strcpy(error, "DB file don`t exits");
        return 0;
    }
    return 1;
}
int checkIndexExists(FILE* indexTable, char* error, int id) {
    fseek(indexTable, 0, SEEK_END);
    long indexTableSize = ftell(indexTable);
    if (indexTableSize == 0 || id * INDEXER_SIZE > indexTableSize) {
        strcpy(error, "no this ID in the table");
        return 0;
    }
    return 1;
}
int checkRecordExists(struct Indexer indexer, char* error) {
    if (indexer.exists == 0 ) {
        strcpy(error, "the record was deleted");
        return 0;
    }
    return 1;
}
int checkIfRecordExist(struct Storage storage, int ingredientId, char *error ) {
    FILE* ordersDb = fopen(INGREDIENT_DATA, "a+b");
    struct Ingredient order;
    fseek(ordersDb, storage.ingredientFirstAddress, SEEK_SET);
    for (int i = 0; i < storage.ingredientCount; i++) {
        fread(&order, INGREDIENT_SIZE, 1, ordersDb);
        fclose(ordersDb);
        if (order.Id == ingredientId)
        {
            if(order.exists)
                return 1;
            else
            {
                strcpy(error, "Ingredient was deleted");
                return 0;
            }
        }
        ordersDb = fopen(INGREDIENT_DATA, "r+b");
        fseek(ordersDb, order.nextAddress, SEEK_SET);
    }
    strcpy(error, "The Storage do not have this Ingredient, the Ingredinet has been deleted or does not exist");
    fclose(ordersDb);
    return 0;
}
void info() {
    FILE* indexTable = fopen(Storage_IND, "rb");
    if (indexTable == NULL) {
        printf("Error: database files do not exist\n");
        return;
    }
    int storageCount = 0;
    int ingredientCount = 0;
    fseek(indexTable, 0, SEEK_END);
    int indAmount = ftell(indexTable) / sizeof(struct Indexer);
    struct Storage client;
    char error[51];
    for (int i = 1; i <= indAmount; i++)
    {
        if (getStorage(&client, i, error))
        {
            storageCount++;
            ingredientCount += client.ingredientCount;
            printf("Storage #%d has %d Ingredients\n", i, client.ingredientCount);
        }
    }
    fclose(indexTable);
    printf("Total Storages: %d\n", storageCount);
    printf("Total Ingredients: %d\n", ingredientCount);
}




