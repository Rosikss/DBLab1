#include <stdio.h>
#include "Master.h"
#include "Input.h"
#include "OutputEntities.h"
#include "Slave.h"
#include "Storage.h"
#include "Ingredient.h"


#define INDEXER_SIZE sizeof(struct Indexer)
int main()
{
    struct Storage client;
    struct Ingredient order;
    while (1)
    {
        int choice;
        int id;
        char error[20];
        printf("Acceptable Options:\n0 Insert Storage\n1 Get Storage\n2 Update Storage\n3 Delete Storage\n4 Print List of Storages\n5 Insert Ingredient\n6 Get Ingredient\n7 Update Ingredient\n8 Delete Ingredient \n9 Output List of Ingredients\n10 Info\n11 Exit\n");
        scanf("%d", &choice);
        switch (choice) {
            case 0:
                inputStorage(&client);
                insertStorage(client);
                break;
            case 1:
                printf("Enter ID: \n");
                scanf("%d", &id);
                if(getStorage(&client, id, error) != 0)
                {
                    OutputStorage(client);
                }
                else
                {
                    printf("Error: %s\n", error);
                }
                break;
            case 2:
                printf("Enter ID: \n");
                scanf("%d", &id);
                getStorage(&client, id, error);
                inputStorage(&client);
                if(updateStorage(client, error, id) == 0)
                    printf("%s", error);
                else
                    printf("Update was successfully\n");
                break;
            case 3:
                printf("Enter ID: \n");
                scanf("%d", &id);
                if(deleteStorage(id, error) != 0)
                   printf("Deleted successfully\n");
                else
                    printf("Error: %s\n", error);
                break;
            case 4:
                PrintListOfStorage(error);
                break;
            case 5:
                printf("Enter the Storage id\n");
                scanf("%d", &id);
                if(getStorage(&client, id, error) != 0)
                {
                    InputIngredient(&order);
                    order.StorageId = client.id;
                    insertIngredient(client, order, error);
                    printf("Insert was successfully \n");
                }
                else
                {
                    printf("Error %s\n", error);
                }
                break;
            case 6:
                printf("Enter the Storage id \n");
                scanf("%d", &id);
                if(getStorage(&client, id, error))
                {
                    printf("Enter the id of Ingredient \n");
                    scanf("%d", &id);
                    if(checkIfRecordExist(client, id,error))
                    {
                        getIngredient(client, &order, id, error);
                        OutputIngredient(order, client);
                    }
                    else
                    {
                        printf("Error %s\n", error);
                    }
                }
                else
                {
                    printf("Error %s\n", error);
                }
                  break;
            case 7:
                printf("Enter the Storage id \n");
                scanf("%d", &id);
                if(getStorage(&client, id, error))
                {
                    printf("Enter the id of Ingredient\n ");
                    scanf("%d", &id);
                    if(checkIfRecordExist(client, id,error))
                    {
                        getIngredient(client, &order, id, error);
                        InputIngredient(&order);
                        updateIngredient(order);
                        printf("Updating was successfully \n ");
                    }
                    else
                    {
                        printf("Error %s\n", error);
                    }
                }
                else
                {
                    printf("Error %s \n", error);
                }

                break;
            case 8:
                printf("Enter the Storage id \n");
                scanf("%d", &id);
                if(getStorage(&client, id, error))
                {
                    printf("Enter the id of Ingredient \n");
                    scanf("%d", &id);
                    if(checkIfRecordExist(client, id,error))
                    {
                        getIngredient(client, &order, id, error);
                        deleteIngredient(client, order, error);
                        printf("Deleted was successfully \n");
                    }
                    else
                    {
                        printf("Error %s \n ", error);
                    }
                }
                else
                {
                    printf("Error %s \n ", error);
                }
                break;
            case 9:
                printf("Enter the Storage id \n");
                scanf("%d", &id);
                if(getStorage(&client, id, error))
                {
                   if(client.ingredientCount != 0)
                   {
                       PrintList(client);
                   }
                   else
                   {
                       printf("This Storage has no Ingredients\n");
                   }
                }
                else
                {
                    printf("Error %s \n ", error);
                }
                break;
            case 10:
                info();
                break;
            case 11:
                return 0;
            default:
                printf("No command :( \n");
        }
        printf("\n");
    }
}
