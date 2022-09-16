# Finite Element Analysis Demo

## Prerequesities

1. C++ compiler (tested on GCC 9.4)
2. Python 3.7 and above (optional, for [result processing](#result_processing)
2. CMake 3.10 and above
3. Eigen (3.4.0)

For Eigen installation run `download_eigen.sh` script.

## Build and run

Use `build.sh` script to build executable file. The executable file can be located in `build\Release|Debug\` directory.

Run `fem_demo`:
```shell
fem_demo <path_to_mesh> [poisson_ratio] [young_modulus]
```
e.g.:
```
fem_demo data/mesh_coarse.k 0.3 2.e11
```
It will generate `resut.txt` with displacements and `stress.txt` with stresses.

Use `run_tests.sh` script to run unit-testing.

## Result processing

Install requirements:
```
pip install scripts/reqiremenst.txt
```

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

e.g.:
```
prostprocess.py -m data/mesh_coarse.k -r result.txt -s stress.txt --output_dir coarse_results
```
