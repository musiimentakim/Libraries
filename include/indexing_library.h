#ifndef INDEXING_LIBRARY_H
#define INDEXING_LIBRARY_H

#include <stdlib.h>
#include <string.h>

// Define constants for error codes
#define SUCCESS 0
#define ERR_MEMORY_ALLOCATION 1
#define ERR_TERM_NOT_FOUND 2
#define ERR_INVALID_ARGUMENT 3

// Structure representing an individual index node
typedef struct IndexNode {
    char *term;              // The indexed term
    int *doc_ids;            // List of associated document IDs
    int doc_count;           // Number of document IDs
    int capacity;            // Capacity of the doc_ids array
    struct IndexNode *next;  // Pointer to the next node in case of hash collisions
} IndexNode;

// Structure representing the hash table
typedef struct {
    IndexNode **table;  // Array of pointers to index nodes
    size_t size;        // Number of buckets in the hash table
} HashTable;

// Metrics structure to track statistics
typedef struct {
    size_t term_count;  // Number of terms in the index
    size_t doc_count;   // Total number of documents indexed
    double load_factor; // Load factor of the hash table
} IndexMetrics;


// Function declarations
size_t hash_function(const char *str, size_t table_size);
HashTable *init_index(size_t table_size);
int add_to_index(HashTable *index, const char *term, int doc_id);
int add_batch_to_index(HashTable *index, const char **terms, int *doc_ids, size_t count);
int search_index(HashTable *index, const char *term, int *results, int max_results);
int search_index_prefix(HashTable *index, const char *prefix, int *results, int max_results);
int resize_table(HashTable **index, size_t new_size);
IndexMetrics get_metrics(HashTable *index);
void free_index(HashTable *index);
void log_message(const char *message);

#endif // INDEXING_LIBRARY_H
