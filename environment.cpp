#include "environment.hpp"

#include <cassert>
#include <cmath>

#include "interpreter_semantic_error.hpp"

using namespace std;
const double PI = atan2(0, -1);

Environment::Environment() {
	init();
}

Expression add(const std::vector<Atom>& args) {
	if (args.empty()) {
		throw InterpreterSemanticError("Wrong number of args for +");
	}
	double result = 0.0;
	for (const Atom& arg : args) {
		if (arg.type == NumberType) {
			result += arg.value.num_value;
		}
		else {
			throw InterpreterSemanticError("Invalid argument type for +");
		}
	}

	return Expression(result);
}

Expression mul(const std::vector<Atom>& args) {
	if (args.empty()) {
		throw InterpreterSemanticError("mul failed, expected at least one argument");
	}

	Number result = 1.0;

	for (const Atom& arg : args) {
		if (arg.type != NumberType) {
			throw InterpreterSemanticError("mul failed, argument not a number");
		}

		result *= arg.value.num_value;
	}

	return Expression(result);
}

Expression div(const std::vector<Atom>& args) {
	if (args.size() != 2) {
		throw InterpreterSemanticError("div failed, expected exactly two arguments");
	}

	if (args[0].type != NumberType || args[1].type != NumberType) {
		throw InterpreterSemanticError("div failed, arguments not numbers");
	}

	if (args[1].value.num_value == 0) {
		throw InterpreterSemanticError("div failed, division by zero");
	}

	Number result = args[0].value.num_value / args[1].value.num_value;
	return Expression(result);
}

Expression subneg(const std::vector<Atom>& args) {
	if (args.size() == 1) {
		if (args[0].type != NumberType) {
			throw InterpreterSemanticError("subneg failed, expected a single number argument");
		}
		Number result = -args[0].value.num_value;
		return Expression(result);
	}

	if (args.size() == 2) {
		if (args[0].type != NumberType || args[1].type != NumberType) {
			throw InterpreterSemanticError("subneg failed, expected two number arguments");
		}
		Number result = args[0].value.num_value - args[1].value.num_value;
		return Expression(result);
	}

	throw InterpreterSemanticError("subneg failed, invalid number of arguments");
}

Expression gt(const std::vector<Atom>& args) {
	if (args.size() != 2) {
		throw InterpreterSemanticError("gt failed, expected exactly two arguments");
	}

	if (args[0].type != NumberType || args[1].type != NumberType) {
		throw InterpreterSemanticError("gt failed, arguments not numbers");
	}

	bool result = args[0].value.num_value > args[1].value.num_value;
	return Expression(result);
}

Expression lt(const std::vector<Atom>& args) {
	if (args.size() != 2) {
		throw InterpreterSemanticError("lt failed, expected exactly two arguments");
	}

	if (args[0].type != NumberType || args[1].type != NumberType) {
		throw InterpreterSemanticError("lt failed, arguments not numbers");
	}

	bool result = args[0].value.num_value < args[1].value.num_value;
	return Expression(result);
}

Expression gteq(const std::vector<Atom>& args) {
	if (args.size() != 2) {
		throw InterpreterSemanticError("gteq failed, expected exactly two arguments");
	}

	if (args[0].type != NumberType || args[1].type != NumberType) {
		throw InterpreterSemanticError("gteq failed, arguments not numbers");
	}

	bool result = args[0].value.num_value >= args[1].value.num_value;
	return Expression(result);
}

Expression eq(const std::vector<Atom>& args) {
	if (args.size() != 2) {
		throw InterpreterSemanticError("eq failed, expected exactly two arguments");
	}

	if (args[0].type != NumberType || args[1].type != NumberType) {
		throw InterpreterSemanticError("eq failed, arguments not numbers");
	}

	bool result = args[0].value.num_value == args[1].value.num_value;
	return Expression(result);
}

Expression lteq(const std::vector<Atom>& args) {
	if (args.size() != 2) {
		throw InterpreterSemanticError("lteq failed, expected exactly two arguments");
	}

	if (args[0].type != NumberType || args[1].type != NumberType) {
		throw InterpreterSemanticError("lteq failed, arguments not numbers");
	}

	bool result = args[0].value.num_value <= args[1].value.num_value;
	return Expression(result);
}

