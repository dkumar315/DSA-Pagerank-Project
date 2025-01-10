// COMP2521 Assignment - Simple Graph Structure-Based Search Engine
// Part 2 - Inverted Index
//
// Written by:
// Devaansh Kumar (z5474396)
//
// Written on: 11/01/2024
//
// Description:
// This program reads a collection of URLs from "collection.txt". It processes
// the content of each URL and extracts all the words appearing in it. It
// inserts all the words to a binary search tree named indices. Each node in the
// tree has a linked list which stores all the URLs in which that particular
// word has appeared. The resulting inverted index is then printed using
// in-order traversal to the output file, "invertedIndex.txt" in ascending
// (or alphabetical) order.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_PAGES 1000
#define MAX_URL_LENGTH 1000
#define MAX_WORD_LENGTH 1000

struct Pages {
    char filename[MAX_URL_LENGTH];
    struct Pages *next;
};

struct InvertedIndex {
    char word[MAX_WORD_LENGTH];
    struct Pages *fileList;
    struct InvertedIndex *left;
    struct InvertedIndex *right;
};

// Function Prototypes
void readCollectionFile(char *filename, struct InvertedIndex **indices);
void readUrl(char *filename, struct InvertedIndex **indices);
void normalizeWord(char *word);
struct InvertedIndex *insertWord(
    struct InvertedIndex *indices, char *word, char *filename);
void insertFileName(struct Pages **fileList, char *filename);
void freeInvertedIndex(struct InvertedIndex *indices);
void freeFileList(struct Pages *fileList);
void printInvertedIndex(FILE *file, struct InvertedIndex *indices);

int main() {
    // Opens "invertedIndex.txt" in write mode
    // This is the output file for the inverted index
    char *filename = "invertedIndex.txt";
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    struct InvertedIndex *indices = NULL;

    // Opens and reads URLs from the collection file and processes each URL
    readCollectionFile("collection.txt", &indices);

    // Prints the inverted indices to the output file
    // Prints in alphabetical order using in-order traversal
    printInvertedIndex(file, indices);

    // Frees the memory allocated to the indices array
    // This prevents memory leaks
    freeInvertedIndex(indices);

    fclose(file);

    return 0;
}

// Opens and reads the collection file which stores all the URLs
// It then processes all the URLs iteratively
void readCollectionFile(char *filename, struct InvertedIndex **indices) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char url[MAX_URL_LENGTH];
    while (fscanf(file, "%s", url) == 1) {
        // Stores the URL in the newFile array
        char newFile[MAX_URL_LENGTH];
        sprintf(newFile, "%s.txt", url);

        // Processes the URL files to stores all the words
        readUrl(newFile, indices);
    }

    fclose(file);
}


// Reads all the words from a specific URL file
// It normalises the word
// It updates the indices struct by adding the word to the write position
void readUrl(char *filename, struct InvertedIndex **indices) {
    // Opens and the reads the web page
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening '%s'\n", filename);
        exit(EXIT_FAILURE);
    }

    int start = 0;
    char word[MAX_WORD_LENGTH];

    // Firstly, checks where the info of the page starts from
    // After starting to store the words to the indices struct,
    // Checks where the info ends
    while (fscanf(file, "%s", word) == 1) {
        if (strcmp(word, "#end") == 0) {
            start = 0;
        }

        if (start == 1) {
            //Nomralizes the word and then adds it to the BST
            normalizeWord(word);

            if (strlen(word) > 0) {
                *indices = insertWord(*indices, word, filename);
            }
        }

        if (strcmp(word, "Section-2") == 0) {
            start = 1;
        }
    }

    fclose(file);
}

