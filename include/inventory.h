#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdbool.h>

#define MAX_NAME_LENGTH 50
#define MAX_ITEMS 1000

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    int quantity;
    float price;
} InventoryItem;

typedef struct {
    InventoryItem items[MAX_ITEMS];
    int count;
    int next_id;
} Inventory;

// Core inventory functions
void inventory_init(Inventory *inv);
int inventory_add_item(Inventory *inv, const char *name, int quantity, float price);
bool inventory_update_item(Inventory *inv, int id, const char *name, int quantity, float price);
bool inventory_delete_item(Inventory *inv, int id);
InventoryItem* inventory_find_by_id(Inventory *inv, int id);
int inventory_get_index_by_id(Inventory *inv, int id);

// Sorting functions
typedef enum {
    SORT_BY_ID,
    SORT_BY_NAME,
    SORT_BY_QUANTITY,
    SORT_BY_PRICE
} SortCriteria;

void inventory_sort(Inventory *inv, SortCriteria criteria, bool ascending);

// Search functions
int inventory_search(Inventory *inv, const char *query, InventoryItem *results, int max_results);

#endif