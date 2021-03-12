/**
 * Copyright © 2021 Al Timofeyev. All rights reserved.
 * @file    BinaryTree.hpp
 *
 * @author  Al Timofeyev
 * @date    January 29, 2021
 * @brief   A generic Binary Tree data structure.
 *
 * Version: 1.0
 * Modified By:
 * Modified Date:
 * *********************************************************/

#ifndef BinaryTree_hpp
#define BinaryTree_hpp

#include <queue>
#include <stack>
#include <iostream>

/**
 * @struct  Node
 * @brief   The Node struct is meant to hold the element and pointers to the left and right child elements.
 *
 * @tparam T    Any data type or class.
 */
template <typename T>
struct Node
{
    // ------------- FIELDS -------------
    T element;      /**< The element. */
    Node<T>* left;  /**< Pointer to the left child node in the tree. */
    Node<T>* right; /**< Pointer to the right child node in the tree. */
    
    // ---------- CONSTRUCTORS ----------
    /** Copy Constructor. */
    Node(const T& element) : element(element), left(nullptr), right(nullptr) {}
    /** Move Constructor. */
    Node(T&& element) : element(std::forward<T>(element)), left(nullptr), right(nullptr) {}
    /** Struct Destructor. */
    ~Node()
    {
        delete left;
        left = nullptr;
        delete right;
        right = nullptr;
    }
    
    // ----- NON-MEMEBER OPERATORS ------
    friend std::ostream& operator<<(std::ostream& output, const Node<T>& node)
    {
        if (node.left && node.right)
            return output << *node.left << ", " << node.element << ", " << *node.right;
        else if(node.left)
            return output << *node.left << ", " << node.element;
        else if(node.right)
            return output << node.element << ", " << *node.right;
        else
            return output << node.element;
    }
};

/**
 * @class   BinaryTree
 * @brief   A generic Binary Tree class.
 * @details This Binary Tree is templated to use any data type or class (typename T).
 * @tparam T    Any data type or class.
 *
 * @note    Any class or data type used with this tree class MUST implement its own
 *          Relational Operators (<, >, <=, >=, ==, !=) in order for this tree class to
 *          work correctly. All primitive data types (int, float, double, char, string, bool)
 *          already have this operator functionality, so no implementation for them
 *          is needed. But any custom class that is used with this tree class NEEDS
 *          to implement its own Relational Operators.
 */
template <typename T>
class BinaryTree
{
public:
    // ---------- CONSTRUCTORS ----------
    BinaryTree();
    BinaryTree(const BinaryTree<T>& copyTree);
    BinaryTree(BinaryTree<T>&& moveTree);
    ~BinaryTree();
    
    // ----------- FUNCTIONS ------------
    void insert(const T element);
    bool bfsearch(const T& element);
    bool dfsearch(const T& element);
    void remove(const T element);
    void clear();
    int size();
    bool empty();
    
    int depth(const T& element);
    int height(const T& element);
    void invertTree();
    
    void printInorder();
    void printPreorder();
    void printPostorder();
    
    // ----------- OPERATORS ------------
    BinaryTree<T>& operator=(const BinaryTree& copyTree);
    BinaryTree<T>& operator=(BinaryTree&& moveTree);
    
    // ----- NON-MEMEBER OPERATORS ------
    template <typename Type>
    friend std::ostream& operator<<(std::ostream& output, const BinaryTree<Type>& tree);
    
private:
    // ------------- FIELDS -------------
    Node<T>* root;  /**< The root of the tree. */
    int treeSize;   /**< The size of the tree. */
    
    // ----------- FUNCTIONS ------------
    int getDepth(const Node<T>* current, const T& element, int depth);
    int getHeight(const Node<T>* current, const T& element, int height, bool elementFound);
    void invert(Node<T>* invertNode);
    void print(const Node<T>* node, const char& printOrder);
    void swap(BinaryTree<T>& otherTree);
};

#endif /* BinaryTree_hpp */


// **************************************************************************
// **************************************************************************
// ****!!!! IMPLEMENTATION !!!! IMPLEMENTATION !!!! IMPLEMENTATION !!!! *****
// **************************************************************************
// **************************************************************************

// ------------------------------------------------------
// -------------------- CONSTRUCTORS --------------------
// ------------------------------------------------------
/**
 * @brief   Default Constructor.
 *
 * @tparam T    Any data type or class.
 *
 * @details Initializes this tree object with a nullptr root and size of 0;
 */
