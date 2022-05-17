/*
 * BinarySearchTree.cpp
 * COMP15
 * Spring 2021
 *
 * Implementation of the Binary Search Tree class.
 * Behaves like a standard BST except that it handles multiplicity
 * If you insert a number twice  and then remove it once, then it will
 * still be in the data structure
 */

#include <cstring>
#include <iostream>
#include <limits>

#include "BinarySearchTree.h"

using namespace std;

/* Purpose: Initializes an empty BinarySearchTree */
BinarySearchTree::BinarySearchTree()
{
        root = nullptr;
}

/* Purpose: Destructor -- frees all memory associated with the BST */
BinarySearchTree::~BinarySearchTree()
{
        // walk tree in post-order traversal and delete
        post_order_delete(root);
        root = nullptr;  // not really necessary, since the tree is going
                         // away, but might want to guard against someone
                         // using a pointer after deleting
}
/*
 * Purpose: Deletes the BST using postorder traversal
 * Parameters: A node from a tree to start traversing from
 * Returns: None
 */
void BinarySearchTree::post_order_delete(Node *node)
{
        // base case of an empty tree
        if (node == nullptr) {
            return;
        }
        // delete left then right subtrees, in postorder fashion
        post_order_delete(node->left);
        post_order_delete(node->right);
        // delete the current node after its subtrees have been deleted
        delete node;
}

/* Purpose: Copy constructor -- creates a BST with the same information as
 *          the one passed in
 * Parameters: A reference BST to be copied
 * Returns: The freshly copied BST
 */
BinarySearchTree::BinarySearchTree(const BinarySearchTree &source)
{
        // use pre-order traversal to copy the tree
        root = pre_order_copy(source.root);
}

/* Purpose: Assignment operator -- makes the current BST contain the same
 *          elements as the one on the right hand side
 * Parameters: A reference BST to be compare to and copied
 * Returns: The assigned BST
 */
BinarySearchTree &BinarySearchTree::operator=(const BinarySearchTree &source)
{
        // check for self assignment
        if (this == &source) {
            return *this;
        }
        // delete the current tree
        post_order_delete(this->root);
        // copy the tree
        this->root = pre_order_copy(source.root);
        
        return *this;
}

/*
 * Purpose: Creates a copy of a BST using preorder traversal
 * Parameters: A node of a tree to be copied
 * Returns: A node from the freshly copied tree
 */
BinarySearchTree::Node *BinarySearchTree::pre_order_copy(Node *node) const
{
        // base case of an empty tree
        if (node == nullptr) {
            return nullptr;
        }
        // otherwise create a new identical node
        Node *newNode = new Node;
        newNode->data = node->data;
        newNode->count = node->count;
        newNode->left = pre_order_copy(node->left);
        newNode->right = pre_order_copy(node->right);
        
        return newNode;
}

/*
 * Purpose: Gives us the smallest data value stored in the tree
 * Parameters: None
 * Returns: The data value of a node
 */
int BinarySearchTree::find_min() const
{
        // if the tree is empty return the largest possible integer
        if (root == nullptr)
                return numeric_limits<int>::max(); // INT_MAX
        // otherwise return the data of the smallest node in the tree
        return find_min(root)->data;
}

/*
 * Purpose: Finds the element with the smallest data value in the tree
 * Parameters: A node from a tree to be searched
 * Returns: The node in the tree with the smallest data value
 */
BinarySearchTree::Node *BinarySearchTree::find_min(Node *node) const
{
        // if the root is the smallest node there is nothing else to return
        if (node->left == nullptr) {
            return node;
        }
        // otherwise traverse left until there are no smaller nodes left
        return find_min(node->left);
}

/*
 * Purpose: Gives us the largest data value stored in the tree 
 * Parameters: None
 * Returns: The data value of a node
 */
int BinarySearchTree::find_max() const
{
        // if the root tree is empty return the smallest possible integer
        if (root == nullptr)
            return numeric_limits<int>::min(); // INT_MIN
        // otherwise return the data of the largest node in the tree
        return find_max(root)->data;
}

