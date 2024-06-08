/**
 * @file SkipList.h
 *
 * @brief Implementation of a Skip List data structure.
 *
 * This header file provides the declaration of the SkipList and SkipListNode classes.
 * A Skip List is a probabilistic data structure that allows fast search, insertion,
 * and deletion operations. It maintains multiple levels of linked lists, where each
 * higher level acts as an "express lane" for the levels below, enabling efficient
 * traversal and manipulation of the list.
 *
 * @autor Maximilian Kautzsch
 * @copyright Copyright (c) 2024 Maximilian Kautzsch
 * Licensed under MIT License.
 */

#pragma once

#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <vector>
#include <cstdlib>

/**
 * @brief Represents a node in the skip list.
 *
 * Each node contains a value and a vector of pointers to other nodes at various levels.
 *
 * @tparam T The data type of the node's value.
 */
template<typename T>
class SkipListNode {
public:
  T value;
  std::vector<SkipListNode*> next;

  SkipListNode(T val, int level);
};

/**
 * @brief Represents a skip list data structure.
 *
 * Skip list allows fast search, insertion, and deletion operations.
 *
 * @tparam T The data type of the values stored in the skip list.
 */
template<typename T>
class SkipList {
private:
  int max_level;
  SkipListNode<T>* header;
  std::vector<SkipListNode<T>*> update;

  int randomLevel();

public:
  SkipList(int max_lvl);
  ~SkipList();

  void insert(T val);
  bool remove(T val);
  T getMedian() const;
  T at(int index) const;
};

template<typename T>
SkipListNode<T>::SkipListNode(T val, int level)
  : value(val), next(level + 1, nullptr) {}

/**
 * @brief Generates a random level for node insertion.
 *
 * @return A random level for the new node.
 */
template<typename T>
int SkipList<T>::randomLevel() {
  int level = 0;
  while (rand() % 2 == 0 && level < max_level)
    level++;
  return level;
}

/**
 * @brief Constructs a SkipList object.
 *
 * Initializes the skip list with a maximum level and creates the header node.
 *
 * @param max_lvl The maximum level of the skip list.
 */
template<typename T>
SkipList<T>::SkipList(int max_lvl)
  : max_level(max_lvl) {
  header = new SkipListNode<T>(T(), max_level);
}

/**
 * @brief Destructs the SkipList object.
 *
 * Cleans up all the nodes in the skip list to free memory.
 */
template<typename T>
SkipList<T>::~SkipList() {
  SkipListNode<T>* node = header->next[0];
  while (node != nullptr) {
    SkipListNode<T>* temp = node;
    node = node->next[0];
    delete temp;
  }
  delete header;
}

/**
 * @brief Inserts a new value into the skip list.
 *
 * Adds a new node with the specified value at the appropriate position in the skip list.
 *
 * @param val The value to be inserted.
 */
template<typename T>
void SkipList<T>::insert(T val) {
  update.resize(max_level + 1);
  SkipListNode<T>* current = header;

  for (int i = max_level; i >= 0; i--) {
    while (current->next[i] != nullptr && current->next[i]->value < val)
      current = current->next[i];
    update[i] = current;
  }

  current = current->next[0];
  if (current == nullptr || current->value != val) {
    int new_level = randomLevel();
    if (new_level > max_level) {
      update.resize(new_level + 1);
      update[max_level + 1] = header;
      max_level = new_level;
    }

    SkipListNode<T>* newNode = new SkipListNode<T>(val, new_level);
    for (int i = 0; i <= new_level; i++) {
      newNode->next[i] = update[i]->next[i];
      update[i]->next[i] = newNode;
    }
  }
}

/**
 * @brief Removes a value from the skip list.
 *
 * Deletes the node containing the specified value from the skip list.
 *
 * @param val The value to be removed.
 * @return True if the value was found and removed, false otherwise.
 */
template<typename T>
bool SkipList<T>::remove(T val) {
  SkipListNode<T>* current = header;
  bool found = false;

  for (int i = max_level; i >= 0; i--) {
    while (current->next[i] != nullptr && current->next[i]->value < val)
      current = current->next[i];
    if (current->next[i] != nullptr && current->next[i]->value == val) {
      found = true;
      update[i] = current;
    }
  }

  if (!found)
    return false;

  current = update[0]->next[0];
  for (int i = 0; i <= max_level; i++) {
    if (update[i]->next[i] != current)
      break;
    update[i]->next[i] = current->next[i];
  }

  while (max_level > 0 && header->next[max_level] == nullptr)
    max_level--;

  delete current;
  return true;
}

/**
 * @brief Retrieves the median value from the skip list.
 *
 * Traverses the skip list to find and return the median value.
 *
 * @return The median value in the skip list.
 */
template<typename T>
T SkipList<T>::getMedian() const {
  int count = 0;
  SkipListNode<T>* current = header->next[0];
  while (current != nullptr) {
    count++;
    current = current->next[0];
  }

  current = header->next[0];
  for (int i = 0; i < count / 2; i++) {
    current = current->next[0];
  }

  return current->value;
}

/**
 * @brief Retrieves the value at the specified index in the skip list.
 *
 * @param index The index of the value to retrieve.
 * @return The value at the specified index.
 */
template<typename T>
T SkipList<T>::at(int index) const {
  int count = 0;
  SkipListNode<T>* current = header->next[0];
  while (current != nullptr) {
    if (count == index)
      return current->value;
    count++;
    current = current->next[0];
  }
  throw std::out_of_range("Index out of range");
}

#endif  // SKIPLIST_H