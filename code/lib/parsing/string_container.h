/*******************************************************************\

Module: Container for C-Strings

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef STRING_CONTAINER_H
#define STRING_CONTAINER_H

#include <list>

#include <ext/hash_map>

size_t hash_string(const char *s);

struct string_ptrt
{
  const char *s;
  
  const char *c_str() const
  {
    return s;
  }
  
  explicit string_ptrt(const char *_s):s(_s)
  {
  }

  friend bool operator==(const string_ptrt a, const string_ptrt b);
};

bool operator==(const string_ptrt a, const string_ptrt b);

class string_ptr_hash
{
public:
  size_t operator()(const string_ptrt s) const { return hash_string(s.s); }
};

class string_containert
{
public:
  unsigned operator[](const char *s)
  {
    return get(s);
  }
  
  string_containert()
  {
    // allocate empty string -- this gets index 0
    get("");
  }
  
  const std::string &string(unsigned no) const
  {
    return *string_vector[no];
  }
  
  const char *c_str(unsigned no) const
  {
    return string(no).c_str();
  }
  
protected:
  typedef __gnu_cxx::hash_map<string_ptrt, unsigned, string_ptr_hash> hash_tablet;
  hash_tablet hash_table;
  
  unsigned get(const char *s)
  {
    string_ptrt string_ptr(s);

    hash_tablet::iterator it=hash_table.find(string_ptr);
    
    if(it!=hash_table.end())
      return it->second;

    unsigned r=hash_table.size();

    // these are stable
    string_list.push_back(s);
    string_ptrt result(string_list.back().c_str());

    hash_table[result]=r;
    
    // these are not
    string_vector.push_back(&string_list.back());

    return r;
  }
  
  typedef std::list<std::string> string_listt;
  string_listt string_list;
  
  typedef std::vector<std::string *> string_vectort;
  string_vectort string_vector;
};

extern string_containert string_container;

#endif
