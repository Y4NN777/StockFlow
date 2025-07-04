#include "gui.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Only CSS class helper - NO deprecated functions
static void add_css_class(GtkWidget *widget, const char *class_name) {
    if (widget && class_name) {
        GtkStyleContext *context = gtk_widget_get_style_context(widget);
        gtk_style_context_add_class(context, class_name);
    }
}

GtkWidget* create_main_window(AppData *app_data) {
    // Create main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "StockFlow - Professional Inventory Management");
    gtk_window_set_default_size(GTK_WINDOW(window), 1200, 768);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    
    // Create main container
    GtkWidget *main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_vbox);
    
    // Create branded header with enhanced styling
    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    add_css_class(header_box, "app-header");
    gtk_box_pack_start(GTK_BOX(main_vbox), header_box, FALSE, FALSE, 0);
    
    // App title with large font
    GtkWidget *title_label = gtk_label_new("StockFlow");
    add_css_class(title_label, "app-title");
    gtk_widget_set_halign(title_label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(header_box), title_label, FALSE, FALSE, 0);
    
    // App subtitle with medium font
    GtkWidget *subtitle_label = gtk_label_new("Professional Inventory Management System");
    add_css_class(subtitle_label, "app-subtitle");
    gtk_widget_set_halign(subtitle_label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(header_box), subtitle_label, FALSE, FALSE, 0);
    
    // Create content container
    GtkWidget *content_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    add_css_class(content_container, "main-content");
    gtk_box_pack_start(GTK_BOX(main_vbox), content_container, TRUE, TRUE, 0);
    
    // Setup toolbar
    setup_toolbar(app_data, content_container);
    
    // Create horizontal paned widget
    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(content_container), paned, TRUE, TRUE, 0);
    
    // Setup left panel (tree view) with padding
    GtkWidget *left_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_container_set_border_width(GTK_CONTAINER(left_vbox), 20);
    gtk_paned_pack1(GTK_PANED(paned), left_vbox, TRUE, TRUE);
    
    // Search section title
    GtkWidget *search_title = gtk_label_new("🔍 Search Inventory");
    add_css_class(search_title, "section-title");
    gtk_widget_set_halign(search_title, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(left_vbox), search_title, FALSE, FALSE, 0);
    
    // Search entry with enhanced styling
    app_data->search_entry = gtk_search_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->search_entry), "Search by product name...");
    add_css_class(app_data->search_entry, "search-entry");
    gtk_widget_set_size_request(app_data->search_entry, -1, 40);
    g_signal_connect(app_data->search_entry, "search-changed", G_CALLBACK(on_search_entry_changed), app_data);
    gtk_box_pack_start(GTK_BOX(left_vbox), app_data->search_entry, FALSE, FALSE, 0);
    
    // Inventory table title
    GtkWidget *table_title = gtk_label_new("📊 Inventory Items");
    add_css_class(table_title, "section-title");
    gtk_widget_set_halign(table_title, GTK_ALIGN_START);
    gtk_widget_set_margin_top(table_title, 8);
    gtk_box_pack_start(GTK_BOX(left_vbox), table_title, FALSE, FALSE, 0);
    
    // Setup tree view with enhanced styling
    setup_tree_view(app_data);
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    add_css_class(scrolled, "inventory-table");
    gtk_container_add(GTK_CONTAINER(scrolled), app_data->tree_view);
    gtk_box_pack_start(GTK_BOX(left_vbox), scrolled, TRUE, TRUE, 0);
    
    // Setup right panel (input form) with enhanced spacing
    GtkWidget *right_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_container_set_border_width(GTK_CONTAINER(right_vbox), 16);
    gtk_paned_pack2(GTK_PANED(paned), right_vbox, FALSE, TRUE);
    setup_input_form(app_data, right_vbox);
    
    // Enhanced status bar
    app_data->status_label = gtk_label_new("🚀 StockFlow Ready - Professional Inventory Management");
    gtk_widget_set_halign(app_data->status_label, GTK_ALIGN_START);
    add_css_class(app_data->status_label, "status-bar");
    gtk_box_pack_end(GTK_BOX(content_container), app_data->status_label, FALSE, FALSE, 0);
    
    app_data->window = window;
    return window;
}

