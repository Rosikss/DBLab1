#pragma once
#include "Master.h"
#include "Slave.h"
#include "OutputEntities.h"

#define INGREDIENT_GARBAGE "ingredient_garbage.txt"

int updateStorage(struct Storage storage, char* error, int id);
struct Ingredient FindLastAddress(FILE *database, struct Storage *client, struct Ingredient *previous)
        {
    for (int i = 0; i < client->ingredientCount; i++) {
        fread(previous, INGREDIENT_SIZE, 1, database);
        fseek(database, previous->nextAddress, SEEK_SET);
    }
    return (*previous);
}
void NextAddress(FILE* database, struct Storage *storage, struct Ingredient *ingredient)
        {
            fclose(database);
            database = fopen(INGREDIENT_DATA, "r+b");
    struct Ingredient previous;
    fseek(database, storage->ingredientFirstAddress, SEEK_SET);
    previous = FindLastAddress(database, storage, &previous);
    previous.nextAddress = ingredient->selfAddress;
    fwrite(&previous, INGREDIENT_SIZE, 1, database);
}
void overwriteGarbageAddress(int garbageCount, FILE* garbageZone, struct Ingredient* record) {
    int * deletedIds = malloc(garbageCount * sizeof(int));
    for (int i = 0; i < garbageCount; i++) {
        fscanf(garbageZone, "%d", deletedIds + i);
    }

    record->selfAddress = deletedIds[0];
    record->nextAddress = deletedIds[0];
    fclose(garbageZone);
    garbageZone = fopen(INGREDIENT_GARBAGE, "wb");
    fprintf(garbageZone, "%d", garbageCount - 1);
    for (int i = 1; i < garbageCount; i++)
    {
        fprintf(garbageZone, " %d", deletedIds[i]);
    }
    free(deletedIds);
    fclose(garbageZone);
}
int insertIngredient(struct Storage storage, struct Ingredient ingredient, char * error)
{
    FILE* database = fopen(INGREDIENT_DATA, "a+b");
    FILE* garbageZone = fopen(INGREDIENT_GARBAGE, "a+b");
    int garbageCount = 0;
    fscanf(garbageZone, "%d", &garbageCount);
    ingredient.exists=1;
    struct Ingredient ingredient1;
    fseek(database, 0, SEEK_END);
    if (garbageCount != 0)
    {
        overwriteGarbageAddress(garbageCount, garbageZone, &ingredient);
        fclose(database);
        //database = fopen(ORDER_DATA, "r+b");
        database = fopen(INGREDIENT_DATA, "rb");
        fseek(database, ingredient.selfAddress, SEEK_SET);
        fread(&ingredient1, INGREDIENT_SIZE, 1, database);
        ingredient.Id= ingredient1.Id;
        fclose(database);
        database = fopen(INGREDIENT_DATA, "r+b");
        fseek(database, ingredient.selfAddress, SEEK_SET);
    }
    else
    {
        if (ftell(database) != 0) {
            fseek(database, -(INGREDIENT_SIZE), SEEK_END);
            fread(&ingredient1, INGREDIENT_SIZE, 1, database);
            ingredient.Id = ingredient1.Id + 1;
        }
        else
        {
            ingredient.Id = 1;
        }
        int dbSize = ftell(database);
        ingredient.selfAddress = dbSize;
        ingredient.nextAddress = dbSize;
        fseek(database, 0, SEEK_END);
    }
    printf("Your Ingredient id is %d \n", ingredient.Id);
    fwrite(&ingredient, INGREDIENT_SIZE, 1, database);
    if (!storage.ingredientCount)
    {
        storage.ingredientFirstAddress = ingredient.selfAddress;
    }
    else
    {
        NextAddress(database, &storage, &ingredient);
    }
    fclose(database);
    int count = storage.ingredientCount + 1;
    storage.ingredientCount=count;
    updateStorage(storage, error, storage.id);
    return 1;
}
int getIngredient(struct Storage storage, struct Ingredient* ingredient, int orderId, char* error) {

    FILE* database = fopen(INGREDIENT_DATA, "rb");
    fseek(database, storage.ingredientFirstAddress, SEEK_SET);
    fread(ingredient, INGREDIENT_SIZE, 1, database);

    for (int i = 0; i < storage.ingredientCount; i++) {
        if (ingredient->Id == orderId)
        {
            fclose(database);
            return 1;
        }
        fseek(database, ingredient->nextAddress, SEEK_SET);
        fread(ingredient, INGREDIENT_SIZE, 1, database);
    }
    fclose(database);
    return 0;
}
int updateIngredient(struct Ingredient ingredient)
        {
    FILE* database = fopen(INGREDIENT_DATA, "r+b");
    fseek(database, ingredient.selfAddress, SEEK_SET);
    fwrite(&ingredient, INGREDIENT_SIZE, 1, database);
    fclose(database);
    return 1;
}
void noteDeletedIngredient(int address)
{
    FILE* garbageZone = fopen(INGREDIENT_GARBAGE, "rb");
    int garbageCount;
    fscanf(garbageZone, "%d", &garbageCount);
    int* deletedAddresses = malloc(garbageCount * sizeof(long));

    for (int i = 0; i < garbageCount; i++) {
        fscanf(garbageZone, "%d", deletedAddresses + i);
    }
    fclose(garbageZone);
    garbageZone = fopen(INGREDIENT_GARBAGE, "wb");
    fprintf(garbageZone, "%d", garbageCount + 1);
    for (int i = 0; i < garbageCount; i++) {
        fprintf(garbageZone, " %d", deletedAddresses[i]);
    }

    fprintf(garbageZone, " %d", address);
    free(deletedAddresses);
    fclose(garbageZone);
}
void relinkAddresses(FILE* database, struct Ingredient previous, struct Ingredient ingredient, struct Storage* storage) {
    if (ingredient.selfAddress == storage->ingredientFirstAddress) {
        if (ingredient.selfAddress == ingredient.nextAddress) {
            storage->ingredientFirstAddress = -1;
        } else {
            storage->ingredientFirstAddress = ingredient.nextAddress;
        }
    } else {
        if (ingredient.selfAddress == ingredient.nextAddress) {
            previous.nextAddress = previous.selfAddress;
        }
        else {
            previous.nextAddress = ingredient.nextAddress;
        }
        fseek(database, previous.selfAddress, SEEK_SET);
        fwrite(&previous, INGREDIENT_SIZE, 1, database);
    }
}

