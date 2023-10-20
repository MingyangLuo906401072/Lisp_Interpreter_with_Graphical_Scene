#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

// system includes
#include <string>
#include <istream>
#include <vector>


// module includes
#include "expression.hpp"
#include "environment.hpp"

// Interpreter has
// Environment, which starts at a default
// parse method, builds an internal AST
// eval method, updates Environment, returns last result
class Interpreter{
public:
  bool parse(std::istream & expression) noexcept;
  Expression eval();
protected:
  Environment env;
  Expression ast;
  std::vector<Atom> graphics;
};


#endif
