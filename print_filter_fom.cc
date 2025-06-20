#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "TFile.h"
#include "TTreeReader.h"
#include "TCanvas.h"
#include "TPaveStats.h"

using namespace std;

const string centerStr(int width, const string&);
void printUsage();
void printBars();
void printHeader();
double calFOM();

double
    ePart_avg_l1{0.}, ePart_avg_f{0.}, ePart_avg_h1{0.},
    eDep_avg_l1{0.}, eDep_avg_f{0.}, eDep_avg_h1{0.},
    eDep_avg_eff_l1{0.}, eDep_avg_eff_f{0.}, eDep_avg_eff_h1{0.};
        
unsigned long long
    np_l1{0}, np_f{0}, np_h1{0},
    np_eff_l1{0}, np_eff_f{0}, np_eff_h1{0};

int main(int argc, char **argv)
{
    if(argc != 5)
    {
        printUsage();
        return -1;
    }
    const string path = argv[1];
    const int t_start = stoi(argv[2]), t_end = stoi(argv[3]), t_step = stoi(argv[4]);

    vector<int> thickness{};
    vector<double> foms{};
    double t = t_start;
    do{
        thickness.push_back(t);
        t += t_step;
    }while(t <= t_end);

    char buffer[256];
    printBars();
    printHeader();
    printBars();
    for(const auto &t : thickness)
    {
        auto fullpath = path + "_" + to_string(t) + "umCu.root";
        auto tfile = TFile::Open(fullpath.c_str(), "READ");
        TTreeReader tr_l1("led001", tfile), tr_f("filter", tfile), tr_h1("hed001", tfile);

        TTreeReaderValue<double> eDepSum_l1(tr_l1, "eDepSum"), eDepSum__f(tr_f, "eDepSum"), eeDepSum__h1(tr_h1, "eDepSum");
        TTreeReaderValue<int> prmary_l1(tr_l1, "prmary"), prmary_f(tr_f, "prmary"), prmary_h1(tr_h1, "prmary");
        TTreeReaderValue<int> nparts_l1(tr_l1, "nParts"), nparts_f(tr_f, "nParts"), nparts_h1(tr_h1, "nParts");
        TTreeReaderValue<vector<double> > eParts_l1(tr_l1, "eParts"), eParts_f(tr_f, "eParts"), eParts_h1(tr_h1, "eParts");
        TTreeReaderValue<vector<double> > eDeps_l1(tr_l1, "eDeps"), eDeps_f(tr_f, "eDeps"), eDeps_h1(tr_h1, "eDeps");
        TTreeReaderValue<vector<int> > partIDs_l1(tr_l1, "partEncs"), partIDs_f(tr_f, "partEncs"), partIDs_h1(tr_h1, "partEncs");

        assert(tr_l1.GetEntries() == tr_f.GetEntries() && tr_f.GetEntries() == tr_h1.GetEntries());
        ePart_avg_l1 = 0., ePart_avg_f = 0., ePart_avg_h1 = 0.,
        eDep_avg_l1 = 0., eDep_avg_f = 0., eDep_avg_h1 = 0.,
        eDep_avg_eff_l1 = 0., eDep_avg_eff_f = 0., eDep_avg_eff_h1 = 0.;
            
        np_l1 = 0, np_f = 0, np_h1 = 0,
        np_eff_l1 = 0, np_eff_f = 0, np_eff_h1 = 0;  
        for(Long64_t i = 0;i < tr_l1.GetEntries();i++)
        {
            tr_l1.Next(), tr_f.Next(), tr_h1.Next();         
            
            if(*prmary_l1 == 1)
            {
                ePart_avg_l1 += eParts_l1->front();
                eDep_avg_l1 += eDeps_l1->front();
                np_l1++;
                if(eDeps_l1->front() > 10.0)
                {
                    eDep_avg_eff_l1 += eDeps_l1->front();
                    np_eff_l1++;
                }
            }
            if(*prmary_f == 1)
            {
                ePart_avg_f += eParts_f->front();
                eDep_avg_f += eDeps_f->front();
                np_f++;
                if(eDeps_f->front() > 10.0)
                {
                    np_eff_f++;
                    eDep_avg_eff_f += eDeps_f->front();
                }
            }
            if(*prmary_h1 == 1)
            {
                ePart_avg_h1 += eParts_h1->front();
                eDep_avg_h1 += eDeps_h1->front();
                np_h1++;
                if(eDeps_h1->front() > 10.0)
                {
                    np_eff_h1++;
                    eDep_avg_eff_h1 += eDeps_h1->front();
                }
            }
        }
        ePart_avg_l1 = ePart_avg_l1/np_l1, ePart_avg_f = np_f, ePart_avg_h1 = ePart_avg_h1/np_h1;
        eDep_avg_l1 = eDep_avg_l1/np_l1, eDep_avg_f = eDep_avg_f/np_f, eDep_avg_h1 = eDep_avg_h1/np_h1;
        eDep_avg_eff_l1 = eDep_avg_eff_l1/np_eff_l1, eDep_avg_eff_f = eDep_avg_eff_f/np_eff_f, eDep_avg_eff_h1 = eDep_avg_eff_h1/np_eff_h1;
        foms.push_back((eDep_avg_h1 - eDep_avg_l1)*(eDep_avg_h1 - eDep_avg_l1)*np_eff_h1);

        cout
            << setw(13) << t << '|'
            << setw(9) << ePart_avg_l1 << '|'
            << setw(9) << ePart_avg_h1 << '|'

            << setw(9) << eDep_avg_l1 << '|'
            << setw(9) << eDep_avg_eff_h1 << '|'
            << setw(9) << eDep_avg_h1 << '|'
            << setw(9) << eDep_avg_eff_h1 << '|'         

            << setw(10) << np_l1 << '|'
            << setw(10) << np_eff_l1 << '|'
            << setw(10) << np_h1 << '|'
            << setw(10) << np_eff_h1 << '|'
            << setw(15) << foms.back() << '|' << endl;

        tfile->Close();
    }
    printBars();
    return 0;
}

