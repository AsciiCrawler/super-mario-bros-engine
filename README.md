#### Super Mario Bros Engine

##### Requirements 
- CMake 3.23 or newer
- A C++14 compiler (GCC or Clang) on Linux
- SDL3 development package (e.g. `sudo dnf install SDL3-devel`)
- A GPU/driver supporting OpenGL 3.3 core
- GNU Make (optional, for the shortcuts below)

##### Build
```sh
make
```

##### Run
```sh
make run
```
`make run` builds first, then starts the game from inside `build/`, because assets and shaders are loaded with relative paths. Use `make clean` to delete `build/`.

`assets/` is copied into `build/` when CMake configures. `make` and `make run` re-run CMake every time, so asset changes are picked up automatically.

Without Make, run the same commands directly; re-run the first one after changing anything in `assets/`:
```sh
cmake -S . -B build
cmake --build build
cd build && ./main
```

##### Controls
| Key | Action |
|---|---|
| A / D | Move left / right |
| W / S | Aim up / down (S also falls faster) |
| Shift (held) | Run |
| Space | Jump (hold for a higher jump) |
| K | Dash in the held direction |
| Esc | Quit |
