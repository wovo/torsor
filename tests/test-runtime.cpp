// ==========================================================================
//
// test-runtime.hpp
//
// torsor runtime tests
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
#include "torsor.hpp"


// ==========================================================================
//
// simple test framework
//
// ==========================================================================

int tests_total = 0;
int tests_failed = 0;

template< typename A, typename B >
void test_ok( 
   bool ok,                          // Test OK?
   const char * f, int n,            // file name and line number
   const char * ta, const char *tb,  // arguments, stringified
   const A & a, const B & b          // arguments, as-is
){
   ++tests_total;	
   if( ! ok ){
      ++tests_failed;
      std::cout 
         << f << ":" << std::dec << n 
         << " check failed \n" 
         << "   HWLIB_TEST_EQUAL( " << ta << " , " << tb << " )\n"
         << "   left  \"" << ta << "\" = " << a << "\n"
         << "   right \"" << tb << "\" = " << b << "\n\n";
   }
}

#define TEST_EQUAL( a, b ) \
   test_ok( (a) == (b), __FILE__, __LINE__, #a, #b, a, b );   

#define TEST_NOT_EQUAL( a, b ) \
   test_ok( (a) != (b), __FILE__, __LINE__, #a, #b, a, b );   

int test_end(){
   if( tests_failed == 0 ){
      std::cout 
	     << "\nRuntime test success: " 
		 << std::dec << tests_total
		 << " test(s) were successfull\n";
      return 0;
   } else {	   
      std::cout 
	     << "\nRUNTIME TEST FAILURE: " 
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

void test_constructor(){
   torsor< int > a;
   torsor< int > b;
   TEST_EQUAL( a, b );
   a += 2;
   TEST_NOT_EQUAL( a, b );
   b = b + 2;
   TEST_EQUAL( a, b );
}

int main(){
   test_constructor();

   return test_end();
}