#include <iostream>
#include <iomanip>

template <typename T, int Size>
Lariat<T, Size>::Lariat() : head_(nullptr), tail_(nullptr), size_(0), nodecount_(0), asize_(Size) {}

template <typename T, int Size>
Lariat<T, Size>::Lariat(const Lariat &rhs)
    : head_(nullptr), tail_(nullptr), size_(0), nodecount_(0), asize_(Size)
{
    LNode *node = rhs.head_;

    while (node)
    {
        for (int i = 0; i < node->count; i++)
        {
            push_back(node->values[i]);
        }

        node = node->next;
    }
}

template <typename T, int Size>
template <typename U, int Size2>
Lariat<T, Size>::Lariat(const Lariat<U, Size2> &rhs)
    : head_(nullptr), tail_(nullptr), size_(0), nodecount_(0), asize_(Size)
{
    auto node = rhs.head_;

    while (node)
    {
        for (int i = 0; i < node->count; i++)
        {
            push_back(static_cast<T>(node->values[i]));
        }

        node = node->next;
    }
}

template <typename T, int Size>
Lariat<T, Size>::~Lariat()
{
    LNode *curr = head_;
    while (curr)
    {
        LNode *next = curr->next;
        delete curr;
        curr = next;
    }
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
    nodecount_ = 0;
}

template <typename T, int Size>
Lariat<T, Size> &Lariat<T, Size>::operator=(const Lariat<T, Size> &rhs)
{
    if (this != &rhs)
    {
        clear();

        LNode *node = rhs.head_;

        while (node)
        {
            for (int i = 0; i < node->count; i++)
            {
                push_back(node->values[i]);
            }

            node = node->next;
        }
    }
    return *this;
}

template <typename T, int Size>
template <typename U, int Size2>
Lariat<T, Size> &Lariat<T, Size>::operator=(const Lariat<U, Size2> &rhs)
{
    clear();

    auto *node = rhs.head_;

    while (node)
    {
        for (int i = 0; i < node->count; i++)
        {
            push_back(static_cast<T>(node->values[i]));
        }

        node = node->next;
    }

    return *this;
}

template <typename T, int Size>
void Lariat<T, Size>::insert(int index, const T &value)
{
    if (index < 0 || index > size_)
    {
        throw(LariatException(LariatException::E_BAD_INDEX, "Subscript is out of range"));
    }

    if (index == 0)
    {
        push_front(value);
        return;
    }

    if (index == size_)
    {
        push_back(value);
        return;
    }

    LNode *node;
    int local_index;
    std::tie(node, local_index) = findElement(index);

    if (node->count == Size)
    {
        split(node, local_index);

        if (local_index > Size / 2)
        {
            node = node->next;
            local_index = local_index - (Size / 2) - 1;
        }
    }

    shiftUp(node, local_index);

    node->values[local_index] = value;
    node->count++;
    size_++;
}

template <typename T, int Size>
void Lariat<T, Size>::push_back(const T &value)
{
    if (head_ == nullptr)
    {
        head_ = new LNode;
        nodecount_++;
        head_->next = nullptr;
        head_->prev = nullptr;
        tail_ = head_;
    }

    if (tail_->count == Size)
    {
        split(tail_, Size - 1);
    }

    tail_->values[tail_->count] = value;
    tail_->count++;
    size_++;
}

template <typename T, int Size>
void Lariat<T, Size>::push_front(const T &value)
{
    if (head_ == nullptr)
    {
        head_ = new LNode;
        nodecount_++;
        head_->next = nullptr;
        head_->prev = nullptr;
        tail_ = head_;
    }

    if (head_->count == 0)
    {
        head_->values[0] = value;
        head_->count++;
        size_++;
        return;
    }

    if (head_->count == Size)
    {
        split(head_, 0);
        if (head_->next == nullptr)
        {
            tail_ = head_->next;
        }
    }

    shiftUp(head_, 0);
    head_->values[0] = value;
    head_->count++;
    size_++;
}

template <typename T, int Size>
void Lariat<T, Size>::erase(int index)
{
    // check for out of bounds
    if (index < 0 || index >= size_)
    {
        throw LariatException(LariatException::E_BAD_INDEX, "Subscript is out of range");
    }

    // get node and local index
    LNode *node;
    int local_index;
    std::tie(node, local_index) = findElement(index);

    // shift elements in node to the left
    shiftDown(node, local_index);

    // decrement count of node
    node->count--;
    size_--;
}

template <typename T, int Size>
void Lariat<T, Size>::pop_back()
{
    // if tail is empty, return
    if (tail_ == nullptr)
    {
        return;
    }
    tail_->count--;
    size_--;
    if (tail_->count == 0)
    {
        if (tail_->prev == nullptr)
        {
            head_ = tail_ = nullptr;
        }
        else
        {
            tail_ = tail_->prev;
            tail_->next = nullptr;
        }
        nodecount_--;
    }
}
template <typename T, int Size>
void Lariat<T, Size>::pop_front()
{
    // if head is empty, return
    if (!head_)
    {
        return;
    }
    shiftDown(head_, 0);
    head_->count--;
    size_--;

    if (head_->count <= 0)
    {
        LNode *node = head_;
        head_ = head_->next;
        head_->prev = nullptr;
        delete node;
        nodecount_--;
    }
}

