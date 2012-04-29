
class json_exception :public std::exception {
};

class json_value {
public:
  virtual const std::type_info& type();
  virtual ~json_value();
  virtual std::ostream& write(std::ostream& os) const =0;
};

class json_string :public json_value {
public:
  typedef std::string value_type;
private:
  const value_type *str;
public:
  json_string(std::string* x);
  ~json_string();
  std::ostream& write(std::ostream& os) const;
  operator const value_type&() const;
};

class json_array :public json_value {
public:
  typedef std::vector<json_value*> value_type;
private:
  const value_type *arr;
public:
  json_array(const value_type* x);
  ~json_array();
  std::ostream& write(std::ostream& os) const;
  operator const value_type&() const;
};
    
class json_number :public json_value {
private:
  const double number;
public:
  json_number(const double number);
  std::ostream& write(std::ostream& os) const;
  operator const double&() const;
};

class json_object :public json_value {
public:
  typedef std::map<json_string*, json_value*> value_type;
private:
  const value_type *obj;
public:
  json_object(value_type* obj);
  ~json_object();
  std::ostream& write(std::ostream& os) const;
  operator const value_type&() const;
};

class json_null :public json_value {
};

class json_bool :public json_value {
  const bool val;
public:
  json_bool(const bool x);
  std::ostream& write(std::ostream& os) const;
  operator bool() const;
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
