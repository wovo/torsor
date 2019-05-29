// ==========================================================================
//
// test-compiler-message.hpp
//
// torsor compiler error messages tests
//
// This is a man-in-the-loop test:
// enable a test by removing the comment marks,
// compile it using the command 'make build'
// in the root directory, and check that the compiler error
// is reasonable.
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

int main(){
   int _int;	
   torsor< int > _torsor;
   torsor<torsor<int>> _torsor_torsor = _torsor + _torsor;
   
   // can't assign 2 to a torsor
   //_torsor = 2;
   
   // can't assign a torsor from a torsor<torsor<T>>
   //_torsor = _torsor_torsor;
   
   _int = _torsor - _torsor;
   
}