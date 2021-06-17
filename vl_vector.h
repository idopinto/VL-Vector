
#ifndef _VL_VECTOR_H_
#define _VL_VECTOR_H_
#include <exception>
#include <cmath>
template<class T, size_t StaticCapacity = 16>
class vl_vector {
 public:
  /* ---Life-Span operations---*/
  vl_vector ();
  vl_vector (const vl_vector<T, StaticCapacity> &other);
  template<class InputIterator>
  vl_vector (InputIterator first, InputIterator last);
  vl_vector (T v, size_t count);
  ~vl_vector ();
  /*---------------------------*/
  /* Methods*/
  size_t size () const;
  size_t capacity () const;
  bool empty ();
  T &at (size_t index);
  T *data ();
  const T *data () const;
  bool contains (const T &v) const;

  void push_back (const T &v);
  void pop_back ();
  T* insert(T* pos,const T &v);

  /*Iterator Support*/
  typedef T* iterator;
  typedef const T* const_iterator;

  iterator begin(){return (iterator)data();}
  iterator end(){return (iterator)data()+_size;}
  const_iterator begin() const {return (const_iterator)data();}
  const_iterator end() const {return (const_iterator)data()+_size;}
  const_iterator cbegin() const{return (const_iterator)data();}
  const_iterator cend() const {return (const_iterator)data()+_size;}

  typedef  std::reverse_iterator<iterator> reverse_iterator;
  typedef  std::reverse_iterator<const_iterator> const_reverse_iterator;

  reverse_iterator rbegin(){ return end();}
  const_reverse_iterator rbegin()const { return end (); }
  reverse_iterator rend (){ return begin(); }
  const_reverse_iterator rend () const { return begin(); }
  const_reverse_iterator crbegin() const { return rbegin(); }
  const_reverse_iterator crend  () const { return rend (); }

  vl_vector<T, StaticCapacity> &
  operator= (const vl_vector<T, StaticCapacity> &other);
  T &operator[] (size_t index);
  T operator[] (size_t index) const;
  bool operator== (vl_vector<T, StaticCapacity> &rhs);
  bool operator!= (vl_vector<T, StaticCapacity> &rhs);
  void print();

 private:
  T _static_vector[StaticCapacity]; /*static_container located in stack*/
  T *_dynamic_vector; /*dynamic container allocated on the heap*/
  size_t _size; /* # of T elements the vector*/
  size_t _capacity; /* current maximum # of T elements that can be stored in vector*/
  size_t CapC (size_t size, size_t k); /* Capacity function*/
  void extend_vector ();
  void switch_to_static_vector (size_t k);


};
/*Default Constructor*/
template<class T, size_t StaticCapacity>
vl_vector<T, StaticCapacity>::vl_vector ()
{
  _size = 0;
  _capacity = StaticCapacity;
  _dynamic_vector = nullptr;
}
/*Copy Constructor*/
template<class T, size_t StaticCapacity>
vl_vector<T, StaticCapacity>::vl_vector (const vl_vector<T, StaticCapacity> &other)
    : vl_vector ()
{
  if (this->_size > StaticCapacity)
    {
      _dynamic_vector = new T[_capacity];
    }
  for (int i = 0; i < _size; ++i)
    {
      data ()[i] = other._static_vector[i];
    }
}
/*Sequence based Constructor*/
template<class T, size_t StaticCapacity>
template<class InputIterator>
vl_vector<T, StaticCapacity>::vl_vector (InputIterator first, InputIterator last)
    : vl_vector ()
{
  size_t distance = std::distance (first, last);
  _capacity = CapC (_size, distance);
  if (_capacity != StaticCapacity)
    {
      _dynamic_vector = new T[_capacity];
    }
  size_t i = 0; // can iterate over first?
  for (auto it = first; it != last; ++it)
    {
      data ()[i++] = *it;
    }
}
/*Single-Value initialized Constructor*/
template<class T, size_t StaticCapacity>
vl_vector<T, StaticCapacity>::vl_vector (T v, size_t count) : vl_vector ()
{
  _capacity = CapC (_size, count);
  if (_capacity != StaticCapacity)
    {
      _dynamic_vector = new T[_capacity];
    }
  for (size_t i = 0; i < count; ++i)
    {
      data ()[i] = v;
    }
}
/*Destructor*/
template<class T, size_t StaticCapacity>
vl_vector<T, StaticCapacity>::~vl_vector ()
{
  if (_size > StaticCapacity)
    {
      delete[] _dynamic_vector;
    }
}
/*size*/
template<class T, size_t StaticCapacity>
size_t vl_vector<T, StaticCapacity>::size () const
{ return _size; }
/*capacity*/
template<class T, size_t StaticCapacity>
size_t vl_vector<T, StaticCapacity>::capacity () const
{ return _capacity; }
/*empty*/
template<class T, size_t StaticCapacity>
bool vl_vector<T, StaticCapacity>::empty ()
{ return _size == 0; }
/*at*/
template<class T, size_t StaticCapacity>
T &vl_vector<T, StaticCapacity>::at (size_t index)
{
  if (index >= _size)
    throw std::out_of_range ("ERROR");
  return data ()[index];
}
/*Data*/
template<class T, size_t StaticCapacity>
T *vl_vector<T, StaticCapacity>::data ()
{ return (_size <= StaticCapacity) ? _static_vector : _dynamic_vector; }

