#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"

node_t *store_the_words(char *line, node_t *list);

int check_formatting(node_t *list, int boolean);

int check_change(node_t *list, int boolean);

int get_width(node_t *list, int width);

int get_indent(node_t *list, int indent);

int main(int argc, char *argv[]) {
    FILE *input_stream = NULL;
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    node_t *list = NULL;
    char *t = NULL;
    char *copy = NULL;

    //formatting specific variables
    int formatting = 0; //0 for off, 1 for on
    int change = 0; //0 if no change, 1 if change
    int width = 80;
    int indent = 0;
    int num_chars = 0;
    int printed = 0; //if line has printed before formatting off this is 1 for true

    if (argc < 2) {
        input_stream = stdin;
    } else {
        input_stream = fopen(argv[1], "r");
        if (input_stream == NULL) {
            fprintf(stderr, "%s: unable to open %s\n",
                argv[0], argv[1]);
            exit(1);
        }
    }

    //This loop dynamically allocates memory for each line of the input    
    while((read = getline(&line, &len, input_stream)) != -1){
        if(line != NULL){
                //copy the line
                copy = strdup(line);
                t = strtok(copy, " ");
                /*remove new line character so there won't be random new lines in 
                formatted text*/
                if(copy[read-1] == '\n' && read != 1){
                    copy[read-1] = '\0';
                }
                //store all words in linked list
                list = store_the_words(t, list);
                //check if there is a formatting command, if there is check what it is.
                change = check_change(list, change);
                if(change == 1){
                    formatting = check_formatting(list, formatting);
                }
                //no formatting or change means print the original line
                if(formatting == 0 && change == 0){
                    printf("%s", line);
                    printed = 0;
                }
                //don't print out a line with a change as the {{}} isn't included in out
                else if(formatting == 0 && change == 1){
                    num_chars = 0;
                }
                /*if there is a formatting command and a change, we need to figure out
                what the formatting command is*/
                else if(formatting == 1 && change == 1){
                    width = get_width(list, width);
                    indent = get_indent(list, indent);
                    num_chars = 0;
                }
                /*if the formatting is on, the words need to be printed according to
                format*/
                /* NOTE!!!!, the print_the_words is located in the LIST.C file,
                as it is really a linked list operation, similar to print_list would
                be*/
                /*printed is only turned on if num_chars isn't 0, since 0 would mean
                nothing was actually printed*/
                else if(formatting == 1 && change == 0){
                    num_chars = print_the_words(list, width, indent, num_chars);
                    if(num_chars != 0){
                        printed = 1;
                    }
                }
        }
        /*if the tokenized line is just a new line character, and formatting is on,
        a second new line has to be printed to signal end of a paragraph*/
        if(strncmp(t, "\n", 1) == 0){
            if(printed == 1){
                printf("\n");
                printed = 0;
            }
            num_chars = 0;
        }
        //free all unfreed memory in the linked list
        node_t *temp_n = NULL;
        for( ; list != NULL; list = temp_n){
            temp_n = list->next;
            free(list);
        }
        //free the allocated memory for the copied line
        free(copy);
    }
    //all tests include newline at end, manually add one if formatting is on at end
    if(formatting == 1)
        printf("\n");
    
    if(line){
        free(line);
    }

    /* Time to finish. */

    fclose(input_stream);
    exit(0); 
}

//this function goes through the line and adds to the end of the linked list
node_t *store_the_words(char *line, node_t *list){
    while (line != NULL){
        list = add_end(list, new_node(line));
        line = strtok(NULL, " ");
    }
    return list;
}

//this func checks the first node in the list, if it is a {{ there is a formatting cmd
int check_change(node_t *list, int boolean){
    if(compare(list, "{{", 2) != 0){
        boolean = 0;
    }
    else boolean = 1;
    return boolean;
}

//this func checks the second node in the list, is only called if there is a fmt cmd
int check_formatting(node_t *list, int boolean){
    if(compare(list->next, "off", 3) == 0){
        boolean = 0; //turn off formatting
    }
    else if(compare(list->next, "!", 1) == 0){
        boolean = (boolean + 1) % 2; //switch on to off and vice versa
    }
    else{
        boolean = 1;
    }
    return boolean;
}

//this func checks if the second node in the list is only a number, as that is the width
int get_width(node_t *list, int width){
    node_t *next = NULL;
    next = list->next;
    char *word = next->word;
    if(isdigit(*word)){
        width = strtol(word, &word, 10);
    }
    return width;
}

//this function looks for any indenting command
int get_indent(node_t *list, int indent){
    long temp;
    int success = sscanf(list->next->word, "%*[^0123456789]%li", &temp);
    if(success == 1){
            if(compare(list->next, ">", 1) == 0){
                indent = temp;
            }
            else if(compare(list->next, "+", 1) == 0){
                indent = indent + temp;
            }
            else if(compare(list->next, "-", 1) == 0){
                indent = indent - temp;
            }
    }
    return indent;
}
