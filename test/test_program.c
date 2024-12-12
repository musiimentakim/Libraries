#include "indexing_library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void test_hash_function() {
    // Test case for the hash function: Ensuring proper hashing of strings.
    size_t table_size = 10;

    // Test hashing of normal strings
    size_t hash1 = hash_function("term1", table_size);
    size_t hash2 = hash_function("term2", table_size);
    
    // Hashes for different terms should not be the same
    assert(hash1 != hash2);

    // Test hashing of an empty string
    size_t hash_empty = hash_function("", table_size);
    
    // Hashing for empty string should be consistent for each run
    assert(hash_empty == (size_t)hash_function("", table_size));
    
    printf("test_hash_function passed.\n");
}

void test_log_message() {
    // Test case for logging functionality.
    // Since log_message writes to stderr, we will redirect stderr to a file and check the content.

    freopen("log_test.txt", "w", stderr);  // Redirect stderr to a file
    log_message("Test log message");
    fclose(stderr);

    // Open the log file and check the content
    FILE *log_file = fopen("log_test.txt", "r");
    char buffer[256];
    fgets(buffer, sizeof(buffer), log_file);
    
    // Check if the correct log message was written
    assert(strcmp(buffer, "LOG: Test log message\n") == 0);
    fclose(log_file);

    printf("test_log_message passed.\n");
}

void test_init_index() {
    // Test case for the init_index function: Verifying initialization of the hash table.
    
    HashTable *index = init_index(10);
    assert(index != NULL);  // Ensure the index was created successfully
    assert(index->size == 10);  // Verify the table size
    assert(index->table != NULL);  // Verify the table is allocated

    // Test with invalid table size (0)
    HashTable *invalid_index = init_index(0);
    assert(invalid_index == NULL);  // Ensure NULL is returned for invalid size

    free_index(index);  // Clean up the index after the test

    printf("test_init_index passed.\n");
}

void test_add_to_index() {
    // Test case for adding terms to the index.

    HashTable *index = init_index(10);

    // Add a single term "term1" with document ID 1
    assert(add_to_index(index, "term1", 1) == SUCCESS);

    // Add another term "term2" with document ID 2
    assert(add_to_index(index, "term2", 2) == SUCCESS);

    // Add "term1" again, with a different document ID 3
    assert(add_to_index(index, "term1", 3) == SUCCESS);

    // Verify "term1" has two document IDs
    int results[2];
    int count = search_index(index, "term1", results, 2);
    assert(count == 2);
    assert(results[0] == 1);
    assert(results[1] == 3);

    // Test invalid input (NULL term)
    assert(add_to_index(index, NULL, 1) == ERR_INVALID_ARGUMENT);

    free_index(index);  // Clean up after the test

    printf("test_add_to_index passed.\n");
}

void test_search_index() {
    // Test case for searching terms in the index.

    HashTable *index = init_index(10);
    add_to_index(index, "term1", 1);
    add_to_index(index, "term1", 2);
    add_to_index(index, "term2", 3);

    // Search for "term1", should return 2 document IDs
    int results[2];
    int count = search_index(index, "term1", results, 2);
    assert(count == 2);
    assert(results[0] == 1);
    assert(results[1] == 2);

    // Search for a non-existent term "term3", should return error
    count = search_index(index, "term3", results, 2);
    assert(count == ERR_TERM_NOT_FOUND);

    // Test invalid input (NULL term)
    count = search_index(index, NULL, results, 2);
    assert(count == ERR_INVALID_ARGUMENT);

    free_index(index);  // Clean up after the test

    printf("test_search_index passed.\n");
}

void test_resize_table() {
    // Test case for resizing the hash table.
    
    HashTable *index = init_index(5);
    add_to_index(index, "term1", 1);
    add_to_index(index, "term2", 2);

    // Resize the table to a larger size (10)
    assert(resize_table(&index, 10) == SUCCESS);

    // Verify the terms still exist after resizing
    int results[2];
    int count = search_index(index, "term1", results, 2);
    assert(count == 1);
    assert(results[0] == 1);

    // Resize the table to a smaller size (3)
    // Note: This might cause loss of data if not handled properly
    assert(resize_table(&index, 3) == SUCCESS);

    free_index(index);  // Clean up after the test

    printf("test_resize_table passed.\n");
}

void test_free_index() {
    // Test case for freeing the hash table and its memory.
    
    HashTable *index = init_index(5);
    add_to_index(index, "term1", 1);
    
    // Free the memory allocated for the index
    free_index(index);
    
    // Test freeing an empty index
    index = init_index(0);
    free_index(index);

    printf("test_free_index passed.\n");
}

int main() {
    // Run all tests
    
    test_hash_function();
    test_log_message();
    test_init_index();
    test_add_to_index();
    test_search_index();
    test_resize_table();
    test_free_index();

    printf("All tests passed.\n");

    return 0;
}

