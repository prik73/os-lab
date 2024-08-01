#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_PARAGRAPH_LENGTH 1000
#define MAX_WORD_LENGTH 50
#define MAX_WORDS 100

typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void addWord(WordCount wordCounts[], int *numWords, char *word) {
    toLowerCase(word);
    for (int i = 0; i < *numWords; i++) {
        if (strcmp(wordCounts[i].word, word) == 0) {
            wordCounts[i].count++;
            return;
        }
    }
    strcpy(wordCounts[*numWords].word, word);
    wordCounts[*numWords].count = 1;
    (*numWords)++;
}

void countWords(char *paragraph, WordCount wordCounts[], int *numWords) {
    char *token = strtok(paragraph, " ,.-!?;\n");
    while (token != NULL) {
        addWord(wordCounts, numWords, token);
        token = strtok(NULL, " ,.-!?;\n");
    }
}

void printWordCounts(WordCount wordCounts[], int numWords) {
    printf("Word frequencies:\n");
    for (int i = 0; i < numWords; i++) {
        printf("%s: %d\n", wordCounts[i].word, wordCounts[i].count);
    }
}

int main() {
    char paragraph[MAX_PARAGRAPH_LENGTH];
    WordCount wordCounts[MAX_WORDS];
    int numWords = 0;

    printf("Enter a paragraph:\n");
    fgets(paragraph, MAX_PARAGRAPH_LENGTH, stdin);

    countWords(paragraph, wordCounts, &numWords);
    printWordCounts(wordCounts, numWords);

    return 0;
}
