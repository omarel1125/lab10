#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int count;  // To store the count of occurrences of the word
};

// Initializes a new Trie node
struct TrieNode *createTrieNode() {
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (node) {
        node->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            node->children[i] = NULL;
        }
    }
    return node;
}

// Inserts a word into the Trie
void insert(struct TrieNode *root, char *word) {
    struct TrieNode *current = root;
    while (*word) {
        int index = *word - 'a';
        if (!current->children[index]) {
            current->children[index] = createTrieNode();
        }
        current = current->children[index];
        ++word;
    }
    current->count++;
}

// computes the number of occurances of the word
int numberOfOccurrences(struct TrieNode *root, char *word) {
    struct TrieNode *current = root;
    while (*word) {
        int index = *word - 'a';
        if (!current->children[index]) {
            return 0;
        }
        current = current->children[index];
        ++word;
    }
    return current->count;
}

// Deallocates memory used by the Trie
void deallocateTrie(struct TrieNode *root) {
    if (root == NULL) return;
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (root->children[i]) {
            deallocateTrie(root->children[i]);
        }
    }
    free(root);
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    int numWords = 0;
    char word[100]; // Assume maximum word length is 100 characters

    while (fscanf(file, "%s", word) != EOF) {
        pInWords[numWords] = strdup(word);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void) {
    char *inWords[256];

    //read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct TrieNode *root = createTrieNode();
    for (int i = 0; i < numWords; ++i) {
        insert(root, inWords[i]);
    }

   // parse lineby line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    int numPWords = sizeof(pWords) / sizeof(pWords[0]);

    for (int i = 0; i < numPWords; ++i) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(root, pWords[i]));
    }

    deallocateTrie(root);

    return 0;
}
