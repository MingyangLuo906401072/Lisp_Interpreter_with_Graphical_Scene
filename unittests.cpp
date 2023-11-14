#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"
#include "tokenize.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "interpreter.hpp"
#include "interpreter_semantic_error.hpp"
#include <sstream>

TEST_CASE("Tokenize function test cases") {
    // Test case 1: Tokenize an empty input
    std::istringstream emptyStream("");
    TokenSequenceType result1 = tokenize(emptyStream);
    REQUIRE(result1.empty());

    // Test case 2: Tokenize a simple expression with OPEN and CLOSE
    std::istringstream simpleStream("(add 2 3)");
    TokenSequenceType result2 = tokenize(simpleStream);
    TokenSequenceType expected2 = { "(", "add", "2", "3", ")" };
    REQUIRE(result2 == expected2);

    // Test case 3: Tokenize a line with a comment
    std::istringstream commentStream("This is a test ; comment");
    TokenSequenceType result3 = tokenize(commentStream);
    TokenSequenceType expected3 = { "This", "is", "a", "test" };
    REQUIRE(result3 == expected3);

    // Test case 4: Tokenize a complex input with multiple lines
    std::istringstream complexStream("Line 1\n(Line 2 ; Comment\nLine 3)\n(Line 4)");
    TokenSequenceType result4 = tokenize(complexStream);
    TokenSequenceType expected4 = { "Line", "1", "(", "Line", "2", "Line", "3", ")", "(", "Line", "4", ")" };
    REQUIRE(result4 == expected4);

    // Add more test cases here to cover various scenarios.

    // Test case 5: Tokenize a line with leading and trailing spaces
    std::istringstream spaceStream("  Spaces  ");
    TokenSequenceType result5 = tokenize(spaceStream);
    TokenSequenceType expected5 = { "Spaces" };
    REQUIRE(result5 == expected5);
}

TEST_CASE("Tokenize function additional test cases") {
    // Test case 6: Tokenize a line with multiple spaces between tokens
    std::istringstream spaceStream2("Token1    Token2   Token3");
    TokenSequenceType result6 = tokenize(spaceStream2);
    TokenSequenceType expected6 = { "Token1", "Token2", "Token3" };
    REQUIRE(result6 == expected6);

    // Test case 7: Tokenize a complex input with nested parentheses
    std::istringstream nestedStream("(outer (inner 1) (inner 2))");
    TokenSequenceType result7 = tokenize(nestedStream);
    TokenSequenceType expected7 = { "(", "outer", "(", "inner", "1", ")", "(", "inner", "2", ")", ")" };
    REQUIRE(result7 == expected7);

    // Test case 8: Tokenize a mix of spaces and comments
    std::istringstream mixStream("Token1 ; Comment\n   Token2  Token3 ; Comment\nToken4");
    TokenSequenceType result8 = tokenize(mixStream);
    TokenSequenceType expected8 = { "Token1", "Token2", "Token3", "Token4" };
    REQUIRE(result8 == expected8);

    // Test case 9: Tokenize a single character input
    std::istringstream singleCharStream("A");
    TokenSequenceType result9 = tokenize(singleCharStream);
    TokenSequenceType expected9 = { "A" };
    REQUIRE(result9 == expected9);

    // Test case 10: Tokenize an input with no spaces
    std::istringstream noSpaceStream("NoSpacesHere");
    TokenSequenceType result10 = tokenize(noSpaceStream);
    TokenSequenceType expected10 = { "NoSpacesHere" };
    REQUIRE(result10 == expected10);
}

