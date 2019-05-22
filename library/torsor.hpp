// ==========================================================================
//
// torsor.hpp
//
// a C++ one-header one-class library for expressing and enforcing
// the difference between relative and absolute (anchored) values
//
// https://www.github.com/wovo/torsor
// 
// Copyright Wouter van Ooijen - 2019
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

#ifndef torsor_hpp
#define torsor_hpp

// this file contains Doxygen lines
/// @file

/// absolute version of an arithmetic value type
///
/// For a value type that denotes a ratio scale value (a value
/// for which addition yields a value on the same scale), the
/// torsor of that (base) type is the corresponding interval scale 
/// (anchored) type.
///
/// The operations on the torsor are limited to: 
/// - adding or subtracting a base type value (yields a torsor value)
/// - subtracting two torsors (yields a base type value)
/// - printing a torsor (prints @ followed by its base type value)
/// 
/// A the base type of a torsor must have a constructor that
/// accepts a (single) 0 argument.
template< typename T >
class torsor {
private:

   T value;
   
public:

   torsor():value( 0 ){}
   
   template< typename U >
   constexpr torsor operator+( const U & right ) const {
      return torsor( value + right );      
   }
   
   template< typename U >
   constexpr torsor operator-( const U & right ) const {
      return torsor( value - right );      
   }
   
   template< typename U >
   constexpr auto operator-( const torsor< U > & right ) const {
      return value - right;      
   }
   

}; // class torsor

#endif // ifndef torsor_hpp