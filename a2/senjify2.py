#!/usr/bin/env python3

import fileinput
import sys

def main():

    #default width, indent, and default is formatting off
    width = 80
    indent = 0
    num_chars = 0
    formatting = False

    #printed variable to track if a line actually printed prior to
    #formatting turning off
    printed = False

    for line in fileinput.input():
        token_line = line.strip('')
        words = token_line.split()
        if words:
            #check if there is a formatting command
            change = words[0] == '{{'

            #check for formatting
            if change:
                formatting = check_formatting(words, formatting)

            #case 1, formatting is off, and there was no change
            #the line is just printed onto the screen
            #formatting is off, so printed is off
            if formatting == False and change == False:
                print(token_line, end = '')
                printed = False

            #case 2, formatting is changed to off, num_chars becomes 0
            #don't print this line, as it is something with {{
            elif formatting == False and change == True:
                num_chars = 0

            #case 3, formatting is turned on. Check for what the formatting command is
            #in case going from on to on, reset counter
            elif formatting == True and change == True:
                width = get_width(words, width)
                indent = get_indent(words, indent)
                num_chars = 0

            #case 4, formatting has been on. In this case, print formatted lines using func
            #set printed during formatting to true
            elif formatting == True and change == False:
                 num_chars = print_the_words(width, indent, words, num_chars)
                 printed = True

        #check if the line is a new line. if it is, print it.
        #if lines have been printed, the new line signifies end of paragraph, and two new lines are required
        if token_line == "\n":
            print()

            if printed:
                print()
                printed = False

            num_chars = 0
            
    #if the file ends on a formatted line, there will be no new line at the end, which is required
    if formatting:
        print()



def check_formatting(words, boolean):

    #there is no formatting command if the line doesn't start with {{
    if words[0] != '{{':
        return boolean

    #check if the command is off
    elif words[1] == 'off':
        return False

    #! means to return whatever boolean isn't
    elif words[1] == '!':
        return not boolean

    #every other command turns formatting on
    else:
        return True



def get_indent(words, indent):

    #check for indenting command
    if words[1][0] == '>':
        words[1] = words[1].strip('>')
        return int(words[1])

    #check if the command is to increase the indent
    elif words[1][0] == '+':
        words[1] = words[1].strip('+>')
        return indent + int(words[1])

    #check if the command is to decrease the indent
    elif words[1][0] == '-':
        words[1] = words[1].strip('->')
        return indent - int(words[1])

    #remaining cases are on, or a width, which don't affect indent
    else:
        return indent



def get_width(words, width):
    #if the command is a number, that is the width. otherwise, it is an indent or on
    if words[1].isnumeric():
        return int(words[1])
    else:
        return width



def print_the_words(width, indent, stored_words, num_chars):

    for i in stored_words:

        #check if word is too long for line
        if num_chars + len(i) + 1 > width:
            print()
            num_chars = 0

        #print a space unless at start of line
        if num_chars != 0:
            print(" ", end = '')
            num_chars = num_chars + 1

        #if at the start of new line, print the indent
        elif num_chars == 0:
            print(" " * indent, end = '')
            num_chars = indent
        
        #print the word
        print(i, end = '')
        num_chars = num_chars + len(i)

        #if a new line character occurs, reset the counter
        if i == "\n":
            num_chars = 0 
        
    return num_chars


if __name__ == "__main__":
    main()
