
#ifndef _VL_VECTOR_H_
#define _VL_VECTOR_H_
#include <iostream>
#include <stdexcept>
#include <exception>
#include <cmath>
#include <algorithm>

#define STATIC_CAP 16
#define OUT_OF_RNG "Error: out of range"
enum Mode { Stack , Heap };
template<class T, size_t StaticCapacity = STATIC_CAP>
class vl_vector {
 public:
  typedef T *iterator;
  typedef const T *const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  /*Iterator Support*/
  iterator begin ()
  { return (iterator) data (); }
  iterator end ()
  { return (iterator) data () + _size; }
  const_iterator begin () const
  { return (const_iterator) data (); }
  const_iterator end () const
  { return (const_iterator) data () + _size; }
  const_iterator cbegin () const
  { return (const_iterator) data (); }
  const_iterator cend () const
  { return (const_iterator) data () + _size; }
  /*Reverse-Iterator Support*/
  reverse_iterator rbegin ()
  { return (reverse_iterator) end (); }
  const_reverse_iterator rbegin () const
  { return (const_reverse_iterator) end (); }
  reverse_iterator rend ()
  { return (reverse_iterator) begin (); }
  const_reverse_iterator rend () const
  { return (const_reverse_iterator) begin (); }
  const_reverse_iterator crbegin () const
  { return (const_reverse_iterator) rbegin (); }
  const_reverse_iterator crend () const
  { return (const_reverse_iterator) rend (); }

  /* ---Life-Span operations---*/
  /*Default Constructor*/
  vl_vector (): _size (0), _capacity (StaticCapacity), _dynamic_vector (nullptr),_mode(Stack)
  {}
  /*Copy Constructor*/
  vl_vector (const vl_vector<T, StaticCapacity> &other) : vl_vector ()
  {
    _capacity = other._capacity;
    _size = other._size;
    if (this->_size > StaticCapacity)
      {
        _dynamic_vector = new T[_capacity];
        _mode = Heap;
      }
    for (size_t i = 0; i < _size; ++i)
      {
        data ()[i] = other.data ()[i];
      }

  }
  /*Sequence based Constructor*/
  template<class ForwardIterator>
  vl_vector (ForwardIterator first, ForwardIterator last) :vl_vector ()
  {
    size_t distance = std::distance (first, last);
    _capacity = cap_c (_size, distance);
    _size += distance;
    if (_capacity != StaticCapacity)
      {
        _dynamic_vector = new T[_capacity];
        _mode = Heap;
      }
    size_t i = 0;
    for (auto it = first; it != last; ++it)
      {
        data ()[i++] = *it;
      }

  }
  /*Single-Value initialized Constructor*/
  vl_vector (size_t count, const T &v) : vl_vector ()
  {
    _capacity = cap_c (_size, count);
    _size = count;
    if (_capacity != StaticCapacity)
      {
        _dynamic_vector = new T[_capacity];
        _mode =Heap;
      }
    for (size_t i = 0; i < count; ++i)
      {
        data ()[i] = v;
      }
  }

  ~vl_vector ()
  {
    if(_mode == Heap)
      {delete[] _dynamic_vector;}
  }
  /*---------------------------*/
  /* Methods*/
  size_t size () const
  { return _size; }
  size_t capacity () const
  { return _capacity; }
  bool empty () const
  { return _size == 0; }
  T &at (size_t index)
  {
    if (index >= _size)
      throw std::out_of_range (OUT_OF_RNG);
    return data ()[index];
  }
  T at (size_t index) const
  {
    if (index >= _size)
      throw std::out_of_range (OUT_OF_RNG);
    return data ()[index];
  }
  T *data ()
  {return (_mode == Stack) ?  _static_vector : _dynamic_vector;}
  const T *data () const
  {return (_mode == Stack) ?  _static_vector : _dynamic_vector;}
  bool contains (const T &v) const
  {return (std::find (begin(),end(),v) != end());}
  void push_back (const T &v)
  {
    if (_capacity != cap_c (_size, 1))
      { extend_vector (1); }
    data ()[++_size - 1] = v;
  }
  void pop_back ()
  {
    if (empty())
      { return; }
    if (_size > StaticCapacity and _size - 1 <= StaticCapacity)
      { switch_to_static_vector (1); }
    --_size;
  }
  iterator insert (const_iterator pos, const T &v)
  {
    size_t distance = std::distance (cbegin (), pos);
    if (_capacity != cap_c (_size, 1))
      { extend_vector (1); }
    _size++;
    std::copy_backward (begin () + distance, end (), end () + 1);
    data ()[distance] = v;
    return begin() + distance;
  }
  template<class ForwardIterator>
  iterator insert (const_iterator pos, ForwardIterator first, ForwardIterator last)
  {
    size_t k = std::distance (first, last); /*# of elements to insert*/
    size_t distance = std::distance (cbegin (), pos); /*Distance between the beginning of the vector to the given position*/
    if (_capacity != cap_c (_size, k))
      {
        extend_vector (k);
        _size += k;
        std::copy_backward (begin () + distance, end (), end () + k);
      }
    else
      {
        std::copy_backward (begin () + distance, end (), end () + k);
        _size += k;
      }
    size_t i = distance;
    for (auto it = first; it != last; ++it)
      {
        data ()[i++] = *it;
      }
    return begin() + distance;
  }
  iterator erase (const_iterator pos)
  {
    size_t distance = std::distance (cbegin (), pos);
    if (_size == 0)
      { return begin () + distance; }
    std::copy ((iterator) pos + 1, end (), (iterator) pos);
    pop_back ();
    return begin () + distance;
  }

