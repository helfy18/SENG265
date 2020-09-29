#!/usr/bin/env python3

#
# UVic SENG 265, Summer 2020, Assignment #4
#
# This test-driver program invokes methods on the class to be
# completed for the assignment.
#
# THIS IS THE VERSION OF THE TESTER THAT WILL BE USED WITH
# YOUR SUBMISSION OF senjify4.py
#

import sys
import argparse
from senjify4 import SENJIFY


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('infile', type=str, help='input text file',
        nargs="?")

    args = parser.parse_args()

    if not args.infile:
        input_stream = sys.stdin
    else:
        try:
            input_stream = open(args.infile)
        except:
            print("Cannot open", args.infile)
            sys.exit(1)

    orig_stdout = sys.stdout
    sys.stdout = None

    senjify = SENJIFY(input_stream)
    result = senjify.format()

    sys.stdout = orig_stdout
    if result != []:
        print("\n".join(result))

    input_stream.close()


if __name__ == "__main__":
    main()
