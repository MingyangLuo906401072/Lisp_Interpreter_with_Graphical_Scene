#include "expression.hpp"

// system includes
#include <sstream>
#include <cmath>
#include <cctype>
#include <tuple>

// Constructor for a Boolean Expression
Expression::Expression(bool tf) {
    head.type = BooleanType;
    head.value.bool_value = tf;
}

// Constructor for a Number Expression
Expression::Expression(double num) {
    head.type = NumberType;
    head.value.num_value = num;
}

// Constructor for a Symbol Expression
Expression::Expression(const std::string& sym) {
    head.type = SymbolType;
    head.value.sym_value = sym;
}

// Constructor for an Expression with a single Point atom with value
Expression::Expression(std::tuple<double, double> value) {
    head.type = PointType;
    head.value.point_value.x = std::get<0>(value);
    head.value.point_value.y = std::get<1>(value);
}

// Constructor for an Expression with a single Line atom with starting point and ending point
Expression::Expression(std::tuple<double, double> start, std::tuple<double, double> end) {
    head.type = LineType;
    head.value.line_value.first.x = std::get<0>(start);
    head.value.line_value.first.y = std::get<1>(start);
    head.value.line_value.second.x = std::get<0>(end);
    head.value.line_value.second.y = std::get<1>(end);
}

// Constructor for an Expression with a single Arc atom with center point, starting point, and spanning angle in radians
Expression::Expression(std::tuple<double, double> center, std::tuple<double, double> start, double angle) {
    head.type = ArcType;
    head.value.arc_value.center.x = std::get<0>(center);
    head.value.arc_value.center.y = std::get<1>(center);
    head.value.arc_value.start.x = std::get<0>(start);
    head.value.arc_value.start.y = std::get<1>(start);
    head.value.arc_value.span = angle;
}

bool Expression::operator==(const Expression& exp) const noexcept {
    // Compare the head
    if (head.type != exp.head.type) {
        return false;
    }
    switch (head.type) {
    case BooleanType:
        return head.value.bool_value == exp.head.value.bool_value;
    case NumberType:
        return std::abs(head.value.num_value - exp.head.value.num_value) <= std::numeric_limits<double>::epsilon();
    case SymbolType:
        return head.value.sym_value == exp.head.value.sym_value;
    case PointType:
        return head.value.point_value == exp.head.value.point_value;
    case LineType:
        return (head.value.line_value.first == exp.head.value.line_value.first &&
            head.value.line_value.second == exp.head.value.line_value.second);
    case ArcType:
        return (head.value.arc_value.center == exp.head.value.arc_value.center &&
            head.value.arc_value.start == exp.head.value.arc_value.start &&
            head.value.arc_value.span == exp.head.value.arc_value.span);
    default:
        return true; // NoneType, should always be equal
    }
}

std::ostream& operator<<(std::ostream& out, const Expression& exp) {
    switch (exp.head.type) {
    case BooleanType:
        out << (exp.head.value.bool_value ? "True" : "False");
        break;
    case NumberType:
        out << exp.head.value.num_value;
        break;
    case SymbolType:
        out << exp.head.value.sym_value;
        break;
    case PointType:
        out << "(" << exp.head.value.point_value.x << "," << exp.head.value.point_value.y << ")";
        break;
    case LineType:
        out << "(" << "(" << exp.head.value.line_value.first.x << "," << exp.head.value.line_value.first.y << "),("
            << exp.head.value.line_value.second.x << "," << exp.head.value.line_value.second.y << "))";
        break;
    case ArcType:
        out << "((" << exp.head.value.arc_value.center.x << "," << exp.head.value.arc_value.center.y << "),("
            << exp.head.value.arc_value.start.x << "," << exp.head.value.arc_value.start.y << ") "
            << exp.head.value.arc_value.span << ")";
        break;
    default:
        out << "None";
    }

    // If there is a tail, recursively print the tail
    for (const Expression& subexp : exp.tail) {
        out << " " << subexp;
    }

    return out;
}

bool is_valid_number(const std::string& token) {
    if (token == "+" || token == "-" || token == "*" || token == "/") {
        return false;
    }
    // Check if the token is a valid number (integer, floating-point, or scientific notation)
    size_t len = token.length();
    size_t dot_count = 0;
    bool has_e = false;

    for (size_t i = 0; i < len; ++i) {
        char c = token[i];

        if (i == 0 && (c == '+' || c == '-')) {
            // Skip leading plus or minus sign
        }
        else if (std::isdigit(c) != 0) {  // Compare result to 0
            // Digit, continue
        }
        else if (c == '.' && dot_count == 0 && !has_e) {
            // Decimal point (only allow one, and not after 'e' or 'E')
            dot_count++;
        }
        else if ((c == 'e' || c == 'E') && !has_e) {
            // Exponent 'e' or 'E' (only allow one)
            has_e = true;
            if (i + 1 < len && (token[i + 1] == '+' || token[i + 1] == '-')) {
                // Skip leading plus or minus sign for exponent
                i++;
            }
        }
        else {
            // Invalid character found
            return false;
        }
    }

    return true;
}

bool is_valid_bool(const std::string& token) {
    return token == "True" || token == "False";
}

bool token_to_atom(const std::string& token, Atom& atom) {
    if (token.empty()) {
        return false;
    }

    if (is_valid_number(token)) {

        try {
            double num = std::stod(token);
            atom.type = NumberType;
            atom.value.num_value = num;
            return true;
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Error: Invalid argument while parsing number: " << e.what() << std::endl;
            return false;
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Error: Number out of range: " << e.what() << std::endl;
            return false;
        }
    }

    if (is_valid_bool(token)) {
        atom.type = BooleanType;
        atom.value.bool_value = (token == "True");
        return true;
    }

    if (std::isalpha(token[0]) != 0 || token == "+" || token == "-" || token == "*" || token == "/" || token == "<" || token == ">" ||
        token == ">=" || token == "<=" || token == "=") {
        // The symbol starts with an alphabet character or allowed special characters, so it's a valid symbol
        atom.type = SymbolType;
        atom.value.sym_value = token;
        return true;
    }

    return false; // Token is not a valid number, boolean, or symbol
}

