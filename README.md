# StockFlow - Professional Inventory Management System

🚀 **StockFlow** is a modern, cross-platform inventory management application built with C and GTK3, featuring professional branding and an intuitive user interface.

## ✨ Features

- 🎨 **Professional Branding** - Beautiful gradient UI with StockFlow design language
- ➕ **Smart Inventory Management** - Add, update, and delete inventory items with validation
- 🔍 **Real-time Search** - Live filtering with case-insensitive search
- 📊 **Intelligent Sorting** - Sortable columns (ID, Name, Quantity, Price)
- 💾 **Data Persistence** - Robust CSV format with automatic backup
- ✅ **Input Validation** - Prevents invalid data entry
- 🌐 **Cross-platform GUI** - Runs seamlessly on Linux, Windows, and macOS
- 🏗️ **Modular Architecture** - Clean, maintainable code structure

## 🎨 StockFlow Design

StockFlow features a modern gradient design with:
- **Primary Colors**: Purple-blue gradient (#667eea to #764ba2)
- **Professional Typography**: Clean, readable fonts with proper hierarchy
- **Intuitive Icons**: Emoji-based icons for better user experience
- **Responsive Layout**: Adaptive interface that works on different screen sizes

## Requirements

- GCC compiler
- GTK3 development libraries
- pkg-config

## 🚀 Quick Start with StockFlow

1. **Install Dependencies:**
   ```bash
   # Ubuntu/Debian
   make install-deps-ubuntu
   
   # CentOS/RHEL/Fedora
   make install-deps-redhat
   
   # macOS (with Homebrew)
   make install-deps-macos
   
   # Windows (with MSYS2)
   make install-deps-windows
Build and Launch StockFlow:

make
make run
Start Managing Your Inventory:

📦 Add products using the intuitive form
🔍 Search inventory in real-time
📊 Sort by any column
💾 Save/load your data automatically
Platform-Specific Instructions
Linux (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install build-essential pkg-config libgtk-3-dev
make
./bin/inventory_system
Linux (CentOS/RHEL/Fedora)
# CentOS/RHEL
sudo yum install gcc pkg-config gtk3-devel

# Fedora (newer versions)
sudo dnf install gcc pkg-config gtk3-devel

make
./bin/inventory_system
macOS
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install gtk+3 pkg-config

make
./bin/inventory_system
Windows (MSYS2)
Install MSYS2 from https://www.msys2.org/
Open MSYS2 terminal and run:
pacman -Syu
pacman -S mingw-w64-x86_64-gtk3 mingw-w64-x86_64-pkg-config mingw-w64-x86_64-gcc make

make
./bin/inventory_system.exe
Build Targets
make or make all - Build the application
make clean - Remove build artifacts
make run - Build and run the application
make debug - Build with debug symbols
make release - Build optimized release version
make check-deps - Verify required dependencies
make help - Show available targets
📖 Using StockFlow
📦 Adding Products
Fill in the "Product Name", "Quantity", and "Price" fields
Click "➕ Add Item" to add the product to your inventory
✏️ Updating Products
Select a product from the inventory list
Modify the fields as needed in the management panel
Click "✏️ Update" to save your changes
🗑️ Removing Products
Select a product from the inventory list
Click "🗑️ Delete" and confirm the action
🔍 Smart Search
Use the search bar to filter products by name
Search is case-insensitive and updates as you type
Clear the search to view all inventory
📊 Intelligent Sorting
Click any column header to sort by that criterion
Click again to reverse the sort order
Supports sorting by ID, Name, Quantity, or Price
💾 Data Management
Click "💾 Save Inventory" to export to CSV
Click "📂 Load Inventory" to import from CSV
StockFlow automatically loads your data on startup
📄 StockFlow Data Format
StockFlow saves your inventory in a clean, portable CSV format that's compatible with Excel and other spreadsheet applications:

ID,Name,Quantity,Price
1,"Gaming Laptop",15,1299.99
2,"Wireless Mouse",75,29.99
3,"Mechanical Keyboard",23,89.95
This format ensures your data is:

✅ Portable - Works with any CSV-compatible application
✅ Human-readable - Easy to view and edit manually if needed
✅ Future-proof - Standard format that will always be supported
Code Structure
src/
├── main.c      - Application entry point
├── inventory.c - Core inventory logic
├── gui.c       - GTK GUI implementation
└── utils.c     - Utilities and file I/O

include/
├── inventory.h - Inventory data structures
├── gui.h       - GUI function prototypes
└── utils.h     - Utility function prototypes
🤝 Contributing to StockFlow
StockFlow follows professional modular programming principles with clean separation of concerns:

inventory.c/h - Core business logic, platform-independent
gui.c/h - StockFlow-branded GTK interface implementation
utils.c/h - Cross-cutting utilities (validation, file I/O)
main.c - Application initialization and StockFlow branding
The codebase is designed for:

🏗️ Modularity - Easy to extend and maintain
🎨 Consistency - Uniform StockFlow design language
🔧 Reliability - Robust error handling and validation
🌐 Portability - Cross-platform compatibility
📜 License
StockFlow is released under the MIT License - build amazing inventory solutions!