Expression log10(const std::vector<Atom>& args) {
	if (args.size() != 1) {
		throw InterpreterSemanticError("log10 failed, expected exactly one argument");
	}

	if (args[0].type != NumberType) {
		throw InterpreterSemanticError("log10 failed, argument not a number");
	}

	if (args[0].value.num_value <= 0) {
		throw InterpreterSemanticError("log10 failed, argument must be greater than zero");
	}

	Number result = float(log10(args[0].value.num_value));
	return Expression(result);
}

Expression pow(const std::vector<Atom>& args) {
	if (args.size() != 2) {
		throw InterpreterSemanticError("pow failed, expected exactly two arguments");
	}

	if (args[0].type != NumberType || args[1].type != NumberType) {
		throw InterpreterSemanticError("pow failed, arguments not numbers");
	}

	Number base = args[0].value.num_value;
	Number exponent = args[1].value.num_value;

	Number result = pow(base, exponent);
	return Expression(result);
}

Expression Not(const std::vector<Atom>& args) {
	if (args.size() != 1) {
		throw InterpreterSemanticError("not failed, expected exactly one argument");
	}

	if (args[0].type != BooleanType) {
		throw InterpreterSemanticError("not failed, argument not a boolean");
	}

	bool result = !args[0].value.bool_value;
	return Expression(result);
}

Expression Or(const std::vector<Atom>& args) {
	if (args.empty()) {
		throw InterpreterSemanticError("or failed, expected at least one argument");
	}

	for (const Atom& arg : args) {
		if (arg.type != BooleanType) {
			throw InterpreterSemanticError("or failed, argument not a boolean");
		}

		if (arg.value.bool_value) {
			return Expression(true);
		}
	}

	return Expression(false);
}

Expression And(const std::vector<Atom>& args) {
	if (args.empty()) {
		throw InterpreterSemanticError("and failed, expected at least one argument");
	}

	for (const Atom& arg : args) {
		if (arg.type != BooleanType) {
			throw InterpreterSemanticError("and failed, argument not a boolean");
		}

		if (!arg.value.bool_value) {
			return Expression(false);
		}
	}

	return Expression(true);
}

Expression makePoint(const std::vector<Atom>& args) {
	if (args.size() != 2) {
		throw InterpreterSemanticError("point failed, expected exactly two arguments");
	}

	if (args[0].type != NumberType || args[1].type != NumberType) {
		throw InterpreterSemanticError("point failed, arguments not numbers");
	}

	Expression point(
		std::make_tuple(args[0].value.num_value, args[1].value.num_value)
	);

	return point;
}

Expression makeLine(const std::vector<Atom>& args) {
	if (args.size() != 2) {
		throw InterpreterSemanticError("line failed, expected exactly two arguments");
	}

	if (args[0].type != PointType || args[1].type != PointType) {
		throw InterpreterSemanticError("line failed, arguments not points");
	}

	Expression line(
		std::make_tuple(
			args[0].value.point_value.x,
			args[0].value.point_value.y
		),
		std::make_tuple(
			args[1].value.point_value.x,
			args[1].value.point_value.y
		)
	);

	return line;
}

Expression makeArc(const std::vector<Atom>& args) {
	if (args.size() != 3) {
		throw InterpreterSemanticError("arc failed, expected exactly three arguments");
	}

	if (args[0].type != PointType || args[1].type != PointType || args[2].type != NumberType) {
		throw InterpreterSemanticError("arc failed, invalid argument types");
	}

	Expression arc(
		std::make_tuple(
			args[0].value.point_value.x,
			args[0].value.point_value.y
		),
		std::make_tuple(
			args[1].value.point_value.x,
			args[1].value.point_value.y
		),
		args[2].value.num_value
	);

	return arc;
}

Expression sinFunc(const std::vector<Atom>& args) {
	if (args.size() != 1) {
		throw InterpreterSemanticError("sin failed, expected exactly one argument");
	}

	if (args[0].type != NumberType) {
		throw InterpreterSemanticError("sin failed, argument not a number");
	}

	Number result = sin(args[0].value.num_value);
	return Expression(result);
}

Expression cosFunc(const std::vector<Atom>& args) {
	if (args.size() != 1) {
		throw InterpreterSemanticError("cos failed, expected exactly one argument");
	}

	if (args[0].type != NumberType) {
		throw InterpreterSemanticError("cos failed, argument not a number");
	}

	Number result = cos(args[0].value.num_value);
	return Expression(result);
}

