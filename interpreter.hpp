#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

// system includes
#include <string>
#include <iostream>
#include <sstream>

// module includes
#include "environment.hpp"
#include "tokenize.hpp"

// Interpreter has
// Environment, which starts at a default
// parse method, builds an internal AST
// eval method, updates Environment, returns last result
class Interpreter {
public:
    Interpreter();
    bool parse(std::istream& expression) noexcept;
    Expression read_from_tokens(TokenSequenceType& tokens);
    Expression atom(const std::string& token);
    Expression eval(const Expression& exp);
    Expression eval();
    const std::vector<Atom>& getGraphicsVector() const;


protected:
    Environment env;
    Expression ast;
    std::vector<Atom> graphics;

};

#endif

