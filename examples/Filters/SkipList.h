/**
 * @file SkipList.h
 * @brief Implementation of a Skip List data structure.
 * @details This header file provides the declaration of the SkipList and SkipListNode classes.
 *          A Skip List is a probabilistic data structure that allows fast search, insertion,
 *          and deletion operations. It maintains multiple levels of linked lists, where each
 *          higher level acts as an "express lane" for the levels below, enabling efficient
 *          traversal and manipulation of the list.
 * @author Maximilian Kautzsch
 * @date June 12, 2025
 * @copyright Copyright (c) 2024 Maximilian Kautzsch
 * Licensed under MIT License.
 */

#pragma once

#ifndef SKIPLIST_H
#define SKIPLIST_H

#include "Vector.h"  // Custom vector library for Arduino

/**
 * @class SkipListNode
 * @brief Represents a node in the skip list.
 * @tparam T The data type of the node's value.
 * @details Each node contains a value and a vector of pointers to other nodes at various levels.
 */
template <typename T>
class SkipListNode
{
public:
    /**
     * @brief The value stored in the node.
     */
    T value;

    /**
     * @brief A vector of pointers to the next nodes at each level.
     */
    Vector<SkipListNode<T> *> next;

    /**
     * @brief Constructs a SkipListNode with the given value and level.
     * @param val The value to store in the node.
     * @param level The level of the node, determining the size of the next vector.
     */
    SkipListNode(T val, int level);
};

/**
 * @class SkipList
 * @brief Represents a skip list data structure.
 * @tparam T The data type of the values stored in the skip list.
 * @details A skip list allows fast search, insertion, and deletion operations by maintaining multiple levels of linked lists.
 */
template <typename T>
class SkipList
{
private:
    /**
     * @brief The maximum level of the skip list.
     */
    int max_level;

    /**
     * @brief The header node of the skip list.
     */
    SkipListNode<T> *header;

    /**
     * @brief A vector used to keep track of update positions during insertion and deletion.
     */
    Vector<SkipListNode<T> *> update;

    /**
     * @brief Generates a random level for a new node.
     * @return A random level between 0 and max_level.
     * @details The level is randomly chosen based on a coin flip (random(2) == 0) until max_level is reached.
     * @note The user should call randomSeed() in their setup() function to ensure proper randomness.
     */
    int randomLevel();

public:
    /**
     * @brief Constructs a SkipList with the specified maximum level.
     * @param max_lvl The maximum level of the skip list.
     * @details Initializes the skip list with the given maximum level and creates the header node.
     */
    SkipList(int max_lvl);

    /**
     * @brief Destructs the SkipList object.
     * @details Cleans up all the nodes in the skip list to free memory.
     */
    ~SkipList();

    /**
     * @brief Inserts a new value into the skip list.
     * @param val The value to insert.
     * @details Adds a new node with the specified value at the appropriate position in the skip list.
     */
    void insert(T val);

    /**
     * @brief Removes a value from the skip list.
     * @param val The value to remove.
     * @return True if the value was found and removed, false otherwise.
     * @details Deletes the node containing the specified value from the skip list.
     */
    bool remove(T val);

    /**
     * @brief Retrieves the median value from the skip list.
     * @return The median value.
     * @details Traverses the skip list to find and return the median value.
     */
    T getMedian() const;

    /**
     * @brief Retrieves the value at the specified index in the skip list.
     * @param index The index of the value to retrieve.
     * @param value Reference to store the retrieved value.
     * @return True if the index is valid, false otherwise.
     * @details Traverses the skip list to find and return the value at the given index.
     */
    bool at(int index, T& value) const;
};

template <typename T>
SkipListNode<T>::SkipListNode(T val, int level) : value(val), next(level + 1, nullptr) {}

template <typename T>
int SkipList<T>::randomLevel()
{
    int level = 0;
    while (random(2) == 0 && level < max_level)
        level++;
    return level;
}

template <typename T>
SkipList<T>::SkipList(int max_lvl) : max_level(max_lvl)
{
    header = new SkipListNode<T>(T(), max_level);
}

template <typename T>
SkipList<T>::~SkipList()
{
    SkipListNode<T> *node = header->next[0];
    while (node != nullptr)
    {
        SkipListNode<T> *temp = node;
        node = node->next[0];
        delete temp;
    }
    delete header;
}

template <typename T>
void SkipList<T>::insert(T val)
{
    update.resize(max_level + 1);
    SkipListNode<T> *current = header;

    for (int i = max_level; i >= 0; i--)
    {
        while (current->next[i] != nullptr && current->next[i]->value < val)
            current = current->next[i];
        update[i] = current;
    }

    current = current->next[0];
    if (current == nullptr || current->value != val)
    {
        int new_level = randomLevel();
        if (new_level > max_level)
        {
            update.resize(new_level + 1);
            update[max_level + 1] = header;
            max_level = new_level;
        }

        SkipListNode<T> *newNode = new SkipListNode<T>(val, new_level);
        for (int i = 0; i <= new_level; i++)
        {
            newNode->next[i] = update[i]->next[i];
            update[i]->next[i] = newNode;
        }
    }
}

template <typename T>
bool SkipList<T>::remove(T val)
{
    SkipListNode<T> *current = header;
    bool found = false;

    for (int i = max_level; i >= 0; i--)
    {
        while (current->next[i] != nullptr && current->next[i]->value < val)
            current = current->next[i];
        if (current->next[i] != nullptr && current->next[i]->value == val)
        {
            found = true;
            update[i] = current;
        }
    }

    if (!found)
        return false;

    current = update[0]->next[0];
    for (int i = 0; i <= max_level; i++)
    {
        if (update[i]->next[i] != current)
            break;
        update[i]->next[i] = current->next[i];
    }

    while (max_level > 0 && header->next[max_level] == nullptr)
        max_level--;

    delete current;
    return true;
}

template <typename T>
T SkipList<T>::getMedian() const
{
    int count = 0;
    SkipListNode<T> *current = header->next[0];
    while (current != nullptr)
    {
        count++;
        current = current->next[0];
    }

    if (count == 0)
        return T(); // Return default value (not ideal but works for now)

    current = header->next[0];
    for (int i = 0; i < count / 2; i++)
    {
        current = current->next[0];
    }

    return current->value;
}

template <typename T>
bool SkipList<T>::at(int index, T& value) const
{
    int count = 0;
    SkipListNode<T> *current = header->next[0];
    while (current != nullptr)
    {
        if (count == index)
        {
            value = current->value;
            return true;
        }
        count++;
        current = current->next[0];
    }
    return false;
}

#endif // SKIPLIST_H