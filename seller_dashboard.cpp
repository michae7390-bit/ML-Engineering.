#include "seller_dashboard.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

void SellerDashboard::initializeData() {
    // Initialize sample products
    products.push_back(Product(1, "Smart Watch Pro", 299.99, 45, "Electronics", "Advanced fitness tracking", 4.5, 234));
    products.push_back(Product(2, "Wireless Headphones", 149.99, 128, "Electronics", "Noise-cancelling technology", 4.7, 567));
    products.push_back(Product(3, "USB-C Cable", 12.99, 500, "Accessories", "Premium quality 2-meter cable", 4.3, 1200));
    products.push_back(Product(4, "Phone Stand", 24.99, 200, "Accessories", "Adjustable metal stand", 4.2, 456));
    products.push_back(Product(5, "Webcam HD", 89.99, 67, "Electronics", "1080p with microphone", 4.6, 189));
    
    // Initialize sample orders
    orders.push_back(Order(1001, 101, {1, 3}, 312.98, "2024-01-15"));
    orders.push_back(Order(1002, 102, {2}, 149.99, "2024-01-16"));
    orders.push_back(Order(1003, 103, {4, 5}, 114.98, "2024-01-17"));
    orders.push_back(Order(1004, 104, {1, 2, 3}, 462.97, "2024-01-18"));
    orders.push_back(Order(1005, 105, {5}, 89.99, "2024-01-19"));
    
    orders[0].status = "Delivered";
    orders[1].status = "Shipped";
    orders[2].status = "Processing";
    orders[3].status = "Pending";
    orders[4].status = "Delivered";
    
    // Initialize revenue history
    revenueHistory.push_back(Revenue("2024-01-15", 12500.50, 45));
    revenueHistory.push_back(Revenue("2024-01-16", 15234.75, 52));
    revenueHistory.push_back(Revenue("2024-01-17", 13890.25, 48));
    revenueHistory.push_back(Revenue("2024-01-18", 18765.00, 61));
    revenueHistory.push_back(Revenue("2024-01-19", 16345.50, 55));
    
    // Calculate total revenue
    for (const auto& rev : revenueHistory) {
        totalRevenue += rev.amount;
    }
    
    // Calculate average rating
    double ratingSum = 0;
    for (const auto& product : products) {
        ratingSum += product.rating;
    }
    totalRating = ratingSum / products.size();
}

void SellerDashboard::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void SellerDashboard::displayHeader() {
    std::cout << Colors::BG_GRAY << Colors::BOLD << Colors::BRIGHT_CYAN;
    drawBorder(100);
    std::cout << "║" << std::setw(98) << "   SELLER DASHBOARD   " << "║" << std::endl;
    std::cout << "║" << std::setw(98) << "" << "║" << std::endl;
    drawBorder(100);
    std::cout << Colors::RESET;
}

void SellerDashboard::displayFooter() {
    std::cout << Colors::BOLD << Colors::BRIGHT_CYAN;
    drawBorder(100);
    std::cout << Colors::RESET;
}

void SellerDashboard::drawBorder(int width) {
    std::cout << "╔";
    for (int i = 0; i < width - 2; ++i) std::cout << "═";
    std::cout << "╗" << std::endl;
}

std::string SellerDashboard::formatCurrency(double amount) {
    std::stringstream ss;
    ss << "$" << std::fixed << std::setprecision(2) << amount;
    return ss.str();
}

std::string SellerDashboard::getStatusColor(const std::string& status) {
    if (status == "Delivered") return Colors::BRIGHT_GREEN;
    if (status == "Shipped") return Colors::BRIGHT_BLUE;
    if (status == "Processing") return Colors::BRIGHT_YELLOW;
    if (status == "Pending") return Colors::BRIGHT_MAGENTA;
    if (status == "Cancelled") return Colors::RED;
    return Colors::WHITE;
}

void SellerDashboard::pauseForUser() {
    std::cout << "\n" << Colors::BRIGHT_YELLOW << "Press Enter to continue..." << Colors::RESET;
    std::cin.ignore();
}

bool SellerDashboard::confirmAction(const std::string& message) {
    std::cout << Colors::BRIGHT_YELLOW << message << " (y/n): " << Colors::RESET;
    char response;
    std::cin >> response;
    return response == 'y' || response == 'Y';
}

