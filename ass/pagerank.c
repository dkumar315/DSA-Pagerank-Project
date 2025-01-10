// COMP2521 Assignment - Simple Graph Structure-Based Search Engine
// Part 1 - PageRank Calculation
//
// Written by:
// Devaansh Kumar (z5474396)
//
// Written on: 10/01/2024
//
// Description:
// This program calculates the PageRank values for a collection of web pages
// stored in a collection file. It calculates the outdegrees for each page by
// accessing all the URLs. It then calculates the PageRank values of each page
// using provided parameters such as damping factor, sum of PageRank differences
// and maximum iterations. The PageRank list is then sorted in the descending
// order on the basis of the PageRank values. It is then written to a file named
// "pagerankList.txt" as the output.

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define NO_OF_ARGUMENTS 4
#define MAX_URL_LENGTH 1000
#define MAX_PAGES 1000
#define MAX_PAGE_INFO 1000

struct Page {
    char url[MAX_URL_LENGTH];
    int outdegree;
    double pagerank;
    int isScanned;
};

// Function Prototypes
void readCollectionFile(char *filename, struct Page *pages, int *numPages);
void initializeAdjMatrix(int numPages, int adjMatrix[numPages][numPages]);
void calculateOutdegree(struct Page *pages, int numPages, 
    int adjMatrix[numPages][numPages]);
void updateAdjMatrix(char filename[MAX_URL_LENGTH + 4], struct Page *pages, 
    int numPages, int adjMatrix[numPages][numPages], char line[MAX_PAGE_INFO], 
    int i, int *outdegree);
void calculatePageRank(struct Page *pages, int numPages, 
    int adjMatrix[numPages][numPages], 
    double d, double diffPR, int maxIterations);
void updatePagerank(struct Page *pages, int numPages, 
    int adjMatrix[numPages][numPages], double d, 
    double *pageranks, double *newPageranks);
void sortPages(struct Page *pages, int numPages);
void writePageRankList(struct Page *pages, int numPages);

int main(int argc, char **argv) {
    if (argc != NO_OF_ARGUMENTS) {
        fprintf(stderr, 
            "Usage: %s <damping_factor> <diffPR> <maxIterations>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct Page pages[MAX_PAGES];
    int numPages;

    // Reads the collection file
    // Stores the URLs and the number of URLs
    readCollectionFile("collection.txt", pages, &numPages);

    int adjMatrix[MAX_PAGES][MAX_PAGES];
    // Initializes the adjacency matrix
    // Assigns zero to each and every value
    initializeAdjMatrix(numPages, adjMatrix);

    // Calculates the outdegree of each page and updates the adjacency matrix
    calculateOutdegree(pages, numPages, adjMatrix);

    // Calculates the PageRank
    // Uses the damping factor, sum of PageRank differences and 
    // maximum number of iterations
    calculatePageRank(pages, numPages, adjMatrix, 
        atof(argv[1]), atof(argv[2]), atoi(argv[3]));

    // Write the sorted PageRank list to a pagerankList.txt
    writePageRankList(pages, numPages);

    return 0;
}

// Read the collection file
// Stores the URLs and the number of URLs
void readCollectionFile(char *filename, struct Page *pages, int *numPages) {
    // Opens the file collection.txt which stores all the URLs
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening %s", filename);
        exit(EXIT_FAILURE);
    }

    // Number of pages are stored using pointers, 
    // so that it can be used anywhere in the program
    *numPages = 0;
    while (fscanf(file, "%s", pages[*numPages].url) == 1) {
        (*numPages)++;
    }

    fclose(file);
}

// Initializes the adjacency matrix
void initializeAdjMatrix(int numPages, int adjMatrix[numPages][numPages]) {
    for (int i = 0; i < numPages; i++) {
        for (int j = 0; j < numPages; j++) {
            // Assigns zero to each and every value
            adjMatrix[i][j] = 0;
        }
    }
}

// Calculates the outdegree of each page and updates the adjacency matrix
void calculateOutdegree(
    struct Page *pages, int numPages, int adjMatrix[numPages][numPages]
) {
    for (int i = 0; i < numPages; i++) {
        int outdegree = 0;
        char filename[MAX_URL_LENGTH + 4];

        // Stores the URL name stored in the pages array in variable filename
        snprintf(filename, sizeof(filename), "%s.txt", pages[i].url);

        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            fprintf(stderr, "Error opening %s\n", filename);
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < numPages; i++) {
            pages[i].isScanned = 0;
        }

        // Reads all the lines in the current page to calculate the outdegree
        char line[MAX_PAGE_INFO];
        while (fscanf(file, "%s", line) == 1) {
            updateAdjMatrix(filename, pages, numPages, 
                adjMatrix, line, i, &outdegree);
        }

        // Stores the outdegree for the current page in the pages array
        pages[i].outdegree = outdegree;

        fclose(file);
    }
}

