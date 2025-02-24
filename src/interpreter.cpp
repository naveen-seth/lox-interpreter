export module Interpreter;
import std;
import Lexer;
import Expressions;
import TokenTypes;

using namespace expr;
using namespace token;

export class Lox {
 public:
  static void runFile(const std::filesystem::path &path) {
    std::ifstream ifs{path};
    auto source = std::string{std::istreambuf_iterator<char>{ifs},
                              std::istreambuf_iterator<char>{}};

    // Test lexer
    Lexer lexer{source};
    auto lexedTokens = lexer.ScanTokens();
    std::println("Tokenized input file:");
    for (std::size_t i = 0; i < lexedTokens.size(); ++i) {
      std::print("{} ", lexedTokens.tokenList[i]);
    }
    std::print("\n\n");

    // Test AST printer
    std::println("AST printer singleton tests:");
    Expr expr1{Literal{Token{String{"Hello, world!"}}}};
    std::println("AST test1:{}", formatExpr(expr1));
    Expr expr2{Binary{Literal{Token{Number{10}}}, Token{Star{}},
                      Literal{Token{Number{3.141}}}}};
    std::println("AST test2:{}", formatExpr(expr2));
  }
};
