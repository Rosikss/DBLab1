#pragma once
#include "Master.h"

void OutputStorage(struct Storage storage)
{
    printf("Storage's numberOfProducts: %d\n", storage.numberOfProducts);
    printf("Storage's DateOfPurchase: %s\n", storage.DateOfPurchase);
    printf("Storage's Supplier: %s\n", storage.Supplier);
}

void OutputIngredient(struct Ingredient ingredient, struct Storage storage)
{
    printf("The storage id %d \n", storage.id);
    printf("Ingredient Name: %s\n", ingredient.Name);
    printf("Ingredient PricePerUnit: %lf\n", ingredient.PricePerUnit);
    printf("Ingredient MeasurementsUnit: %s\n", ingredient.MeasurementsUnit);
}
