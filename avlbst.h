#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insert_fix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void remove_fix(AVLNode<Key, Value>* n, int8_t diff);
    void rotate_left(AVLNode<Key, Value>* n);
    void rotate_right(AVLNode<Key, Value>* n);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
     if(this->empty()){
      this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
      return;
    }

    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);
    // Use prev in case temp == nullptr, there's no way to call getParent()
    AVLNode<Key, Value>* prev = nullptr;

    while(temp != nullptr){
      prev = temp;
      if(temp->getKey() > new_item.first){
        temp = temp->getLeft();
      }
      else if(temp->getKey() < new_item.first){
        temp = temp->getRight();
      }
      else{
        temp->setValue(new_item.second);
        return;
      }
    }

    AVLNode<Key, Value>* n = new AVLNode<Key, Value>(new_item.first, new_item.second, prev);
    if(prev->getKey() > new_item.first){
      prev->setLeft(n);
    }
    else{
      prev->setRight(n);
    }

    insert_fix(prev, n);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n){
    // Base Case
    if(p == nullptr){return;}

    // Update balance of p (not of g)
    if (n == p->getLeft()) {
        p->updateBalance(-1);
    } else {
        p->updateBalance(1);
    }

    if(p->getBalance() == 0){
        return;
    }
    else if (p->getBalance() == 1 || p->getBalance() == -1){
        insert_fix(p->getParent(), p);
        return;
    }
    else if(p->getBalance() < -1){
      if(n->getBalance() <= 0){
        rotate_right(p);
      }
      else{
        rotate_left(n);
        rotate_right(p);
      }
      return;
    }
    else if(p->getBalance() > 1){
      if(n->getBalance() >= 0){
        rotate_left(p);
      }
      else{
        rotate_right(n);
        rotate_left(p);
      }
      return;
    }
}

template<class Key, class Value> 
void AVLTree<Key, Value>::rotate_right(AVLNode<Key, Value>* n)
{ 
  if(n == nullptr){return;} 

  AVLNode<Key, Value>* c = n->getLeft(); 
  if(c == nullptr){return;} 

  AVLNode<Key, Value>* np = n->getParent(); 
  AVLNode<Key, Value>* cr = c->getRight();
  
  // Part 1: Detach cr, Attach to n_left
  n->setLeft(cr);
  if(cr != nullptr){cr->setParent(n);} 

  // Part 2: Detach c, Attach to np
  c->setParent(np); 
  if(np == nullptr){ 
    this->root_ = c; 
  } 
  else if(np->getLeft() == n){
    np->setLeft(c); 
  } 
  else if(np->getRight() == n){
    np->setRight(c);
  } 
  
  // Part 3: Detach n, Attach to c_right
  n->setParent(c); 
  c->setRight(n);

  // Reset Balance
  int c_height_l = this->height(static_cast<AVLNode<Key, Value>*>(c->getLeft()));
  int c_height_r = this->height(static_cast<AVLNode<Key, Value>*>(c->getRight()));
  c->setBalance((int8_t)(c_height_r - c_height_l));

  int n_height_l = this->height(static_cast<AVLNode<Key, Value>*>(n->getLeft()));
  int n_height_r = this->height(static_cast<AVLNode<Key, Value>*>(n->getRight()));
  n->setBalance((int8_t)(n_height_r - n_height_l));
}