template <typename T>
BinaryTree<T>::BinaryTree() : root(nullptr), treeSize(0) {}

/**
 * @brief   Copy Constructor.
 *
 * @tparam T        Any data type or class.
 * @param copyTree  The tree whose contents will be copied into this tree object.
 */
template <typename T>
BinaryTree<T>::BinaryTree(const BinaryTree<T>& copyTree) : root(nullptr), treeSize(0)
{
    Node<T>** currentNode;
    std::queue<Node<T>**> treeQueue;
    treeQueue.push(&root);
    
    Node<T>* copyNode;
    std::queue<Node<T>*> copyTreeQueue;
    copyTreeQueue.push(copyTree.root);
    
    while(!copyTreeQueue.empty())
    {
        currentNode = treeQueue.front();
        treeQueue.pop();
        
        copyNode = copyTreeQueue.front();
        copyTreeQueue.pop ();
        
        (*currentNode) = new Node<T>(copyNode->element);
        
        if(copyNode->left != nullptr)
        {
            treeQueue.push(&(*currentNode)->left);
            copyTreeQueue.push(copyNode->left);
        }
        if(copyNode->right != nullptr)
        {
            treeQueue.push(&(*currentNode)->right);
            copyTreeQueue.push(copyNode->right);
        }
        
        treeSize++;
    }
}

/**
 * @brief   Move Constructor.
 *
 * @tparam T        Any data type or class.
 * @param moveTree  The tree whose contents will be moved into this tree object.
 *
 * @note std::move() needs to be used to call this constructor.
 */
template <typename T>
BinaryTree<T>::BinaryTree(BinaryTree<T>&& moveTree) : root(nullptr), treeSize(0)
{
    Node<T>** currentNode;
    std::queue<Node<T>**> treeQueue;
    treeQueue.push(&root);
    
    Node<T>* moveNode;
    std::queue<Node<T>*> moveTreeQueue;
    moveTreeQueue.push(moveTree.root);
    
    while(!moveTreeQueue.empty())
    {
        currentNode = treeQueue.front();
        treeQueue.pop();
        
        moveNode = moveTreeQueue.front();
        moveTreeQueue.pop ();
        
        (*currentNode) = new Node<T>(std::move(moveNode->element));
        
        if(moveNode->left != nullptr)
        {
            treeQueue.push(&(*currentNode)->left);
            moveTreeQueue.push(moveNode->left);
        }
        if(moveNode->right != nullptr)
        {
            treeQueue.push(&(*currentNode)->right);
            moveTreeQueue.push(moveNode->right);
        }
        
        treeSize++;
    }
    moveTree.clear();
}

/**
 * @brief   Class Destructor.
 *
 * @tparam T    Any data type or class.
 *
 * @details Recursively clears the tree using clear() function.
 */
template <typename T>
BinaryTree<T>::~BinaryTree()
{
    clear();
}


// ------------------------------------------------------
// --------------------- FUNCTIONS ----------------------
// ------------------------------------------------------
/**
 * @brief   Inserts element into the tree in level order, at first available position.
 *
 * @tparam T        Any data type or class.
 * @param element   The element you want to add to the tree.
 *
 * @details Breadth First insertion. No duplicates allowed.
 */
template <typename T>
void BinaryTree<T>::insert(const T element)
{
    if(root == nullptr)
        root = new Node<T>(element);
    else if(root->element == element)   // Make sure root isn't a duplicate of element.
        return;
    else
    {
        std::queue<Node<T>*> treeQueue;
        treeQueue.push(root);
        Node<T>* currentNode;
        
        while(!treeQueue.empty())
        {
            currentNode = treeQueue.front();
            treeQueue.pop();
            
            if(currentNode->left != nullptr)
            {
                if(currentNode->left->element == element)  // Make sure left child isn't a duplicate of element.
                    return;
                else
                    treeQueue.push(currentNode->left);
            }
            else
            {
                currentNode->left = new Node<T>(element);
                break;
            }
            if(currentNode->right != nullptr)
            {
                if(currentNode->right->element == element) // Make sure right child isn't a duplicate of element.
                    return;
                else
                    treeQueue.push(currentNode->right);
            }
            else
            {
                currentNode->right = new Node<T>(element);
                break;
            }
        }
    }
    
    treeSize++;
}

