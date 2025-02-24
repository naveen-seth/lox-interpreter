module;

template <typename... Ts>
struct Overload : Ts... {
  using Ts::operator()...;
};

export module Expressions;

import std;
import TokenTypes;

using namespace token;

// Slighty modified version of
// https://www.foonathan.net/2022/05/recursive-variant-box/
template <typename T> class Box {
  std::unique_ptr<T> m_inner;

public:
  // Automatic construction from a `T`, not a `T*`.
  Box(T &&obj) : m_inner(new T(std::move(obj))) {}
  Box(const T &obj) : m_inner(new T(obj)) {}

  Box(const Box &other) : Box(*other.m_inner) {}
  Box &operator=(const Box &other) {
    // Case: assignment to self
    if (this == &other) {
      return *this;
    }
    *m_inner = *other.m_inner;
    return *this;
  }

  // Disallowing move rules out Box with deleted m_inner
  Box(Box &&) = delete;
  Box &operator=(Box &&) = delete;

  ~Box() = default;

  T &operator*() { return *m_inner; }
  const T &operator*() const { return *m_inner; }

  T *operator->() { return m_inner.get(); }
  const T *operator->() const { return m_inner.get(); }
};

// Syntax definition
export namespace expr {

using Expr = std::variant<struct Literal, Box<struct Unary>, Box<struct Binary>, Box<struct Grouping>>;

struct Literal {
  Token token;
};

struct Unary {
  Token op;
  Expr expr;
};

struct Binary {
  Expr lhsExpr;
  Token op;
  Expr rhsExpr;
};

struct Grouping {
  Expr expr;
};

// AST printer
std::string parenthesize(const std::string &name, std::initializer_list<Expr> exprs);


[[nodiscard]] std::string formatExpr(const Expr &expr) {
  return std::visit(
      Overload{[](const Literal &literal) {
                 return parenthesize(std::format("{}", literal.token), {});
               },
               [](const Box<Unary> &unary) {
                 return parenthesize(std::format("{}", unary->op),
                                     {unary->expr});
               },
               [](const Box<Binary> &binary) {
                 return parenthesize(std::format("{}", binary->op),
                                     {binary->lhsExpr, binary->rhsExpr});
               },
               [](const Box<Grouping> &group) {
                 return parenthesize("group", {group->expr});
               }
      },
      expr);
}

std::string parenthesize(const std::string &name,
                         std::initializer_list<Expr> exprs) {
  std::string res{"(" + name};
  for (const auto &expr : exprs) {
    res.append(" ");
    res.append(formatExpr(expr));
  }
  res.append(")");
  return res;
}
}

