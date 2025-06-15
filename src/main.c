#include <gtk/gtk.h>
#include <stdio.h>
#include "inventory.h"
#include "gui.h"
#include "utils.h"

static void on_window_destroy(GtkWidget *widget, gpointer data) {
    (void)widget; // Suppress unused parameter warning
    (void)data;   // Suppress unused parameter warning
    gtk_main_quit();
}

static void setup_css() {
    GtkCssProvider *provider = gtk_css_provider_new();
    const char *css_data = 
        /* StockFlow Brand Colors */
        "window { "
        "    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); "
        "    color: #2c3e50; "
        "}"
        
        /* Main content area */
        ".main-content { "
        "    background-color: rgba(255, 255, 255, 0.95); "
        "    border-radius: 12px; "
        "    margin: 8px; "
        "    box-shadow: 0 8px 32px rgba(102, 126, 234, 0.2); "
        "}"
        
        /* Header/Title area */
        ".app-header { "
        "    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); "
        "    color: white; "
        "    padding: 16px 20px; "
        "    border-radius: 12px 12px 0 0; "
        "    box-shadow: 0 2px 12px rgba(102, 126, 234, 0.3); "
        "}"
        
        ".app-title { "
        "    font-size: 24px; "
        "    font-weight: bold; "
        "    text-shadow: 0 2px 4px rgba(0, 0, 0, 0.3); "
        "}"
        
        ".app-subtitle { "
        "    font-size: 12px; "
        "    opacity: 0.9; "
        "    margin-top: 4px; "
        "}"
        
        /* Toolbar */
        ".toolbar { "
        "    background: linear-gradient(to bottom, #f8f9ff, #e3e7ff); "
        "    padding: 8px 16px; "
        "    border-bottom: 1px solid #d1d9ff; "
        "}"
        
        /* Buttons with StockFlow branding */
        "button { "
        "    margin: 3px; "
        "    padding: 8px 16px; "
        "    border-radius: 6px; "
        "    border: none; "
        "    font-weight: 500; "
        "    transition: all 0.2s ease; "
        "}"
        
        "button:not(.toolbar button) { "
        "    background: linear-gradient(135deg, #667eea, #764ba2); "
        "    color: white; "
        "    box-shadow: 0 2px 8px rgba(102, 126, 234, 0.3); "
        "}"
        
        "button:hover:not(.toolbar button) { "
        "    background: linear-gradient(135deg, #5a6fd8, #6a4190); "
        "    box-shadow: 0 4px 12px rgba(102, 126, 234, 0.4); "
        "    transform: translateY(-1px); "
        "}"
        
        "button:active:not(.toolbar button) { "
        "    transform: translateY(0); "
        "    box-shadow: 0 2px 6px rgba(102, 126, 234, 0.3); "
        "}"
        
        /* Toolbar buttons */
        ".toolbar button { "
        "    background: linear-gradient(to bottom, #ffffff, #f0f2ff); "
        "    border: 1px solid #c7d2fe; "
        "    color: #4338ca; "
        "}"
        
        ".toolbar button:hover { "
        "    background: linear-gradient(to bottom, #f0f2ff, #e0e7ff); "
        "    border-color: #a5b4fc; "
        "}"
        
        /* Input fields */
        "entry { "
        "    border: 2px solid #e5e7eb; "
        "    border-radius: 6px; "
        "    padding: 8px 12px; "
        "    background: white; "
        "    transition: border-color 0.2s ease; "
        "}"
        
        "entry:focus { "
        "    border-color: #667eea; "
        "    box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.1); "
        "}"
        
        /* Search entry special styling */
        ".search-entry { "
        "    background: linear-gradient(to right, #ffffff, #f8fafc); "
        "    border: 2px solid #cbd5e1; "
        "    border-radius: 20px; "
        "    padding: 10px 16px; "
        "}"
        
        ".search-entry:focus { "
        "    border-color: #667eea; "
        "    background: white; "
        "}"
        
        /* TreeView */
        "treeview { "
        "    background: white; "
        "    border: 1px solid #e5e7eb; "
        "    border-radius: 6px; "
        "}"
        
        "treeview header button { "
        "    background: linear-gradient(to bottom, #f8fafc, #f1f5f9); "
        "    border: none; "
        "    border-right: 1px solid #e2e8f0; "
        "    color: #475569; "
        "    font-weight: 600; "
        "    padding: 12px 8px; "
        "}"
        
        "treeview header button:hover { "
        "    background: linear-gradient(to bottom, #e2e8f0, #cbd5e1); "
        "}"
        
        /* Input frame */
        ".input-frame { "
        "    margin: 16px; "
        "    padding: 20px; "
        "    background: linear-gradient(135deg, #ffffff, #f8fafc); "
        "    border-radius: 8px; "
        "    border: 1px solid #e2e8f0; "
        "    box-shadow: 0 1px 3px rgba(0, 0, 0, 0.1); "
        "}"
        
        ".input-frame label { "
        "    color: #374151; "
        "    font-weight: 600; "
        "    font-size: 14px; "
        "}"
        
        /* Status bar with StockFlow branding */
        ".status-bar { "
        "    background: linear-gradient(135deg, #667eea, #764ba2); "
        "    color: white; "
        "    padding: 8px 16px; "
        "    border-radius: 0 0 12px 12px; "
        "    font-weight: 500; "
        "    text-shadow: 0 1px 2px rgba(0, 0, 0, 0.2); "
        "}"
        
        /* Scrolled windows */
        "scrolledwindow { "
        "    border-radius: 6px; "
        "    border: 1px solid #e5e7eb; "
        "}"
        
        /* Frame titles */
        "frame > label { "
        "    color: #667eea; "
        "    font-weight: bold; "
        "    font-size: 16px; "
        "}";
    
    gtk_css_provider_load_from_data(provider, css_data, -1, NULL);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
    g_object_unref(provider);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    // Initialize application data
    AppData app_data = {0};
    Inventory inventory;
    inventory_init(&inventory);
    app_data.inventory = &inventory;
    app_data.selected_id = -1;
    
    // Setup CSS
    setup_css();
    
    // Create main window
    app_data.window = create_main_window(&app_data);
    g_signal_connect(app_data.window, "destroy", G_CALLBACK(on_window_destroy), NULL);
    
    // Load existing inventory if available
    if (load_inventory_from_file(&inventory, "inventory.csv")) {
        refresh_tree_view(&app_data);
        update_status(&app_data, "✅ StockFlow Ready - Inventory loaded successfully!");
    } else {
        update_status(&app_data, "🚀 StockFlow Ready - Welcome to Professional Inventory Management");
    }
    
    gtk_widget_show_all(app_data.window);
    gtk_main();
    
    return 0;
}