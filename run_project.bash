#!/bin/bash

# The directories for the results aren't created automatically using Root so are created here.
if [ ! -d "fit_results" ]; then
  mkdir fit_results
  mkdir fit_results/plots
  mkdir fit_results/results
fi
# Run the full physics anaylsis.
python3 physics_analysis.py
# Run the tests.
cd test
pytest


