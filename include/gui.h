#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "inventory.h"

// Column identifiers for TreeView
enum {
    COL_ID = 0,
    COL_NAME,
    COL_QUANTITY,
    COL_PRICE,
    NUM_COLS
};

typedef struct {
    GtkWidget *window;
    GtkWidget *tree_view;
    GtkListStore *list_store;
    GtkWidget *search_entry;
    GtkWidget *name_entry;
    GtkWidget *quantity_entry;
    GtkWidget *price_entry;
    GtkWidget *add_button;
    GtkWidget *update_button;
    GtkWidget *delete_button;
    GtkWidget *status_label;
    
    Inventory *inventory;
    int selected_id;
} AppData;

// GUI initialization and setup
GtkWidget* create_main_window(AppData *app_data);
void setup_tree_view(AppData *app_data);
void setup_input_form(AppData *app_data, GtkWidget *container);
void setup_toolbar(AppData *app_data, GtkWidget *container);

// GUI update functions
void refresh_tree_view(AppData *app_data);
void update_status(AppData *app_data, const char *message);
void clear_input_fields(AppData *app_data);
void populate_input_fields(AppData *app_data, const InventoryItem *item);

// Signal handlers
void on_add_button_clicked(GtkWidget *widget, gpointer data);
void on_update_button_clicked(GtkWidget *widget, gpointer data);
void on_delete_button_clicked(GtkWidget *widget, gpointer data);
void on_tree_selection_changed(GtkTreeSelection *selection, gpointer data);
void on_search_entry_changed(GtkWidget *widget, gpointer data);
void on_column_header_clicked(GtkTreeViewColumn *column, gpointer data);
void on_save_clicked(GtkWidget *widget, gpointer data);
void on_load_clicked(GtkWidget *widget, gpointer data);

#endif