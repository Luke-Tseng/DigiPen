#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <tuple>
#include <map>
#include <deque>
#include <set>
#include <algorithm>

class TS {
    public:
        using LHStype = std::tuple< std::string, char >;        // <current state> <current symbol>
        using RHStype = std::tuple< char, int, std::string >;   // <new symbol> <direction> <new state>
    public:
        TS( std::string const& filename ) : rules(), initial_state() 
        {
            // read file 
            // initial state
            // <current state> <current symbol> <new symbol> <direction> <new state>
            // check for duplicates

            std::ifstream infile( filename.c_str() ); // closed automatically
            if ( infile.fail() ) {
                std::cout << "Cannon open " << filename << std::endl;
                throw "Cannot open input file";
            }

            std::string line;
            // lines from file
            while ( std::getline(infile, line) ) {
                removeComments( line ); // not really needed

                if ( line.substr( 0,14 ) == "initial state " ) { // initial_state
                    // delete prefix
                    line = line.substr( 14 ); // 14-end
                    std::istringstream iss( line );
                    iss >> initial_state;
                } else { // rule
                    std::string current_state,  new_state;
                    char        current_symbol, new_symbol;
                    char dir_ch;
                    int  dir;
                    std::istringstream iss( line );
                    iss >> current_state >> current_symbol >> new_symbol >> dir_ch >> new_state;
                    if ( iss.fail() ) {
                        iss.clear();
                    } else {
                        switch ( dir_ch ) {
                            case 'l': 
                            case 'L': 
                                dir = -1; break;
                            case 'r': 
                            case 'R': 
                                dir = 1; break;
                            case '*': 
                                dir = 0; break;
                            default: 
                                throw "direction symbol should be l,L,r,R,*"; break;
                        }


                        std::tuple< std::string, char >      st_sy      = std::make_tuple( current_state, current_symbol );
                        std::tuple< char, int, std::string > sy_d_st    = std::make_tuple( new_symbol, dir, new_state );

                        // check for duplicates
                        if ( rules.find( st_sy ) != rules.end() ) {
                            throw "duplicate rule";
                        } else { // insert rule
                            rules[ st_sy ] = sy_d_st; // new rule
                        }
                    }
                }
            }

        }
        ////////////////////////////////////////////////////////////////////////
        std::string rule2string( std::map< LHStype, RHStype >::const_iterator it_r ) const 
        {
            LHStype lhs    = it_r->first;
            RHStype rhs    = it_r->second;

            std::ostringstream oss;
            oss << std::get<0>( lhs ) << " " << std::get<1>( lhs ) 
                << " " << std::get<0>( rhs ) << " " << std::get<2>( rhs );
            switch ( std::get<1>( rhs ) ) {
                case -1: oss << " left\n"; break;
                case  1: oss << " right\n"; break;
                case  0: oss << " stay\n"; break;
                default: oss << " ERROR\n"; break;
            }
            return oss.str();
        }


        ////////////////////////////////////////////////////////////////////////
        template< typename CallBack > 
        CallBack run( std::string const& init_tape, CallBack cb )
        {
            int                 caret { 0 };
            std::string         state { initial_state };
            std::deque< char >  tape  { init_tape.begin(), init_tape.end() };
            if ( tape.size() == 0 ) {
                tape.push_back( '_' ); // something has to be under caret
            }

            // spaces to underscores
            std::replace( tape.begin(), tape.end(), ' ', '_' );

            cb.Start( tape, caret, state );

            bool halt = false;
            do {
                std::tuple< std::string, char > matches[3] 
                    = { std::make_tuple( state, tape[caret] ),  // exact match 
                        std::make_tuple( "*", tape[caret] ),    // state  ='*'
                        std::make_tuple( state, '*' )           // symbol = '*'
                    };

                int c = 0; // how many rules match the (state,symbol) pair
                std::map< LHStype, RHStype >::const_iterator it_rule_chosen;
                for ( auto const& m : matches ) {
                    std::map< LHStype, RHStype >::const_iterator it_rule = rules.find( m );
                    if ( it_rule != rules.end() ) {
                        ++c;
                        it_rule_chosen = it_rule;
                    }
                }

                // no or multiple matches
                if ( c == 0 ) {
                    std::cout << "no rule matches: " << state << " " << tape[caret] << std::endl;
                    throw "no rule matches";
                }
                if ( c > 1  ) {
                    std::cout << "multiple rules match: " << state << " " << tape[caret] << std::endl;
                    throw "multiple rules match";
                }

                // dealing with a single matching rule
                RHStype const& sy_d_st = it_rule_chosen->second;

                char        new_symbol = std::get<0>( sy_d_st );
                int         dir        = std::get<1>( sy_d_st );
                std::string new_state  = std::get<2>( sy_d_st );
                
                // special cases with wildcards
                if ( new_symbol == '*' ) { // keep old char
                    new_symbol = tape[caret];
                }
                if ( new_state == "*" ) { // keep old state
                    new_state = state;
                }

                // update 
                state = new_state;
                tape[caret] = new_symbol;
                caret += dir;

                // extend tape ?
                if ( caret == -1 ) {
                    tape.push_front( '_' );
                    caret = 0;
                } else if ( caret == static_cast<int>( tape.size() ) ) {
                    tape.push_back( '_' );
                }

                // halt?
                if ( new_state == "halt" or new_state == "halt-accept" ) {
                    halt = true;
                }
                
                if ( not cb.Step( tape, caret, state, rule2string( it_rule_chosen ) ) ) {
                    throw "manually terminated";
                }
            } while ( not halt );

            // trim final deque
            tape.erase( // delete leading '_' 
                    tape.begin(),
                    std::find_if( tape.begin(), tape.end(), []( char ch ) { return ch != '_'; } )
                    );
//            tape.erase( // delete '_' in the end
//                    tape.rbegin(),
//                    std::find_if( tape.rbegin(), tape.rend(), []( char ch ) { return ch != '_'; } )
//                    );
            // erase does not like reverse iterators -- (note to self - why????), so here is some ugliness:
            tape.erase( // delete '_' in the end
                    tape.end() - ( std::find_if( tape.rbegin(), tape.rend(), []( char ch ) { return ch != '_'; } ) - tape.rbegin() ), 
                    tape.end()
                    );
            cb.Final( tape, state );
            return cb;
        }
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
    private: // data
        std::map< LHStype, RHStype > rules;
        std::string initial_state;
    private: // methods
        void removeComments( std::string & line ) const {
            line.erase( 
                    std::find( line.begin(), line.end(), ';' ),
                    line.end()
                    );
        }
        ////////////////////////////////////////////////////////////////////////
        bool hasSpaces( std::string const& line ) const {
                    return std::find( line.begin(), line.end(), ' ' ) != line.end();
        }
        ////////////////////////////////////////////////////////////////////////
        void print_tape( std::deque< char > const& tape, int caret, std::string const& s ) const
        {
            for( char const& ch : tape ) {
                std::cout << ch << " ";
            }
            std::cout << std::endl;
            std::cout << std::setw( 2*caret+3 ) << "^ (" << s << ")" << std::endl;
        }
        ////////////////////////////////////////////////////////////////////////
};
