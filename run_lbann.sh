#!/bin/bash

# Activate the spack environment
spack env activate lbann
# Activate the mamba environment
conda activate lbann_bench_3.9

# Add to PYTHONPATH
export PYTHONPATH="/p/gpfs1/namankul/spack/opt/spack/linux-rhel7-power9le/gcc-8.3.1/lbann-develop-kihqzaq6yk2kh7plhjgki74a7ytmr2pf/lib/python3.9/site-packages:$PYTHONPATH"

