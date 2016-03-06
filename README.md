NAME
====

jo - JSON output from a shell

SYNOPSIS
========

jo [-p] [-a] [word word ...]

DESCRIPTION
===========

*jo* creates a JSON string on *stdout* from *word*s given it as
arguments or read from *stdin*. Without option `-a` it generates an
object whereby each *word* is a `key=value` (or `key@value`) pair with
*key* being the JSON object element and *value* its value. *jo* attempts
to guess the type of *value* in order to create number (using
*strtod(3)*), string, or null values in JSON.

*jo* treats `key@value` specifically as boolean JSON elements: if the
value begins with `T`, `t`, or the numeric value is greater than zero,
the result is `true`, else `false`. A missing or empty value behind the
colon results in a `null` JSON element.

*jo* creates an array instead of an object when `-a` is specified.

EXAMPLES
========

Create an object. Note how the incorrectly-formatted float value becomes
a string:

    $ jo tst=1457081292 lat=12.3456 cc=FR badfloat=3.14159.26 name="JP Mens" nada= coffee@T
    {"tst":1457081292,"lat":12.3456,"cc":"FR","badfloat":"3.14159.26","name":"JP Mens","nada":null,"coffee":true}

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

Create objects within objects; this works because if the first character
of value is an open brace or a bracket we attempt to decode the
remainder as JSON. Beware spaces in strings ...

    $ jo -p name=JP object=$(jo fruit=Orange hungry@0 point=$(jo x=10 y=20 list=$(jo -a 1 2 3 4 5)) number=17) sunday@0
    {
     "name": "JP",
     "object": {
      "fruit": "Orange",
      "hungry": false,
      "point": {
       "x": 10,
       "y": 20,
       "list": [
        1,
        2,
        3,
        4,
        5
       ]
      },
      "number": 17
     },
     "sunday": false
    }

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

Probably.

If a value given to *jo* expands to empty in the shell, then *jo*
produces a `null` in object mode, and might appear to hang in array
mode; it is not hanging, rather it's reading *stdin*. This is not a bug.

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
-   strtod(3)

AUTHOR
======

Jan-Piet Mens <http://jpmens.net>

