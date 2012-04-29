
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

json_string::json_string(string* x)
  : value(x) {
}

json_string::~json_string() {
  delete value;
}

json_string::operator const value_type&() const {
  return *value;
}

ostream& json_string::write(ostream& os) const {
  os<<'"';
  for(string::const_iterator iter=value->begin(); iter!=value->end(); ++iter) {
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
  
std::ostream& operator<<(std::ostream& os, const json_value* x) {
  return x->write(os);
}
  
json_array::json_array(const value_type* x)
  : value(x) {
}

json_array::~json_array() {
  for(value_type::const_iterator iter=value->begin(); iter!=value->end(); ++iter)
    delete *iter;
  delete value;
}

json_array::operator const value_type&() const {
  return *value;
}

ostream& json_array::write(ostream& os) const {
  cout<<'[';
  for(value_type::const_iterator iter=value->begin(); iter != value->end();) {
    (*iter)->write(os);
    ++iter;
    if(iter!=value->end()) os <<',';
  }
  os<<']';
}

json_object::json_object(value_type* x)
  : value(x) {
}

json_object::~json_object() {
  for(value_type::const_iterator iter=value->begin(); iter!=value->end(); ++iter) {
    delete iter->first;
    delete iter->second;
  }
  delete value;
}

json_object::operator const value_type&() const {
  return *value;
}

ostream& json_object::write(ostream& os) const {
  os<<'{';
  for(value_type::const_iterator iter=value->begin(); iter!=value->end();) {
    os << iter->first <<':' << iter->second;
    ++iter;
    if(iter!=value->end()) os <<',';
  }
  os<<'}';
}

json_bool::json_bool(const bool x)
  : value(x) {
}

ostream& json_bool::write(ostream& os) const {
  os<< (value ? "true" : "false");
  return os;
}

json_bool::operator bool() const {
  return value;
}

json_number::json_number(const double x)
  : value(x) {
}

json_number::operator const double&() const {
  return value;
}

ostream& json_number::write(ostream&os) const {
  os<<value;
  return os;
}

ostream& json_null::write(std::ostream& os) const {
  os<<"null";
  return os;
}
