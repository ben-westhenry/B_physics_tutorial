# Obtaining B meson signal events.

## Project Description

This project is for  new students coming on to the B to charm analysis for the first time and
aims to demonstrate how to establish the signal yield from a mass 
distribution using a simple simulated example. First the mass distribution is 
generated where a Gaussian distribution is generated for the B->DK signal events (with 
mean 5279.42 based on the PDG mass of the B meson) and an exponential distribution 
is generated for the random combinatorial background. A fit is performed to the 
distributions to obtain the signal and background yields and the probability density 
function parameters. In real LHCb analyses obtaining the number of signal events is 
often integral to performing particle physics measurements. This example is simplified 
-in this decay channel there would often be multiple more complicated backgrounds and
a simultaneous fit to both B->DK and B->DPi decay channels. Real data would 
also normally require processing such as physics informed selections to reduce the 
background events compared to signal events and an optimised ML classifier to reduce the 
combinatorial background. In this example the mass distribution is generated so the 
signal is reasonably clean compared to the combinatorial background.

## Environment

The python environment here is managed using 'conda', to create the environment:

```
conda env create -n <env_name> --file env.yaml
```

To activate this environment once created:

```
conda activate <env_name>
```

## Generating the mass distribution and performing the fit.

To run the project generating the mass distributions and performing the fit:

```
bash run_project.bash
```

This will generate the simulated mass distribution, perform the fit and then run 
the tests using pytest. 
- The data is generated in B_gen/B_mass_data_generation.py. 
- The fit is undertaken in B_fit/Bmass_fit.cpp.
- The data and fit are performed using physics_analysis.py.
- The plot of the fit and the results will be outputted to fit_results/.
- The tests are in test/test_physics_analysis.py