TEST_CASE("Tokenize function additional test cases (continued)") {
    // Test case 11: Tokenize an input with mixed spaces and tabs
    std::istringstream mixedSpacesTabsStream("Space\tTab\tMixed");
    TokenSequenceType result11 = tokenize(mixedSpacesTabsStream);
    TokenSequenceType expected11 = { "Space", "Tab", "Mixed" };
    REQUIRE(result11 == expected11);

    // Test case 12: Tokenize an input with multiple comments on the same line
    std::istringstream multiCommentStream("This is a ; comment1 ; comment2");
    TokenSequenceType result12 = tokenize(multiCommentStream);
    TokenSequenceType expected12 = { "This", "is", "a" };
    REQUIRE(result12 == expected12);

    // Test case 13: Tokenize an input with only comments
    std::istringstream commentsOnlyStream(" ; Comment1\n; Comment2\n; Comment3");
    TokenSequenceType result13 = tokenize(commentsOnlyStream);
    TokenSequenceType expected13 = {};
    REQUIRE(result13 == expected13);

    // Test case 14: Tokenize an input with a mix of OPEN and CLOSE characters
    std::istringstream mixedOpenCloseStream("(1 (2 (3)) 4 (5 6))");
    TokenSequenceType result14 = tokenize(mixedOpenCloseStream);
    TokenSequenceType expected14 = { "(", "1", "(", "2", "(", "3", ")",")", "4", "(", "5", "6", ")", ")" };
    REQUIRE(result14 == expected14);
}

TEST_CASE("Expression constructor and equality tests") {
    SECTION("Expression constructor and equality for Booleans") {
        Expression exp1(true);
        Expression exp2(false);
        Expression exp3(true);

        REQUIRE(exp1 == exp3); // True and True
        REQUIRE(!(exp1 == exp2)); // True and False
        REQUIRE(exp2 == exp2); // False and False
    }

    SECTION("Expression constructor and equality for Numbers") {
        Expression exp1(42.0);
        Expression exp2(3.14);
        Expression exp3(42.0);

        REQUIRE(exp1 == exp3); // 42.0 and 42.0
        REQUIRE_FALSE(exp1 == exp2); // 42.0 and 3.14
    }

    SECTION("Equality and Inequality for atom head types") {
        std::string s1 = "apple";
        std::string s2 = "banana";
        Expression exp1(s1);
        Expression exp2(s2);
        Expression exp3(s1);
        Expression exp4(6.0);

        // Test inequality of two Expression objects with different symbols
        REQUIRE_FALSE(exp2 == exp3); // "apple" and "banana" should not be equal

        // Test equality of two Expression objects with the same symbol
        REQUIRE((exp1 == exp3)); // "apple" and "apple" should be equal

        // Test equality of two Expression objects with the different atom type
        REQUIRE_FALSE(exp1 == exp4); // "apple" and 6 should not be equal
    }

    SECTION("Expression constructor and equality for mixed types") {
        std::string s1 = "apple";
        Expression exp1(true);
        Expression exp2(42.0);
        Expression exp3(s1);

        Expression exp4(true);
        Expression exp5(42.0);
        Expression exp6(s1);

        REQUIRE(exp1 == exp4); // True and True
        REQUIRE(exp2 == exp5); // 42.0 and 42.0
        REQUIRE(exp3 == exp6); // "apple" and "apple"
    }
}

TEST_CASE("is_valid_number and is_valid_bool tests") {
    SECTION("Valid number and bool tests") {
        REQUIRE(is_valid_number("42"));
        REQUIRE(is_valid_number("-3.14"));
        REQUIRE(is_valid_number("1e5"));
        REQUIRE(is_valid_bool("True"));
        REQUIRE(is_valid_bool("False"));
    }

    SECTION("Invalid number and bool tests") {
        REQUIRE_FALSE(is_valid_number("abc")); // Not a valid number
        REQUIRE_FALSE(is_valid_number("3.14.5")); // Invalid double dot
        REQUIRE_FALSE(is_valid_bool("true")); // Case-sensitive
        REQUIRE_FALSE(is_valid_bool("True123")); // Extra characters
    }
}

TEST_CASE("token_to_atom tests") {
    SECTION("Valid tokens") {
        Atom atom;

        REQUIRE(token_to_atom("42", atom));
        REQUIRE(atom.type == NumberType);
        REQUIRE(atom.value.num_value == 42.0);

        REQUIRE(token_to_atom("True", atom));
        REQUIRE(atom.type == BooleanType);
        REQUIRE(atom.value.bool_value);

        REQUIRE(token_to_atom("apple", atom));
        REQUIRE(atom.type == SymbolType);
        REQUIRE(atom.value.sym_value == "apple");
    }

    SECTION("Invalid tokens") {
        Atom atom;

        REQUIRE_FALSE(token_to_atom("[22abc", atom));
        REQUIRE_FALSE(token_to_atom("54True123", atom));
        REQUIRE_FALSE(token_to_atom("%5", atom));
    }
}

