#include <algorithm>
#include "vector.h"
Vector::ValueType& Vector::Iterator::operator*() const {
    return *pointer_;
}
Vector::ValueType* Vector::Iterator::operator->() const {
    return pointer_;
}
Vector::Iterator& Vector::Iterator::operator++() {
    ++pointer_;
    return *this;
}
Vector::Iterator Vector::Iterator::operator++(int) {
    ValueType* temporary_pointer = pointer_;
    ++pointer_;
    return Vector::Iterator(temporary_pointer);
}
Vector::Iterator& Vector::Iterator::operator--() {
    --pointer_;
    return *this;
}
Vector::Iterator Vector::Iterator::operator--(int) {
    ValueType* temporary_pointer = pointer_;
    --pointer_;
    return Vector::Iterator(temporary_pointer);
}
Vector::Iterator Vector::Iterator::operator+(Vector::DifferenceType shift) {
    return Vector::Iterator(shift + pointer_);
}
Vector::DifferenceType Vector::Iterator::operator-(Vector::Iterator other) {
    return pointer_ - other.pointer_;
}
Vector::Iterator& Vector::Iterator::operator+=(Vector::DifferenceType shift) {
    pointer_ += shift;
    return *this;
}
Vector::Iterator& Vector::Iterator::operator-=(Vector::DifferenceType shift) {
    pointer_ -= shift;
    return *this;
}
bool Vector::Iterator::operator==(const Vector::Iterator& other) const {
    return pointer_ == other.pointer_;
}
bool Vector::Iterator::operator!=(const Vector::Iterator& other) const {
    return pointer_ != other.pointer_;
}
Vector::Iterator& Vector::Iterator::operator=(Vector::Iterator other) {
    pointer_ = other.pointer_;
    return *this;
}
std::strong_ordering Vector::Iterator::operator<=>(const Vector::Iterator& other) const {
    return pointer_ <=> other.pointer_;
}
Vector::Iterator::Iterator(Vector::ValueType* pointer) : pointer_(pointer) {
}
Vector::Vector() : size_(0), capacity_(0), data_(new ValueType[capacity_]) {
}
Vector::Vector(size_t size) : size_(size), capacity_(size), data_(new ValueType[size]) {
    for (SizeType i = 0; i < size_; ++i) {
        data_[i] = 0;
    }
}
Vector::Vector(std::initializer_list<ValueType> list)
    : size_(0), capacity_(list.size()), data_(new ValueType[list.size()]) {
    for (auto element : list) {
        data_[size_++] = std::move(element);
    }
}
Vector::Vector(const Vector& other) : size_(other.size_), capacity_(other.capacity_), data_(new ValueType[capacity_]) {
    for (SizeType i = 0; i < other.size_; ++i) {
        data_[i] = other.data_[i];
    }
}
void Vector::Reserve(Vector::SizeType new_capacity) {
    if (new_capacity <= capacity_) {
        return;
    }
    new_capacity = std::max(capacity_ * 2, new_capacity);
    ValueType* new_data = new ValueType[new_capacity];
    for (SizeType i = 0; i < size_; ++i) {
        new_data[i] = data_[i];
    }
    delete[] data_;
    capacity_ = new_capacity;
    data_ = new_data ?: throw std::bad_alloc();
}
Vector& Vector::operator=(const Vector& other) {
    if (this == &other) {
        return *this;
    }
    Reserve(other.capacity_);
    for (SizeType i = 0; i < other.size_; ++i) {
        data_[i] = other.data_[i];
    }
    return *this;
}
Vector::~Vector() {
    delete[] data_;
}
Vector::SizeType Vector::Size() const {
    return size_;
}
Vector::SizeType Vector::Capacity() const {
    return capacity_;
}
const Vector::ValueType* Vector::Data() const {
    return data_;
}
Vector::ValueType& Vector::operator[](size_t position) {
    return position < size_ ? data_[position] : throw std::runtime_error("Out of range!!!");
}
Vector::ValueType Vector::operator[](size_t position) const {
    Vector::ValueType element = position < size_ ? data_[position] : throw std::runtime_error("Out of range!!!");
    return element;
}
bool Vector::operator==(const Vector& other) const {
    if (size_ != other.size_) {
        return false;
    }
    for (SizeType i = 0; i < size_; ++i) {
        if (data_[i] != other.data_[i]) {
            return false;
        }
    }
    return true;
}
bool Vector::operator!=(const Vector& other) const {
    if (size_ != other.size_) {
        return true;
    }
    for (SizeType i = 0; i < size_; ++i) {
        if (data_[i] != other.data_[i]) {
            return true;
        }
    }
    return false;
}
std::strong_ordering Vector::operator<=>(const Vector& other) const {
    for (SizeType i = 0; i < std::min(size_, other.size_); ++i) {
        if (data_[i] != other.data_[i]) {
            return data_[i] <=> other.data_[i];
        }
    }
    return size_ <=> other.size_;
}
void Vector::Clear() {
    for (SizeType i = 0; i < size_; ++i) {
        data_[i].~ValueType();
    }
    size_ = 0;
}
void Vector::PushBack(const Vector::ValueType& new_element) {
    Reserve(size_ + 1);
    data_[size_++] = new_element;
}
void Vector::PopBack() {
    size_ == 0 ? throw std::runtime_error("Empty Array!") : --size_;
}
void Vector::Swap(Vector& other) {
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
    std::swap(data_, other.data_);
}
Vector::Iterator Vector::Begin() {
    return Vector::Iterator(data_);
}
Vector::Iterator Vector::End() {
    return Vector::Iterator(data_ + size_);
}
Vector::Iterator Vector::begin() {
    return Vector::Iterator(data_);
}
Vector::Iterator Vector::end() {
    return Vector::Iterator(data_ + size_);
}
