UNFINISHED - WORK IN PROGRESS 

# Torsor

![torsor pokemon card](http://www.mypokecard.com/my/galery/JWOi565xQRaB.jpg)

This is a C++ one-header one-class library for expressing and enforcing
the difference between relative and absolute (anchored) values.
I hope it doesn't pozone you.

author: Wouter van Ooijen (wouter@voti.nl)<BR>
source license: [boost](
   https://www.boost.org/users/license.html)<BR>
documentation license (this file): [CC BY-SA 2.5](
   https://creativecommons.org/licenses/by-sa/2.5/)<BR>

------------------------------------------------------------------------------
## Introduction

This very small library used the type system to help you prevent 
a class of erroneous operations. 
It can also help to make an API simpler and more elegant.

As I see it, a torsor is a mathematical abstraction over a group 
(a set of values with associated operations) that assigns a special 
meaning to one value. 

In terms of scales, for a value type that denotes a ratio scale value 
(a value for which addition yields a value on the same scale), the
torsor of that type is the corresponding interval scale 
(anchored) type.

Examples of ratio scales and their corresponding torsors 
(anchored interval scales) are:

| ratio scale **T**         | interval scale **torsor< T >**   |
|---------------------------|----------------------------------|
| temperature difference    | absolute temperature             |
| distance vector           | location                         |
| duration                  | moment in time                   |

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
## Further reading

The 
[torsor wiki](https://en.wikipedia.org/wiki/Torsor_(algebraic_geometry))
is not very readable for a non-mathematician,
but this 
[Torsors Made Easy](http://math.ucr.edu/home/baez/torsors.html) 
page is quite readable.
This [blog from The n-Category Cafe](
https://golem.ph.utexas.edu/category/2013/06/torsors_and_enriched_categorie.html) 
tries to be accessible, but I guess I am not part of the intended audience.

------------------------------------------------------------------------------
## Interface

------------------------------------------------------------------------------
## Use

The library is the single header file library/torsor.hpp, 
so you can copy it to some
suitable place (where your compiler can find it) and insert

```C++
#include <torsor.hpp>
```

in your source file(s).

The file contains Doxygen comments.

------------------------------------------------------------------------------
## Usage example: timing

Imagine a timing library for a small embedded system that uses the 
type *duration* to express an amount of time. 
It has a function *now()* that
returns the current time, expressed as the time elapsed since
some (undefined) epoch. 
(For an embedded system the epoch could
be the moment the system was last switched on.) 
The type returned by *now()* should be *torsor\<duration>*,
because it is not a duration, 
but the difference between two such values *is* a duration.

```C++
using duration = ...
torsor< duration > now();
```

Our timing library is likely to have a function that can be
called to wait some time. 
In fact, it will likely have two such functions:
one that takes and amount of time as argument and waits for that
amount of time, and one that takes a moment in time as argument,
and waits until that moment has arrived. 
The argument of the first function is a *duration* (an amount of time),
the argument of the second function is a *moment* in time.
Making the argument of the second function a torsor 
the two functions can be overloaded.

```C++
// wait for the specified amount of time
void wait( duration ); 

// wait untill the specified moment in time
void wait( torsor< duration > );
```

With these definitions a user can't make the mistake of adding
two moments.

```C++
auto a = now();
auto b = now();
a +  b: // won't compile
```
This simple benchmark function shows that the difference between two
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
but that library does not generalise the concept of a 
ratio value range and its corresponding torsor (interval value range).

------------------------------------------------------------------------------
## Usage example: rectangle

A graphics library will have a type *location* that specifies 
a place on the graphics screen. 
A class that represents a rectangle object on the screen will
take one argument to specify the start (upper left) point of the rectangle, 
and one more argument. 
But what does that second argument specify, the end (lower right) point,
or the size of the rectangle 
(the distance between the upper-left and lower-right points).
Both are valid choices.

When you realize that a location on the screen is actually the torsor
of a distance on the screen, the two options can be provided by
two constructors that different second arguments.

```C++
using distance = ...
using location = torsor< distance >;

class rectangle {
   . . .
public:
   rectangle( location start, location end );
   
   rectangle( location start, distance size ): 
      rectangle( start, start + size ){}
}; 
```

------------------------------------------------------------------------------
## Limitations

The operations on a torsor are limited to adding or subtracting a
base type value, or subtraction two torsors to yield a base type value.
As a colleague remarked, this makes it difficult to average 
a number of torsor values, which is a perfectly sensible operation.