TEST_CASE("Environment Initialization") {
    Environment env;

    SECTION("Check if environment is empty after initialization") {
        REQUIRE_FALSE(env.isKnown("undefined_symbol"));
        REQUIRE_FALSE(env.isExp("undefined_symbol"));
        REQUIRE_FALSE(env.isProc("undefined_symbol"));
    }

    SECTION("Check for known symbols after initialization") {
        REQUIRE(env.isKnown("pi"));
        REQUIRE(env.isExp("pi"));
        REQUIRE_FALSE(env.isProc("pi"));
    }
}

TEST_CASE("Environment Expression Manipulation") {
    Environment env;

    SECTION("Add and Retrieve Expressions") {
        Expression exp(3.14);
        env.addExp("my_expression", exp);
        REQUIRE(env.isKnown("my_expression"));
        REQUIRE(env.isExp("my_expression"));

        Expression retrieved = env.getExp("my_expression");
        REQUIRE(retrieved == exp);
    }

    SECTION("Add and Retrieve Expressions with Different Types") {
        Expression exp1(3.14);
        env.addExp("my_expression1", exp1);
        REQUIRE(env.isExp("my_expression1"));
        REQUIRE(env.isKnown("my_expression1"));

        Expression exp2(true);
        env.addExp("my_expression2", exp2);
        REQUIRE(env.isExp("my_expression2"));
        REQUIRE(env.isKnown("my_expression2"));

        Expression retrieved1 = env.getExp("my_expression1");
        REQUIRE(retrieved1 == exp1);

        Expression retrieved2 = env.getExp("my_expression2");
        REQUIRE(retrieved2 == exp2);
    }

    SECTION("Adding and Retrieving Procedures") {
        env.init(); // Reinitialize the environment

        REQUIRE(env.isProc("<"));
        REQUIRE(env.isProc(">="));
        REQUIRE(env.isProc("+"));
        REQUIRE(env.isProc("<="));
        REQUIRE(env.isProc("and"));
        REQUIRE(env.isProc("not"));
        REQUIRE(env.isProc("or"));
        REQUIRE(env.isProc("="));
        REQUIRE(env.isProc(">"));
        REQUIRE(env.isProc("*"));
        REQUIRE(env.isProc("-"));
        REQUIRE(env.isProc("log10"));
        REQUIRE(env.isProc("/"));
        REQUIRE(env.isProc("pow"));

        Procedure addProc = env.getProc("+");
        Procedure mulProc = env.getProc("*");

        // Check if the retrieved procedures are not nullptr
        REQUIRE(addProc != nullptr);
        REQUIRE(mulProc != nullptr);
    }
}

TEST_CASE("Environment Symbol Lookup") {
    Environment env;

    SECTION("Retrieve Unknown Symbol") {
        REQUIRE_FALSE(env.isKnown("undefined_symbol"));
        REQUIRE_THROWS(env.getExp("undefined_symbol"));
        REQUIRE_THROWS(env.getProc("undefined_symbol"));
    }

    SECTION("Retrieve Known Symbol Type") {
        Expression exp(3.14);
        env.addExp("my_expression", exp);

        REQUIRE(env.isExp("my_expression"));
        REQUIRE_FALSE(env.isProc("my_expression"));
    }

    SECTION("Retrieve Known Procedure Type") {
        env.init(); // Reinitialize the environment

        REQUIRE(env.isProc("+"));
        REQUIRE(env.isProc("*"));

        REQUIRE_FALSE(env.isExp("+"));
        REQUIRE_FALSE(env.isExp("*"));
    }
}

