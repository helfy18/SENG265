#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 132
#define MAX_NUM_LINES 50000
#define DEFAULT_LINE_WIDTH 80

long formatting = 0;
int num_words = 0;
char words[MAX_NUM_LINES][MAX_LINE_LENGTH];
int counter = 0; //counter for characters when printing

typedef struct format{
    long width; //default width is 80
    long indent; //default indent is 0
    int swtch; // 0 for off, 1 for on
}format;

format fmt; //this is the format for the output, changes throughout the file

long check_fmt(char *line, char *next); //declaring function before use

void store_the_words(char *line){ //adapted from lecture slides of Dr. Zastre
   
    char buffer[MAX_LINE_LENGTH]; //buffer array
    char *t; // temporary token
    int num = 0; // location of word in array to detect formatting commands
    
    strncpy(buffer, line, MAX_LINE_LENGTH);

    //don't tokenize new line character at end of line
    if(buffer[strlen(buffer)-1] == '\n' && strlen(buffer) != 1){
        buffer[strlen(buffer)-1] = '\0';
    }

    t = strtok(buffer, " ");

    //store the words in the array
    while (t!= NULL) {
        strncpy(words[num_words++], t, 20);
        num++;
        t = strtok(NULL, " ");

        /*if we are at the second word in a line, we can check
        //if it is a formatting instruction*/
        if(num == 1){
            formatting = check_fmt(words[num_words - 1], t);
        }
    }
    
    //if formatting is off, don't save the words in the array    
    if(formatting == 1 || fmt.swtch == 0){
        num_words = num_words - num;
    }
}   

long check_fmt(char *line, char *next){
    char str[15];
    char str2[15];
    char str3[15];
    char str4[15];
    long ret;

    //initialize strings for comparisons involving formatting instructions
    strncpy(str, "{{", 15);
    strncpy(str2, "on", 15);
    strncpy(str3, "off", 15);
    strncpy(str4, "!", 15);

    //check if there are square brackets at start of line (for format)
    ret = strncmp(str, line, 2);
    
    if (ret == 0){
        //check for width instruction
        if(isdigit(*next)){
            fmt.swtch = 1; //turn on formatting
            char *temp = next;
            fmt.width = strtol(temp, &temp, 10); //change width
            if(counter != 0){
                printf("\n"); //print new line when changing format
            }
            counter = 0;
        }
        //check for on instruction
        else if(strncmp(str2, next, 2) == 0){
            fmt.swtch = 1; // turn on formatting
            if(counter != 0){
                printf("\n"); //print new line when changing format
            }
            counter = 0;
        }
        //check for off instruction
        else if(strncmp(str3, next, 2) == 0){
            fmt.swtch = 0; //turn off formatting
            if(counter != 0){
                printf("\n"); //print new line when changing format
            }
            counter = 0;
         }
        //check for change symbol
        else if(strncmp(str4, next, 1) == 0){
            fmt.swtch = (fmt.swtch + 1)%2; //if on, turn off and vice versa
            counter = 0;
        }
        //only option left is indent
        else{
            long temp2;
            char *discard;
            int success = sscanf(next, "%*[^0123456789]%li", &temp2);

            fmt.indent = temp2; // turn on indent
            fmt.swtch = 1; // make sure that formatting is on
        }
        
        return 1; //1 signals that theres was a change in format
    }else return 0; //0 signals format remains the same
}

void print_words(char *line){
    int len = strlen(line);

    //check if the next word will go over the edge of the line
    if(len + counter + 1 > fmt.width){
        printf("\n");
        counter = 0;
    }
    //check if a space should be placed in between words
    if(counter != 0 && strncmp(line, "\n", 1) != 0){
        printf(" ");
        counter++;
    }
    //check if at the beginning of a line
    else if(counter == 0){
        /*if the line is supposed to start with another new line,
        make new line and exit, so the line won't be double and indented*/
        if(strncmp(line, "\n", 1) == 0){
            printf("\n");
            return;
        }
        //if the line is not starting with a new line, it is safe to indent
        else{
            int j = 0;
            for(j; j < fmt.indent; j++){
                printf(" ");
                counter++;
            }
        }
    }
    printf("%s", line);
    //update number of characters
    counter = counter + len;
    /*if the character is a new line, reset the counter and make a second
    new line, as it signals the end of a paragraph*/
    if(strncmp(line, "\n", 1) == 0){
        counter = 0;
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    //implement default formatting settings
    fmt.width = DEFAULT_LINE_WIDTH; fmt.indent = 0; fmt.swtch = 0;
    char buffer[MAX_LINE_LENGTH];
    int i = 0;

    //file must be run by piping in a .txt file using cat
    if(stdin == NULL){
        fprintf(stderr, "You must provide a file\n");
        exit(1);
    }
    
    //while loop that goes through stdin line by line and copies into buffer
    while(fgets(buffer, sizeof(char) * MAX_LINE_LENGTH, stdin)){

        store_the_words(buffer); //tokenize each line
        /*if formatting is turned off and the formatting hasn't changed
        print unformatted line*/
        if(fmt.swtch == 0 && formatting == 0){
            printf("%s", buffer);
        }
        //otherwise print tokens according to formatting
        else{
            for(i; i < num_words; i++){
                print_words(words[i]);
            }
        }
    }
    //if the file ends on a formatted line, a new line character is needed
    if(fmt.swtch == 1){   
        printf("\n");
    }

    return 0;

}
