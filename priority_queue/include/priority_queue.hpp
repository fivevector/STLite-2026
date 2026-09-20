#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cmath>       // in case you need it
#include <cstddef>     // for size_t
#include <functional>  // for std::less

#include "exceptions.hpp"

namespace sjtu {

/**
 * @brief A container automatically sorting its contents, similar to
 * std::priority_queue but with extra functionalities.
 *
 * The extra functionalities are:
 * - Merge two priority queues into one (with good time complexity).
 * - Clear all elements in the queue.
 * - Limited exception safety for some operations (e.g. push, pop, top, merge)
 * when the comparator throws exceptions from `Compare` only.
 *
 * This @priority_queue does not support passing an underlying container as a template parameter.
 * Also, it does not support passing a comparator object as a constructor argument.
 *
 */
template <class T, class Compare = std::less<T>>
class priority_queue {
   private:
    struct Node {
        T value;
        Node* child;
        Node* sibling;
        size_t degree;
        Node(const T& val) : value(val), child(nullptr), sibling(nullptr), degree(0) {}
    };
    struct NodeState {
        Node* node;
        Node* child;
        Node* sibling;
        size_t degree;
        NodeState(Node* n) : node(n), child(n->child), sibling(n->sibling), degree(n->degree) {}
        NodeState() : node(nullptr), child(nullptr), sibling(nullptr), degree(0) {}
    };
    Node* root;
    size_t current_size;
    Compare comp;

    static Node* copy_tree(Node* node) {
        if (node == nullptr) return nullptr;
        Node* new_node = new Node(node->value);
        new_node->degree = node->degree;
        new_node->child = copy_tree(node->child);
        new_node->sibling = copy_tree(node->sibling);
        return new_node;
    }
    static void destroy_tree(Node* node) {
        if (node == nullptr) return;
        destroy_tree(node->child);
        destroy_tree(node->sibling);
        delete node;
    }
    static Node* merge_lists(Node* a, Node* b) {
        if(!a) return b;
        if(!b) return a;
        Node* head = nullptr;
        Node* tail = nullptr;
        if(a->degree <= b->degree) {
            head = a;
            a = a->sibling;
        } else {
            head = b;
            b = b->sibling;
        }
        tail = head;
        while(a && b) {
            if(a->degree <= b->degree) {
                tail->sibling = a;
                a = a->sibling;
            } else {
                tail->sibling = b;
                b = b->sibling;
            }
            tail = tail->sibling;
        }
        tail->sibling = a ? a : b;
        return head;
    }
    static Node* consolidate(Node* head, Compare comp) {
        if(!head) return nullptr;
        Node* prev = nullptr;
        Node* curr = head;
        Node* next = head->sibling;
        while(next) {
            if(curr->degree != next->degree || (next->sibling && next->sibling->degree == curr->degree)) {
                prev = curr;
                curr = next;
            } else {
                if(comp(next->value, curr->value)) {
                    curr->sibling = next->sibling;
                    next->sibling = curr->child;
                    curr->child = next;
                    curr->degree++;
                } else {
                    if(prev) {
                        prev->sibling = next;
                    } else {
                        head = next;
                    }
                    curr->sibling = next->child;
                    next->child = curr;
                    next->degree++;
                    curr = next;
                }
            }
            next = curr->sibling;
        }
        return head;
    }
    static Node* reverse_list(Node* node) {
        Node* prev = nullptr;
        Node* curr = node;
        while(curr) {
            Node* next = curr->sibling;
            curr->sibling = prev;
            prev = curr;
            curr = next;
        }
        return prev;
    }

   public:
    priority_queue() : root(nullptr), current_size(0), comp(Compare()) {}
    priority_queue(const priority_queue& other) : root(nullptr), current_size(0), comp(other.comp) {
        // Copy constructor
        if (other.root != nullptr) {
            root = copy_tree(other.root);
            current_size = other.current_size;
        }
    }
    ~priority_queue() {
        clear();
    }

    priority_queue& operator=(const priority_queue& other) {
        if(this != &other) {
            clear();
            if (other.root != nullptr) {
                root = copy_tree(other.root);
                current_size = other.current_size;
            }
        }
        return *this;
    }

