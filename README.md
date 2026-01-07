# els - Extended ls

`els` (extended ls) is a small C program that implements an own version of the Linux command `ls`.
The goal of this project is to recreate basic functionality of the standard `ls` tool and extend it
with additional features and options.

The project was developed in the context of the System-oriented Programming course and focuses on
practical aspects of C programming, such as filesystem access, command-line arguments, and modular
program structure.

## Build and Usage

### Build

To build the program, run:
```shell
make
```

### Run

To run the program without arguments:
```shell
make run
```

To run the program with arguments:
```shell
make run ARGS="<args>"
```

After building, you can also directly run the binary:
```shell
./build/els [OPTIONS] [PATH]
```

### Test

To build and run the tests:
```shell
make test
```

### Clean

To remove all build artifacts:
```shell
make clean
```