/**
 * @brief   Breadth First Search.
 *
 * @tparam T        Any data type or class.
 * @param element   The element being searched for in the tree.
 * @return          A boolean flag.
 *
 * @details Returns true if the element is in the tree, otherwise returns false.
 */
template <typename T>
bool BinaryTree<T>::bfsearch(const T& element)
{
    if(root == nullptr)
        return false;
    else
    {
        std::queue<Node<T>*> treeQueue;
        treeQueue.push(root);
        Node<T>* currentNode;
        
        while(!treeQueue.empty())
        {
            currentNode = treeQueue.front();
            treeQueue.pop();
            
            if(currentNode->element == element)
                return true;
            else
            {
                if(currentNode->left != nullptr)
                    treeQueue.push(currentNode->left);
                if(currentNode->right != nullptr)
                    treeQueue.push(currentNode->right);
            }
        }
    }
    
    return false;
}

/**
 * @brief   Depth First Search.
 *
 * @tparam T        Any data type or class.
 * @param element   The element being searched for in the tree.
 * @return          A boolean flag.
 *
 * @details Returns true if the element is in the tree, otherwise returns false.
 *
 * @note    This Depth First Search uses inorder traversal.
 */
template <typename T>
bool BinaryTree<T>::dfsearch(const T& element)
{
    if(root == nullptr)
        return false;
    else
    {
        std::stack<Node<T>*> treeStack;
        treeStack.push(root);
        Node<T>* currentNode = root->left;
        
        while(!treeStack.empty() || currentNode != nullptr)
        {
            if(currentNode != nullptr)
            {
                treeStack.push(currentNode);
                currentNode = currentNode->left;
                continue;
            }
            
            currentNode = treeStack.top();
            treeStack.pop();
            
            if(currentNode->element == element)
                return true;
            
            currentNode = currentNode->right;
        }
    }
    
    return false;
}

/**
 * @brief   Removes the specified element from the tree.
 *
 * @tparam T        Any data type or class.
 * @param element   The element to be removed from the tree.
 */
template <typename T>
void BinaryTree<T>::remove(const T element)
{
    if(root == nullptr)
        return;
    
    std::queue<Node<T>*> treeQueue;
    treeQueue.push(root);
    Node<T>* currentNode;
    Node<T>* deepestParent = nullptr;
    Node<T>* deleteNode = nullptr;
    
    while(!treeQueue.empty())
    {
        currentNode = treeQueue.front();
        treeQueue.pop();
        
        if(currentNode->element == element) // If we found the element we want to delete.
            deleteNode = currentNode;
        
        // Deepest parent can only be found as long as currentNode has at least one child.
        if(currentNode->left != nullptr || currentNode->right != nullptr)
            deepestParent = currentNode;
        
        if(currentNode->left != nullptr)
            treeQueue.push(currentNode->left);
        if(currentNode->right != nullptr)
            treeQueue.push(currentNode->right);
    }
    
    if(deleteNode == nullptr)           // If element doesn't exist in the tree.
        return;
    else if(deepestParent == nullptr)   // If root is only value in the tree.
    {
        clear();
        return;
    }
    
    // Otherwise overwrite the deleteNode element with deepest element in tree.
    if(deepestParent->right != nullptr) // If deepestParent has a right child.
    {
        deleteNode->element = deepestParent->right->element;
        deleteNode = deepestParent->right;
        deepestParent->right = nullptr;
    }
    else    // Else if deepestParent only has a left child.
    {
        deleteNode->element = deepestParent->left->element;
        deleteNode = deepestParent->left;
        deepestParent->left = nullptr;
    }
    
    delete deleteNode;
    treeSize--;
}

/**
 * @brief   Clears the entire tree recursively and resets all field elements to default.
 *
 * @tparam T    Any data type or class.
 */
template <typename T>
void BinaryTree<T>::clear()
{
    delete root;
    root = nullptr;
    treeSize = 0;
}

/**
 * @brief   Returns the size of/number of nodes in the tree.
 *
 * @tparam T    Any data type or class.
 * @return      The size of the tree.
 */
template <typename T>
int BinaryTree<T>::size()
{
    return treeSize;
}

/**
 * @brief   Returns true if the tree is empty and false otherwise.
 *
 * @tparam T    Any data type or class.
 * @return      A boolean flag.
 */
template <typename T>
bool BinaryTree<T>::empty()
{
    return treeSize == 0;
}