// It normalises the word by
// removing symbols and punctuations from the end and
// converts the word to lowercase.
void normalizeWord(char *word) {
    int len = strlen(word);

    // Checks particularly for these 6 punctuations in the end of the word
    // Removes it if it exists
    // Symbols at start or in the middle of the word are left as it is
    while (len > 0 && (word[len - 1] == '.' || word[len - 1] == ',' || 
                       word[len - 1] == ':' || word[len - 1] == ';' || 
                       word[len - 1] == '?' || word[len - 1] == '*')) {
        len--;
    }

    word[len] = '\0';

    // Converts the word to lower-case
    for (int i = 0; i < len; i++) {
        word[i] = tolower(word[i]);
    }
}

// Inserts the words to the indices struct
// Updates the BST by comparing strings and inserting in the right place
// Also inserts the URLs in which the word appears to the word's linked list
struct InvertedIndex *insertWord(
    struct InvertedIndex *indices, char *word, char *filename
) {
    if (indices == NULL) {
        indices = (struct InvertedIndex *)malloc(sizeof(struct InvertedIndex));
        if (indices == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
        }

        // Inserts the word to the BST
        strcpy(indices->word, word);
        indices->fileList = NULL;
        indices->left = indices->right = NULL;

        // Inserts the URL in which the word appeared
        insertFileName(&(indices->fileList), filename);
    } else {
        // Compares the words
        int cmp = strcmp(word, indices->word);

        // Recursively calls the function
        // Finds the right place for the word to be inserted
        if (cmp == 0) {
            insertFileName(&(indices->fileList), filename);
        } else if (cmp < 0) {
            indices->left = insertWord(indices->left, word, filename);
        } else {
            indices->right = insertWord(indices->right, word, filename);
        }
    }

    return indices;
}

// Inserts the URL's name into the list of filenames for a word
void insertFileName(struct Pages **fileList, char *filename) {
    // Remvoes the ".txt" from the end of the word
    // before entering it into the linked list
    char *extension = strstr(filename, ".txt");
    if (extension != NULL) {
        *extension = '\0';
    }

    // Checks if the particular URL already exists in the list
    struct Pages *current = *fileList;
    while (current != NULL) {
        if (strcmp(current->filename, filename) == 0) {
            return;
        }

        current = current->next;
    }
    
    struct Pages *newNode = (struct Pages *)malloc(sizeof(struct Pages));
    if (newNode == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    strcpy(newNode->filename, filename);
    newNode->next = NULL;

    current = *fileList;
    struct Pages *prev = NULL;

    // Iteratively finds the place to insert the URL in the list
    // URLs are to be stored in ascending order (or alphabetical order)
    while (current != NULL && strcmp(current->filename, filename) < 0) {
        prev = current;
        current = current->next;
    }
    
    if (prev == NULL) {
        newNode->next = *fileList;
        *fileList = newNode;
    } else {
        prev->next = newNode;
        newNode->next = current;
    }
}

// Frees the memory allocated to the Binary Search Tree
// and the linked list stored at every node
void freeInvertedIndex(struct InvertedIndex *indices) {
    if (indices != NULL) {
        // Traverses the tree recursively
        freeInvertedIndex(indices->left);
        freeInvertedIndex(indices->right);

        freeFileList(indices->fileList);

        free(indices);
    }
}

// Frees the memory allocated for the list of filenames
void freeFileList(struct Pages *fileList) {
    while (fileList != NULL) {
        struct Pages *next = fileList->next;
        free(fileList);
        fileList = next;
    }
}

// Prints the inverted index to the output file, "invertedIndex.txt"
// Uses in-order traversal to print all the words in alphabetical order
void printInvertedIndex(FILE *file, struct InvertedIndex *indices) {
    if (indices != NULL) {
        // Recursively traverses and prints the left subtree
        printInvertedIndex(file, indices->left);
        fprintf(file, "%s", indices->word);

        // Iteratively prints all the URLs associated to the current word
        struct Pages *current = indices->fileList;
        while (current != NULL) {
            fprintf(file, " %s", current->filename);
            current = current->next;
        }

        fprintf(file, "\n");

        // Recursively traverses and prints the right subtree
        printInvertedIndex(file, indices->right);
    }
}