void SellerDashboard::displayMainDashboard() {
    while (true) {
        clearScreen();
        displayHeader();
        
        // Seller info
        std::cout << Colors::BOLD << Colors::BRIGHT_CYAN << "\n📊 SELLER INFORMATION\n" << Colors::RESET;
        std::cout << "├─ Seller ID: " << Colors::BRIGHT_BLUE << sellerId << Colors::RESET << std::endl;
        std::cout << "├─ Seller Name: " << Colors::BRIGHT_BLUE << sellerName << Colors::RESET << std::endl;
        std::cout << "├─ Total Revenue: " << Colors::BRIGHT_GREEN << formatCurrency(totalRevenue) << Colors::RESET << std::endl;
        std::cout << "├─ Average Rating: " << Colors::BRIGHT_YELLOW << std::fixed << std::setprecision(2) << totalRating << " ★" << Colors::RESET << std::endl;
        std::cout << "├─ Total Products: " << Colors::BRIGHT_BLUE << products.size() << Colors::RESET << std::endl;
        std::cout << "└─ Total Orders: " << Colors::BRIGHT_BLUE << orders.size() << Colors::RESET << std::endl;
        
        // Quick stats
        std::cout << "\n" << Colors::BOLD << Colors::BRIGHT_CYAN << "📈 QUICK STATS\n" << Colors::RESET;
        
        double avgOrderValue = totalRevenue / orders.size();
        int totalSold = 0;
        for (const auto& prod : products) totalSold += prod.sold;
        
        std::cout << "├─ Average Order Value: " << Colors::BRIGHT_GREEN << formatCurrency(avgOrderValue) << Colors::RESET << std::endl;
        std::cout << "├─ Total Units Sold: " << Colors::BRIGHT_YELLOW << totalSold << Colors::RESET << std::endl;
        std::cout << "└─ Total Inventory: " << Colors::BRIGHT_BLUE;
        
        int totalInventory = 0;
        for (const auto& prod : products) totalInventory += prod.quantity;
        std::cout << totalInventory << Colors::RESET << " units" << std::endl;
        
        // Menu
        std::cout << "\n" << Colors::BOLD << Colors::BRIGHT_MAGENTA << "═══════════════════════════════════════════════════\n";
        std::cout << "                    MAIN MENU\n";
        std::cout << "═══════════════════════════════════════════════════\n" << Colors::RESET;
        
        std::cout << Colors::BRIGHT_CYAN << "1." << Colors::RESET << " View All Products\n";
        std::cout << Colors::BRIGHT_CYAN << "2." << Colors::RESET << " View All Orders\n";
        std::cout << Colors::BRIGHT_CYAN << "3." << Colors::RESET << " View Revenue Analytics\n";
        std::cout << Colors::BRIGHT_CYAN << "4." << Colors::RESET << " Add New Product\n";
        std::cout << Colors::BRIGHT_CYAN << "5." << Colors::RESET << " Update Product Stock\n";
        std::cout << Colors::BRIGHT_CYAN << "6." << Colors::RESET << " Process Order\n";
        std::cout << Colors::BRIGHT_CYAN << "7." << Colors::RESET << " Inventory Warnings\n";
        std::cout << Colors::BRIGHT_CYAN << "8." << Colors::RESET << " View Profile\n";
        std::cout << Colors::BRIGHT_CYAN << "9." << Colors::RESET << " Exit\n";
        
        std::cout << "\n" << Colors::BRIGHT_YELLOW << "Enter your choice (1-9): " << Colors::RESET;
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                displayProducts();
                break;
            case 2:
                displayOrders();
                break;
            case 3:
                displayRevenue();
                break;
            case 4:
                addProduct();
                break;
            case 5:
                updateProductStock();
                break;
            case 6:
                processOrder();
                break;
            case 7:
                displayInventoryWarnings();
                break;
            case 8:
                displayProfile();
                break;
            case 9:
                clearScreen();
                std::cout << Colors::BRIGHT_GREEN << "\n Thank you for using Seller Dashboard! Goodbye!\n" << Colors::RESET;
                return;
            default:
                std::cout << Colors::RED << "\n❌ Invalid choice! Please try again." << Colors::RESET;
                pauseForUser();
        }
    }
}

