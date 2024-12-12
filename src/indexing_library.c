#include "indexing_library.h"
#include <stdio.h>

/**
 * name : hash_function
 * args : const char *str, size_t table_size
 * function: Hash function to map strings to hash table indices
 * return_type: size_t
 **/
size_t hash_function(const char *str, size_t table_size) {
    size_t hash = 0;
    while (*str) {
        hash = (hash * 31 + *str++) % table_size; // Prime multiplier ensures better distribution
    }
    return hash;
}

/**
 * name : log_message
 * args : const char *message
 * function: Logs messages to the console for debugging or monitoring
 * return_type: none
 **/
void log_message(const char *message) {
    fprintf(stderr, "LOG: %s\n", message);
}

/**
 * name : init_index
 * args : size_t table_size
 * function: Initializes the hash table
 * return_type: HashTable*
 **/
/**
 * name : init_index
 * args : size_t table_size
 * function: Initializes the hash table
 * return_type: HashTable*
 **/
HashTable *init_index(size_t table_size) {
    if (table_size == 0) {
        log_message("Invalid table size: must be greater than 0.");
        return NULL; // Return NULL for invalid size
    }

    HashTable *index = malloc(sizeof(HashTable));
    if (!index) {
        log_message("Failed to allocate memory for HashTable.");
        return NULL;
    }

    index->size = table_size;
    index->table = calloc(table_size, sizeof(IndexNode *));
    if (!index->table) {
        log_message("Failed to allocate memory for hash table buckets.");
        free(index);
        return NULL;
    }

    log_message("HashTable initialized successfully.");
    return index;
}

/**
 * name : add_to_index
 * args : HashTable *index, const char *term, int doc_id
 * function: Adds a term and document ID to the index
 * return_type: int
 **/
int add_to_index(HashTable *index, const char *term, int doc_id) {
    if (!index || !term) return ERR_INVALID_ARGUMENT;

    size_t hash = hash_function(term, index->size);
    IndexNode *node = index->table[hash];

    // Search for the term in the linked list
    while (node) {
        if (strcmp(node->term, term) == 0) {
            // Term found, add doc_id
            if (node->doc_count == node->capacity) {
                node->capacity *= 2;
                int *new_ids = realloc(node->doc_ids, node->capacity * sizeof(int));
                if (!new_ids) return ERR_MEMORY_ALLOCATION;
                node->doc_ids = new_ids;
            }
            node->doc_ids[node->doc_count++] = doc_id;
            return SUCCESS;
        }
        node = node->next;
    }

    // Term not found, create a new node
    node = malloc(sizeof(IndexNode));
    if (!node) return ERR_MEMORY_ALLOCATION;

    node->term = strdup(term);
    if (!node->term) {
        free(node);
        return ERR_MEMORY_ALLOCATION;
    }

    node->doc_ids = malloc(10 * sizeof(int)); // Initial capacity
    if (!node->doc_ids) {
        free(node->term);
        free(node);
        return ERR_MEMORY_ALLOCATION;
    }

    node->doc_count = 1;
    node->capacity = 10;
    node->doc_ids[0] = doc_id;
    node->next = index->table[hash];
    index->table[hash] = node;

    return SUCCESS;
}

/**
 * name : add_batch_to_index
 * args : HashTable *index, const char **terms, int *doc_ids, size_t count
 * function: Adds multiple terms and document IDs in batch
 * return_type: int
 **/
int add_batch_to_index(HashTable *index, const char **terms, int *doc_ids, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        int result = add_to_index(index, terms[i], doc_ids[i]);
        if (result != SUCCESS) return result;
    }
    return SUCCESS;
}

/**
 * name : search_index
 * args : HashTable *index, const char *term, int *results, int max_results
 * function: Searches for a term in the index
 * return_type: int
 **/
int search_index(HashTable *index, const char *term, int *results, int max_results) {
    if (!index || !term) return ERR_INVALID_ARGUMENT;

    size_t hash = hash_function(term, index->size);
    IndexNode *node = index->table[hash];

    while (node) {
        if (strcmp(node->term, term) == 0) {
            int count = (node->doc_count < max_results) ? node->doc_count : max_results;
            memcpy(results, node->doc_ids, count * sizeof(int));
            return count;
        }
        node = node->next;
    }

    return ERR_TERM_NOT_FOUND;
}

/**
 * name : get_metrics
 * args : HashTable *index
 * function: Retrieves metrics about the hash table
 * return_type: IndexMetrics
 **/
IndexMetrics get_metrics(HashTable *index) {
    size_t term_count = 0, doc_count = 0;
    for (size_t i = 0; i < index->size; ++i) {
        IndexNode *node = index->table[i];
        while (node) {
            term_count++;
            doc_count += node->doc_count;
            node = node->next;
        }
    }
    IndexMetrics metrics = {term_count, doc_count, (double)term_count / index->size};
    return metrics;
}

/**
 * name : resize_table
 * args : HashTable **index, size_t new_size
 * function: Resizes the hash table to accommodate more terms
 * return_type: int
 **/
int resize_table(HashTable **index, size_t new_size) {
    HashTable *new_index = init_index(new_size);
    if (!new_index) return ERR_MEMORY_ALLOCATION;

    for (size_t i = 0; i < (*index)->size; ++i) {
        IndexNode *node = (*index)->table[i];
        while (node) {
            add_to_index(new_index, node->term, node->doc_ids[0]); // Simplified
            node = node->next;
        }
    }

    free_index(*index);
    *index = new_index;
    return SUCCESS;
}

/**
 * name : free_index
 * args : HashTable
 * function: Frees all memory allocated for the hash table
 * return_type: none
 **/
void free_index(HashTable *index) {
    if (index == NULL) {
        return; // Avoid dereferencing a NULL pointer
    }

    for (size_t i = 0; i < index->size; ++i) {
        IndexNode *node = index->table[i];
        while (node) {
            IndexNode *temp = node;
            node = node->next;
            free(temp->term);  // Free dynamically allocated term
            free(temp->doc_ids);  // Free dynamically allocated doc_ids (assuming it's dynamically allocated)
            free(temp);  // Free the node itself
        }
    }

    free(index->table);  // Free the table array
    free(index);  // Free the HashTable struct itself
}

