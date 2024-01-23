#include <iostream>
#include "turing-sim.h"

struct FinalTape { // Callback with minimum info - for grading
    int max_steps;
    int steps = 0;
    FinalTape( int m = 1000 ) : max_steps(m) {} // max_steps = watchdog 
    //                       tape          caret  state               rule
    void Start  ( std::deque<char> const&, int,   std::string const&                      ) {
    }
    bool Step   ( std::deque<char> const&, int,   std::string const&, std::string const&  ) {
        bool go = ( ++steps < max_steps );
#if 0
        if ( !go ) { std::cout << "Stop after " << steps << std::endl; }
        return true;
#else
        return (go); // maximum steps
#endif
    }
    void Final  ( std::deque<char> const& tape,   std::string const&                      ) {
        std::cout << "Final: " << std::endl;
        PrintTape( tape );
    }
    protected: // helper methods for future subclasses
    void PrintTape( std::deque<char> const& tape ) {
        std::for_each( tape.begin(), tape.end(), []( char ch ) { std::cout << ch << " "; } );
        std::cout << std::endl;
    }
    void PrintState( std::string state ) {
        if ( state == "halt-accept" ) {
            state = "halt";
        }
        std::cout << state << std::endl; 
    }
    void PrintCaret( int caret, std::string const& state ) {
        std::cout << std::setw( 2*caret+3 ) << "^ (" << state << ")" << std::endl;
    }
};
////////////////////////////////////////////////////////////////////////////
struct FinalTapeState : public FinalTape { // Callback for grading WITH final state (has to be halt or halt-accept)
    //                       tape               caret        state                    rule
    void Start  ( std::deque<char> const&,      int,         std::string const&                           ) {}
    bool Step   ( std::deque<char> const&,      int,         std::string const&,      std::string const&  ) { 
        return true;
    }
    void Final  ( std::deque<char> const& tape,              std::string const& state                     ) {
        std::cout << "Final: " << std::endl;
        PrintTape( tape );
        PrintState( state );
    }
};
////////////////////////////////////////////////////////////////////////////
struct StartFinal : public FinalTapeState { // start and final tapes
    //                       tape               caret        state                    rule
    void Start  ( std::deque<char> const& tape, int caret,   std::string const& state      ) {
        std::cout << "Start: " << std::endl;
        PrintTape( tape );
        PrintCaret( caret, state );
        std::cout << std::endl;
    }
};
////////////////////////////////////////////////////////////////////////////
struct AllSteps : public StartFinal { // debugging: show all steps all states
    int max_steps;
    int steps = 0;
    AllSteps( int m = 1000 ) : max_steps(m) {}
    bool Step   ( std::deque<char> const& tape, int caret, std::string const& state, std::string const& rule ) {
        std::cout << "After using rule: " << rule;
        PrintTape( tape );
        PrintCaret( caret, state );
        std::cout << std::endl;
        return (++steps < max_steps); // maximum steps
    }
};

// examples - not used for grading
void test0() try
{
    std::cout << "================================================\n";
    TS ts_unary_add1( "unary_add.tm" );
    int num_steps = ts_unary_add1.run( "1111+111111=", FinalTape() ).steps; // just the tape
    std::cout << "Number of steps " << num_steps << std::endl;
    std::cout << "================================================\n";
    TS ts_unary_add2( "unary_add.tm" );
    num_steps = ts_unary_add2.run( "1111+111111=", FinalTapeState() ).steps; // tape and final state
    std::cout << "================================================\n";
    TS ts_unary_add3( "unary_add.tm" );
    num_steps = ts_unary_add3.run( "1111+111111=", AllSteps(1000) ).steps; // all steps - for debugging
    std::cout << "Number of steps " << num_steps << std::endl;
}
catch( char const * str ) {
    std::cerr << str << std::endl;
}

// change this to AllSteps for debugging
using CallBackType = FinalTape;

void test1() try
{
    TS ts( "ab2xyz.tm" );

    ts.run( "AB", CallBackType() );
}
catch( char const * str ) {
    std::cerr << str << std::endl;
}

void test2() try
{
    TS ts( "ab2xyz.tm" );
    ts.run( "AAAAB", CallBackType() );
}
catch( char const * str ) {
    std::cerr << str << std::endl;
}

void test3() try
{
    TS ts( "ab2xyz.tm" );
    ts.run( "BBBAB", CallBackType() ); 
}
catch( char const * str ) {
    std::cerr << str << std::endl;
}

void test4() try
{
    TS ts( "ab2xyz.tm" );
    ts.run( "ABAAA", CallBackType() );
}
catch( char const * str ) {
    std::cerr << str << std::endl;
}

void test5() try
{
    TS ts( "ab2xyz.tm" );
    ts.run( "ABBBB", CallBackType() );
}
catch( char const * str ) {
    std::cerr << str << std::endl;
}

void test6() try
{
    TS ts( "ab2xyz.tm" );
    ts.run( "AAABAA", CallBackType() );
}
catch( char const * str ) {
    std::cerr << str << std::endl;
}

void test7() try
{
    TS ts( "ab2xyz.tm" );
    ts.run( "AAABBB", CallBackType() );
}
catch( char const * str ) {
    std::cerr << str << std::endl;
}

void test8() try
{
    TS ts( "ab2xyz.tm" );
    ts.run( "BBABAA", CallBackType() );
}
catch( char const * str ) {
    std::cerr << str << std::endl;
}

void test9() try
{
    TS ts( "ab2xyz.tm" );
    ts.run( "BBABBB", CallBackType() );
}
catch( char const * str ) {
    std::cerr << str << std::endl;
}

void test10() try
{
    TS ts( "ab2xyz.tm" );
    ts.run( "ABABAAABBBAB", CallBackType() );
}
catch( char const * str ) {
    std::cerr << str << std::endl;
}

void test11() try
{
    TS ts( "ab2xyz.tm" );
    ts.run( "A", CallBackType() ); 
}
catch( char const * str ) {
    std::cerr << str << std::endl;
}

void test12() try
{
    TS ts( "ab2xyz.tm" );
    ts.run( "ABA", FinalTapeState() ); 
}
catch( char const * str ) {
    std::cerr << str << std::endl;
}

void (*pTests[])( ) = { 
    test0,test1,test2,test3,test4,
    test5,test6,test7,test8,test9,
    test10,test11,test12
};


#include <cstdio>
int main(int argc, char ** argv) {
    if ( argc == 2 ) { // if 1 argument is provided, assume it is test number
        int test = 0;
        std::sscanf(argv[1],"%i",&test);
        pTests[test]();
    } else {
        std::cerr << "Usage ./" << argv[0] << " <test number>\n";
    }
}
