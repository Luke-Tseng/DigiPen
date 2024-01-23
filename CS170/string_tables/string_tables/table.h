#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <fstream>
#include <iostream>

namespace CS170 {
    class Table {
        // ...
        public:
        Table(int Rows_, int Cols_);
        Table(const Table& rhs);
        ~Table();
        // ...
        void Reverse(int row, int col);
        void Reverse(int row);
        void Reverse(void);
        Table operator=(const Table& rhs);
        Table operator+(const Table& rhs);
        Table operator*(const Table& rhs);
        // getter and setter (overloaded similar to operator[] from notes)
        std::string       & operator () ( unsigned int r, unsigned int c );
        std::string const & operator () ( unsigned int r, unsigned int c ) const;

        // ...

        void Display( std::ostream & os ) const;    // provided
        void Display2( std::ostream & os ) const;   // fancy    (10% of your grade)
        private:
            int numRows_;
            int numCols_;
            std::string** ppData_;
            Table AllocateTable(int Rows_, int Cols_);
    };

    std::ostream & operator<<( std::ostream & os, Table const& t );
}
#endif
