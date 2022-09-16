# Finite Element Analysis Demo

## Prerequesities

1. C++ compiler (tested on GCC 9.4)
2. CMake 3.10 and above
3. Eigen (3.4.0)

For Eigen installation run `download_eigen.sh` script.

## Build and run

Use `build.sh` script to build executable file. The executable file can be located in `build\Release|Debug\` directory.

Run `fem_demo`:
```shell
fem_demo <path_to_mesh> [poisson_ratio] [young_modulus]
```

Use `run_tests.sh` script to run unit-testing.

## Result processing

Use `scripts/postprocess.py` script to visualize results:

```
usage: postprocess.py [-h] [-m MESH] [-r RESULT] [-s STRESS] [--output_dir OUTPUT_DIR]

optional arguments:
  -h, --help            show this help message and exit
  -m MESH, --mesh MESH  Path to mesh file.
  -r RESULT, --result RESULT
                        Path to result file
  -s STRESS, --stress STRESS
                        Path to stress file
  --output_dir OUTPUT_DIR
                        Path to output directory
```

