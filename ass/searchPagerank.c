// COMP2521 Assignment - Simple Graph Structure-Based Search Engine
// Part 3 - Simple Search Engine
//
// Written by:
// Devaansh Kumar (z5474396)
//
// Written on: 09/01/2024
//
// Description:
// This program implements a simple search engine. It reads pagerank information
// and inverted index data from the specified files. It matches the words with
// the search terms or keywords being searched. It filters the URLs in which the
// words appear. It keeps track of the count and PageRank value of the
// particular URLs. It sorts the URLs on the basis of count (or PageRank, if the
// count is equal) in descending order. It only displays the top thirty results.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MIN_ARGUMENTS 2
#define MAX_URL_LENGTH 1000
#define MAX_PAGES 1000
#define MAX_SEARCH_TERMS 1000
#define MAX_WORD_LENGTH 1000
#define MAX_RESULTS 30

struct Pagerank {
    char url[MAX_URL_LENGTH];
    int outdegree;
    double pagerank;
};

struct SearchIndex {
    char url[MAX_URL_LENGTH];
    int count;
    double pagerank;
};

// Function Prototypes
void readPagerankList(char *filename, struct Pagerank *pages, int *numPages);
void readInvertedIndex(char *filename, struct SearchIndex *results, 
    int *numResults, struct Pagerank *pages, int numPages, 
    int argc, char **argv);
int urlExists(struct Pagerank *pages, int numPages, char word[MAX_WORD_LENGTH]);
void insertUrl(struct SearchIndex *results, int *numResults,  
    struct Pagerank *pages, int numPages, char word[MAX_WORD_LENGTH]);
void sortResults(struct SearchIndex *results, int numResults);

int main(int argc, char **argv) {
    if (argc < MIN_ARGUMENTS) {
        fprintf(stderr, 
            "Usage: %s <search term 1> <search term 2> ...\n", argv[0]);
        return 1;
    }

    struct Pagerank pages[MAX_PAGES];
    int numPages = 0;

    // Opens and scans the txt file to store the PageRank values
    // Also keeps track of the number of URLs
    readPagerankList("pagerankList.txt", pages, &numPages);

    struct SearchIndex results[MAX_SEARCH_TERMS];
    int numResults = 0;

    // Opens and reads the invertedIndex file
    // Stores the URLs and the number of URLs
    readInvertedIndex("invertedIndex.txt", results, &numResults,
                    pages, numPages, argc, argv);

    // Sorts the rsulting URLs on the basis of count
    // or PageRank value if count is equal
    sortResults(results, numResults);

    int limit = 0;
    if (numResults >= MAX_RESULTS) {
        limit = MAX_RESULTS;
    } else {
        limit = numResults;
    }

    // Displays the top 30 results
    for (int i = 0; i < limit; i++) {
        printf("%s\n", results[i].url);
    }

    return 0;
}

// Opens and reads the pagerankList file
// Stores the URLs, their outdegrees and their PageRank values
// Also keeps track of the number of URLs
void readPagerankList(char *filename, struct Pagerank *pages, int *numPages) {
    // Opens the "pagerankList.txt" file in read mode
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening %s", filename);
        exit(EXIT_FAILURE);
    }

    // Scans the txt file to store all the pagerank values regarding the URLs
    *numPages = 0;
    while (fscanf(file, "%999[^,], %d, %lf\n", pages[*numPages].url, 
        &pages[*numPages].outdegree, &pages[*numPages].pagerank) == 3) {
        (*numPages)++;
    }

    fclose(file);
}

// Scans through the whole txt file
// Stores all the URLs of the respective words which match the search term
void readInvertedIndex(
    char *filename, struct SearchIndex *results, int *numResults, 
    struct Pagerank *pages, int numPages, int argc, char **argv
) {
    // Opens the "invertedIndex.txt" file in read mode
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening %s", filename);
        exit(EXIT_FAILURE);
    }

    int found = 0;
    char word[MAX_WORD_LENGTH];

    // Keeps track of the number of results to be printed as output
    // Iteratively scans all the words and their respective URLs
    // If the word matches the search term, it stores the URL it appears in
    *numResults = 0;
    while (fscanf(file, "%s", word) == 1) {
        // Checks if the word is a valid URL name and is stored or not
        if (!urlExists(pages, numPages, word)) {
            found = 0;
        }

        if (found == 1) {
            // Inserts the URL to results array
            insertUrl(results, numResults, pages, numPages, word);
        }
        
        for (int i = 0; i < argc - 1 && (!strstr(word, "url")); i++) {
            if (strcmp(word, argv[i + 1]) == 0) {
                found = 1;
            }
        }
    }
    
    fclose(file);
}

// Checks if the URL is valid and exists in the stored pages array
int urlExists(
    struct Pagerank *pages, int numPages, char word[MAX_WORD_LENGTH]
) {
    for (int i = 0; i < numPages; i++) {
        if (strcmp(word, pages[i].url) == 0) {
            return 1;
        }
    }
    return 0;
}

// Stores the URL in the results array
// Keep count of the times the URL has appeared
// Also stores the particular URL's PageRank value
void insertUrl(
    struct SearchIndex *results, int *numResults, 
    struct Pagerank *pages, int numPages, char word[MAX_WORD_LENGTH]
) {
    int j = 0;
    // Checks if the URL already exists in the results
    for (j = 0; j <= (*numResults); j++) {
        if (strcmp(word, results[j].url) == 0) {
            break;
        }
    }

    // Increments the count of the URL if it already exists
    if (j <= (*numResults)) {
        results[j].count++;
    } 
    // Stores the URL in the results if it doesn't exist
    // Sets it's count to 1 and stores the PageRank Value for the pages array
    else {
        strcpy(results[(*numResults)].url, word);
        results[(*numResults)].count = 1;

        for (int k = 0; k < numPages; k++) {
            if (strcmp(results[(*numResults)].url, pages[k].url) == 0) {
                results[(*numResults)].pagerank = pages[k].pagerank;
            }
        }

        (*numResults)++;
    }
}

// Sorts the resulting URLs using bubble sort
// Sorts in descending order on the basis of their count
// If the number of times the URLs appear is same,
// they are sorted on the basis of their PageRank values
void sortResults(struct SearchIndex *results, int numResults) {
    for (int i = 0; i < numResults - 1; i++) {
        for (int j = 0; j < numResults - i - 1; j++) {
            if (results[j].count < results[j + 1].count || 
                (results[j].count == results[j + 1].count && 
                results[j].pagerank < results[j + 1].pagerank)) {
                struct SearchIndex temp = results[j];
                results[j] = results[j + 1];
                results[j + 1] = temp;
            }
        }
    }
}