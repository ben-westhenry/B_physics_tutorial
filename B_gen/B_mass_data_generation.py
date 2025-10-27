import uproot
import numpy as np
import pandas as pd

def generate_B_signal_data(width=10, num_events=10000):
    """
    Generate gaussian B mass signal distribution, B mass value in PDG 2025 is 5279.41
    """
    Bmass_mean=5279.42
    return np.random.normal(Bmass_mean,width, num_events)

def generate_combinatorial_data(scale=100, num_events=3000):
    """
    Generate combinatorial bkg in the B mass distribution
    """
    return  5000.00 + np.random.exponential(scale, num_events)


def save_data(data, data_name ):
    Bmass_pandaFrame = pd.DataFrame.from_dict({"B_mass": data})
    """
    Save generated distributions to .root file, a compressed data file format particularly useful when
    handling extremely large data files
    """
    with uproot.recreate("data/" + data_name +".root") as outfile:
        outfile["DecayTree"] = Bmass_pandaFrame


def generate_data(gaussian_width, num_signal_events, exponential_scale, num_background_events):
    """
    Wraps previous functions to generate the B mass signal and background data and then combine into
    one distribution and write to a .root file
    """
    sig_data = generate_B_signal_data(gaussian_width, num_signal_events)
    comb_data = generate_combinatorial_data(exponential_scale,  num_background_events)
    sig_plus_comb_data = np.concatenate((sig_data, comb_data))
    save_data(sig_plus_comb_data, "Combine_B_mass")
