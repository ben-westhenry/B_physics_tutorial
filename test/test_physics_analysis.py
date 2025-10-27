import sys
sys.path.append('../')
from B_gen import B_mass_data_generation
import pandas as pd
import uproot
import numpy as np


def test_root_data_file_exists():
    """
    Checks .root file with the generated data exists.
    """
    testFilename = "../data/Combine_B_mass.root"
    assert open(testFilename, 'r')


def test_num_sig_data():
    """
    Check the expected number of signal events has been generated.
    """
    assert len(B_mass_data_generation.generate_B_signal_data()) == 10000

def test_num_B_comb_data():
    """
    Check the expected number of background events has been generated.
    """
    assert len(B_mass_data_generation.generate_combinatorial_data()) == 3000

def test_num_root_file():
    """
    Check the number of events saved to the .root file
    are the same as the number generated signal + background events.
    """
    root_tree="../data/Combine_B_mass.root:DecayTree"
    n_sig=len(B_mass_data_generation.generate_B_signal_data())
    n_bkg=len(B_mass_data_generation.generate_combinatorial_data())
    assert len(pd.DataFrame.from_dict(uproot.concatenate(root_tree, expressions=["B_mass"], library='np'))) == n_sig+n_bkg


def test_fit_results_file_exits():
    """
    Checks .txt file with the fit results exists.
    """
    testFilename = "../fit_results/results/B_mass_fit_results.txt"
    assert open(testFilename, 'r')

def test_fit_results():
    """
    Checks the fitted values align with generated data within reasonable limits.
    """
    testFilename="../fit_results/results/B_mass_fit_results.txt"
    with open(testFilename, 'r') as inFile:
        for line in inFile:
            print(len(line))
            if len(line) > 1:
                # Results in form mean =  5279.1 +/-  (-0.10195, 0.10196) L(5240 - 5310)
                # put results into a list
                lineList = line.replace("(-","").replace("(","").replace(")","").replace(",","").split()
                # Fit parameter name is the 1st item
                paramName = lineList[0]
                # We do not want to consider B mass as mB was just the range of the fit so was not a fit result
                if paramName != "B_mass":
                    # Fit parameter value is the 3rd item
                    fitValue = np.float64(lineList[2])
                    # Fit error lower bound value is the 5th item
                    errorValueLow = np.float64(lineList[4])
                    # Fit error higher bound value is the 6th item
                    errorValueHigh = np.float64(lineList[5])
                    # Check the fitted values for all our fitted parameters align with generated data within
                    # reasonable limits
                    if paramName == "nsig":
                        assert (fitValue - errorValueLow) < 10000 < (fitValue + errorValueHigh)
                    if paramName == "nbkg":
                        assert (fitValue - errorValueLow) < 3000 < (fitValue + errorValueHigh)
                    if paramName == "mean":
                        assert (fitValue - errorValueLow*2.0) < 5279.42 < (fitValue + errorValueHigh*2.0)
                    if paramName == "sigma":
                        assert (fitValue - errorValueLow*2.5) < 10 < (fitValue + errorValueHigh*2.5)
                    if paramName == "CombSlope":
                        assert (fitValue - errorValueLow*2.5) < -0.01 < (fitValue + errorValueHigh*2.5)

