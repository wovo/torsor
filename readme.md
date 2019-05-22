A torsor is a tool to strengthen use the type system 
to prevent a class of erroneous operations. 
It can also help to make an API simpler.

For a value type that denotes a ratio scale value (a value
for which addition yields a value on the same scale), the
torsor of that type is the corresponding interval scale 
(anchored) type.

Examples of ratio scales and their corresponding anchored 
interval scales are:

+---------------------------+------------------------------+
| T                         | torsor< T >                  |
+---------------------------+------------------------------+
| temperature difference    | absolute temperature         |
+---------------------------+------------------------------+
| distance vector           | location                     |
+---------------------------+------------------------------+
| duration                  | moment in time               |
+---------------------------+------------------------------+

In a unit system like SI a torsor and its ratio type have the 
same unit. 
But just like adding two values that have different SI units 
makes no sense, adding two torsor values makes no sense.
Consider time: it makes sense to add two durations 
(10 seconds + 5 seconds = 15 seconds) but it makes no sense
to add today and tomorrow. 

The torsor class template uses the type system to block
such meaningless operations at compile time. 
It is designed to have zero runtime overhead.