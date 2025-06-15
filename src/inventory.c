#define _GNU_SOURCE  // Enable GNU extensions including strcasecmp
#include "inventory.h"
#include "utils.h"    // For string_contains_ignore_case function
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>  // For strcasecmp

void inventory_init(Inventory *inv) {
    inv->count = 0;
    inv->next_id = 1;
    memset(inv->items, 0, sizeof(inv->items));
}

int inventory_add_item(Inventory *inv, const char *name, int quantity, float price) {
    if (inv->count >= MAX_ITEMS || !name || strlen(name) == 0) {
        return -1;
    }
    
    InventoryItem *item = &inv->items[inv->count];
    item->id = inv->next_id++;
    strncpy(item->name, name, MAX_NAME_LENGTH - 1);
    item->name[MAX_NAME_LENGTH - 1] = '\0';
    item->quantity = quantity;
    item->price = price;
    
    inv->count++;
    return item->id;
}

bool inventory_update_item(Inventory *inv, int id, const char *name, int quantity, float price) {
    InventoryItem *item = inventory_find_by_id(inv, id);
    if (!item || !name || strlen(name) == 0) {
        return false;
    }
    
    strncpy(item->name, name, MAX_NAME_LENGTH - 1);
    item->name[MAX_NAME_LENGTH - 1] = '\0';
    item->quantity = quantity;
    item->price = price;
    
    return true;
}

bool inventory_delete_item(Inventory *inv, int id) {
    int index = inventory_get_index_by_id(inv, id);
    if (index == -1) {
        return false;
    }
    
    // Shift remaining items down
    for (int i = index; i < inv->count - 1; i++) {
        inv->items[i] = inv->items[i + 1];
    }
    inv->count--;
    
    return true;
}

InventoryItem* inventory_find_by_id(Inventory *inv, int id) {
    for (int i = 0; i < inv->count; i++) {
        if (inv->items[i].id == id) {
            return &inv->items[i];
        }
    }
    return NULL;
}

int inventory_get_index_by_id(Inventory *inv, int id) {
    for (int i = 0; i < inv->count; i++) {
        if (inv->items[i].id == id) {
            return i;
        }
    }
    return -1;
}

static int compare_by_id(const void *a, const void *b) {
    const InventoryItem *item_a = (const InventoryItem *)a;
    const InventoryItem *item_b = (const InventoryItem *)b;
    return item_a->id - item_b->id;
}

static int compare_by_name(const void *a, const void *b) {
    const InventoryItem *item_a = (const InventoryItem *)a;
    const InventoryItem *item_b = (const InventoryItem *)b;
    return strcasecmp(item_a->name, item_b->name);
}

static int compare_by_quantity(const void *a, const void *b) {
    const InventoryItem *item_a = (const InventoryItem *)a;
    const InventoryItem *item_b = (const InventoryItem *)b;
    return item_a->quantity - item_b->quantity;
}

static int compare_by_price(const void *a, const void *b) {
    const InventoryItem *item_a = (const InventoryItem *)a;
    const InventoryItem *item_b = (const InventoryItem *)b;
    if (item_a->price < item_b->price) return -1;
    if (item_a->price > item_b->price) return 1;
    return 0;
}

void inventory_sort(Inventory *inv, SortCriteria criteria, bool ascending) {
    int (*compare_func)(const void *, const void *) = NULL;
    
    switch (criteria) {
        case SORT_BY_ID:
            compare_func = compare_by_id;
            break;
        case SORT_BY_NAME:
            compare_func = compare_by_name;
            break;
        case SORT_BY_QUANTITY:
            compare_func = compare_by_quantity;
            break;
        case SORT_BY_PRICE:
            compare_func = compare_by_price;
            break;
    }
    
    if (compare_func) {
        qsort(inv->items, inv->count, sizeof(InventoryItem), compare_func);
        
        // Reverse if descending order
        if (!ascending) {
            for (int i = 0; i < inv->count / 2; i++) {
                InventoryItem temp = inv->items[i];
                inv->items[i] = inv->items[inv->count - 1 - i];
                inv->items[inv->count - 1 - i] = temp;
            }
        }
    }
}

int inventory_search(Inventory *inv, const char *query, InventoryItem *results, int max_results) {
    if (!query || strlen(query) == 0) {
        // Return all items if no query
        int count = (inv->count < max_results) ? inv->count : max_results;
        memcpy(results, inv->items, count * sizeof(InventoryItem));
        return count;
    }
    
    int result_count = 0;
    for (int i = 0; i < inv->count && result_count < max_results; i++) {
        if (string_contains_ignore_case(inv->items[i].name, query)) {
            results[result_count++] = inv->items[i];
        }
    }
    
    return result_count;
}
