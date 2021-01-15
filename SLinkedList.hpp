/**
 * Copyright © 2020 Al Timofeyev. All rights reserved.
 * @file    SLinkedList.hpp
 *
 * @author  Al Timofeyev
 * @date    December 23, 2020
 * @brief   A generic Singly Linked List data structure.
 *
 * Version: 1.0
 * Modified By:
 * Modified Date:
 * *********************************************************/

#ifndef SLinkedList_hpp
#define SLinkedList_hpp

#include <iostream>
#include <stdexcept>

/**
 * @struct  Node
 * @brief   The Node struct is meant to hold the data and pointer to the next list element.
 *
 * @tparam T    Any data type or class.
 */
template <typename T>
struct Node
{
    // ------------- FIELDS -------------
    T data;         /**< The data. */
    Node<T>* next;  /**< Pointer to the next node in the list. */
    
    // ---------- CONSTRUCTORS ----------
    /** Copy Constructor. */
    Node(const T& data) : data(data), next(nullptr) {}
    /** Move Constructor. */
    Node(T&& data) : data(std::forward<T>(data)), next(nullptr) {}
    /** Struct Destructor. */
    ~Node()
    {
        delete next;
        next = nullptr;
    }
    
    // ----- NON-MEMEBER OPERATORS ------
    friend std::ostream& operator<<(std::ostream& output, const Node<T>& node)
    {
        if(node.next)
            return output << node.data << ", " << *node.next;
        else
            return output << node.data;
    }
};

/**
 * @class   SLinkedList
 * @brief   A generic Singly Linked List class.
 * @details This Singly Linked List is templated to use any data type or class.
 * @tparam T    Any data type or class.
 */
template <typename T>
class SLinkedList
{
public:
    // ---------- CONSTRUCTORS ----------
    SLinkedList();
    SLinkedList(const SLinkedList<T>& copyList);
    SLinkedList(SLinkedList<T>&& moveList);
    ~SLinkedList();
    
    // ----------- FUNCTIONS ------------
    void addFirst(const T data);
    void addLast(const T data);
    void insert(const T data, const int index);
    void remove(const int index);
    void clear();
    T pop();
    T pop(const int index);
    T peek();
    T peek(const int index);
    int size();
    bool empty();
    
    // ----------- OPERATORS ------------
    T& operator[](const int index);
    bool operator==(const SLinkedList& compareList);
    SLinkedList<T>& operator=(const SLinkedList& copyList);
    SLinkedList<T>& operator=(SLinkedList&& moveList);
    
    // ----- NON-MEMEBER OPERATORS ------
    template <typename Type>
    friend std::ostream& operator<<(std::ostream& output, const SLinkedList<Type>& list);
    
private:
    // ------------- FIELDS -------------
    Node<T>* head;  /**< The head of the list. */
    Node<T>* tail;  /**< The tail of the lsit. */
    int listSize;   /**< The size of the list. */
    
    // ----------- FUNCTIONS ------------
    void swap(SLinkedList<T>& other);
};

#endif /* SLinkedList_hpp */


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
 * @details Initializes this linked list object with a nullptr head and tail and size of 0;
 */
template <typename T>
SLinkedList<T>::SLinkedList() : head(nullptr), tail(nullptr), listSize(0) {}

/**
 * @brief   Copy Constructor.
 *
 * @tparam T        Any data type or class.
 * @param copyList  The list whose contents will be copied into this linked list object.
 */
template <typename T>
SLinkedList<T>::SLinkedList(const SLinkedList<T>& copyList) : head(nullptr), tail(nullptr), listSize(0)
{
    Node<T>** currentNode = &head;
    for(Node<T>* copyNode = copyList.head; copyNode; copyNode = copyNode->next)
    {
        (*currentNode) = new Node<T>(copyNode->data);
        
        if(copyNode->next == nullptr)
            tail = *currentNode;
        
        currentNode = &(*currentNode)->next;
        listSize++;
    }
}

