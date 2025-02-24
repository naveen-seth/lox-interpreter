import std;
import Interpreter;

static std::string binary_name;

auto runPrompt() -> void;
auto run(const std::filesystem::path &path) -> void;

auto main(int argc, char *argv[]) -> int {
  binary_name = argv[0];
  --argc;

  std::println("Notice: Currently fully implemented lexer prototype AST printer");
  std::println("Testing entrypoint: {} <filename>", binary_name);
  
  if (argc > 1) {
    std::print("Usage: {} [options]\n", binary_name);
    return 1;
  } else if (argc == 1) {
    run(argv[1]);
  } else if (argc == 0) {
    runPrompt();
  }

  return 0;
}

auto runPrompt() -> void {
  while (true) {
    std::print(">>> ");
    std::string input;
    std::getline(std::cin, input);
    if (input == "exit") {
      break;
    }
    std::print("{}\n", input);
  }
}

auto run(const std::filesystem::path &path) -> void {
  Lox::runFile(path);
  // Lexer scanner{source};
  // auto tokens = scanner.scanTokens();
  // std::println("Nr. tokens: {}", tokens.size());
  // for (std::size_t i{0}; i < tokens.size(); ++i) {
  //   std::cout << tokens[i] << " ";
  // }
  // std::cout << '\n';
}