    /** Adds one element to the queue. */
    void push(const T& value) {
        Node* old_root = root;
        size_t saved_count = 0;
        for(Node* p = root; p != nullptr; p = p->sibling) ++saved_count;
        NodeState* states = new NodeState[saved_count];
        size_t used = 0;
        for(Node* p = root; p != nullptr; p = p->sibling) states[used++] = NodeState(p);
        Node* new_node = new Node(value);
        try {
            root = merge_lists(root, new_node);
            root = consolidate(root, comp);
            current_size++;
        } catch (...) {
            for(size_t i = 0; i < used; ++i) {
                states[i].node->child = states[i].child;
                states[i].node->sibling = states[i].sibling;
                states[i].node->degree = states[i].degree;
            }
            root = old_root;
            delete new_node;
            delete[] states;
            throw;
        }
        delete[] states;
    }

    /**
     * Returns a read-only reference of the first element in the queue.
     *
     * @throws container_is_empty when the first element does not exist.
     */
    const T& top() const {
        Node* now_node = root;
        if(now_node == nullptr) throw container_is_empty();
        Node* max_node = now_node;
        for(; now_node->sibling != nullptr; now_node = now_node->sibling) {
            if(comp(max_node->value, now_node->sibling->value)) {
                max_node = now_node->sibling;
            }
        }
        return max_node->value;
    }

    /**
     * Removes the first element in the queue.
     *
     * @throws container_is_empty when the first element does not exist.
     */
    void pop() {
        Node* best_node = root;
        Node* best_prev = nullptr;
        Node* current_node = root;
        Node* current_prev = nullptr;
        if(!root) throw container_is_empty();

        /**
         * STEP 1: Find and remove the node with the highest priority (best_node) and its previous node (best_prev).
         * STEP 2: Merge the children of best_node into the root list.
         * STEP 3: Consolidate the root list to maintain the binomial heap properties.
         */
        while(current_node) {
            if(comp(best_node->value, current_node->value)) {
                best_node = current_node;
                best_prev = current_prev;
            }
            current_prev = current_node;
            current_node = current_node->sibling;
        }

        Node* old_root = root;
        size_t saved_count = 0;
        for(Node* p = root; p != nullptr; p = p->sibling) ++saved_count;
        for(Node* p = best_node->child; p != nullptr; p = p->sibling) ++saved_count;
        NodeState* states = new NodeState[saved_count];
        size_t used = 0;
        for(Node* p = root; p != nullptr; p = p->sibling) states[used++] = NodeState(p);
        for(Node* p = best_node->child; p != nullptr; p = p->sibling) states[used++] = NodeState(p);
        try {
        if(best_prev) {
            best_prev->sibling = best_node->sibling;
        } else {
            root = best_node->sibling;
        }
        Node* child = best_node->child;
        if(child) {
            child = reverse_list(child);
            root = merge_lists(root, child);
            root = consolidate(root, comp);
        }current_size--;
        delete best_node;
        } catch (...) {
            for(size_t i = 0; i < used; ++i) {
                states[i].node->child = states[i].child;
                states[i].node->sibling = states[i].sibling;
                states[i].node->degree = states[i].degree;
            }
            root = old_root;
            delete[] states;
            throw;
        }
        delete[] states;
    }
        
    /** Returns the number of elements in the queue. */
    size_t size() const {
        return current_size;
    }

    /** Returns whether there is any element in the queue. */
    bool empty() const {
        return current_size == 0;
    }

    /** Clears all elements in the queue. */
    void clear() {
        destroy_tree(root);
        root = nullptr;
        current_size = 0;
    }

    /**
     * @brief Merges two priority queues into one.
     *
     * The merged data shall be stored in the current priority queue and the
     * other priority queue shall be cleared after merging.
     *
     * The time complexity shall be O(log n) or better.
     */
    void merge(priority_queue& pq) {
        if(this == &pq) return;  // Avoid self-merge
        Node* old_root = root;
        Node* other_root = pq.root;
        size_t saved_count = 0;
        for(Node* p = root; p != nullptr; p = p->sibling) ++saved_count;
        for(Node* p = other_root; p != nullptr; p = p->sibling) ++saved_count;
        NodeState* states = new NodeState[saved_count];
        size_t used = 0;
        for(Node* p = root; p != nullptr; p = p->sibling) states[used++] = NodeState(p);
        for(Node* p = other_root; p != nullptr; p = p->sibling) states[used++] = NodeState(p);
        try {
            root = merge_lists(root, other_root);
            root = consolidate(root, comp);
            current_size += pq.current_size;
            pq.root = nullptr;
            pq.current_size = 0;
        } catch (...) {
            for(size_t i = 0; i < used; ++i) {
                states[i].node->child = states[i].child;
                states[i].node->sibling = states[i].sibling;
                states[i].node->degree = states[i].degree;
            }
            root = old_root;
            pq.root = other_root;
            delete[] states;
            throw;
        }
        delete[] states;
    }
};

}  // namespace sjtu

#endif