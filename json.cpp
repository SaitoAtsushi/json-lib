#include <stdio.h>
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
  for(string::iterator iter=str->begin(); iter!=str->end(); ++iter) {
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
  
json_array::json_array()
  : arr(new value_type) {
}

json_array::~json_array() {
  for(value_type::iterator iter=arr->begin(); iter!=arr->end(); ++iter)
    delete *iter;
  delete arr;
}

json_array& json_array::insert(json_value* x) {
  arr->push_back(x);
  return *this;
}

ostream& json_array::write(ostream& os) const {
  cout<<'[';
  for(value_type::iterator iter=arr->begin(); iter != arr->end();) {
    (*iter)->write(os);
    ++iter;
    if(iter!=arr->end()) os <<',';
  }
  os<<']';
}

json_object::json_object()
  : obj(new value_type) {
}

json_object::~json_object() {
  for(value_type::iterator iter=obj->begin(); iter!=obj->end(); ++iter) {
    delete iter->first;
    delete iter->second;
  }
  delete obj;
}

ostream& json_object::write(ostream& os) const {
  os<<'{';
  for(value_type::iterator iter=obj->begin(); iter!=obj->end();) {
    os << iter->first <<':' << iter->second;
    ++iter;
    if(iter!=obj->end()) os <<',';
  }
  os<<'}';
}

json_object& json_object::insert(json_string* k, json_value* v) {
  obj->insert(map<json_string*, json_value*>::value_type(k, v));
  return *this;
}

json_bool::json_bool(bool x) : val(x) {
}

ostream& json_bool::write(ostream& os) const {
  os<< (val ? "true" : "false");
  return os;
}

json_number::json_number(double number)
  : number(number) {
}

ostream& json_number::write(ostream&os) const {
  os<<number;
  return os;
}

string integer_to_utf8(int x) {
  string str;
  static struct {
    int maxlimit; int flagbit;
  } table[]= {
    {0x80, 0}, {0x20, 0xC0}, {0x10, 0xE0}, {8, 0xF0}, {4, 0xF8}, {2, 0xFC}
  };
  int i;
  char result[6];
  for(i=0; i<6; i++, x/=0x40) {
    if(x < table[i].maxlimit) { result[i] = x | table[i].flagbit; break; }
    else result[i] = (x%0x40)|0x80;
  }
  if(i==6) throw json_exception();
  for(;i>=0; i--) str+=result[i];
  return str;
}

parse::parse(istream &in)
  : in(in) {
}

json_object* parse::object() {
  char ch;
  json_object* obj=new json_object();
  json_string* k;
  json_value* v;

  while(1) {
    while(isspace(ch=in.get()));
    if(ch=='}') break;
    else if(ch=='"') k=parse::string();
    else throw json_exception();
    while(isspace(ch=in.get()));
    if(ch!=':') throw json_exception();
    v=parse::value();
    obj->insert(k, v);
    while(isspace(ch=in.get()));
    if(ch==',')  ;
    else if(ch=='}') break;
    else throw json_exception();
  }
  return obj;
}

json_array* parse::array() {
  char ch;
  json_array* jarray=new json_array();

  while(1) {
    while(isspace(ch=in.get()));
    if(ch==']') break;
    else in.unget();
    jarray->insert(parse::value());
    while(isspace(ch=in.get()));
    if(ch==',');
    else if(ch==']') break;
    else throw json_exception();
  }
  return jarray;
}

int char_to_integer(char x) {
  return
    (x>='0' && x<='9') ? x-'0' :
    (x>='a' && x<='f') ? x-'a'+10 :
    (x>='A' && x<='F') ? x-'A'+10 :
    -1;
}

json_string* parse::string() {
  std::string* x=new std::string;
  char ch;
  while((ch=in.get()), ch != '"') {
    if(ch == '\\') {
      ch=in.get();
      switch(ch) {
      case '"'  :
      case '\\' :
      case '/'  : *x+=ch; break;
      case 'b'  : *x+='\b'; break;
      case 'f'  : *x+='\f'; break;
      case 'n'  : *x+='\n'; break;
      case 'r'  : *x+='\r'; break;
      case 't'  : *x+='\t'; break;
      case 'u'  : {
        int i, num;
        for(i=0, num=0; i<4 && isxdigit(ch=in.get()); i++) {
          num*=16;
          num+=char_to_integer(ch);
        }
        if(i<4) in.unget();
        *x+=integer_to_utf8(num);
        break;
      }
      default: throw json_exception();
      }
    } else {
      *x+=ch;
    }
  }
  return new json_string(x);
}

json_number* parse::number() {
  char ch;
  double n=0;
  
  ch=in.get();
  int sign;
  if (ch == '-') sign = -1;
  else {
    in.unget();
    sign = 1;
  }
  while(isdigit(ch=in.get())) n=n*10+(ch-'0');
  if(ch=='.') {
    double d=0;
    for(double col=0.1; isdigit(ch=in.get()); col*=0.1) d+=(ch-'0')*col;
    n+=d;
  }
  if(ch=='e' || ch=='E') {
    int plus, e;
    ch=in.get();
    if(ch=='+') plus = 1;
    else if(ch=='-') plus = 0;
    else {
      plus = 1;
      in.unget();
    }
    for(e=0; (ch=in.get(), isdigit(ch));) {
      e*=10;
      e+=ch-'0';
    }
    in.unget();
    n *= pow(plus ? 10.0 : -0.1, e);
  } else {
    in.unget();
  }

  return new json_number(n);
}

json_value* parse::value() {
  char ch;
  json_value* return_value;
  
  while(isspace(ch=in.get()));
  if(ch == '{') {
    return_value = parse::object();
  }
  else if(ch == '[') {
    return_value = parse::array();
  }
  else if(ch == '"') {
    return_value = parse::string();
  }
  else if(ch == '-' || isdigit(ch)) {
    in.unget();
    return_value = parse::number();
  }
  return return_value;
}