// Helper function to update the adjacency matrix
void updateAdjMatrix(
    char filename[MAX_URL_LENGTH + 4], struct Page *pages, int numPages, 
    int adjMatrix[numPages][numPages], char line[MAX_PAGE_INFO], 
    int i, int *outdegree
) {
    char *extension = strstr(filename, ".txt");
    if (extension != NULL) {
        *extension = '\0';
    }

    for (int j = 0; j < numPages; j++) {
        if (strcmp(line, pages[j].url) == 0 && 
            strcmp(line, filename) != 0 && 
            pages[j].isScanned == 0) {
            pages[j].isScanned = 1;
            // Increments the outdegree for the current page
            // Uses pointer to access outdegree afterwards 
            // to calculate the PageRank
            adjMatrix[j][i] = 1;
            (*outdegree)++;
        }
    }
}

// Calculates the PageRank of each page
void calculatePageRank(
    struct Page *pages, int numPages, int adjMatrix[numPages][numPages], 
    double d, double diffPR, int maxIterations
) {
    double pageranks[numPages];
    double newPageranks[numPages];

    // Initialize PageRank values
    for (int i = 0; i < numPages; i++) {
        pageranks[i] = 1.0 / numPages;
    }

    int iteration = 0;
    double diff = diffPR;

    // Iteratively updates the PageRank values
    while (iteration < maxIterations && diff >= diffPR) {
        updatePagerank(pages, numPages, adjMatrix, d, pageranks, newPageranks);

        // Calculate the difference between new and old PageRank values
        diff = 0;
        for (int i = 0; i < numPages; i++) {
            diff += fabs(newPageranks[i] - pageranks[i]);
        }

        // Stores the PageRank value to the pages array 
        // Updates the PageRank value for the next iteration
        for (int i = 0; i < numPages; i++) {
            pageranks[i] = newPageranks[i];
            pages[i].pagerank = newPageranks[i];
        }

        iteration++;
    }
}

// Helper function to update the PageRank values in each iteration
void updatePagerank(
    struct Page *pages, int numPages, int adjMatrix[numPages][numPages], 
    double d, double *pageranks, double *newPageranks
) {
    for (int i = 0; i < numPages; i++) {
        newPageranks[i] = (1 - d) / numPages;

        for (int j = 0; j < numPages; j++) {
            if (adjMatrix[i][j] == 1) {
                newPageranks[i] += d * pageranks[j] / pages[j].outdegree;
            }
        }
    }
}

// Writes the sorted PageRank list to a file
void writePageRankList(struct Page *pages, int numPages) {
    // Opens the pagerankList.txt file in write mode
    FILE *file = fopen("pagerankList.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening pagerankList.txt");
        exit(EXIT_FAILURE);
    }

    // Sorts the PageRank list on the basis of PageRank
    sortPages(pages, numPages);

    // Writes the list to the txt file as output
    for (int i = 0; i < numPages; i++) {
        fprintf(file, "%s, %d, %.7lf\n", 
            pages[i].url, pages[i].outdegree, pages[i].pagerank);
    }

    fclose(file);
}

// Sorts the PageRank list using bubble sort
// Sorts in the descending order on the basis of PageRank value
void sortPages(struct Page *pages, int numPages) {
    for (int i = 0; i < numPages - 1; i++) {
        for (int j = 0; j < numPages - i - 1; j++) {
            if (pages[j].pagerank < pages[j + 1].pagerank) {
                struct Page temp = pages[j];
                pages[j] = pages[j + 1];
                pages[j + 1] = temp;
            }
        }
    }
}