/*
 * linkedlist.c
 *
 * Based on the implementation approach described in "The Practice 
 * of Programming" by Kernighan and Pike (Addison-Wesley, 1999).
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "list.h"


node_t *new_node(char *val) {
    assert( val != NULL);

    node_t *temp = (node_t *)emalloc(sizeof(node_t));

    strncpy(temp->word, val, MAX_WORD_LEN);
    temp->next = NULL;

    return temp;
}


node_t *add_front(node_t *list, node_t *new) {
    new->next = list;
    return new;
}


node_t *add_end(node_t *list, node_t *new) {
    node_t *curr;

    if (list == NULL) {
        new->next = NULL;
        return new;
    }

    for (curr = list; curr->next != NULL; curr = curr->next);
    curr->next = new;
    new->next = NULL;
    return list;
}


node_t *peek_front(node_t *list) {
    return list;
}


node_t *remove_front(node_t *list) {
    if (list == NULL) {
        return NULL;
    }

    return list->next;
}

//a function to simplify the compares in senjify3.c, as typing ->word would be more confusing
int compare(node_t *list, char *string, int n){
    return strncmp(list->word, string, n);
}

//a function to print the words according to formatted commands
int print_the_words(node_t *list, int width, int indent, int num_chars){
    node_t *temp_n = NULL;
    for( ; list != NULL; list = temp_n){
        if(compare(list, "\n", 1) != 0){
            if(num_chars + strlen(list->word) + 1 > width){
                printf("\n");
                num_chars = 0;
            }
        
            if(num_chars != 0){
                printf(" ");
                num_chars++;
            }

            else{
                int i;
                for(i = 0; i < indent; i++){
                    printf(" ");
                    num_chars++;
                }
            }

            printf("%s", list->word);
            num_chars = num_chars + strlen(list->word);
        }
        else{
            printf("\n");
            num_chars = 0;
        }
        temp_n = list->next;
    }
    return num_chars;
}

void apply(node_t *list,
           void (*fn)(node_t *list, void *),
           void *arg)
{
    for ( ; list != NULL; list = list->next) {
        (*fn)(list, arg);
    }
}