Expression arctanFunc(const std::vector<Atom>& args) {
	if (args.size() != 2) {
		throw InterpreterSemanticError("arctan failed, expected exactly two arguments");
	}

	if (args[0].type != NumberType || args[1].type != NumberType) {
		throw InterpreterSemanticError("arctan failed, arguments not numbers");
	}

	Number result = atan2(args[0].value.num_value, args[1].value.num_value);
	return Expression(result);
}

Expression LiteralBool(bool x) {
	Expression result;
	result.head.type = BooleanType;
	result.head.value.bool_value = x;
	return result;
}

Expression LiteralNumber(Number num) {
	Expression result;
	result.head.type = NumberType;
	result.head.value.num_value = num;
	return result;
}

bool Environment::isKnown(const Symbol& sym) {
	return envmap.find(sym) != envmap.end();
}

bool Environment::isExp(const Symbol& sym) {
	auto found = envmap.find(sym);
	return found != envmap.end() && found->second.type == ExpressionType;
}

// Get an expression associated with a symbol
Expression Environment::getExp(const Symbol& sym) const {
	auto it = envmap.find(sym);
	if (it != envmap.end() && it->second.type == ExpressionType) {
		return it->second.exp;
	}
	throw InterpreterSemanticError("Symbol not found or does not contain an expression.");
}

void Environment::addExp(const Symbol& sym, const Expression& exp) {
	EnvResult result;
	result.type = ExpressionType;
	result.exp = exp;

	auto it = envmap.find(sym);
	if (it != envmap.end()) {
		throw InterpreterSemanticError("Symbol redefinition is not allowed.");
	}

	envmap[sym] = result;
}

bool Environment::isProc(const Symbol& sym) const {
	auto it = envmap.find(sym);
	return (it != envmap.end() && it->second.type == ProcedureType);
}

Procedure Environment::getProc(const Symbol& sym) const {
	auto it = envmap.find(sym);
	if (it != envmap.end() && it->second.type == ProcedureType) {
		return it->second.proc;
	}
	throw std::runtime_error("Symbol not found or does not contain a procedure.");
}

void Environment::init() {
	envmap.clear();

	envmap["<"];
	envmap["<"].type = ProcedureType;
	envmap["<"].proc = lt;

	envmap[">="];
	envmap[">="].type = ProcedureType;
	envmap[">="].proc = gteq;

	envmap["+"];
	envmap["+"].type = ProcedureType;
	envmap["+"].proc = add;

	envmap["<="];
	envmap["<="].type = ProcedureType;
	envmap["<="].proc = lteq;

	envmap["and"];
	envmap["and"].type = ProcedureType;
	envmap["and"].proc = And;

	envmap["not"];
	envmap["not"].type = ProcedureType;
	envmap["not"].proc = Not;

	envmap["or"];
	envmap["or"].type = ProcedureType;
	envmap["or"].proc = Or;

	envmap["="];
	envmap["="].type = ProcedureType;
	envmap["="].proc = eq;

	envmap[">"];
	envmap[">"].type = ProcedureType;
	envmap[">"].proc = gt;

	envmap["*"];
	envmap["*"].type = ProcedureType;
	envmap["*"].proc = mul;

	envmap["-"];
	envmap["-"].type = ProcedureType;
	envmap["-"].proc = subneg;

	envmap["log10"];
	envmap["log10"].type = ProcedureType;
	envmap["log10"].proc = log10;

	envmap["/"];
	envmap["/"].type = ProcedureType;
	envmap["/"].proc = div;

	envmap["pi"];
	envmap["pi"].type = ExpressionType;
	envmap["pi"].exp = LiteralNumber(PI);

	envmap["pow"];
	envmap["pow"].type = ProcedureType;
	envmap["pow"].proc = pow;

	envmap["point"];
	envmap["point"].type = ProcedureType;
	envmap["point"].proc = makePoint;

	envmap["line"];
	envmap["line"].type = ProcedureType;
	envmap["line"].proc = makeLine;

	envmap["arc"];
	envmap["arc"].type = ProcedureType;
	envmap["arc"].proc = makeArc;

	envmap["sin"];
	envmap["sin"].type = ProcedureType;
	envmap["sin"].proc = sinFunc;

	envmap["cos"];
	envmap["cos"].type = ProcedureType;
	envmap["cos"].proc = cosFunc;

	envmap["arctan"];
	envmap["arctan"].type = ProcedureType;
	envmap["arctan"].proc = arctanFunc;

}
