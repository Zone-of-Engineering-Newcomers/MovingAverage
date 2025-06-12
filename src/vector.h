/**
 * @file Vector.h
 * @brief A custom vector library for Arduino, mimicking std::vector.
 * @author Grok 3
 * @date June 12, 2025
 * @details This library provides a template class Vector<T> that behaves similarly to std::vector<T>, but is implemented for use in Arduino environments where the standard C++ library is not available. Due to Arduino's limitations, such as limited memory and lack of exception support, some features are adapted accordingly.
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <Arduino.h>

/**
 * @class Vector
 * @brief A dynamic array container similar to std::vector.
 * @tparam T The type of elements stored in the vector.
 * @details This class provides a dynamic array that can resize itself automatically when elements are added or removed. It supports most of the functionality of std::vector, but with adaptations for Arduino's constraints.
 */
template <class T>
class Vector {
public:
    // Member types
    /**
     * @typedef value_type
     * @brief The type of elements stored in the vector.
     */
    typedef T value_type;

    /**
     * @typedef size_type
     * @brief Type used for sizes and capacities.
     */
    typedef size_t size_type;

    /**
     * @typedef difference_type
     * @brief Type used for differences between iterators.
     */
    typedef ptrdiff_t difference_type;

    /**
     * @typedef reference
     * @brief Reference to an element in the vector.
     */
    typedef T& reference;

    /**
     * @typedef const_reference
     * @brief Const reference to an element in the vector.
     */
    typedef const T& const_reference;

    /**
     * @typedef pointer
     * @brief Pointer to an element in the vector.
     */
    typedef T* pointer;

    /**
     * @typedef const_pointer
     * @brief Const pointer to an element in the vector.
     */
    typedef const T* const_pointer;

    /**
     * @typedef iterator
     * @brief Iterator type for the vector.
     * @note Uses simple pointers (T*) for simplicity, unlike std::vector's more complex iterator classes.
     */
    typedef T* iterator;

    /**
     * @typedef const_iterator
     * @brief Const iterator type for the vector.
     * @note Uses const pointers (const T*) for simplicity.
     */
    typedef const T* const_iterator;

    // Constructors
    /**
     * @brief Constructs an empty vector with no elements.
     */
    Vector() : data_(nullptr), size_(0), capacity_(0) {}

    /**
     * @brief Constructs a vector with n default-inserted elements.
     * @param n The number of elements to initially create.
     */
    explicit Vector(size_type n) : data_(new T[n]), size_(n), capacity_(n) {
        for (size_type i = 0; i < size_; ++i) {
            data_[i] = T();
        }
    }

    /**
     * @brief Constructs a vector with n elements, each initialized to val.
     * @param n The number of elements to initially create.
     * @param val The value to initialize each element with.
     */
    Vector(size_type n, const T& val) : data_(new T[n]), size_(n), capacity_(n) {
        for (size_type i = 0; i < size_; ++i) {
            data_[i] = val;
        }
    }

    /**
     * @brief Constructs a vector from a range [first, last).
     * @tparam InputIterator Iterator type for the range.
     * @param first Iterator to the first element of the range.
     * @param last Iterator to the past-the-end element of the range.
     */
    template <class InputIterator>
    Vector(InputIterator first, InputIterator last) : data_(nullptr), size_(0), capacity_(0) {
        for (; first != last; ++first) {
            push_back(*first);
        }
    }

