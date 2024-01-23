// //////////////////////////////////////////////////////////////////////////////
#ifndef VECTOR_H
#define VECTOR_H
// //////////////////////////////////////////////////////////////////////////////
namespace CS170  {
  class Vector  {
    public: 
      // Default constructor
      Vector( void );


      // Destructor
      ~Vector(  );

      // Adds a node to the back of the array. If there is
      // not enough room in the array, it will be grown to
      // handle more items.
      void push_back( int value );


      // Adds a node to the front of the array. If there is
      // not enough room in the array, it will be grown to
      // handle more items. All items to the right must be
      // shifted over one element to the right.
      void push_front( int value );


      // Subscript operators for const and non-const
      // int get( unsigned index ) const;
      // int &get( unsigned index );


      int operator[](unsigned index) const;
      int& operator[](unsigned index);

      // Deletes the underlying array and sets size_ to 0
      void clear( void );


      // Return true if the vector is empty, otherwise, false
      bool empty( void ) const;


      // Returns the number of elements in the vector
      unsigned size( void ) const;


      // Returns the size of the underlying array
      unsigned capacity( void ) const;


      // The number of memory allocations that have occurred
      unsigned allocations( void ) const;
    private:
      int *array_;   // The dynamically allocated array
      unsigned size_;                 // The number of elements in the array
      unsigned capacity_;             // The allocated size of the array
      unsigned allocs_;               // Number of allocations (resizes)


      // Makes sure the subscript or position is valid.
      // Aborts the program if it's invalid.
      void check_bounds( unsigned index ) const;


      // Grows the array when necessary. (Twice as large as before.)
      // Sizes of the array: 0, 1, 2, 4, 8, 16, etc.
      // Copies the existing values into the new array. 
      // Deletes the old array.
      void grow( void );


      // Other private methods...
  };
}                            // namespace CS170


#endif // VECTOR_H
