# B+ Tree-Based Customer Account Management System

## Overview

This project implements a **B+ Tree** for efficient storage and management of customer account records. The B+ Tree allows for quick search, insertion, and deletion of customer accounts based on a primary key (account ID), and supports bulk operations and range queries. The program reads customer account data from a file, allowing for dynamic addition, deletion, and retrieval of records.

## Key Features

- **B+ Tree Implementation**: Efficient data structure for storing customer account records. Supports searching, inserting, deleting, and range querying.
- **Account Management**: The system handles operations for customer account information including IDs, bank details, tax status, and balances.
- **File I/O Support**: Load and save customer accounts to and from text files.
- **Range Queries**: Supports querying k-successive records and writing them to a file.
- **Interactive Menu**: Allows users to perform operations like adding, removing, or searching for accounts.

## Program Flow

The program provides an interactive menu for the following functionalities:

1. **Print the B+ Tree**: Display the current state of the B+ Tree.
2. **Add a New Record**: Insert a new customer account record into the tree.
3. **Delete a Record**: Remove a customer account from the tree.
4. **Search for a Record**: Retrieve details of a specific customer account.
5. **Search for k Records**: Search and retrieve multiple records by primary keys.
6. **Search for k Successive Records**: Retrieve a range of records starting from a given key.
7. **Delete the B+ Tree**: Clear the entire B+ Tree from memory.
8. **Exit**: Terminate the program.

## Data Structure

The B+ Tree is structured with internal nodes and leaf nodes, where:
- **Internal Nodes**: Contain keys to guide the search and links to child nodes.
- **Leaf Nodes**: Contain the actual customer account records.

Each account is stored as a structure with the following attributes:
- `id`: Customer Account ID (primary key)
- `B_id`: Bank ID
- `C_id`: Client ID
- `name`: Account holder’s name
- `TAX_st`: Tax status
- `bal`: Account balance

## Files

- **CustomerAccount22.txt**: Input file containing customer account details in a delimited format.
- **Ana_account.txt**: Output file storing search results of k-record queries.
- **Ana_7.txt**: Output file storing search results of k-successive record queries.


## Instructions to Run

1. Load the customer account data from the provided file or manually input accounts.
2. Select from the menu to perform actions like inserting, searching, or deleting records.
3. Use the option to print the current state of the B+ Tree and validate the structure.

## Use Cases

- **Banking Systems**: Efficient storage and retrieval of customer account information.
- **Database Systems**: Structured organization and fast access to large datasets.
- **Financial Applications**: Querying multiple records or accounts in succession.

## Future Enhancements

- **Multithreading Support**: Enhance performance for concurrent access to the B+ Tree.
- **Advanced Search Filters**: Implement search filters based on additional fields like balance range or tax status.
- **Graphical User Interface (GUI)**: Provide a user-friendly interface to visualize the B+ Tree and manage accounts.