void setup_tree_view(AppData *app_data) {
    // Create list store
    app_data->list_store = gtk_list_store_new(NUM_COLS, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT, G_TYPE_FLOAT);
    
    // Create tree view with enhanced styling
    app_data->tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(app_data->list_store));
    add_css_class(app_data->tree_view, "data-table");
    
    // Create columns with enhanced styling
    const char *column_titles[] = {"ID", "Product Name", "Quantity", "Price ($)"};
    
    for (int i = 0; i < NUM_COLS; i++) {
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
        
        // Enhanced cell renderer setup
        g_object_set(renderer, 
                     "ypad", 8,
                     "xpad", 12,
                     NULL);
        
        GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(
            column_titles[i], renderer, "text", i, NULL);
        
        gtk_tree_view_column_set_resizable(column, TRUE);
        gtk_tree_view_column_set_sort_column_id(column, i);
        gtk_tree_view_column_set_clickable(column, TRUE);
        gtk_tree_view_column_set_expand(column, TRUE);
        
        // Right align price column
        if (i == COL_PRICE) {
            g_object_set(renderer, "xalign", 1.0, NULL);
        }
        
        gtk_tree_view_append_column(GTK_TREE_VIEW(app_data->tree_view), column);
        g_signal_connect(column, "clicked", G_CALLBACK(on_column_header_clicked), app_data);
    }
    
    // Setup selection
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(app_data->tree_view));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
    g_signal_connect(selection, "changed", G_CALLBACK(on_tree_selection_changed), app_data);
}

void setup_input_form(AppData *app_data, GtkWidget *container) {
    // Create frame with enhanced title
    GtkWidget *frame = gtk_frame_new("📝 Item Management");
    add_css_class(frame, "input-frame");
    gtk_box_pack_start(GTK_BOX(container), frame, FALSE, TRUE, 0);
    
    // Create main form container
    GtkWidget *form_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_container_set_border_width(GTK_CONTAINER(form_vbox), 20);
    gtk_container_add(GTK_CONTAINER(frame), form_vbox);
    
    // Create input grid with enhanced spacing
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 16);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 16);
    gtk_box_pack_start(GTK_BOX(form_vbox), grid, FALSE, FALSE, 0);
    
    // Name field with enhanced styling
    GtkWidget *name_label = gtk_label_new("📦 Product Name:");
    add_css_class(name_label, "form-label");
    gtk_widget_set_halign(name_label, GTK_ALIGN_END);
    app_data->name_entry = gtk_entry_new();
    add_css_class(app_data->name_entry, "form-input");
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->name_entry), "Enter product name...");
    gtk_widget_set_size_request(app_data->name_entry, 250, 42);
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), app_data->name_entry, 1, 0, 1, 1);
    
    // Quantity field with enhanced styling  
    GtkWidget *quantity_label = gtk_label_new("📊 Quantity:");
    add_css_class(quantity_label, "form-label");
    gtk_widget_set_halign(quantity_label, GTK_ALIGN_END);
    app_data->quantity_entry = gtk_entry_new();
    add_css_class(app_data->quantity_entry, "form-input");
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->quantity_entry), "Stock count...");
    gtk_widget_set_size_request(app_data->quantity_entry, 250, 42);
    gtk_grid_attach(GTK_GRID(grid), quantity_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), app_data->quantity_entry, 1, 1, 1, 1);
    
    // Price field with enhanced styling
    GtkWidget *price_label = gtk_label_new("💰 Price ($):");
    add_css_class(price_label, "form-label");
    gtk_widget_set_halign(price_label, GTK_ALIGN_END);
    app_data->price_entry = gtk_entry_new();
    add_css_class(app_data->price_entry, "form-input");
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->price_entry), "0.00");
    gtk_widget_set_size_request(app_data->price_entry, 250, 42);
    gtk_grid_attach(GTK_GRID(grid), price_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), app_data->price_entry, 1, 2, 1, 1);
    
    // Action buttons with enhanced styling
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_halign(button_box, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(form_vbox), button_box, FALSE, FALSE, 0);
    
    app_data->add_button = gtk_button_new_with_label("➕ Add Item");
    app_data->update_button = gtk_button_new_with_label("✏️ Update");
    app_data->delete_button = gtk_button_new_with_label("🗑️ Delete");
    
    // Apply button styling
    add_css_class(app_data->add_button, "primary-button");
    add_css_class(app_data->update_button, "secondary-button");
    add_css_class(app_data->delete_button, "danger-button");
    
    gtk_widget_set_size_request(app_data->add_button, 140, 44);
    gtk_widget_set_size_request(app_data->update_button, 140, 44);
    gtk_widget_set_size_request(app_data->delete_button, 140, 44);
    
    gtk_widget_set_sensitive(app_data->update_button, FALSE);
    gtk_widget_set_sensitive(app_data->delete_button, FALSE);
    
    gtk_box_pack_start(GTK_BOX(button_box), app_data->add_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), app_data->update_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), app_data->delete_button, FALSE, FALSE, 0);
    
    // Connect signals
    g_signal_connect(app_data->add_button, "clicked", G_CALLBACK(on_add_button_clicked), app_data);
    g_signal_connect(app_data->update_button, "clicked", G_CALLBACK(on_update_button_clicked), app_data);
    g_signal_connect(app_data->delete_button, "clicked", G_CALLBACK(on_delete_button_clicked), app_data);
}

