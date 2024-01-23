#ifndef RANGELIST_H
#define RANGELIST_H

#include <cstddef>   /* NULL */
#include <ostream>

// forward declaration
class NodeProxy; 

class RangeList {
  public:
    RangeList();
    RangeList(const RangeList& rhs);
    ~RangeList();
    RangeList& operator=(const RangeList& rhs);
    // NodeProxy &operator=(int rhs);
    // NodeProxy& operator[]( int pos );
    NodeProxy operator[]( int pos );
    int operator*( const RangeList& rhs);
    RangeList operator+( const RangeList& rhs);

    int get( int position ) const;
    int insert_value( int position, int value );
    int insert_nonoverlapping_range( int begin, int end, int value );
    void printf_elements( char const * format, int dimension, bool do_sanity_check, bool indices = true ) const; 
    void print_raw( ) const;
    struct RangeNode* operator[](unsigned int pos) const ;
    friend class NodeProxy;
  private:
    struct RangeNode {
      int begin;
      int end;
      int value;
      struct RangeNode * next;
      void dump_node ( ) const;
      RangeNode( int b, int e, int v ) : begin(b), end(e), value(v), next(NULL) {}
    };
    RangeNode * pHead;
  private: // helpers
    struct RangeNode *createNode(int position, int value, struct RangeNode* next);
};

class NodeProxy {
  public:
    NodeProxy(RangeList& parent, int pos) : _parent(parent), _pos(pos) {}
    NodeProxy& operator=(int value)
    {
      _parent.insert_value(_pos, value);
      return *this;
    }

    operator int()
    {
      return _parent.get(_pos);
    }

    NodeProxy& operator+=( const NodeProxy& rhs)
    {
      int value = _parent.get(_pos);
      _parent.insert_value(_pos, value + rhs._parent.get(rhs._pos));
      return *this;
    }

    NodeProxy& operator+=( int val )
    {
      int value = _parent.get(_pos);
      _parent.insert_value(_pos, value + val);
      return *this;
    }

    NodeProxy& operator*=( int val )
    {
      int value = _parent.get(_pos);
      _parent.insert_value(_pos, value * val);
      return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const NodeProxy& p)
    {
      os << p._parent.get(p._pos);
      return os;
    }
  private:
    RangeList& _parent;
    int _pos;
};
#endif
