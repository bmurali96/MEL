#pragma once

#include <vector>

namespace mel {

//==============================================================================
// CLASS DECLARATION
//==============================================================================

template <typename T>
class RingBuffer {

public:

    /// Default constructor
    RingBuffer(std::size_t capacity) :
        capacity_(capacity),
        size_(0),
        front_(0),
        back_(0),
        buffer_(capacity_)
    { }

    /// Read access
    const T& operator[](std::size_t index) const {
        return buffer_[(front_ + index) % capacity_];
    }

    /// Write access
    T& operator[](std::size_t index) {
        return buffer_[(front_ + index) % capacity_];
    }

    /// Adds a new element at the back of the RingBuffer
    void push_back(const T& value) {
        buffer_[back_] = value;
        if (full()) {
            if (front_ == back_) {
                if (++front_ == capacity_)
                    front_ = 0;
            }
        }
        else
            ++size_;
        if (++back_ == capacity_)
            back_ = 0;
    }

    /// Adds a new element at the front of the RingBuffer
    void push_front(const T& value) {
        if (full()) {
            if (back_ == front_) {
                if (back_ == 0)
                    back_ = capacity_;
                --back_;
            }
        }
        else
            ++size_;
        if (front_ == 0)
            front_ = capacity_;
        --front_;
        buffer_[front_] = value;
    }

    /// Removes the element from the back of the RingBuffer and returns it
    T pop_back() {
        if (empty())
            return T();
        --size_;
        if (back_ == 0)
            back_ = capacity_;
        --back_;
        T value = buffer_[back_];
        return value;
    }

    /// Removes the element from the front of the RingBuffer and returns it
    T pop_front() {
        if (empty())
            return T();
        --size_;
        T value = buffer_[front_];
        if (++front_ == capacity_)
            front_ = 0;
        return value;
    }

    /// Returns true if the RingBuffer is empty
    bool empty() const {
        return size_ == 0;
    }

    /// Returns true if the RingBuffer is full
    bool full() const {
        return size_ == capacity_;
    }

    /// Returns occupied size of the RingBuffer
    size_t size() const {
        return size_;
    }


    /// Returns the capacity of the RingBuffer
    size_t capacity() const {
        return capacity_;
    }

    /// Resizes teh RingBuffer to a new capacity
    void resize(std::size_t capacity) {
        if (capacity <= size_) {
            size_ = capacity;
            back_ = 0;
        }
        else
            back_ = size_;
        std::vector<T> new_buffer(capacity);
        for (std::size_t i = 0; i < size_; ++i)
            new_buffer[i] = buffer_[(front_ + i) % capacity_];
        capacity_ = capacity;
        front_ = 0;
        buffer_ = new_buffer;
    }

    size_t capacity_;        ///< the maximum capacity of the RingBuffer
    size_t size_;            ///< current occupied size of the RingBuffer
    size_t front_;           ///< front index of the RingBuffer
    size_t back_;            ///< back index of the RingBuffer
    std::vector<T> buffer_;  ///< underlying buffer array
};

} // mel

//==============================================================================
// CLASS DOCUMENTATION
//==============================================================================
//
// https://en.wikipedia.org/wiki/Circular_buffer
//
//                                   ILLUSION        ACTUAL MEMORY
//                                                  f/b
// RingBuffer<int> x(5)    =>    [ ][ ][ ][ ][ ]    [0][0][0][0][0]
//                                                   f  b
// x.push_back(1)          =>    [1][ ][ ][ ][ ]    [1][0][0][0][0]
//
// ... 2, 3, ...
//                                                   f           b
// x.push_back(4)          =>    [1][2][3][4][ ]    [1][2][3][4][0]
//                                                  b/f
// x.push_back(5)          =>    [1][2][3][4][5]    [1][2][3][4][5]
//                                                     b/f
// x.push_back(6)          =>    [2][3][4][5][6]    [6][2][3][4][5]
//                                                   b  f
// x.pop_back()            =>    [2][3][4][5][ ]    [6][2][3][4][5]
//                                                  b/f
// x.push_front(1)         =>    [1][2][3][4][5]    [1][2][3][4][5]
//                                                              b/f
// x.push_front(0)         =>    [0][1][2][3][4]    [1][2][3][4][0]
//                                                   f           b
// x.pop_front()           =>    [1][2][3][4][ ]    [1][2][3][4][0]