void deleteIngredient(struct Storage storage, struct Ingredient ingredient, char* error)
        {
    FILE* database = fopen(INGREDIENT_DATA, "r+b");
    struct Ingredient previous;
    fseek(database, storage.ingredientFirstAddress, SEEK_SET);
    do {
        fread(&previous, INGREDIENT_SIZE, 1, database);
        fseek(database, previous.nextAddress, SEEK_SET);
    }
    while (previous.nextAddress != ingredient.selfAddress && ingredient.selfAddress != storage.ingredientFirstAddress);
    relinkAddresses(database, previous, ingredient, &storage);
            noteDeletedIngredient(ingredient.selfAddress);
            ingredient.exists = 0;

    fseek(database, ingredient.selfAddress, SEEK_SET);
    fwrite(&ingredient, INGREDIENT_SIZE, 1, database);
    fclose(database);

    storage.ingredientCount--;
            updateStorage(storage, error, storage.id);
}
void PrintList(struct Storage storage)
{
    FILE* ordersDb = fopen(INGREDIENT_DATA, "a+b");
    struct Ingredient order;
    fseek(ordersDb, storage.ingredientFirstAddress, SEEK_SET);
    for (int i = 0; i < storage.ingredientCount; i++) {
        fread(&order, INGREDIENT_SIZE, 1, ordersDb);
        if (order.exists)
        {
            OutputIngredient(order, storage);
            printf("<====================================>\n");
        }
        fseek(ordersDb, order.nextAddress, SEEK_SET);
    }
    fclose(ordersDb);
}