# Segmented Vector

A simple implementation of a segmented vector, a data structure that behaves like a normal `std::vector` with the difference that it never reallocates.
This allows to take a pointer to any element that will stay valid for the whole lifetime of the vector.

I made this just for fun and there's definitely room for improvements and optimizations.

Build with:

```bash
cmake -S . -B build
cmake --build build
```
