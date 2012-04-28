
class json_exception :public std::exception {
};

class json_value {
public:
  virtual const std::type_info& type();
  virtual ~json_value();
  virtual std::ostream& write(std::ostream& os) const =0;
};

class json_string :public json_value {
private:
  std::string *str;
public:
  json_string(std::string* x);
  ~json_string();
  std::ostream& write(std::ostream& os) const;
};

class json_array :public json_value {
public:
  typedef std::vector<json_value*> value_type;
private:
  json_array::value_type *arr;
public:
  json_array();
  ~json_array();
  json_array& insert(json_value* x);
  std::ostream& write(std::ostream& os) const;
};

    
class json_number :public json_value {
private:
  double number;
public:
  json_number(double number);
  std::ostream& write(std::ostream& os) const;
};

class json_object :public json_value {
public:
  typedef std::map<json_string*, json_value*> value_type;
private:
   value_type *obj;
public:
  json_object();
  ~json_object();
  json_object& insert(json_string* k, json_value* v);
  std::ostream& write(std::ostream& os) const;
};

class json_null :public json_value {
};

class json_bool :public json_value {
  bool val;
public:
  json_bool(bool x);
  std::ostream& write(std::ostream& os) const;
};

std::ostream& operator<<(std::ostream& os, const json_value* value);

class parse {
private:
  std::istream& in;
  json_string* string();
  json_object* object();
  json_array*  array();
  json_number* number();
public:
  parse(std::istream &in);
  json_value* value();
};
