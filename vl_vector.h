
#ifndef _VL_VECTOR_H_
#define _VL_VECTOR_H_
#include <exception>

template <class T,size_t StaticCapacity = 16>
class vl_vector{
    public:
    /* Life-Span operations*/
    // TODO DefaultConstructor -  DONE
    vl_vector():
    {
        _size = 0;
        _capacity = StaticCapacity;
        _dynamic_vector = nullptr;
        for (int i = 0; i < _capacity; ++i) {_static_vector[i] = 0;}
    }

    // TODO CopyConstructor - DONE
    vl_vector(const vl_vector<T,StaticCapcity> &other) : vl_vector()
    {
      if(this->_size > StaticCapacity)
        {
          _dynamic_vector = new T[_capacity];
        }
      for (int i = 0; i < _size; ++i)
        {
          data()[i] = other._static_vector[i];
        }
    }

    // TODO Sequence based Constructor -NOT DONE
    template<class InputIterator>
    vl_vector(InputIterator first, InputIterator last) : vl_vector()
    {

      for (int it = first; it != last ; ++i)
        {

        }
    }

    // TODO Single-value initialized constructor - NOT DONE
    vl_vector(T v, size_t count) : vl_vector()
    {
    }

    // TODO Destructor - DONE
    ~vl_vector()
    {
      if(_size > StaticCapacity)
        {
          delete[] _dynamic_vector;
        }
    }
    /*--------------------*/
    /* Methods*/
    // TODO size
    /**
    * this function returns the current size of the vector(# of elements)
    * return type size_t
    **/
    size_t size() const {return _size;}


    // TODO capacity
    /**
     * this function the current capacity of the vector
     * return type size_t. (NO NEED to calculate capC)
     **/
    size_t capacity() const {return _capacity;}
    // TODO empty
    /**
     * this function returns true if the vector is empty. false otherwise
     * return type bool
     */
     bool empty()
    {

    }

    // TODO at
    T& at(size_t index)
    {
       if((index < 0) ||(index >= _capacity))
         {
           throw std::out_of_range; // check out how to throw
         }
        return data()[i];
    }
    // TODO data

    T* data()
    {
      return (_size < StaticCapacity) ? _static_vector:_dynamic_vector;
    }
    // TODO pushback

    void push_back(T &v)
    {

    }
    private:
    T _static_vector[StaticCapacity]; /*static_container located in stack*/
    T* _dynamic_vector; /*dynamic container allocated on the heap*/
    size_t _size; /* # of T elements the vector*/
    size_t _capacity; /* current maximum # of T elements that can be stored in vector*/
};
#endif //_VL_VECTOR_H_
