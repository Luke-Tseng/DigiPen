#include "table.h"
#include <iomanip>
#include <algorithm>
#include <string>

namespace CS170 {

    Table::Table(int Rows_, int Cols_) {
        numRows_ = Rows_;
        numCols_ = Cols_;
        ppData_ = new std::string* [Rows_]; // pointers to rows
        for (int r = 0; r < Rows_; ++r) {
            ppData_[r] = new std::string[Cols_];  // rows with data
        }
    }

    Table::Table(const Table& rhs) {
        numRows_ = rhs.numRows_;
        numCols_ = rhs.numCols_;
        ppData_ = new std::string * [numRows_]; // pointers to rows
        for (int r = 0; r < numRows_; ++r) {
            ppData_[r] = new std::string[numCols_];  // rows with data
        }
        for (int i = 0; i < numRows_; ++i) {
            for (int j = 0; j < numCols_; ++j) {
                ppData_[i][j] = rhs.ppData_[i][j];
            }
        }
    }

    Table::~Table() {
        for (int r = 0; r < numRows_; ++r) {
            delete[] ppData_[r];
        }
        delete[] ppData_;
    }

    void Table::Reverse(int row, int col) {
        reverse(ppData_[row][col].begin(), ppData_[row][col].end());
    }

    void Table::Reverse(int row) {
        for (int i = 0; i < numCols_/2; ++i) {
            std::string temp = ppData_[row][i];
            ppData_[row][i] = ppData_[row][numCols_ - 1 - i];
            ppData_[row][numCols_ - 1 - i] = temp;
        }
    }

    void Table::Reverse(void) {
        Table temp = *this;

        for (int r = 0; r < numRows_; ++r) {
            delete[] ppData_[r];
        }
        delete[] ppData_;
        ppData_ = new std::string * [numCols_]; // pointers to rows
        for (int r = 0; r < numCols_; ++r) {
            ppData_[r] = new std::string[numRows_]();  // rows with data
        }
        for (int i = 0; i < numCols_; ++i) {
            for (int j = 0; j < numRows_; ++j) {
                ppData_[i][j] = temp.ppData_[j][i];
            }
        }
        int tempNum = numCols_;
        numCols_ = numRows_;
        numRows_ = tempNum;
    }

    Table Table::operator=(const Table& rhs) {
        if (this == &rhs) {
            return *this;
        }
        for (int r = 0; r < numRows_; ++r) {
            delete[] ppData_[r];
        }
        delete[] ppData_;
        numRows_ = rhs.numRows_;
        numCols_ = rhs.numCols_;
        ppData_ = new std::string * [numRows_]; // pointers to rows
        for (int r = 0; r < numRows_; ++r) {
            ppData_[r] = new std::string[numCols_];  // rows with data
        }
        for (int i = 0; i < numRows_; ++i) {
            for (int j = 0; j < numCols_; ++j) {
                ppData_[i][j] = rhs.ppData_[i][j];
            }
        }
        return *this;
    }

    Table Table::operator+(const Table& rhs) {
        int maxR = std::max(numRows_, rhs.numRows_);
        int maxC = std::max(numCols_, rhs.numCols_);
        Table temp = AllocateTable(maxR, maxC);
        for (int i = 0; i < maxR; ++i) {
            for (int j = 0; j < maxC; ++j) {
                if (i < numRows_ && j < numCols_) {
                    temp.ppData_[i][j] += ppData_[i][j];
                }
                if (i < rhs.numRows_ && j < rhs.numCols_) {
                    temp.ppData_[i][j] += rhs.ppData_[i][j];
                }
            }
        }
        return temp;
    }

    Table Table::operator*(const Table& rhs) {
        int maxR = numRows_ * numRows_;
        int maxC = numCols_ * rhs.numCols_;
        Table temp = AllocateTable(maxR, maxC);

        for (int r1 = 0; r1 < numRows_; ++r1) {
            for (int c1 = 0; c1 < numCols_; ++c1) {
                for (int r2 = 0; r2 < rhs.numRows_; ++r2) {
                    for (int c2 = 0; c2 < rhs.numCols_; ++c2) {
                        int r = r1 * rhs.numRows_ + r2;
                        int c = c1 * rhs.numCols_ + c2;
                        temp.ppData_[r][c] = ppData_[r1][c1] + rhs.ppData_[r2][c2];
                    }
                }
            }
        }
        return temp;
    }

    std::string& Table::operator() (unsigned int r, unsigned int c) {
        return ppData_[r][c];
    }

    std::string const& Table::operator() (unsigned int r, unsigned int c) const {
        return ppData_[r][c];
    }

    void Table::Display(std::ostream& os) const
    {
        for (int r = 0; r < numRows_; ++r) {
            for (int c = 0; c < numCols_; ++c) {
                os << ppData_[r][c] << " ";
            }
            os << std::endl;
        }
    }

    void Table::Display2(std::ostream& os) const
    {
        // ... your code, used setw, setfill, etc 
        int largest = 0;
        for (int r = 0; r < numRows_; ++r) {
            for (int c = 0; c < numCols_; ++c) {
                largest = 0;
                for (int i = 0; i < numRows_; ++i) {
                    int temp = (int)ppData_[i][c].length();
                    if (temp > largest) {
                        largest = temp;
                    }
                }
                largest += 3;
                os << "+" << std::setfill('-') << std::setw(largest);
            }
            os << "+" << std::endl;
            for (int c2 = 0; c2 < numCols_; ++c2) {
                largest = 0;
                for (int i = 0; i < numRows_; ++i) {
                    int temp = (int) ppData_[i][c2].length();
                    if (temp > largest) {
                        largest = temp;
                    }
                }
                os << "| ";
                for(int sub = 0; sub < largest - (int) ppData_[r][c2].length();++sub) {
                    os << " ";
                }
                os << ppData_[r][c2] << " ";
            }
            os << "|" << std::endl;
        }
        for (int c = 0; c < numCols_; ++c) {
            largest = 0;
            for (int i = 0; i < numRows_; ++i) {
                int temp = (int) ppData_[i][c].length();
                if (temp > largest) {
                    largest = temp;
                }
            }
            largest += 3;
            os << "+" << std::setfill('-') << std::setw(largest);
        }
        os << "+" << std::endl;
    }

    std::ostream& operator<<(std::ostream& os, Table const& t)
    {
        //t.Display2(os);   // fancy (see fancy_out* )
        t.Display( os );   // simple (see out* )

        return os;
    }

    Table Table::AllocateTable(int Rows_, int Cols_) {
        Table temp(Rows_, Cols_);
        return temp;
    }
}