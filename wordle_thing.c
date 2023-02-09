/*
 * Author: Pierce Lane
 * Last Edited: 2/5/2023
 * Wordle thing! Yay! Grab a friend!
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char alpha[27] = "abcdefghijklmnopqrstuvwxyz";
char alpha_status[27] = "--------------------------";

int is_length_five(char* str){
    int str_length = 0;

    //validate guess length
    while (*(str+str_length)){
        str_length++;
    }
    if (str_length == 5){
        return 1;
    } else {
        return 0;
    }
}

int is_in_alphabet(char* str){
    int i = 0, pos;
    char *substring;
    while (*(str + i)){
        substring = strchr(alpha, str[i]);
        pos = substring - alpha;
        if (!(pos >= 0))
            return 0;
        i++;
    }
    return 1;
}

void remove_alpha(const char * guess){
    int i = 0, pos;
    char *str = alpha;
    char *substring;
    while (guess[i]){
        substring = strchr(alpha, guess[i]);
        pos = substring - str;
        if (pos >= 0){
            alpha[pos] = '-';
        }
        i++;
    }
}

void check_word(const char * guess, const char * word){
    int i = 0, pos = -1;
    
    char copy[6], result[6];
    result[5] = 0;
    strcpy(copy, word);

    while (*(copy + i)){
        if (copy[i] == guess[i]){
            result[i] = '#';
            copy[i] = '#';

        } else {
            result[i] = '-';
        }

        i++;
    }

    //find ? spots and put them in result
    i = 0;
    char *str = copy;
    char *substring;
    while (*(copy + i)) {
        substring = strchr(copy, guess[i]);
        pos = substring - str;

        if (pos >= 0 && result[i] != '#'){
            result[i] = '?';
            copy[pos] = '-';
        }

        i++;
    }
    
    //print out result and format it for player viewing and update alpha_status
    char letter;
    i = 0;
    printf("\t");
    //set text color to yellow
    while (*(result + i)){
        letter = guess[i];
        pos = letter - 'a';

        if (result[i] == '#'){
            printf("\033[0;32m%c  ", letter);
            alpha_status[pos] = '#';
        
        } else if (result[i] == '?') {
            printf("\033[0;33m%c  ", letter);

            if (alpha_status[pos] != '#')
                alpha_status[pos] = '?';

        } else {
            printf("\033[1;30m%c  ", letter);
            if (alpha_status[pos] != '#' && alpha_status[pos] != '?')
                alpha_status[pos] = 'X';

        }
        i++;
    }
    printf("\033[0m");
}

void print_alpha(){
    printf("\n");
    int i = 0;
    char status, letter;
    while (*(alpha + i)){
        status = alpha_status[i];
        letter = alpha[i];

        switch (status){
            case '#':
                printf("\033[0;32m%c", letter);
                break;
            case '?':
                printf("\033[0;33m%c", letter);
                break;
            case 'X':
                printf("\033[1;30m%c", letter);
                break;
            case '-':
                printf("\033[0;37m%c", letter);
                break;
        }
        printf(" ");

        i++;
    }
    printf("\033[0m");
}

void reset_alpha_status(){
    int i;
    for (i = 0; i < 26; i++){
        alpha_status[i] = '-';
    }
}

int main(){
    //clear the console
    start: system("cls");
        reset_alpha_status();

        //get the word and validate length
        char word[100] = "";
        while (!is_length_five(word) || !is_in_alphabet(word)){
            printf("First player please enter a 5 letter lowercase word: ");
            scanf("%s", word);
            system("cls");
        }

        printf("Second player:\n");
        char guess[100] = "";
        int num_guesses = 1;
        //while guess is not word
        while (strcmp(guess, word) != 0 && num_guesses <= 6){
            //get guess
            printf("%d) Enter a 5 letter guess: ", num_guesses);
            scanf("%s", guess);
            
            //validate guess length
            if (!is_length_five(guess) || !is_in_alphabet(guess)){
                printf("Please enter a 5 letter lowercase guess.\n\n");
                continue;
            }
            
            //compare the guess to the word and print out how the user did
            check_word(guess, word);
            //print out the alphabet
            print_alpha();
            //increment the number of guesses
            num_guesses++;

            printf("\n\n\n");
        }

        printf("-----------------------------------------");
        if (strcmp(guess, word) == 0){
            //if the user guessed the word, congradulate them
            printf("\nYou did it! The word was %s!", word);
        } else {
            //if they failed, scold them and tell them the word
            printf("\nYou didn't do it :(\nThe word was %s.\n", word);
        }

        char go_again[2];
        printf("\nPlay again? (y/n): ");
        scanf("%s", go_again);

        int pos = strchr(go_again, 'y') - go_again;
        int pos_big_y = strchr(go_again, 'Y') - go_again;
        if (pos >= 0 || pos_big_y >= 0){
            goto start;
        }

    system("pause");

    return 0;
}
