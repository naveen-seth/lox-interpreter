# Lox

Implementation of the Lox language from Crafting Interpreters by Robert Nystrom in modern C++.

Status:

- Lexer: Fully implemented
- AST traversal/printer: prototype
- Parser/Evaluator: TODO

# How to run

```bash
podman build -t lox --target release  .
podman run --rm -it localhost/lox:latest 
```

# Manual build

```bash
cmake -S . --preset <preset>
cmake --build --preset <preset> --target --install

# Run
./out/<preset>/bin/main
```