void SellerDashboard::displayProducts() {
    clearScreen();
    displayHeader();
    
    std::cout << Colors::BOLD << Colors::BRIGHT_CYAN << "\n📦 ALL PRODUCTS\n" << Colors::RESET;
    drawBorder(120);
    
    // Table header
    std::cout << "║ " << Colors::BOLD << std::setw(3) << "ID" 
              << " │ " << std::setw(20) << "Product Name"
              << " │ " << std::setw(12) << "Price"
              << " │ " << std::setw(10) << "Stock"
              << " │ " << std::setw(15) << "Category"
              << " │ " << std::setw(8) << "Rating"
              << " │ " << std::setw(8) << "Sold"
              << " │ " << Colors::RESET << std::endl;
    
    drawBorder(120);
    
    for (const auto& product : products) {
        std::cout << "║ " << std::setw(3) << product.id
                  << " │ " << std::setw(20) << product.name.substr(0, 20)
                  << " │ " << Colors::BRIGHT_GREEN << std::setw(12) << formatCurrency(product.price) << Colors::RESET
                  << " │ " << Colors::BRIGHT_BLUE << std::setw(10) << product.quantity << Colors::RESET
                  << " │ " << std::setw(15) << product.category.substr(0, 15)
                  << " │ " << Colors::BRIGHT_YELLOW << std::setw(8) << std::fixed << std::setprecision(1) << product.rating << "★" << Colors::RESET
                  << " │ " << Colors::BRIGHT_MAGENTA << std::setw(8) << product.sold << Colors::RESET
                  << " │" << std::endl;
    }
    
    drawBorder(120);
    
    std::cout << Colors::DIM << "\nTotal products: " << products.size() << Colors::RESET << std::endl;
    pauseForUser();
}

void SellerDashboard::displayOrders() {
    clearScreen();
    displayHeader();
    
    std::cout << Colors::BOLD << Colors::BRIGHT_CYAN << "\n📋 ALL ORDERS\n" << Colors::RESET;
    drawBorder(110);
    
    // Table header
    std::cout << "║ " << Colors::BOLD << std::setw(8) << "Order ID"
              << " │ " << std::setw(12) << "Customer"
              << " │ " << std::setw(15) << "Amount"
              << " │ " << std::setw(15) << "Status"
              << " │ " << std::setw(15) << "Date"
              << " │ " << Colors::RESET << std::endl;
    
    drawBorder(110);
    
    for (const auto& order : orders) {
        std::cout << "║ " << std::setw(8) << order.orderId
                  << " │ " << std::setw(12) << "CUST-" + std::to_string(order.customerId)
                  << " │ " << Colors::BRIGHT_GREEN << std::setw(15) << formatCurrency(order.totalAmount) << Colors::RESET
                  << " │ " << getStatusColor(order.status) << std::setw(15) << order.status << Colors::RESET
                  << " │ " << std::setw(15) << order.orderDate
                  << " │" << std::endl;
    }
    
    drawBorder(110);
    
    std::cout << "\n" << Colors::BRIGHT_YELLOW << "Enter Order ID to view details (0 to skip): " << Colors::RESET;
    int orderId;
    std::cin >> orderId;
    
    if (orderId != 0) {
        viewOrderDetails(orderId);
    }
    
    pauseForUser();
}

void SellerDashboard::viewOrderDetails(int orderId) {
    auto it = std::find_if(orders.begin(), orders.end(),
                          [orderId](const Order& o) { return o.orderId == orderId; });
    
    if (it != orders.end()) {
        clearScreen();
        std::cout << Colors::BOLD << Colors::BRIGHT_CYAN << "\n📄 ORDER DETAILS\n" << Colors::RESET;
        std::cout << "├─ Order ID: " << Colors::BRIGHT_BLUE << it->orderId << Colors::RESET << std::endl;
        std::cout << "├─ Customer ID: " << Colors::BRIGHT_BLUE << it->customerId << Colors::RESET << std::endl;
        std::cout << "├─ Total Amount: " << Colors::BRIGHT_GREEN << formatCurrency(it->totalAmount) << Colors::RESET << std::endl;
        std::cout << "├─ Status: " << getStatusColor(it->status) << it->status << Colors::RESET << std::endl;
        std::cout << "├─ Order Date: " << it->orderDate << std::endl;
        std::cout << "└─ Items in Order: " << it->productIds.size() << " products\n" << std::endl;
        
        std::cout << Colors::BOLD << Colors::BRIGHT_CYAN << "Items:\n" << Colors::RESET;
        for (int prodId : it->productIds) {
            auto prod = std::find_if(products.begin(), products.end(),
                                    [prodId](const Product& p) { return p.id == prodId; });
            if (prod != products.end()) {
                std::cout << "  • " << prod->name << " - " << formatCurrency(prod->price) << std::endl;
            }
        }
    } else {
        std::cout << Colors::RED << "❌ Order not found!" << Colors::RESET << std::endl;
    }
}