const string centerStr(int width, const string &s)
{
    width = width > s.size()?width - s.size():0;
    int left, right;
    if(width%2 == 1)
        left = width/2 + 1, right = width/2;
    else
        left = width/2, right = width/2;
    return string(left, ' ') + s + string(right, ' ');
}

void printUsage()
{
    cout << "Usage : ./print_filter_fom [data_subname] [t_start] [t_end] [t_step]" << endl;
    cout << "For example, the names of data files are " << endl;
    cout << "tube160kVp_150umLED_3mmHED_0umCu.root ~ tube160kVp_1500umLED_3mmHED_1500umCu.root in 100 um steps" << endl;
    cout << ">> ./print_filter_fom tube160kVp_150umLED_3mmHED 0 1500 100" << endl;
}

void printBars()
{
    cout << string(150, '-') << endl;
}

void printHeader()
{
    cout << centerStr(13, " ")
    << '|' << centerStr(19, "E_mean [keV]")
    << '|' << centerStr(39, "E_dep_mean [keV] (eff > 10 keV)")
    << '|' << centerStr(43, "Nph [#] (eff > 10 keV)") << '|' << centerStr(15, "FOM") << endl;
    cout << centerStr(13, "filt. [umCu]")
    << '|' << centerStr(9, "LED") << '|' << centerStr(9, "HED")
    << '|' << centerStr(9, "LED") << '|' << centerStr(9, "LED_eff")  << '|' << centerStr(9, "HED") << '|' << centerStr(9, "HED_eff")
    << '|' << centerStr(10, "LED") << '|' << centerStr(10, "LED_eff")  << '|' << centerStr(10, "HED") << '|' << centerStr(10, "HED_eff") << '|' << endl;
}

// FORM factor estimation
double calFOM()
{
    return (eDep_avg_h1 - eDep_avg_l1)*(eDep_avg_h1 - eDep_avg_l1)*np_eff_h1;
}