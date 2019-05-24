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


// ==========================================================================
//
// concepts for protecting the tensor operators
//
// I use the phrase 'can be ... from X' to refer to a right operand X.
//
// ==========================================================================

// doxygen doesn't handle concepts
///@cond INTERNAL

// concept for the torsor copy constructor
template< typename V, typename W >
concept bool torsor_can_be_constructed_from = requires ( W w ) {  
   V( w );
};

// concept for the torsor assignment operator
template< typename V, typename W >
concept bool torsor_can_be_assigned_from = requires ( V & v, W w ) {  
   v = w;
};

// conecpt for the ( torsor + value ) operator
template< typename V, typename W >
concept bool torsor_can_be_added_by_value = requires ( V v, W w ) {  
   ( v + w );
};

// concept for the ( torsor - value ) operator
template< typename V, typename W >
concept bool torsor_can_be_subtracted_by_value = requires ( V v, W w ) {  
   ( v - w );
};

// concept for the ( torsor - torsor ) operator
template< typename V, typename W >
concept bool torsor_can_be_subtracted_by_torsor = requires ( V v, W w ) {  
   ( v - w );
};

// concept for the ( torsor += value ) operator
template< typename V, typename W >
concept bool torsor_can_be_update_added_by_value = requires ( V v, W w ) {  
   ( v += w );
};

// concept for the ( torsor -= value ) operator
template< typename V, typename W >
concept bool torsor_can_be_update_subtracted_by_value = requires ( V v, W w ){
   ( v -= w );
};

// concept for the ( torsor == torsor ) operator
template< typename V, typename W >
concept bool torsors_can_be_compare_equal = requires ( V v, W w ){
   ( v == w );
};

// concept for the ( torsor == torsor ) operator
template< typename V, typename W >
concept bool torsors_can_be_compare_unequal = requires ( V v, W w ){
   ( v != w );
};

// concept for the ( COUT << torsor ) operator
template< typename COUT, typename W >
concept bool torsor_can_be_printed_to = requires( COUT cout, char c, W w ){
   ( cout << c );
   ( cout << w );
};

///@endcond


// ==========================================================================
//
// the tensor template class itself
//
// ==========================================================================

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
/// The base type of a torsor must have a constructor that
/// accepts a (single) 0 argument.
template< typename T >
class torsor final {
private:

   // (only) torsors of (any type) can 
   // - access the value of torsors of any (same or other) type
   // - create non-zero torsors of any (same or other) type
   template<typename> friend class torsor;
   
   // the stored base type value
   T value;

   // create a non-zero torsor value
   constexpr torsor( const T & value ):
      value( value )
   {}

public:

   // =======================================================================
   //
   // constructor and assignment
   //
   // =======================================================================

   /// default torsor constructor
   ///
   /// Create a torsor with the zero (anchor) value.
   constexpr torsor():value( 0 ){}

   /// create from another torsor
   ///
   /// Create a torsor from another torsor, which must have a base type
   /// from which our own base type can be copy-constructed.
   template< typename U >
   ///@cond INTERNAL
   requires torsor_can_be_constructed_from< T, U >
   ///@endcond
   constexpr torsor( const torsor< U > & right ):
      value( right.value )
   {}

   /// assign a torsor from another torsor
   ///
   /// Copy the vaue from another torsor, which must have a base type
   /// that can be assigned to our base type.
   template< typename U >
   ///@cond INTERNAL
   requires torsor_can_be_assigned_from< T, U >
   ///@endcond
   torsor & operator=( const torsor< U > & right ) const {
      value = right.value;
      return *this;
   }

   // =======================================================================
   //
   // adda and subtract
   //
   // =======================================================================

   /// add a torsor and a value
   ///
   /// Add a value to ourselve.
   /// The base types of our torsor and the value must be addable.
   /// The result is a torsor of the type 
   /// and with the value of that addition.
   template< typename U >
   ///@cond INTERNAL
   requires torsor_can_be_added_by_value< T, U >
   ///@endcond
   constexpr auto operator+( const U & right ) const {
      return ::torsor< decltype( value + right ) >( value + right );      
   }
   
   /// subtract a torsor by a value
   ///
   /// Subtract a value from ourselve.
   /// The base types of our torsor and the value must be subtractable.
   /// The result is a torsor of the type 
   /// and with the value of that subtraction.
   template< typename U >
   ///@cond INTERNAL
   requires torsor_can_be_subtracted_by_value< T, U >
   ///@endcond
   constexpr auto operator-( const U & right ) const {
      return ::torsor< decltype( value - right ) >( value - right );     
   }
   
   /// subtract two torsors
   ///
   /// Subtract a torsor from ourselve.
   /// The base types of our torsor and the other torsor 
   /// must be subtractable.
   /// The result is of the type and has the value of that subtraction.
   template< typename U >
   ///@cond INTERNAL
   requires torsor_can_be_subtracted_by_torsor< T, U >
   ///@endcond
   constexpr auto operator-( const torsor< U > & right ) const {
      return value - right.value;      
   }

   /// add a value to into a torsor
   ///
   /// Add a value into ourselve.
   /// The base types of our torsor and the value 
   /// must be accumulate-addable.
   /// The result is a ourselve.
   template< typename U >
   ///@cond INTERNAL
   requires torsor_can_be_update_added_by_value< T, U >
   ///@endcond
   torsor & operator+=( const U & right ){
      value += right;
      return *this;
   }

   /// subtract a value to into a torsor
   ///
   /// Subtract a value into ourselve.
   /// The base types of our torsor and the value 
   /// must be accumulate-addable.
   template< typename U >
   ///@cond INTERNAL
   requires torsor_can_be_update_subtracted_by_value< T, U >
   ///@endcond
   torsor & operator-=( const U & right ){
      value -= right;
      return *this;
   }

   // =======================================================================
   //
   // compare
   //
   // =======================================================================

   /// compare torsors for equality
   ///
   /// Compare two torsors for equality.
   /// The base types of te torsors must support comparing for equality.
   /// The result is te result of that comparison.
   template< typename U >
   ///@cond INTERNAL
   requires torsors_can_be_compare_equal< T, U >
   ///@endcond
   constexpr auto operator==( const torsor< U > & right ) const {
      return value == right.value;
   }

   /// compare torsors for inequality
   ///
   /// Compare two torsors for inequality.
   /// The base types of te torsors must support comparing for inequality.
   /// The result is te result of that comparison.
   template< typename U >
   ///@cond INTERNAL
   requires torsors_can_be_compare_unequal< T, U >
   ///@endcond
   constexpr auto operator!=( const torsor< U > & right ) const {
      return value != right.value;
   }

   // =======================================================================
   //
   // print
   //
   // =======================================================================

   /// print a torsor to a cout-like object
   ///
   /// The torsor value is printed, preceded by a '@'
   /// character.
   /// The left argument must support printing (using operator<<)
   /// of a char and of a base type value.
   template< typename COUT >
   ///@cond INTERNAL
   requires torsor_can_be_printed_to< COUT, T >
   ///@endcond
   friend COUT & operator<<( COUT & cout, const torsor & right ){
      cout << '@';
      cout << right.value;
      return cout;
   }

}; // template class torsor

#endif // ifndef torsor_hpp