/**
 * Copyright © 2021 Al Timofeyev. All rights reserved.
 * @file    Stack.hpp
 *
 * @author  Al Timofeyev
 * @date    January 18, 2021
 * @brief   A generic Stack data structure.
 *
 * Version: 1.0
 * Modified By:
 * Modified Date:
 * *********************************************************/

#ifndef Stack_hpp
#define Stack_hpp

#include <iostream>
#include <stdexcept>

/**
 * @struct  Node
 * @brief   The Node struct is meant to hold the data and pointer to the previous stack element.
 *
 * @tparam T    Any data type or class.
 */
template <typename T>
struct Node
{
    // ------------- FIELDS -------------
    T data;             /**< The data. */
    Node<T>* previous;  /**< Pointer to the previous node in the stack. */
    
    // ---------- CONSTRUCTORS ----------
    /** Copy Constructor. */
    Node(const T& data) : data(data), previous(nullptr) {}
    /** Move Constructor. */
    Node(T&& data) : data(std::forward<T>(data)), previous(nullptr) {}
    /** Struct Destructor. */
    ~Node()
    {
        delete previous;
        previous = nullptr;
    }
    
    // ----- NON-MEMEBER OPERATORS ------
    friend std::ostream& operator<<(std::ostream& output, const Node<T>& node)
    {
        if(node.previous)
            return output << *node.previous << ", " << node.data;
        else
            return output << node.data;
    }
};

/**
 * @class   Stack
 * @brief   A generic Stack class.
 * @details This Stack class is templated to use any data type or class.
 * @tparam T    Any data type or class.
 */
template <typename T>
class Stack
{
public:
    // ---------- CONSTRUCTORS ----------
    Stack();
    Stack(const Stack<T>& copyStack);
    Stack(Stack<T>&& moveStack);
    ~Stack();
    
    // ----------- FUNCTIONS ------------
    void push(const T data);
    T pop();
    T peek();
    int size();
    bool empty();
    void clear();
    
    // ----------- OPERATORS ------------
    Stack<T>& operator=(const Stack& copyStack);
    Stack<T>& operator=(Stack&& moveStack);
    
    // ----- NON-MEMEBER OPERATORS ------
    template <typename Type>
    friend std::ostream& operator<<(std::ostream& output, const Stack<Type>& stack);
    
private:
    // ------------- FIELDS -------------
    Node<T>* top;   /**< The top of the stack. */
    int stackSize;  /**< The size of the stack. */
    
    // ----------- FUNCTIONS ------------
    void swap(Stack<T>& other);
};

#endif /* Stack_hpp */

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
 * @details Initializes this stack object with a nullptr top and size of 0;
 */
template <typename T>
Stack<T>::Stack() : top(nullptr), stackSize(0) {}

/**
 * @brief   Copy Constructor.
 *
 * @tparam T        Any data type or class.
 * @param copyStack The stack whose elements will be copied into this stack object.
 */
template <typename T>
Stack<T>::Stack(const Stack<T>& copyStack) : top(nullptr), stackSize(0)
{
    Node<T>** currentNode = &top;
    for(Node<T>* copyNode = copyStack.top; copyNode; copyNode = copyNode->previous)
    {
        (*currentNode) = new Node<T>(copyNode->data);
        
        currentNode = &(*currentNode)->previous;
        stackSize++;
    }
}

/**
 * @brief   Move Constructor.
 *
 * @tparam T        Any data type or class.
 * @param moveStack The stack whose elements will be moved into this stack object.
 *
 * @note std::move() needs to be used to call this constructor.
 */
template <typename T>
Stack<T>::Stack(Stack<T>&& moveStack) : top(nullptr), stackSize(0)
{
    Node<T>** currentNode = &top;
    for(Node<T>* moveNode = moveStack.top; moveNode; moveNode = moveNode->previous)
    {
        (*currentNode) = new Node<T>(std::move(moveNode->data));
        
        currentNode = &(*currentNode)->previous;
        stackSize++;
    }
    moveStack.clear();
}

/**
 * @brief   Class Destructor.
 *
 * @tparam T    Any data type or class.
 *
 * @details Recursively clears the stack using clear() function.
 */
template <typename T>
Stack<T>::~Stack()
{
    clear();
}

// ------------------------------------------------------
// --------------------- FUNCTIONS ----------------------
// ------------------------------------------------------
/**
 * @brief   Adds element to the top of the stack.
 *
 * @tparam T    Any data type or class.
 * @param data  The element you want to add to the stack.
 */
