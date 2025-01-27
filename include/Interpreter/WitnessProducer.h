#ifndef INTERPRETER_WITNESS_PRODUCER_H
#define INTERPRETER_WITNESS_PRODUCER_H

#include "Interpreter/Program.h"
#include "Utils/Generator.h"
#include "Utils/Unit.h"

namespace interpreter {

bool match(
    const PredicateReference &pr,
    const PredicateReference &matcher,
    std::vector<Value> &existentialVariables,
    std::vector<Value> &universalVariables
);

bool match(
    const VariableRef &vr,
    const VariableRef &matcher,
    std::vector<Value> &existentialVariables,
    std::vector<Value> &universalVariables
);

bool match(
    const VariableRef &vr,
    const ConstructorRef &cr,
    std::vector<Value> &existentialVariables,
    std::vector<Value> &universalVariables
);

bool match(
    const ConstructorRef &cl,
    const ConstructorRef &cr,
    std::vector<Value> &existentialVariables,
    std::vector<Value> &universalVariables
);

bool match(
    const Value &left,
    const Value &right,
    std::vector<Value> &existentialVariables,
    std::vector<Value> &universalVariables
);

Expression instantiate(
    const Expression &expr,
    const std::vector<Value> &variables
);

/**
 * A generator which enumerates the witnesses of expr.
 * 
 * @param prog The enclosing program in which to resolve predicates.
 * @param expr The expression to be proven.
 * @param variables Enclosing scope in which to lookup variable values.
 */
Generator<Unit> witnesses(
    const Program &prog,
    const Expression expr,
    std::vector<Value> &variables
);

Generator<Unit> witnesses(const TruthValue &tv);

Generator<Unit> witnesses(
    const Program &prog,
    const PredicateReference &pr,
    std::vector<Value> &enclosingVariables
);

Generator<Unit> witnesses(
    const Program &prog,
    const Conjunction conj,
    std::vector<Value> &variables
);

} // namespace interpreter

#endif // INTERPRETER_WITNESS_PRODUCER_H
