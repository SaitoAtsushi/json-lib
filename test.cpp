#include <iostream>
#include <strstream>
#include <vector>
#include <map>
#include <string>
#include "json.h"
using namespace std;

int main(void) {
  parse parser(cin);
  cout<<parser.start()<<endl;
}
