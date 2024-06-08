![Logo](/CSVantage/images/CSVantage_header_logo.png)

## 📋 Table of Contents

1. ✨ What is CSVantage?
2. 🔎 What can CSVantage do?
3. 🚀 Installation Process
4. 💻 Technology Used
5. 📷 Screenshots

## ✨ What is CSVantage?
CSVantage is a **Linux CLI application** built in **C** that performs **CRUD** operations on CSV files. 
This is an excellent tool for simple data manipulation and management.

## 🔎 What can CSVantage do?
CSVantage allows users to interact with CSV files through a user-friendly command line interface without the need for SQL commands. Here's what you can accomplish with CSVantage:

- **Create New CSV Files**: Start from scratch and create new CSV files, adding fields as you go.
- **Load Existing CSV Files**: Open any CSV file in the same directory as the application to perform operations.
- **CRUD Operations**: 
  - **Create**: Add new rows and columns to your CSV files.
  - **Read**: You can read and display specific lines, columns, the entire table, or check if a particular value exists in the table.
  - **Update**: Utilize a simple find and replace feature, or a more targeted update where a specific value in a row can trigger an update to another value in the same row (e.g., updating the 'salary' field when the employee's name is 'Mark').
  - **Delete**: Remove specific rows, entire columns, or the entire CSV file if no longer needed.
  
These operations straightforward and require no prior database knowledge.

## 🚀 Installation Process
Installation is easy and straightforward but CSVantage **ONLY** works on linux machines.

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/TimothyPav/c_database.git
   ```

2. **Navigate into the src Directory:**
    ```bash
   cd CSVantage/src
   ```

3. **Run 'Make' to compile and run the application:**
    ```bash
    make
    ```

## 💻 Technology Used

CSVantage is built using the following technologies:

- **C Programming Language**
- **GNU Compiler Collection (GCC)**
- **Makefile**

## 📷 Screenshots

Below are a few screenshots demonstrating the usage of CSVantage for various operations:

### Main Menu
![Main Menu](/CSVantage/images/main_menu)