/**
 * @brief   Returns depth of the specified element in tree.
 *
 * @tparam T        Any data type or class.
 * @param element   The element whose depth we are calculating.
 * @return          Depth of element.
 */
template <typename T>
int BinaryTree<T>::depth(const T& element)
{
    if(root == nullptr) // Tree is empty.
        return -1;
    
    if(root->element == element)
        return 0;
    
    int leftDepth = getDepth(root->left, element, 1);
    int rightDepth = getDepth(root->right, element, 1);
    
    return (leftDepth < rightDepth) ? rightDepth : leftDepth;
}

/**
 * @brief   Returns height of the specified element in tree.
 *
 * @tparam T        Any data type or class.
 * @param element   The element whose height we are calculating.
 * @return          Height of element.
 */
template <typename T>
int BinaryTree<T>::height(const T& element)
{
    if(root == nullptr) // Tree is empty.
        return -1;
    
    return getHeight(root, element, 0, false);
}

/**
 * @brief   Recursively inverts the tree.
 *
 * @tparam T    Any data type or class.
 */
template <typename T>
void BinaryTree<T>::invertTree()
{
    invert(root);
}

/**
 * @brief   Prints tree Inorder.
 *
 * @tparam T    Any data type or class.
 */
template <typename T>
void BinaryTree<T>::printInorder()
{
    if(root == nullptr)
    {
        std::cout << "()" << std::endl;
        return;
    }
    
    std::cout << "[root: " << root->element << "]\t" << "(";
    print(root, 'i');
    std::cout << ")" << std::endl;
}

/**
 * @brief   Prints tree Preorder.
 *
 * @tparam T    Any data type or class.
 */
template <typename T>
void BinaryTree<T>::printPreorder()
{
    if(root == nullptr)
    {
        std::cout << "()" << std::endl;
        return;
    }
    
    std::cout << "[root: " << root->element << "]\t" << "(";
    print(root, 'r');
    std::cout << ")" << std::endl;
}

/**
 * @brief   Prints tree Postorder.
 *
 * @tparam T    Any data type or class.
 */
template <typename T>
void BinaryTree<T>::printPostorder()
{
    if(root == nullptr)
    {
        std::cout << "()" << std::endl;
        return;
    }
    
    std::cout << "[root: " << root->element << "]\t" << "(";
    print(root, 'o');
    std::cout << ")" << std::endl;
}

/**
 * @brief   Calculates the depth of an element in tree.
 *
 * @tparam T        Any data type or class.
 * @param current   The current Node we are looking at in tree.
 * @param element   The element whose depth we are calculating.
 * @param depth     The depth of the current node.
 * @return          Depth of element.
 */
template <typename T>
int BinaryTree<T>::getDepth(const Node<T>* current, const T& element, int depth)
{
    if(current == nullptr)  // Element was not found.
        return -1;
    else if(current->element == element)
        return depth;
    
    int leftDepth = getDepth(current->left, element, depth+1);
    int rightDepth = getDepth(current->right, element, depth+1);
    
    return (leftDepth < rightDepth) ? rightDepth : leftDepth;
}

/**
 * @brief   Calculates the height of an element in tree.
 *
 * @tparam T            Any data type or class.
 * @param current       The current Node we are looking at in tree.
 * @param element       The element whose height we are calculating.
 * @param height        The height of the current node.
 * @param elementFound  True if the element was found in the tree, otherwise false.
 * @return              Height of element.
 */
template <typename T>
int BinaryTree<T>::getHeight(const Node<T>* current, const T& element, int height, bool elementFound)
{
    if(current == nullptr && !elementFound) // Element was not found in tree/subtree.
        return -1;
    else if(current == nullptr && elementFound)
        return height;
    
    if(elementFound)
        height++;
    else if(current->element == element)
        elementFound = true;
    
    int leftHeight = getHeight(current->left, element, height, elementFound);
    int rightHeight = getHeight(current->right, element, height, elementFound);
    
    return (leftHeight < rightHeight) ? rightHeight : leftHeight;
}

/**
 * @brief   Inverts all the left and right children of a node in the tree.
 *
 * @tparam T            Any data type or class.
 * @param invertNode    The node whose children will be inverted.
 */
template <typename T>
void BinaryTree<T>::invert(Node<T>* invertNode)
{
    if(invertNode == nullptr)
        return;
    
    Node<T>* temp = invertNode->left;
    invertNode->left = invertNode->right;
    invertNode->right = temp;
    
    invert(invertNode->left);
    invert(invertNode->right);
}

