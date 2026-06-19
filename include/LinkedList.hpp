#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include "Exceptions.hpp"

template <class T>
class LinkedList {
private:
    template <class U, class Derived>
    friend class ListSequence;

    struct Node {
        T value;
        Node* next;

        explicit Node(const T& value) : value(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int length;

    void ValidateIndex(int index) const {
        if (index < 0 || index >= length) {
            throw IndexOutOfRangeException("index is outside list bounds");
        }
    }

    Node* GetNode(int index) const {
        ValidateIndex(index);

        Node* current = head;
        int currentIndex = 0;

        while (currentIndex < index) {
            current = current->next;
            ++currentIndex;
        }

        return current;
    }

    void CopyFrom(const LinkedList<T>& other) {
        Node* current = other.head;

        while (current != nullptr) {
            Append(current->value);
            current = current->next;
        }
    }

public:
    LinkedList() : head(nullptr), tail(nullptr), length(0) {}

    LinkedList(T* items, int count) : head(nullptr), tail(nullptr), length(0) {
        if (count < 0) {
            throw InvalidArgumentException("list size cannot be negative");
        }

        if (count > 0 && items == nullptr) {
            throw InvalidArgumentException("source array is null while count is positive");
        }

        for (int i = 0; i < count; ++i) {
            Append(items[i]);
        }
    }

    LinkedList(const LinkedList<T>& list) : head(nullptr), tail(nullptr), length(0) {
        CopyFrom(list);
    }

    LinkedList(LinkedList<T>&& other) noexcept
        : head(other.head), tail(other.tail), length(other.length) {
        other.head = nullptr;
        other.tail = nullptr;
        other.length = 0;
    }

    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this == &other) {
            return *this;
        }

        Clear();
        CopyFrom(other);

        return *this;
    }

    LinkedList<T>& operator=(LinkedList<T>&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Clear();

        head = other.head;
        tail = other.tail;
        length = other.length;

        other.head = nullptr;
        other.tail = nullptr;
        other.length = 0;

        return *this;
    }

    ~LinkedList() {
        Clear();
    }

    void Clear() {
        Node* current = head;

        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }

        head = nullptr;
        tail = nullptr;
        length = 0;
    }

    T GetFirst() const {
        if (length == 0) {
            throw EmptySequenceException("cannot get first item from empty list");
        }

        return head->value;
    }

    T GetLast() const {
        if (length == 0) {
            throw EmptySequenceException("cannot get last item from empty list");
        }

        return tail->value;
    }

    T Get(int index) const {
        return GetNode(index)->value;
    }

    LinkedList<T>* GetSubList(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex < 0 || startIndex >= length || endIndex >= length) {
            throw IndexOutOfRangeException("sublist boundaries are outside list bounds");
        }

        if (startIndex > endIndex) {
            throw InvalidArgumentException("startIndex cannot be greater than endIndex");
        }

        LinkedList<T>* result = new LinkedList<T>();
        Node* current = head;
        int currentIndex = 0;

        while (currentIndex < startIndex) {
            current = current->next;
            ++currentIndex;
        }

        while (currentIndex <= endIndex) {
            result->Append(current->value);
            current = current->next;
            ++currentIndex;
        }

        return result;
    }

    int GetLength() const {
        return length;
    }

    void Append(T item) {
        Node* node = new Node(item);

        if (length == 0) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }

        ++length;
    }

    void Prepend(T item) {
        Node* node = new Node(item);

        if (length == 0) {
            head = node;
            tail = node;
        } else {
            node->next = head;
            head = node;
        }

        ++length;
    }

    void InsertAt(T item, int index) {
        if (index < 0 || index > length) {
            throw IndexOutOfRangeException("insert index is outside list bounds");
        }

        if (index == 0) {
            Prepend(item);
            return;
        }

        if (index == length) {
            Append(item);
            return;
        }

        Node* previous = GetNode(index - 1);
        Node* node = new Node(item);

        node->next = previous->next;
        previous->next = node;

        ++length;
    }

    LinkedList<T>* Concat(const LinkedList<T>* list) const {
        if (list == nullptr) {
            throw InvalidArgumentException("cannot concatenate with null list");
        }

        LinkedList<T>* result = new LinkedList<T>(*this);
        Node* current = list->head;

        while (current != nullptr) {
            result->Append(current->value);
            current = current->next;
        }

        return result;
    }

    T& operator[](int index) {
        return GetNode(index)->value;
    }

    const T& operator[](int index) const {
        return GetNode(index)->value;
    }
};

#endif
