
#include <exception>
#include <iostream>
#include <typeinfo>
#include <string>
#include <vector>
#include <map>
#include <strstream>
#include <cmath>
#include "json.h"
using namespace std;

const type_info& json_value::type() {
  return typeid(*this);
}

json_value::~json_value() {
}

json_string::json_string(string* x) : str(x) {
}

json_string::~json_string() {
  delete str;
}

ostream& json_string::write(ostream& os) const {
  os<<'"';
  for(string::const_iterator iter=str->begin(); iter!=str->end(); ++iter) {
    switch(*iter) {
    case '"' : os<<"\\\""; break;
    case '\\': os<<"\\\\"; break;
    case '/' : os<<"\\/";  break;
    case '\b': os<<"\\b";  break;
    case '\f': os<<"\\f";  break;
    case '\n': os<<"\\n";  break;
    case '\r': os<<"\\r";  break;
    case '\t': os<<"\\t";  break;
    default  : os<<*iter;  break;
    }
  }
  os<<'"';
  return os;
}
  
std::ostream& operator<<(std::ostream& os, const json_value* value) {
  return value->write(os);
}
  
json_array::json_array(const value_type* arr)
  : arr(arr) {
}

json_array::~json_array() {
  for(value_type::const_iterator iter=arr->begin(); iter!=arr->end(); ++iter)
    delete *iter;
  delete arr;
}

ostream& json_array::write(ostream& os) const {
  cout<<'[';
  for(value_type::const_iterator iter=arr->begin(); iter != arr->end();) {
    (*iter)->write(os);
    ++iter;
    if(iter!=arr->end()) os <<',';
  }
  os<<']';
}

json_object::json_object(value_type* obj)
  : obj(obj) {
}

json_object::~json_object() {
  for(value_type::const_iterator iter=obj->begin(); iter!=obj->end(); ++iter) {
    delete iter->first;
    delete iter->second;
  }
  delete obj;
}

ostream& json_object::write(ostream& os) const {
  os<<'{';
  for(value_type::const_iterator iter=obj->begin(); iter!=obj->end();) {
    os << iter->first <<':' << iter->second;
    ++iter;
    if(iter!=obj->end()) os <<',';
  }
  os<<'}';
}

json_bool::json_bool(const bool x) : val(x) {
}

ostream& json_bool::write(ostream& os) const {
  os<< (val ? "true" : "false");
  return os;
}

json_number::json_number(const double number)
  : number(number) {
}

ostream& json_number::write(ostream&os) const {
  os<<number;
  return os;
}
