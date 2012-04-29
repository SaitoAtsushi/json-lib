
class json_exception :public std::exception {
};

class json_value {
public:
  virtual const std::type_info& type();
  virtual ~json_value();
  virtual std::ostream& write(std::ostream&) const =0;
};

class json_string :public json_value {
public:
  typedef std::string value_type;
private:
  const value_type *value;
public:
  json_string(std::string*);
  ~json_string();
  std::ostream& write(std::ostream&) const;
  operator const value_type&() const;
};

class json_array :public json_value {
public:
  typedef std::vector<json_value*> value_type;
private:
  const value_type *value;
public:
  json_array(const value_type*);
  ~json_array();
  std::ostream& write(std::ostream&) const;
  operator const value_type&() const;
};
    
class json_number :public json_value {
private:
  const double value;
public:
  json_number(const double);
  std::ostream& write(std::ostream&) const;
  operator const double&() const;
};

class json_object :public json_value {
public:
  typedef std::map<json_string*, json_value*> value_type;
private:
  const value_type *value;
public:
  json_object(value_type*);
  ~json_object();
  std::ostream& write(std::ostream&) const;
  operator const value_type&() const;
};

class json_null :public json_value {
  std::ostream& write(std::ostream&) const;
};

class json_bool :public json_value {
  const bool value;
public:
  json_bool(const bool);
  std::ostream& write(std::ostream&) const;
  operator bool() const;
};

std::ostream& operator<<(std::ostream&, const json_value*);

class parse {
private:
  std::istream& in;
  json_string* string();
  json_object* object();
  json_array*  array();
  json_number* number();
  json_value* value();
public:
  parse(std::istream &);
  json_value* start();
};
