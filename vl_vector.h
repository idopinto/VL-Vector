
#ifndef _VL_VECTOR_H_
#define _VL_VECTOR_H_
#include <iostream>
#include <stdexcept>
#include <exception>
#include <cmath>
#include <algorithm>

#define STATIC_CAP 16
#define OUT_OF_RNG "Error: out of range"

/**
 * enum Mode indicates whether in STACK or HEAP
 */
enum Mode { STACK , HEAP};

template<class T, size_t StaticCapacity = STATIC_CAP>
class vl_vector {
 public:

  typedef T *iterator;
  typedef const T *const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  /**Iterator Support**/
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
  /**Reverse-Iterator Support**/
  virtual reverse_iterator rbegin ()
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

  /** ----------Life-Span operations------------**/

/**
 * Default Constructor
 */
  vl_vector ():_dynamic_vector(nullptr)
  {}

  /**
   * Copy Constructor
   * @param other
   */
  vl_vector (const vl_vector<T, StaticCapacity> &other) :
  _size(other._size),_capacity(other._capacity)
  {
    build_dynamic_vec_if_needed();
    for (size_t i = 0; i < _size; ++i)
       data ()[i] = other.data ()[i];
  }

  /**
   * Sequence based Constructor
   * @tparam ForwardIterator - Iterator of T elements
   * @param first
   * @param last
   */
  template<class ForwardIterator>
  vl_vector (ForwardIterator first, ForwardIterator last) :vl_vector ()
  {insert(begin(),first,last);}

  /**
   * Single-Value initialized Constructor
   * @param count
   * @param v
   */
  vl_vector (size_t count, const T &v) :
  _size(count),_capacity(cap_c(0,count))
  {
    build_dynamic_vec_if_needed();
    std::fill(begin(),end(),v);
  }

  /**
   * Destructor
   */
  virtual ~vl_vector ()
  {
    if(_mode == HEAP)
      {delete[] _dynamic_vector;}
  }
  /**------------------Methods--------------------**/
  /**
   *
   * @return current size of vl_vector
   */
 virtual size_t size () const
  { return _size; }
  /**
   *
   * @return current capacity of vl_vector
   */
  size_t capacity () const
  { return _capacity; }

  /**
   *
   * @return true if vl_vector is empty. false otherwise.
   */
  virtual bool empty () const
  { return size() == 0; }
  /**
   *
   * @param index
   * @return value associated with index in vl_vector by reference
   * throws exception if out of range
   */
  T &at (size_t index)
  {
    if (index >= _size)
      throw std::out_of_range (OUT_OF_RNG);
    return data ()[index];
  }
  /**
   * const version of at
   * @param index
   * @return value associated with index in vl_vector by value
   */
  T at (size_t index) const
  {
    if (index >= _size)
      throw std::out_of_range (OUT_OF_RNG);
    return data ()[index];
  }
/**
 *
 * @return pointer of current array that holds elements in
 * Heap or stack based on current mode.
 */
  T *data ()
  {return (_mode == STACK) ? _static_vector : _dynamic_vector;}
  /**
   * const version of data()
   * @return pointer of current array that holds elements in
    * Heap or stack based on current mode.
   */
  const T *data () const
  {return (_mode == STACK) ? _static_vector : _dynamic_vector;}
  /**
   *
   * @param v
   * @return true if v is in vl_vector,false otherwise.
   */
  bool contains (const T &v) const
  {return (std::find (begin(),end(),v) != end());}
  /**
   * push_back adds v to the end of vl_vector
   * @param v - T element
   */
  virtual void push_back (const T &v)
  {
    if (_capacity != cap_c (_size, 1))
      { extend_vector (1); }
    data ()[++_size - 1] = v;
  }
  /**
   * pop_back removes the last element in vl_vector
   */
  virtual void pop_back ()
  {
    if (empty())
      { return; }
    if (_size > StaticCapacity and _size - 1 <= StaticCapacity)
      { switch_to_static_vector (1); }
    --_size;
  }
  /**
   * insert 1 inserts v before position iterated
   * @param pos - const_iterator(supports also non-const iterator)
   * @param v - T element to add
   * @return iterator which points the to new element v
   * (if const_iterator given, returns const_iterator)
   */
  virtual iterator insert (const_iterator pos, const T &v)
  {return insert(pos,(const_iterator)&v,(const_iterator)&v+1);}

