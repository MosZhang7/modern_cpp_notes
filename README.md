# Modern C++ Notes (C++11–C++2x)

## 0. Introduction
- Goals of Modern C++
- Major changes from C++11, C++14, C++17, C++20
- How to read the standard and cppreference
- How to maintain this document for future standards

---

# Part I · Modernized Language Foundations

## 1. Value Categories & Object Model
- lvalue / xvalue / prvalue
- Temporary lifetime extension
- Reference collapsing rules
- std::move and std::forward
- decltype rules
- (Future extensions: if upcoming standards adjust value category rules)

## 2. RAII and Resource Management
- Constructors, destructors, move semantics
- default / delete functions
- noexcept and its role in move operations
- Smart pointers: unique_ptr, shared_ptr, weak_ptr
- enable_shared_from_this & weak_from_this
- Custom deleters and get_deleter()
- Ownership guidelines and common pitfalls

## 3. Memory Model Basics
- Stack vs heap vs static storage
- Alignment
- Type layout, triviality, POD vs non-POD
- Object lifetime rules

---

# Part II · C++20 Modules

## 4. C++20 Modules
- Motivation: replacing headers, improving build times
- `module`, `export module`, `import`, partitions
- Public vs private module partitions
- Header units
- How modules change template compilation
- Organizing a project with modules
- Migrating a CMake project to modules
- Compiler support (GCC/Clang/MSVC)
- (Future extensions: changes from C++23/26/29)

---

# Part III · Templates & Generic Programming

## 5. Template Fundamentals
- Template argument deduction
- Function templates vs class templates
- Specialization and partial specialization
- Name lookup (ADL, two-phase lookup)
- Variadic templates
- Type traits and metafunctions

## 6. Compile-Time Programming
- constexpr, consteval, constinit
- Fold expressions
- Type lists and basic template metaprogramming utilities
- constexpr containers and algorithms
- Immediate functions (consteval)

## 7. Concepts & Constraints (C++20)
- Why Concepts exist
- Writing concepts using `requires`
- Constrained templates and overload resolution
- Common standard concepts (integral, regular, invocable, etc.)
- Designing APIs with concepts
- (Future extensions: any new concepts introduced later)

---

# Part IV · Expressive Language Features

## 8. Lambdas & Closures
- Capture modes: =, &, this, *this
- init-capture
- Generic lambdas
- Lambdas as template parameters
- Lifetime and pitfalls

## 9. Error Handling
- noexcept and exception guarantees
- When not to use exceptions
- Designing error-return types
- std::expected (C++23)
- Assertions, contracts (future standard proposals)

## 10. Standard Library Containers
- Vector/string growth behavior
- List/map/unordered_map internals
- Emplacement semantics
- Allocator and polymorphic_allocator (pmr)
- Container performance characteristics
- (Future extensions from C++23/26)

---

# Part V · Concurrency, Multithreading & Coroutines

## 11. Concurrency Basics
- std::thread, parameter passing, std::ref
- atomics and memory order
- mutex, lock_guard, unique_lock
- condition_variable
- latch, barrier
- Concurrency design patterns

## 12. Coroutines (C++20)
- Coroutine state machine model
- co_await, co_yield, co_return
- promise_type explained
- Awaitable and Awaiter rules
- Building task, generator, async models
- Using coroutines in networking and database frameworks
- (Future: Executors, networking TS, coroutine extensions)

---

# Part VI · API & Library Design Practices

## 13. API Design Principles
- const-correctness
- Designing with string_view, span, and other non-owning views
- strong typedefs
- Implicit conversions vs explicit
- ABI stability and pImpl
- Header/module layout guidelines

---

# Part VII · I/O, Filesystem, and System Interfaces

## 14. Filesystem and OS Interaction
- std::filesystem
- File streams and performance considerations
- chrono and time APIs

## 15. Networking & Socket Basics
- POSIX sockets overview
- Blocking vs non-blocking
- Epoll/select/kqueue vs async I/O
- (Future: C++ standard networking library when it arrives)

---

# Part VIII · Tooling, Build Systems, and Engineering

## 16. Build Systems & Tooling
- CMake best practices
- GCC/Clang/MSVC differences
- Sanitizers: asan, msan, tsan, ubsan
- Static analyzers
- perf / valgrind

## 17. Testing & Quality Assurance
- GoogleTest basics
- Property-based testing
- Benchmarks and microbenchmarking

---

# Part IX · Practical Patterns & Case Studies

## 18. Modern C++ Patterns
- Type erasure (std::function, std::any, custom)
- CRTP patterns
- Optional/variant best practices
- Visitor patterns in Modern C++
- Small object optimization (SOO)

## 19. Case Studies
- Designing a lightweight RPC framework  
- Designing a database connectivity layer (your picodbc)
- Coroutine-based connection management
- Real-world performance tuning stories

---

# Part X · Future Standards & Extensions

## 20. Tracking New Standards (C++23/26/29+)
- Summary of what changed in newer standards
- Ranges extensions (C++20/23)
- Reflection proposals
- Pattern matching proposals
- Executor model
- Networking TS adoption
- Notes for upgrading this document

---

## Appendix
- Cheat sheets (value categories, type deduction rules, template rules)
- Glossary of important Modern C++ terms