  iterator erase (const_iterator first, const_iterator last)
  {
    if (_size == 0)
      { return begin (); }
    size_t dis = std::distance (cbegin (), first);
    size_t k = std::distance (first, last);
//  std::copy(last,cend(),first);
    std::copy ((iterator) last, end (), (iterator) first);
    if (_size > StaticCapacity and _size - k <= StaticCapacity)
      { switch_to_static_vector (k); }
    _size -= k;
    return begin () + dis;
  }
  void clear ()
  { erase (data(), data() + _size); }



  vl_vector<T, StaticCapacity> &
  operator= (const vl_vector<T, StaticCapacity> &other)
  {
    if (this != &other)
      {
        // de-allocates old allocations
        if (_mode == Heap)
          {
            delete[] _dynamic_vector;
            _dynamic_vector = nullptr;
          }

        _size = other.size ();
        _capacity = other.capacity ();
        _mode = Stack;
        if (_size > StaticCapacity)
          {
            _dynamic_vector = new T[_capacity];
            _mode = Heap;
          }
        for (size_t i = 0; i < _size; ++i)
          {data ()[i] = other.data ()[i];}
      }
    return *this;
  }
  T &operator[] (size_t index)
  { return data ()[index]; }
  T operator[] (size_t index) const
  { return data ()[index]; }
  bool operator== (const vl_vector<T, StaticCapacity> &rhs) const
  {
    if (_size != rhs.size ())
      { return false; }
    for (size_t i = 0; i < _size; i++)
      {
        if (data ()[i] != rhs.data ()[i])
          { return false; }
      }
    return true;
  }
  bool operator!= (const vl_vector<T, StaticCapacity> &rhs) const
  { return !(rhs == *this); }
  void print ();

 private:
  T _static_vector[StaticCapacity]; /*static_container located in stack*/
  T *_dynamic_vector; /*dynamic container allocated on the heap*/
  Mode _mode;
  size_t _size{}; /* # of T elements the vector*/
  size_t _capacity; /* current maximum # of T elements that can be stored in vector*/
  size_t cap_c (size_t size, size_t k); /* Capacity function*/
  void extend_vector (size_t k);
  void switch_to_static_vector (size_t k);

};

template<class T, size_t StaticCapacity>
size_t vl_vector<T, StaticCapacity>::cap_c (size_t size, size_t k)
{
  return (size + k <= StaticCapacity) ? StaticCapacity : std::floor (
      (3 * (size + k)) / 2);
}
template<class T, size_t StaticCapacity>
void vl_vector<T, StaticCapacity>::extend_vector (size_t k)
{
  if (_size + k > _capacity)
    {_capacity = cap_c (_size, k);}
  T *new_dynamic_vector = new T[_capacity];
  std::copy (begin(),end(),new_dynamic_vector);
  delete[] _dynamic_vector;
  _dynamic_vector = new_dynamic_vector;
  _mode = Heap;
}

template<class T, size_t StaticCapacity>
void vl_vector<T, StaticCapacity>::switch_to_static_vector (size_t k)
{
  _capacity = StaticCapacity;
  for (size_t i = 0; i < _size - k; ++i)
    {_static_vector[i] = _dynamic_vector[i];}
  delete[] _dynamic_vector;
  _dynamic_vector = nullptr;
  _mode = Stack;
}

template<class T, size_t StaticCapacity>
void vl_vector<T, StaticCapacity>::print ()
{
  iterator it = this->begin ();
  if (_size > StaticCapacity)
    {
      std::cout << "Dynamic Vector [";
    }
  else
    {
      std::cout << "Static Vector [";
    }
  while (it != end ())
    {
      std::cout << *it << " ";
      it++;
    }
  std::cout << "]" << std::endl;
  std::cout << "Size: " << _size << " Capacity: " << _capacity << std::endl;
}



#endif //_VL_VECTOR_H_
