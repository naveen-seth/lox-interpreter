import std;
import Interpreter;

static std::string binary_name;

void runPrompt();
void run(const std::filesystem::path &path);

int main(int argc, char *argv[]) {
  binary_name = argv[0];
  --argc;

  std::println(
      "Notice: Currently fully implemented lexer prototype AST printer");
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

void runPrompt() {
  // Placeholder
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

void run(const std::filesystem::path &path) {
  // Current entrypoint for testing
  Lox::runFile(path);
}