template<class Key, class Value> 
void AVLTree<Key, Value>::rotate_left(AVLNode<Key, Value>* n)
{ 
  if(n == nullptr){return;} 

  AVLNode<Key, Value>* c = n->getRight(); 
  if(c == nullptr){return;} 

  AVLNode<Key, Value>* np = n->getParent(); 
  AVLNode<Key, Value>* cr = c->getLeft();
  
  // Part 1: Detach cr, Attach to n_right
  n->setRight(cr);
  if(cr != nullptr){cr->setParent(n);} 

  // Part 2: Detach c, Attach to np
  c->setParent(np); 
  if(np == nullptr){ 
    this->root_ = c; 
  } 
  else if(np->getLeft() == n){
    np->setLeft(c); 
  } 
  else if(np->getRight() == n){
    np->setRight(c);
  } 
  
  // Part 3: Detach n, Attach to c_left
  n->setParent(c); 
  c->setLeft(n);

  // Reset Balance
  int n_height_l = this->height(static_cast<AVLNode<Key, Value>*>(n->getLeft()));
  int n_height_r = this->height(static_cast<AVLNode<Key, Value>*>(n->getRight()));
  n->setBalance((int8_t)(n_height_r - n_height_l));

  int c_height_l = this->height(static_cast<AVLNode<Key, Value>*>(c->getLeft()));
  int c_height_r = this->height(static_cast<AVLNode<Key, Value>*>(c->getRight()));
  c->setBalance((int8_t)(c_height_r - c_height_l));
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
  AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    // If found nothing
    if(curr == nullptr){return;}


    // Two children (swap with predecessor) --> One or No child
    if(curr->getLeft() != nullptr && curr->getRight() != nullptr){
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(curr));
        // after swap, there'll be only no right child of curr
        nodeSwap(curr, pred);  // curr at most 1 children
    }

    // Figure out what that child is: Left or Right or NULL
    AVLNode<Key, Value>* child;
    if (curr->getLeft() != nullptr) {
        child = curr->getLeft();
    } else {
        child = curr->getRight();
    }

    // Root Case (0 or 1 child)
    if(curr == this->root_){
        this->root_ = child; // NULL or (Left or Right)

        // if not null -- update child's parent
        // child's balance not affected
        if(child != nullptr){
            child->setParent(nullptr);
        }
        delete curr;
        return;
    }

    // Non Root Case (0 or 1 child)
    AVLNode<Key, Value>* parent = curr->getParent();
    int8_t diff;

    if(parent->getLeft() == curr){ 
        diff = 1;
        parent->setLeft(child); // NULL or (Left or Right)
        // if not null -- update child's parent
        if(child != nullptr){
            child->setParent(parent);
        }
        delete curr;
    }
    else{
        diff = -1;
        parent->setRight(child); // NULL or (Left or Right)
        // if not null -- update child's parent
        if(child != nullptr){
            child->setParent(parent);
        }
        delete curr;
    }

    remove_fix(parent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove_fix(AVLNode<Key, Value>* n, int8_t diff)
{
  if(n == nullptr){return;}
  n->updateBalance(diff);

  AVLNode<Key, Value>* p = n->getParent();
  int8_t new_diff = 0;
  if(p != nullptr){
    if(p->getLeft() == n){new_diff = 1;}
    else{new_diff = -1;}
  }

  if(n->getBalance() == -1 || n->getBalance() == 1){
    return;
  }
  else if(n->getBalance() == 0){
    if(p == nullptr){return;}

    remove_fix(p, new_diff);
    return;
  }
  else if(n->getBalance() < -1){
    AVLNode<Key, Value>* c = n->getLeft();

    if(c->getBalance() == 0){
      rotate_right(n);
      return; // Do not recurse when b(c) = 0;
    }
    else if(c->getBalance() < 0){
      rotate_right(n);

      if(p == nullptr){return;}
      remove_fix(p, new_diff);
    }
    else{
      rotate_left(c);
      rotate_right(n);

      if(p == nullptr){return;}
      remove_fix(p, new_diff);
    }
    return;
  }
  else if(n->getBalance() > 1){
    AVLNode<Key, Value>* c = n->getRight();

    if(c->getBalance() == 0){
      rotate_left(n);
      return; // Do not recurse when b(c) = 0;
    }
    else if(c->getBalance() > 0){
      rotate_left(n);

      if(p == nullptr){return;}
      remove_fix(p, new_diff);
    }
    else{
      rotate_right(c);
      rotate_left(n);

      if(p == nullptr){return;}
      remove_fix(p, new_diff);
    }
    return;
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
