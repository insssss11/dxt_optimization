#include <vector>
#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPaveStats.h"
int main(void)
{
    const std::string dir = "./output/hardening/";
    const std::string prefix = "tube";
    const std::vector<std::string> tube_voltages{"080kVp", "100kVp", "120kVp", "140kVp", "160kVp", "180kVp", "200kVp"};

    auto canvas = new TCanvas("", "");
    char buffer[256];
    for(int i = 0;i < tube_voltages.size();i++)
    {
        canvas->Clear();

        auto filename = dir + prefix + tube_voltages[i] + ".root";
        auto tfile = TFile::Open(filename.c_str(), "READ");

        auto tree1 = static_cast<TTree*>(tfile->Get("led001"));
        auto tree2 = static_cast<TTree*>(tfile->Get("filter"));
        auto tree3 = static_cast<TTree*>(tfile->Get("hed001"));
        tree1->Draw("eParts[0]>>hist1(400., 0., 200.)", "prmary == 1");
        tree2->Draw("eParts[0]>>hist2(400., 0., 200.)", "prmary == 1");
        tree3->Draw("eParts[0]>>hist3(400., 0., 200.)", "prmary == 1");

        auto hist1 = static_cast<TH1D*>(gDirectory->Get("hist1"));
        auto hist2 = static_cast<TH1D*>(gDirectory->Get("hist2"));
        auto hist3 = static_cast<TH1D*>(gDirectory->Get("hist3"));
        assert(hist1 != nullptr && hist2 != nullptr && hist3 != nullptr);
        auto title = tube_voltages[i] + ";Energy (keV);Count (cnt/keV)";
        hist1->SetTitle(title.c_str());
        hist1->SetNameTitle("From the tube", title.c_str());
        hist1->GetYaxis()->SetRangeUser(0, 10000.);
    
        hist2->SetLineColor(kBlue);
        hist2->SetNameTitle("150 um GOS", title.c_str());
        
        hist3->SetLineColor(kRed);
        hist3->SetNameTitle("150 um GOS + 0.6 mm Cu", title.c_str());
        
        hist1->SetStats(true);
        hist2->SetStats(true);        
        hist3->SetStats(true);        

        hist1->Draw();
        hist2->Draw("SAMES");
        hist3->Draw("SAMES");
        canvas->Update();

        TPaveStats *stats1 = (TPaveStats*)hist1->FindObject("stats");
        TPaveStats *stats2 = (TPaveStats*)hist2->FindObject("stats");
        TPaveStats *stats3 = (TPaveStats*)hist3->FindObject("stats");

        stats2->SetX1NDC(stats1->GetX1NDC());
        stats2->SetX2NDC(stats1->GetX2NDC());
        stats2->SetY1NDC(2*stats1->GetY1NDC() - stats1->GetY2NDC());
        stats2->SetY2NDC(stats1->GetY1NDC());
        stats2->SetTextColor(kBlue);
        stats2->Draw();
    
        stats3->SetX1NDC(stats2->GetX1NDC());
        stats3->SetX2NDC(stats2->GetX2NDC());
        stats3->SetY1NDC(2*stats2->GetY1NDC() - stats2->GetY2NDC());
        stats3->SetY2NDC(stats2->GetY1NDC());
        stats3->SetTextColor(kRed);
        stats3->Draw();

        auto outfile = dir + prefix + tube_voltages[i] + ".png";
        canvas->Modified();
        canvas->SaveAs(outfile.c_str());
    }
    delete canvas;
    return 0;
}