  /**
   * insert 2 - inserts sequence [first,last) before position iterator in vl_vector
   * @tparam ForwardIterator
   * @param pos  - const_iterator(supports also non-const iterator)
   * @param first
   * @param last
   * @return iterator of vl_vector that points to
   * the first element in the sequence . (if const_iterator given, returns const_iterator)
   */
  template<class ForwardIterator>
  iterator insert (const_iterator pos, ForwardIterator first, ForwardIterator last)
  {
    size_t k = std::distance (first, last);
    /*# of elements to insert*/
    size_t distance = std::distance (cbegin (), pos);
    /*Distance between the beginning of the vector to the given position*/
    if(_size + k > _capacity)
      { extend_vector (k);}
    std::move_backward (begin () + distance, end (), end () + k);
    size_t i = distance;
    for (auto it = first; it != last; ++it)
      {
        data ()[i++] = *it;
      }
      _size+=k;
    return begin() + distance;
  }

  /**
   * erase 1- removes the element which pos points to.
   * @param pos - const_iterator (supports non-const iterator)
   * @return iterator to the element which is after the removed element
   * (if const_iterator given, returns const_iterator)
   */
  iterator erase (const_iterator pos)
  {
    size_t distance = std::distance (cbegin (), pos);
    if (_size == 0)
      { return begin () + distance; }
    std::copy ((iterator) pos + 1, end (), (iterator) pos);
    pop_back ();
    return begin () + distance;
  }

  /**
   * erase 2-  removes sequence [first,last) from vl_vector
   * @param first -const_iterator (supports non-const iterator)
   * @param last - const_iterator (supports non-const iterator)
   * @return iterator to the element which is after the removed element
   * (if const_iterator given, returns const_iterator)
   */
  iterator erase (const_iterator first, const_iterator last)
  {
    if (_size == 0)
      { return begin (); }
    size_t distance = std::distance (cbegin (), first);
    size_t k = std::distance (first, last);
    std::copy ((iterator) last, end (), (iterator) first);
    if (_size > StaticCapacity and _size - k <= StaticCapacity)
      { switch_to_static_vector (k); }
    _size -= k;
    return begin () + distance;
  }

  /**
   * clear removes all the elements in vl_vector.
   */
  virtual void clear ()
  { erase (data(), data() + size()); }


  /**------------------Operators--------------------**/
  /**
   * Assignment operator
   * @param other
   * @return *this
   */
  vl_vector<T, StaticCapacity> &
  operator= (const vl_vector<T, StaticCapacity> &other)
  {
    if (this != &other)
      {
        // de-allocates old allocations
        if (_mode == HEAP)
          {
            delete[] _dynamic_vector;
            _dynamic_vector = nullptr;
          }
        _size = other._size;
//        _size = other.size();
        _capacity = other.capacity ();
        _mode = STACK;
        build_dynamic_vec_if_needed();
        for (size_t i = 0; i < _size; ++i)
          {data ()[i] = other.data ()[i];}
      }
    return *this;
  }
  /**
   * Element access by brackets.
   * @param index
   * @return value associated with vl_vector (based on mode)
   * no exception thrown if out_of_range
   */
  virtual T &operator[] (size_t index) noexcept
  { return data ()[index]; }
    /**
   * const version -Element access by brackets.
   * @param index
   * @return value associated with vl_vector (based on mode)
   * no exception thrown if out_of_range
   */