TEST_CASE("Environment Symbol Lookup 2") {
    Environment env;

    SECTION("Retrieve Unknown Symbol") {
        REQUIRE_FALSE(env.isKnown("undefined_symbol"));
        REQUIRE_THROWS(env.getExp("undefined_symbol"));
    }

    SECTION("Retrieve Known Symbol Type") {
        Expression exp(3.14);
        env.addExp("my_expression", exp);

        REQUIRE(env.isExp("my_expression"));
    }
}

TEST_CASE("Environment Expression Reassignment") {
    Environment env;

    SECTION("Reassign Expression") {
        Expression exp(3.14);
        env.addExp("my_expression", exp);

        Expression newExp(42.0);

        // Attempt to reassign an expression, which should not be allowed
        REQUIRE_THROWS(env.addExp("my_expression", newExp));
    }
}

TEST_CASE("Environment Additional Test Cases") {
    Environment env;

    SECTION("Attempt to Add Expression with Existing Symbol") {
        Expression exp1(3.14);
        env.addExp("my_expression", exp1);

        Expression exp2(42.0);

        // Attempt to add an expression with the same symbol, which should not be allowed
        REQUIRE_THROWS(env.addExp("my_expression", exp2));

        // Ensure that the original expression remains unchanged
        Expression retrieved = env.getExp("my_expression");
        REQUIRE(retrieved == exp1);
    }

    SECTION("Retrieve Unknown Symbol Value") {
        REQUIRE_FALSE(env.isKnown("undefined_symbol"));
        REQUIRE_THROWS(env.getExp("undefined_symbol"));
    }

    SECTION("Check for Known Procedures") {
        env.init(); // Reinitialize the environment

        REQUIRE(env.isProc("+"));
        REQUIRE(env.isProc("*"));
        REQUIRE(env.isProc("<"));

        // Attempt to retrieve procedures for existing symbols
        Procedure addProc = env.getProc("+");
        Procedure mulProc = env.getProc("*");
        Procedure ltProc = env.getProc("<");

        REQUIRE(addProc != nullptr);
        REQUIRE(mulProc != nullptr);
        REQUIRE(ltProc != nullptr);
    }

    SECTION("Retrieve Unknown Procedure") {
        REQUIRE_FALSE(env.isProc("undefined_proc"));
        REQUIRE_THROWS(env.getProc("undefined_proc"));
    }
}

TEST_CASE("Environment Additional Test Cases 2") {
    Environment env;

    SECTION("Add and Retrieve Expressions") {
        Expression exp1(3.14);
        env.addExp("my_expression", exp1);

        // Verify that the added expression can be retrieved
        Expression retrieved1 = env.getExp("my_expression");
        REQUIRE(retrieved1 == exp1);

        std::string s1 = "hello";
        Expression exp2(s1);
        env.addExp("string_expression", exp2);

        // Verify that expressions with different types can be added and retrieved
        Expression retrieved2 = env.getExp("string_expression");
        REQUIRE(retrieved2 == exp2);
    }

    SECTION("Add and Retrieve Built-in Procedures") {
        env.init(); // Reinitialize the environment

        // Verify that built-in procedures can be retrieved
        Procedure addProc = env.getProc("+");
        Procedure mulProc = env.getProc("*");
        Procedure ltProc = env.getProc("<");

        REQUIRE(addProc != nullptr);
        REQUIRE(mulProc != nullptr);
        REQUIRE(ltProc != nullptr);
    }

    SECTION("Attempt to Retrieve Procedure as Expression") {
        env.init(); // Reinitialize the environment

        // Attempt to retrieve a procedure as an expression, which should not be allowed
        REQUIRE_FALSE(env.isExp("+"));
        REQUIRE_THROWS(env.getExp("+"));
    }

    SECTION("Attempt to Redefine Expressions") {
        Expression exp1(3.14);
        env.addExp("my_expression", exp1);

        // Attempt to redefine an existing expression, which should not be allowed
        std::string s1 = "hello";
        Expression exp2(s1);
        REQUIRE_THROWS(env.addExp("my_expression", exp2));
    }

    SECTION("Check if Unknown Symbols Are Not Expressions or Procedures") {
        // Check for unknown symbols in the environment
        REQUIRE_FALSE(env.isKnown("unknown_symbol"));
        REQUIRE_FALSE(env.isExp("unknown_symbol"));
        REQUIRE_FALSE(env.isProc("unknown_symbol"));
    }
}

