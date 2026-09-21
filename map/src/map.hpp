/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<
    class Key,
    class T,
    class Compare = std::less <Key>
> class map {
 public:
  /**
   * the internal type of data.
   * it should have a default constructor, a copy constructor.
   * You can use sjtu::map as value_type by typedef.
   */
  typedef pair<const Key, T> value_type;
  struct Node {
    value_type value;
    Node* left;
    Node* right;
    Node* parent;
    bool color;  // true for red, false for black
    Node(const value_type& val) : value(val), left(nullptr), right(nullptr), parent(nullptr), color(true) {}
  };
  /**
   * see BidirectionalIterator at CppReference for help.
   *
   * if there is anything wrong throw invalid_iterator.
   *     like it = map.begin(); --it;
   *       or it = map.end(); ++end();
   */
 private:
  Node* root;
  size_t current_size;
  Compare comp;

  static void destroy_tree(Node* node) {
    if (node == nullptr) return;
    destroy_tree(node->left);
    destroy_tree(node->right);
    delete node;
  }

  static Node* copy_tree(Node* node) {
    if (node == nullptr) return nullptr;
    Node* new_node = new Node(node->value);
    new_node->color = node->color;
    new_node->left = copy_tree(node->left);
    new_node->right = copy_tree(node->right);
    if (new_node->left != nullptr) new_node->left->parent = new_node;
    if (new_node->right != nullptr) new_node->right->parent = new_node;
    return new_node;
  }

  Node* find_node(const Key& key) const {
    Node* current = root;
    while (current != nullptr) {
      if (comp(key, current->value.first)) {
        current = current->left;
      } else if (comp(current->value.first, key)) {
        current = current->right;
      } else return current;  
    }
    return nullptr;  
  }

  static bool is_red(Node* node) {
    return node != nullptr && node->color;
  }
  static bool is_black(Node* node) {
    return node == nullptr || !node->color;
  }
  static void set_red(Node* node) {
    if (node != nullptr) node->color = true;
  }
  static void set_black(Node* node) {
    if (node != nullptr) node->color = false;
  }

  static Node* minimum(Node* node) {
    if (node == nullptr) return nullptr;
    while (node->left != nullptr) node = node->left;
    return node;
  }
  static Node* maximum(Node* node) {
    if (node == nullptr) return nullptr;
    while (node->right != nullptr) node = node->right;
    return node;
  }

  static void left_rotate(Node*& root, Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != nullptr) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
  }
  static void right_rotate(Node*& root, Node* y) {
    Node* x = y->left;
    y->left = x->right;
    if (x->right != nullptr) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == nullptr) root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
  }

  static auto transplant(Node*& root, Node* u, Node* v) -> void {
    if(u->parent == nullptr) root = v;
    else if(u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    if(v != nullptr) v->parent = u->parent;
  }
 public:
  class const_iterator;
  class iterator {
    friend class map;
    friend class const_iterator;
   private:
    /**
     * TODO add data members
     *   just add whatever you want.
     */
    map *my_map;
    Node* node;
   public:
    iterator() : my_map(nullptr), node(nullptr) {
      // TODO
    }

    iterator(map *m, Node* n) : my_map(m), node(n) {
      // TODO
    }

    iterator(const iterator &other) : my_map(other.my_map), node(other.node) {
      // TODO
    }

    /**
     * TODO iter++
     */
    iterator operator++(int) {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    /**
     * TODO ++iter
     */
    iterator &operator++() {
      if(my_map == nullptr || node == nullptr) throw invalid_iterator();
      if(node->right != nullptr) {
        node = node->right;
        while(node->left != nullptr) 
          node = node->left;
        return *this;
      } else {
        Node* parent = node->parent;
        while(parent != nullptr && node == parent->right) {
          node = parent;
          parent = parent->parent;
        }
        node = parent;
        return *this;
      }
    }

    /**
     * TODO iter--
     */
    iterator operator--(int) {
      iterator tmp = *this;
      --(*this);
      return tmp;
    }

    /**
     * TODO --iter
     */
    iterator &operator--() {
      if(my_map == nullptr) throw invalid_iterator();
      if(node == nullptr) {
        node = maximum(my_map->root);
        if(node == nullptr) throw invalid_iterator();
        return *this;
      }
      if(node->left != nullptr) {
        node = node->left;
        while(node->right != nullptr) 
          node = node->right;
        return *this;
      } else {
        Node* parent = node->parent;
        if(parent == nullptr) throw invalid_iterator();
        while(parent != nullptr && node == parent->left) {
          node = parent;
          parent = parent->parent;
        }
        node = parent;
        return *this;
      }
    }
    

    /**
     * a operator to check whether two iterators are same (pointing to the same memory).
     */
    value_type &operator*() const {
      if(my_map == nullptr || node == nullptr) throw invalid_iterator();
      return node->value;
    }

    bool operator==(const iterator &rhs) const {
      return my_map == rhs.my_map && node == rhs.node;
    }

    bool operator==(const const_iterator &rhs) const {
      return my_map == rhs.my_map && node == rhs.node;
    }

    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const {
      return !(*this == rhs);
    }

    bool operator!=(const const_iterator &rhs) const {
      return !(*this == rhs);
    }

    /**
     * for the support of it->first.
     * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
     */
    value_type *operator->() const
    noexcept {
      return &(node->value);
    }
  };
  class const_iterator {
    // it should has similar member method as iterator.
    //  and it should be able to construct from an iterator.
    friend class map;
    friend class iterator;
   private:
    // data members.
    const map* my_map;
    const Node* node;
   public:
    const_iterator() : my_map(nullptr), node(nullptr) {
      // TODO
    }

    const_iterator(const map *m, const Node* n) : my_map(m), node(n) {
      // TODO
    }

    const_iterator(const const_iterator &other) : my_map(other.my_map), node(other.node) {
      // TODO
    }

    const_iterator(const iterator &other) : my_map(other.my_map), node(other.node) {
      // TODO
    }
    // And other methods in iterator.
    // And other methods in iterator.
    // And other methods in iterator.
    /**
     * TODO iter++
     */
    const_iterator operator++(int) {
      const_iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    /**
     * TODO ++iter
     */
    const_iterator &operator++() {
      if(my_map == nullptr || node == nullptr) throw invalid_iterator();
      if(node->right != nullptr) {
        node = node->right;
        while(node->left != nullptr) 
          node = node->left;
        return *this;
      } else {
        Node* parent = node->parent;
        if(parent == nullptr) throw invalid_iterator();
        while(parent != nullptr && node == parent->right) {
          node = parent;
          parent = parent->parent;
        }
        node = parent;
        return *this;
      }
    }

    /**
     * TODO iter--
     */
    const_iterator operator--(int) {
      const_iterator tmp = *this;
      --(*this);
      return tmp;
    }

    /**
     * TODO --iter
     */
    const_iterator &operator--() {
      if(my_map == nullptr) throw invalid_iterator();
      if(node == nullptr) {
        node = maximum(my_map->root);
        if(node == nullptr) throw invalid_iterator();
        return *this;
      }
      if(node->left != nullptr) {
        node = node->left;
        while(node->right != nullptr) 
          node = node->right;
        return *this;
      } else {
        Node* parent = node->parent;
        if(parent == nullptr) throw invalid_iterator();
        while(parent != nullptr && node == parent->left) {
          node = parent;
          parent = parent->parent;
        }
        node = parent;
        return *this;
      }
    }
    

    /**
     * a operator to check whether two iterators are same (pointing to the same memory).
     */
    const value_type &operator*() const {
      if(my_map == nullptr || node == nullptr) throw invalid_iterator();
      return node->value;
    }
    
    const value_type *operator->() const {
      if(my_map == nullptr || node == nullptr) throw invalid_iterator();
      return &(node->value);
    }

    bool operator==(const iterator &rhs) const {
      return my_map == rhs.my_map && node == rhs.node;
    }

    bool operator==(const const_iterator &rhs) const {
      return my_map == rhs.my_map && node == rhs.node;
    }

    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const {
      return !(*this == rhs);
    }

    bool operator!=(const const_iterator &rhs) const {
      return !(*this == rhs);
    }
  };

  /**
   * TODO two constructors
   */
  map() : root(nullptr), current_size(0), comp() {
  }

  map(const map &other) {
    current_size = other.current_size;
    comp = other.comp;
    root = copy_tree(other.root);
  }

  /**
   * TODO assignment operator
   */
  map &operator=(const map &other) {
    if (this != &other) {
      destroy_tree(root);
      current_size = other.current_size;
      comp = other.comp;
      root = copy_tree(other.root);
    }
    return *this;
  }

  /**
   * TODO Destructors
   */
  ~map() {
    destroy_tree(root);
    current_size = 0;
  }

  /**
   * TODO
   * access specified element with bounds checking
   * Returns a reference to the mapped value of the element with key equivalent to key.
   * If no such element exists, an exception of type `index_out_of_bound'
   */
  T &at(const Key &key) {
    Node* node = find_node(key);
    if (node == nullptr) throw index_out_of_bound();
    return node->value.second;
  }

  const T &at(const Key &key) const {
    const Node* node = find_node(key);
    if (node == nullptr) throw index_out_of_bound();
    return node->value.second;
  }

  /**
   * TODO
   * access specified element
   * Returns a reference to the value that is mapped to a key equivalent to key,
   *   performing an insertion if such key does not already exist.
   */
  T &operator[](const Key &key) {
    Node* node = find_node(key);
    if (node == nullptr) {
      pair<iterator, bool> result = insert(value_type(key, T()));
      return result.first->second;
    }
    return node->value.second;
  }

  /**
   * behave like at() throw index_out_of_bound if such key does not exist.
   */
  const T &operator[](const Key &key) const {
    const Node* node = find_node(key);
    if (node == nullptr) throw index_out_of_bound();
    return node->value.second;
  }

  /**
   * return a iterator to the beginning
   */
  iterator begin() {
    if(current_size == 0) return end();
    Node* node = root;
    while(node->left != nullptr) node = node->left;
    return iterator(this, node);
  }

  const_iterator cbegin() const {
    if(current_size == 0) return cend();
    Node* node = root;
    while(node->left != nullptr) node = node->left;
    return const_iterator(this, node);
  }

  /**
   * return a iterator to the end
   * in fact, it returns past-the-end.
   */
  iterator end() {
    return iterator(this, nullptr);
  }

  const_iterator cend() const {
    return const_iterator(this, nullptr);
  }

  /**
   * checks whether the container is empty
   * return true if empty, otherwise false.
   */
  bool empty() const {
    return current_size == 0;
  }

  /**
   * returns the number of elements.
   */
  size_t size() const {
    return current_size;
  }

  /**
   * clears the contents
   */
  void clear() {
    destroy_tree(root);
    root = nullptr;
    current_size = 0;
  }

  /**
   * insert an element.
   * return a pair, the first of the pair is
   *   the iterator to the new element (or the element that prevented the insertion),
   *   the second one is true if insert successfully, or false.
   */
  pair<iterator, bool> insert(const value_type &value) {
    if(find_node(value.first) != nullptr) {
      return pair<iterator, bool>(find(value.first), false);
    }
    Node* new_node = new Node(value);
    Node* inserted_node = new_node;
    Node* parent = nullptr;
    Node* current = root;
    while(current != nullptr) {
      parent = current;
      if(comp(new_node->value.first, current->value.first)) {
        current = current->left;
      } else {
        current = current->right;
      }
    }

    new_node->parent = parent;
    
    if(parent == nullptr) {
      root = new_node;
    } else if(comp(new_node->value.first, parent->value.first)) {
      parent->left = new_node;
    } else parent->right = new_node;
    set_red(new_node);

    while (new_node != root && is_red(new_node->parent)) {
      Node* parent = new_node->parent;
      Node* grandparent = parent->parent;
      if(parent == grandparent->left) {
        Node* uncle = grandparent->right;
        if(is_red(uncle)) {
          set_black(parent);
          set_black(uncle);
          set_red(grandparent);
          new_node = grandparent;    //case 1: Red-red violation
        } else{ //LR
          if(new_node == parent->right) {
            new_node = parent;
            left_rotate(root, new_node);
            parent = new_node->parent;
            grandparent = parent->parent;
          }
          set_black(parent);
          set_red(grandparent);
          right_rotate(root, grandparent);
        }
      } else {
        Node* uncle = grandparent->left;
        if(is_red(uncle)) {
          set_black(parent);
          set_black(uncle);
          set_red(grandparent);
          new_node = grandparent;
        } else {
          if(new_node == parent->left) {
            new_node = parent;
            right_rotate(root, new_node);
            parent = new_node->parent;
            grandparent = parent->parent;
          }
          set_black(parent);
          set_red(grandparent);
          left_rotate(root, grandparent);
        }
      }
    }

    ++current_size;
    set_black(root);
    return pair<iterator, bool>(iterator(this, inserted_node), true);
  }

  /**
   * erase the element at pos.
   *
   * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
   */
  void erase(iterator pos) {
    if(pos.my_map != this || pos.node == nullptr) throw invalid_iterator();
    Node* z = pos.node;
    Node* y = z;
    bool y_original_color = y->color;
    Node* x = nullptr;
    Node* x_parent = nullptr;
    if(z->left == nullptr) {
      x = z->right;
      transplant(root, z, z->right);
      x_parent = z->parent;
    } else if(z->right == nullptr) {
      x = z->left;
      transplant(root, z, z->left);
      x_parent = z->parent;
    } else {
      y = minimum(z->right);
      y_original_color = y->color;
      x = y->right;
      if(y->parent == z) {
        if(x != nullptr) x->parent = y;
        x_parent = y;
      } else {
        transplant(root, y, y->right);
        y->right = z->right;
        if(y->right != nullptr) y->right->parent = y;
        x_parent = y->parent;
      }
      transplant(root, z, y);
      y->left = z->left;
      if(y->left != nullptr) y->left->parent = y;
      y->color = z->color;
    }
    delete z;
    --current_size; 

    if(y_original_color == false) {
      while(x != root && is_black(x)) {
        if(x == x_parent->left) {
          Node* w = x_parent->right;
          if(is_red(w)) {
            set_black(w);
            set_red(x_parent);
            left_rotate(root, x_parent);
            w = x_parent->right;
          }
          if(is_black(w->left) && is_black(w->right)) {
            set_red(w);
            x = x_parent;
            x_parent = x->parent;
          } else {
            if(is_black(w->right)) {
              set_black(w->left);
              set_red(w);
              right_rotate(root, w);
              w = x_parent->right;
            }
            w->color = x_parent->color;
            set_black(x_parent);
            set_black(w->right);
            left_rotate(root, x_parent);
            x = root;
          }
        } else {
          Node* w = x_parent->left;
          if(is_red(w)) {
            set_black(w);
            set_red(x_parent);
            right_rotate(root, x_parent);
            w = x_parent->left;
          }
          if(is_black(w->right) && is_black(w->left)) {
            set_red(w);
            x = x_parent;
            x_parent = x->parent;
          } else {
            if(is_black(w->left)) {
              set_black(w->right);
              set_red(w);
              left_rotate(root, w);
              w = x_parent->left;
            }
            w->color = x_parent->color;
            set_black(x_parent);
            set_black(w->left);
            right_rotate(root, x_parent);
            x = root;
          }
        }
      }
      set_black(x);
    }

    set_black(root);
  }

  /**
   * Returns the number of elements with key
   *   that compares equivalent to the specified argument,
   *   which is either 1 or 0
   *     since this container does not allow duplicates.
   * The default method of check the equivalence is !(a < b || b > a)
   */
  size_t count(const Key &key) const {
    return find_node(key) != nullptr ? 1 : 0;
  }

  /**
   * Finds an element with key equivalent to key.
   * key value of the element to search for.
   * Iterator to an element with key equivalent to key.
   *   If no such element is found, past-the-end (see end()) iterator is returned.
   */
  iterator find(const Key &key) {
    Node* node = find_node(key);
    if (node == nullptr) {
      return end();
    }
    return iterator(this, node);
  }

  const_iterator find(const Key &key) const {
    const Node* node = find_node(key);
    if (node == nullptr) {
      return cend();
    }
    return const_iterator(this, node);
  }
};

}

#endif
