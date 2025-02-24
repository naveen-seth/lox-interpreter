module;
#include <cassert>
export module Lexer;
import TokenTypes;
import std;

using namespace token;
export using LexedTokenList = token::LexedTokenList;

export class Lexer {
 public:
  explicit Lexer(const std::string &source) : m_source{source} {}
  explicit Lexer(std::string &&source) : m_source{std::move(source)} {}
  [[nodiscard]] LexedTokenList ScanTokens();

 private:
  std::string m_source;
  LexedTokenList m_lexedTokenList;

  std::uint32_t m_last{0};
  std::uint32_t m_current{0};
  std::uint32_t m_line{1};
  std::uint32_t m_thisLineStart{0};

  void scanToken();
  void addLexedToken(Token &&token);
  [[nodiscard]] bool isAtEnd();
  void incrementLine();
  char advance();
  [[nodiscard]] bool advanceIfMatch(char expected);
  [[nodiscard]] char peek();
  [[nodiscard]] char peekNext();

  void handleComment();
  void handleString();
  void handleNumber();
  void handleIdentifierOrKeyword();
};

[[nodiscard]] bool isDigit(char ch) { return (ch >= '0' && ch <= '9'); }

[[nodiscard]] bool isAlpha(char ch) {
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

[[nodiscard]] bool isAlphaNumeric(char ch) {
  return isAlpha(ch) || isDigit(ch);
}

[[nodiscard]] LexedTokenList Lexer::ScanTokens() {
  while (!isAtEnd()) {
    m_last = m_current;
    scanToken();
  }

  addLexedToken(EOF{});
  return m_lexedTokenList;
}

void Lexer::addLexedToken(Token &&token) {
  m_lexedTokenList.push_back(std::move(token), m_line,
                             m_current - m_thisLineStart);
}

[[nodiscard]] bool Lexer::isAtEnd() { return m_current == m_source.size(); }

void Lexer::incrementLine() {
  ++m_line;
  m_thisLineStart = m_current;
}

char Lexer::advance() {
  m_current++;
  return m_source[m_current - 1];
};

[[nodiscard]] bool Lexer::advanceIfMatch(char expected) {
  if (isAtEnd()) {
    return false;
  }
  if (m_source[m_current] != expected) {
    return false;
  }
  ++m_current;
  return true;
}

[[nodiscard]] char Lexer::peek() {
  if (isAtEnd()) {
    return '\0';
  }
  return m_source[m_current];
}

[[nodiscard]] char Lexer::peekNext() {
  if (m_current + 1 >= m_source.size()) {
    return '\0';
  }
  return m_source[m_current + 1];
};

void Lexer::handleComment() {
  while (peek() != '\n' && !isAtEnd()) {
    advance();
  }
}

void Lexer::handleString() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n') {
      incrementLine();
    }
    advance();
  }

  if (isAtEnd()) {
    std::print("Error at line {}, column {}: Unterminated String", m_line,
               m_current - m_thisLineStart);
    return;
  }

  // Consume left over '"'
  advance();
  auto val = m_source.substr(m_last + 1, m_current - m_last - 2);
  addLexedToken(String{val});
}

void Lexer::handleNumber() {
  while (isDigit(peek())) {
    advance();
  }

  // Look for a fractional part
  if (peek() == '.' && isDigit(peekNext())) {
    advance();
    while (isDigit(peek())) {
      advance();
    }
  }

  // Parse to double
  double val = 0;
  std::from_chars(std::next(m_source.data(), m_last),
                  std::next(m_source.data(), m_current), val);
  addLexedToken(Number{val});
}

void Lexer::handleIdentifierOrKeyword() {
  while (isAlphaNumeric(peek())) {
    advance();
  }

  auto lexeme = m_source.substr(m_last, m_current - m_last);

  if (const auto it = keywordTokenMap.find(lexeme);
      it != keywordTokenMap.end()) {
    addLexedToken(Token{it->second});
  } else {
    addLexedToken(Identifier{lexeme});
  }

  m_last = m_current;
}

void Lexer::scanToken() {
  const auto ch = advance();
  switch (ch) {
    case '(':
      addLexedToken(LeftParen{});
      break;
    case ')':
      addLexedToken(RightParen{});
      break;
    case '{':
      addLexedToken(LeftBrace{});
      break;
    case '}':
      addLexedToken(RightBrace{});
      break;
    case ',':
      addLexedToken(Comma{});
      break;
    case '.':
      addLexedToken(Period{});
      break;
    case ';':
      addLexedToken(Semicolon{});
      break;
    case '-':
      addLexedToken(Minus{});
      break;
    case '+':
      addLexedToken(Plus{});
      break;
    case '*':
      addLexedToken(Star{});
      break;
    case '!':
      addLexedToken(advanceIfMatch('=') ? Token{BangEqual{}} : Token{Bang{}});
      break;
    case '=':
      addLexedToken(advanceIfMatch('=') ? Token{EqualEqual{}} : Token{Equal{}});
      break;
    case '<':
      addLexedToken(advanceIfMatch('=') ? Token{LessEqual{}} : Token{Less{}});
      break;
    case '>':
      addLexedToken(advanceIfMatch('=') ? Token{GreaterEqual{}}
                                        : Token{Greater{}});
      break;
    case '/':
      if (peek() == '/') {
        handleComment();
        break;
      } else {
        addLexedToken(Slash{});
        break;
      }
    case '"':
      handleString();
      break;
      // Ignore whitspace
    case ' ':
    case '\r':
    case '\t':
      break;
    case '\n':
      incrementLine();
      break;

    default:
      if (isDigit(ch)) {
        handleNumber();
      } else if (isAlpha(ch)) {
        handleIdentifierOrKeyword();
      } else {
        std::print("Error at line {}, column {}: Unrecognized token '{}'",
                   m_line, m_current - m_thisLineStart, ch);
      }
  };
}
