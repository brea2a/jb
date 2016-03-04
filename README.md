NAME
====

jo - JSON output from a shell

SYNOPSIS
========

jo [-p] [-a] word [word ...]

DESCRIPTION
===========

*jo* creates a JSON string on *stdout* from *word*s given it as
arguments. Without option `-a` it generates an object whereby each
*word* is a `key=value` pair with *key* being the JSON object element
and *value* its value. *jo* attempts to guess the type of *value* in
order to create number, string, or null values in JSON.

*jo* creates an array instead of an object when `-a` is specified.

EXAMPLES
========

Create an object. Note how the incorrectly-formatted float value becomes
a string:

    $ jo lat=12.3456 cc=FR notPi=3.14159.26 name="JP Mens" money= 
    {"lat":12.3456,"cc":"FR","notPi":"3.14159.26","name":"JP Mens","money":null}

Pretty-print an array with a list of files in the current directory:

    $ jo -p -a *
    [
     "Makefile",
     "README.md",
     "jo.1",
     "jo.c",
     "jo.pandoc",
     "json.c",
     "json.h",
    ]

OPTIONS
=======

*jo* understands the following options.

-a
:   Interpret the list of *words* as array values and produce an array
    instead of an object.

-p
:   Pretty-print the JSON string on output instead of the terse one-line
    output it prints by default.

BUGS
====

Yes.

RETURN CODES
============

*jo* exits with a code 0 on success and non-zero on failure after
indicating what caused the failure.

AVAILABILITY
============

<http://github.com/jpmens/jo>

CREDITS
=======

-   This program uses `json.[ch]`, by Joseph A. Adams.

SEE ALSO
========

-   <https://stedolan.github.io/jq/>
-   <https://github.com/micha/jsawk>

AUTHOR
======

Jan-Piet Mens <http://jpmens.net>