TEST_CASE("Interpreter Basic Tests") {
    Interpreter interpreter;

    SECTION("Parsing Valid Expressions") {
        // Test parsing valid expressions
        std::istringstream s1("(+ 1 2)");
        std::istringstream s2("(define x 5)");
        std::istringstream s3("(if true 10 20)");
        std::istringstream s4("(begin (define x 5) (+ x 10))");
        REQUIRE(interpreter.parse(s1));
        REQUIRE(interpreter.parse(s2));
        REQUIRE(interpreter.parse(s3));
        REQUIRE(interpreter.parse(s4));
    }

    SECTION("Parsing Invalid Expressions") {
        // Test parsing invalid expressions
        std::istringstream s1("");
        std::istringstream s2(")(");
        std::istringstream s3("(+ 1 2");
        std::istringstream s4("()");
        REQUIRE_FALSE(interpreter.parse(s1));
        REQUIRE_FALSE(interpreter.parse(s2));
        REQUIRE_FALSE(interpreter.parse(s3));
        REQUIRE_FALSE(interpreter.parse(s4));
    }

    SECTION("Evaluating Valid Expressions") {
        // Test evaluating valid expressions
        std::istringstream s1("(+ 1 2)");
        interpreter.parse(s1);
        Expression result1 = interpreter.eval();
        REQUIRE(result1.head.value.num_value == 3.0);

        std::istringstream s2("(if True 10 20)");
        interpreter.parse(s2);
        Expression result2 = interpreter.eval();
        REQUIRE(result2.head.value.num_value == 10.0);

        std::istringstream s3("(begin (define x 5) (+ x 10))");
        interpreter.parse(s3);
        Expression result3 = interpreter.eval();
        REQUIRE(result3.head.value.num_value == 15.0);
    }

    SECTION("Evaluating Invalid Expressions") {
        // Test evaluating invalid expressions
        REQUIRE_THROWS_AS(interpreter.eval(), InterpreterSemanticError);
    }
}