void SellerDashboard::displayRevenue() {
    clearScreen();
    displayHeader();
    
    std::cout << Colors::BOLD << Colors::BRIGHT_CYAN << "\n💰 REVENUE ANALYTICS\n" << Colors::RESET;
    drawBorder(90);
    
    std::cout << "║ " << Colors::BOLD << std::setw(15) << "Date"
              << " │ " << std::setw(15) << "Revenue"
              << " │ " << std::setw(15) << "Orders"
              << " │ " << Colors::RESET << std::endl;
    
    drawBorder(90);
    
    double maxRevenue = 0;
    for (const auto& rev : revenueHistory) {
        if (rev.amount > maxRevenue) maxRevenue = rev.amount;
    }
    
    for (const auto& rev : revenueHistory) {
        double percentage = (rev.amount / maxRevenue) * 100;
        std::cout << "║ " << std::setw(15) << rev.date
                  << " │ " << Colors::BRIGHT_GREEN << std::setw(15) << formatCurrency(rev.amount) << Colors::RESET
                  << " │ " << Colors::BRIGHT_YELLOW << std::setw(15) << rev.orderCount << Colors::RESET
                  << " │" << std::endl;
        
        // Draw bar chart
        std::cout << "║ " << std::setw(15) << ""
                  << " │ " << Colors::BRIGHT_MAGENTA;
        for (int i = 0; i < static_cast<int>(percentage / 2); ++i) std::cout << "█";
        std::cout << Colors::RESET << std::setw(20 - static_cast<int>(percentage / 2)) << ""
                  << " │ " << std::setw(15) << ""
                  << " │" << std::endl;
    }
    
    drawBorder(90);
    
    std::cout << "\n" << Colors::BRIGHT_CYAN << "Total Revenue: " << Colors::BRIGHT_GREEN << formatCurrency(totalRevenue) << Colors::RESET << std::endl;
    std::cout << Colors::BRIGHT_CYAN << "Average Daily Revenue: " << Colors::BRIGHT_GREEN << formatCurrency(totalRevenue / revenueHistory.size()) << Colors::RESET << std::endl;
    
    pauseForUser();
}

void SellerDashboard::addProduct() {
    clearScreen();
    displayHeader();
    
    std::cout << Colors::BOLD << Colors::BRIGHT_CYAN << "\n➕ ADD NEW PRODUCT\n" << Colors::RESET;
    
    std::cout << Colors::BRIGHT_YELLOW << "Enter product name: " << Colors::RESET;
    std::string name;
    std::cin.ignore();
    std::getline(std::cin, name);
    
    std::cout << Colors::BRIGHT_YELLOW << "Enter price: " << Colors::RESET;
    double price;
    std::cin >> price;
    
    std::cout << Colors::BRIGHT_YELLOW << "Enter quantity: " << Colors::RESET;
    int quantity;
    std::cin >> quantity;
    
    std::cout << Colors::BRIGHT_YELLOW << "Enter category: " << Colors::RESET;
    std::string category;
    std::cin.ignore();
    std::getline(std::cin, category);
    
    std::cout << Colors::BRIGHT_YELLOW << "Enter description: " << Colors::RESET;
    std::string description;
    std::getline(std::cin, description);
    
    int newId = products.back().id + 1;
    products.push_back(Product(newId, name, price, quantity, category, description));
    
    std::cout << "\n" << Colors::BRIGHT_GREEN << "✓ Product added successfully! Product ID: " << newId << Colors::RESET << std::endl;
    pauseForUser();
}

void SellerDashboard::updateProductStock() {
    clearScreen();
    displayHeader();
    
    displayProducts();
    
    std::cout << Colors::BRIGHT_YELLOW << "Enter Product ID to update stock: " << Colors::RESET;
    int productId;
    std::cin >> productId;
    
    auto it = std::find_if(products.begin(), products.end(),
                          [productId](const Product& p) { return p.id == productId; });
    
    if (it != products.end()) {
        std::cout << Colors::BRIGHT_CYAN << "Current stock: " << Colors::BRIGHT_BLUE << it->quantity << Colors::RESET << std::endl;
        std::cout << Colors::BRIGHT_YELLOW << "Enter new quantity: " << Colors::RESET;
        int newQuantity;
        std::cin >> newQuantity;
        
        it->quantity = newQuantity;
        std::cout << Colors::BRIGHT_GREEN << "✓ Stock updated successfully!" << Colors::RESET << std::endl;
    } else {
        std::cout << Colors::RED << "❌ Product not found!" << Colors::RESET << std::endl;
    }
    
    pauseForUser();
}

