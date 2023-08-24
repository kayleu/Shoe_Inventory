#include <stdio.h>
#include <stdlib.h>

struct shoe {
    int stockNo;
    int styleNo;
    int sizes[12];
    float price;
};

// Function prototypes
void UserGuide();
void saveInventoryToFile(struct shoe *inventory, int count);
int findRecordByStockNo(struct shoe *inventory, int count, int stockNo);
int findRecordByStyleNo(struct shoe *inventory, int count, int styleNo);
void createNewInventory(struct shoe *inventory, int *count);
void readInventory(struct shoe *inventory, int count);
void updateInventory(struct shoe *inventory, int count);
void deleteInventory(struct shoe *inventory, int *count);
int confirmDeletion();

// Function prototype to load inventory from file
void loadInventoryFromFile(struct shoe *inventory, int *count);

int main() {
    struct shoe *inventory = (struct shoe *)malloc(sizeof(struct shoe));
    int count = 0;

    // Load existing inventory from file
    loadInventoryFromFile(inventory, &count);

    FILE *shoeinv = fopen("shoe_inventory.txt", "w");
    if (shoeinv == NULL) {
        printf("Error opening file for writing.\n");
        free(inventory);
        return 1;
    }
    fclose(shoeinv);

    int choice;
    while (1) {
        UserGuide();

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createNewInventory(inventory, &count);
                break;
            case 2:
                readInventory(inventory, count);
                break;
            case 3:
                updateInventory(inventory, count);
                break;
            case 4:
                deleteInventory(inventory, &count);
                break;
            case 5:
                if (confirmDeletion()) {
                    deleteInventory(inventory, &count);
                }
                break;
            case 6:
                printf("\nDo you confirm the exit? Press [1] - Yes. Press [0] - No: ");
                scanf("%d", &choice);
                if (choice == 1) {
                    printf("\nThank you for using our program!");
                    free(inventory);
                    return 0;
                }
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

void loadInventoryFromFile(struct shoe *inventory, int *count) {
    FILE *shoeinv = fopen("shoe_inventory.txt", "r");
    if (shoeinv == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    while (!feof(shoeinv)) {
        struct shoe newRecord;
        int read = fscanf(shoeinv, "%d %d %f", &newRecord.stockNo, &newRecord.styleNo, &newRecord.price);
        if (read != 3) {
            break; // EOF or error occurred
        }

        for (int i = 0; i < 12; i++) {
            fscanf(shoeinv, " %d", &newRecord.sizes[i]);
        }

        inventory[*count] = newRecord;
        (*count)++;
    }

    fclose(shoeinv);
}

void UserGuide() {
    printf("======================================================\n");
    printf("\t\t   SHOE INVENTORY\n");
    printf("======================================================\n");
    printf("\nChoose an action: ");
    printf("\n\n[1] Create a new inventory");
    printf("\n[2] Read inventory");
    printf("\n[3] Update inventory");
    printf("\n[4] Delete an existing inventory");
    printf("\n[5] Exit\n");
}

void saveInventoryToFile(struct shoe *inventory, int count) {
    FILE *shoeinv = fopen("shoe_inventory.txt", "a");
    if (shoeinv == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(shoeinv, "%d %d %.2f", inventory[i].stockNo, inventory[i].styleNo, inventory[i].price);
        for (int j = 0; j < 12; j++) {
            fprintf(shoeinv, " %d", inventory[i].sizes[j]);
        }
        fprintf(shoeinv, "\n");
    }

    fclose(shoeinv);
}

int findRecordByStockNo(struct shoe *inventory, int count, int stockNo) {
    for (int i = 0; i < count; i++) {
        if (inventory[i].stockNo == stockNo) {
            return i;
        }
    }
    return -1; // Record not found
}

int findRecordByStyleNo(struct shoe *inventory, int count, int styleNo) {
    for (int i = 0; i < count; i++) {
        if (inventory[i].styleNo == styleNo) {
            return i;
        }
    }
    return -1; // Record not found
}

void createNewInventory(struct shoe *inventory, int *count) {
    struct shoe newRecord;

    printf("Enter stock number: ");
    scanf("%d", &newRecord.stockNo);

    printf("Enter style number (0 to 50): ");
    scanf("%d", &newRecord.styleNo);

    printf("Enter price: ");
    scanf("%f", &newRecord.price);

    printf("\nNo. of Pairs for each size (3 - 14):\n");
    for (int i = 0; i < 12; i++) {
        printf("Size %d: ", i + 3);
        scanf("%d", &newRecord.sizes[i]);
    }

    inventory[*count] = newRecord;
    (*count)++;
    saveInventoryToFile(inventory, *count);
    printf("Record added successfully.\n");
}

void readInventory(struct shoe *inventory, int count) {

    if (count == 0) {
        printf("\n\tNo records found.\n\n");
        return;
    }

    int choice;
    printf("Choose what to read:\n");
    printf("[1] Read a specified stock number\n");
    printf("[2] Read a specified style number\n");
    printf("[3] Read size and no. of pairs per sizes\n");
    printf("[4] Read the price\n");
    printf("[5] Read the whole record\n");
    printf("[6] Read the entire inventory\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: {
            int stockNo;
            printf("Enter the stock number to read: ");
            scanf("%d", &stockNo);

            int index = findRecordByStockNo(inventory, count, stockNo);
            if (index == -1) {
                printf("\n\tRecord not found.\n\n");
                return;
            }

            printf("\n----------------------------------------------");
            printf("\nStock Number: %d", inventory[index].stockNo);
            printf("\nStyle Number: %d", inventory[index].styleNo);
            printf("\nPrice: %.2f", inventory[index].price);
            printf("\nSizes and No. of Pairs per Sizes:\n");

            for (int i = 0; i < 12; i++) {
                printf("Size %d: %d pairs\n", i + 3, inventory[index].sizes[i]);
            }

            printf("----------------------------------------------\n\n");
            break;
        }
        case 2: {
            int styleNo;
            printf("Enter the style number to read: ");
            scanf("%d", &styleNo);

            int index = findRecordByStyleNo(inventory, count, styleNo);
            if (index == -1) {
                printf("\n\tRecord not found.\n\n");
                return;
            }

            printf("\n----------------------------------------------");
            printf("\nStock Number: %d", inventory[index].stockNo);
            printf("\nStyle Number: %d", inventory[index].styleNo);
            printf("\nPrice: %.2f", inventory[index].price);
            printf("\nSizes and No. of Pairs per Sizes:\n");

            for (int i = 0; i < 12; i++) {
                printf("Size %d: %d pairs\n", i + 3, inventory[index].sizes[i]);
            }

            printf("----------------------------------------------\n\n");
            break;
        }
        case 3: {
            int stockOrStyleNo;
            printf("Enter stock number or style number to read sizes and no. of pairs per sizes: ");
            scanf("%d", &stockOrStyleNo);

            int index = findRecordByStockNo(inventory, count, stockOrStyleNo);
            if (index == -1) {
                index = findRecordByStyleNo(inventory, count, stockOrStyleNo);
            }

            if (index == -1) {
                printf("\n\tRecord not found.\n\n");
                return;
            }

            printf("\n----------------------------------------------");
            printf("\nSizes and No. of Pairs per Sizes for Stock/Style Number %d:\n", stockOrStyleNo);

            for (int i = 0; i < 12; i++) {
                printf("Size %d: %d pairs\n", i + 3, inventory[index].sizes[i]);
            }

            printf("----------------------------------------------\n\n");
            break;
        }
        case 4: {
            int stockOrStyleNo;
            printf("Enter stock number or style number to read the price: ");
            scanf("%d", &stockOrStyleNo);

            int index = findRecordByStockNo(inventory, count, stockOrStyleNo);
            if (index == -1) {
                index = findRecordByStyleNo(inventory, count, stockOrStyleNo);
            }

            if (index == -1) {
                printf("\n\tRecord not found.\n\n");
                return;
            }

            printf("\n----------------------------------------------");
            printf("\nPrice for Stock/Style Number %d: %.2f", stockOrStyleNo, inventory[index].price);
            printf("\n----------------------------------------------\n\n");
            break;
        }
        case 5: {
            int stockOrStyleNo;
            printf("Enter stock number or style number to read the whole record: ");
            scanf("%d", &stockOrStyleNo);

            int index = findRecordByStockNo(inventory, count, stockOrStyleNo);
            if (index == -1) {
                index = findRecordByStyleNo(inventory, count, stockOrStyleNo);
            }

            if (index == -1) {
                printf("\n\tRecord not found.\n\n");
                return;
            }

            printf("\n----------------------------------------------");
            printf("\nStock Number: %d", inventory[index].stockNo);
            printf("\nStyle Number: %d", inventory[index].styleNo);
            printf("\nPrice: %.2f", inventory[index].price);
            printf("\nSizes and No. of Pairs per Sizes:\n");

            for (int i = 0; i < 12; i++) {
                printf("Size %d: %d pairs\n", i + 3, inventory[index].sizes[i]);
            }

            printf("----------------------------------------------\n\n");
            break;
        }
        case 6: {
            printf("\n----------------------------------------------");
            printf("\n\t\t ENTIRE INVENTORY\n");
            printf("----------------------------------------------");

            for (int i = 0; i < count; i++) {
                printf("\nStock Number: %d", inventory[i].stockNo);
                printf("\nStyle Number: %d", inventory[i].styleNo);
                printf("\nPrice: %.2f", inventory[i].price);
                printf("\nSizes and No. of Pairs per Sizes:\n");

                for (int j = 0; j < 12; j++) {
                    printf("Size %d: %d pairs\n", j + 3, inventory[i].sizes[j]);
                }

                printf("----------------------------------------------\n");
            }

            printf("\n\n");
            break;
        }
        default:
            printf("Invalid choice. Please try again.\n");
    }
}

void updateInventory(struct shoe *inventory, int count) {
    int stockOrStyleNo;
    printf("Enter stock number or style number: ");
    scanf("%d", &stockOrStyleNo);

    int index = findRecordByStockNo(inventory, count, stockOrStyleNo);
    if (index == -1) {
        index = findRecordByStyleNo(inventory, count, stockOrStyleNo);
    }

    if (index == -1) {
        printf("\n\tRecord not found.\n\n");
        return;
    }

    int choice;
    printf("Choose what to update:\n");
    printf("[1] Price\n");
    printf("[2] Stock on hand for a specific size\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("\nCurrent Price: %.2f\n", inventory[index].price);
            printf("Enter new price: ");
            scanf("%f", &inventory[index].price);
            break;
        case 2: {
            int sizeToChange;
            printf("Enter the size (3 to 14) to change stock on hand: ");
            scanf("%d", &sizeToChange);

            if (sizeToChange >= 3 && sizeToChange <= 14) {
                printf("Enter the new stock on hand for size %d: ", sizeToChange);
                scanf("%d", &inventory[index].sizes[sizeToChange - 3]);
            } else {
                printf("Invalid size. Please enter a size between 3 and 14.\n");
            }
            break;
        }
        default:
            printf("Invalid choice. No updates performed.\n");
            return;
    }

    saveInventoryToFile(inventory, count);
    printf("Inventory updated successfully.\n");
}

void deleteInventory(struct shoe *inventory, int *count) {
    int choice;
    printf("\nChoose what to delete:\n");
    printf("[1] Delete a specific record by stock number or style number\n");
    printf("[2] Delete multiple records by stock number or style number\n");
    printf("[3] Delete entire inventory\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: {
            int stockOrStyleNo;
            printf("Enter stock number or style number to delete: ");
            scanf("%d", &stockOrStyleNo);

            int index = findRecordByStockNo(inventory, *count, stockOrStyleNo);
            if (index == -1) {
                index = findRecordByStyleNo(inventory, *count, stockOrStyleNo);
            }

            if (index == -1) {
                printf("\n\tRecord not found.\n\n");
                return;
            }

            // Shift remaining elements to overwrite the deleted element
            for (int i = index; i < *count - 1; i++) {
                inventory[i] = inventory[i + 1];
            }

            (*count)--;
            saveInventoryToFile(inventory, *count);
            printf("Inventory record deleted successfully.\n");
            break;
        }
        case 2: {
            printf("Enter the number of records you want to delete: ");
            int numRecords;
            scanf("%d", &numRecords);

            for (int i = 0; i < numRecords; i++) {
                deleteInventory(inventory, count);
            }
            break;
        }
        case 3: {
            *count = 0; // Set the count to zero to delete the entire inventory
            saveInventoryToFile(inventory, *count);
            printf("Entire inventory deleted successfully.\n");
            break;
        }
        default:
            printf("Invalid choice. No deletions performed.\n");
    }
}

int confirmDeletion() {
    int choice;
    printf("Are you sure you want to delete the inventory record? Press [1] - Yes. Press [0] - No: ");
    scanf("%d", &choice);
    return choice == 1;
}