void setup_toolbar(AppData *app_data, GtkWidget *container) {
    GtkWidget *toolbar = gtk_toolbar_new();
    add_css_class(toolbar, "main-toolbar");
    gtk_box_pack_start(GTK_BOX(container), toolbar, FALSE, FALSE, 0);
    
    // Save button with enhanced styling
    GtkToolItem *save_item = gtk_tool_button_new(NULL, "💾 Save Inventory");
    gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(save_item), "document-save");
    gtk_widget_set_tooltip_text(GTK_WIDGET(save_item), "Save inventory to CSV file");
    add_css_class(GTK_WIDGET(save_item), "toolbar-button");
    g_signal_connect(save_item, "clicked", G_CALLBACK(on_save_clicked), app_data);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save_item, -1);
    
    // Load button with enhanced styling
    GtkToolItem *load_item = gtk_tool_button_new(NULL, "📂 Load Inventory");
    gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(load_item), "document-open");
    gtk_widget_set_tooltip_text(GTK_WIDGET(load_item), "Load inventory from CSV file");
    add_css_class(GTK_WIDGET(load_item), "toolbar-button");
    g_signal_connect(load_item, "clicked", G_CALLBACK(on_load_clicked), app_data);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), load_item, -1);
    
    // Separator
    GtkToolItem *sep = gtk_separator_tool_item_new();
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep, -1);
    
    // Enhanced branding info
    GtkToolItem *brand_item = gtk_tool_item_new();
    GtkWidget *brand_label = gtk_label_new("StockFlow v1.0 - Professional Inventory Management");
    gtk_widget_set_margin_start(brand_label, 24);
    add_css_class(brand_label, "toolbar-text");
    gtk_container_add(GTK_CONTAINER(brand_item), brand_label);
    gtk_tool_item_set_expand(brand_item, TRUE);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), brand_item, -1);
}

