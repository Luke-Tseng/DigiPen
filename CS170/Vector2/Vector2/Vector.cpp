#include <iostream>  // cout
#include <cstdlib>   // abort
#include "Vector.h"

namespace CS170
{

    Vector::Vector(void) : array_(0), size_(0), capacity_(0), allocs_(0)
    {
    }

    Vector::~Vector() {
        delete[] array_;
    }

    // Copy constructor
    Vector::Vector(Vector const& rhs) {
        array_ = rhs.array_;
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;
        allocs_ = rhs.allocs_;
    }

    // Assignment operator
    Vector& Vector::operator=(Vector const& rhs) {
        if (this == &rhs) {
            return* this;
        }
        *this = rhs;
        return *this;
    }

    // Constructor to create a Vector from an array
    Vector::Vector(int const array[], unsigned size) {
        size_ = size;
        while (size_ >= capacity_) {
            grow();
        }
        for (int i = 0; i < (unsigned)size_; ++i) {
            array_[i] = array[i];
        }
    }

    // Adds a node to the back of the array. If there is
    // not enough room in the array, it will be grown to
    // handle more items.
    void Vector::push_back(int value) {
        if (size_ >= capacity_) {
            grow();
        }
        array_[size_] = value;
        ++size_;
    }

    // Adds a node to the front of the array. If there is
    // not enough room in the array, it will be grown to
    // handle more items. All items to the right must be
    // shifted over one element to the right.
    void Vector::push_front(int value) {
        if (size_ >= capacity_) {
            grow();
        }
        for (int i = (int)size_ - 1; i >= 0; --i) {
            array_[i + 1] = array_[i];
        }
        array_[0] = value;
        ++size_;
    }

    // Removes the last element. Does nothing if empty.
    void Vector::pop_back() {
        if (empty()) {
            return;
        }
        array_[size_] = 0;
        size_--;
    }

    // Removes the first element. Does nothing if empty.
    void Vector::pop_front() {
        if (empty()) {
            return;
        }
        for (int i = 0; i < (signed) size_-1; ++i) {
            array_[i] =  array_[i+1];
        }
        size_--;
    }

    // Inserts a new node at the specified position. Causes an
    // abort() if the position is invalid. (Calls check_bounds)
    void Vector::insert(int value, unsigned position) {
        check_bounds(position);
        if (size_ >= capacity_) {
            grow();
        }
        for (int i = size_; i > (signed) position; --i) {
            array_[i] = array_[i-1];
        }
        array_[position] = value;
        size_++;
    }

    // Removes an element with the specified value (first occurrence)
    void Vector::remove(int value) {
        for (int i = 0;i < (int)size_; ++i) {
            if (array_[i] == value) {
                for (int j = i; j < (int)size_; ++j) {
                    array_[j] = array_[j + 1];
                }
                size_--;
                return;
            }
        }
    }

    int Vector::operator[](unsigned index) const {
        check_bounds(index);
        return array_[index];
    }

    int& Vector::operator[](unsigned index) {
        check_bounds(index);
        return array_[index];
    }

    // Deletes the underlying array and sets size_ to 0
    void Vector::clear(void) {
        delete[] array_;
        capacity_ = 0;
        size_ = 0;
        array_ = 0;
    }

    bool Vector::empty(void) const
    {
        return size_ == 0;
    }

    // Concatenates a vector onto the end of this vector.
    Vector& Vector::operator+=(const Vector& rhs) {
        for (int i = 0; i < (signed) rhs.size_; ++i) {
            push_back(rhs[i]);
        }
        return *this;
    }

    // Concatenates two Vectors.
    Vector Vector::operator+(const Vector& rhs) const {
        Vector v;
        for (int i = 0; i < (signed) size_; ++i) {
            v[i] = array_[i];
        }
        for (int i = 0; i < (signed) rhs.size_ ; ++i) {
            v[i+size_] = rhs.array_[i];
        }
        return v;
    }


    unsigned Vector::size(void) const
    {
        return size_;
    }

    unsigned Vector::capacity(void) const
    {
        return capacity_;
    }

    unsigned Vector::allocations(void) const
    {
        return allocs_;
    }

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    // private 
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    void Vector::check_bounds(unsigned index) const
    {
        // Don't have to check for < 0 because index is unsigned
        if (index >= size_)
        {
            std::cout << "Attempting to access index " << index << ".";
            std::cout << " The size of the array is " << size_ << ". Aborting...\n";
            std::abort();
        }
    }

    // Grows the array when necessary. (Twice as large as before.)
    // Sizes of the array: 0, 1, 2, 4, 8, 16, etc.
    // Copies the existing values into the new array. 
    // Deletes the old array.
    void Vector::grow(void)
    {
        // Double the capacity ( or set to 1 if it was zero)
        capacity_ = (capacity_) ? capacity_ * 2 : 1;
        int* tempArray = new int[Vector::capacity_]();
        for (int i = 0; i < (int)size_; ++i) {
            tempArray[i] = array_[i];
        }
        delete[] array_;
        array_ = tempArray;
        ++allocs_;
    }

} // namespace CS170
