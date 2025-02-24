module;

template <typename... Ts>
struct Overload : Ts... {
  using Ts::operator()...;
};

export module TokenTypes;
import std;

export namespace token {

// Unambiguous single-char tokens
struct LeftParen {};
struct RightParen {};
struct LeftBrace {};
struct RightBrace {};
struct Comma {};
struct Period {};
struct Semicolon {};
struct Minus {};
struct Plus {};
struct Star {};
struct EOF {};
// Ambiguous single-/multi-char tokens
struct Slash {};
struct Bang {};
struct BangEqual {};
struct Equal {};
struct EqualEqual {};
struct Greater {};
struct GreaterEqual {};
struct Less {};
struct LessEqual {};
// Special keywords
struct Fun {};
struct Var {};
struct Nil {};
struct If {};
struct Else {};
struct For {};
struct Print {};
struct And {};
struct Or {};
// Value tokens
struct Number {
  double value{};
};
struct String {
  std::string value;
};
struct Identifier {
  std::string value;
};
struct Boolean {
  bool value;
};

using Token = std::variant<LeftParen, RightParen, LeftBrace, RightBrace, Comma,
                           Period, Semicolon, Minus, Plus, Star, Slash, Bang,
                           BangEqual, Equal, EqualEqual, Greater, GreaterEqual,
                           Less, LessEqual, Number, String, Identifier, Fun,
                           Var, If, Else, For, Print, And, Or, Nil, EOF>;

const std::flat_map<std::string, Token> keywordTokenMap = {
    {"fun", Fun{}},     {"var", Var{}},   {"nil", Nil{}},
    {"if", If{}},       {"else", Else{}}, {"for", For{}},
    {"print", Print{}}, {"and", And{}},   {"or", Or{}},
};

struct LexedToken {
  Token token;
  std::uint32_t lineNum;
  std::uint32_t colNum;
};

class LexedTokenList {
 public:
  std::vector<Token> tokenList;
  std::vector<std::uint32_t> lineNumList;
  std::vector<std::uint32_t> colNumList;

  void push_back(Token&& token, std::uint32_t lineNum, std::uint32_t colNum) {
    tokenList.push_back(std::move(token));
    lineNumList.push_back(lineNum);
    colNumList.push_back(colNum);
  }

  [[nodiscard]] std::size_t size() const { return tokenList.size(); }
};

}  // namespace token

export std::string tokenToString(token::Token token) {
  using namespace token;
  return std::visit(Overload{
                        [&](LeftParen) -> std::string { return "("; },
                        [&](RightParen) -> std::string { return ")"; },
                        [&](LeftBrace) -> std::string { return "{"; },
                        [&](RightBrace) -> std::string { return "}"; },
                        [&](Comma) -> std::string { return ","; },
                        [&](Period) -> std::string { return "."; },
                        [&](Semicolon) -> std::string { return ";"; },
                        [&](Minus) -> std::string { return "-"; },
                        [&](Plus) -> std::string { return "+"; },
                        [&](Star) -> std::string { return "*"; },
                        [&](Slash) -> std::string { return "/"; },
                        [&](Bang) -> std::string { return "+"; },
                        [&](BangEqual) -> std::string { return "!="; },
                        [&](Equal) -> std::string { return "="; },
                        [&](EqualEqual) -> std::string { return "=="; },
                        [&](Greater) -> std::string { return ">"; },
                        [&](GreaterEqual) -> std::string { return ">="; },
                        [&](Less) -> std::string { return "<"; },
                        [&](LessEqual) -> std::string { return "<="; },
                        [&](Number number) -> std::string {
                          return std::format("{}", number.value);
                        },
                        [&](String string) -> std::string {
                          return std::format("\"{}\"", string.value);
                        },
                        [&](Boolean boolean) -> std::string {
                          return std::format("{}", boolean.value);
                        },
                        [&](Identifier identifier) -> std::string {
                          return std::format("{}", identifier.value);
                        },
                        [&](Nil) -> std::string { return "Nil"; },
                        [&](Fun) -> std::string { return "Fun"; },
                        [&](Var) -> std::string { return "Var"; },
                        [&](If) -> std::string { return "If"; },
                        [&](Else) -> std::string { return "Else"; },
                        [&](For) -> std::string { return "For"; },
                        [&](Print) -> std::string { return "Print"; },
                        [&](And) -> std::string { return "And"; },
                        [&](Or) -> std::string { return "Or"; },
                        [&](EOF) -> std::string { return "EOF"; },
                    },
                    token);
}

export template <>
struct std::formatter<token::Token> : std::formatter<std::string> {
  auto format(const token::Token& token, std::format_context& ctx) const {
    return std::formatter<std::string>::format(tokenToString(token), ctx);
  }
};
export std::ostream& operator<<(std::ostream& out, const token::Token& token) {
  out << tokenToString(token);
  return out;
}
