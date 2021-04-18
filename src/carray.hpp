#pragma once

#include <algorithm>
#include <cassert>
#include <memory>

template <typename T>
class CArray
{
  static const size_t factor = 2;

public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = T*;
  using const_iterator = const T*;
  using difference_type = ptrdiff_t;
  using size_type = size_t;

  CArray()
    : m_ptr{nullptr}
    , m_size{0}
    , m_capacity{0}
  {}

  CArray(const CArray& _array)
    : m_ptr{nullptr}
    , m_size{_array.m_size}
    , m_capacity{_array.m_size}
  {
    if (_array.m_size != 0)
    {
      m_ptr = std::make_unique<T[]>(_array.m_size);
      std::copy(_array.m_ptr.get(), _array.m_ptr.get() + _array.m_size, m_ptr.get());
    }
  }

  CArray& operator=(const CArray& _array)
  {
    auto tmp = _array;
    tmp.swap(*this);

    return *this;
  }

  CArray(CArray&&) = default;
  CArray& operator=(CArray&&) = default;

  ~CArray() = default;

  iterator begin()
  {
    return m_ptr.get();
  }

  iterator end()
  {
    return m_ptr.get() + m_size;
  }

  iterator begin() const
  {
    return m_ptr.get();
  }

  iterator end() const
  {
    return m_ptr.get() + m_size;
  }

  void push_back(const T& _value)
  {
    if (m_size == m_capacity)
    {
      m_capacity = std::max<size_t>(1, m_capacity * CArray::factor);
      auto ptr = std::make_unique<T[]>(m_capacity);

      if (m_ptr)
      {
        std::copy(m_ptr.get(), m_ptr.get() + m_size, ptr.get());
      }

      m_ptr = std::move(ptr);
    }

    m_ptr[m_size] = _value;
    m_size += 1;
  }

  void insert(size_t _index, const T& _value)
  {
    assert(_index <= m_size);

    if (m_size == m_capacity)
    {
      m_capacity = std::max<size_t>(1, m_capacity * CArray::factor);
      auto ptr = std::make_unique<T[]>(m_capacity);

      if (m_ptr)
      {
        std::copy(m_ptr.get(), m_ptr.get() + _index, ptr.get());
        std::copy(m_ptr.get() + _index, m_ptr.get() + m_size, ptr.get() + _index + 1);
      }

      m_ptr = std::move(ptr);
    }
    else
    {
      std::copy_backward(m_ptr.get() + _index, m_ptr.get() + m_size, m_ptr.get() + m_size + 1);
    }

    m_ptr[_index] = _value;
    m_size += 1;
  }

  void erase(size_t _index)
  {
    assert(_index < m_size);

    std::rotate(m_ptr.get() + _index, m_ptr.get() + _index + 1, m_ptr.get() + m_size);
    m_size -= 1;
  }

  void clear()
  {
    // Например, для std::vector стандарт требует, чтобы clear() не менял емкость вектора.
    m_size = 0;
  }

  size_t capacity() const
  {
    return m_capacity;
  }

  size_t size() const
  {
    return m_size;
  }

  T& operator[](size_t _index)
  {
    assert(_index < m_size);
    return m_ptr[_index];
  }

  const T& operator[](size_t _index) const
  {
    assert(_index < m_size);
    return m_ptr[_index];
  }

  void swap(CArray& _other)
  {
    std::swap(m_ptr, _other.m_ptr);
    std::swap(m_size, _other.m_size);
    std::swap(m_capacity, _other.m_capacity);
  }

private:
  std::unique_ptr<T[]> m_ptr;
  size_t m_size;
  size_t m_capacity;
};
