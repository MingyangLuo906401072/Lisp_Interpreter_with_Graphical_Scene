#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

// system includes
#include <map>

// module includes
#include "expression.hpp"

class Environment {
public:
    Environment();
    bool isKnown(const Symbol& sym);
    bool isExp(const Symbol& sym);
    Expression getExp(const Symbol& sym) const;
    void addExp(const Symbol& sym, const Expression& exp);
    bool isProc(const Symbol& sym) const;
    Procedure getProc(const Symbol& sym) const;
    void init();

private:
    // Environment is a mapping from symbols to expressions or procedures
    enum EnvResultType { ExpressionType, ProcedureType };
    struct EnvResult {
        EnvResultType type;
        Expression exp;
        Procedure proc;
    };

    std::map<Symbol, EnvResult> envmap;
};

#endif
