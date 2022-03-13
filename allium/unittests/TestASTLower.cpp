#include <gtest/gtest.h>

#include "Interpreter/ASTLower.h"
#include "SemAna/Builtins.h"

using namespace TypedAST;

TEST(TestASTLower, variables_of_uninhabited_types_are_marked) {
    AST ast(
        { Type(TypeDecl("Void"), {}) },
        {},
        {
            Predicate(
                PredicateDecl("foo", { Parameter("Void", false) }, {}),
                {
                    Implication(
                        PredicateRef("foo", { AnonymousVariable(Name<Type>("Void")) }),
                        TruthLiteral(true)
                    )
                },
                {}
            )
        }
    );

    interpreter::MatcherValue anon((interpreter::MatcherVariable(
        interpreter::MatcherVariable::anonymousIndex, false)
    ));
    EXPECT_EQ(
        lower(ast),
        interpreter::Program(
            {
                interpreter::Predicate({
                    interpreter::Implication(
                        interpreter::PredicateReference(0, { anon }),
                        interpreter::TruthValue(true),
                        0
                    ),
                })
            },
            Optional<interpreter::PredicateReference>()
        )
    );
}

TEST(TestASTLower, string_is_not_uninhabited) {
    AST ast(
        TypedAST::builtinTypes,
        {},
        {
            Predicate(
                PredicateDecl("p", { Parameter("String", false) }, {}),
                {
                    Implication(
                        PredicateRef("p", {
                            Value(Variable("x", Name<Type>("String"), true, false))
                        }),
                        Expression(TruthLiteral(true))
                    )
                },
                {}
            )
        }
    );

    const interpreter::MatcherValue v = lower(ast).getPredicate(0).implications[0].head.arguments[0];
    EXPECT_EQ(v, interpreter::MatcherValue(interpreter::MatcherVariable(0, true)));
}

TEST(TestASTLower, existential_variables_are_uniquely_indexed) {
    AST ast(
        {
            Type(
                TypeDecl("T"),
                { Constructor("t", {}) }
            )
        },
        {},
        {
            Predicate(
                PredicateDecl("foo", { Parameter("T", false), Parameter("T", false) }, {}),
                {
                    Implication(
                        PredicateRef("foo", {
                            AnonymousVariable(Name<Type>("T")),
                            AnonymousVariable(Name<Type>("T"))
                        }),
                        Expression(PredicateRef("foo", {
                            Value(Variable("x", Name<Type>("T"), true, true)),
                            Value(Variable("y", Name<Type>("T"), true, true))
                        }))
                    )
                },
                {}
            )
        }
    );

    interpreter::MatcherValue anon((interpreter::MatcherVariable(
        interpreter::MatcherVariable::anonymousIndex
    )));
    EXPECT_EQ(
        lower(ast),
        interpreter::Program(
            {
                interpreter::Predicate({
                    interpreter::Implication(
                        interpreter::PredicateReference(0, { anon, anon }),
                        interpreter::Expression(interpreter::PredicateReference(
                            0,
                            {
                                interpreter::MatcherValue(interpreter::MatcherVariable(0, true)),
                                interpreter::MatcherValue(interpreter::MatcherVariable(1, true))
                            }
                        )),
                        2
                    ),
                })
            },
            Optional<interpreter::PredicateReference>()
        )
    );
}