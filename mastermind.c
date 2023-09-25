/*
This is a Mastermind game. The computer will choose a code that is a sequence of four colors from green, blue, red,
yellow, white, and orange. The colors will be chosen at random with no duplicates. The player then has 4 guesses to figure out the code. 
A guess consists of a sequence of four colors. After each guess, the computer will tell the player how many of the colors are
correct and in the correct place, and how many of the colors are correct and in the wrong
place. For example, if the code is GROW (with letters standing for colors in the obvious way)
and the guess is BOGW, the the player would be told that one color is correct and in the
correct location and two colors are correct and in the wrong location.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// Define global constants
const int CODE_LENGTH = 4;
const int NUM_COLORS = 6;
const int MAX_GUESSES = 4;

// Define enumerated types
enum color {GREEN, BLUE, RED, YELLOW, WHITE, ORANGE};

// Function prototypes
void print_rules(void);
void generate_code(enum color code[]);
bool is_valid_guess(enum color guess[]);
void get_guess(enum color guess[]);
void check_guess(enum color code[], enum color guess[], int* num_correct, int* num_misplaced);
void print_feedback(int num_correct, int num_misplaced);
bool play_game(enum color code[]);
void print_code(enum color code[]);
bool wants_to_play_again(void);

// Function name: print_rules
// This prints the rules for the Mastermind game. There is no input.
void print_rules(void) { 
    printf("Welcome to Mastermind!\n");
    printf("The goal of the game is to guess the secret code consisting of four colors.\n");
    printf("Each color can be used only once in the code.\n");
    printf("After each guess, you will receive feedback consisting of two numbers:\n");
    printf("- The number of correct colors in the correct position.\n");
    printf("- The number of correct colors in the wrong position.\n");
    printf("You have %d guesses to guess the code.\n", MAX_GUESSES);
    printf("The possible colors are:\n");
    printf("G - Green\n");
    printf("B - Blue\n");
    printf("R - Red\n");
    printf("Y - Yellow\n");
    printf("W - White\n");
    printf("O - Orange\n");
    printf("Good luck!\n");
}

// Function name: generate_code
// Generate a random code
// The function generates a code of length 'CODE_LENGTH' by randomly selecting colors from the 'NUM_COLORS' options. 
// The generated code is stored in the input array 'code'.
// Inputs:
// - code: an array of enumerated color values representing the code
void generate_code(enum color code[]) { 
    int used_colors[NUM_COLORS];  // Initialize to all 0
    memset(used_colors, 0, sizeof(used_colors));
    for (int i = 0; i < CODE_LENGTH; i++) {
        int color;
        do {
            color = rand() % NUM_COLORS;
        } while (used_colors[color]); // No duplicates
        used_colors[color] = 1;
        code[i] = color;
    }
}

// Function name: is_valid_guess
// The function takes an array of color codes as input and checks if the guess is valid or not. 
// A guess is considered valid if it contains exactly CODE_LENGTH colors, and each color is a valid code within the range of 0 to NUM_COLORS-1
// Inputs:
// - guess: an array of enumerated color values representing the guess input
bool is_valid_guess(enum color guess[]) { 
    int is_used[NUM_COLORS];  // Initialize to all 0
    memset(is_used, 0, sizeof(is_used));
    for (int i = 0; i < CODE_LENGTH; i++) {
        int color = guess[i];
        if (color < 0 || color >= NUM_COLORS) { // in tandem with latest instruction, even if the user enters OOBG, we note that it is valid.
            return false;
        }
        is_used[color] = 1;
    }
    return true;
}

// Function name: get_guess
// Prompts the user to enter a string of capital letters representing a guess for the code. 
// It then reads in the string using scanf, and converts each character to an enumerated color value using a switch statement. 
// If the character is not one of the valid colors, it prints an error message and returns. If the guess is valid, it stores the enumerated color values in the guess array. 
// Finally, it checks if the guess is valid using the is_valid_guess function and prints an error message if it is not. The function does not return a value.
// Inputs:
// - guess: an array of enumerated color values representing the guess input
void get_guess(enum color guess[]) {  
    char guess_str[CODE_LENGTH + 1];  // +1 for null terminator
    printf("Enter your guess as a string of %d capital letters (e.g. RBOW for red, blue, orange, white):\n", CODE_LENGTH);
    scanf("%s", guess_str);
    for (int i = 0; i < CODE_LENGTH; i++) {
        char c = guess_str[i];
        enum color color;
        switch (c) {
            case 'G':
                color = GREEN;
                break;
            case 'B':
                color = BLUE;
                break;
            case 'R':
                color = RED;
                break;
            case 'Y':
                color = YELLOW;
                break;
            case 'W':
                color = WHITE;
                break;
            case 'O':
                color = ORANGE;
                break;
            default:
                printf("Invalid guess! Please try again.\n");
                return;
        }
        guess[i] = color;
    }
    if (!is_valid_guess(guess)) {
        printf("Invalid guess! Please try again.\n");
    }
}


// Function name: check_guess
// Compares the user's guess to the code and calculates the number of exact matches and misplaced matches.
// An exact match is when the color in the guess is in the same position as the code.
// A misplaced match is when the color in the guess is in a different position than the code.
// Inputs:
// - code: an array of enumerated color values representing the code
// - guess: an array of enumerated color values representing the user's guess
// - num_correct: a pointer to an integer that will be updated to the number of exact matches
// - num_misplaced: a pointer to an integer that will be updated to the number of misplaced matches
void check_guess(enum color code[], enum color guess[], int *num_correct, int *num_misplaced) {
    int code_used[NUM_COLORS];
    memset(code_used, 0, sizeof(code_used));
    int guess_used[NUM_COLORS];
    memset(guess_used, 0, sizeof(guess_used));

    *num_correct = 0;
    *num_misplaced = 0;

    // Check for exact matches
    for (int i = 0; i < CODE_LENGTH; i++) {
        if (guess[i] == code[i]) {
            (*num_correct)++;
            code_used[code[i]] = 1;
            guess_used[guess[i]] = 1;
        }
    }

    // Check for misplaced matches
    for (int i = 0; i < CODE_LENGTH; i++) {
        if (guess[i] != code[i]) {
            int color = guess[i];
            if (!guess_used[color]) {
                for (int j = 0; j < CODE_LENGTH; j++) {
                    if (color == code[j] && !code_used[color]) {
                        (*num_misplaced)++;
                        code_used[color] = 1;
                        guess_used[color] = 1;
                        break;
                    }
                }
            }
        }
    }
}

// Function name: print_feedback
// Prints the number of colors in correct position and in wrong position
// Inputs:
// - num_correct: a pointer to an integer that will be updated to the number of exact matches
// - num_misplaced: a pointer to an integer that will be updated to the number of misplaced matches
void print_feedback(int num_correct, int num_misplaced) {
    printf("Number of correct colors in the correct position: %d\n", num_correct);
    printf("Number of correct colors in the wrong position: %d\n", num_misplaced);
}

// Function name: play_game
// Enter color code and return a boolean, either true if guessed correctly within the number of MAX_GUESSES, no otherwise
// Inputs:
// - code: an array of enumerated color values representing the code
bool play_game(enum color code[]) { 
    enum color guess[CODE_LENGTH];
    int num_correct = 0;
    int num_misplaced = 0;
    
    for (int i = 0; i < MAX_GUESSES; i++) {
        printf("Guess %d: ", i + 1);
        get_guess(guess);
        if (!is_valid_guess(guess)) {
            printf("Invalid guess! Please try again.\n");
            i--;
            continue;
        }
        check_guess(code, guess, &num_correct, &num_misplaced);
        print_feedback(num_correct, num_misplaced);
        if (num_correct == CODE_LENGTH) {
            printf("Congratulations! You guessed the code.\n");
            print_code(code);
            return true;
        }
    }

    printf("Sorry, you ran out of guesses.\n");
    print_code(code);
    return false;
}

// Function name: print_code
// Input the enumerated code values (e.g. GREEN RED BLUE YELLOW) and the function outputs and prints the letter values (e.g. GRBY)
// Inputs:
// - code: an array of enumerated color values representing the code
void print_code(enum color code[]) { 
    printf("The code is: ");
    for (int i = 0; i < CODE_LENGTH; i++) {
        switch (code[i]) {
            case GREEN:
                printf("G");
                break;
            case BLUE:
                printf("B");
                break;
            case RED:
                printf("R");
                break;
            case YELLOW:
                printf("Y");
                break;
            case WHITE:
                printf("W");
                break;
            case ORANGE:
                printf("O");
                break;
            default:
                break;
        }
    }
    printf("\n");
}

// Function name: wants_to_play_again
// Prompts the user if they wish to play the game again. Returns true if the user wants to play again (i.e. if they answer 'y' or 'Y'), false otherwise
bool wants_to_play_again(void) { 
    char answer;

    printf("Do you want to play again? (y/n)\n");
    scanf(" %c", &answer);

    return (answer == 'y' || answer == 'Y');
}

int main(void) {
    int games_played = 0;
    int games_won = 0;
    bool play_again = true;
    srand((unsigned)time(NULL));  // Seed the random number generator

    print_rules();

    while (play_again) {
        games_played++;
        enum color code[CODE_LENGTH];

        generate_code(code);

        if (play_game(code)) {
            games_won++;
        }

        play_again = wants_to_play_again();
    }
    printf("\nThanks for playing!\n");
    printf("You won %d out of %d games.\n", games_won, games_played);
    return 0;
}
