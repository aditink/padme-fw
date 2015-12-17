#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <CLHEP/Units/SystemOfUnits.h>

#include "HistoManager.hh"
#include "G4UnitsTable.hh"
#include "Constants.hh"
#include "MyEvent.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
 :rootFile(0),ntupl(0)
{
  // histograms
  for (G4int k=0; k<MaxHisto; k++){ 
    histo[k] = 0;    
    histo2[k] = 0;    
  }
  // ntuple
  ntupl = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  if ( rootFile ) delete rootFile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::book()
{ 
  // Creating a tree container to handle histograms and ntuples.
  // This tree is associated to an output file.
  G4String fileName = "UBTF.root";
  rootFile = new TFile(fileName,"RECREATE");
  if(!rootFile) {
    G4cout << " HistoManager::book :"<<" problem creating the ROOT TFile "<< G4endl;
    return;
  }
  
  histo[1] = new TH1D("h1", "EtotCrys ", 900, 0., 900*CLHEP::MeV);
  if (!histo[1]) G4cout << "\n can't create histo 1" << G4endl;
  histo[2] = new TH1D("h2", "EBeam ", 900, 0., 900*CLHEP::MeV);
  if (!histo[2]) G4cout << "\n can't create histo 2" << G4endl;
  histo[3] = new TH1D("h3", "EtotTracker", 900, 0., 900.*CLHEP::MeV);
  if (!histo[3]) G4cout << "\n can't create histo 3" << G4endl;
  histo[4] = new TH1D("h4", "Ncells per e- on target E>0.1MeV", 151,-0.5,150.5);
  if (!histo[4]) G4cout << "\n can't create histo 4" << G4endl;  
  histo[5] = new TH1D("h5", "Ncells per cluster E>0.1MeV", 151,-0.5,150.5);
  if (!histo[5]) G4cout << "\n can't create histo 5" << G4endl;  
  histo[6] = new TH1D("h6", "EClus", 900,0.,900.);
  if (!histo[6]) G4cout << "\n can't create histo 6" << G4endl;  
  histo[7] = new TH1D("h7", "Seed cell",ECalNCells,0.,ECalNCells);
  if (!histo[7]) G4cout << "\n can't create histo 7" << G4endl;  
  histo[8] = new TH1D("h8", "Theta", 200,0.,10.);
  if (!histo[8]) G4cout << "\n can't create histo 8" << G4endl;  
  histo[9] = new TH1D("h9", "MMiss2", 475,-50.,900.);
  if (!histo[9]) G4cout << "\n can't create histo 9" << G4endl;  
  histo[10] = new TH1D("h10", "MMiss2 50MeV<ECl<400MeV Rad>3", 475,-50.,900.);
  if (!histo[10]) G4cout << "\n can't create histo 10" << G4endl;  
  histo[11] = new TH1D("h11", "NClusters", 30,-0.5,29.5);
  if (!histo[11]) G4cout << "\n can't create histo 9" << G4endl;  
  histo[12] = new TH1D("h12", "XCoord", 400,-10.,10.);
  if (!histo[12]) G4cout << "\n can't create histo 9" << G4endl;  
  histo[13] = new TH1D("h13", "YCoord", 400,-10.,10.);
  if (!histo[13]) G4cout << "\n can't create histo 13" << G4endl; 
  histo[14] = new TH1D("h14", "Primary interaction", 5,-0.5,4.5);
  if (!histo[14]) G4cout << "\n can't create histo 14" << G4endl; 
  histo[15] = new TH1D("h15", "Uboson Z decay",1000.,0.,1.);
  if (!histo[15]) G4cout << "\n can't create histo 15" << G4endl; 

  histo2[1] = new TH2D("h21", "XY cell weigh energy ",ECalNRow,-ECalSizeX*0.5,ECalSizeX*0.5,ECalNCol,-ECalSizeY*0.5,ECalSizeY*0.5);
  if (!histo2[1]) G4cout << "\n can't create histo 11" << G4endl;  
  histo2[2] = new TH2D("h22", "XY weigh energy ",600,-30., 30.,600,-30.,30.);
  if (!histo2[2]) G4cout << "\n can't create histo 11" << G4endl;  
  histo2[3] = new TH2D("h23", "nCell vs ETotCal",15,-0.5, 14.5,200,0.,100.);
  if (!histo2[3]) G4cout << "\n can't create histo 11" << G4endl;
  histo2[4] = new TH2D("h24", "nCellCl vs ECluster",ECalNCells*0.5,-0.5, ECalNCells*0.5-0.5,BeamEnergy/2,0.,BeamEnergy);
  if (!histo2[4]) G4cout << "\n can't create histo 11" << G4endl;
  histo2[5] = new TH2D("h25", "X Y cluster Pos ",ECalNRow*10,-ECalSizeX*0.5,ECalSizeX*0.5,ECalNRow*10,-ECalSizeY*0.5,ECalSizeY*0.5);
  if (!histo2[5]) G4cout << "\n can't create histo 15" << G4endl;
  histo2[6] = new TH2D("h26", "Energy vs Theta calo ",275,0.,550.,60,0.,12.);
  if (!histo2[6]) G4cout << "\n can't create histo 16" << G4endl;
  histo2[7] = new TH2D("h27", "Energy vs Theta calo >50 MeV ",275,0.,550.,60,0.,12.);
  if (!histo2[7]) G4cout << "\n can't create histo 16" << G4endl;
  histo2[8] = new TH2D("h28", "Energy vs Theta Brem",250,50.,550.,60,0.,12.);
  if (!histo2[8]) G4cout << "\n can't create histo 18" << G4endl;
  histo2[9] = new TH2D("h29", "Energy vs Theta Annhi ",200,50.,450.,50,0.,10.);
  if (!histo2[9]) G4cout << "\n can't create histo 19" << G4endl;
 
  //Hystogram of generated variables 30-39
  histo2[30] = new TH2D("h30", "Energy vs Theta Gen ",275,0.,550.,60,0.,12.);
  if (!histo2[30]) G4cout << "\n can't create histo 30" << G4endl;
  histo2[31] = new TH2D("h31", "Energy vs Theta Gen Brem ",275,0.,550.,60,0.,12.);
  if (!histo2[31]) G4cout << "\n can't create histo 31" << G4endl;
  histo2[32] = new TH2D("h32", "Energy vs Theta Gen Anni",275,0.,550.,60,0.,12.);
  if (!histo2[32]) G4cout << "\n can't create histo 32" << G4endl;
  histo2[33] = new TH2D("h33", "Energy vs Theta Gen in Acc",275,0.,550.,60,0.,12.);
  if (!histo2[33]) G4cout << "\n can't create histo 33" << G4endl;
  histo2[34] = new TH2D("h34", "Energy vs Theta Gen Brem in Acc ",275,0.,550.,60,0.,12.);
  if (!histo2[34]) G4cout << "\n can't create histo 31" << G4endl;
  histo2[35] = new TH2D("h35", "Energy vs Theta Gen Anni in Acc",275,0.,550.,60,0.,12.);
  if (!histo2[35]) G4cout << "\n can't create histo 32" << G4endl;
  histo2[36] = new TH2D("h36", "Beam X Y ",200,-5.,5.,200,-5.,5.);
  if (!histo2[36]) G4cout << "\n can't create histo 36" << G4endl;

  // create 1 ntuple in subdirectory "tuples"
  //
  ntupl = new TTree("U101", "Envent");
  ntupl->Branch("Nevent", &(myEvt.NTNevent), "Nevent/I");
  ntupl->Branch("ETot", &(myEvt.NTEtot), "ETot/D");
  ntupl->Branch("IDProc", &(myEvt.NTIDProc), "IDProc/D");

  ntupl->Branch("PBeam", &(myEvt.NTPBeam), "PBeam/D");
  ntupl->Branch("PPrim", &(myEvt.NTPriBeam), "PPrim/D");
  ntupl->Branch("XBeam", &(myEvt.NTXBeam), "XBeam/D");
  ntupl->Branch("YBeam", &(myEvt.NTYBeam), "YBeam/D");

  ntupl->Branch("NClusters", &(myEvt.NTNCluster), "NClusters/I");
  ntupl->Branch("NTracks",   &(myEvt.NTNTracks), "NTracks/I");
  ntupl->Branch("NHEPVetoTracks", &(myEvt.NTNHEPVetoTracks), "NHEPVetoTracks/I");

  ntupl->Branch("NPVetoTracks", &(myEvt.NTNPVetoTracks), "NPVetoTracks/I");
  ntupl->Branch("NEleVetoTracks", &(myEvt.NTNEleVetoTracks), "NEleVetoTracks/I");

  ntupl->Branch("NSAC", &(myEvt.NTSACNHit), "NSAC/I");
  ntupl->Branch("NLAV", &(myEvt.NTLAVNHit), "NLAV/I");
  ntupl->Branch("NTarget", &(myEvt.NTNTarget), "NTarget/I");

  ntupl->Branch("ESAC", (myEvt.NTSACE), "ESAC[100]/D");
  ntupl->Branch("TSAC", (myEvt.NTSACT), "TSAC[100]/D");
  ntupl->Branch("PTypeSAC", (myEvt.NTSACPType), "PTypeSAC[100]/D");
  ntupl->Branch("XSAC", (myEvt.NTSACX), "XSAC[100]/D");
  ntupl->Branch("YSAC", (myEvt.NTSACY), "YSAC[100]/D");


  ntupl->Branch("ELAV", (myEvt.NTLAVE), "ELAV[100]/D");
  ntupl->Branch("TLAV", (myEvt.NTLAVT), "TLAV[100]/D");
  ntupl->Branch("PTypeLAV", (myEvt.NTLAVPType), "PTypeLAV[100]/D");
  ntupl->Branch("XLAV", (myEvt.NTLAVX), "XLAV[100]/D");
  ntupl->Branch("YLAV", (myEvt.NTLAVY), "YLAV[100]/D");

  ntupl->Branch("ECluster", (myEvt.NTECluster), "ECluster[20]/D"); 
  ntupl->Branch("QCluster", (myEvt.NTQCluster), "QCluster[20]/D");
  ntupl->Branch("XCluster", (myEvt.NTXCluster), "XCluster[20]/D");
  ntupl->Branch("YCluster", (myEvt.NTYCluster), "YCluster[20]/D");
  ntupl->Branch("ThCluster",(myEvt.NTThCluster),"ThCluster[20]/D");
  ntupl->Branch("M2Cluster",(myEvt.NTM2Cluster),"M2Cluster[20]/D");
  ntupl->Branch("TCluster", (myEvt.NTTCluster), "TCluster[20]/D");  //last modified

  ntupl->Branch("ECell",    (myEvt.NTECell), "ECell[1000]/D");
  ntupl->Branch("QCell",    (myEvt.NTQCell), "QCell[1000]/D");
  ntupl->Branch("TCell",    (myEvt.NTTCell), "TCell[1000]/D");

  ntupl->Branch("ETracker", (myEvt.NTETracker), "ETracker[100]/D");
  ntupl->Branch("TrackerLay",(myEvt.NTTrackerLay), "TrackerLay[100]/D");
  ntupl->Branch("TTracker", (myEvt.NTTrackerTime), "TTracker[100]/D");
  ntupl->Branch("ZTracker", (myEvt.NTTrackerZ), "ZTracker[100]/D");
  ntupl->Branch("XTracker", (myEvt.NTTrackerX), "XTracker[100]/D");
  ntupl->Branch("YTracker", (myEvt.NTTrackerY), "YTracker[100]/D");

  ntupl->Branch("ETarget", &(myEvt.NTETarget), "ETarget/D");
  ntupl->Branch("TTarget", &(myEvt.NTTTarget), "TTarget/D");
  ntupl->Branch("XTarget", &(myEvt.NTXTarget), "XTarget/D");
  ntupl->Branch("YTarget", &(myEvt.NTYTarget), "YTarget/D");

  ntupl->Branch("HEPVetoTrEne" ,(myEvt.NTHEPVetoTrkEne),   "NTHEPVetoTrkEne[100]/D");
  ntupl->Branch("HEPVetoNFing" ,(myEvt.NTHEPVetoTrkFinger),"NTHEPVetoTrkFinger[100]/I");
  ntupl->Branch("HEPVetoTrTime",(myEvt.NTHEPVetoTrkTime),  "NTHEPVetoTrkTime[100]/D");
  ntupl->Branch("HEPVetoFingE", (myEvt.NTHEPVetoFingerE),  "NTHEPVetoFingE[100]/D");
  ntupl->Branch("HEPVetoX", (myEvt.NTHEPVetoX),  "NTHEPVetoX[100]/D");
  ntupl->Branch("HEPVetoY", (myEvt.NTHEPVetoY),  "NTHEPVetoY[100]/D");
 
  ntupl->Branch("PVetoTrEne" ,(myEvt.NTPVetoTrkEne),   "NTPVetoTrkEne[100]/D");
  ntupl->Branch("PVetoNFing" ,(myEvt.NTPVetoTrkFinger),"NTPVetoTrkFinger[100]/I");
  ntupl->Branch("PVetoTrTime",(myEvt.NTPVetoTrkTime),  "NTPVetoTrkTime[100]/D");
  ntupl->Branch("PVetoFingE", (myEvt.NTPVetoFingerE),  "NTPVetoFingE[100]/D");
  ntupl->Branch("PVetoX",     (myEvt.NTPVetoX),        "NTPVetoX[100]/D");
  ntupl->Branch("PVetoY",     (myEvt.NTPVetoY),        "NTPVetoY[100]/D");

  ntupl->Branch("EleVetoTrEne" ,(myEvt.NTEleVetoTrkEne),   "NTEleVetoTrkEne[100]/D");
  ntupl->Branch("EleVetoNFing" ,(myEvt.NTEleVetoTrkFinger),"NTEleVetoTrkFinger[100]/I");
  ntupl->Branch("EleVetoTrTime",(myEvt.NTEleVetoTrkTime),  "NTEleVetoTrkTime[100]/D");
  ntupl->Branch("EleVetoFingE", (myEvt.NTEleVetoFingerE),  "NTEleVetoFingE[100]/D");
  ntupl->Branch("EleVetoX",     (myEvt.NTEleVetoX),        "NTEleVetoX[100]/D");
  ntupl->Branch("EleVetoY",     (myEvt.NTEleVetoY),        "NTEleVetoY[100]/D");

  MySimEvent *mySim = (MyEvent::GetInstance())->GetSimEvent();
  MyEventGenerator *myGen = (MyEvent::GetInstance())->GetGenEvent();
  
  if(IsTrackerRecoON==1){
    ntupl->Branch("NTNTrClus", &(myEvt.NTNTrClus),"NTNTrClus/I");
    ntupl->Branch("NTTrClusX", myEvt.NTTrClusX,"NTTrClusX[1000]/D");
    ntupl->Branch("NTTrClusY", myEvt.NTTrClusY,"NTTrClusY[1000]/D");
    ntupl->Branch("NTTrClusZ", myEvt.NTTrClusZ,"NTTrClusZ[1000]/D");
    ntupl->Branch("NTTrClusLayer", myEvt.NTTrClusLayer,"NTTrClusLayer[1000]/I");
  } 
  ntGen = new TTree("genEvt", "genEvent");
  //  ntGen->Branch("NPrimaries","");   
  ntSim = new TTree("simEvt", "simEvent");
  //ntSim->Branch("simEvt","MySimEvent",&mySim);
  G4cout << "\n----> Histogram file is opened in " << fileName << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::save()
{ 
  if (rootFile) {
    rootFile->Write();        // Writing the histograms to the file
    rootFile->Close();        // and closing the tree (and the file)
    G4cout << "\n----> Histogram Tree is saved \n" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillHisto(G4int ih, G4double xbin, G4double weight)
{
  if (ih >= MaxHisto) {
    G4cout << "---> warning from HistoManager::FillHisto() : histo " << ih
           << " does not exist. (xbin=" << xbin << " weight=" << weight << ")"
           << G4endl;
    return;
  }
 if  (histo[ih]) { histo[ih]->Fill(xbin, weight); }
}

void HistoManager::FillHisto2(G4int ih, G4double xbin, G4double ybin, G4double weight)
{
  if (ih >= MaxHisto) {
    G4cout << "---> warning from HistoManager::FillHisto() : histo " << ih
           << " does not exist. (xbin=" <<xbin<<" "<<ybin<< " weight=" << weight << ")"
           << G4endl;
    return;
  }
  if  (histo2[ih]) {histo2[ih]->Fill(xbin, ybin, weight); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Normalize(G4int ih, G4double fac)
{
  if (ih >= MaxHisto) {
    G4cout << "---> warning from HistoManager::Normalize() : histo " << ih
           << " does not exist. (fac=" << fac << ")" << G4endl;
    return;
  }
  if (histo[ih]) histo[ih]->Scale(fac);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillNtuple(NTEvent* Evt)
{
  if (ntupl) ntupl->Fill();
  if(ntSim) ntSim->Fill();
  if(ntGen) ntGen->Fill();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::PrintStatistic()
{
  if(histo[1]) {
    G4cout << "\n ----> print histograms statistic \n" << G4endl;
    
    G4cout<<" ECry : mean = " << G4BestUnit(histo[1]->GetMean(), "Energy") 
      << " rms = " << G4BestUnit(histo[1]->GetRMS(),  "Energy") << G4endl;
    G4cout<< " EBeam : mean = " << G4BestUnit(histo[2]->GetMean(), "Energy") 
      << " rms = " << G4BestUnit(histo[2]->GetRMS(),  "Energy") << G4endl;
    G4cout 
      << " ETracker : mean = " << G4BestUnit(histo[3]->GetMean(), "Energy") 
      << " rms = " << G4BestUnit(histo[3]->GetRMS(),  "Energy") << G4endl;
    //    G4cout 
    //    << " LGap : mean = " << G4BestUnit(histo[4]->GetMean(), "Length") 
    //            << " rms = " << G4BestUnit(histo[4]->GetRMS(),  "Length") << G4endl;
    
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