    /**
     * @brief Copy constructor.
     * @param other The vector to copy from.
     */
    Vector(const Vector& other) : data_(new T[other.capacity_]), size_(other.size_), capacity_(other.capacity_) {
        for (size_type i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    /**
     * @brief Move constructor.
     * @param other The vector to move from.
     * @note This constructor is only available if C++11 is supported.
     */
    Vector(Vector&& other) : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    /**
     * @brief Destructor.
     * @details Frees the dynamically allocated memory.
     */
    ~Vector() {
        delete[] data_;
    }

    // Assignment
    /**
     * @brief Copy assignment operator.
     * @param other The vector to copy from.
     * @return Reference to this vector.
     */
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data_;
            data_ = new T[other.capacity_];
            size_ = other.size_;
            capacity_ = other.capacity_;
            for (size_type i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other The vector to move from.
     * @return Reference to this vector.
     * @note This operator is only available if C++11 is supported.
     */
    Vector& operator=(Vector&& other) {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    // Element access
    /**
     * @brief Accesses the element at the specified position with bounds checking.
     * @param pos The position of the element to access.
     * @return Reference to the element at pos.
     * @throws std::out_of_range if pos is not within the range [0, size()).
     * @note In Arduino, since exceptions are not supported, this function will not throw as expected. Ensure pos is within range to avoid undefined behavior.
     */
    reference at(size_type pos) {
        if (pos >= size_) throw; // Simplified error handling
        return data_[pos];
    }

    /**
     * @brief Accesses the element at the specified position with bounds checking (const version).
     * @param pos The position of the element to access.
     * @return Const reference to the element at pos.
     * @throws std::out_of_range if pos is not within the range [0, size()).
     * @note In Arduino, since exceptions are not supported, this function will not throw as expected. Ensure pos is within range to avoid undefined behavior.
     */
    const_reference at(size_type pos) const {
        if (pos >= size_) throw; // Simplified error handling
        return data_[pos];
    }

    /**
     * @brief Accesses the element at the specified position without bounds checking.
     * @param pos The position of the element to access.
     * @return Reference to the element at pos.
     * @note No bounds checking is performed. Accessing pos >= size() results in undefined behavior.
     */
    reference operator[](size_type pos) { return data_[pos]; }

    /**
     * @brief Accesses the element at the specified position without bounds checking (const version).
     * @param pos The position of the element to access.
     * @return Const reference to the element at pos.
     * @note No bounds checking is performed. Accessing pos >= size() results in undefined behavior.
     */
    const_reference operator[](size_type pos) const { return data_[pos]; }

    /**
     * @brief Returns a reference to the first element in the vector.
     * @return Reference to the first element.
     * @note Calling this on an empty vector results in undefined behavior.
     */
    reference front() { return data_[0]; }

    /**
     * @brief Returns a const reference to the first element in the vector.
     * @return Const reference to the first element.
     * @note Calling this on an empty vector results in undefined behavior.
     */
    const_reference front() const { return data_[0]; }

    /**
     * @brief Returns a reference to the last element in the vector.
     * @return Reference to the last element.
     * @note Calling this on an empty vector results in undefined behavior.
     */
    reference back() { return data_[size_ - 1]; }

    /**
     * @brief Returns a const reference to the last element in the vector.
     * @return Const reference to the last element.
     * @note Calling this on an empty vector results in undefined behavior.
     */
    const_reference back() const { return data_[size_ - 1]; }

    /**
     * @brief Returns a pointer to the underlying array serving as element storage.
     * @return Pointer to the first element of the array.
     */
    pointer data() { return data_; }

    /**
     * @brief Returns a const pointer to the underlying array serving as element storage.
     * @return Const pointer to the first element of the array.
     */
    const_pointer data() const { return data_; }

    // Iterators
    /**
     * @brief Returns an iterator to the beginning of the vector.
     * @return Iterator pointing to the first element.
     */
    iterator begin() { return data_; }

    /**
     * @brief Returns a const iterator to the beginning of the vector.
     * @return Const iterator pointing to the first element.
     */
    const_iterator begin() const { return data_; }

    /**
     * @brief Returns an iterator to the end of the vector.
     * @return Iterator pointing to one past the last element.
     */
    iterator end() { return data_ + size_; }

    /**
     * @brief Returns a const iterator to the end of the vector.
     * @return Const iterator pointing to one past the last element.
     */
    const_iterator end() const { return data_ + size_; }

    // Capacity
    /**
     * @brief Checks if the vector is empty.
     * @return true if the vector has no elements, false otherwise.
     */
    bool empty() const { return size_ == 0; }

    /**
     * @brief Returns the number of elements in the vector.
     * @return The size of the vector.
     */
    size_type size() const { return size_; }

    /**
     * @brief Returns the maximum number of elements the vector can hold.
     * @return The maximum possible size.
     * @note This is a theoretical limit and may be less due to memory constraints.
     */
    size_type max_size() const { return static_cast<size_type>(-1) / sizeof(T); }

    /**
     * @brief Requests that the vector capacity be at least enough to contain n elements.
     * @param new_cap The minimum capacity requested.
     * @note If new_cap is greater than the current capacity, the capacity is increased.
     */
    void reserve(size_type new_cap) {
        if (new_cap > capacity_) {
            grow(new_cap);
        }
    }

    /**
     * @brief Returns the number of elements that the vector has currently reserved space for.
     * @return The current capacity.
     */
    size_type capacity() const { return capacity_; }

    // Modifiers
    /**
     * @brief Clears the vector, removing all elements.
     * @note Memory is not deallocated; capacity remains unchanged.
     */
    void clear() {
        size_ = 0;
        // Note: Memory is not deallocated to avoid frequent reallocations
    }

    /**
     * @brief Inserts elements at the specified position.
     * @param pos Iterator before which the content will be inserted.
     * @param value The value to insert.
     * @return Iterator pointing to the inserted element.
     */
    iterator insert(const_iterator pos, const T& value) {
        size_type index = pos - data_;
        if (size_ == capacity_) {
            grow(size_ + 1);
        }
        for (size_type i = size_; i > index; --i) {
            data_[i] = data_[i - 1];
        }
        data_[index] = value;
        ++size_;
        return data_ + index;
    }

    /**
     * @brief Inserts n copies of value at the specified position.
     * @param pos Iterator before which the content will be inserted.
     * @param n Number of elements to insert.
     * @param value The value to insert.
     * @return Iterator pointing to the first inserted element.
     */
    iterator insert(const_iterator pos, size_type n, const T& value) {
        size_type index = pos - data_;
        if (size_ + n > capacity_) {
            grow(size_ + n);
        }
        for (size_type i = size_; i > index; --i) {
            data_[i + n - 1] = data_[i - 1];
        }
        for (size_type i = 0; i < n; ++i) {
            data_[index + i] = value;
        }
        size_ += n;
        return data_ + index;
    }

    /**
     * @brief Inserts elements from a range [first, last) at the specified position.
     * @tparam InputIterator Iterator type for the range.
     * @param pos Iterator before which the content will be inserted.
     * @param first Iterator to the first element of the range.
     * @param last Iterator to the past-the-end element of the range.
     * @return Iterator pointing to the first inserted element.
     */
    template <class InputIterator>
    iterator insert(const_iterator pos, InputIterator first, InputIterator last) {
        size_type index = pos - data_;
        size_type n = 0;
        for (InputIterator it = first; it != last; ++it) {
            ++n;
        }
        if (size_ + n > capacity_) {
            grow(size_ + n);
        }
        for (size_type i = size_; i > index; --i) {
            data_[i + n - 1] = data_[i - 1];
        }
        for (size_type i = 0; first != last; ++first, ++i) {
            data_[index + i] = *first;
        }
        size_ += n;
        return data_ + index;
    }

    /**
     * @brief Erases the element at the specified position.
     * @param pos Iterator to the element to erase.
     * @return Iterator following the last removed element.
     */
    iterator erase(const_iterator pos) {
        size_type index = pos - data_;
        for (size_type i = index; i < size_ - 1; ++i) {
            data_[i] = data_[i + 1];
        }
        --size_;
        return data_ + index;
    }

    /**
     * @brief Erases the elements in the range [first, last).
     * @param first Iterator to the first element to erase.
     * @param last Iterator to one past the last element to erase.
     * @return Iterator following the last removed element.
     */
    iterator erase(const_iterator first, const_iterator last) {
        size_type index = first - data_;
        size_type n = last - first;
        for (size_type i = index; i < size_ - n; ++i) {
            data_[i] = data_[i + n];
        }
        size_ -= n;
        return data_ + index;
    }

    /**
     * @brief Adds an element to the end of the vector.
     * @param value The value to add.
     */
    void push_back(const T& value) {
        if (size_ == capacity_) {
            grow(size_ + 1);
        }
        data_[size_++] = value;
    }

    /**
     * @brief Removes the last element from the vector.
     * @note Calling this on an empty vector has no effect.
     */
    void pop_back() {
        if (size_ > 0) {
            --size_;
        }
    }

    /**
     * @brief Resizes the vector to contain count elements.
     * @param count The new size.
     * @note If count > size(), new elements are default-inserted. If count < size(), excess elements are destroyed.
     */
    void resize(size_type count) {
        if (count > size_) {
            if (count > capacity_) {
                grow(count);
            }
            for (size_type i = size_; i < count; ++i) {
                data_[i] = T();
            }
        }
        size_ = count;
    }

    /**
     * @brief Resizes the vector to contain count elements, initializing new elements with val.
     * @param count The new size.
     * @param value The value to initialize new elements with.
     */
    void resize(size_type count, const T& value) {
        if (count > size_) {
            if (count > capacity_) {
                grow(count);
            }
            for (size_type i = size_; i < count; ++i) {
                data_[i] = value;
            }
        }
        size_ = count;
    }

    /**
     * @brief Swaps the contents of this vector with another.
     * @param other The vector to swap with.
     */
    void swap(Vector& other) {
        T* temp_data = data_;
        size_type temp_size = size_;
        size_type temp_capacity = capacity_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = temp_data;
        other.size_ = temp_size;
        other.capacity_ = temp_capacity;
    }

private:
    T* data_;         ///< Pointer to the dynamically allocated array
    size_type size_;  ///< Number of elements currently in the vector
    size_type capacity_; ///< Total number of elements that the vector can hold without reallocation

    /**
     * @brief Reallocates the vector's storage to hold at least min_capacity elements.
     * @param min_capacity The minimum capacity required.
     * @note This function is called internally when the vector needs to grow.
     */
    void grow(size_type min_capacity) {
        size_type new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
        if (new_capacity < min_capacity) new_capacity = min_capacity;
        T* new_data = new T[new_capacity];
        for (size_type i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }
};

// Non-member functions
/**
 * @brief Checks if two vectors are equal.
 * @param lhs The left-hand side vector.
 * @param rhs The right-hand side vector.
 * @return true if the vectors have the same size and elements, false otherwise.
 */
template <class T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
    if (lhs.size() != rhs.size()) return false;
    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i]) return false;
    }
    return true;
}

/**
 * @brief Checks if two vectors are not equal.
 * @param lhs The left-hand side vector.
 * @param rhs The right-hand side vector.
 * @return true if the vectors are not equal, false otherwise.
 */
template <class T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs == rhs);
}

#endif // VECTOR_H