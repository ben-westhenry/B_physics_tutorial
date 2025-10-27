#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif

#include "RooFit.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooFitResult.h"
#include "TCanvas.h"
#include "TLegend.h"

using namespace RooFit;
using namespace std;

//Plot the fitted distribution and the data with the break down of signal and background components
void plot_fit_to_dist(RooDataSet * Bmass_RooDataSet, string plot_name, RooRealVar mB, RooAddPdf BmPDF, RooGaussian sigPDF, RooExponential bkgPDF ){

    //Define plot name for the .pdf file format
    TString plot_pdf =  plot_name  ;
    TString plot_name_pdf_start =  plot_name + ".pdf[";
    TString plot_name_pdf =  plot_name + ".pdf";
    TString plot_name_pdf_end =  plot_name + ".pdf]";

    //Creat TCanvas to plot on
    auto c = new TCanvas("Canvas", "Canvas", 1500, 600);
    //To allow the plots to be save to .pdf
    c->SaveAs(plot_name_pdf_start);

    //Define a frame based on the mB range and tite on plot
    RooPlot * frame = mB.frame(Title("Fit to B^{#pm} distribution"));
    //Plot the B mass data
    Bmass_RooDataSet->plotOn(frame);
    //Plot the full fitted signal+background distribution from the fit
    BmPDF.plotOn(frame);
    //Plot the signal distribution from the fit in red
    BmPDF.plotOn(frame, Components(sigPDF), LineColor(kRed),Name("sig"));
    //Plot the background distribution from the fit in gray
    BmPDF.plotOn(frame, Components(bkgPDF), LineColor(kGray),Name("bkg"));
    //Draw the frame onto the canvas
    frame->Draw();

    //Define a legend for the plot (if editing size of legend 1st and 3rd arguments are x and 2nd and 4th components are y)
    TLegend *leg = new TLegend();
    //Set Fill, line colour and text size within legend
    leg->SetFillColor(kWhite);
    leg->SetLineColor(kWhite);
    leg->SetTextSize(0.035);
    //Add label of B mass signal decay to the signal distribution for the legend
    leg->AddEntry(frame->findObject("sig"),"B^{#pm} #rightarrow DK^{#pm}");
    //Add label for the combinatorial background distribution for the legend
    leg->AddEntry(frame->findObject("bkg"),"Comb.");
    //Draw the legend on the canvas
    leg->Draw();

    //To finish saving the plots to .pdf file format
    c->SaveAs(plot_name_pdf);
    c->SaveAs(plot_name_pdf_end);

}

//Perform the fit of signal+background probability distribution to the B mass data and call function to plot the result.
void B_mass_fit(){

    //Define name of pdf file the fit plots will be saved to
    string plot_name="fit_results/plots/Bmass_plot";

    //Read in the B mass data we have generated from .root file format
    TFile * Bmass_Data = new TFile("data/Combine_B_mass.root","read");
    TTree * Bmass_Data_Tree = (TTree*) Bmass_Data->Get("DecayTree");

    // --- Construct signal Probability Density Function ---

    //Define the B mass range to perform the fit over
    RooRealVar mB("B_mass","m_{B}", 5000,5800, "MeV/c^{2}");
    //Create RooDataset for the data over the specified B mass range
    RooDataSet *Bmass_RooDataSet = new RooDataSet("Bmass_RooDataSet","Bmass_RooDataSet",Bmass_Data_Tree,mB);
    //Initialise starting values and the range of values for mean of the Gaussian for the signal distribution in the fit
    RooRealVar mean("mean","mean of gaussians",5270,5240,5310) ;
    //Initialise starting values and the range of values for width of the Gaussian for the signal distribution in the fit
    RooRealVar sigma("sigma","width of gaussian",10,0,20) ;
    //Gaussian distribution for the signal distribution for the fit
    RooGaussian sigPDF("sig","Signal component",mB,mean,sigma) ;

    // --- Construct background Probability Density Function ---

    //Initialise starting values and the range of values for the exponential slope for the combinatorial background
    //distribution in the fit
    RooRealVar CombSlope("CombSlope", "CombMSlope", -0.00280, -.1, .0);
    ////Exponential distribution for the combinatorial background distribution for the fit
    RooExponential bkgPDF("combPdf", "combMPdf", mB, CombSlope);

    // --- Construct signal+background Probability Density Function ---

    //Initialise starting values and the range of values for the number of signal events in the fit
    RooRealVar nsig("nsig","#signal events",800,0.,20000) ;
    //Initialise starting values and the range of values for the number of background events in the fit
    RooRealVar nbkg("nbkg","#background events",200,0.,10000) ;
    //Gaussian+exponential distribution for fit
    RooAddPdf BmPDF("BmPDF","gaussian+combinatorial",RooArgList(sigPDF,bkgPDF),RooArgList(nsig,nbkg)) ;

    //Perform the fit of the full PDF distribution to the B mass data, errors calculated using MINOS
    RooFitResult* result = BmPDF.fitTo(*Bmass_RooDataSet,Minos(true));
    //Save the values determined by the fit to a .txt file
    RooArgSet* currentParams = BmPDF.getVariables();
    TString fileName = "fit_results/results/B_mass_fit_results.txt";
    currentParams->writeToFile(fileName);

    //call the plot function to plot the fit to the B mass distribution
    plot_fit_to_dist(Bmass_RooDataSet, plot_name , mB, BmPDF, sigPDF, bkgPDF);

}