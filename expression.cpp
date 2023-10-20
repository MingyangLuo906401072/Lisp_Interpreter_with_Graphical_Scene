#include "expression.hpp"


// system includes
#include <sstream>
#include <cmath>
#include <limits>
#include <cctype>
#include <tuple>

Expression::Expression(bool tf){
  // TODO: implement this function
}

Expression::Expression(double num){
  // TODO: implement this function
}

Expression::Expression(const std::string & sym){
  // TODO: implement this function
}

Expression::Expression(std::tuple<double,double> value){
  // TODO: implement this function
}

Expression::Expression(std::tuple<double,double> start,
		       std::tuple<double,double> end){
  // TODO: implement this function
}


Expression::Expression(std::tuple<double,double> center,
		       std::tuple<double,double> start,
		       double angle){
  // TODO: implement this function
}

bool Expression::operator==(const Expression & exp) const noexcept{
  // TODO: implement this function
  return false;
}

std::ostream & operator<<(std::ostream & out, const Expression & exp){
  // TODO: implement this function
}

bool token_to_atom(const std::string & token, Atom & atom){
  // TODO: implement this function
  // return true if it a token is valid. otherwise, return false.
  return true;
}

