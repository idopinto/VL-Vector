//
// Created by Ido_2 on 6/22/2021.
//

#ifndef _VL_STRING_H_
#define _VL_STRING_H_
#include "vl_vector.h"
#include <cstring>
template<size_t StaticCapacity=STATIC_CAP>

class vl_string : public vl_vector<char,StaticCapacity> {
 public:
  /**
   * Default Constructor
   */
  vl_string () : vl_vector<char,StaticCapacity> (1, '\0')
  {}
  /**
   * Copy constructor
   * @param other
   */
  vl_string (const vl_string<StaticCapacity> &other)=default;

  /**
   * Implicit Constructor.
   *  Receives const char*
   * @param str
   */
  vl_string(const char* str): vl_string()
  {this->insert(this->begin(),str, str+strlen(str));}

  /**
   *
   * @return string length (size-1)
   */
  size_t size () const override
  {return this->_size-1;}

  /**
   * Push back inserts char to the end of the string
   * @param v - character to push
   */
  void push_back (const char &v) override
  {this->insert (this->end()-1,v);}

  /**
   * Pop_back removes the last character in the string
   * (Not including Null terminator)
   */
  void pop_back () override
  {this->erase (this->end()-2,this->end()-1);}

  /**
   *  contains searches for sub-string in vl_string
   * @param c
   * @return true if sub-string c found, false otherwise.
   */
  bool contains(const char* c) const
  {return (std::search(this->begin(), this->end(),
                       c,c +strlen(c))!=this->end());}

  /**
   * += operator
   * @param v - instance of vl_string
   * @return *this
   */
  vl_string<>& operator+=(const vl_string<> &v)
  {
    this->insert (this->end()-1,v.begin(),v.end()-1);
    return *this;
  }
  /**
 * += operator
 * @param str - const char* string
 * @return *this
 */
  vl_string<>& operator+=(const char* str)
  {
    this->insert (this->end()-1,str,str+ strlen (str));
    return *this;
  }
  /**
 * += operator
 * @param ch - character (char)
 * @return *this
 */
  vl_string<>& operator+=( const char &ch)
  {
    this->push_back (ch);
    return *this;
  }
/**
 * operator+
 * @param rhs - instance of vl_string
 * @return copy of the addition.
 */
  vl_string<> operator+(const vl_string<> &rhs) const
  {
    vl_string<> new_str(*this);
    new_str +=rhs;
    return new_str;
  }
  /**
 * operator+
 * @param rhs - const char* string
 * @return copy of the addition.
 */
  vl_string<> operator+(const char* rhs) const
  {
    vl_string<> new_str(*this);
    new_str += rhs;
    return new_str;
  }
  /**
 * operator+
 * @param ch - character
 * @return copy of the addition.
 */
  vl_string<> operator+( const char &ch)
  {
    vl_string<> new_str(*this);
    new_str += ch;
    return new_str;
  }


  /**
   * implicit type conversion to const char*
   * @return
   */
  operator const char*() const {
    return  (char*)this->data();
  }
};
#endif //_VL_STRING_H_
