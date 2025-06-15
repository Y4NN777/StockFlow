#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <gtk/gtk.h>
#include "inventory.h"

// Input validation
bool validate_name(const char *name);
bool validate_quantity(const char *quantity_str, int *quantity);
bool validate_price(const char *price_str, float *price);

// File I/O
bool save_inventory_to_file(const Inventory *inv, const char *filename);
bool load_inventory_from_file(Inventory *inv, const char *filename);

// String utilities
void trim_string(char *str);
bool string_contains_ignore_case(const char *haystack, const char *needle);

// Error handling
void show_error_dialog(GtkWidget *parent, const char *message);
void show_info_dialog(GtkWidget *parent, const char *message);

#endif