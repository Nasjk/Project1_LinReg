/********************************************************************************
 * @brief Implementation of dynamic container::Vectors of any data type.
 ********************************************************************************/
#pragma once

#include <container.hpp>

namespace yrgo {
namespace container {

/********************************************************************************
 * @brief Class for implementation of dynamic container::Vectors.
 ********************************************************************************/
template <typename T>
class Vector {
  public:

    /********************************************************************************
     * @brief Default constructor, creates empty container::Vector.
     ********************************************************************************/
    Vector(void) noexcept = default;

    /********************************************************************************
     * @brief Creates container::Vector of specified size.
     *
     * @param size
     *        The size of the container::Vector, i.e. the number of elements it can hold.
     ********************************************************************************/
    Vector(const size_t size) noexcept {
        Resize(size);
    }

    /********************************************************************************
     * @brief Creates container::Vector containing referenced values.
     *
     * @param values
     *        Reference to the values to store in newly created container::Vector.
     ********************************************************************************/
    template <size_t size>
    Vector(const T (&values)[size]) noexcept {
        if (Resize(size)) {
            Assign(values);
        }
    }

    /********************************************************************************
     * @brief Creates container::Vector as a copy of referenced source.
     *
     * @param source
     *        Reference to container::Vector whose content is copied to the new container::Vector.
     ********************************************************************************/
    Vector(const Vector& source) noexcept { 
        Copy(source); 
    }

    /********************************************************************************
     * @brief Move constructor, moves content from referenced source to assigned
     *        container::Vector. The source is emptied after the move operation is performed.
     *
     * @param source
     *        Reference to container::Vector whose content is moved to assigned container::Vector.
     ********************************************************************************/
    Vector(Vector&& source) noexcept {
        data_ = source.data_;
        size_ = source.size_;
        source.data_ = nullptr;
        source.size_ = 0;
    }

    /********************************************************************************
     * @brief Destructor, clears memory allocated for referenced container::Vector.
     *
     * @param source
     *        Reference to container::Vector whose content is moved to assigned container::Vector.
     ********************************************************************************/
    ~Vector(void) noexcept { Clear(); }

    /********************************************************************************
     * @brief Index operator, returns reference to the element at specified index 
     *        in referenced container::Vector. 
     *
     * @param index
     *        Index to searched element.
     * @return
     *        A reference to the element at specified index.
     ********************************************************************************/
    T& operator[](const size_t index) noexcept {
        return data_[index];
    }

    /********************************************************************************
     * @brief Index operator, returns reference to the element at specified index 
     *        in referenced container::Vector. 
     *
     * @param index
     *        Index to searched element.
     * @return
     *        A reference to the element at specified index.
     ********************************************************************************/
    const T& operator[](const size_t index) const noexcept {
        return data_[index];
    }

    /********************************************************************************
     * @brief Assignment operator, copies the content of referenced container::Vector to
     *        assigned container::Vector. Previous values are cleared before copying.
     *
     * @param values
     *        Referenced values to copy.    
     ********************************************************************************/
    template <size_t size>
    void operator=(const T (&values)[size]) noexcept {
        Clear();
        Copy(values);
    }

    /********************************************************************************
     * @brief Assignment operator, copies the content of referenced container::Vector to 
     *        assigned container::Vector. Previous values are cleared before copying.
     *
     * @param source
     *        Reference to container::Vector containing the the values to add.    
     ********************************************************************************/
    void operator=(const Vector& source) noexcept {
        Clear();
        Copy(source);
    }

    /********************************************************************************
     * @brief Addition operator, pushes referenced values to the back of the container::Vector.
     *
     * @param values
     *        Reference to the values to add.
     ********************************************************************************/
    template <size_t size>
    void operator+=(const T (&values)[size]) noexcept {
        AddValues(values);
    }

    /********************************************************************************
     * @brief Adds values from referenced container::Vector to the back of assigned container::Vector.
     *
     * @param source
     *        Reference to container::Vector containing the the values to add.
     ********************************************************************************/
    void operator+=(const Vector& source) noexcept {
        AddValues(source);
    }

    /********************************************************************************
     * @brief Returns a pointer to the data stored in specified container::Vector.
     *
     * @return
     *        A pointer to allocated memory block containing stored elements.
     ********************************************************************************/
    T* Data(void) noexcept{
        return data_;
    }

    /********************************************************************************
     * @brief Returns a pointer to the data stored in specified container::Vector.
     *
     * @return
     *        A pointer to allocated memory block containing stored elements.
     ********************************************************************************/
    const T* Data(void) const noexcept {
        return data_;
    }

    /********************************************************************************
     * @brief Returns the size of referenced container::Vector, i.e. the number of elements 
     *        it can hold.
     *
     * @return
     *        The size of the container::Vector as the number of elements it can hold.
     ********************************************************************************/
    size_t Size(void) const noexcept {
        return size_;
    }

    /********************************************************************************
     * @brief Checks if referenced container::Vector is empty.
     *
     * @return
     *        True if referenced container::Vector is empty, else false.
     ********************************************************************************/
    bool Empty(void) const noexcept {
        return size_ == 0 ? true : false;
    }

    /********************************************************************************
     * @brief Returns the start address of referenced container::Vector.
     *
     * @return
     *        A pointer to the first element of referenced container::Vector.
     ********************************************************************************/
    T* begin(void) noexcept { return data_; }

    /********************************************************************************
     * @brief Returns the start address of referenced container::Vector.
     *
     * @return
     *        A pointer to the first element of referenced container::Vector.
     ********************************************************************************/
    const T* begin(void) const noexcept { return data_; }