/**
 * @brief   Move Constructor.
 *
 * @tparam T        Any data type or class.
 * @param moveList  The list whose contents will be moved into this linked list object.
 *
 * @note std::move() needs to be used to call this constructor.
 */
template <typename T>
SLinkedList<T>::SLinkedList(SLinkedList<T>&& moveList) : head(nullptr), tail(nullptr), listSize(0)
{
    Node<T>** currentNode = &head;
    for(Node<T>* moveNode = moveList.head; moveNode; moveNode = moveNode->next)
    {
        (*currentNode) = new Node<T>(std::move(moveNode->data));
        
        if(moveNode->next == nullptr)
            tail = *currentNode;
        
        currentNode = &(*currentNode)->next;
        listSize++;
    }

    moveList.clear();
}

/**
 * @brief   Class Destructor.
 *
 * @tparam T    Any data type or class.
 *
 * @details Recursively clears the list using clear() function.
 */
template <typename T>
SLinkedList<T>::~SLinkedList()
{
    clear();
}


// ------------------------------------------------------
// --------------------- FUNCTIONS ----------------------
// ------------------------------------------------------
/**
 * @brief   Adds data to the front of the list.
 *
 * @tparam T    Any data type or class.
 * @param data  The data you want to add to the list.
 */
template <typename T>
void SLinkedList<T>::addFirst(const T data)
{
    Node<T>* node_newHead = new Node<T>(data);
    if(head == nullptr)
        tail = node_newHead;
    else
        node_newHead->next = head;
    
    head = node_newHead;
    listSize++;
}

/**
 * @brief   Adds data to the end of the list.
 *
 * @tparam T    Any data type or class.
 * @param data  The data you want to add to the list.
 */
template <typename T>
void SLinkedList<T>::addLast(const T data)
{
    Node<T>* node_newTail = new Node<T>(data);
    if(head == nullptr)
        head = node_newTail;
    else
        tail->next = node_newTail;
    
    tail = node_newTail;
    listSize++;
}

/**
 * @brief   Inserts data into the list at the specified index.
 *
 * @tparam T    Any data type or class.
 * @param data  The data you want to add to the list.
 * @param index Index at which to add the data into the list.
 */
template <typename T>
void SLinkedList<T>::insert(const T data, const int index)
{
    if(index <= 0)
        addFirst(data);
    else if(index >= listSize)
        addLast(data);
    else
    {
        Node<T>* node = new Node<T>(data);
        Node<T>* temp = head;
        for(int i = 1; i < index; i++)
            temp = temp->next;
        
        node->next = temp->next;
        temp->next = node;
        listSize++;
    }
}

/**
 * @brief   Removes data from the list at the specified index.
 *
 * @tparam T    Any data type or class.
 * @param index Index at which to remove data from the list.
 */
template <typename T>
void SLinkedList<T>::remove(const int index)
{
    // Do nothing if list is empty or index out of range.
    if(listSize == 0 || index < 0 || index >= listSize)
        return;
    
    Node<T>* deleteNode = head;
    
    if(index == 0)  // Remove the head of list.
    {
        head = head->next;
        if(head == nullptr) // If list is empty, make sure tail is set to nullptr.
            tail = nullptr;
    }
    else if(index == listSize-1)    // Removing the tail of list.
    {
        Node<T>* temp = head;
        for(int i = 1; i < listSize-1; i++)
            temp = temp->next;
        
        deleteNode = temp->next;
        temp->next = deleteNode->next;
        tail = temp;
    }
    else
    {
        Node<T>* temp = head;
        for(int i = 1; i < index; i++)
            temp = temp->next;
        
        deleteNode = temp->next;
        temp->next = deleteNode->next;
    }
    
    deleteNode->next = nullptr;
    delete deleteNode;
    listSize--;
}

/**
 * @brief   Clears the entire list recursively and resets all field elements to default.
 *
 * @tparam T    Any data type or class.
 */
template <typename T>
void SLinkedList<T>::clear()
{
    delete head;
    head = nullptr;
    tail = nullptr;
    listSize = 0;
}

