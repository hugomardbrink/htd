# Personal library for C

## Installation
This library can easily be installed using CMake and FetchContent.
Add this to your CMakeLists.txt file:

```cmake
include(FetchContent)

FetchContent_Declare(
  htd
  GIT_REPOSITORY https://github.com/hugomardbrink/htd.git
  GIT_TAG main 
)

FetchContent_MakeAvailable(htd)

add_executable(myapp main.c)
target_link_libraries(myapp PRIVATE htd::htd)
```

## Features
### Primitives
Use shorter more concise names for common C types.
- `i8`: 8-bit signed integer
- `u8`: 8-bit unsigned integer
- `i16`: 16-bit signed integer
- `u16`: 16-bit unsigned integer
- `i32`: 32-bit signed integer
- `u32`: 32-bit unsigned integer
- `i64`: 64-bit signed integer
- `u64`: 64-bit unsigned integer
- `f32`: 32-bit floating point
- `f64`: 64-bit floating point
- `usize`: Unsigned integer of the same size as a pointer
- `isize`: Signed integer of the same size as a pointer

### Data structures
- `Dynamic array`: A dynamic array that can grow and shrink in size.
- `Hash map`: A hash map that uses murmur3, open addressing (double hashing) and tombstone deletion.
- `Priority queue`: A priority queue that uses a binary heap, custom comparator.

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Testing

```bash
cd build
ctest
```

## Local install

```bash
cd build
make install
```

## Local uninstall

```bash
cd build
make uninstall
```