/**
 * @brief   Prints the binary tree.
 *
 * @tparam T            Any data type or class.
 * @param node          The root element in the tree.
 * @param printOrder    The order in which to print the tree (In-, Pre-, Post- order).
 */
template <typename T>
void BinaryTree<T>::print(const Node<T>* node, const char& printOrder)
{
    if(node == nullptr)
        return;
    
    switch (printOrder)
    {
        case 'i':   // Inorder
            print(node->left, printOrder);
            if (node->left && node->right)
                std::cout << ", " << node->element << ", ";
            else if(node->left)
                std::cout << ", " << node->element;
            else if(node->right)
                std::cout << node->element << ", ";
            else
                std::cout << node->element;
            print(node->right, printOrder);
            break;
        case 'r':   // Preorder
            std::cout << node->element;
            if (node->left && node->right)
            {
                std::cout << ", ";
                print(node->left, printOrder);
                std::cout << ", ";
                print(node->right, printOrder);
            }
            else if(node->left)
            {
                std::cout << ", ";
                print(node->left, printOrder);
            }
            else if(node->right)
            {
                std::cout  << ", ";
                print(node->right, printOrder);
            }
            break;
        case 'o':   // Postorder
            if (node->left && node->right)
            {
                print(node->left, printOrder);
                std::cout << ", ";
                print(node->right, printOrder);
                std::cout << ", ";
            }
            else if(node->left)
            {
                print(node->left, printOrder);
                std::cout << ", ";
            }
            else if(node->right)
            {
                std::cout  << ", ";
                print(node->right, printOrder);
            }
            std::cout << node->element;
            break;
    }
}

/**
 * @brief   Swaps Trees.
 *
 * @tparam T    Any data type or class.
 * @param other The other tree with which to swap elements.
 */
template <typename T>
void BinaryTree<T>::swap(BinaryTree<T>& other)
{
    Node<T>* tempRoot = root;
    root = other.root;
    other.root = tempRoot;
    
    int tempSize = treeSize;
    treeSize = other.treeSize;
    other.treeSize = tempSize;
}


// ------------------------------------------------------
// --------------------- OPERATORS ----------------------
// ------------------------------------------------------
/**
 * @brief   Copy assignment operator.
 *
 * @tparam T        Any data type or class.
 * @param copyTree  The tree object from which to copy elements.
 * @return          A reference to a copied tree object.
 *
 * @details Copies a tree with the help of the copy constructor and a custom swap function.
 */
template <typename T>
BinaryTree<T>& BinaryTree<T>::operator=(const BinaryTree& copyTree)
{
    BinaryTree<T> tempTree(copyTree);
    tempTree.swap(*this);
    return *this;
}

/**
 * @brief   Move assignment operator.
 *
 * @tparam T        Any data type or class.
 * @param moveTree  The tree object from which to move elements.
 * @return          A reference to a moved tree object.
 *
 * @details Moves tree elements from the provided tree into this tree object.
 *          The provided tree object is empty after the move is complete.
 *
 * @note    std::move() needs to be used to call this operator.
 */
template <typename T>
BinaryTree<T>& BinaryTree<T>::operator=(BinaryTree&& moveTree)
{
    if(this == &moveTree)   // Make sure this and moveTree are not the same object.
        return *this;
    
    this->clear();
    moveTree.swap(*this);
    return *this;
}


// ------------------------------------------------------
// --------------- NON-MEMEBER OPERATORS ----------------
// ------------------------------------------------------
/**
 * @brief   Output stream operator.
 *
 * @tparam T        Any data type or class.
 * @param output    The output stream (usually std::cout).
 * @param tree      The tree object that will be printed.
 *
 * @details Prints the tree elements to the specified output stream using inorder traversal.
 *
 * @note    Any class or data type used with this tree class MUST implement its own
 *          operator<< in order for this operator<< to work correctly. All primitive
 *          data types (int, float, double, char, string, bool) already have this operator
 *          functionality so no implementation for them is needed. But any custom class that
 *          is used with this tree class NEEDS to implement its own operator<<.
 */
template <typename T>
std::ostream& operator<<(std::ostream& output, const BinaryTree<T>& tree)
{
    if (tree.root)
        return output << "[root: " << tree.root->element << "]\t" << "(" << *tree.root << ")";
    else
        return output << "()";
}
