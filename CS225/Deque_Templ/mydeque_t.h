#ifndef DEQUE_H
#define DEQUE_H
#include <fstream>

namespace DigiPen {

    template< typename T>
    class Deque {
        public:
            // big 4
            Deque();                        // note that inside class declaration "Deque" automatically refers to Deque<T>
            Deque( Deque const& rhs );      // copy from exactly same deque only
            Deque& operator=(Deque rhs);    // assign from exactly same deque only
            ~Deque();

            // from array - exactly same type
            Deque( T * array_, unsigned int size_ );

            // simple stuff
            int   Size() const;
            bool  Empty() const;
            void  Clear();
            int   Capacity() const;

            // real stuff
            void  Push_front(T val);
            T     Pop_front();
            void  Push_back(T val);
            T     Pop_back();
            T&    operator[](unsigned int pos) ;
            T     operator[](unsigned int pos) const ;

            // usefull stuff
            void swap( Deque& other ) ;

            // non-standard stuff
            Deque&    operator+=(const Deque& rhs);       // concatenate 2 deques
            Deque     operator+(const Deque& rhs) const;  // concatenate 2 deques
            Deque&    reverse() ;                // reverse in-place
            Deque     operator~() const;                 // return new reverse

            // implemented
            void Print() const;
        private:
            // implemented
            // NOP if new_capacity <= current size
            void reallocate( int new_capacity );
        private:
            int b;    // index where deque's data starts
            int e;    // index where deque's data ends
            int size;
            int capacity; // size of the array
            T * array;  // data array
    };

    // non friend - but has to be declared in the header
    template< typename T>
    std::ostream & operator<<(std::ostream & os, Deque<T> const& d);

}

#include "mydeque_t.cpp"
#endif
