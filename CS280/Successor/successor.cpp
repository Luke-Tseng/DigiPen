#include "successor.h"

Node *successor(Node *pRoot, INT key)
{
    if (pRoot->key == key)
    {
        if (pRoot->right)
        {
            Node *curr = pRoot->right;
            while (curr->left)
            {
                curr = curr->left;
            }
            return curr;
        }
        else
        {
            Node *curr = pRoot;
            Node *parent = curr->parent;
            while (parent && parent->right == curr)
            {
                curr = parent;
                parent = curr->parent;
            }
            return parent;
        }
    }

    if (pRoot->key > key)
    {
        return successor(pRoot->left, key);
    }
    else
    {
        return successor(pRoot->right, key);
    }
}

Node2 *successor2(Node2 *pRoot, INT key)
{
    Node2 *node = pRoot;
    Node2 *stack[100];
    int top = -1;

    if (node == NULL)
    {
        return NULL;
    }

    while (node != NULL && node->key != key)
    {
        stack[++top] = node;
        if (node->key > key)
        {
            node = node->left;
        }
        else
        {
            node = node->right;
        }
    }
    
    if (node->right)
    {
        node = node->right;
        while (node->left)
        {
            node = node->left;
        }
        return node;
    }
    else
    {
        while (top >= 0 && stack[top]->right == node)
        {
            node = stack[top--];
        }
        if (top < 0)
        {
            return NULL;
        }
        else
        {
            return stack[top];
        }
    }
}