TEST_CASE("Parsing Valid Expressions") {
    Interpreter interpreter;

    SECTION("Parsing a simple number") {
        std::istringstream input("(42)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.num_value == 42);
    }

    SECTION("Parsing a simple boolean") {
        std::istringstream input("(True)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.bool_value == true);
    }

    SECTION("Parsing a simple list") {
        std::istringstream input("(+ 1 2)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.type == NumberType);
        REQUIRE(interpreter.eval().head.value.num_value == 3);
    }

    SECTION("Parsing a nested list") {
        std::istringstream input("(+ 1 (* 2 3))");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.type == NumberType);
        REQUIRE(interpreter.eval().head.value.num_value == 7);
    }

    // Add more test cases for various valid expressions as needed.
}

TEST_CASE("Parsing Invalid Expressions") {
    Interpreter interpreter;

    SECTION("Parsing an invalid number") {
        std::istringstream input("42.5.1");
        REQUIRE_FALSE(interpreter.parse(input));
    }

    SECTION("Parsing an unbalanced parenthesis") {
        std::istringstream input("(+ 1 2");
        REQUIRE_FALSE(interpreter.parse(input));
    }

    SECTION("Parsing an invalid symbol") {
        std::istringstream input("in_val!d");
        REQUIRE_FALSE(interpreter.parse(input));
    }
}

TEST_CASE("Evaluating Expressions with 'define'") {
    Interpreter interpreter;

    SECTION("Defining and evaluating a symbol") {
        std::istringstream input("(define my_var 42)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.num_value == 42);

        std::istringstream input2("(my_var)");
        REQUIRE(interpreter.parse(input2));
        REQUIRE(interpreter.eval().head.value.num_value == 42);
    }
}

TEST_CASE("Evaluating Built-in Procedures") {
    Interpreter interpreter;

    SECTION("Addition") {
        std::istringstream input("(+ 2 3)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.num_value == 5);
    }

    SECTION("Subtraction") {
        std::istringstream input("(- 10 5)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.num_value == 5);
    }

    SECTION("Multiplication") {
        std::istringstream input("(* 4 6)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.num_value == 24);
    }

    SECTION("Division") {
        std::istringstream input("(/ 20 4)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.num_value == 5);
    }

    SECTION("Comparison (<=)") {
        std::istringstream input("(<= 3 3)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.bool_value == true);
    }
}

TEST_CASE("Evaluating Built-in Procedures 2") {
    Interpreter interpreter;

    SECTION("Addition") {
        std::istringstream input("(+ 7 3)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.num_value == 10);
    }

    SECTION("Subtraction") {
        std::istringstream input("(- 12 5)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.num_value == 7);
    }

    SECTION("Multiplication") {
        std::istringstream input("(* 9 6)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.num_value == 54);
    }

    SECTION("Division") {
        std::istringstream input("(/ 30 4)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.num_value == 7.5);
    }

    SECTION("Comparison (<=)") {
        std::istringstream input("(<= 3 7)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.bool_value == true);
    }

    SECTION("Comparison (>)") {
        std::istringstream input("(> 8 2)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.bool_value == true);
    }

    SECTION("Logical AND") {
        std::istringstream input("(and True False)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.bool_value == false);
    }

    SECTION("Logical OR") {
        std::istringstream input("(or True False)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.bool_value == true);
    }

    SECTION("Equality (=)") {
        std::istringstream input("(= 10 10)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.bool_value == true);
    }

    SECTION("Power (pow)") {
        std::istringstream input("(pow 3 2)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.num_value == 9);
    }

    SECTION("Logarithm (log10)") {
        std::istringstream input("(log10 100)");
        REQUIRE(interpreter.parse(input));
        double result = interpreter.eval().head.value.num_value;
        double expected = 2.0;
        double margin = 1e-12;
        REQUIRE(result == Approx(expected).epsilon(margin));
    }

    SECTION("Greater Than or Equal (>=)") {
        std::istringstream input("(>= 10 5)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.bool_value == true);
    }

    SECTION("Logical NOT (not)") {
        std::istringstream input("(not False)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.bool_value == true);
    }
}


TEST_CASE("Evaluating Conditional Expressions") {
    Interpreter interpreter;

    SECTION("Basic If-Else (True condition)") {
        std::istringstream input("(if True 42 24)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.num_value == 42);
    }

    SECTION("Basic If-Else (False condition)") {
        std::istringstream input("(if False 42 24)");
        REQUIRE(interpreter.parse(input));
        REQUIRE(interpreter.eval().head.value.num_value == 24);
    }
}

TEST_CASE("Invalid Expressions") {
    Interpreter interpreter;

    SECTION("Invalid Arithmetic Expression") {
        std::istringstream input("(/ 10 0)");
        REQUIRE(interpreter.parse(input));
        REQUIRE_THROWS_AS(interpreter.eval(), InterpreterSemanticError);
    }

    SECTION("Invalid If-Else Expression") {
        std::istringstream input("(if 3 42 24)");
        REQUIRE(interpreter.parse(input));
        REQUIRE_THROWS_AS(interpreter.eval(), InterpreterSemanticError);
    }
}

TEST_CASE("InterpreterSemanticError Constructor") {
    SECTION("Creating an error with a message") {
        const std::string errorMessage = "Test error message";
        InterpreterSemanticError error(errorMessage);
        REQUIRE(error.what() == errorMessage);
    }
}

TEST_CASE("Environment addExp Tests") {
    Environment env;

    SECTION("Adding a new expression") {
        const Symbol symbol = "x";
        Expression exp;
        exp.head.type = NumberType;
        exp.head.value.num_value = 42.0;

        env.addExp(symbol, exp);

        REQUIRE(env.isKnown(symbol));
        REQUIRE(env.isExp(symbol));
        REQUIRE(env.getExp(symbol) == exp);
    }

    SECTION("Redefining a symbol") {
        const Symbol symbol = "y";
        Expression exp1, exp2;
        exp1.head.type = NumberType;
        exp1.head.value.num_value = 10.0;
        exp2.head.type = NumberType;
        exp2.head.value.num_value = 20.0;

        env.addExp(symbol, exp1);

        // Redefining the symbol should throw an error
        REQUIRE_THROWS_AS(env.addExp(symbol, exp2), InterpreterSemanticError);
    }
}

TEST_CASE("Atom Conversion") {
    Interpreter interpreter;

    SECTION("Numbers") {
        std::string numToken = "42.5";
        Expression result = interpreter.atom(numToken);
        REQUIRE(result.head.type == NumberType);
        REQUIRE(result.head.value.num_value == Approx(42.5).epsilon(1e-12));
    }

    SECTION("Booleans") {
        std::string trueToken = "True";
        Expression trueResult = interpreter.atom(trueToken);
        REQUIRE(trueResult.head.type == BooleanType);
        REQUIRE(trueResult.head.value.bool_value == true);

        std::string falseToken = "False";
        Expression falseResult = interpreter.atom(falseToken);
        REQUIRE(falseResult.head.type == BooleanType);
        REQUIRE(falseResult.head.value.bool_value == false);
    }

    SECTION("Symbols") {
        std::string symbolToken = "+";
        Expression result = interpreter.atom(symbolToken);
        REQUIRE(result.head.type == SymbolType);
        REQUIRE(result.head.value.sym_value == "+");
    }

    SECTION("Invalid Tokens") {
        std::string invalidToken = "_invalid_token";
        REQUIRE_THROWS_AS(interpreter.atom(invalidToken), InterpreterSemanticError);
    }
}

TEST_CASE("Point constructor and equality operator", "[point]") {
    SECTION("Create Point and check equality") {
        Expression point1(std::make_tuple(1.0, 2.0));
        Expression point2(std::make_tuple(1.0, 2.0));
        Expression point3(std::make_tuple(3.0, 4.0));

        REQUIRE(point1 == point2);  // Same points, should be equal
        REQUIRE_FALSE(point1 == point3);  // Different points, should not be equal
    }
}

TEST_CASE("Line constructor and equality operator", "[line]") {
    SECTION("Create Line and check equality") {
        Expression line1(std::make_tuple(1.0, 2.0), std::make_tuple(3.0, 4.0));
        Expression line2(std::make_tuple(1.0, 2.0), std::make_tuple(3.0, 4.0));
        Expression line3(std::make_tuple(5.0, 6.0), std::make_tuple(7.0, 8.0));

        REQUIRE(line1 == line2);  // Same lines, should be equal
        REQUIRE_FALSE(line1 == line3);  // Different lines, should not be equal
    }
}

TEST_CASE("Arc constructor and equality operator", "[arc]") {
    SECTION("Create Arc and check equality") {
        Expression arc1(std::make_tuple(1.0, 2.0), std::make_tuple(3.0, 4.0), 1.5);
        Expression arc2(std::make_tuple(1.0, 2.0), std::make_tuple(3.0, 4.0), 1.5);
        Expression arc3(std::make_tuple(5.0, 6.0), std::make_tuple(7.0, 8.0), 2.0);

        REQUIRE(arc1 == arc2);  // Same arcs, should be equal
        REQUIRE_FALSE(arc1 == arc3);  // Different arcs, should not be equal
    }
}

TEST_CASE("Expression output stream operator", "[output]") {
    SECTION("Print Expression") {
        Expression point(std::make_tuple(1.0, 2.0));
        Expression line(std::make_tuple(1.0, 2.0), std::make_tuple(3.0, 4.0));
        Expression arc(std::make_tuple(1.0, 2.0), std::make_tuple(3.0, 4.0), 1.5);

        std::ostringstream pointStream, lineStream, arcStream;
        pointStream << point;
        lineStream << line;
        arcStream << arc;

        REQUIRE(pointStream.str() == "(1,2)");
        REQUIRE(lineStream.str() == "((1,2),(3,4))");
        REQUIRE(arcStream.str() == "((1,2),(3,4) 1.5)");
    }
}

TEST_CASE("Expression type checking", "[type]") {
    SECTION("Check Expression type") {
        Expression point(std::make_tuple(1.0, 2.0));
        Expression line(std::make_tuple(1.0, 2.0), std::make_tuple(3.0, 4.0));
        Expression arc(std::make_tuple(1.0, 2.0), std::make_tuple(3.0, 4.0), 1.5);

        REQUIRE(point.head.type == PointType);
        REQUIRE(line.head.type == LineType);
        REQUIRE(arc.head.type == ArcType);
    }
}

TEST_CASE("Expression equality for different types", "[equality]") {
    SECTION("Check Expression equality for different types") {
        Expression point(std::make_tuple(1.0, 2.0));
        Expression line(std::make_tuple(1.0, 2.0), std::make_tuple(3.0, 4.0));
        Expression arc(std::make_tuple(1.0, 2.0), std::make_tuple(3.0, 4.0), 1.5);

        REQUIRE_FALSE(point == line);  // Different types, should not be equal
        REQUIRE_FALSE(point == arc);   // Different types, should not be equal
        REQUIRE_FALSE(line == arc);    // Different types, should not be equal
    }
}

TEST_CASE("Expression output stream operator for different types", "[output]") {
    SECTION("Print Point Expression") {
        Expression point(std::make_tuple(1.0, 2.0));
        std::ostringstream pointStream;
        pointStream << point;
        REQUIRE(pointStream.str() == "(1,2)");
    }

    SECTION("Print Line Expression") {
        Expression line(std::make_tuple(1.0, 2.0), std::make_tuple(3.0, 4.0));
        std::ostringstream lineStream;
        lineStream << line;
        REQUIRE(lineStream.str() == "((1,2),(3,4))");
    }

    SECTION("Print Arc Expression") {
        Expression arc(std::make_tuple(1.0, 2.0), std::make_tuple(3.0, 4.0), 1.5);
        std::ostringstream arcStream;
        arcStream << arc;
        REQUIRE(arcStream.str() == "((1,2),(3,4) 1.5)");
    }
}

TEST_CASE("Expression constructors and output stream operator", "[expression]") {
    SECTION("Point Expression") {
        Expression point(std::make_tuple(1.0, 2.0));
        REQUIRE(point.head.type == PointType);
        REQUIRE(point.head.value.point_value.x == 1.0);
        REQUIRE(point.head.value.point_value.y == 2.0);
    }

    SECTION("Line Expression") {
        Expression line(std::make_tuple(1.0, 2.0), std::make_tuple(3.0, 4.0));
        REQUIRE(line.head.type == LineType);
        REQUIRE(line.head.value.line_value.first.x == 1.0);
        REQUIRE(line.head.value.line_value.first.y == 2.0);
        REQUIRE(line.head.value.line_value.second.x == 3.0);
        REQUIRE(line.head.value.line_value.second.y == 4.0);
    }

    SECTION("Arc Expression") {
        Expression arc(std::make_tuple(1.0, 2.0), std::make_tuple(3.0, 4.0), 1.5);
        REQUIRE(arc.head.type == ArcType);
        REQUIRE(arc.head.value.arc_value.center.x == 1.0);
        REQUIRE(arc.head.value.arc_value.center.y == 2.0);
        REQUIRE(arc.head.value.arc_value.start.x == 3.0);
        REQUIRE(arc.head.value.arc_value.start.y == 4.0);
        REQUIRE(arc.head.value.arc_value.span == 1.5);
    }

    SECTION("Output stream operator for different types") {
        Expression point(std::make_tuple(1.0, 2.0));
        Expression line(std::make_tuple(1.0, 2.0), std::make_tuple(3.0, 4.0));
        Expression arc(std::make_tuple(1.0, 2.0), std::make_tuple(3.0, 4.0), 1.5);

        std::ostringstream output;

        output << point;
        REQUIRE(output.str() == "(1,2)");

        output.str(""); // Clear the stream
        output << line;
        REQUIRE(output.str() == "((1,2),(3,4))");

        output.str(""); // Clear the stream
        output << arc;
        REQUIRE(output.str() == "((1,2),(3,4) 1.5)");
    }
}