template <typename T>
void Stack<T>::push(const T data)
{
    Node<T>* node_newTop = new Node<T>(data);
    node_newTop->previous = top;
    top = node_newTop;
    stackSize++;
}

/**
 * @brief   Removes and returns the top of the stack.
 *
 * @tparam T    Any data type or class.
 * @return      The removed element.
 *
 * @throw   std::underflow_error
 * @warning Throws an Underflow Error exception if the stack is empty when function is called.
 */
template <typename T>
T Stack<T>::pop()
{
    if(top == nullptr)
        throw std::underflow_error("Stack is Empty");
    
    Node<T>* deleteTop = top;
    T popped_data = top->data;
    top = top->previous;
    
    deleteTop->previous = nullptr;
    delete deleteTop;
    stackSize--;
    
    return popped_data;
}

/**
 * @brief   Returns, but does not remove, the top of the stack.
 *
 * @tparam T    Any data type or class.
 * @return      The element located at the top of the stack.
 *
 * @throw   std::underflow_error
 * @warning Throws an Underflow Error exception if the stack is empty when function is called.
 */
template <typename T>
T Stack<T>::peek()
{
    if(top == nullptr)
        throw std::underflow_error("Stack is Empty");
    
    return top->data;
}

/**
 * @brief   Returns the size of the stack.
 *
 * @tparam T    Any data type or class.
 * @return      The size of the stack.
 */
template <typename T>
int Stack<T>::size()
{
    return stackSize;
}

/**
 * @brief   Returns true if the stack is empty and false otherwise.
 *
 * @tparam T    Any data type or class.
 * @return      A boolean flag.
 */
template <typename T>
bool Stack<T>::empty()
{
    return stackSize == 0;
}

/**
 * @brief   Clears the entire stack recursively and resets all field elements to default.
 *
 * @tparam T    Any data type or class.
 */
template <typename T>
void Stack<T>::clear()
{
    delete top;
    top = nullptr;
    stackSize = 0;
}

/**
 * @brief   Swaps Stacks.
 *
 * @tparam T    Any data type or class.
 * @param other The other stack object with which to swap elements.
 */
template <typename T>
void Stack<T>::swap(Stack<T>& other)
{
    Node<T>* tempTop = top;
    top = other.top;
    other.top = tempTop;
    
    int tempSize = stackSize;
    stackSize = other.stackSize;
    other.stackSize = tempSize;
}

// ------------------------------------------------------
// --------------------- OPERATORS ----------------------
// ------------------------------------------------------
/**
 * @brief   Copy assignment operator.
 *
 * @tparam T        Any data type or class.
 * @param copyStack The stack object from which to copy elements.
 * @return          A reference to a copied stack object.
 *
 * @details Copies a stack with the help of the copy constructor and a custom swap function.
 */
template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& copyStack)
{
    Stack<T> tempStack(copyStack);
    tempStack.swap(*this);
    return *this;
}

/**
 * @brief   Move assignment operator.
 *
 * @tparam T        Any data type or class.
 * @param moveStack The stack object from which to move elements.
 * @return          A reference to a moved stack object.
 *
 * @details Moves stack elements from the provided stack into this stack object.
 *          The provided stack object is empty after the move is complete.
 *
 * @note    std::move() needs to be used to call this operator.
 */
template <typename T>
Stack<T>& Stack<T>::operator=(Stack&& moveStack)
{
    if(this == &moveStack)  // Make sure this and moveStack are not the same object.
        return *this;
    
    this->clear();
    moveStack.swap(*this);
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
 * @param stack     The stack object that will be printed.
 *
 * @details Prints the stack elements to the specified output stream. Prints starting from the
 *          bottom of the stack and finishes printing at the top of the stack (BOTTOM, ... , TOP).
 *
 * @note    Any class or data type used with this stack class MUST implement its
 *          own operator<< in order for this operator<< to work correctly. All primitive
 *          data types (int, float, double, char, string, bool) already have this operator
 *          functionality so no implementation for them is needed. But any custom class that
 *          is used with this stack class NEEDS to implement its own operator<<.
 */
template <typename T>
std::ostream& operator<<(std::ostream& output, const Stack<T>& stack)
{
    if (stack.top)
        return output << "(" << *stack.top << ")";
    else
        return output << "()";
}