void refresh_tree_view(AppData *app_data) {
    gtk_list_store_clear(app_data->list_store);
    
    const char *search_text = gtk_entry_get_text(GTK_ENTRY(app_data->search_entry));
    
    if (strlen(search_text) == 0) {
        // Show all items
        for (int i = 0; i < app_data->inventory->count; i++) {
            GtkTreeIter iter;
            InventoryItem *item = &app_data->inventory->items[i];
            gtk_list_store_append(app_data->list_store, &iter);
            gtk_list_store_set(app_data->list_store, &iter,
                COL_ID, item->id,
                COL_NAME, item->name,
                COL_QUANTITY, item->quantity,
                COL_PRICE, item->price,
                -1);
        }
    } else {
        // Show filtered items
        InventoryItem results[MAX_ITEMS];
        int count = inventory_search(app_data->inventory, search_text, results, MAX_ITEMS);
        
        for (int i = 0; i < count; i++) {
            GtkTreeIter iter;
            gtk_list_store_append(app_data->list_store, &iter);
            gtk_list_store_set(app_data->list_store, &iter,
                COL_ID, results[i].id,
                COL_NAME, results[i].name,
                COL_QUANTITY, results[i].quantity,
                COL_PRICE, results[i].price,
                -1);
        }
    }
}

void update_status(AppData *app_data, const char *message) {
    gtk_label_set_text(GTK_LABEL(app_data->status_label), message);
}

void clear_input_fields(AppData *app_data) {
    gtk_entry_set_text(GTK_ENTRY(app_data->name_entry), "");
    gtk_entry_set_text(GTK_ENTRY(app_data->quantity_entry), "");
    gtk_entry_set_text(GTK_ENTRY(app_data->price_entry), "");
    app_data->selected_id = -1;
    gtk_widget_set_sensitive(app_data->update_button, FALSE);
    gtk_widget_set_sensitive(app_data->delete_button, FALSE);
}

void populate_input_fields(AppData *app_data, const InventoryItem *item) {
    char buffer[32];
    
    gtk_entry_set_text(GTK_ENTRY(app_data->name_entry), item->name);
    
    snprintf(buffer, sizeof(buffer), "%d", item->quantity);
    gtk_entry_set_text(GTK_ENTRY(app_data->quantity_entry), buffer);
    
    snprintf(buffer, sizeof(buffer), "%.2f", item->price);
    gtk_entry_set_text(GTK_ENTRY(app_data->price_entry), buffer);
    
    app_data->selected_id = item->id;
    gtk_widget_set_sensitive(app_data->update_button, TRUE);
    gtk_widget_set_sensitive(app_data->delete_button, TRUE);
}

// Signal handlers (unchanged)
void on_add_button_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    AppData *app_data = (AppData *)data;
    
    const char *name = gtk_entry_get_text(GTK_ENTRY(app_data->name_entry));
    const char *quantity_str = gtk_entry_get_text(GTK_ENTRY(app_data->quantity_entry));
    const char *price_str = gtk_entry_get_text(GTK_ENTRY(app_data->price_entry));
    
    int quantity;
    float price;
    
    if (!validate_name(name)) {
        show_error_dialog(app_data->window, "Please enter a valid item name.");
        return;
    }
    
    if (!validate_quantity(quantity_str, &quantity)) {
        show_error_dialog(app_data->window, "Please enter a valid quantity (non-negative integer).");
        return;
    }
    
    if (!validate_price(price_str, &price)) {
        show_error_dialog(app_data->window, "Please enter a valid price (non-negative number).");
        return;
    }
    
    int id = inventory_add_item(app_data->inventory, name, quantity, price);
    if (id == -1) {
        show_error_dialog(app_data->window, "Failed to add item. Inventory might be full.");
        return;
    }
    
    refresh_tree_view(app_data);
    clear_input_fields(app_data);
    update_status(app_data, "✅ Item added successfully - StockFlow updated!");
}

void on_update_button_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    AppData *app_data = (AppData *)data;
    
    if (app_data->selected_id == -1) {
        show_error_dialog(app_data->window, "No item selected for update.");
        return;
    }
    
    const char *name = gtk_entry_get_text(GTK_ENTRY(app_data->name_entry));
    const char *quantity_str = gtk_entry_get_text(GTK_ENTRY(app_data->quantity_entry));
    const char *price_str = gtk_entry_get_text(GTK_ENTRY(app_data->price_entry));
    
    int quantity;
    float price;
    
    if (!validate_name(name) || !validate_quantity(quantity_str, &quantity) || !validate_price(price_str, &price)) {
        show_error_dialog(app_data->window, "Please enter valid values for all fields.");
        return;
    }
    
    if (inventory_update_item(app_data->inventory, app_data->selected_id, name, quantity, price)) {
        refresh_tree_view(app_data);
        clear_input_fields(app_data);
        update_status(app_data, "✏️ Item updated successfully - StockFlow synchronized!");
    } else {
        show_error_dialog(app_data->window, "Failed to update item.");
    }
}

