// ==========================================================================
//
// test-runtime.hpp
//
// torsor compiler error messages tests
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


void test_constructor(){
   torsor< int > a;
   torsor< int > b;
   
   // created equal
   TEST_EQUAL( a, b );
   
   // equality is not accidental
   a += 2;
   TEST_NOT_EQUAL( a, b );
   b += 2;
   TEST_EQUAL( a, b );
}

void test_add(){
   torsor< int > a;
   torsor< int > b;
   
   a += 2;
   TEST_EQUAL( a, b + 2 );
   TEST_EQUAL( a, 2 + b );
   
   a = 2;
}

int main(){
   test_constructor();
   test_add();

   return test_end();
}