template <typename T, int Size>
T &Lariat<T, Size>::operator[](int index)
{
    LNode *node;
    int local_index;
    std::tie(node, local_index) = findElement(index);

    if (node)
    {
        return node->values[local_index];
    }

    throw(LariatException(LariatException::E_BAD_INDEX, "Subscript is out of range"));
}

template <typename T, int Size>
const T &Lariat<T, Size>::operator[](int index) const
{
    LNode *node;

    int local_index;
    std::tie(node, local_index) = findElement(index);

    if (node)
    {
        return node->values[local_index];
    }

    throw(LariatException(LariatException::E_BAD_INDEX, "Subscript is out of range"));
}

template <typename T, int Size>
T &Lariat<T, Size>::first()
{
    return head_->values[0];
}

template <typename T, int Size>
const T &Lariat<T, Size>::first() const
{
    return head_->values[0];
}

template <typename T, int Size>
T &Lariat<T, Size>::last()
{
    return tail_->values[tail_->count - 1];
}

template <typename T, int Size>
const T &Lariat<T, Size>::last() const
{
    return tail_->values[tail_->count - 1];
}

template <typename T, int Size>
unsigned Lariat<T, Size>::find(const T &value) const
{
    unsigned index = 0;
    LNode *current = head_;
    while (current)
    {
        for (int i = 0; i < current->count; i++)
        {
            if (current->values[i] == value)
            {
                return index + i;
            }
        }
        index += current->count;
        current = current->next;
    }
    return size_;
}

template <typename T, int Size>
size_t Lariat<T, Size>::size() const
{
    return size_;
}

template <typename T, int Size>
void Lariat<T, Size>::clear(void)
{
    LNode *current = head_;
    while (current)
    {
        LNode *next = current->next;
        delete current;
        current = next;
    }
    head_ = tail_ = nullptr;
    size_ = 0;
    nodecount_ = 0;
}

template <typename T, int Size>
void Lariat<T, Size>::compact()
{
    if (head_ == nullptr)
    {
        return;
    }

    LNode *lft = head_;
    LNode *rgt = head_->next;

    while (lft->count == Size && rgt != nullptr)
    {
        lft = rgt;
        rgt = rgt->next;
    }

    while (rgt != nullptr)
    {
        int rtot = rgt->count;
        rgt->count = 0;
        int rpos = 0;

        while (rpos < rtot)
        {
            if (lft->count == Size)
            {
                lft = lft->next;
            }

            lft->values[lft->count++] = rgt->values[rpos++];
        }

        rgt = rgt->next;
    }

    LNode *cur = lft->next;
    lft->next = nullptr;
    tail_ = lft;

    while (cur != nullptr)
    {
        LNode *temp = cur;
        cur = cur->next;
        delete temp;
        nodecount_--;
    }
}

template <typename T, int Size>
void Lariat<T, Size>::split(LNode *node, int local_index)
{
    // create new node
    LNode *newNode = new LNode;
    newNode->next = node->next;
    newNode->prev = node;
    if (node->next != nullptr)
    {
        node->next->prev = newNode;
    }
    node->next = newNode;
    if (tail_ == node)
    {
        tail_ = newNode;
    }
    nodecount_++;

    // find half for the nodes
    int half = Size / 2;

    if (Size % 2 == 0 && local_index > (Size / 2))
    {
        half = (Size / 2) + 1;
    }

    if (Size % 2 != 0 && local_index > (Size / 2))
    {
        half = (Size + 1) / 2;
    }

    // insert values into the other node
    for (int i = half; i < Size; i++)
    {
        newNode->values[newNode->count] = node->values[i];
        node->count--;
        newNode->count++;
    }
}

template <typename T, int Size>
std::pair<typename Lariat<T, Size>::LNode *, int> Lariat<T, Size>::findElement(int index)
{
    LNode *curr = head_;
    int currIndex = 0;

    while (curr != nullptr)
    {

        if (currIndex + curr->count > index)
        {
            break;
        }
        currIndex += curr->count;
        curr = curr->next;
    }

    return std::make_pair(curr, index - currIndex);
}

template <typename T, int Size>
void Lariat<T, Size>::swap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

template <typename T, int Size>
void Lariat<T, Size>::shiftUp(LNode *node, int index)
{
    for (int i = node->count; i > index; i--)
    {
        swap(node->values[i], node->values[i - 1]);
    }
}

template <typename T, int Size>
void Lariat<T, Size>::shiftDown(LNode *node, int index)
{
    for (int i = index; i < node->count - 1; i++)
    {
        swap(node->values[i], node->values[i + 1]);
    }
}

#if 1
template <typename T, int Size>
std::ostream &operator<<(std::ostream &os, Lariat<T, Size> const &list)
{
    typename Lariat<T, Size>::LNode *current = list.head_;
    int index = 0;
    while (current)
    {
        os << "Node starting (count " << current->count << ")\n";
        for (int local_index = 0; local_index < current->count; ++local_index)
        {
            os << index << " -> " << current->values[local_index] << std::endl;
            ++index;
        }
        os << "-----------\n";
        current = current->next;
    }
    return os;
}
#else // fancier
#endif