void on_delete_button_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    AppData *app_data = (AppData *)data;
    
    if (app_data->selected_id == -1) {
        show_error_dialog(app_data->window, "No item selected for deletion.");
        return;
    }
    
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(app_data->window),
        GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
        "Are you sure you want to delete this item?");
    
    int response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    if (response == GTK_RESPONSE_YES) {
        if (inventory_delete_item(app_data->inventory, app_data->selected_id)) {
            refresh_tree_view(app_data);
            clear_input_fields(app_data);
            update_status(app_data, "🗑️ Item deleted successfully - StockFlow updated!");
        } else {
            show_error_dialog(app_data->window, "Failed to delete item.");
        }
    }
}

void on_tree_selection_changed(GtkTreeSelection *selection, gpointer data) {
    AppData *app_data = (AppData *)data;
    GtkTreeModel *model;
    GtkTreeIter iter;
    
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        int id;
        gtk_tree_model_get(model, &iter, COL_ID, &id, -1);
        
        InventoryItem *item = inventory_find_by_id(app_data->inventory, id);
        if (item) {
            populate_input_fields(app_data, item);
        }
    } else {
        clear_input_fields(app_data);
    }
}

void on_search_entry_changed(GtkWidget *widget, gpointer data) {
    (void)widget;
    AppData *app_data = (AppData *)data;
    refresh_tree_view(app_data);
    
    const char *search_text = gtk_entry_get_text(GTK_ENTRY(app_data->search_entry));
    if (strlen(search_text) > 0) {
        char status[256];
        snprintf(status, sizeof(status), "🔍 StockFlow Search: Found results for '%s'", search_text);
        update_status(app_data, status);
    } else {
        update_status(app_data, "📊 StockFlow: Displaying all inventory items");
    }
}

void on_column_header_clicked(GtkTreeViewColumn *column, gpointer data) {
    AppData *app_data = (AppData *)data;
    int column_id = gtk_tree_view_column_get_sort_column_id(column);
    
    SortCriteria criteria;
    switch (column_id) {
        case COL_ID: criteria = SORT_BY_ID; break;
        case COL_NAME: criteria = SORT_BY_NAME; break;
        case COL_QUANTITY: criteria = SORT_BY_QUANTITY; break;
        case COL_PRICE: criteria = SORT_BY_PRICE; break;
        default: return;
    }
    
    inventory_sort(app_data->inventory, criteria, true);
    refresh_tree_view(app_data);
    update_status(app_data, "📊 StockFlow: Inventory sorted and organized!");
}

void on_save_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    AppData *app_data = (AppData *)data;
    
    if (save_inventory_to_file(app_data->inventory, "inventory.csv")) {
        update_status(app_data, "💾 StockFlow: Inventory saved successfully!");
        show_info_dialog(app_data->window, "✅ Success!\n\nYour inventory has been saved to 'inventory.csv'.\nStockFlow keeps your data safe!");
    } else {
        show_error_dialog(app_data->window, "❌ Save Failed\n\nUnable to save inventory. Please check file permissions.");
    }
}

void on_load_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    AppData *app_data = (AppData *)data;
    
    if (load_inventory_from_file(app_data->inventory, "inventory.csv")) {
        refresh_tree_view(app_data);
        clear_input_fields(app_data);
        update_status(app_data, "📂 StockFlow: Inventory loaded successfully!");
        show_info_dialog(app_data->window, "✅ Welcome Back!\n\nYour inventory has been loaded from 'inventory.csv'.\nStockFlow is ready for action!");
    } else {
        show_error_dialog(app_data->window, "❌ Load Failed\n\nUnable to load inventory file. Please check if 'inventory.csv' exists.");
    }
}