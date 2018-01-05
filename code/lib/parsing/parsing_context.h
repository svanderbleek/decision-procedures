#ifndef PARSING_CONTEXT_H
#define PARSING_CONTEXT_H

#include "ast.h"

namespace parsing_context {

class parsing_astt:public ::ast::astt
{
public:
  int get_identifier(const dstring s);
  void set_type(unsigned ids, unsigned t);
};

extern parsing_astt parsing_ast;
extern std::ostream *err;

bool parse_file(const std::string &file);

}

#endif
