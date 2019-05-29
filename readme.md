# ** UNFINISHED - WORK IN PROGRESS **

# Torsor

### Summary

Torsor is a very small C++ one-header one-class library 
for expressing and enforcing the difference between relative 
and absolute (anchored) values, aka torsors.
Much like a unit system, this library uses the type system 
to help you prevent some erroneous operations.
It can also help to make an API simpler and more elegant. 
By design, it has no runtime impact.

<!---
![torsor pokemon card](http://www.mypokecard.com/my/galery/JWOi565xQRaB.jpg)
I hope it doesn't pozone you.
-->

### Author

> Wouter van Ooijen (wouter@voti.nl)

### Licensing

> source license [boost](
https://www.boost.org/users/license.html)

> documentation license (this file and te other .md files): [CC BY-SA 2.5](
https://creativecommons.org/licenses/by-sa/2.5/)

### Requires

> gcc > 6.2 (I used GCC/MinGW 7.3.0) with -fconcepts

### Gitprint

[Gitprint-formatted version of this document](
https://gitprint.com/wovo/torsor/blob/master/readme.md)

------------------------------------------------------------------------------

# Introduction

We are used to numerical value types that can be 
added, subtracted, multiplied, and divided. 
But for some real-world values only a more limited 
set of operations make sense.

The most illustrating example is perhaps time
(as measured in some unit, let's assume seconds).
There are two subtly distinct notions of time:

- a duration (how long somehing took)
- a moment in time (when something happened)

It makes sense to add two durations 
(10 seconds + 5 seconds = 15 seconds) but it makes no sense
to add two moments, like today and tomorrow, or now and 10 minutes ago.
Just like adding meters to seconds doesn't make any sense,
adding two time moments doesn't make any sense.
Subtracting two moments on the other hand does make sense,
but the result is a duration, rather than a moment in time.

In terms of scales, for a value type that denotes a ratio scale value 
(a value for which addition yields a value on the same scale), the
torsor of that type is the corresponding interval scale 
(anchored) type.

In mathematical terms, the set of moments in time is the
*torsor* of the set of time durations.

Examples of ratio scales and their corresponding torsors 
(anchored interval scales) are:

| ratio scale **T**         | interval scale **torsor< T >**   |
|---------------------------|----------------------------------|
| duration                  | moment in time                   |
| temperature difference    | absolute temperature             |
| distance vector           | location                         |

Whether a scale is a torsor or not has nothing to do with its
unit: in a unit system (like SI) a ratio type and its torsor have the 
same unit. 

But just like adding two values that have different SI units 
makes no sense, adding two torsor values makes no sense.
The torsor class template uses the type system to block
such meaningless operations at compile time. 
It is designed to have zero runtime overhead.

Having different types for a ration scale and its torsor
can make an API more elegant, because it makes the difference
explicit in the type system, instead of requiring 
functions with different names.

------------------------------------------------------------------------------

# Mathematical background

As I understand it, a torsor is a mathematical abstraction over a group 
(a set of values with associated operations) that assigns a special 
meaning to one value.

- The 
[torsor wiki](https://en.wikipedia.org/wiki/Torsor_(algebraic_geometry))
is not very readable for a non-mathematician.

- This 
[Torsors Made Easy](http://math.ucr.edu/home/baez/torsors.html) 
page is quite readable.

- This [blog from The n-Category Cafe](
https://golem.ph.utexas.edu/category/2013/06/torsors_and_enriched_categorie.html) 
tries to be accessible, but I guess I am not part of the intended audience.

- [This page](https://ncatlab.org/nlab/show/torsor) gives the slogan
"A torsor is like a group that has forgotten its neutral element."
Otherwise I found it hard to read.

------------------------------------------------------------------------------

# Interface summary

In simple terms: with a torsor you can 

- construct, copy, assign

- add and subtract things that can be added to or subtracted from its base

- compare torsors, which means comparing their base values

- subtract two torsors, which yields its base type

More formally: the library provides a final class 
template *torsor\<typename B>*. 
The type B must have a constructor that can be called with a single value 0.

The library supports the following operations:

- default constructor, copy constructor, assignment operator

- for each torsor\< B > t and X x: 

  ( t + x ), ( x + t ), ( t - x ), ( x - t )
  
  These operators are provided if and only if
they are available for B and X.
The result is a torsor of the decltype( t op x ) or ( x op t).

- for each torsor\< B > t and X x: 

  ( t += x ), ( t -= x )

  These operators are provided if and only if
they are available for B and X. 
The result is a reference to the (appropriately modified) t.

- for each torsor\< B > b and torsor\< C > c: 

  ( b > c ), ( b >= c ), ( b < c ), 
( b <= c ), ( b == c ), ( b != c )

  These operators are provided if and only if
they are available for B and C. 
The result is the result of the same comparison
on the base value: ( t op c ) or (c op t ).

All operators are const and constexpr, where appropriate.
There are currently no exception annotations 
(I work with -fno-exceptions).
The library itself doen't generate any exceptions, but
the operations it does with the base type could.

------------------------------------------------------------------------------

# Use

The library is the single header file library/torsor.hpp, 
so you can copy it to some
suitable place (where your compiler can find it) and insert

```C++
#include <torsor.hpp>
```

in your source file(s).

The library uses concepts, so GCC 6.2 or later is required, 
with the *-fconcepts* command-line flag.

The torsor.hpp file contains Doxygen comments.
The command
'''
make docs
'''
in the root directory generates the Doxygen pages
(Doxygen required),
and a pdf version of this file
(pandoc required).

------------------------------------------------------------------------------

# Usage example: timing

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

// wait until the specified moment in time
void wait( torsor< duration > );
```

With these definitions a user can't make the mistake of adding
two moments.

```C++
auto a = now();
auto b = now();
a + b: // won't compile
```
This simple benchmark function shows that the difference between two
moments is a duration.

```C++
template< typename F >
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

# Usage example: rectangle

A graphics library will have a type *location* that specifies 
a place on the graphics screen. 
A class that represents a rectangle object on the screen will
take one argument to specify the start (upper left) point of the rectangle, 
and one more argument. 
But what does that second argument specify; 

- the end (lower right) point, or 

- the size of the rectangle 
(the distance between the upper-left and lower-right points)?

Both are valid choices.

When you realize that a location on the screen is actually the torsor
of a distance on the screen, the two options can be provided by
two constructors that take different second arguments.

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

# Usage example: temperature

In his [talk at ACCU 2019](
https://www.youtube.com/watch?v=nN5ya6oNImg)
Mateusz Pusz' asked what (if anything) is the sum of two temperatures,
for instance 10 degrees Celcius + 20 degrees Celcius?
He proposed these possible answers:

 - 30 degrees Celcius
 
 - 303 degrees Celcius
 
 - meaningless 
   
He then gave reasonable arguments for all three answers.

With the torsor concept we can unravel this problem by first asking for
more information: are those values to be interpreted as 
temperature differences, or as absolute temperatures 
(the torsor of temperature differences). 

If they are temperature differences we must flog the author and
insist that in the future he writes them as N degrees 
(without the Celcius), because in that case X degrees 
Celcius is the same as X degrees Kelvin,
so appending 'Celcius' or 'Kelvin' is misleading.
(Let's forget about Fahrenheit and Reaumur.)
In that case, 10 degrees + 20 degrees is without any doubt 30 degrees.

If the values are tosor (absolute) temperatures, adding them is meaningless,
insofar that it doesn't produce an absolute temperature or a
temperature difference. 

One could argue that if the addition means anything, it is that
it produces something that:

- when divided by 2, yields an absolute (torsor) temperature

- when you subtract an absolute (torsor) temperature from it, it
   yields an absolute (torsor) temperature.
   
I doubt that is usefull to anyone 
(but check the note at the end of this file about averaging).

------------------------------------------------------------------------------

# Limitations

The torsor class limits the operations on a torsor 
to adding or subtracting a base type value, 
or subtraction two torsors to yield a base type value.
As a colleague remarked, this makes it difficult to average 
a number of torsor values, which is a perfectly sensible operation.

With the current installation, 
you can't use torsor\< torsor\< T >>, because
a torsor requires its base type to have a constructor that accepts
0 as its (only) parameter. 
This is because built-in types like int and double require
this 0 tio be initialized, but a user shouldn't have the option
to initialize a torsor to a specific value. 
The constructor could accept an int and ignore its value,
but that would allow a misleading construction like

```C++
   torsor< int > t( 15 );
```

------------------------------------------------------------------------------

# To do list

- find a nice torsor picture that isn't a user-defined pokemon

