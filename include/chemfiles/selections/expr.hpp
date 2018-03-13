// Chemfiles, a modern library for chemistry file reading and writing
// Copyright (C) Guillaume Fraux and contributors -- BSD license

#ifndef CHEMFILES_SELECTION_EXPR_HPP
#define CHEMFILES_SELECTION_EXPR_HPP

#include <string>
#include <memory>
#include <cassert>
#include <functional>

namespace chemfiles {

class Frame;
class Match;

namespace selections {

/// Abstract base class for selectors in the selection AST
class Selector {
public:
    /// Pretty-printing of this selector. The output should use a shift
    /// of `delta` spaces in case of multilines output.
    virtual std::string print(unsigned delta = 0) const = 0;
    /// Check if the `match` is valid in the given `frame`.
    virtual bool is_match(const Frame& frame, const Match& match) const = 0;

    Selector() = default;
    virtual ~Selector() = default;

    Selector(Selector&&) = default;
    Selector& operator=(Selector&&) = default;

    Selector(const Selector&) = delete;
    Selector& operator=(const Selector&) = delete;
};

using Ast = std::unique_ptr<Selector>;

/// Combine selections by using a logical `and` operation
class And final: public Selector {
public:
    And(Ast lhs, Ast rhs): lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
    std::string print(unsigned delta) const override;
    bool is_match(const Frame& frame, const Match& match) const override;
private:
    Ast lhs_;
    Ast rhs_;
};

/// Combine selections by using a logical `or` operation
class Or final: public Selector {
public:
    Or(Ast lhs, Ast rhs): lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
    std::string print(unsigned delta) const override;
    bool is_match(const Frame& frame, const Match& match) const override;
private:
    Ast lhs_;
    Ast rhs_;
};

/// Unary negation of a selection
class Not final: public Selector {
public:
    explicit Not(Ast ast): ast_(std::move(ast)) {}
    std::string print(unsigned delta) const override;
    bool is_match(const Frame& frame, const Match& match) const override;
private:
    Ast ast_;
};

/// Selection matching all atoms
class All final: public Selector {
public:
    All() = default;
    std::string print(unsigned delta) const override;
    bool is_match(const Frame& frame, const Match& match) const override;
};

/// Selection matching no atoms
class None final: public Selector {
public:
    None() = default;
    std::string print(unsigned delta) const override;
    bool is_match(const Frame& frame, const Match& match) const override;
};

/// Abstract base class for string selector
class StringSelector: public Selector {
public:
    StringSelector(std::string value, bool equals, unsigned argument):
        value_(std::move(value)), equals_(equals), argument_(argument)
    {
        assert(argument <= 3 && "argument must be less than 3 in SingleSelector");
    }
    virtual ~StringSelector() = default;

    /// Get the value for the atom at index `i` in the `frame`
    virtual const std::string& value(const Frame& frame, size_t i) const = 0;
    /// Get the property name
    virtual std::string name() const = 0;

    bool is_match(const Frame& frame, const Match& match) const override final;
    std::string print(unsigned delta) const override final;

private:
    /// The value to check against
    std::string value_;
    /// Are we checking for equality or inequality?
    bool equals_;
    /// Which atom in the candidate match are we checking?
    unsigned argument_;
};

/// Select atoms using their type
class Type final: public StringSelector {
public:
    Type(std::string value, bool equals, unsigned argument):
        StringSelector(std::move(value), equals, argument) {}

    std::string name() const override;
    const std::string& value(const Frame& frame, size_t i) const override;
};

/// Select atoms using their name
class Name final: public StringSelector {
public:
    Name(std::string value, bool equals, unsigned argument):
        StringSelector(std::move(value), equals, argument) {}

    std::string name() const override;
    const std::string& value(const Frame& frame, size_t i) const override;
};

/// Select atoms using their residue name
class Resname final: public StringSelector {
public:
    Resname(std::string value, bool equals, unsigned argument):
        StringSelector(std::move(value), equals, argument) {}

    std::string name() const override;
    const std::string& value(const Frame& frame, size_t i) const override;
};

class MathExpr;
using MathAst = std::unique_ptr<MathExpr>;

/// Expression for math selectors
class Math final: public Selector {
public:
    enum class Operator {
        EQUAL,
        NOT_EQUAL,
        LESS,
        LESS_EQUAL,
        GREATER,
        GREATER_EQUAL,
    };

    Math(Operator op, MathAst lhs, MathAst rhs): op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

    bool is_match(const Frame& frame, const Match& match) const override;
    std::string print(unsigned delta) const override;

private:
    Operator op_;
    MathAst lhs_;
    MathAst rhs_;
};

/// Abstract base class for mathematical expressions
class MathExpr {
public:
    MathExpr() = default;
    virtual ~MathExpr() = default;