void SellerDashboard::processOrder() {
    clearScreen();
    displayHeader();
    
    displayOrders();
    
    std::cout << Colors::BRIGHT_YELLOW << "Enter Order ID to process: " << Colors::RESET;
    int orderId;
    std::cin >> orderId;
    
    auto it = std::find_if(orders.begin(), orders.end(),
                          [orderId](const Order& o) { return o.orderId == orderId; });
    
    if (it != orders.end()) {
        std::cout << Colors::BRIGHT_CYAN << "Current status: " << getStatusColor(it->status) << it->status << Colors::RESET << std::endl;
        std::cout << Colors::BRIGHT_YELLOW << "\nSelect new status:\n" << Colors::RESET;
        std::cout << "1. Pending\n2. Processing\n3. Shipped\n4. Delivered\n";
        std::cout << Colors::BRIGHT_YELLOW << "Enter choice: " << Colors::RESET;
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1: it->status = "Pending"; break;
            case 2: it->status = "Processing"; break;
            case 3: it->status = "Shipped"; break;
            case 4: it->status = "Delivered"; break;
            default: std::cout << Colors::RED << "Invalid choice!" << Colors::RESET; return;
        }
        
        std::cout << Colors::BRIGHT_GREEN << "✓ Order status updated successfully!" << Colors::RESET << std::endl;
    } else {
        std::cout << Colors::RED << "❌ Order not found!" << Colors::RESET << std::endl;
    }
    
    pauseForUser();
}

void SellerDashboard::displayProfile() {
    clearScreen();
    displayHeader();
    
    std::cout << Colors::BOLD << Colors::BRIGHT_CYAN << "\n👤 SELLER PROFILE\n" << Colors::RESET;
    std::cout << "├─ Seller ID: " << Colors::BRIGHT_BLUE << sellerId << Colors::RESET << std::endl;
    std::cout << "├─ Name: " << Colors::BRIGHT_BLUE << sellerName << Colors::RESET << std::endl;
    std::cout << "├─ Total Revenue: " << Colors::BRIGHT_GREEN << formatCurrency(totalRevenue) << Colors::RESET << std::endl;
    std::cout << "├─ Average Rating: " << Colors::BRIGHT_YELLOW << std::fixed << std::setprecision(2) << totalRating << " ★" << Colors::RESET << std::endl;
    std::cout << "├─ Products Listed: " << Colors::BRIGHT_BLUE << products.size() << Colors::RESET << std::endl;
    std::cout << "├─ Total Orders: " << Colors::BRIGHT_BLUE << orders.size() << Colors::RESET << std::endl;
    std::cout << "└─ Account Status: " << Colors::BRIGHT_GREEN << "ACTIVE ✓" << Colors::RESET << std::endl;
    
    pauseForUser();
}

void SellerDashboard::displayInventoryWarnings() {
    clearScreen();
    displayHeader();
    
    std::cout << Colors::BOLD << Colors::BRIGHT_YELLOW << "\n⚠️ INVENTORY WARNINGS\n" << Colors::RESET;
    
    std::vector<const Product*> lowStockProducts;
    for (const auto& product : products) {
        if (product.quantity < 50) {
            lowStockProducts.push_back(&product);
        }
    }
    
    if (lowStockProducts.empty()) {
        std::cout << Colors::BRIGHT_GREEN << "✓ All products have healthy stock levels!\n" << Colors::RESET;
    } else {
        drawBorder(90);
        std::cout << "║ " << Colors::BOLD << std::setw(20) << "Product Name"
                  << " │ " << std::setw(12) << "Current Stock"
                  << " │ " << std::setw(12) << "Status" << Colors::RESET
                  << " │" << std::endl;
        drawBorder(90);
        
        for (const auto* product : lowStockProducts) {
            std::string status;
            std::string statusColor;
            
            if (product->quantity < 10) {
                status = "CRITICAL";
                statusColor = Colors::RED;
            } else if (product->quantity < 30) {
                status = "LOW";
                statusColor = Colors::BRIGHT_YELLOW;
            } else {
                status = "WARNING";
                statusColor = Colors::BRIGHT_MAGENTA;
            }
            
            std::cout << "║ " << std::setw(20) << product->name.substr(0, 20)
                      << " │ " << Colors::BRIGHT_BLUE << std::setw(12) << product->quantity << Colors::RESET
                      << " │ " << statusColor << std::setw(12) << status << Colors::RESET
                      << " │" << std::endl;
        }
        
        drawBorder(90);
    }
    
    pauseForUser();
}
