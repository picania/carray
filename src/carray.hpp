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

  CArray(
      const CArray& _array
    )
    : m_ptr{nullptr}
    , m_size{_array.m_size}
    , m_capacity{_array.m_size}
  {
    if (_array.m_size != 0)
    {
      m_ptr = std::allocator_traits<std::allocator<T>>::allocate(m_allocator, m_capacity);
      for (size_t i = 0; i != _array.m_size; ++i)
      {
        std::allocator_traits<std::allocator<T>>::construct(m_allocator, &m_ptr[i], _array.m_ptr[i]);
      }
    }
  }

  CArray& operator=(
      const CArray& _array
    )
  {
    auto tmp = _array;
    tmp.swap(*this);

    return *this;
  }

  ~CArray()
  {
    for (size_t i = 0; i != m_size; ++i)
    {
      std::allocator_traits<std::allocator<T>>::destroy(m_allocator, &m_ptr[i]);
    }

    std::allocator_traits<std::allocator<T>>::deallocate(m_allocator, m_ptr, m_capacity);
  }

  iterator begin()
  {
    return m_ptr;
  }

  iterator end()
  {
    return m_ptr + m_size;
  }

  const_iterator begin() const
  {
    return m_ptr;
  }

  const_iterator end() const
  {
    return m_ptr + m_size;
  }

  void push_back(
      const T& _value
    )
  {
    if (m_size == m_capacity)
    {
      const size_t old_capacity = m_capacity;
      m_capacity = std::max<size_t>(1, m_capacity * CArray::factor);
      T* ptr = std::allocator_traits<std::allocator<T>>::allocate(m_allocator, m_capacity);

      if (m_ptr)
      {
        for (size_t i = 0; i != m_size; ++i)
        {
          std::allocator_traits<std::allocator<T>>::construct(m_allocator, &ptr[i], m_ptr[i]);
          std::allocator_traits<std::allocator<T>>::destroy(m_allocator, &m_ptr[i]);
        }

        std::allocator_traits<std::allocator<T>>::deallocate(m_allocator, m_ptr, old_capacity);
      }

      m_ptr = ptr;
    }

    std::allocator_traits<std::allocator<T>>::construct(m_allocator, &m_ptr[m_size], _value);
    m_size += 1;
  }

  void insert(
      size_t _index,
      const T& _value
    )
  {
    assert(_index <= m_size);

    if (m_size == m_capacity)
    {
      const auto old_capacity = m_capacity;
      m_capacity = std::max<size_t>(1, m_capacity * CArray::factor);
      T* ptr = std::allocator_traits<std::allocator<T>>::allocate(m_allocator, m_capacity);

      if (m_ptr)
      {
        for (size_t i = 0; i != _index; ++i)
        {
          std::allocator_traits<std::allocator<T>>::construct(m_allocator, &ptr[i], m_ptr[i]);
          std::allocator_traits<std::allocator<T>>::destroy(m_allocator, &m_ptr[i]);
        }

        for (size_t i = _index; i != m_size; ++i)
        {
          std::allocator_traits<std::allocator<T>>::construct(m_allocator, &ptr[i + 1], m_ptr[i]);
          std::allocator_traits<std::allocator<T>>::destroy(m_allocator, &m_ptr[i]);
        }

        std::allocator_traits<std::allocator<T>>::deallocate(m_allocator, m_ptr, old_capacity);
      }

      m_ptr = ptr;
    }
    else
    {
      // copy backward
      auto it = m_ptr + m_size - 1;
      auto dest = m_ptr + m_size;
      auto tail = m_ptr + _index;
      for (; dest != tail; --it, --dest)
      {
        std::allocator_traits<std::allocator<T>>::construct(m_allocator, dest, *it);
        std::allocator_traits<std::allocator<T>>::destroy(m_allocator, it);
      }
    }

    std::allocator_traits<std::allocator<T>>::construct(m_allocator, &m_ptr[_index], _value);
    m_size += 1;
  }

  void erase(
      size_t _index
    )
  {
    assert(_index < m_size);

    std::allocator_traits<std::allocator<T>>::destroy(m_allocator, &m_ptr[_index]);

    for (size_t i = _index + 1, j = _index; i != m_size; ++i, ++j)
    {
      std::allocator_traits<std::allocator<T>>::construct(m_allocator, &m_ptr[j], m_ptr[i]);
      std::allocator_traits<std::allocator<T>>::destroy(m_allocator, &m_ptr[i]);
    }

    m_size -= 1;
  }

  void erase(
      const_iterator _from,
      const_iterator _to
    )
  {
    const auto len = std::distance(_from, _to);
    assert(len >= 0);

    for (auto it = _from; it != _to; ++it)
    {
      std::allocator_traits<std::allocator<T>>::destroy(m_allocator, it);
    }

    const auto tail = end();
    for (auto it = _to, dest = _from; it != tail; ++it, ++dest)
    {
      std::allocator_traits<std::allocator<T>>::construct(m_allocator, dest, *it);
      std::allocator_traits<std::allocator<T>>::destroy(m_allocator, it);
    }

    m_size -= len;
  }

  void clear()
  {
    for (size_t i = 0; i != m_size; ++i)
    {
      std::allocator_traits<std::allocator<T>>::destroy(m_allocator, &m_ptr[i]);
    }
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

  T& operator[](
      size_t _index
    )
  {
    assert(_index < m_size);
    return m_ptr[_index];
  }

  const T& operator[](
      size_t _index
    ) const
  {
    assert(_index < m_size);
    return m_ptr[_index];
  }

  void swap(
      CArray& _other
    )
  {
    std::swap(m_ptr, _other.m_ptr);
    std::swap(m_size, _other.m_size);
    std::swap(m_capacity, _other.m_capacity);
    std::swap(m_allocator, _other.m_allocator);
  }

private:
  T* m_ptr;
  size_t m_size;
  size_t m_capacity;
  std::allocator<T> m_allocator;
};