    /// Evaluate the expression and get the value
    virtual double eval(const Frame& frame, const Match& match) const = 0;
    /// Pretty-print the expression
    virtual std::string print() const = 0;
};

// Addition
class Add final: public MathExpr {
public:
    Add(MathAst lhs, MathAst rhs): lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

    double eval(const Frame& frame, const Match& match) const override;
    std::string print() const override;
private:
    MathAst lhs_;
    MathAst rhs_;
};

// Substraction
class Sub final: public MathExpr {
public:
    Sub(MathAst lhs, MathAst rhs): lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

    double eval(const Frame& frame, const Match& match) const override;
    std::string print() const override;
private:
    MathAst lhs_;
    MathAst rhs_;
};

// Multiplication
class Mul final: public MathExpr {
public:
    Mul(MathAst lhs, MathAst rhs): lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

    double eval(const Frame& frame, const Match& match) const override;
    std::string print() const override;
private:
    MathAst lhs_;
    MathAst rhs_;
};

// Division
class Div final: public MathExpr {
public:
    Div(MathAst lhs, MathAst rhs): lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

    double eval(const Frame& frame, const Match& match) const override;
    std::string print() const override;
private:
    MathAst lhs_;
    MathAst rhs_;
};

/// Power raising
class Pow final: public MathExpr {
public:
    Pow(MathAst lhs, MathAst rhs): lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

    double eval(const Frame& frame, const Match& match) const override;
    std::string print() const override;
private:
    MathAst lhs_;
    MathAst rhs_;
};

/// Unary minus operator
class Neg final: public MathExpr {
public:
    Neg(MathAst ast): ast_(std::move(ast)) {}

    double eval(const Frame& frame, const Match& match) const override;
    std::string print() const override;

private:
    MathAst ast_;
};


/// Function call
class Function final: public MathExpr {
public:
    Function(std::function<double(double)> fn, std::string name, MathAst ast):
        fn_(std::move(fn)), name_(std::move(name)), ast_(std::move(ast)) {}

    double eval(const Frame& frame, const Match& match) const override;
    std::string print() const override;

private:
    std::function<double(double)> fn_;
    std::string name_;
    MathAst ast_;
};

/// Literal number
class Number final: public MathExpr {
public:
    Number(double value): value_(value) {}

    double eval(const Frame& frame, const Match& match) const override;
    std::string print() const override;

private:
    double value_;
};

/// Abstract base class for numeric properties
class NumericProperty: public MathExpr {
public:
    NumericProperty(unsigned argument): argument_(argument) {}
    virtual ~NumericProperty() = default;

    double eval(const Frame& frame, const Match& match) const override final;
    std::string print() const override final;

    /// Get the value of the property for the atom at index `i` in the `frame`
    virtual double value(const Frame& frame, size_t i) const = 0;
    /// Get the name of the property
    virtual std::string name() const = 0;
private:
    /// Which atom in the candidate match are we checking?
    unsigned argument_;
};

/// Select atoms using their index in the frame.
class Index final: public NumericProperty {
public:
    Index(unsigned argument): NumericProperty(argument) {}
    std::string name() const override;
    double value(const Frame& frame, size_t i) const override;
};

/// Select atoms using their residue id (residue number)
class Resid final: public NumericProperty {
public:
    Resid(unsigned argument): NumericProperty(argument) {}
    std::string name() const override;
    double value(const Frame& frame, size_t i) const override;
};

/// Select atoms using their mass.
class Mass final: public NumericProperty {
public:
    Mass(unsigned argument): NumericProperty(argument) {}
    std::string name() const override;
    double value(const Frame& frame, size_t i) const override;
};

enum class Coordinate {
    X = 0,
    Y = 1,
    Z = 2,
};

/// Select atoms using their position in space. The selection can be created by
/// `x <op> <val>`, `y <op> <val>` or `z <op> <val>`, depending on the component
/// of the position to use.
class Position final: public NumericProperty {
public:
    Position(unsigned argument, Coordinate coordinate): NumericProperty(argument), coordinate_(coordinate) {}
    std::string name() const override;
    double value(const Frame& frame, size_t i) const override;
private:
    Coordinate coordinate_;
};

/// Select atoms using their velocity. The selection can be created by `vx <op>
/// <val>`, `vy <op> <val>` or `vz <op> <val>`, depending on the component of
/// the velocity to use.
class Velocity final: public NumericProperty {
public:
    Velocity(unsigned argument, Coordinate coordinate): NumericProperty(argument), coordinate_(coordinate) {}
    std::string name() const override;
    double value(const Frame& frame, size_t i) const override;
private:
    Coordinate coordinate_;
};

}} // namespace chemfiles && namespace selections

#endif