/*
 * Purpose: Finds the element with the highest data value in the tree
 * Parameters: A Node from a tree
 * Returns: The address of the Node with the largest data value
 */
BinarySearchTree::Node *BinarySearchTree::find_max(Node *node) const
{
        // if the root is the biggest node there is nothing else to return
        if (node->right == nullptr) {
            return node;
        }
        // otherwise traverse right until there are no larger nodes left
        return find_max(node->right);
}

/*
 * Purpose: Check if the tree contains a certain value
 * Parameters: The data value the tree should be checked for
 * Returns: A boolean - true if the tree contains the value, false otherwise
 */
bool BinarySearchTree::contains(int value) const
{
        // if the tree is empty the value cannot be in it
        if (root == nullptr) {
            return false;
        }
        // otherwise traverse the tree until the value is found
        return contains(root, value);
}

/*
 * Purpose: Check if the tree contains a certain value
 * Parameters: The data value the tree should be checked for and a node 
               from the tree to be searched
 * Returns: A boolean - true if the tree contains the value, false otherwise
 */
bool BinarySearchTree::contains(Node *node, int value) const
{
        // if the tree is empty the value cannot be in it
        if (node == nullptr) {
            return false;
        }
        // if the first element is the value return true
        else if (node->data == value) {
            return true;
        }
        // otherwise search the left or right subtrees until the value is found
        return contains(node->left, value) or contains(node->right, value);
}

/*
 * Purpose: Creates a new node in a separate function to improve modularity
 * Parameters: The data value the new node should contain
 * Returns: The newly created node
 */
BinarySearchTree::Node *BinarySearchTree::newNodeHelper(int value)
{
        Node *newNode = new Node;
        newNode->data = value;
        newNode->count = 1;
        newNode->left = nullptr;
        newNode->right = nullptr;
    
        return newNode;
}

/*
 * Purpose: Inserts a value into the tree
 * Parameters: The value to be added to the tree
 * Returns: None
 */
void BinarySearchTree::insert(int value)
{
        insert(root, nullptr, value);
}

/*
 * Purpose: Adds a value to the tree. If it is a duplicate, it will increment 
            the count of that element.
 * Parameters: A node from a tree, its parent node, and value to be added
 * Returns: None
 */
void BinarySearchTree::insert(Node *node, Node *parent, int value)
{
        // base case of an empty tree
        if (node == nullptr) {
            // if the tree is empty make the new inserted element the root
            if (root == nullptr) {
                root = newNodeHelper(value);
                return;
            }
            // a value less than the parent gets put on the parent's left
            if (value < parent->data) {
                parent->left = newNodeHelper(value);
                return;
            }
            // a value greater than the parent gets put on the parent's right
            else if (value > parent->data) {
                parent->right = newNodeHelper(value);
                return;
            }
        }
        // traverse left if the value is less than the current element data
        else if (value < node->data) {
            insert(node->left, node, value);
        }
        // traverse right if the value is greater than the current element data
        else if (value > node->data) {
            insert(node->right, node, value);
        }
        // if the other cases weren't hit we're at the desired element
        else {
            node->count++;
        }
}

/*
 * Purpose: Removes a value from the tree
 * Parameters: The value to be removed
 * Returns: A boolean - true if an element is removed, false otherwise
 */
bool BinarySearchTree::remove(int value)
{
        return remove(root, nullptr, value);
}

/*
 * Purpose: Removes an element with the given value from the tree
 * Parameters: A tree's node, its parent node, and the value to be removed
 * Returns: A boolean - true if an element is removed, false otherwise
 */
bool BinarySearchTree::remove(Node *node, Node *parent, int value)
{
        // base case of an empty tree
        if (node == nullptr) {
            return false;
        }
        // if the value is less than the current node traverse left
        else if (value < node->data) {
            return remove(node->left, node, value);;
        }
        // if the value is less than the current node traverse right
        else if (value > node->data) {
            return remove(node->right, node, value);
        }
        // otherwise the current node must contain the value
        else {
            node->count--;
            // if all duplicates have been removed already
            if (node->count <= 0) {
                removeHelper(node, parent);
            }
            return true;
        }
}

