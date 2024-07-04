#include "s21_RBTree.h"

template <class T, class Compare>
RBTree<T, Compare>::RBTree() noexcept : root_(nullptr), size_(0) {}

template <class T, class Compare>
RBTree<T, Compare>::RBTree(std::initializer_list<Key> const &items) : root_(nullptr), size_(0)
{
    for (const auto &item : items)
    {
        insert(item);
    }
}

template <class T, class Compare>
RBTree<T, Compare>::RBTree(const RBTree &other) : root_(nullptr), size_(0)
{
    *this = other;
}
