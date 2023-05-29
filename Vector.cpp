#include "Vector.h"

#include <algorithm>
#include <stdexcept>



Vector::Vector(const Value *rawArray, const size_t size, float coef)
        : _multiplicativeCoef{coef}, _capacity{size} {
    _data = new Value[_capacity];
    std::copy(rawArray, rawArray + size, _data);
    _size = size;
}

Vector::Vector(const Vector &other)
        : _multiplicativeCoef{other._multiplicativeCoef}, _capacity{other._size} {
    _data = new Value[_capacity];
    std::copy(other._data, other._data + other._size, _data);
    _size = other._size;
}

Vector &Vector::operator=(const Vector &other) {
    if (this == &other) {
        return *this;
    }
    _size = other.size();
    _multiplicativeCoef = other._multiplicativeCoef;
    _capacity = std::min(other.capacity(), _size * (size_t) _multiplicativeCoef);
    delete[] _data;

    _data = new Value[_capacity];
    for (size_t i = 0; i < _size; ++i) {
        _data[i] = other._data[i];
    }

    return *this;
}


Vector::Vector(Vector &&other) noexcept
        : _data{other._data}, _size{other._size}, _capacity{other._capacity},
          _multiplicativeCoef{other._multiplicativeCoef} {
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
}

Vector &Vector::operator=(Vector &&other) noexcept {
    if (this != &other) {
        delete[] _data;
        _data = other._data;
        _size = other._size;
        _capacity = other._capacity;
        _multiplicativeCoef = other._multiplicativeCoef;

        other._data = nullptr;
        other._size = 0;
        other._capacity = 0;
    }
    return *this;
}

Vector::~Vector() {
    delete[] _data;
}

void Vector::pushBack(const Value &value) {
    if (_size == _capacity) {
        reserve(std::max(_capacity * (size_t) _multiplicativeCoef, (size_t) _multiplicativeCoef));
    }

    _data[_size] = value;
    ++_size;
}

void Vector::pushFront(const Value &value) {
    if (_size == _capacity) {
        reserve(std::max(_capacity * (size_t) _multiplicativeCoef, (size_t) _multiplicativeCoef));
    }

    for (size_t i = _size; i > 0; --i) {
        _data[i] = _data[i - 1];
    }
    _data[0] = value;
    ++_size;
}

void Vector::insert(const Value &value, size_t pos) {
    if (_size == _capacity) {
        reserve(_capacity * (size_t) _multiplicativeCoef);
    }
    for (size_t i = _size; i > pos; --i) {
        _data[i] = _data[i - 1];
    }
    _data[pos] = value;
    ++_size;
}

void Vector::insert(const Vector &vector, size_t pos) {
    insert(vector._data, vector._size, pos);
}

void Vector::insert(const Value *values, size_t size, size_t pos) {
    if(pos > _size){
        throw std::out_of_range("Index out of range");
    }
    if (_size + size > _capacity) {
        reserve(static_cast<size_t>(_capacity * _multiplicativeCoef + size));
    }
    Value *pos_ptr = _data + pos;
    std::copy_backward(pos_ptr, _data + _size, _data + _size + size);
    std::copy(values, values + size, pos_ptr);
    _size += size;
}

void Vector::popBack() {
    if (!_size) {
        throw std::out_of_range("Vector's size is equal to zero");
    }
    --_size;
}

void Vector::popFront() {
    if (!_size) {
        throw std::out_of_range("Vector's size is equal to zero");
    } else {
        erase(0);
    }
}

void Vector::erase(size_t pos, size_t count) {
    if (pos >= _size) {
        return; // если позиция за пределами границ, то ничего не делаем
    }

    // Вычисляем количество удаляемых элементов
    const size_t actual_count = std::min(count, _size - pos);

    // Сдвигаем элементы массива на их новые позиции
    for (size_t i = pos; i < _size - actual_count; ++i) {
        _data[i] = _data[i + actual_count];
    }

    // Обнуляем последние элементы
    for (size_t i = _size - actual_count; i < _size; ++i) {
        _data[i] = 0;
    }

    // Уменьшаем размер вектора на количество удаленных элементов
    _size -= actual_count;
}

void Vector::eraseBetween(size_t beginPos, size_t endPos) {
    this->erase(beginPos, endPos - beginPos);
}

size_t Vector::size() const {
    return _size;
}

size_t Vector::capacity() const {
    return _capacity;
}

double Vector::loadFactor() const {
    //return static_cast<double>(_size) / _capacity;
    return (_size) / _capacity;
}

Value &Vector::operator[](size_t idx) {
    if (idx >= _size) {
        throw std::out_of_range("Index out of range.");
    }
    return _data[idx];
}

const Value &Vector::operator[](size_t idx) const {
    if (idx >= _size) {
        throw std::out_of_range("Index out of range");
    }
    return _data[idx];
}




void Vector::reserve(size_t capacity) {
    if (capacity > _capacity) {
        Value *tmpArray = new Value[capacity];
        std::copy(_data, _data + _size, tmpArray);
        delete[] _data;
        _data = tmpArray;
        _capacity = capacity;
    }
}


long long Vector::find(const Value &value) const {
    for (size_t i = 0; i < _size; i++) {
        if (_data[i] == value) {
            return i;
        }
    }
    return -1;
}

void Vector::shrinkToFit() {
    if (_capacity > _size) {
        Value *newData = new Value[_size];
        std::copy(_data, _data + _size, newData);
        delete[] _data;
        _data = newData;
        _capacity = _size;
    }
}

Vector::Iterator::Iterator(Value *ptr) : _ptr(ptr) {}

Value &Vector::Iterator::operator*() {
    return *_ptr;
}

Value *Vector::Iterator::operator->() {
    return _ptr;
}

const Value *Vector::Iterator::operator->() const {
    return _ptr;
}



Vector::Iterator Vector::Iterator::operator++() {
    ++_ptr;
    return *this;
}

Vector::Iterator Vector::Iterator::operator++(int) {
    Iterator tmp = *this;
    ++_ptr;
    return tmp;
}

bool Vector::Iterator::operator==(const Vector::Iterator &other) const {
    return _ptr == other._ptr;
}

bool Vector::Iterator::operator!=(const Vector::Iterator &other) const {
    return _ptr != other._ptr;
}

Vector::Iterator Vector::begin() {
    return Iterator(_data);
}

Vector::Iterator Vector::end() {
    return Iterator(_data + _size);
}


