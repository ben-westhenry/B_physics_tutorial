import sys
sys.path.append('../')
from B_gen import B_mass_data_generation
import os


def main():
    """
    Generates the Bmass data distribution and writes to .root file using B_gen.
    Then C++ script uses RooFit to fit probability density functions for the B mass distribution to establish
    PDF shapes and signal and bkg yields and creates plots in PDF format and writes the results to a .txt file.
    """
    gaussian_width = 10
    num_signal_events = 10000
    exponential_scale = 100
    num_background_events = 3000
    # Generate B mass distribution.
    B_mass_data_generation.generate_data(gaussian_width, num_signal_events, exponential_scale, num_background_events)
    # Fit to B mass distribution.
    os.system("root -q B_fit/B_mass_fit.cpp")

if "__main__" == __name__:
    main()