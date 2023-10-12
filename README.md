# All data structures written by myself
- View WRITEUP.pdf for statistics
- The created Bloom filter, Bit Vector, Hash Table, and Linked List are used to create a "chat moderation" system that detects banned words.
- The Bloom Filter and Bit Vector are used to increase efficiency to provide an estimate on whether or not the word exists in the Hash Table.

How to run and use banhammer.
================================================

IGNORE "test.c" and "test"!!!!!
--------------------
>
> This is a testing file and is NOT intended to be used!
>

What banhammer do?
------------------
> Reads stdin and detects if there are words that are found in the files "newspeak.txt" and "oldspeak.txt"
>
> "newspeak.txt" includes banned words along with a translation
> "oldspeak.txt" includes banned words that do not have a new translation
>
> Note: you MUST name your files "newspeak.txt" and "oldspeak.txt" or else the program
> WILL NOT WORK!
>
>


Command Line Options
--------------------
>banhammer
>------------------------------------------------------------------------------------------------
>Usage: ./banhammer [options]

>  ./banhammer will read in words from stdin, identify any badspeak or old speak and output an
>  appropriate punishment message. The badspeak and oldspeak (with the newspeak translation) 
>  that caused the punishment will be printed after the message. If statistics are enabled
>  punishment messages are supressed and only statistics will be printed.

>    -t <ht_size>: Hash table size set to <hf_size>. (default: 1000)\
>    -f <bf_size>: Bloom filter size set to <bf_size>. (default 2^19)\
>    -s          : Enables the printing of statistics.\
>    -m          : Enables move-to-front rule.\
>    -h          : Display program synopsis and usage.
>


How to build and run the program.
=================================

Step One:
---------
> Ensure that the files:

> "banhammer.c"\
> "bf.c"\
> "bf.h"\
> "bv.c"\
> "bv.h"\
> "city.c"\
> "city.h"\
> "ht.c"\
> "ht.h"\
> "ll.c"\
> "ll.h"\
> "node.c"\
> "node.h"\
> "parser.c"\
> "parser.h"\
> "test.c"\
> "messages.h"\
> "Makefile"
> --------------------------------------------
> These files are not included in the directory, 
> you must make them yourself
> "oldspeak.txt"
> "newspeak.txt"
>

> Ensure all the files are inside the same directory.
>
Step Two:
---------
> Open the terminal in the directory and type "make"
>
Step Three:
-----------
> Use the usage methods of ./banhammer located above in the "Command Line Options" section
>
> You may also use the -h with the program to print the usage to the terminal instead
> of looking through the README

Cleanup:
-----------
> Type "make clean" to delete all derived files except the created binaries.
> type "make spotless" to delete all derived files includeing the created binaries.