    /********************************************************************************
     * @brief Returns the end address of referenced container::Vector.
     *
     * @return
     *        A pointer to the address after the last element of referenced container::Vector.
     ********************************************************************************/
    T* end(void) noexcept { return data_ + size_; }

    /********************************************************************************
     * @brief Returns the end address of referenced container::Vector.
     *
     * @return
     *        A pointer to the address after the last element of referenced container::Vector.
     ********************************************************************************/
    const T* end(void) const noexcept { return data_ + size_; }

    /********************************************************************************
     * @brief Returns the address of the last element stored in referenced container::Vector.
     *
     * @return
     *        A pointer to the last element stored in referenced container::Vector.
     ********************************************************************************/
    T* last(void) noexcept { return size_ > 0 ? end() - 1 : nullptr; }

    /********************************************************************************
     * @brief Returns the address of the last element stored in referenced container::Vector.
     *
     * @return
     *        A pointer to the last element stored in referenced container::Vector.
     ********************************************************************************/
    const T* last(void) const noexcept { return size_ > 0 ? end() - 1 : nullptr; }

    /********************************************************************************
     * @brief Clears content of referenced container::Vector by deallocating memory on the
     *        heap. All member variables are reset to starting values.
     ********************************************************************************/
    void Clear(void) noexcept {
        detail::Delete<T>(data_);
        size_ = 0;
    }

    /********************************************************************************
     * @brief Resizes referenced container::Vector to specified new size via reallocation
     *        of heap allocated memory block. The memory block is unchanged if
     *        the memory allocation fails.
     *
     * @param new_size
     *        The new size of the container::Vector after reallocation.
     * @return
     *        True if the reallocation succeeded, else false.
     ********************************************************************************/
    bool Resize(const size_t new_size) noexcept {
        auto copy{detail::Resize<T>(data_, new_size)};
        if (copy == nullptr) return false;
        data_ = copy;
        size_ = new_size;
        return true;
    }

    /********************************************************************************
     * @brief Pushes new value to the back of referenced container::Vector.
     *
     * @param value
     *        Reference to the new value to push to the container::Vector.
     * @return
     *        True if the value was pushed to the back of the container::Vector, else false.
     ********************************************************************************/
    bool PushBack(const T& value) noexcept {
        if (Resize(size_ + 1)) {
            data_[size_ - 1] = value;
            return true;
        } else {
            return false;
        }
    }

    /******************************************************************************** 
     * @brief Pops value at the back of referenced container::Vector via reallocation.
     *
     * @return
     *        True if the last value of the container::Vector was popped, else false.
     ********************************************************************************/
    bool PopBack(void) noexcept {
        if (size_ <= 1) {
            Clear();
            return true;
        } else {
            return Resize(size_ - 1);
        }
    }

  private:
    T* data_{nullptr}; /* Pointer to dynamically allocated memory block. */
    size_t size_{};    /* The size of the container::Vector in number of elements it can hold. */

    /********************************************************************************
     * @brief Copies the content of referenced source. All previous elements are
     *        either removed or overwritten.
     *
     * @param source
     *        Reference to container::Vector whose content is copied.
     * @return
     *        True if the content of the source container::Vector was copied, else false.
     ********************************************************************************/
    bool Copy(const Vector& source) noexcept {
        if (Resize(source.size_)) {
            Assign(source);
            return true;
        } else {
            return false;
        }
    }

    /********************************************************************************
     * @brief Assigns referenced values without changing the container::Vector size. An offset 
     *        can be specified to assign the values further back in referenced container::Vector. 
     *        The offset should be set to the index of the first assigned value.
     *
     * @param values
     *        Referenced values to assign.
     * @param offset
     *        Offset to assign values from a specified starting index.
     ********************************************************************************/
    template <size_t size>
    void Assign(const T (&values)[size], const size_t offset = 0) noexcept {
        for (size_t i{}; i < size && offset + i < size_; ++i) {
            data_[offset + i] = values[i];
        }
    }

    /********************************************************************************
     * @brief Assigns values from referenced source container::Vector without changing the
     *        container::Vector size. An offset can be specified to assign the values further
     *        back in referenced container::Vector. The offset should be set to the index of
     *        the first assigned value.
     *
     * @param source
     *        Reference to container::Vector containing values to assign.
     * @param offset
     *        Offset to assign values from a specified starting index.
     ********************************************************************************/
    void Assign(const Vector& source, const size_t offset = 0) noexcept {
        for (size_t i{}; offset + i < size_ && i < source.size_; ++i) {
            data_[offset + i] = source.data_[i];
        }
    }

    /********************************************************************************
     * @brief Adds referenced values to the back of the container::Vector.
     *
     * @param values
     *        Reference to values to copy and add to the back of the container::Vector.
     * @return
     *        True if the values were added, else false.
     ********************************************************************************/
    template <size_t size>
    bool AddValues(const T (&values)[size]) noexcept {
        const auto offset{size_};
        if (Resize(size_ + size)) {
            Assign(values, offset);
            return true;
        } else {
            return false;
        }
    }

    /********************************************************************************
     * @brief Adds values from referenced source container::Vector to the back of the container::Vector.
     *
     * @param source
     *        Reference to container::Vector whose content is copied and added to the container::Vector.
     * @return
     *        True if the values were added, else false.
     ********************************************************************************/
    bool AddValues(const Vector& source) noexcept {
        const auto offset{size_};
        if (Resize(size_ + source.size_)) {
            Assign(source, offset);
            return true;
        } else {
            return false;
        }
    }
};

} /* namespace container */
} /* namespace yrgo */