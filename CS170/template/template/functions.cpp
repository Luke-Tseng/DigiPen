#include <iostream>

namespace CS170 {
    template <typename T>
    static void shift(T* begin, T* end) {
        T* temp = begin;
        while (*begin != *end) {
            *begin = *(begin + 1);
            ++begin;
        }
        begin = temp;
    }
    template <typename T>
    void display(T const* begin, T const* end)
    {
        if (begin != end) std::cout << *begin++;
        while (begin < end) {
            std::cout << ", " << *begin;
            ++begin;
        }
        std::cout << std::endl;
    }

    template <typename T>
    void swap(T* left, T* right)
    {
        T temp(*right);
        (*right) = (*left);
        (*left) = temp;
    }

    template <typename T>
    T* min_element(T* begin, T* end)
    {
        T* min = begin;
        while (begin != end) {
            if (*begin < *min) min = begin;
            ++begin;
        }
        return min;
    }

    template <typename T>
    T const* min_element(T const* begin, T const* end)
    {
        T const* min = begin;
        while (begin != end) {
            if (*begin < *min) min = begin;
            ++begin;
        }
        return min;
    }

    /////////////////////////////////////////////////////////////////// 
  // swap 2 ranges
    template <typename T, typename T2>
    void swap_ranges(T* begin, T* end, T2* range) {
        while (begin != end) {
            T temp = *range;
            *range = *begin;
            *begin = temp;
            ++range;
            ++begin;
        }
    }



    /////////////////////////////////////////////////////////////////// 
    // remove specified value from the range
    // function should NOT reallocate the array, so all that remove does
    // is move the elements with the specified value into the end of the range
    // return to client a pointer to the first of the "removed" elements:
    // input: 1 6 3 8 2 3 5 - remove 3
    //        1 6 8 2 5 3 3 - resulting range
    //                  ^
    //                  | return value
    template <typename T>
    T* remove(T* begin, T* end, T value) {
        int count = 0;
        while ((begin + count) != end) {
            if (*begin == value) {
                shift(begin, end);
                ++count;
            }

            if (*begin != value && (begin + count) != end) {
                ++begin;
            }
        }
        end = begin;
        return begin;
    }
    /////////////////////////////////////////////////////////////////// 
    // overwrite all elements in the range with a given value
    template <typename T>
    void fill(T* begin, T* end, T value) {
        while (begin != end) {
            *begin = value;
            ++begin;
        }
    }
    /////////////////////////////////////////////////////////////////// 
    // count elements that are equal to a given value 
    template <typename T>
    T count(T const* begin, T const* end, T value) {
        T count = 0;
        while (begin != end) {
            if(*begin == value) ++count;
            ++begin;
        }
        return count;
    }
    /////////////////////////////////////////////////////////////////// 
    // add all element of the array together
    template <typename T>
    T sum(T const* begin, T const* end) {
        T sum = 0;
        while (begin != end) {
            sum += *begin;
            ++begin;
        }
        return sum;
    }
    /////////////////////////////////////////////////////////////////// 
    // copy all elements from first range into second
    // if first range is longer then second the function will crash (client's fault)
    // if first range is shorter, then some elements in the second range will remain unchanged.
    // Return the pointer into the second range that points to the first element that was NOT 
    // overwritten (equivalently the next after the last that was overwritten):
    // 1 2 3 4 5      - first range
    // 9 9 9 9 9 9 9  - second range
    // 1 2 3 4 5 9 9  - second range after copy
    //           ^
    //           | return value

    template <typename T, typename T2>
    T2* copy(T const* begin, T const* end, T2* range) {
        while (begin != end) {
            *range = *begin;
            ++range;
            ++begin;
        }
        return range;
    }
    /////////////////////////////////////////////////////////////////// 
    // compare 2 ranges, similar to copy you compare each element of the first range to a 
    // corresponding element of the second. 
    // if first range is longer then second the function will crash (client's fault)
    // if first range is shorter, then some elements in the second range will NOT be compared.
    // 1 2 3
    // 1 2    crash
    // ===========
    // 1 2 3
    // 1 5 5 false - not equal
    // ===========
    // 1 2 3
    // 1 2 5 5 false - not equal
    // ===========
    // 1 2 3
    // 1 2 3 5 true - equal
    template <typename T, typename T2>
    bool equal(T* begin, T* end, T2* range) {
        while (begin != end) {
            if (*begin != *range) {
                return false;
            }
            ++begin;
            ++range;
        }
        return true;
    }
    /////////////////////////////////////////////////////////////////// 
    // replace all elements in the range that are equal to old_value with a given value
    template <typename T>
    void replace(T* begin, T* end, T oldValue, T newValue) {
        while (begin != end) {
            if (*begin == oldValue) *begin = newValue;
            ++begin;
        }
    }
    /////////////////////////////////////////////////////////////////// 
    // pointer to the largest element.
    template <typename T>
    T const* max_element(T const* begin, T const* end) {
        T const* max = begin;
        while (begin != end) {
            if (*begin > *max) max = begin;
            ++begin;
        }
        return max;
    }
    template <typename T>
    T* max_element(T* begin, T* end) {
        T* max = begin;
        while (begin != end) {
            if (*begin > *max) max = begin;
            ++begin;
        }
        return max;
    }
    /////////////////////////////////////////////////////////////////// 
    // pointer to the *first* element in the range that is equal to a given value
    template <typename T>
    T* find(T* begin, T* end, T value) {
        while (begin != end) {
            if (*begin == value) {
                return begin;
            }
            ++begin;
        }
        return end;
    }

    template <typename T>
    T const* find(T const* begin, T const* end, T value) {
        while (begin != end) {
            if (*begin == value) {
                return begin;
            }
            ++begin;
        }
        return end;
    }

}