template<class T, size_t StaticCapacity>
const T *vl_vector<T, StaticCapacity>::data () const
{
  return (_size <= StaticCapacity) ? _static_vector : _dynamic_vector;
}
/*push_back*/
template<class T, size_t StaticCapacity>
void vl_vector<T, StaticCapacity>::push_back (const T &v)
{
  if (_capacity != CapC (_size, 1))
    { extend_vector (); }
  _size++;
  data ()[_size - 1] = v;

}
template<class T, size_t StaticCapacity>
size_t vl_vector<T, StaticCapacity>::CapC (size_t size, size_t k)
{
  return (size + k <= _capacity) ? _capacity : std::floor (
      (3 * (size + k)) / 2);
}
template<class T, size_t StaticCapacity>
void vl_vector<T, StaticCapacity>::extend_vector ()
{
  _capacity = CapC (_size, 1);
  T *new_dynamic_vector = new T[_capacity];
  for (size_t i = 0; i < _size; ++i)
    {
      T element = data ()[i];
      new_dynamic_vector[i] = element;
    }
  delete[] _dynamic_vector;
  _dynamic_vector = new_dynamic_vector;
}

template<class T, size_t StaticCapacity>
void vl_vector<T, StaticCapacity>::pop_back ()
{
  if (_size == 0)
    { return; }
  if (_size > StaticCapacity and _size - 1 <= StaticCapacity)
    { switch_to_static_vector (1); }
  --_size;
}
template<class T, size_t StaticCapacity>
void vl_vector<T, StaticCapacity>::switch_to_static_vector (size_t k)
{
  _capacity = StaticCapacity;
  for (size_t i = 0; i < _size - k; ++i)
    {
      _static_vector[i] = _dynamic_vector[i];
    }
  delete[] _dynamic_vector;
}
template<class T, size_t StaticCapacity>
bool vl_vector<T, StaticCapacity>::contains (const T &v) const
{
  for (int i = 0; i < _size; ++i)
    {
      if (data ()[i] == v)
        { return true; }
    }
  return false;
}

/*Operators*/

template<class T, size_t StaticCapacity>
T vl_vector<T, StaticCapacity>::operator[] (size_t index) const
{
  return data ()[index];
}
template<class T, size_t StaticCapacity>
T &vl_vector<T, StaticCapacity>::operator[] (size_t index)
{
  return data ()[index];
}
template<class T, size_t StaticCapacity>
bool
vl_vector<T, StaticCapacity>::operator== (vl_vector<T, StaticCapacity> &rhs)
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
template<class T, size_t StaticCapacity>
bool
vl_vector<T, StaticCapacity>::operator!= (vl_vector<T, StaticCapacity> &rhs)
{
  return !(rhs == *this);
}
template<class T, size_t StaticCapacity>
vl_vector<T, StaticCapacity> &
vl_vector<T, StaticCapacity>::operator= (const vl_vector<T, StaticCapacity> &other)
{
  if (this != &other)
    {
      // de-allocates old allocations
      if (_size > StaticCapacity)
        {
          delete[] _dynamic_vector;
        }
      //copy rows&cols
      this->_size = other.size ();
      this->_capacity = other.capacity ();
      this->_dynamic_vector = nullptr;
      if (_size > StaticCapacity)
        {
          this->_dynamic_vector = new T[_capacity];
        }
      for (int i = 0; i < _size; ++i)
        {
          this->data ()[i] = other.data ()[i];
        }
    }
  return *this;
}
template<class T, size_t StaticCapacity>
void vl_vector<T, StaticCapacity>::print ()
{
  iterator it = this->begin();
  if(_size > StaticCapacity)
    {
      std::cout<< "Dynamic Vector [";
    }
  else
    {
      std::cout<<"Static Vector [";
    }
  while(it!= end())
    {
      std::cout<< *it <<" ";
      it++;
    }
    std::cout<<"]"<<std::endl;
  std::cout<<"Size: "<<_size<<" Capacity: "<<_capacity<<std::endl;
}
template<class T, size_t StaticCapacity>
T *vl_vector<T, StaticCapacity>::insert (T *pos, const T &v)
{
  iterator last = end();
  if (_capacity != CapC (_size, 1))
    {
      extend_vector ();
    }
    std::move (pos,last,pos + 1);
  _size++;
  *pos = v;

  return pos;
}

#endif //_VL_VECTOR_H_
