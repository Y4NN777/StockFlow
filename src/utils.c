#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <gtk/gtk.h>

bool validate_name(const char *name) {
    if (!name || strlen(name) == 0) {
        return false;
    }
    
    // Check if name contains only valid characters (letters, numbers, spaces, common punctuation)
    for (const char *p = name; *p; p++) {
        if (!isalnum(*p) && *p != ' ' && *p != '-' && *p != '_' && *p != '.' && *p != '(' && *p != ')') {
            return false;
        }
    }
    
    return true;
}

bool validate_quantity(const char *quantity_str, int *quantity) {
    if (!quantity_str || strlen(quantity_str) == 0) {
        return false;
    }
    
    char *endptr;
    long val = strtol(quantity_str, &endptr, 10);
    
    if (*endptr != '\0' || val < 0 || val > INT_MAX) {
        return false;
    }
    
    *quantity = (int)val;
    return true;
}

bool validate_price(const char *price_str, float *price) {
    if (!price_str || strlen(price_str) == 0) {
        return false;
    }
    
    char *endptr;
    double val = strtod(price_str, &endptr);
    
    if (*endptr != '\0' || val < 0.0) {
        return false;
    }
    
    *price = (float)val;
    return true;
}

bool save_inventory_to_file(const Inventory *inv, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        return false;
    }
    
    // Write CSV header
    fprintf(file, "ID,Name,Quantity,Price\n");
    
    // Write inventory items
    for (int i = 0; i < inv->count; i++) {
        const InventoryItem *item = &inv->items[i];
        fprintf(file, "%d,\"%s\",%d,%.2f\n", item->id, item->name, item->quantity, item->price);
    }
    
    fclose(file);
    return true;
}

bool load_inventory_from_file(Inventory *inv, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return false;
    }
    
    inventory_init(inv);
    
    char line[256];
    bool first_line = true;
    int max_id = 0;
    
    while (fgets(line, sizeof(line), file) && inv->count < MAX_ITEMS) {
        // Skip header line
        if (first_line) {
            first_line = false;
            continue;
        }
        
        // Parse CSV line
        char name[MAX_NAME_LENGTH];
        int id, quantity;
        float price;
        
        // Simple CSV parsing (handles quoted names)
        char *token = strtok(line, ",");
        if (!token) continue;
        id = atoi(token);
        
        token = strtok(NULL, ",");
        if (!token) continue;
        // Remove quotes if present
        if (token[0] == '"') {
            token++;
            char *end_quote = strrchr(token, '"');
            if (end_quote) *end_quote = '\0';
        }
        strncpy(name, token, MAX_NAME_LENGTH - 1);
        name[MAX_NAME_LENGTH - 1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue;
        quantity = atoi(token);
        
        token = strtok(NULL, ",\n");
        if (!token) continue;
        price = atof(token);
        
        // Add item to inventory
        InventoryItem *item = &inv->items[inv->count];
        item->id = id;
        strcpy(item->name, name);
        item->quantity = quantity;
        item->price = price;
        inv->count++;
        
        if (id > max_id) {
            max_id = id;
        }
    }
    
    inv->next_id = max_id + 1;
    fclose(file);
    return true;
}

void trim_string(char *str) {
    if (!str) return;
    
    // Trim leading whitespace
    char *start = str;
    while (isspace(*start)) start++;
    
    // Trim trailing whitespace
    char *end = str + strlen(str) - 1;
    while (end > start && isspace(*end)) end--;
    
    // Copy trimmed string
    size_t len = end - start + 1;
    memmove(str, start, len);
    str[len] = '\0';
}

bool string_contains_ignore_case(const char *haystack, const char *needle) {
    if (!haystack || !needle) return false;
    
    char *haystack_lower = g_ascii_strdown(haystack, -1);
    char *needle_lower = g_ascii_strdown(needle, -1);
    
    bool result = (strstr(haystack_lower, needle_lower) != NULL);
    
    g_free(haystack_lower);
    g_free(needle_lower);
    
    return result;
}

void show_error_dialog(GtkWidget *parent, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
        GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void show_info_dialog(GtkWidget *parent, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
        GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}