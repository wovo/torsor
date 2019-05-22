# Torsor

a C++ one-header one-class library for expressing and enforcing
the difference between relative and absolute (anchored) values

2019-05-22

wouter@voti.nl

------------------------------------------------------------------------------
## introduction

This very small library is a tool to use the type system to prevent 
a class of erroneous operations. 
It can also help to make an API simpler.

As I se it, a torsor is mathematical abstraction over a group 
(a set of values with associated operations) that assigns a special 
meaning to one value. 

In terms of scales, for a value type that denotes a ratio scale value 
(a value for which addition yields a value on the same scale), the
torsor of that type is the corresponding interval scale 
(anchored) type.

Examples of ratio scales and their corresponding torsors 
(anchored interval scales) are:

| T                         | torsor< T >                  |
|---------------------------|------------------------------|
| temperature difference    | absolute temperature         |
| distance vector           | location                     |
| duration                  | moment in time               |

In a unit system like SI a torsor and its ratio type have the 
same unit. 
But just like adding two values that have different SI units 
makes no sense, adding two torsor values makes no sense.
Consider time: it makes sense to add two durations 
(10 seconds + 5 seconds = 15 seconds) but it makes no sense
to add today and tomorrow, or now and 10 minutes ago.

The torsor class template uses the type system to block
such meaningless operations at compile time. 
It is designed to have zero runtime overhead.

------------------------------------------------------------------------------
## interface

------------------------------------------------------------------------------
## example

Imagine a timing library that uses the type duration to
express an amount of time. It also has a function now() that
returns the current time, expressed as the time elapsed since
some (undefined) epoch. For an embedded system the epoch could
be the moment the system was last switched on. 