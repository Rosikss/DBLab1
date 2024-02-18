#pragma once
#include <stdio.h>
#include <string.h>
#include "Master.h"
#include "time.h"
#include "Slave.h"
void inputStorage(struct Storage *storage)
{
    int numberOfProducts;
    char DateOfPurchase[10];
    char Supplier[20];

    printf("Enter Storage's NumberOfProducts: ");
    scanf("%d", &numberOfProducts);
    storage->numberOfProducts = numberOfProducts;

    printf("Enter Storage's DateOfPurchase: ");
    scanf("%s", DateOfPurchase);
    strcpy(storage->DateOfPurchase, DateOfPurchase);

    printf("Enter Storage's Supplier: ");
    scanf("%s", Supplier);
    strcpy(storage->Supplier, Supplier);
}

void InputIngredient(struct Ingredient *ingredient)
{
    char Name[50];
    double PricePerUnit;
    char MeasurementsUnit[50];

    printf("Enter Ingredient's Name: ");
    scanf("%s", Name);
    strcpy(ingredient->Name, Name);

    printf("Enter Ingredient's PricePerUnit: ");
    scanf("%lf", &PricePerUnit);
    ingredient->PricePerUnit = PricePerUnit;

    printf("Enter Ingredient's MeasurementsUnit: ");
    scanf("%s", MeasurementsUnit);
    strcpy(ingredient->MeasurementsUnit, MeasurementsUnit);
}