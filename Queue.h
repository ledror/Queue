#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <new>

template <class T>
class Queue {
private:

    //private node struct to implement the queue
    struct Node {
        T data; //data stores inside the node
        Node* next; //pointer to next node

        //C'tor of a node, makes a node with given data that points to nullptr
        Node(T data) :
            data(data),
            next(nullptr)
        { }
    };

    Node* m_front; //front of the queue
    Node* m_rear; //rear of the queue
    int m_size; //size of the queue, dynamically increasing and decreasing as the queue changes

    /* RawIterator:
     *      Iterator class that supports both const iteration and normal iteration
     *      requires a template that decides which type of iteration to do
     *      for normal iteration: use Iterator
     *      for const iteration: use ConstIterator
     */
    template<typename Modified_Type>
    class RawIterator;

public:

    /**
     * @brief Construct a new Queue
     * 
     */
    Queue();

    /**
     * @brief Copy Constructor for a Queue
     *
     * @param other - Queue to copy
     */
    Queue(const Queue& other);

    /**
     * @brief Destroys the Queue
     * 
     */
    ~Queue();

    /**
     * @brief Assignment operator of a Queue
     * 
     * @param other - Queue to copy & assign
     * @return - reference to the copied queue
     */
    Queue& operator=(const Queue& other);

    /**
     * @brief Inserts in the back of the Queue
     * 
     * @param val - value to be inserted
     */
    void pushBack(const T& val);

    /**
     * @brief Returns a reference to the front of the queue
     * 
     * @return - reference to the data stored in the front
     */
    T& front();

    /**
     * @brief Returns a const reference to the front of the queue
     * 
     * @return - const reference to the data stored in the front
     */
    const T& front() const;

    /**
     * @brief Pops the element in the front of the list
     * 
     */
    void popFront();

    /**
     * @brief Returns the size of the list
     * 
     * @return - size of the list
     */
    int size() const;

    /**
     * @brief Iterator class for Queue
     * 
     */
    typedef Queue<T>::RawIterator<T> Iterator;

    /**
     * @brief Const Iterator class for Queue
     * 
     */
    typedef Queue<T>::RawIterator<const T> ConstIterator;
    
    /**
     * @brief Returns an Iterator pointing to the front of the Queue
     */
    Iterator begin()
    {
        return Iterator(this, m_front);
    }

    /**
     * @brief Returns an Iterator pointing to the end of the Queue 
     */
    Iterator end()
    {
        return Iterator(this, nullptr); //nullptr is the element "beyond" the last element
    }

    /**
     * @brief Returns a Const Iterator pointing to the front of the Queue 
     */
    ConstIterator begin() const
    {
        return ConstIterator(this, m_front);
    }

    /**
     * @brief Returns a Const Iterator pointing to the end of the Queue
     */
    ConstIterator end() const
    {
        return ConstIterator(this, nullptr); //nullptr is the element "beyond" the last element
    }

    /**
     * @brief Exception Class to deal with invalid operations done on an empty Queue
     * 
     *  Invalid operators on an empty Queue:
     *      front, popFront
     */
    class EmptyQueue {};
};

/**
 * @brief Filters a Queue using a given predict
 * 
 * @param queue - queue to filter through
 * @param predict - predict to filter by
 * @return - new filtered Queue
 */
template<typename T, typename FuncType>
Queue<T> filter(const Queue<T>& queue, FuncType predict)
{
    Queue<T> filtered;
    for(const T& data : queue)
    {
        if(predict(data)){
            filtered.pushBack(data);
        }
    }
    return filtered;
}

/**
 * @brief Transforms a Queue in-place according to a map
 * 
 * @param queue - queue to transform
 * @param transformer - mapping operator
 */
template<typename T, typename FuncType>
void transform(Queue<T>& queue, FuncType transformer)
{
    Queue<T> transformed = queue; //making a temporary queue to operate on
    for(T& data : transformed)
    {
        transformer(data);
    }
    queue = transformed; //assigning the transformed queue
}

template<typename T>
Queue<T>::Queue() :
        m_front(nullptr),
        m_rear(nullptr),
        m_size(0)
        /* default values for a Queue:
         *
         *  m_front - nullptr
         *  m_rear - nullptr
         *  m_size - 0
         */
    { }

template<typename T>
Queue<T>::Queue(const Queue& other) :
    m_front(nullptr),
    m_rear(nullptr),
    m_size(0)
{
    try{
        for(ConstIterator iter = other.begin(); iter != other.end(); ++iter){
            this->pushBack(*iter);
        }
    } catch(std::bad_alloc&){
        this->~Queue();
        throw;
    }
}

template<typename T>
Queue<T>::~Queue()
{
    while(m_size > 0){
        this->popFront();
    }
}

