//Design and implement a lexical analyzer using C language to recognize all valid tokens in the input program. The lexical analyzer should ignore redundant spaces, tabs and newlines. It should also ignore comments. 
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main() {
    FILE *input, *output;   // File pointers for input and output files
    int l = 1;              // Line number counter
    int t = 0;              // Token number counter
    int j = 0;              // Index variable for looping
    char ch, str[20];       // Variable to hold the current character and a buffer for strings

    // Open the input file in read mode and the output file in write mode
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");

    // Check if input file was opened successfully
    if (input == NULL) {
        fprintf(stderr, "Error opening input file.\n");
        return 1;
    }
    // Check if output file was opened successfully
    if (output == NULL) {
        fprintf(stderr, "Error opening output file.\n");
        fclose(input);
        return 1;
    }

    // Define an array of keywords to check against
    char keyword[30][30] = {"int", "main", "if", "else", "do", "while"};
    // Write the header for the output file
    fprintf(output, "Line no. \t Token no. \t\t Token \t\t Lexeme\n\n");

    // Read characters from the input file one by one
    while ((ch = fgetc(input)) != EOF) { // Change from while (!feof(input)) to while ((ch = fgetc(input)) != EOF)
        
        // Handle preprocessor directives
        if (ch == '#') {
            // Skip the rest of the line
            while ((ch = fgetc(input)) != '\n' && ch != EOF);
            l++; // Increment line number after skipping the directive line
            continue; // Skip to the next character
        }

        // Skip whitespace characters and count new lines
        if (isspace(ch)) {
            if (ch == '\n') {
                l++; // Increment line number for each new line
            }
            continue; // Skip to the next character
        }

        // Handle comments starting with //
        if (ch == '/') {
            char next = fgetc(input); // Look at the next character
            if (next == '/') { // Check for single-line comment
                // Skip the rest of the line
                while ((ch = fgetc(input)) != '\n' && ch != EOF);
                l++; // Increment line number after the comment line
                continue; // Skip to the next character
            } else {
                ungetc(next, input); // Put back the character if it's not a comment
            }
        }

        // Handle comments starting with /* and ending with */
        if (ch == '/') {
            char next = fgetc(input); // Look at the next character
            if (next == '*') { // Check for multi-line comment
                while (1) {
                    ch = fgetc(input); // Read characters within the comment
                    if (ch == '\n') {
                        l++; // Increment line number for each new line within the comment
                    }
                    if (ch == '*') {
                        if ((ch = fgetc(input)) == '/') { // Check for end of multi-line comment
                            break;
                        } else {
                            ungetc(ch, input); // Put back the character if it's not the end of the comment
                        }
                    }
                    if (ch == EOF) { // End of file encountered
                        break;
                    }
                }
                continue; // Skip to the next character
            } else {
                ungetc(next, input); // Put back the character if it's not a comment
            }
        }

        // Handle operators
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            fprintf(output, "%7d\t\t %7d\t\t Operator\t %7c\n", l, t++, ch);
        }
        // Handle special symbols
        else if (ch == ';' || ch == '{' || ch == '}' || ch == '(' || ch == ')' || ch == '?' || ch == '@' || ch == '!' || ch == '%') {
            fprintf(output, "%7d\t\t %7d\t\t Special symbol\t %7c\n", l, t++, ch);
        }
        // Handle digits
        else if (isdigit(ch)) {
            fprintf(output, "%7d\t\t %7d\t\t Digit\t\t %7c\n", l, t++, ch);
        }
        // Handle identifiers and keywords
        else if (isalpha(ch)) {
            int i = 0;
            str[i++] = ch; // Start building the string with the current character
            while (isalnum((ch = fgetc(input))) || ch == '_') {
                if (i < sizeof(str) - 1) { // Ensure buffer does not overflow
                    str[i++] = ch;
                }
            }
            str[i] = '\0'; // Null-terminate the string
            ungetc(ch, input); // Put back the character if it's not part of the identifier

            // Check if the string is a keyword
            int flag = 0;
            for (j = 0; j < 6; j++) {
                if (strcmp(str, keyword[j]) == 0) {
                    flag = 1; // Set flag if it's a keyword
                    break;
                }
            }
            // Write the result to the output file
            if (flag == 1) {
                fprintf(output, "%7d\t\t %7d\t\t Keyword\t %7s\n", l, t++, str);
            } else {
                fprintf(output, "%7d\t\t %7d\t\t Identifier\t %7s\n", l, t++, str);
            }
        }
    }

    // Close the files
    fclose(input);
    fclose(output);
    return 0;
}

          
   
