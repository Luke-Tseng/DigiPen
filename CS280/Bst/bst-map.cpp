#include <iostream>
#include <stack>

// static data members
template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator
	CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::end_it = CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator(nullptr);

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const
	CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::const_end_it = CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const(nullptr);

// Node functions

template <typename KEY_TYPE, typename VALUE_TYPE>
CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node::Node(KEY_TYPE k, VALUE_TYPE val, Node *p, int h, int b, Node *l, Node *r) : key(k), value(val), height(h), balance(b),
																													   parent(p),
																													   left(l), right(r)
{
}

template <typename KEY_TYPE, typename VALUE_TYPE>
KEY_TYPE const &CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node::Key() const
{
	return key;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
VALUE_TYPE &CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node::Value()
{
	return value;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node *CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node::first()
{
	Node *node = this;
	while (node && node->left != nullptr)
		node = node->left;
	return node;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node *CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node::last()
{
	Node *node = this;
	while (node && node->right != nullptr)
		node = node->right;
	return node;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node *CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node::increment()
{
	if (right != nullptr)
	{
		Node *current = right;
		while (current->left != nullptr)
			current = current->left;
		return current;
	}

	Node *current = this;
	while (current->parent != nullptr && current->parent->right == current)
		current = current->parent;
	return current->parent;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node *CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node::decrement()
{
	if (left != nullptr)
	{
		Node *current = left;
		while (current->right != nullptr)
			current = current->right;
		return current;
	}

	Node *current = this;
	while (current->parent != nullptr && current->parent->left == current)
		current = current->parent;
	return current->parent;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator &CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator::operator=(const BSTmap_iterator &rhs)
{
	this->p_node = rhs.p_node;
	return *this;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator &CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator::operator++()
{
	p_node = p_node->increment();
	return *this;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator
CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator::operator++(int)
{
	BSTmap_iterator copy(*this);
	++(*this); // or operator++();
	return copy;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node &CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator::operator*()
{
	return *(this->p_node);
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node *CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator::operator->()
{
	return p_node;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
bool CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator::operator!=(const BSTmap::BSTmap_iterator &rhs)
{
	return p_node != rhs.p_node;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
bool CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator::operator==(const BSTmap::BSTmap_iterator &rhs)
{
	return p_node == rhs.p_node;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const::BSTmap_iterator_const(Node *p) : p_node(p)
{
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const &CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const::operator=(const BSTmap_iterator_const &rhs)
{
	p_node = rhs;
}
template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const &CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const::operator++()
{
	p_node = p_node->increment();
	return *this;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const::operator++(int)
{
	BSTmap_iterator_const copy(*this);
	++(*this);
	return copy;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node const &CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const::operator*()
{
	return *p_node;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node const *CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const::operator->()
{
	return p_node;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
bool CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const::operator!=(const BSTmap_iterator_const &rhs)
{
	return p_node != rhs.p_node;
}
template <typename KEY_TYPE, typename VALUE_TYPE>
bool CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const::operator==(const BSTmap_iterator_const &rhs)
{
	return p_node == rhs.p_node;
}

// in implementation file
// method's name and return value have to be fully qualified with
// all required namespace and class names (like CS280::BSTmap<KEY_TYPE,VALUE_TYPE>::BSTmap)
// but the argument list does not have to be:
// function argument list automatically falls into the method's namespace (Koenig lookup)
template <typename KEY_TYPE, typename VALUE_TYPE>
CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap() : pRoot(nullptr), size_(0)
{
}

// Copy constructor
template <typename KEY_TYPE, typename VALUE_TYPE>
CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap(const BSTmap &rhs) : pRoot(nullptr), size_(rhs.size_)
{
	if (rhs.pRoot != nullptr)
	{
		copyTree(rhs.pRoot, pRoot, nullptr);
		size_ = rhs.size_;
	}
}

template <typename KEY_TYPE, typename VALUE_TYPE>
CS280::BSTmap<KEY_TYPE, VALUE_TYPE> &CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::operator=(const BSTmap<KEY_TYPE, VALUE_TYPE> &rhs)
{
	if (this != &rhs)
	{
		deleteTree(pRoot);

		pRoot = nullptr;
		size_ = 0;
		if (rhs.pRoot != nullptr)
		{
			copyTree(rhs.pRoot, pRoot, nullptr);
			size_ = rhs.size_;
		}
	}

	return *this;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
void CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::copyTree(Node *rhsNode, Node *&newNode, Node *parent)
{
	if (rhsNode == nullptr)
	{
		newNode = nullptr;
		return;
	}

	newNode = new Node(rhsNode->Key(), rhsNode->Value(), parent, 0, 0, nullptr, nullptr);

	copyTree(rhsNode->left, newNode->left, newNode);
	copyTree(rhsNode->right, newNode->right, newNode);
}

template <typename KEY_TYPE, typename VALUE_TYPE>
CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::~BSTmap()
{
	if (pRoot)
		deleteTree(pRoot);
}

template <typename KEY_TYPE, typename VALUE_TYPE>
unsigned int CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::size()
{
	return size_;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
VALUE_TYPE &CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::operator[](KEY_TYPE const &key)
{
	if (pRoot == nullptr)
	{
		pRoot = new Node(key, VALUE_TYPE(), nullptr, 0, 0, nullptr, nullptr);
		this->size_++;
		return pRoot->Value();
	}
	else
	{
		Node *newNode = insert(pRoot, pRoot, key);
		return newNode->Value();
	}
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node *CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::insert(Node *&tree, Node *parent, KEY_TYPE const &key)
{
	if (tree == nullptr)
	{
		tree = new Node(key, VALUE_TYPE(), parent, 0, 0, nullptr, nullptr);
		this->size_++;
		return tree;
	}

	if (key < tree->Key())
	{
		return insert(tree->left, tree, key);
	}
	else if (key > tree->Key())
	{
		return insert(tree->right, tree, key);
	}
	else
	{
		return tree;
	}
}

template <typename KEY_TYPE, typename VALUE_TYPE>
void CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::deleteTree(Node *tree)
{
	if (tree == nullptr)
		return;

	deleteTree(tree->left);
	deleteTree(tree->right);
	delete tree;
}

// typename is used to help compiler to parse
template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::begin()
{
	if (pRoot)
		return BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator(pRoot->first());
	else
		return end_it;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::end()
{
	return end_it;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::find(KEY_TYPE const &key)
{
	Node *node = pRoot;
	while (node)
	{
		if (key < node->Key())
			node = node->left;
		else if (key > node->Key())
			node = node->right;
		else
			return BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator(node);
	}
	return end_it;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
void CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::erase(BSTmap_iterator it)
{
	if (it == end_it)
		return;
	deleteNode(it.p_node);
}

template <typename KEY_TYPE, typename VALUE_TYPE>
void CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::deleteNode(Node *node)
{
	if (!node->left && !node->right)
	{
		if (node != pRoot)
		{
			Node *&n = Node::findNode(node);
			n = nullptr;
		}
		else
		{
			pRoot = nullptr;
		}
		delete node;
		size_--;
	}
	else if (node->left && node->right)
	{
		Node *suc = node->decrement();
		node->Value() = suc->Value();
		node->key = suc->key;
		deleteNode(suc);
	}
	else
	{
		Node *child = (node->left) ? node->left : node->right;

		if (node != pRoot)
		{
			Node *&n = Node::findNode(node);
			n = child;
			child->parent = node->parent;
		}
		else
		{
			pRoot = child;
		}
		delete node;
		size_--;
	}
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node *&CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node::findNode(Node *node)
{
	if (node->parent->right == node)
		return node->parent->right;
	else
		return node->parent->left;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::begin() const
{
	if (pRoot)
		return BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator(pRoot->first());
	else
		return end_it;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::end() const
{
	return end_it;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::find(KEY_TYPE const &key) const
{
	Node *node = pRoot;
	while (node)
	{
		if (key < node->Key())
			node = node->left;
		else if (key > node->Key())
			node = node->right;
		else
			return BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator(node);
	}
	return end_it;
}

////////////////////////////////////////////////////////////
// do not change this code from here to the end of the file
/* figure out whether node is left or right child or root
 * used in print_backwards_padded
 */
template <typename KEY_TYPE, typename VALUE_TYPE>
char CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::getedgesymbol(const Node *node) const
{
	const Node *parent = node->parent;
	if (parent == nullptr)
		return '-';
	else
		return (parent->left == node) ? '\\' : '/';
}

template <typename KEY_TYPE, typename VALUE_TYPE>
int CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::getdepth(const Node *node) const
{
	int depth = 0;
	while (node->parent)
	{
		depth++;
		node = node->parent;
	}
	return depth;
}

/* this is another "ASCII-graphical" print, but using
 * iterative function.
 * Left branch of the tree is at the bottom
 */
template <typename KEY_TYPE, typename VALUE_TYPE>
std::ostream &CS280::operator<<(std::ostream &os, BSTmap<KEY_TYPE, VALUE_TYPE> const &map)
{
	map.print(os);
	return os;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
void CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::print(std::ostream &os, bool print_value) const
{
	if (pRoot)
	{
		BSTmap<KEY_TYPE, VALUE_TYPE>::Node *b = pRoot->last();
		while (b)
		{
			int depth = getdepth(b);
			int i;
			/* printf(b); */

			char edge = getedgesymbol(b);
			switch (edge)
			{
			case '-':
				for (i = 0; i < depth; ++i)
					std::printf("       ");
				os << b->key;
				if (print_value)
				{
					os << " -> " << b->value;
				}
				os << std::endl;
				break;
			case '\\':
				for (i = 0; i < depth; ++i)
					std::printf("       ");
				os << edge << std::endl;
				for (i = 0; i < depth; ++i)
					std::printf("       ");
				os << b->key;
				if (print_value)
				{
					os << " -> " << b->value;
				}
				os << std::endl;
				break;
			case '/':
				for (i = 0; i < depth; ++i)
					std::printf("       ");
				os << b->key;
				if (print_value)
				{
					os << " -> " << b->value;
				}
				os << std::endl;
				for (i = 0; i < depth; ++i)
					std::printf("       ");
				os << edge << std::endl;
				break;
			}
			b = b->decrement();
		}
	}
	std::printf("\n");
}
