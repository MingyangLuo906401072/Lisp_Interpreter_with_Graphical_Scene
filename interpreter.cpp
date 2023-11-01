#include "interpreter.hpp"

// system includes
#include <stack>
#include <stdexcept>

// module includes
#include "interpreter_semantic_error.hpp"


Interpreter::Interpreter() {
    env.init();
}

Expression Interpreter::atom(const std::string& token) {
    Atom atom;
    if (token_to_atom(token, atom)) {
        return Expression(atom);
    }

    throw InterpreterSemanticError("Error: Invalid token:" + token);
}

Expression Interpreter::read_from_tokens(TokenSequenceType& tokens) {
    if (tokens.empty()) {
        throw InterpreterSemanticError("Error: Unexpected end of input.");
    }

    std::string token(tokens.front());
    tokens.pop_front();

    if (token == "(") {
        Expression exp;
        exp.head.type = ListType;

        while (!tokens.empty() && tokens.front() != ")") {
            exp.tail.push_back(read_from_tokens(tokens));
        }

        if (tokens.empty() || tokens.front() != ")") {
            throw InterpreterSemanticError("Error: Unmatched parentheses.");
        }

        tokens.pop_front(); // Pop the closing parenthesis

        if (exp.tail.empty()) {
            throw InterpreterSemanticError("Error: Empty list.");
        }

        return exp;
    }

    if (token == ")") {
        throw InterpreterSemanticError("Error: Empty parentheses.");
    }

    return atom(token);
}

bool Interpreter::parse(std::istream& expression) noexcept {
    // Tokenize the program string and build the expression
    TokenSequenceType tokens = tokenize(expression);

    // Check if the tokens are empty or the first token is not '('
    if (tokens.empty()) {
        std::cerr << "Error: Empty tokens." << std::endl;
        return false; // Parsing failed
    }

    try {
        ast = read_from_tokens(tokens);
    }
    catch (const InterpreterSemanticError& e) {
        std::cerr << e.what() << std::endl;
        return false; // Parsing failed
    }

    // Check for extra tokens after parsing
    if (!tokens.empty()) {
        std::cerr << "Error: Extra tokens found after parsing." << std::endl;
        ast = Expression();
        return false; // Parsing failed
    }

    if (ast.head.type != ListType) {
        std::cerr << "Error: Not a list." << std::endl;
        ast = Expression();
        return false; // Parsing failed
    }

    return true; // Return true if parsing was successful
}

Expression Interpreter::eval(const Expression& exp) {
    if (exp.head.type == ListType) {
        // Handle a list as a special form or procedure call
        if (exp.tail.empty()) {
            // Empty list, no further evaluation needed
            return exp;
        }

        Expression firstExp = exp.tail[0];

        if (firstExp.head.type == SymbolType) {
            const Symbol& symbolName = firstExp.head.value.sym_value;

            if (symbolName == "define") {
                // Handle the 'define' special form
                if (exp.tail.size() != 3 || exp.tail[1].head.type != SymbolType) {
                    throw InterpreterSemanticError("Error: Invalid 'define' syntax.");
                }
                const Symbol& definedSymbol = exp.tail[1].head.value.sym_value;
                Expression definedValue = eval(exp.tail[2]);
                if (!env.isKnown(definedSymbol) && definedSymbol != "if" && definedSymbol != "define" && definedSymbol != "begin") {
                    env.addExp(definedSymbol, definedValue);
                    return definedValue;
                }
                throw InterpreterSemanticError("Error: Invalid define Symbol.");
            }
            else if (symbolName == "begin") {
                // Handle the 'begin' special form
                Expression result;
                for (size_t i = 1; i < exp.tail.size(); ++i) {
                    result = eval(exp.tail[i]);
                }
                return result;
            }
            else if (symbolName == "if") {
                // Handle the 'if' special form
                if (exp.tail.size() != 4) {
                    throw InterpreterSemanticError("Error: Invalid 'if' syntax.");
                }
                Expression condition = eval(exp.tail[1]);
                if (condition.head.type != BooleanType) {
                    throw InterpreterSemanticError("Error: 'if' condition must evaluate to a Boolean.");
                }
                return eval(condition.head.value.bool_value ? exp.tail[2] : exp.tail[3]);
            }
            else if (symbolName == "draw") {
                // Handle the 'draw' special form
                if (exp.tail.size() < 2) {
                    throw InterpreterSemanticError("Error: Invalid 'draw' syntax.");
                }
                for (size_t i = 1; i < exp.tail.size(); ++i) {
                    Expression graphic = eval(exp.tail[i]);
                    if (graphic.head.type == PointType || graphic.head.type == LineType || graphic.head.type == ArcType) {
                        graphics.push_back(graphic.head);
                    }
                    else {
                        throw InterpreterSemanticError("Error: Invalid(non-graphic) atoms after 'draw'.");
                    }
                }
                return Expression(); // Return an empty expression
            }
            else {
                if (env.isKnown(symbolName)) {
                    // Symbol represents a procedure call
                    if (env.isProc(symbolName)) {
                        const Procedure& proc = env.getProc(symbolName);
                        std::vector<Atom> args;

                        // Evaluate the remaining expressions in the list as arguments
                        for (size_t i = 1; i < exp.tail.size(); ++i) {
                            Expression argExp = eval(exp.tail[i]);
                            args.push_back(argExp.head);
                        }

                        // Call the procedure with the evaluated arguments
                        return proc(args);
                    }
                    else if (env.isExp(symbolName)) {
                        // Symbol represents a user-defined expression or "pi"
                        return env.getExp(symbolName);
                    }
                }
                throw InterpreterSemanticError("Error: Unknown symbol: " + symbolName);
            }
        }
        if (firstExp.head.type == BooleanType || firstExp.head.type == NumberType) {
            return firstExp;
        }
        else {
            throw InterpreterSemanticError("Error: Invalid procedure, expression, number, boolean or special form: " + exp.tail[0].head.value.sym_value);
        }
    }
    else if (exp.head.type == SymbolType || exp.head.type == PointType || exp.head.type == ArcType || exp.head.type == LineType) {
        // Handle symbols, graphics
        const Symbol& symbolName = exp.head.value.sym_value;

        if (env.isKnown(symbolName)) {
            // Symbol represents a procedure call
            if (env.isProc(symbolName)) {
                const Procedure& proc = env.getProc(symbolName);
                std::vector<Atom> args;

                // Evaluate the remaining expressions in the list as arguments
                for (size_t i = 0; i < exp.tail.size(); ++i) {
                    Expression argExp = eval(exp.tail[i]);
                    args.push_back(argExp.head);
                }

                // Call the procedure with the evaluated arguments
                return proc(args);
            }
            else if (env.isExp(symbolName)) {
                // Symbol represents a user-defined expression or "pi"
                return env.getExp(symbolName);
            }
        }
        throw InterpreterSemanticError("Error: Unknown type: " + symbolName);
    }
    else if (exp.head.type == BooleanType || exp.head.type == NumberType) {
        return exp;
    }
    else {
        throw InterpreterSemanticError("Error: Invalid procedure, expression, number, boolean or special form: " + exp.tail[0].head.value.sym_value);
    }
}

Expression Interpreter::eval() {
    // Ensure that the AST is not empty.
    if (ast.head.type != NoneType) {
        return eval(ast);
    }
    throw InterpreterSemanticError("Error: No expression to evaluate.");
}
