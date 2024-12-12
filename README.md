# Indexing Library

## Overview

This is a simple C library that provides functionality for indexing terms (like words) and associating them with document IDs. It includes basic operations for adding terms to the index, searching for terms, and resizing the underlying hash table.

## Features

- **Hash Table Implementation**: Efficient storage and lookup for terms.
- **Search Operations**: Supports searching for terms in the index.
- **Dynamic Resizing**: Resizes the hash table automatically when it becomes too full.

## Functions

### `HashTable* init_index(size_t table_size)`
Initializes a new hash table with the specified size.

- **Parameters**: 
  - `table_size`: The initial size of the hash table.
- **Returns**: 
  - A pointer to a new `HashTable` struct, or `NULL` if memory allocation fails.

### `void add_to_index(HashTable* index, const char* term, int doc_id)`
Adds a term to the hash table, associating it with a document ID.

- **Parameters**:
  - `index`: A pointer to the hash table.
  - `term`: The term to be added to the index.
  - `doc_id`: The document ID to associate with the term.

### `int search_index(HashTable* index, const char* term, int* results, int max_results)`
Searches the index for a specific term and returns the associated document IDs.

- **Parameters**:
  - `index`: A pointer to the hash table.
  - `term`: The term to search for.
  - `results`: An array to store the document IDs.
  - `max_results`: The maximum number of results to store.
- **Returns**: 
  - The number of results found.

### `void resize_table(HashTable** index, size_t new_size)`
Resizes the hash table to a new size. This operation may be needed when the table becomes too full.

- **Parameters**:
  - `index`: A pointer to the hash table.
  - `new_size`: The new size for the hash table.

## Building the Library

To compile the library and tests, run the following commands:

1. **To build the static library:**
   ```bash
   make
