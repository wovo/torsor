// ==========================================================================
//
// main.hpp
//
// torsor compilation tests (things that must not compile)
//
// https://www.github.com/wovo/torsor
// 
// Copyright Wouter van Ooijen - 2019
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

#include <iostream>
#include <vector>
#include "torsor.hpp"


// ==========================================================================
//
// test framework for verifying that certain expressions do NOT compile
//
// A concept (requires clase in the macro) is used to detect if
// an expression is valid. The 'x *' before the expression
// prevents premature evaluation of the expression
// (which is IMO a bug in the current GCC).
//
// I have not found a way to do this inside a block scope, 
// because templates can't be inside a block scope.
//
// ==========================================================================

// hide te type if the right hand side of * 
// to prevent a premature compiler errorn 
// (is this a compiler bug?)
struct dummy_multiply {
	
   template< typename T >
   const T & operator * ( const T & x ){
      return x;
   }      
   
   template< typename T >
   T & operator * ( T & x ){
      return & x;
   }      
};

// the results of the tests
struct result {
   bool         expected;
   int          line;
   std::string  text;
   bool         result;
};
std::vector< result > results;
int add( result r ){
   results.push_back( r );
   return 42;
}

#define CONCAT( A, B ) A ## B

// Check if EXP is legal by using it in the requires clause
// of the first f() template ( the one that returns 1):
// if EXP turns out to be illegal the fallback is selected,
// which returns 0.

#define ERROR2( RES, LINE, EXP )                     \
                                                     \
struct CONCAT( box, LINE ) {                         \
                                                     \
   template< typename T >                            \
   requires requires( T x ) {                        \
      ( x * EXP );                                   \
   }                                                 \
   bool f(){ return 1; }                             \
                                                     \
   template< typename T >                            \
   bool f(){ return 0; }                             \
                                                     \
};                                                   \
                                                     \
auto CONCAT( dummy, LINE ) = add( result{            \
   RES, LINE, #EXP,                                  \
   CONCAT( box, LINE )().f< dummy_multiply >() } );  \

#define ERROR( EXP )   ERROR2( 0, __LINE__, EXP ) 
#define ALLOWED( EXP ) ERROR2( 1, __LINE__, EXP ) 

int main(){

   int tests_failed = 0;

   for( auto & r : results ){
      if( r.expected != r.result )
         std::cout 
            << r.expected << " " 
            << r.result << " " 
            << r.line << " " 
            << r.text << "\n";
      if( r.expected != r.result ){
         ++tests_failed;
      }
   }

   if( tests_failed == 0 ){
      std::cout 
         << "\nCompilation test success: " 
         << std::dec << results.size()
         << " test(s) were successfull\n";
      return 0;
   } else {	   
      std::cout 
         << "\nCOMPILATION TEST FAILURE: " 
         << std::dec << tests_failed
         << " test(s) were NOT successfull\n";
      return -1;
   }

}


// ==========================================================================
//
// the tests 
//
// ==========================================================================

torsor< int > _torsor;
int _int;

ALLOWED(  _torsor  = _torsor              )
ERROR(    _torsor  = _int                 ) 
ALLOWED(             _torsor  + _int      )
ALLOWED(  _torsor  = _torsor  + _int      )
ALLOWED(  _torsor  = _int     + _torsor   )
ERROR(    _torsor  = _int     + _int      ) 

ALLOWED(             _torsor + _torsor    )
ERROR(    _torsor  = _torsor + _torsor    )

ALLOWED(  _torsor += _int                 )
ERROR(    _torsor += _torsor              )
ERROR(    _torsor += _torsor + _torsor    )

ALLOWED(             _torsor  - _int      )
ALLOWED(  _torsor  = _torsor  - _int      )
ERROR(               _int     - _torsor   ) 

ALLOWED(             _torsor - _torsor    )
ERROR(    _torsor  = _torsor - _torsor    )
ALLOWED(  _int     = _torsor - _torsor    )

ALLOWED(  _torsor -= _int                 )
ERROR(    _torsor -= _torsor              )
ERROR(    _torsor -= _torsor + _torsor    )

enum class tag1 {};
enum class tag2 {};

torsor< int, tag1  > _t1;
torsor< int, tag2  > _t2;

ALLOWED(  _t1      = _t1                  )
ERROR(    _t1      = _t2                  )

ALLOWED(  _t1      - _t1                  )
ERROR(    _t1      - _t2                  )

ALLOWED(  _t1      > _t1                  )
ERROR(    _t1      > _t2                  )

ALLOWED(  _t1     >= _t1                  )
ERROR(    _t1     >= _t2                  )

ALLOWED(  _t1      < _t1                  )
ERROR(    _t1      < _t2                  )

ALLOWED(  _t1     <= _t1                  )
ERROR(    _t1     <= _t2                  )


