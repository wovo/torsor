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

const char * error_string = nullptr;

struct TakesAnything
{
    // constructed from anything:
    template<typename T> TakesAnything(T){}
    
    void operator=( TakesAnything ) const { 
       error_string = "assignment =";
       //return *this;
    }       

};

template< typename A, typename B >
void operator+( const A &, const B & ){
   error_string = "diadic +";
}

template< typename A, typename B >
void operator-( const A &, const B & ){
   error_string = "diadic -";
}

/*
template< typename A, typename B >
void operator+=( A &, const B & ){
   error_string = "add-assignment +=";
}
*/

void operator+=( TakesAnything, TakesAnything ){ 
       error_string = "add-assignment +=";
} 

struct result {
   bool         expected;
   int          line;
   std::string  text;
   bool         result;
   const char * used;
};
std::vector< result > results;
int add( result r ){
   results.push_back( r );
   return 42;
}

#define ERROR2( RES, LINE, EXP ){                    \
   (EXP); \
   add( result { RES, LINE, #EXP, \
   error_string == nullptr, error_string } ); \
   error_string = nullptr; \
}

#define ERROR( EXP )   ERROR2( 0, __LINE__, EXP ) 
#define ALLOWED( EXP ) ERROR2( 1, __LINE__, EXP ) 

void run_tests();

int main(){
   
   run_tests();

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

void run_tests(){

torsor< int > _torsor;
int _int;

//ALLOWED(  _torsor  = _torsor              )
//ERROR(    _torsor  = _int                 ) 
ALLOWED(             _torsor  + _int      )
//ALLOWED(  _torsor  = _torsor  + _int      )
//ALLOWED(  _torsor  = _int     + _torsor   )
//ERROR(    _torsor  = _int     + _int      ) 

ALLOWED(             _torsor + _torsor    )
//ERROR(    _torsor  = _torsor + _torsor    )

ALLOWED(  _torsor += _int                 )
ERROR(    _torsor += _torsor              )
ERROR(    _torsor += _torsor + _torsor    )

ALLOWED(             _torsor  - _int      )
//ALLOWED(  _torsor  = _torsor  - _int      )
ERROR(               _int     - _torsor   )

ALLOWED(             _torsor - _torsor    )
//ERROR(    _torsor  = _torsor - _torsor    )
//ALLOWED(  _int     = _torsor - _torsor    )

//ALLOWED(  _torsor -= _int                 )
//ERROR(    _torsor -= _torsor              )
//ERROR(    _torsor -= _torsor + _torsor    )

}