#include <iostream>
#include <iomanip>      // setw
#include <sstream>      // std::stringstream

#include "wb_tree.h"


// check the whole tree 
void check_weights( TreeNode * pRoot, float alpha ) 
// assume pRoot is not NULL
{
    // counts match?
    int total_count = pRoot->size;

    // balances
    if ( pRoot->left ) {
        if ( static_cast<float>( pRoot->left->size ) > alpha*pRoot->size ) {
            std::cout << "left subtree is too heavy at node " << pRoot->data << std::endl;
        }
        total_count -= pRoot->left->size;
        check_weights( pRoot->left, alpha );
    }
    if ( pRoot->right ) {
        if ( static_cast<float>( pRoot->right->size ) > alpha*pRoot->size ) {
            std::cout << "right subtree is too heavy at node " << pRoot->data << std::endl;
        }

        total_count -= pRoot->right->size;
        check_weights( pRoot->right, alpha );
    }

    if ( total_count != 1 ) {
        std::cout << "count mismatch at node " << pRoot->data << "\n";
    }
}

void print_inorder( TreeNode const* pRoot ) {
    if ( pRoot == nullptr ) { return; }

    print_inorder( pRoot->left );
    std::cout << pRoot->data << std::endl;
    print_inorder( pRoot->right );
}

/* this is a "ASCII-graphical" print
 * recursive function. 
 * Left branch of the tree is at the top !!!
 */
void print_padded( TreeNode const* pRoot, int padding, char edge ) {
    if ( pRoot == nullptr ) { return; }

    print_padded( pRoot->left, padding + 5,'/');
    std::cout << std::setw( padding ) << " " << edge << " " << pRoot->data  << "(" << pRoot->size << ")" << std::endl;
    print_padded( pRoot->right, padding + 5,'\\');
}

void print_dot_null( int data, int size, int nullcount, std::ostream & os )
{
    os << "    null" << nullcount << " [shape=point];\n";
    os << "    \"" << data << "(" << size << ")\" -> null" << nullcount << ";\n";
}

void print_dot_aux( TreeNode const* pRoot, std::ostream & os, int & nullcount )
{
    if (pRoot->left) {
        os << "    \"" << pRoot->data << "(" << pRoot->size << ")\" -> \"" << pRoot->left->data << "(" << pRoot->left->size << ")\";\n";
        print_dot_aux( pRoot->left, os, nullcount );
    } else {
        if (pRoot->right) {
            print_dot_null( pRoot->data, pRoot->size, ++nullcount, os );
        }
    }

    if (pRoot->right) {
        os << "    \"" << pRoot->data << "(" << pRoot->size << ")\" -> \"" << pRoot->right->data << "(" << pRoot->right->size << ")\";\n";
        print_dot_aux( pRoot->right, os, nullcount );
    } else {
        if (pRoot->left) {
            print_dot_null( pRoot->data, pRoot->size, ++nullcount, os );
        }
    }
}

void print_dot( TreeNode const* pRoot, std::ostream & os ) {
    os << "digraph {\n";
    os << "    node [fontname=\"Arial\"];\n";

    if ( !pRoot ) {
        os << "\n";
    } else if ( !pRoot->right && !pRoot->left ) {
        os << "    " << pRoot->data << "\n";
    } else {
        int terminal_node_counter = 0;
        print_dot_aux( pRoot, os, terminal_node_counter );
    }

    os << "}\n";
}



