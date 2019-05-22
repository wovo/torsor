UNFINISHED - WORK IN PROGRESS 

# Torsor

a C++ one-header one-class library for expressing and enforcing
the difference between relative and absolute (anchored) values

2019-05-22

wouter@voti.nl

------------------------------------------------------------------------------
## Introduction

This very small library used the type system to help you prevent 
a class of erroneous operations. 
It can also help to make an API simpler and more elegant.

As I see it, a torsor is a mathematical abstraction over a group 
(a set of values with associated operations) that assigns a special 
meaning to one value. 

The 
[torsor wiki](https://en.wikipedia.org/wiki/Torsor_(algebraic_geometry))
is not very readable for a non-mathematician,
but this 
[Torsors Made Easy] page (http://math.ucr.edu/home/baez/torsors.html)
is quite readable.

In terms of scales, for a value type that denotes a ratio scale value 
(a value for which addition yields a value on the same scale), the
torsor of that type is the corresponding interval scale 
(anchored) type.

Examples of ratio scales and their corresponding torsors 
(anchored interval scales) are:

| ratio scale T             | interval scale torsor< T >   |
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
## Interface

------------------------------------------------------------------------------
## Timing example

Imagine a timing library for a small embedded system that uses the 
type *duration* to express an amount of time. 
It has a function *now()* that
returns the current time, expressed as the time elapsed since
some (undefined) epoch. 
(For an embedded system the epoch could
be the moment the system was last switched on.) 
The type returned by *now()* should be *torsor\<duration>*,
because that type itself is not a duration, but the differnce
between two such types is a duration.

```C++
using duration = ...
torsor< duration > now();
```

Our timing library is likely to have a function that can be
called to wait. In fact, it will likely have two such functions:
one that takes and amount of time as argument and waits for that
amount of time, and one that takes a moment in time as argument,
and waits until that moment has arrived. 
The argument of the first function is a *duration* (an amount of time),
the argument of the second function is a *moment* in time.
Hence the two functions can be overloaded.

```C++
void wait( duration );
void wait( torsor< duration > );
```

With these definitions a user can't make the mistake of adding
two moments.

```C++
auto a = now();
auto b = now();
a +  b: // won't compile
```
This simple benchmark function shows that the difference bewteen two
moments is a duration.

```C++
< template F >
duration time_to_run( F work ){
   auto start = now();
   work();
   auto stop = now();
   return stop - start;
};   
```
The distinction between absolute time and moments in time can be found
in the C++ [std::chrono](https://en.cppreference.com/w/cpp/chrono) library,
but it does not generalise the concept of a ration value range and its 
corresponding torsor (interval value range).

------------------------------------------------------------------------------
## Graphics example

------------------------------------------------------------------------------
## Limitations

average