template<typename T>
Queue<T>& Queue<T>::operator=(const Queue& other)
{
    if(this == &other){
        return *this;
    }

    Queue<T> temp(other);
    std::swap(temp.m_front, m_front);
    std::swap(temp.m_rear, m_rear);
    std::swap(temp.m_size, m_size);
        /*  Uses c'tor for temp
         *  swaps pointers of the list and size of temp with this's
         *  when temp gets out of scope, it's d'tor will be called and destroy this's list
         * 
         * if c'tor fails, memory cleans itself and throws bad_alloc to whoever called this function
         */
    return *this;
}

template<typename T>
void Queue<T>::pushBack(const T& val)
{
    Node* temp = new Node(val);
    if(m_size == 0){ //if queue is empty, front=rear
        m_front = m_rear = temp;
        m_size++;
    }
    else{ //normal node insertion
        m_rear->next = temp;
        m_rear = temp;
        m_size++;
    }
}

template<typename T>
T& Queue<T>::front()
{
    if(m_size == 0){ //operation is invalid on an empty queue
        throw EmptyQueue();
    }
    else{ //normal reference to the data
        return m_front->data;
    }
}

template<typename T>
const T& Queue<T>::front() const
{
    if(m_size == 0){ //operation is invalid on an empty queue
        throw EmptyQueue();
    }
    else{ //const reference to the data
        return m_front->data;
    }
}

template<typename T>
void Queue<T>::popFront()
{
    if(m_size == 0){ //operation is invalid on an empty queue
        throw EmptyQueue();
    }
    else{
        Node* temp = m_front;
        m_front = m_front->next;
        delete temp;
        m_size--;
        /*
         *  saving m_front in temp
         *  advanding m_front, deleting temp and decreasing the size
         */
    }
}

template<typename T>
int Queue<T>::size() const
{
    return m_size;
}

/**
 * @brief RawIterator template to support Iterator and ConstIterator classes
 * 
 * @tparam Type - type of queue
 * @tparam Modified_Type
 *      Type - normal iterator
 *      const Type - const iterator
 */
template<typename Type>
template<typename Modified_Type>
class Queue<Type>::RawIterator {
public:
    //allowing the use of ConstIterator with a non-const Queue by conversion
    operator Queue<Type>::RawIterator<const Modified_Type>()
    {
        return Queue<Type>::RawIterator<const Modified_Type>(m_ptr, m_currentNode);
    }
private:    
    const Queue<Type>* m_ptr; //Pointer to the queue to be iterated
    Node* m_currentNode; //Current node iterator points to in queue

    /**
     * @brief Constructor for an Iterator
     * 
     * @param ptr - pointer to the queue to iterate
     * @param node - initial node to point to
     */
    RawIterator(const Queue<Type>* ptr, Node* node) :
        m_ptr(ptr),
        m_currentNode(node)
    { }

    //allows Queue to access private c'tor
    friend class Queue<Type>;
public:

    /**
     * Explicitly stating that we use default c'tor, d'tor and assignment operator
     * 
     */
    RawIterator(const RawIterator&) = default;
    ~RawIterator() = default;
    RawIterator& operator=(const RawIterator&) = default;
    
    /**
     * @brief class for invalid operations done on iterator, thrown in following functions:
     *  
     *  operator* when trying to dereference a node that's past the end
     *  operator++(prefix and postfix) when trying to increment a node that's past the end
     */
    class InvalidOperation {};
    
    /**
     * @brief Returns a reference to the data the iterator currently points to
     * 
     * @return
     *      reference if Iterator
     *      const reference if ConstIterator
     */
    Modified_Type& operator*() const
    {
        if(m_currentNode == nullptr){
            throw InvalidOperation();
        }
        else{
            return m_currentNode->data;
        }
    }

    /**
     * @brief Prefix incrementing the Iterator, making it point to the next object
     * 
     * @return - Iterator after the increment
     */
    RawIterator& operator++()
    {
        if(m_currentNode == nullptr){ //invalid operation if iterator points to the end
            throw InvalidOperation();
        }
        else{
            m_currentNode = m_currentNode->next;
            return *this;
        }
    }

    /**
     * @brief Postfix incrementing the Iterator, making it point to the next object
     * 
     * @return - Iterator before the increment
     */
    RawIterator operator++(int)
    {
        if(m_currentNode == nullptr){ //invalid operation if iterator points to the end
            throw InvalidOperation();
        }
        else{
            RawIterator result = *this;
            m_currentNode = m_currentNode->next;
            return result;
        }
    }

    /**
     * @brief Checks if 2 Iterators point to the same node
     * 
     * @param other - Iterator to compare to
     * @return true if Iterators point to the same element
     * @return false if Iterators point to a different element
     */
    bool operator!=(const RawIterator& other) const
    {
        return !(m_currentNode == other.m_currentNode);
    }
};

#endif