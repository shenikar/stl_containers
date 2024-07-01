#ifndef S21_RBTREE_H_
#define S21_RBTREE_H_

#include <iostream>

template <typename Key, typename T>
class RBTree
{
public:
  struct Node;

public:
  class Iterator;
  class ConstIterator;

  using key_type = Key;
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  class Iterator
  {
    friend class RBTree<Key, T>;

  public:
    Iterator();
    Iterator(Node *node, Node *past_node = nullptr);
    iterator &operator++();
    iterator operator++(int);
    iterator &operator--();
    iterator operator--(int);
    bool operator==(const iterator &other) const;
    bool operator!=(const iterator &other) const;
    reference operator*();
    Node &operator=(Node &other) noexcept;
    friend class RBTree<Key, T>;

  protected:
    Node *iter_node_;
    Node *iter_past_node_;
    Node *move_forward(Node *node);
    Node *move_backward(Node *node);
  };
  class ConstIterator : public Iterator
  {
  public:
    ConstIterator() : Iterator(){};
    const_reference operator*() const
    {
      return Iterator::operator*();
    };
  };

  RBTree();
  RBTree(const RBTree &other);
  RBTree(RBTree &&other);
  RBTree &operator=(const RBTree &other);
  RBTree &operator=(RBTree &&other);
  ~RBTree();
  void insert(key_type &&key, value_type &&value);
  void erase(key_type &&key);
  void clear();
  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;
  bool empty() const;
  size_type size() const;
  size_type max_size() const;
  void swap(RBTree &other);
};

#endif // S21_RBTREE_H_