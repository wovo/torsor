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

struct dummy_multiply {
   template< typename T >
   auto operator * ( const T & x ){
      return x;
   }      
};

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

#define ERROR2( RES, LINE, EXP )                     \
                                                     \
struct CONCAT( box, LINE ) {                         \
                                                     \
   template< typename T >                            \
   requires requires( T x ) {                        \
      ( x * EXP );                                   \
   }                                                 \
   bool f(){ return 0; }                             \
                                                     \
   template< typename T >                            \
   bool f(){ return 1; }                             \
                                                     \
};                                                   \
                                                     \
auto CONCAT( dummy, LINE ) = add( result{            \
   RES, LINE, #EXP,                                  \
   CONCAT( box, LINE )().f< dummy_multiply >() } );  \

#define ERROR( EXP ) ERROR2( 0, __LINE__, EXP ) 
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

torsor< int > a;
int b;

ALLOWED(  a + a  )
ERROR(    a + b  )
ALLOWED(  b + a  )
ERROR(    b + b  )


