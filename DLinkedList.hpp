/**
 * Copyright © 2021 Al Timofeyev. All rights reserved.
 * @file    DLinkedList.hpp
 *
 * @author  Al Timofeyev
 * @date    January 3, 2021
 * @brief   A generic Doubly Linked List data structure.
 *
 * Version: 1.0
 * Modified By:
 * Modified Date:
 * *********************************************************/

#ifndef DLinkedList_hpp
#define DLinkedList_hpp

#include <iostream>
#include <stdexcept>

/**
 * @struct  Node
 * @brief   The Node struct is meant to hold the data and pointers to the previous and next list element.
 *
 * @tparam T    Any data type or class.
 */
template <typename T>
struct Node
{
    // ------------- FIELDS -------------
    T data;             /**< The data. */
    Node<T>* next;      /**< Pointer to the next node in the list. */
    Node<T>* previous;  /**< Pointer to the previous node in the list. */
    
    // ---------- CONSTRUCTORS ----------
    /** Copy Constructor. */
    Node(const T& data) : data(data), next(nullptr), previous(nullptr) {}
    /** Move Constructor. */
    Node(T&& data) : data(std::forward<T>(data)), next(nullptr), previous(nullptr) {}
    /** Struct Destructor. */
    ~Node()
    {
        delete next;
        next = nullptr;
        previous = nullptr;
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
 * @class   DLinkedList
 * @brief   A generic Doubly Linked List class.
 * @details This Doubly Linked List is templated to use any data type or class.
 * @tparam T    Any data type or class.
 */
template <typename T>
class DLinkedList
{
public:
    // ---------- CONSTRUCTORS ----------
    DLinkedList();
    DLinkedList(const DLinkedList<T>& copyList);
    DLinkedList(DLinkedList<T>&& moveList);
    ~DLinkedList();
    
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
    bool operator==(const DLinkedList& compareList);
    DLinkedList<T>& operator=(const DLinkedList& copyList);
    DLinkedList<T>& operator=(DLinkedList&& moveList);
    
    // ----- NON-MEMEBER OPERATORS ------
    template <typename Type>
    friend std::ostream& operator<<(std::ostream& output, const DLinkedList<Type>& list);
    
private:
    // ------------- FIELDS -------------
    Node<T>* head;  /**< The head of the list. */
    Node<T>* tail;  /**< The tail of the lsit. */
    int listSize;   /**< The size of the list. */
    
    // ----------- FUNCTIONS ------------
    void swap(DLinkedList<T>& other);
};

#endif /* DLinkedList_hpp */


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
DLinkedList<T>::DLinkedList() : head(nullptr), tail(nullptr), listSize(0) {}

/**
 * @brief   Copy Constructor.
 *
 * @tparam T        Any data type or class.
 * @param copyList  The list whose contents will be copied into this linked list object.
 */
template <typename T>
DLinkedList<T>::DLinkedList(const DLinkedList<T>& copyList) : head(nullptr), tail(nullptr), listSize(0)
{
    Node<T>** currentNode = &head;
    Node<T>* previousNode = nullptr;
    for(Node<T>* copyNode = copyList.head; copyNode; copyNode = copyNode->next)
    {
        (*currentNode) = new Node<T>(copyNode->data);
        
        (*currentNode)->previous = previousNode;
        previousNode = *currentNode;
        
        currentNode = &(*currentNode)->next;
        listSize++;
    }
    
    tail = previousNode;
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
DLinkedList<T>::DLinkedList(DLinkedList<T>&& moveList) : head(nullptr), tail(nullptr), listSize(0)
{
    Node<T>** currentNode = &head;
    Node<T>* previousNode = nullptr;
    for(Node<T>* moveNode = moveList.head; moveNode; moveNode = moveNode->next)
    {
        (*currentNode) = new Node<T>(std::move(moveNode->data));
        
        (*currentNode)->previous = previousNode;
        previousNode = *currentNode;
        
        currentNode = &(*currentNode)->next;
        listSize++;
    }
    
    tail = previousNode;
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
DLinkedList<T>::~DLinkedList()
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
void DLinkedList<T>::addFirst(const T data)
{
    Node<T>* node_newHead = new Node<T>(data);
    if(head == nullptr)
        tail = node_newHead;
    else
    {
        head->previous = node_newHead;
        node_newHead->next = head;
    }
    
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
void DLinkedList<T>::addLast(const T data)
{
    Node<T>* node_newTail = new Node<T>(data);
    if(head == nullptr)
        head = node_newTail;
    else
    {
        tail->next = node_newTail;
        node_newTail->previous = tail;
    }
    
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
void DLinkedList<T>::insert(const T data, const int index)
{
    if(index <= 0)
    {
        addFirst(data);
        return;
    }
    else if(index >= listSize)
    {
        addLast(data);
        return;
    }
    
    Node<T>* node = new Node<T>(data);
    Node<T>* temp;
    if(index < listSize/2)
    {
        temp = head;
        for(int i = 0; i < index; i++)
            temp = temp->next;
    }
    else
    {
        temp = tail;
        for(int i = listSize-1; i > index; i--)
            temp = temp->previous;
    }
    
    node->next = temp;
    node->previous = temp->previous;
    temp->previous->next = node;
    temp->previous = node;
    listSize++;
}

/**
 * @brief   Removes data from the list at the specified index.
 *
 * @tparam T    Any data type or class.
 * @param index Index at which to remove data from the list.
 */
template <typename T>
void DLinkedList<T>::remove(const int index)
{
    // Do nothing if list is empty or index out of range.
    if(listSize == 0 || index < 0 || index >= listSize)
        return;
    
    Node<T>* deleteNode = head;
    
    if(index == 0)  // Remove the head of list.
    {
        deleteNode = head;
        head = head->next;
        if(head == nullptr) // If list is empty, make sure tail is set to nullptr.
            tail = nullptr;
        else
            head->previous = nullptr;
    }
    else if(index == listSize-1)    // Removing the tail of list.
    {
        deleteNode = tail;
        tail = tail->previous;
        if(tail == nullptr) // If list is empty, make sure head is set to nullptr.
            head = nullptr;
        else
            tail->next = nullptr;
    }
    else if(index < listSize/2)
    {
        deleteNode = head->next;        // Start with element after head.
        for(int i = 1; i < index; i++)
            deleteNode = deleteNode->next;
        
        deleteNode->previous->next = deleteNode->next;
        deleteNode->next->previous = deleteNode->previous;
    }
    else
    {
        deleteNode = tail->previous;    // Start with element before tail.
        for(int i = listSize-2; i > index; i--)
            deleteNode = deleteNode->previous;
        
        deleteNode->previous->next = deleteNode->next;
        deleteNode->next->previous = deleteNode->previous;
    }
    
    deleteNode->next = nullptr;
    deleteNode->previous = nullptr;
    delete deleteNode;
    listSize--;
}

/**
 * @brief   Clears the entire list recursively and resets all field elements to default.
 *
 * @tparam T    Any data type or class.
 */
template <typename T>
void DLinkedList<T>::clear()
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
T DLinkedList<T>::pop()
{
    if(head == nullptr)
        throw std::out_of_range("Linked List is Empty");
    
    Node<T>* deleteNode = head;
    T popped_data = head->data;
    head = head->next;
    if(head == nullptr) // If list is empty, make sure tail is set to nullptr.
        tail = nullptr;
    else
        head->previous = nullptr;
    
    deleteNode->next = nullptr;
    deleteNode->previous = nullptr;
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
T DLinkedList<T>::pop(const int index)
{
    if(head == nullptr)
        throw std::out_of_range("Linked List is Empty");
    else if(index < 0 || index >= listSize)
        throw std::out_of_range("Index is out of range.");
    
    Node<T>* deleteNode = head;
    
    if(index == 0)  // Pop the head of list.
    {
        deleteNode = head;
        head = head->next;
        if(head == nullptr) // If list is empty, make sure tail is set to nullptr.
            tail = nullptr;
        else
            head->previous = nullptr;
    }
    else if(index == listSize-1)    // Pop the tail of list.
    {
        deleteNode = tail;
        tail = tail->previous;
        if(tail == nullptr) // If list is empty, make sure head is set to nullptr.
            head = nullptr;
        else
            tail->next = nullptr;
    }
    else if(index < listSize/2)
    {
        deleteNode = head->next;        // Start with element after head.
        for(int i = 1; i < index; i++)
            deleteNode = deleteNode->next;
        
        deleteNode->previous->next = deleteNode->next;
        deleteNode->next->previous = deleteNode->previous;
    }
    else
    {
        deleteNode = tail->previous;    // Start with element before tail.
        for(int i = listSize-2; i > index; i--)
            deleteNode = deleteNode->previous;
        
        deleteNode->previous->next = deleteNode->next;
        deleteNode->next->previous = deleteNode->previous;
    }
    
    T popped_data = deleteNode->data;
    deleteNode->next = nullptr;
    deleteNode->previous = nullptr;
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
T DLinkedList<T>::peek()
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
T DLinkedList<T>::peek(const int index)
{
    if(head == nullptr)
        throw std::out_of_range("Linked List is Empty");
    else if(index < 0 || index >= listSize)
        throw std::out_of_range("Index is out of range.");
    else if(index == 0)
        return head->data;
    else if(index == listSize-1)
        return tail->data;
    else if(index < listSize/2)
    {
        Node<T>* temp = head->next;     // Start with element after head.
        for(int i = 1; i < index; i++)
            temp = temp->next;
        
        return temp->data;
    }
    else
    {
        Node<T>* temp = tail->previous; // Start with element before tail.
        for(int i = listSize-2; i > index; i--)
            temp = temp->previous;
        
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
int DLinkedList<T>::size()
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
bool DLinkedList<T>::empty()
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
void DLinkedList<T>::swap(DLinkedList<T>& other)
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
T& DLinkedList<T>::operator[](const int index)
{
    if(head == nullptr)
        throw std::out_of_range("Linked List is Empty");
    else if(index < 0 || index >= listSize)
        throw std::out_of_range("Index is out of range.");
    else if(index == 0)
        return head->data;
    else if(index == listSize-1)
        return tail->data;
    
    Node<T>* temp;
    if(index < listSize/2)
    {
        temp = head;
        for(int i = 0; i < index; i++)
            temp = temp->next;
    }
    else
    {
        temp = tail;
        for(int i = listSize-1; i > index; i--)
            temp = temp->previous;
    }
    
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
bool DLinkedList<T>::operator==(const DLinkedList& compareList)
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
DLinkedList<T>& DLinkedList<T>::operator=(const DLinkedList& copyList)
{
    DLinkedList<T> tempList(copyList);
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
DLinkedList<T>& DLinkedList<T>::operator=(DLinkedList&& moveList)
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
std::ostream& operator<<(std::ostream& output, const DLinkedList<T>& list)
{
    if (list.head)
        return output << "(" << *list.head << ")";
    else
        return output << "()";
}
