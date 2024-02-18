#pragma once
struct Ingredient
{
    int Id;
    int StorageId;
    char Name[50];
    double PricePerUnit;
    char MeasurementsUnit[50];
    int exists;
    int selfAddress;
    int nextAddress;
};
