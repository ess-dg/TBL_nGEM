#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TRandom.h"
#include "stdio.h"
#include "stdlib.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>

const double pi = 3.14159265; 
const double mn = 1.674927471e-27;  // in kg
const double hp = 6.62607004e-34;   // in kg * m2/s
const double m2a = 1e+10;    // unit to convert m in A

using namespace std;

//Usage: .L CreateGEMTree.C /CreateGEMRootFile() 

void CreateGEMRootFile()
{
     TTree* t=new TTree("ngem_ev","ngem_ev");
     
     ULong64_t X0,X1,Y0,Y1,clusTX,clusTY,tof,t0,pixelX,pixelY;
     double lambda;
     
     // see below the description of the variables or the read_nGEM_data.h header file 
     
     t->Branch("X0",&X0,"X0/l");    // X position of pixel detected first
     t->Branch("X1",&X1,"X1/l");    // X position of pixel detected last
     t->Branch("Y0",&Y0,"Y0/l");    // Y position of pixel detected first
     t->Branch("Y1",&Y1,"Y1/l");    // Y position of pixel detected last
     t->Branch("clusTX",&clusTX,"clusTX/l");    // Integrated time of the cluster on the X side
     t->Branch("clusTY",&clusTY,"clusTY/l");     // Integrated time of the cluster on the Y side

     t->Branch("tof",&tof,"tof/l");     // Difference between T0 and detector timestamp
     t->Branch("t0",&t0,"t0/l");        // T0 time, looks like a multiple of 3810 ns
     t->Branch("pixelX",&pixelX,"pixelX/l");     // average pixel for cluster detection, X side
     t->Branch("pixelY",&pixelY,"pixelY/l");    // average pixel for cluster detection, Y side
     t->Branch("lambda",&lambda,"lambda/D");    // lambda value
     
     gSystem->Exec("rm ngem.root");
     TFile *rf=new TFile("ngem.root","new");

	vector<string> row;
	string line,word;

	fstream fin;

     fin.open("data_for_root.txt");  
     
     cout<<"    "<<endl;
     
     cout<<"Reading the data file..."<<endl;
     
      while (!fin.eof())
       {
        
        row.clear();
        
        // read an entire row and store it in a string variable 'line'
        
        getline(fin, line);

       // used for breaking words
        
        stringstream s(line);
        
        // read every column data of a row and store it in a string variable, 'word'
        
        while (getline(s, word, '\t')){
        
        // add all the column data of a row to a vector
       
        	row.push_back(word);
        
		//      cout<<"word ="<<word<<endl;    
        
        	}

	    t0=stol(row[0]);
	    tof=stol(row[1]);
	    clusTX=stol(row[2]);
	    clusTY=stol(row[3]);
	    X0=stol(row[4]);
	    X1=stol(row[5]);
	    Y0=stol(row[6]);
	    Y1=stol(row[7]);
	    pixelX=stol(row[8]);
	    pixelY=stol(row[9]);
	    
        lambda = hp*tof/1e+6/mn/41500*m2a;  // WISH

	  //	  cout<<"tof ="<<tof<<"posx ="<<posx<<endl;
		t->Fill();

	
	}
	
    fin.close();
	t->Write();
	
	cout<<"Fill data tree...done"<<endl; 
	cout<<"The data tree has "<<t->GetEntries()<<" entries."<<endl; 
	cout<<"  "<<endl;
		
	
	rf->Close();

// ngem_ev->Draw("tof*6.62607004e-34/1e+6/1.674927471e-27/41500*1e+10")

}
	
