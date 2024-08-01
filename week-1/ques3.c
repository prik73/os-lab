#include <stdio.h>
#include <string.h>

int main() {
    char str[100], temp;
    int i, j;

    // Prompt user to enter a string
    printf("Enter a string: ");
    fgets(str, sizeof(str), stdin);

    // Remove the newline character at the end, if any
    str[strcspn(str, "\n")] = '\0';

    // Get the length of the string
    int len = strlen(str);

    // Reverse the string
    for (i = 0, j = len - 1; i < j; i++, j--) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }

    // Print the reversed string
    printf("Reversed string: %s\n", str);

    return 0;
}