/*
 * Purpose: Helps the remove function determine what type of removal is needed
 * Parameters: A tree's node and its parent node
 * Returns: A boolean - true if an element is removed, false otherwise
 */
void BinarySearchTree::removeHelper(Node *node, Node *parent)
{
        // if the node has no children
        if (node->left == nullptr and node->right == nullptr) {
            removeLeaf(node, parent);
        }
        // if the node has one child
        else if (node->left == nullptr or node->right == nullptr) {
            removeOneChild(node, parent);
        }
        // if the node has two children
        else {
            removeTwoChildren(node);
        }
}

/*
 * Purpose: Helps the remove function do a removal specifically for a leaf
 * Parameters: A tree's node and its parent node
 * Returns: None
 */
void BinarySearchTree::removeLeaf(Node *node, Node *parent)
{
        // if the node is the root of the tree
        if (parent == nullptr) {
            root = nullptr;
        }
        // if if the node is the left subtree release it from the parent
        else if (node->data < parent->data) {
            parent->left = nullptr;
        }
        // if if the node is the right subtree release it from the parent
        else {
            parent->right = nullptr;
        }
        delete node;
        node = nullptr;
}

/*
 * Purpose: Removes a node left of the parent and gives the parent the child
 * Parameters: A tree's node and its parent node
 * Returns: None
 */
void BinarySearchTree::removeLeft(Node *node, Node *parent) 
{
    // if there is no left subtree put the right subtree on the parent's left
    if (node->left == nullptr) {
        parent->left = node->right;
        delete node;
        node = parent->left;
    }
    // if there is no right subtree put the left subtree on the parent's left
    else if (node->right == nullptr) {
        parent->left = node->left;
        delete node;
        node = parent->left;
    }
}

/*
 * Purpose: Removes a node right of the parent and gives the parent the child
 * Parameters: A tree's node and its parent node
 * Returns: None
 */
void BinarySearchTree::removeRight(Node *node, Node *parent)
{
    // if there is no left subtree put the right subtree on the parent's right
    if (node->left == nullptr) {
        parent->right = node->right;
        delete node;
        node = parent->right;
    }
    // if there is no right subtree put the left subtree on the parent's right
    else if (node->right == nullptr) {
        parent->right = node->left;
        delete node;
        node = parent->right;
    }
}

/*
 * Purpose: Determine which sided deletion the a node with one child needs
 * Parameters: A tree's node and its parent node
 * Returns: None
 */
void BinarySearchTree::removeOneChild(Node *node, Node *parent)
{
    // if node is the root of the tree
    if (parent == nullptr) {
        // if there is no left subtree the tree becomes the right subtree
        if (node->left == nullptr) {
            root = node->right;
        }
        // if there is no right subtree the tree becomes the left subtree
        else {
            root = node->left;
        }
        delete node;
        node = nullptr;
    }
    // if the node is on the left of the parent do a left sided removal
    else if (parent->left == node) {
        removeLeft(node, parent);
    }
    // if the node is on the right of the parent do a right sided removal
    else {
        removeRight(node, parent);
    }
}

/*
 * Purpose: Helps remove with nodes with two children needing pointer updates
 * Parameters: A tree's node
 * Returns: None
 */
void BinarySearchTree::removeTwoChildren(Node *node)
{    
    // replace the node with the smallest value on the right subtree
    Node *replacement = find_min(node->right);
    int value = replacement->data;
    node->count = replacement->count;
    replacement->count = 1;
    // delete the old node in the subtree
    remove(value);

    node->data = value;
}

/*
 * Purpose: Gives us the height of the tree
 * Parameters: None
 * Returns: The height of the tree
*/
int BinarySearchTree::tree_height() const
{
        return tree_height(root);
}

/*
 * Purpose: Tells us the number of edges from the farthest leaf to the root 
 * Parameters: None
 * Returns: The count of the maximum edges in a tree
 */
int BinarySearchTree::tree_height(Node *node) const
{
        // if the tree is empty its height is -1 by definition
        if (node == nullptr) {
            return -1;
        }
        // otherwise recursively increment height until the top is reached
        return 1 + max(tree_height(node->left), tree_height(node->right));
}