/**
 * @brief   Removes and returns the head of the list.
 *
 * @tparam T    Any data type or class.
 * @return      The removed data.
 *
 * @throw   std::out_of_range
 * @warning Throws an Out Of Range exception if the list is empty when function is called.
 */
template <typename T>
T SLinkedList<T>::pop()
{
    if(head == nullptr)
        throw std::out_of_range("Linked List is Empty");
    
    Node<T>* deleteNode = head;
    T popped_data = head->data;
    head = head->next;
    if(head == nullptr) // If list is empty, make sure tail is set to nullptr.
        tail = nullptr;
    
    deleteNode->next = nullptr;
    delete deleteNode;
    listSize--;
    
    return popped_data;
}

/**
 * @brief   Removes and returns data from the list at the specified index.
 *
 * @tparam T    Any data type or class.
 * @param index Index at which to retrieve and remove data from the list.
 * @return      The removed data.
 *
 * @throw   std::out_of_range
 * @warning Throws an Out Of Range exception if the list is empty or index is out of range when function is called.
 */
template <typename T>
T SLinkedList<T>::pop(const int index)
{
    if(head == nullptr)
        throw std::out_of_range("Linked List is Empty");
    else if(index < 0 || index >= listSize)
        throw std::out_of_range("Index is out of range.");
    
    Node<T>* deleteNode = head;
    
    if(index == 0)  // Pop the head of list.
    {
        head = head->next;
        if(head == nullptr) // If list is empty, make sure tail is set to nullptr.
            tail = nullptr;
    }
    else if(index == listSize-1)    // Pop the tail of list.
    {
        Node<T>* temp = head;
        for(int i = 1; i < listSize-1; i++)
            temp = temp->next;
        
        deleteNode = temp->next;
        temp->next = deleteNode->next;
        tail = temp;
    }
    else
    {
        Node<T>* temp = head;
        for(int i = 1; i < index; i++)
            temp = temp->next;
        
        deleteNode = temp->next;
        temp->next = deleteNode->next;
    }
    
    T popped_data = deleteNode->data;
    deleteNode->next = nullptr;
    delete deleteNode;
    listSize--;
    
    return popped_data;
}

/**
 * @brief   Returns, but does not remove, the head of the list.
 *
 * @tparam T    Any data type or class.
 * @return      The data located at the head of the list.
 *
 * @throw   std::out_of_range
 * @warning Throws an Out Of Range exception if the list is empty when function is called.
 */
template <typename T>
T SLinkedList<T>::peek()
{
    if(head == nullptr)
        throw std::out_of_range("Linked List is Empty");
    
    return head->data;
}

/**
 * @brief   Returns, but does not remove, data from the list at the specified index.
 *
 * @tparam T    Any data type or class.
 * @param index Index at which to retrieve data from the list.
 * @return      The data located at the specified index of the list.
 *
 * @throw   std::out_of_range
 * @warning Throws an Out Of Range exception if the list is empty or index is out of range when function is called.
 */
template <typename T>
T SLinkedList<T>::peek(const int index)
{
    if(head == nullptr)
        throw std::out_of_range("Linked List is Empty");
    else if(index < 0 || index >= listSize)
        throw std::out_of_range("Index is out of range.");
    else if(index == 0)
        return head->data;
    else if(index == listSize-1)
        return tail->data;
    else
    {
        Node<T>* temp = head;
        for(int i = 0; i < index; i++)
            temp = temp->next;
        return temp->data;
    }
}

/**
 * @brief   Returns the size of the list.
 *
 * @tparam T    Any data type or class.
 * @return      The size of the list.
 */
template <typename T>
int SLinkedList<T>::size()
{
    return listSize;
}

/**
 * @brief   Returns true if the list is empty and false otherwise.
 *
 * @tparam T    Any data type or class.
 * @return      A boolean flag.
 */
template <typename T>
bool SLinkedList<T>::empty()
{
    return listSize == 0;
}

/**
 * @brief   Swaps Linked Lists.
 *
 * @tparam T    Any data type or class.
 * @param other The other linked list with which to swap elements.
 */
