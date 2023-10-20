#include "interpreter.hpp"

// system includes
#include <stack>
#include <stdexcept>
#include <iostream>

// module includes
#include "tokenize.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "interpreter_semantic_error.hpp"

bool Interpreter::parse(std::istream & expression) noexcept{
  // TODO: implement this function
  // return true if input is valid. otherwise, return false.
  return true;
};

Expression Interpreter::eval(){
  // TODO: implement this function
  return Expression();
}
