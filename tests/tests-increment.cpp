// ==========================================================================
//
// main.hpp
//
// torsor tests
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
#include <experimental/type_traits>
#include "torsor.hpp"

torsor< int > a;
int b;

struct dummy_multiply {
   template< typename T >
   auto operator * ( const T & x ){
      return x;
   }      
};


template< typename Q >
struct bla {
   
friend int operator ++( torsor< int >, int ){ return 42; }

template< typename T >
requires requires( T x ) { 
   ( a + a ++ ); 
}
bool f(){ return 1; }

template< typename T >
bool f(){ return 0; }

};

int main(){
   std::cout << ( bla< torsor< int > >().f< dummy_multiply >() ) << "\n";   
}

/*
ERROR( a /+ b )
ERROR2( -, a )
ERROR( a + b )
*/

int tests_total = 0;
int tests_failed = 0;

template <class T, class U>
using equality_t = decltype(std::declval<T>() + std::declval<U>());

template <class T, class U>
using supports_equality = std::experimental::is_detected<equality_t, T, U>;

int ymain(){
   std::cout << ( supports_equality< torsor< int >, int >::value ) << "\n";   
}

namespace test {
   auto operator+( const torsor< int > & a, const int & b ){
      return supports_equality< decltype( a ), decltype( b ) >::value;      
   }      
   auto value(){
      return a + b;
   }      
}

int zmain(){
   std::cout << ( test::value() ) << "\n";   
}

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

void test_end(){
   if( tests_failed == 0 ){
      std::cout 
	     << "\nTest success: " 
		 << std::dec << tests_total
		 << " test were successfull\n";
   } else {	   
      std::cout 
	     << "\nTEST FAILURE: " 
		 << std::dec << tests_failed
		 << " test were NOT successfull\n";
   }
}

void test_constructor(){
   torsor< int > a;
   torsor< int > b;
   TEST_EQUAL( a, b );
   a += 2;
   TEST_NOT_EQUAL( a, b );
   b = b + 2;
   TEST_EQUAL( a, b );
}

int xmain(){
   test_constructor();


/*

   int b;
   torsor< float > f;
   int *p;
   struct {} s;
   a + b;
   a + p;
   //a + s;
   a - f;
   std::cout << a + 7;
*/

   test_end();
}