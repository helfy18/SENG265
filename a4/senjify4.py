import sys
import re

class SENJIFY:

    def __init__(self, input_stream):
        self.input_stream = input_stream


    def format(self):
        result = []
        is_format = False
        is_change = False
        width = 80
        indent = 0
        num_chars = 0
        curr = []

        """regular expression to search for formatting command. group 1 will be command"""
        check_format = re.compile(r"^{{\s(\S*)\s}}")
        
        for line in self.input_stream:
            """the tester prints new lines automatically, so add '' to get extra \n"""
            if line == "\n":
                if curr:
                    buff = ''.join(curr)
                    result.append(buff)
                    curr = []
                num_chars = 0
                result.append("")
                continue
            token_line = line.strip('\n')

            """if the regular expression is a match, there is a formatting command"""
            change = check_format.search(token_line)
            if change:
                is_change = True
                is_format = self.__check_format(change.group(1), is_format) 
            else:
                is_change = False

            if not is_format and not is_change:
                result.append(token_line)

            elif not is_format and is_change:
                if curr:
                   buff = ''.join(curr)
                   result.append(buff)
                   curr = []
                num_chars = 0

            elif is_format and is_change:
                width = self.__get_width(change.group(1), width)
                indent = self.__get_indent(change.group(1), indent)
                num_chars = 0

            elif is_format and not is_change:
                words = token_line.split()
                res_list = self.__format(width, indent, words, num_chars, result, curr)
                num_chars = res_list[0]
                result = res_list[1]
                curr = res_list[2]

        """if there is anything remaining in the buffer list, print"""
        if curr:
            buff = ''.join(curr)
            result.append(buff)
        return result

    """this function checks the command and returns on or off (T or F)"""
    def __check_format(self, command, is_format):
        if command == 'off':
            return False

        if command == "!":
            return not is_format
     
        else:
            return True

    """this function checks the command and returns the width"""
    def __get_width(self, command, width):
        if command.isdigit():
            return int(command)
        return width

    """this function check the command and returns the indent"""
    def __get_indent(self, command, indent):
        p = re.search(r"^\>(\d*)", command)
        if(p):
            return int(p.group(1))
        p = re.search(r"^\+>(\d*)", command)
        if(p):
            return int(p.group(1)) + indent
        p = re.search(r"^\->(\d*)", command)
        if(p):
            return indent - int(p.group(1))
        return indent

    """format the words into the array"""
    def __format(self, width, indent, words, num_chars, result, curr):
        for i in words:
            if i == "\n":
                num_chars = 0
                buff = ''.join(curr)
                result.append(buff)
                curr = []
                continue
            """when the words will wrap over the edge, create a buffer sentence
            and add the sentence to the end of the result list"""
            if num_chars + len(i) + 1 > width:
                buff = ''.join(curr)
                result.append(buff)
                curr = []
                num_chars = 0
            if num_chars != 0:
                curr.append(" ")
                num_chars = num_chars + 1
            else:
                if indent:
                    curr = [" " * indent]
                    num_chars = indent
            curr.append(i)
            num_chars = num_chars + len(i)
        return [num_chars, result, curr]
