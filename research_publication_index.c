#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/indexing_library.h" // Include your library header

#define MAX_PUBLICATION_TITLE_LENGTH 200

void print_menu() {
    printf("\nResearch Publication Index Menu:\n");
    printf("1. Add a publication\n");
    printf("2. Search for a publication\n");
    printf("3. Resize the hash table\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    // Initialize the hash table
    size_t initial_size = 50;
    HashTable* index = init_index(initial_size);
    if (index == NULL) {
        fprintf(stderr, "Failed to initialize index.\n");
        return EXIT_FAILURE;
    }

    int choice;
    char publication_title[MAX_PUBLICATION_TITLE_LENGTH];
    int doc_id; // Represents a unique identifier for the publication, like a URL hash or database ID
    int results[10];
    size_t new_size;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        switch (choice) {
            case 1: // Add a publication
                printf("Enter the publication title (max %d characters): ", MAX_PUBLICATION_TITLE_LENGTH - 1);
                getchar(); // Clear newline from input buffer
                fgets(publication_title, MAX_PUBLICATION_TITLE_LENGTH, stdin);
                publication_title[strcspn(publication_title, "\n")] = '\0'; // Remove newline character
                printf("Enter the document ID for this publication: ");
                if (scanf("%d", &doc_id) != 1) {
                    fprintf(stderr, "Invalid document ID.\n");
                    while (getchar() != '\n'); // Clear input buffer
                    continue;
                }
                add_to_index(index, publication_title, doc_id);
                printf("Publication '%s' with document ID %d added successfully.\n", publication_title, doc_id);
                break;

            case 2: // Search for a publication
                printf("Enter the publication title to search: ");
                getchar(); // Clear newline from input buffer
                fgets(publication_title, MAX_PUBLICATION_TITLE_LENGTH, stdin);
                publication_title[strcspn(publication_title, "\n")] = '\0'; // Remove newline character
                int num_results = search_index(index, publication_title, results, 10);
                if (num_results > 0) {
                    printf("Search results for '%s':\n", publication_title);
                    for (int i = 0; i < num_results; i++) {
                        printf("Document ID: %d\n", results[i]);
                    }
                } else {
                    printf("No results found for '%s'.\n", publication_title);
                }
                break;

            case 3: // Resize the hash table
                printf("Enter the new size for the hash table: ");
                if (scanf("%zu", &new_size) != 1 || new_size <= 0) {
                    fprintf(stderr, "Invalid size.\n");
                    while (getchar() != '\n'); // Clear input buffer
                    continue;
                }
                resize_table(&index, new_size);
                printf("Hash table resized to %zu entries.\n", new_size);
                break;

            case 4: // Exit
                printf("Exiting the program. Goodbye!\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    // Free the hash table
    free_index(index);

    return EXIT_SUCCESS;
}