  virtual T operator[] (size_t index) const noexcept
  { return data ()[index]; }
  /**
   *  ==  operator. const
   * @param rhs
   * @return true if equal. false otherwise.
   */
  virtual bool operator== (const vl_vector<T, StaticCapacity> &rhs) const
  {
    if (_size != rhs._size)
      { return false; }
    for (size_t i = 0; i < _size; i++)
      {
        if (data ()[i] != rhs.data ()[i])
          { return false; }
      }
    return true;
  }
  /**
   *  != operator. const
   * @param rhs
   * @return
   */
  virtual bool operator!= (const vl_vector<T, StaticCapacity> &rhs) const
  { return !(rhs == *this); }

  void print() ;
 protected:
  /**----private attributes-----**/
  T _static_vector[StaticCapacity];/**static_container located in stack**/
  T *_dynamic_vector = nullptr;    /**dynamic container allocated on the heap**/
  Mode _mode =STACK;               /** enum indicator **/
  size_t _size = 0;                    /** # of T elements the vector8*/
  size_t _capacity = StaticCapacity;   /** current maximum # of T elements
                                    *that can be stored in vector**/

  /**---------helper functions-----------**/
  size_t cap_c (size_t size, size_t k); /**Capacity function**/
  void extend_vector (size_t k);
  void switch_to_static_vector (size_t k);
  void build_dynamic_vec_if_needed();

};

/**
 * this function checks if dynamic vector needed.
 * mostly used for constructors.
 * @tparam T
 * @tparam StaticCapacity
 */
template<class T, size_t StaticCapacity>
void vl_vector<T, StaticCapacity>::build_dynamic_vec_if_needed()
{
    if (_size > StaticCapacity)
    {
        _dynamic_vector = new T[_capacity];
        _mode = HEAP;
    }
}

/**
 * this function returns updated capacity based on size+k
 * @tparam T
 * @tparam StaticCapacity
 * @param size
 * @param k
 * @return
 */
template<class T, size_t StaticCapacity>
size_t vl_vector<T, StaticCapacity>::cap_c (size_t size, size_t k)
{
  return (size + k <= StaticCapacity) ? StaticCapacity : std::floor (
      (3 * (size + k)) / 2);
}

/**
 * this function switch to dynamic vector. (from static or dynamic vectors)
 * @tparam T
 * @tparam StaticCapacity
 * @param k
 */
template<class T, size_t StaticCapacity>
void vl_vector<T, StaticCapacity>::extend_vector (size_t k)
{
  if (_size + k > _capacity)
    {_capacity = cap_c (_size, k);}
  T *new_dynamic_vector = new T[_capacity];
  std::copy (begin(),end(),new_dynamic_vector);
  delete[] _dynamic_vector;
  _dynamic_vector = new_dynamic_vector;
  _mode = HEAP;
}

/**
 * this function switch to static vector. delete  dynamic vector entirely
 * @tparam T
 * @tparam StaticCapacity
 * @param k
 */
template<class T, size_t StaticCapacity>
void vl_vector<T, StaticCapacity>::switch_to_static_vector (size_t k)
{
  _capacity = StaticCapacity;
  for (size_t i = 0; i < _size - k; ++i)
    {_static_vector[i] = _dynamic_vector[i];}
  delete[] _dynamic_vector;
  _dynamic_vector = nullptr;
  _mode = STACK;
}


template<class T, size_t StaticCapacity>
void vl_vector<T, StaticCapacity>::print ()
{
    iterator it = this->begin ();
    if (_size > StaticCapacity)
    {std::cout << "Dynamic Vector [";}
    else
    {std::cout << "Static Vector [";}
    while (it != end ())
    {
        std::cout << *it << " ";
        it++;
    }
    std::cout << "]" << std::endl;
    std::cout << "Size: " << _size << " Capacity: " << _capacity << std::endl;
}
#endif //_VL_VECTOR_H_
