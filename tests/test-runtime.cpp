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

#include <string>
#include <sstream>
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
   const char *name,                 // name of the test
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
         << "   " << name << "( " 
         << ta;
      if( tb != 0){         
         std::cout << " , " << tb;
      }         
      std::cout << " )\n"
         << "   left  \"" << ta << "\" = " << a << "\n";
      if( tb != 0 ){   
         std::cout << "   right \"" << tb << "\" = " << b << "\n";
      }         
      std::cout << "\n";
   }
}

#define CHECK_EQUAL( a, b ) \
   test_ok( (a) == (b), "CHECK_EQUAL",  __FILE__, __LINE__, #a, #b, a, b );   

#define CHECK_NOT_EQUAL( a, b ) \
   test_ok( (a) != (b), "CHECK_NOT_EQUAL", __FILE__, __LINE__, #a, #b, a, b );   

#define CHECK_TRUE( a ) \
   test_ok( (a), "CHECK_TRUE", __FILE__, __LINE__, #a, 0, a, 0 );   

#define CHECK_FALSE( a ) \
   test_ok( !(a), "CHECK_TRUE", __FILE__, __LINE__, #a, 0, a, 0 );   

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

   // constructed equal
   CHECK_EQUAL( a, b );
   a += 2;

   // and this is not fake, a real value is maintained
   CHECK_NOT_EQUAL( a, b );
   b += 2;
   CHECK_EQUAL( a, b );
   
   ( b += 2 ) = a;
   CHECK_EQUAL( a, b );
}

void test_addition(){
   torsor< uint8_t > a;
   torsor< uint16_t > b;

   a += 9;
   CHECK_EQUAL( a, b + 9 );
   CHECK_NOT_EQUAL( a, b + 3 );
   
   CHECK_EQUAL( a, 9 + b );
   CHECK_NOT_EQUAL( a, 3 + b );
   
   a += 80;
   CHECK_NOT_EQUAL( a, b );
   b = + a;
   CHECK_EQUAL( a, b );
}

void test_subtraction(){
   torsor< int8_t > a;
   torsor< int16_t > b;

   a -= 9;
   CHECK_EQUAL( a, b - 9 );
   CHECK_NOT_EQUAL( a, b - 3 );
   
   ( b -= 5 ) = a;
   CHECK_EQUAL( a, b );
}

void test_subtraction2(){
   torsor< int8_t > a;
   torsor< int16_t > b;
   
   a += 16;
   b += 50;
   
   CHECK_EQUAL( b - a, 34 );
   CHECK_EQUAL( a - decltype( a )(), 16 );
}

void test_larger(){
   torsor< int16_t > a;
   torsor< int32_t > b;

   CHECK_TRUE(  ( a + 1 ) >  b          );
   CHECK_TRUE(          a > ( b - 1 )   );
   CHECK_FALSE(         a > ( b + 1 )   );
   CHECK_FALSE(         a > b           );
   
   CHECK_TRUE(  ( a + 1 ) >=  a         );
   CHECK_TRUE(          a >= b          );
   CHECK_FALSE( ( a - 1 ) >= b          );
   
}

void test_smaller(){
   torsor< int32_t > a;
   torsor< int16_t > b;

   CHECK_TRUE(          a <  ( b + 1 )  );
   CHECK_TRUE(  ( a - 1 ) <  b          );
   CHECK_FALSE( ( a + 1 ) <  b          );
   CHECK_FALSE(         a <  b          );
   
   CHECK_TRUE(          a <= ( a + 1 )  );
   CHECK_TRUE(          a <= b          );
   CHECK_TRUE(  ( a - 1 ) <= b          );
   
}

void test_print(){
   torsor< int > a;
   std::stringstream s;

   a += 10;
   s << a;
   CHECK_EQUAL( s.str(), "@10" );

   a -= 30;
   s.str( "" );
   s << a;
   CHECK_EQUAL( s.str(), "@-20" );

}

int main(){
   test_constructor();
   test_addition();
   test_subtraction();  
   test_subtraction2();  
   test_larger();  
   test_smaller();  
   test_print();  

   return test_end();
}