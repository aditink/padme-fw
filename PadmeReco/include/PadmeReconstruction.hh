#ifndef PadmeReconstruction_H
#define PadmeReconstruction_H

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TObjArray.h"

#include "TPadmeEvent.hh"

#include "PadmeVReconstruction.hh"

class PadmeReconstruction : public PadmeVReconstruction
{

public:

  PadmeReconstruction(TObjArray*, TString, TFile*, Int_t, UInt_t);
  ~PadmeReconstruction();

  void Init(PadmeVReconstruction*) { return; }
  void Init(Int_t, UInt_t);
  Bool_t NextEvent();
  void EndProcessing();

  PadmeVReconstruction* FindReco(TString);

private:

  TChain* BuildChain(TString);
  TString CheckProtocols(TString);

  void InitDetectorsInfo();

  TObjArray* fInputFileNameList;
  //TString fConfigFileName;
  //TFile* fHistoFile;
  TChain* fMCChain;   ///< Dedicated TChain for MC information reading
  TPadmeEvent* fMCTruthEvent;

  typedef std::vector<PadmeReconstruction*> RecoVector;
  RecoVector fRecoLibrary;
 
  Int_t fNEvt;

};
#endif
