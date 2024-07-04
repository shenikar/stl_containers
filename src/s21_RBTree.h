#ifndef S21_RBTREE_H_
#define S21_RBTREE_H_

#include <functional>

template <class T, class Compare = std::less<T>>
class RBTree
{
private:
  struct Node;
  Node *root_;
  size_t size_;

  bool insert(Node *new_node) noexcept;

public:
  using Key = T;
  using size_type = size_t;

  class Iterator;
  class ConstIterator;

  RBTree() noexcept;
  explicit RBTree(std::initializer_list<Key> const &items);
  RBTree(const RBTree &other);
  RBTree(RBTree &&other) noexcept;
  ~RBTree();

  std::pair<Iterator, bool> insert(const Key &value);
  std::pair<Iterator, bool> insert_or_assign(const Key &key);
  Iterator begin() noexcept;
  Iterator end() noexcept;
  ConstIterator begin() const noexcept;
  ConstIterator end() const noexcept;
  void merge(RBTree<Key, Compare> &other);
  void swap(RBTree &other) noexcept;
  void erase(Iterator pos) noexcept;
  Key &at(const Key &key) &;
  const Key &at(const Key &key) const &;
  Iterator find(const Key &key) noexcept;
  ConstIterator find(const Key &key) const noexcept;
  bool contains(const Key &key) const noexcept;
  void clear() noexcept;
  Key &operator[](const Key &key) & noexcept;
  RBTree &operator=(const RBTree &other) &;
  RBTree &operator=(RBTree &&other) & noexcept;
  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const;
  Iterator insert_replay(const Key &key);
  std::pair<Iterator, Iterator> equal_range(const Key &key) noexcept;
  std::pair<ConstIterator, ConstIterator> equal_range(const Key &key) const noexcept;
  Iterator lower_bound(const Key &key) noexcept;
  ConstIterator lower_bound(const Key &key) const noexcept;
  Iterator upper_bound(const Key &key) noexcept;
  ConstIterator upper_bound(const Key &key) const noexcept;
  size_type count(const Key &key) const noexcept;

private:
  struct Node
  {
    Key key_;
    Node *left_;
    Node *right_;
    Node *parent_;

    explicit Node(Key key, Node *left = nullptr, Node *right = nullptr, Node *parent = nullptr) : key_(std::move(key)), left_(left), right_(right), parent_(parent) {}
  };

public:
  class Iterator
  {
  private:
    Node *node_;

  public:
    friend class RBTree;
    explicit Iterator(Node *node);
    Iterator &operator++() & noexcept;
    Iterator &operator--() & noexcept;
    bool operator==(const Iterator &other) const noexcept;
    bool operator!=(const Iterator &other) const noexcept;
    Key &operator*() const &;
  };

  class ConstIterator
  {
  private:
    Node *node_;

  public:
    friend class RBTree;
    explicit ConstIterator(Node *node);
    ConstIterator &operator++() & noexcept;
    ConstIterator &operator--() & noexcept;
    bool operator==(const ConstIterator &other) const noexcept;
    bool operator!=(const ConstIterator &other) const noexcept;
    const Key &operator*() const &;
  };
};

#include "s21_RBTree.h"

#endif // S21_RBTREE_H_