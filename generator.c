#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_WORDS 10
#define MAX_WORD_LEN 20
#define MAX_OUTPUT_LEN 80
#define DICTIONARY_SIZE 100000
#define MAX_UNIQUE_ANAGRAMS 100000

char dictionary[DICTIONARY_SIZE][MAX_WORD_LEN];
int dictionary_word_count = 0;
char unique_anagrams[MAX_UNIQUE_ANAGRAMS][MAX_OUTPUT_LEN];
int unique_anagram_count = 0;

void swap(char* a, char* b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

bool is_word_in_dictionary(const char* word) {
    for (int i = 0; i < dictionary_word_count; i++) {
        if (strcmp(dictionary[i], word) == 0) {
            return true;
        }
    }
    return false;
}

bool are_all_words_valid(char result[MAX_WORDS][MAX_WORD_LEN], int n) {
    for (int i = 0; i < n; i++) {
        if (!is_word_in_dictionary(result[i])) {
            return false;
        }
    }
    return true;
}

bool is_anagram_unique(const char* anagram) {
    for (int i = 0; i < unique_anagram_count; i++) {
        if (strcmp(unique_anagrams[i], anagram) == 0) {
            return false;
        }
    }
    return true;
}

void add_unique_anagram(const char* anagram) {
    strcpy(unique_anagrams[unique_anagram_count++], anagram);
}

void generateAnagrams(char* str, int start, int len, int n, char result[MAX_WORDS][MAX_WORD_LEN], int word_lengths[]) {
    if (start == len) {
        int word_start = 0;
        char anagram[MAX_OUTPUT_LEN] = {0};

        // Split the permuted string into words according to original word lengths
        for (int i = 0; i < n; i++) {
            strncpy(result[i], &str[word_start], word_lengths[i]);
            result[i][word_lengths[i]] = '\0';
            word_start += word_lengths[i];
            strcat(anagram, result[i]);
            if (i < n - 1) {
                strcat(anagram, " ");
            }
        }

        // Check if all words are valid English words and if the anagram is unique
        if (are_all_words_valid(result, n) && is_anagram_unique(anagram)) {
            add_unique_anagram(anagram);
            printf("%s\n", anagram);
        }

        return;
    }

    for (int i = start; i < len; i++) {
        swap(&str[start], &str[i]);
        generateAnagrams(str, start + 1, len, n, result, word_lengths);
        swap(&str[start], &str[i]); // backtrack
    }
}

void load_dictionary(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open dictionary file.\n");
        exit(1);
    }

    while (fscanf(file, "%s", dictionary[dictionary_word_count]) != EOF) {
        dictionary_word_count++;
    }

    fclose(file);
}

int main() {
    int n;
    char words[MAX_WORDS][MAX_WORD_LEN];
    char combined[MAX_OUTPUT_LEN] = {0};
    char result[MAX_WORDS][MAX_WORD_LEN];
    int word_lengths[MAX_WORDS];
    int total_length = 0;

    // Load the dictionary
    load_dictionary("dictionary.txt");

    printf("Enter the number of words (up to %d): ", MAX_WORDS);
    scanf("%d", &n);
    if (n <= 0 || n > MAX_WORDS) {
        printf("Invalid number of words.\n");
        return 1;
    }

    printf("Enter %d words:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%s", words[i]);
        strcat(combined, words[i]);
        word_lengths[i] = strlen(words[i]);
        total_length += word_lengths[i];
    }

    printf("Generated anagrams (valid English words only, no duplicates):\n");
    generateAnagrams(combined, 0, total_length, n, result, word_lengths);

    return 0;
}
