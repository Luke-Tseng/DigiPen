#include <iostream> // cout, endl
#include <cstdlib>  // atoi

#include "Vector.h"

void Print(const CS170::Vector& array, bool newline = true)
{
    for (unsigned i = 0; i < array.size(); i++)
    {
        std::cout << array[i] << "  ";
    }

    std::cout << "(size=" << array.size() << ", capacity=" << 
        array.capacity() << ", allocs=" << 
        array.allocations() << ")";
    if (newline)
        std::cout << std::endl;
}

void PrintPartial(const CS170::Vector& array, bool newline = true)
{
    for (unsigned i = 1; i < array.size(); i *= 2)
    {
        std::cout << array[i - 1] << "  ";
    }

    std::cout << "(size=" << array.size() << ", capacity=" << 
        array.capacity() << ", allocs=" << 
        array.allocations() << ")";
    if (newline)
        std::cout << std::endl;
}

void TestSwap1(void)
{
    std::cout << "\n********** TestSwap1 **********\n";
    CS170::Vector a, b, c;

    std::cout << "push_back integers:\n";
    for (int i = 0; i < 10; i++)
        a.push_back(i + 1);
    for (int i = 0; i < 5; i++)
        b.push_back(10 * (i + 1));

    std::cout << "a: ";
    Print(a);
    std::cout << "b: ";
    Print(b);

    std::cout << "swap a,b:\n";
    a.swap(b);
    std::cout << "a: ";
    Print(a);
    std::cout << "b: ";
    Print(b);

    std::cout << "swap a,c:\n";
    a.swap(c);
    std::cout << "a: ";
    Print(a);
    std::cout << "c: ";
    Print(c);

    std::cout << "swap b,b:\n";
    b.swap(b);
    std::cout << "b: ";
    Print(b);
}

void TestReverse1(void)
{
    int count = 10;
    std::cout << "\n********** TestReverse1 **********\n";
    CS170::Vector a;

    std::cout << "push_back integers:\n";
    for (int i = 0; i < count; i++)
        a.push_back(i + 1);

    Print(a);
    a.reverse();
    std::cout << "Reversed:\n";
    Print(a);

    std::cout << "Remove last element:\n";
    a.pop_back();
    Print(a);
    a.reverse();
    std::cout << "Reversed:\n";
    Print(a);
}

void TestReverse2(void)
{
    int count = 10;
    std::cout << "\n********** TestReverse2 **********\n";
    CS170::Vector a;

    std::cout << "push_back integers:\n";
    for (int i = 0; i < count; i++)
        a.push_back(i + 1);

    Print(a);
    a.reverse();
    std::cout << "Reversed:\n";
    Print(a);

    while (!a.empty())
    {
        if (a.size() % 2) // odd
        {
            std::cout << "Remove last element:\n";
            a.pop_back();
        }
        else // even
        {
            std::cout << "Remove first element:\n";
            a.pop_front();
        }
        Print(a);
        a.reverse();
        std::cout << "Reversed:\n";
        Print(a);
    }
}

void TestEqual1(void)
{
    std::cout << "\n********** TestEqual1 **********\n";
    CS170::Vector a, b, c;

    std::cout << "push_back integers:\n";
    for (int i = 0; i < 10; i++)
        a.push_back(i + 1);
    for (int i = 0; i < 10; i++)
        b.push_back(i + 1);

    std::cout << "a: ";
    Print(a);
    std::cout << "b: ";
    Print(b);

    if (a == b)
        std::cout << "a is equal to b\n";
    else
        std::cout << "a is NOT equal to b\n";

    std::cout << "remove last element of a:\n";
    a.pop_back();
    std::cout << "a: ";
    Print(a);
    std::cout << "b: ";
    Print(b);
    if (a == b)
        std::cout << "a is equal to b\n";
    else
        std::cout << "a is NOT equal to b\n";

    std::cout << "remove last element of b:\n";
    b.pop_back();
    std::cout << "a: ";
    Print(a);
    std::cout << "b: ";
    Print(b);
    if (a == b)
        std::cout << "a is equal to b\n";
    else
        std::cout << "a is NOT equal to b\n";

    std::cout << "change last element of b to 100:\n";
    b[b.size() - 1] = 100;
    std::cout << "a: ";
    Print(a);
    std::cout << "b: ";
    Print(b);
    if (a == b)
        std::cout << "a is equal to b\n";
    else
        std::cout << "a is NOT equal to b\n";
}

void TestSwapStress(void)
{
    std::cout << "\n********** TestSwapStress **********\n";
    CS170::Vector a, b, c;

    int count = 1000000;

    std::cout << "Pushing back...\n";
    for (int i = 0; i < count; i++)
    {
        a.push_back(i);
        b.push_back(i * 2);
        c.push_back(i * 3);
    }

    std::cout << "Swapping...\n";
    CS170::Vector x;
    for (int i = 0; i < 10000001; i++)
    {
        a.swap(b);
        b.swap(c);
        c.swap(a);
    }

    PrintPartial(a);
    PrintPartial(b);
    PrintPartial(c);

    std::cout << "Done...\n";
}

void TestShrink1(void)
{
    std::cout << "\n********** TestShrink1 **********\n";
    CS170::Vector a;

    std::cout << "push_back 8 integers:\n";
    for (int i = 0; i < 8; i++)
        a.push_back(i);
    Print(a);

    std::cout << "shrink:\n";
    a.shrink_to_fit();
    Print(a);

    std::cout << "pop last 3:\n";
    a.pop_back();
    a.pop_back();
    a.pop_back();
    Print(a);

    std::cout << "shrink:\n";
    a.shrink_to_fit();
    Print(a);

    std::cout << "push_back one integer:\n";
    a.push_back(100);
    Print(a);

    std::cout << "shrink:\n";
    a.shrink_to_fit();
    Print(a);

    std::cout << "shrink:\n";
    a.shrink_to_fit();
    Print(a);

    std::cout << "clear:\n";
    a.clear();
    Print(a);

    std::cout << "shrink:\n";
    a.shrink_to_fit();
    Print(a);
}

typedef void (*Test)(void);
Test Tests[] = {
    TestSwap1,          //1
    TestReverse1,       //2
    TestReverse2,       //3
    TestEqual1,         //4
    TestShrink1,        //5
    TestSwapStress      //6
};

int main(int argc, char **argv)
{
    int test_num = 0;
    if (argc > 1)
        test_num = std::atoi(argv[1]);

    int num = sizeof(Tests) / sizeof(*Tests);
    if (test_num == 0)
    {
        for (int i = 0; i < num; i++)
            Tests[i]();
    }
    else if (test_num > 0 && test_num <= num)
        Tests[test_num - 1]();

    return 0;
}
