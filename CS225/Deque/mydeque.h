#ifndef DEQUE_H
#define DEQUE_H
#include <fstream>

namespace DigiPen {

    class Deque {
        public:
            // big 4
            Deque();
            Deque( Deque const& rhs );
            Deque& operator=(Deque rhs);
            ~Deque();

            // from array
            Deque( int * array_, unsigned int size_ );
            
            // simple stuff
            int   Size() const;
            bool  Empty() const;
            void  Clear();
            int   Capacity() const;

            // real stuff
            void  Push_front(int val);
            int   Pop_front();
            void  Push_back(int val);
            int   Pop_back();
            int&  operator[](unsigned int pos) ;
            int   operator[](unsigned int pos) const ;

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
            int * array;  // data array
    };

    // non friend - but has to be declared in the header
    std::ostream & operator<<(std::ostream & os, const Deque &d);

}

#endif
