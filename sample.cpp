#include <iostream>
#include <strstream>
#include <vector>
#include <map>
#include <string>
#include "json.h"
using namespace std;

int main(void) {
  istrstream x1("{\"name\": \"John Smith\", \"age\": 33}");
  parse p1(x1);
  istrstream x2("[\"milk\", \"bread\", \"eggs\"]");
  parse p2(x2);
  istrstream x3("\"\\u672c\\u65e5\\u306f\\u6674\\u5929\\u306a\\u308a\"");
  parse p3(x3);
  istrstream x4("-243.68e-15");
  parse p4(x4);
  istrstream x5("\"ab\naaa\"");
  parse p5(x5);
  cout<<p1.value()<<endl;
  cout<<p2.value()<<endl;
  cout<<p3.value()<<endl;
  cout<<p4.value()<<endl;
  cout<<p5.value()<<endl;
}
