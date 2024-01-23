////////////////////////////////////////////////////////////////////////////////
#ifndef VECTOR_H
#define VECTOR_H
////////////////////////////////////////////////////////////////////////////////

namespace CS170
{

  class Vector
  {
    public:

      // Default constructor
      Vector(void);

      // Destructor
      ~Vector();

      // Adds a node to the back of the array. If there is
      // not enough room in the array, it will be grown to
      // handle more items.
      void push_back(int value);

      // Adds a node to the front of the array. If there is
      // not enough room in the array, it will be grown to
      // handle more items. All items to the right must be
      // shifted over one element to the right.
      void push_front(int value);

      // Subscript operators for const and non-const
      int operator[](unsigned index) const;
      int& operator[](unsigned index);

      // Deletes the underlying array and sets size_ to 0
      void clear(void);

      // Return true if the vector is empty, otherwise, false
      bool empty(void) const;

      // Returns the number of elements in the vector.
      unsigned size(void) const;

      // Returns the size of the underlying array
      unsigned capacity(void) const;

      // The number of memory allocations that have occurred
      unsigned allocations(void) const;

      // Copy constructor
      Vector(const Vector& rhs);

      // Constructor to create a Vector from an array
      Vector(const int array[], unsigned size);

      // Removes the last element. Does nothing if empty.
      void pop_back(void);

      // Removes the first element. Does nothing if empty.
      void pop_front(void);

      // Inserts a new node at the specified position. 
      void insert(int value, unsigned position);

      // Removes an element with the specified value (first occurrence)
      void remove(int value);

      // Assignment operator
      Vector& operator=(const Vector& rhs);

      // Concatenates a vector onto the end of this vector.
      Vector& operator+=(const Vector& rhs);

      // Concatenates two Vectors.
      Vector operator+(const Vector& rhs) const;

      // Reverses the order of the elements (Linear time)
      void reverse(void);

      // Swaps the contents of other with this Vector (Constant time)
      void swap(Vector &other);

      // Equality operator (Linear time)
      // Both vectors must be the same size.
      bool operator==(const Vector& rhs) const;

      // Reallocates an array to match the size of the number
      // of elements. If size == capacity, does nothing.
      void shrink_to_fit(void);

    private:
      int *array_;        // The dynamically allocated array
      unsigned size_;     // The number of elements in the array
      unsigned capacity_; // The allocated size of the array
      unsigned allocs_;   // Number of allocations (resizes)

      // Private methods...
      void grow(void);

      void swap_pointers( int *& p1, int *& p2 ) 
      {
        int * temp = p1;
        p1 = p2;
        p2 = temp;
      }
      void swap_ints( int& i1, int& i2 ) 
      {
        int temp = i1;
        i1 = i2;
        i2 = temp;
      }
      void swap_unsigned( unsigned& i1, unsigned& i2 ) 
      {
        unsigned temp = i1;
        i1 = i2;
        i2 = temp;
      }


      // Other private methods...
  };

} // namespace CS170

#endif // VECTOR_H
