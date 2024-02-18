#pragma once
#include <malloc.h>
#include "Master.h"
#include "Input.h"
#include "Check.h"
#include "OutputEntities.h"
#include "Slave.h"
#include "Ingredient.h"


#define STORAGE_GARBAGE "storage_garbage.txt"
#define INDEXER_SIZE sizeof(struct Indexer)

int PrintListOfStorage(char *error);
void overwriteGarbageClientElement(int garbageCount, FILE* garbageZone, struct Storage* storage)
        {
    int* deleteIdsFile = malloc(garbageCount * sizeof(int));
    for (int i = 0; i < garbageCount; i++)
    {
        fscanf(garbageZone, "%d", deleteIdsFile + i);
    }
    fclose(garbageZone);
            storage->id = deleteIdsFile[0];
    fopen(STORAGE_GARBAGE, "wb");
    fprintf(garbageZone, "%d", garbageCount - 1);
    for (int i = 1; i < garbageCount; i++)
    {
        fprintf(garbageZone, " %d", deleteIdsFile[i]);
    }
    fclose(garbageZone);
    free(deleteIdsFile);
}
int insertStorage(struct Storage storage){
    FILE* garbageZone = fopen(STORAGE_GARBAGE, "a+b");
    FILE* indexTable = fopen(Storage_IND, "a+b");
    FILE* database = fopen(Storage_DATA, "a+b");
    int garbageCount = 0;
    fscanf(garbageZone, "%d", &garbageCount);
    struct Indexer indexer;
    if (garbageCount !=0 )
    {
        overwriteGarbageClientElement(garbageCount, garbageZone, &storage);
        fclose(database);
        fclose(indexTable);
        indexTable = fopen(Storage_IND, "r+b");
        database = fopen(Storage_DATA, "r+b");
        fseek(indexTable, (storage.id - 1) * INDEXER_SIZE, SEEK_SET);
        fread(&indexer, INDEXER_SIZE, 1, indexTable);
        fseek(database, indexer.address, SEEK_SET);
    }
    else {
        fseek(indexTable, 0, SEEK_END);
        if (ftell(indexTable) != 0) {
            fseek(indexTable, -(INDEXER_SIZE), SEEK_END);
            fread(&indexer, INDEXER_SIZE, 1, indexTable);
            storage.id = indexer.id + 1;
        } else {
            storage.id = 1;
        }
    }
    storage.ingredientFirstAddress = -1;
    storage.ingredientCount = 0;
    fwrite(&storage, Storage_SIZE, 1, database);
    indexer.id = storage.id;
    indexer.exists = 1;
    indexer.address = (storage.id - 1) * Storage_SIZE;
    printf("Your Storage id is %d\n", storage.id);
    fseek(indexTable, (storage.id - 1) * INDEXER_SIZE, SEEK_SET);
    fwrite(&indexer, INDEXER_SIZE, 1, indexTable);
    fclose(database);
    fclose(indexTable);
    return 1;
}
int getStorage(struct Storage* storage, int id, char* error)
        {
    FILE* indexTable = fopen(Storage_IND, "rb");
    FILE* database = fopen(Storage_DATA, "rb");
    if(checkFileExists(indexTable, database, error) == 0)
        return 0;
    struct Indexer indexer;
    if(checkIndexExists(indexTable, error, id) ==0)
        return 0;
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread(&indexer, INDEXER_SIZE, 1, indexTable);
    if (checkRecordExists(indexer, error)==0)
        return 0;
    fseek(database, indexer.address, SEEK_SET);
    fread(storage, Storage_SIZE, 1, database);
    fclose(indexTable);
    fclose(database);
    return 1;
}

int updateStorage(struct Storage storage, char* error, int id) {
    FILE* indexTable = fopen(Storage_IND, "r+b");
    FILE* database = fopen(Storage_DATA, "r+b");
    struct Indexer indexer;
    if (checkFileExists(indexTable, database, error)==0) {
        return 0;
    }
    if (checkIndexExists(indexTable, error, id)==0) {
        return 0;
    }
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread(&indexer, INDEXER_SIZE, 1, indexTable);
    if(checkRecordExists(indexer, error)==0)
        return 0;
    storage.id = id;
    fseek(database, indexer.address, SEEK_SET);
    fwrite(&storage, Storage_SIZE, 1, database);
    fclose(indexTable);
    fclose(database);
    return 1;
}
void listOfDeletedStorage(int id)
{
    FILE* garbage = fopen(STORAGE_GARBAGE, "rb");
    if (garbage == NULL)
    {
        printf("Error occured while opening file \n");
        return;
    }
    int garbageCount;
    fscanf(garbage, "%d", &garbageCount);
    int* deleteIds = malloc(garbageCount * sizeof(int));

    for (int i = 0; i < garbageCount; i++)
    {
        fscanf(garbage, "%d", deleteIds + i);
    }
    fclose(garbage);
    garbage = fopen(STORAGE_GARBAGE, "wb");
    fprintf(garbage, "%d", garbageCount + 1);
    for (int i = 0; i < garbageCount; i++) {
        fprintf(garbage, " %d", deleteIds[i]);
    }
    fprintf(garbage, " %d", id);
    free(deleteIds);
    fclose(garbage);
}
int deleteStorage(int id, char* error) {
    FILE* indexTable = fopen(Storage_IND, "r+b");
    struct Indexer indexer;
    if (indexTable == NULL) {
        strcpy(error, "database files are not created yet");
        return 0;
    }
    if (checkIndexExists(indexTable, error, id)==0) {
        return 0;
    }
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread(&indexer, INDEXER_SIZE, 1, indexTable);
    if(checkRecordExists(indexer, error)==0)
        return 0;
    struct Storage client;
    getStorage(&client, id, error);
    indexer.exists = 0;
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fwrite(&indexer, INDEXER_SIZE, 1, indexTable);
    fclose(indexTable);
    listOfDeletedStorage(id);
    if (client.ingredientCount != 0) {
        FILE* orderDb = fopen(INGREDIENT_DATA, "r+b");
        struct Ingredient order;
        fseek(orderDb, client.ingredientFirstAddress, SEEK_SET);
        for (int i = 0; i < client.ingredientCount; i++)
        {
            fread(&order, INGREDIENT_SIZE, 1, orderDb);
            fclose(orderDb);
            deleteIngredient(client, order, error);
            orderDb = fopen(INGREDIENT_DATA, "r+b");
            fseek(orderDb, order.nextAddress, SEEK_SET);
        }
        fclose(orderDb);
    }
    return 1;
}
 int PrintListOfStorage(char *error)
 {
     FILE* database = fopen(Storage_DATA, "rb");
     FILE* indexTable = fopen(Storage_IND, "rb");
     struct Storage client;
     struct Indexer indexer;
     if (checkFileExists(indexTable, database, error)==0)
     {
         return 0;
     }
     fseek(indexTable, -INDEXER_SIZE, SEEK_END);
     fread(&indexer, INDEXER_SIZE, 1, indexTable);
     rewind(indexTable);
     int endIndex =0;
     getStorage(&client, indexer.id, error);
     endIndex = indexer.id;
     int id = 0;
     int count =0;
     while(id < endIndex)
     {
         fseek(indexTable, id *INDEXER_SIZE, SEEK_SET);
         fread(&indexer, INDEXER_SIZE, 1, indexTable);
         if(indexer.exists)
         {
             printf("<=============================>\n");
             getStorage(&client, indexer.id, error);
             printf("Id %d\n", client.id);
             OutputStorage(client);
             count++;
         }
         id++;
     }
     if(count == 0)
     {
         printf("No Storages\n");
     }
     return 1;
 }


