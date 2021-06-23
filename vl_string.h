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
  vl_string () : vl_vector<char,StaticCapacity> (1, '\0')
  {}
  vl_string (const vl_string<StaticCapacity> &other)=default;

  vl_string(const char* str): vl_string()
  {this->insert(this->begin(),str, str+strlen(str));}

  size_t size () const override
  {return this->_size-1;}

  void push_back (const char &v) override
  {this->insert (this->end()-1,v);}

  void pop_back () override
  {this->erase (this->end()-2,this->end()-1);}

  bool contains(const char* c) const
  {return (std::search(this->begin(), this->end(),c,c +strlen(c))!=this->end());}

  vl_string<>& operator+=(const vl_string<> &v)
  {
    this->insert (this->end()-1,v.begin(),v.end()-1);
    return *this;
  }
  vl_string<>& operator+=(const char* str)
  {
    this->insert (this->end()-1,str,str+ strlen (str));
    return *this;
  }
  vl_string<>& operator+=( const char &str)
  {
    this->push_back (str);
    return *this;
  }

  vl_string<> operator+(const vl_string<> &rhs) const
  {
    vl_string<> new_str(*this);
    new_str +=rhs;
    return new_str;
  }
  vl_string<> operator+(const char* rhs) const
  {
    vl_string<> new_str(*this);
    new_str += rhs;
    return new_str;
  }
  vl_string<> operator+( const char &ch)
  {
    vl_string<> new_str(*this);
    new_str += ch;
    return new_str;
  }

  operator const char*() const {
    return  (char*)this->data();
  }

};
#endif //_VL_STRING_H_