/*
 * Purpose: Gives us the amount of nodes in the tree
 * Parameters: none
 * Returns: The amount of nodes in the tree
 */
int BinarySearchTree::node_count() const
{
        return node_count(root);
}

/*
 * Purpose: Returns the number of nodes/distinct values in the tree
 * Parameters: A node from the tree to be counted
 * Returns: The count of the nodes in a tree
 */
int BinarySearchTree::node_count(Node *node) const
{
        // if the tree is empty it has 0 nodes by definition
        if (node == nullptr) {
            return 0;
        }
        // otherwise recursively increment count
        return 1 + node_count(node->left) + node_count(node->right);
}

/*
 * Purpose: Gives us the total value of the sum of all the values in the tree
 * Parameters: None
 * Returns: The total value of the sum of all values in the tree
 */
int BinarySearchTree::count_total() const
{
        return count_total(root);
}

/*
 * Purpose: Sums all values that have been inserted into the tree
 * Parameters: A node from the tree to be added up
 * Returns: The total value of the sum of all values in the tree
 */
int BinarySearchTree::count_total(Node *node) const
{
        // if the tree is empty there is nothing to add
        if (node == nullptr) {
            return 0;
        }
        // account for duplicates
        int adjustedSum = node->data * node->count;
        // recursively increment the sum
        return adjustedSum + count_total(node->left) + count_total(node->right);
}

/*
 * Purpose: Finds the parent of a node
 * Parameters: A node and its children
 * Returns: The parent node of the node given
 */
BinarySearchTree::Node *BinarySearchTree::find_parent(Node *node,
                                                      Node *child) const
{
        // if the node does not exist there is no specific parent
        if (node == nullptr)
                return nullptr;
        // if either the left or right is equal to the child,
        // we have found the parent
        if (node->left == child or node->right == child) {
                return node;
        }
        // Use the binary search tree invariant to walk the tree
        if (child->data > node->data) {
                return find_parent(node->right, child);
        } else {
                return find_parent(node->left, child);
        }
}

// use the printPretty helper to make the tree look nice
void BinarySearchTree::print_tree() const
{
        size_t      numLayers  = tree_height() + 1;
        size_t      levelWidth = 4;
        const char *rootPrefix = "-> ";

        // Need numLayers * levelWidth characters for each layer of tree.
        // Add an extra levelWidth characters at front to avoid if statement
        // 1 extra char for nul character at end
        char *start = new char[(numLayers + 1) * levelWidth + 1];

        print_tree(root, start + levelWidth, start + levelWidth, rootPrefix);
        delete[] start;
}

// Logic and Output Reference: 
// https://www.techiedelight.com/c-program-print-binary-tree/
void BinarySearchTree::print_tree(Node *node, char *const currPos,
                                  const char *const fullLine,
                                  const char *const branch) const
{
        if (node == nullptr)
                return;

        // 4 characters + 1 for nul terminator
        using TreeLevel                    = char[5];
        static const int       levelLength = sizeof(TreeLevel) - 1;
        static const TreeLevel UP = ".-- ", DOWN = "`-- ", EMPTY = "    ",
                               CONNECT = "   |";
        // Copies prev into dest and advances dest by strlen(prev)
        auto set = [](char *dest, const char *prev) {
                size_t p = strlen(prev);
                return (char *)memcpy(dest, prev, p) + p;
        };

        print_tree(node->right, set(currPos, EMPTY), fullLine, UP);

        // Clear any characters that would immediate precede the "branch"
        // Don't need to check for root (i.e start of array),fullLine is padded
        set(currPos - levelLength, EMPTY);

        // Terminate fullLine at current level
        *currPos = '\0';

        std::cerr << fullLine << branch << node->data
                  << (node->count > 1 ? "*" : "") << endl;

        // Connect upper branch to parent
        if (branch == UP)
                set(currPos - levelLength, CONNECT);

        // Connect lower branch to parent
        print_tree(node->left, set(currPos, CONNECT), fullLine, DOWN);
}
