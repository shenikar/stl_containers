#ifndef S21_RBTREE_H_
#define S21_RBTREE_H_

#include <functional>
#include <stdexcept>
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

  RBTree() noexcept : root_(nullptr), size_(0) {}
  explicit RBTree(std::initializer_list<Key> const &items) : root_(nullptr), size_(0)
  {
    for (const auto &item : items)
    {
      Insert(item);
    }
  }

  RBTree(const RBTree &other) : root_(nullptr), size_(0)
  {
    *this = other;
  }

  RBTree(RBTree &&other) noexcept : root_(other.root_), size_(other.size_)
  {
    other.root_ = nullptr;
    other.size_ = 0;
  }

  ~RBTree()
  {
    Clear();
  }

  std::pair<Iterator, bool> Insert(const Key &value)
  {
    auto new_node = new Node(value);
    auto is_insert = Insert(new_node);
    if (is_insert)
    {
      return std::make_pair(Iterator(new_node), is_insert);
    }
    else
    {
      delete new_node;
      return std::make_pair(Find(value), is_insert);
    }
  }

  std::pair<Iterator, bool> InsertOrAssign(const Key &key)
  {
    auto new_node = new Node(key);
    auto is_insert = Insert(new_node);
    if (!is_insert)
    {
      auto it = Find(key);
      Erase(it);
      Insert(new_node);
    }
    return std::make_pair(Iterator(new_node), is_insert);
  }

  Iterator Begin() noexcept
  {
    if (root_ == nullptr)
    {
      return Iterator(nullptr);
    }
    Node *current = root_;
    while (current->left != nullptr)
    {
      current = current->left;
    }
    return Iterator(current);
  }

  Iterator End() noexcept
  {
    return Iterator(nullptr);
  }

  ConstIterator Begin() const noexcept
  {
    if (root_ == nullptr)
    {
      return ConstIterator(nullptr);
    }
    Node *current = root_;
    while (current->left != nullptr)
    {
      current = current->left;
    }
    return ConstIterator(current);
  }

  ConstIterator End() const noexcept
  {
    return ConstIterator(nullptr);
  }

  void Merge(RBTree<Key, Compare> &other)
  {
    for (auto it = other.Begin(); it != other.End();)
    {
      auto new_node = new Node(*it);
      auto next_it = it;
      ++next_it;
      if (Insert(new_node))
      {
        other.Erase(it);
      }
      else
      {
        delete new_node;
      }
      it = next_it;
    }
  }

  void Swap(RBTree &other) noexcept
  {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
  }

  void Erase(Iterator pos) noexcept
  {
    if (pos.node_ == nullptr)
    {
      return;
    }
    if (pos.node_->left_ == nullptr)
    {
      if (pos.node_->parent_ == nullptr)
      {
        root_ = pos.node_->right_;
        if (root_ != nullptr)
        {
          root_->parent_ = nullptr;
        }
      }
      else
      {
        if (pos.node_->parent_->left_ == pos.node_)
        {
          pos.node_->parent_->left_ = pos.node_->right_;
        }
        if (pos.node_->right_ != nullptr)
        {
          pos.node_->right_->parent_ = pos.node_->parent_;
        }
        else
        {
          pos.node_->parent_->right_ = pos.node_->right_
        }
      }
      delete pos.node_;
      --size_;
    }
    else if (pos.node_->right_ == nullptr)
    {
      if (pos.node_->parent_ == nullptr)
      {
        root_ = pos.node_->left_;
        root_->parent_ = nullptr;
      }
      else
      {
        if (pos.node_->parent_->left_ == pos.node_)
        {
          pos.node_->parent_->left_ = pos.node_->left_;
          pos.node_->left_->parent_ = pos.node_->parent_;
        }
        else
        {
          pos.node_->parent_->right_ = pos.node_->left_;
          pos.node_->left_->parent_ = pos.node_->parent_;
        }
      }
      delete pos.node_;
      --size_;
    }
    else
    {
      Node *successor = pos.node_->right_;
      while (successor->left_ != nullptr)
      {
        successor = successor->left_;
      }
      auto new_node = new Node(successor->key_, pos.node_->left_, pos.node_->right_, pos.node_->parent_);
      if (pos.node_->left_ != nullptr)
      {
        pos.node_->left_->parent_ = new_node;
      }
      if (pos.node_->right_ != nullptr)
      {
        pos.node_->right_->parent_ = new_node;
      }
      delete pos.node_;
      pos.node_ = new_node;
      if (new_node->parent_ == nullptr)
      {
        root_ = new_node;
      }
      Erase(Iterator(successor));
    }
  }

  Key &At(const Key &key) &
  {
    auto it = Find(key);
    if (it != End())
    {
      return *it;
    }
    throw std::out_of_range("Key not found");
  }

  const Key &At(const Key &key) const &
  {
    auto it = Find(key);
    if (it != End())
    {
      return *it;
    }
    throw std::out_of_range("Key not found");
  }
  Iterator Find(const Key &key) noexcept
  {
    auto cmp = Compare{};
    auto current = root_;
    while (current && (cmp(current->key_, key) || cmp(key, current->key_)))
    {
      if (cmp(current->key_, key))
      {
        current = current->right_;
      }
      else
      {
        current = current->left_;
      }
    }
    if (current == nullptr)
    {
      return Iterator(nullptr);
    }
    else
    {
      return Iterator(current);
    }
  }

  ConstIterator Find(const Key &key) const noexcept
  {
    auto cmp = Compare{};
    auto current = root_;
    while (current && (cmp(current->key_, key) || cmp(key, current->key_)))
    {
      if (cmp(current->key_, key))
      {
        current = current->right_;
      }
      else
      {
        current = current->left_;
      }
    }
    if (current == nullptr)
    {
      return ConstIterator(nullptr);
    }
    else
    {
      return ConstIterator(current);
    }
  }

  bool Contains(const Key &key) const noexcept
  {
    return Find(key) != End();
  }

  void Clear() noexcept
  {
    Iterator it = Begin();
    while (it != End())
    {
      Iterator current = it;
      ++it;
      Erase(current);
    }
    root_ = nullptr;
  }

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
#endif // S21_RBTREE_H_
