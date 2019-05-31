# Torsor

### Summary

Torsor is a very small C++ one-header one-class library 
for expressing and enforcing the difference between relative 
and absolute (anchored) values, aka torsors.
Much like a unit system, this library uses the type system 
to help you eliminate erroneous operations at compile-time.
It can also help to make an API simpler and more elegant
by making the difference between relative and absolute 
values explicit in the type system.
By design, it has no runtime impact.

<!---
![torsor pokemon card](http://www.mypokecard.com/my/galery/JWOi565xQRaB.jpg)
I hope it doesn't pozone you.
-->

### Author

> Wouter van Ooijen (wouter@voti.nl)

### Licensing

> copyright: (c) Wouter van Ooijen - 2019
<BR>source license: [boost](
https://www.boost.org/users/license.html)
<BR>documentation license (this file and the other .md files): [CC BY-SA 2.5](
https://creativecommons.org/licenses/by-sa/2.5/)
<BR>Though not required, I appreciate being informed of any use of tis library,
and feedback is of course also welcomed.

### Requires

> gcc > 6.2 (I used GCC/MinGW 7.3.0) with -fconcepts

### Gitprint 

> [Gitprint-formatted version](
https://gitprint.com/wovo/torsor/blob/master/readme.md) of this document

------------------------------------------------------------------------------

# Introduction

We are used to numerical value types that can be 
added, subtracted, multiplied, and divided. 
But for some real-world values only a more limited 
set of operations makes sense.

The most illustrating example is perhaps time
(as measured in some unit, let's assume seconds).
There are two subtly distinct notions of time:

- a duration (how long something took)
- a moment in time (when something happened)

It makes sense to add two durations 
(10 seconds + 5 seconds = 15 seconds) but it makes no sense
to add two moments ( today + tomorrow, or now + 10 minutes ago).
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
between relative and absolute values
explicit in the type system, instead of requiring two 
functions with different names.

------------------------------------------------------------------------------

# Theoretical background

## the four classic scales

The notion of four types of *scales*
(nominal, ordinal, interval and ratio) with increasing sets of
operations, was introduced by Stevens and later made more rigorous. 
This [Wiki](https://en.wikipedia.org/wiki/Level_of_measurement)
gives a good introduction.

- The values in a *nominal* scale can be *distinguished* from each other
(equality and not-equality operators),
but there are no other useful operations on such values.
Examples of nominal scales are colors and biological species.

- The values in an *ordinal* scale can be distinguished like
those of a nominal scale, and in addition they can be *ordered*
(greater, less, greater-or-equal, less-or-equal operators).
Examples of ordinal scales are scores on a bad-average-good-excellent
or similar scale.

- The values of an *interval* scale can be distinguished and 
ordered, like those of an ordinal scale, and in addition the 
*distances between values can be compared*.
In the common case the values are equidistant or continuous,
which means that there is a one-to-one correspondence between
the scale values and the set of integers or rational numbers.
A consequence is that interval scale values can meaningfully be 
averaged.
Examples of interval scales are locations and moments in time.

- The values of a *ratio* scale can be distinguished, ordered and
averaged, like those of an interval scale, and in addition
they can be *multiplied by a scalar value*, or two values can be
divided to get a scalar value. For two ratio scale values
it makes sense to say that one is twice as large as the other.
A ratio scale has a zero value which you can add to any other value
without changing it.
Examples of ratio scales are distances, temperature differences,
and time durations.

In C++, we don't have built-in types with sets of operations 
that match the four scales. As a best approximation, 
an enum class can be used for nominal and interval scales, and
and appropriate numerical type for interval and ratio scales.
Note that these C++ types support operations that are not meaningful
for some scales:

- Nominal scale values expressed as enum class values can be ordered,
which is meaningless.

- Interval scale values expressed as numerical types can be 
multiplied and divided by a scalar value, which is meaningless.
 
- Nominal and interval scale values expressed as numerical values 
can freely be mixed with each other and with scalar (non-scale) values,
which is meaningless.

## units of measurement

A *unit of measurement* is an agreed-upon value in which a 
quantity (a measured value) can be expressed,
by stating the ratio between the measured value 
and the unit of measurement.
When I state that my house is *9 m* high, I mean that 
the height of my house divided by the standard meter yields the value 9.
This [Wiki](https://en.wikipedia.org/wiki/Unit_of_measurement)
gives a good introduction to units of measurement.

Different quantities require different units, but it turns out
that only a limited set of units is sufficient to express all
quantities.
Speed for instance could use its own unit, but when units for 
distance and time are available, they can be re-used as
a derived unit to express speed as distance divided by time.
Different minimal sets can be defined, but the 
[International System of Units](
https://en.wikipedia.org/wiki/International_System_of_Units)
(SI) is the generally accepted one.

The set of meaningful operations on quantities is easily expressed:

- Quantities can only be added or subtracted when they have the same unit.

- Multiplication of quantities multiplies their units.

- Division of quantities divides their units.

- The context of an strongly typed imperative language, a variable
has a unit, and can only hold values expressed in that unit.

In C++, we don't have built-in expressions or types that 
behave in a unit-correct way, but the language abstraction mechanisms
can be used to create such a system.
In such a unit library, each value has an associated 
set of the powers (exponents) for base unit of the system.
These powers are present only at compile type, 
when they are used to generate compiler errors for operations that
are meaningless.

[Boost::units](
https://github.com/boostorg/units) is probably the most widely known
library of this type.
It is a big library, aparently without a gentle introduction.
It is rumoured that the set of people who fully understand
unit tehory is small, as is the set of people who understand
C++11 enough to write (and some people say *use*) such a library.
The intersection of those two sets contains exactly two people,
who happen to be the authors of boots::units.


## torsors

A torsor is a mathematical concept. 
Quoting from the (very readable)
[Torsors Made Easy](
http://math.ucr.edu/home/baez/torsors.html) 
page:

> In a group G you can can add elements and also subtract them. 
> But you can't add elements of a G-torsor X. 
> Instead, you can add an element of G 
> to an element of X and get another element of X. 
> You can also subtract two elements of X and get an element of G.

In my words: the torsor of a ratio scale is 
its corresponding interval scale.

The [nlab torsor page ](
https://ncatlab.org/nlab/show/torsor) 
gives the slogan

> A torsor is like a group that has forgotten its neutral element.

My interpretation: when you measure in a ratio scale 
(for example time duration) but you choose too
anchor your scale at some (arbitrary) moment in time
by defining that moment as 0, you have lost the special meaning of
zero: you can no longer add that 0 value to another value
and get that same value as a result. In fact,
you can no longer add such 'anchored' values at all.

Another interpretation is that by anchoring 
(defining a zero) you have added some unknown amount
to all values,
which makes addition meaningless because the sum
would contain that unknown amount twice.

A real world example: 
the group of time differences (a ratio scale,
which can be added or subtracted, 
and multiplied or divided by a scalar), 
has an associated
torsor which is the group of moments in time (an interval scale).
Such moments can not be meaningfully added, or
multiplied or divided by a scalar.
But two time moments can be subtracted, which yields
a time duration,
and you can add a time moment and a time duration, 
which yields a time duration.

The 
[*torsor wiki*](
https://en.wikipedia.org/wiki/Torsor_(algebraic_geometry))
is probably a good source for a mathematician,
but I found it inaccessible.

This [blog from *The n-Category Cafe*](
golem.ph.utexas.edu/category/2013/06/torsors_and_enriched_categorie.html) 
tries to be accessible, but I guess I am not part of the intended audience.


------------------------------------------------------------------------------

# Interface summary

In simple terms: with a torsor you can 

- construct, copy, assign

- add and subtract things that can be added to or subtracted from its base

- compare torsors, which means comparing their base values

- subtract two torsors, which yields a value of its base type

- print a torsor, which prints '@' followed by the torsors base value

More formally: the library provides a final class 
template *torsor\<typename B>*. 
The type B must have a constructor that can be called with a single value 0.

The library supports the following operations:

- default constructor, copy constructor, assignment operator

- for each torsor\< B > t and X x: 

  ( t + x ), ( x + t ), ( t - x ), ( x - t ), ( + t )
  
  These operators are provided if and only if
they are available for B and X.
The result is a torsor of the decltype( t op x ) or ( x op t).

- for each torsor\< B > t and X x: 

  ( t += x ), ( t -= x )

  These operators are provided if and only if
they are available for B and X. 
The result is a reference to the (appropriately updated) t.

- for each torsor\< B > b and torsor\< C > c: 

  ( b > c ), ( b >= c ), ( b < c ), 
( b <= c ), ( b == c ), ( b != c )

  These operators are provided if and only if
they are available for B and C. 
The result is the result of the same comparison
on the base value: ( t op c ) or (c op t ).

- for each torsor\< B > b and ostream-like s:

  ( s << b )
  
  This operation is provided if and only if
s provides a << operator for a char, and 
a - operator and a << operator for b.
The b value is printed, prefixed by a '@' character.

  This print operator is provided for debug-logging purposes.
A real application will likely either never print torsors,
or define more specific print functions.
Note that the print operator is not a friend, it uses
( t - decltype( t )() ) to retrieve the base value of the torsor.

The requirements required for providing each operation
are checked by concepts.

All operators are inlined, and are 
const and constexpr, where appropriate.
There are currently no exception annotations 
(I work with -fno-exceptions).
The library itself doesn't generate any exceptions, but
the operations it does on the base type could.

The library has no dependencies, mot even on any part of the 
standard library.

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
Mateusz Pusz' asked 

> what (if anything) is the sum of two temperatures,
> for instance 10 degrees Celcius + 20 degrees Celcius?

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
   
This is how the library behaves, buy I doubt that is useful to anyone.

------------------------------------------------------------------------------

# Limitations

The torsor class limits the operations on a torsor 
to basically adding or subtracting a base type value, 
or subtraction two torsors to yield a base type value.
AAding two torsors is possible, but it yields a torsor/<torsor<>>.

As a colleague remarked, taking the average of a number of torsor values
is a valid operation, which can not be done directly with this torsor
class.

------------------------------------------------------------------------------

# To do list

- find a nice torsor picture that isn't a user-defined pokemon (anchor??)
- copyright formats wrong in pandoc
- more tests
- add spaceship (when it becomes available)
- transparent exception passing 
- ::torsor means that it can't be put in a namespace?
- eliminate the torsor(int,42) hack
- move constructor??
- write a matching unit library (when template value parameters works)
- write a matching newtype library, like 
github.com/anthonywilliams/strong_typedef/blob/master/strong_typedef.hpp