template <typename T>
void SLinkedList<T>::swap(SLinkedList<T>& other)
{
    Node<T>* tempHead = head;
    Node<T>* tempTail = tail;
    head = other.head;
    tail = other.tail;
    other.head = tempHead;
    other.tail = tempTail;
    
    int tempSize = listSize;
    listSize = other.listSize;
    other.listSize = tempSize;
}


// ------------------------------------------------------
// --------------------- OPERATORS ----------------------
// ------------------------------------------------------
/**
 * @brief   Subscript operator.
 *
 * @tparam T    Any data type or class.
 * @param index Index at which to retrieve data reference from the list.
 * @return      The reference to the data located at the specified index in the list.
 *
 * @throw   std::out_of_range
 * @warning Throws an Out Of Range exception if the list is empty or index is out of range when function is called.
 *
 * @details Works the same way that the [] subscript operator does for arrays.
 */
template <typename T>
T& SLinkedList<T>::operator[](const int index)
{
    if(head == nullptr)
        throw std::out_of_range("Linked List is Empty");
    else if(index < 0 || index >= listSize)
        throw std::out_of_range("Index is out of range.");
    else if(index == 0)
        return head->data;
    else if(index == listSize-1)
        return tail->data;
    
    Node<T>* temp = head;
    for(int i = 0; i < index; i++)
        temp = temp->next;
    
    return temp->data;
}

/**
 * @brief   Equality comparison operator.
 *
 * @tparam T            Any data type or class.
 * @param compareList   The linked list object with which to compare this linked list object.
 * @return              A boolean flag.
 *
 * @details Returns true only if the objects are either the same object or both objects
 *          have the exact same elements in the exact same index locations. If the elements
 *          are the same but in different index locations, the objects are not considered similar.
 *          A false flag is returned for all other outcomes.
 */
template <typename T>
bool SLinkedList<T>::operator==(const SLinkedList& compareList)
{
    if(this == &compareList)
        return true;
    else if(listSize != compareList.listSize)
        return false;
    else
    {
        Node<T>* currentNode = head;
        Node<T>* compareNode = compareList.head;
        while(currentNode != nullptr && compareNode != nullptr)
        {
            if(currentNode->data != compareNode->data)
                return false;
            
            currentNode = currentNode->next;
            compareNode = compareNode->next;
        }
    }
    
    return true;
}

/**
 * @brief   Copy assignment operator.
 *
 * @tparam T        Any data type or class.
 * @param copyList  The linked list object from which to copy elements.
 * @return          A reference to a copied linked list object.
 *
 * @details Copies a linked list with the help of the copy constructor and a custom swap function.
 */
template <typename T>
SLinkedList<T>& SLinkedList<T>::operator=(const SLinkedList& copyList)
{
    SLinkedList<T> tempList(copyList);
    tempList.swap(*this);
    return *this;
}

/**
 * @brief   Move assignment operator.
 *
 * @tparam T        Any data type or class.
 * @param moveList  The linked list object from which to move elements.
 * @return          A reference to a moved linked list object.
 *
 * @details Moves linked list elements from the provided list into this linked list object.
 *          The provided linked list object is empty after the move is complete.
 *
 * @note    std::move() needs to be used to call this operator.
 */
template <typename T>
SLinkedList<T>& SLinkedList<T>::operator=(SLinkedList&& moveList)
{
    if(this == &moveList)   // Make this and moveList are not the same object.
        return *this;
    
    this->clear();
    moveList.swap(*this);
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
 * @param list      The linked list object that will be printed.
 *
 * @details Prints the list elements to the specified output stream.
 *
 * @note    Any class or data type used with this linked list class MUST implement its
 *          own operator<< in order for this operator<< to work correctly. All primitive
 *          data types (int, float, double, char, string, bool) already have this operator
 *          functionality so no implementation for them is needed. But any custom class that
 *          is used with this linked list class NEEDS to implement its own operator<<.
 */
template <typename T>
std::ostream& operator<<(std::ostream& output, const SLinkedList<T>& list)
{
    if (list.head)
        return output << "(" << *list.head << ")";
    else
        return output << "()";
}
