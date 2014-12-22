#include "GTreeMeson.h"
#include "GTreeManager.h"


GTreeMeson::GTreeMeson(GTreeManager *Manager, const TString& _Name)    :
    GTreeParticle(Manager, _Name),
    subRootinos(new TClonesArray("TClonesArray", GTreeParticle_MaxEntries)),
    subPhotons(new TClonesArray("TClonesArray", GTreeParticle_MaxEntries)),
    subChargedPi(new TClonesArray("TClonesArray", GTreeParticle_MaxEntries))
{
    for(int i=0; i<GTreeParticle_MaxEntries; i++)
    {
        nSubParticles[i]  = 0;
        nSubRootinos[i]   = 0;
        nSubPhotons[i]    = 0;
        nSubChargedPi[i]  = 0;
    }
}

GTreeMeson::~GTreeMeson()
{
    if (subRootinos)
        delete subRootinos;
    if (subPhotons)
        delete subPhotons;
    if (subChargedPi)
        delete subChargedPi;
}

void    GTreeMeson::Clear()
{
    GTreeParticle::Clear();
    //Cause the stored object(TClonesArray) has allocated memory one has to delete to call the destructor.
    subRootinos->Delete();
    subPhotons->Delete();
    subChargedPi->Delete();
}

void    GTreeMeson::SetBranchAdresses()
{
    GTreeParticle::SetBranchAdresses();
    inputTree->SetBranchAddress("nSubParticles", nSubParticles);
    inputTree->SetBranchAddress("nSubRootinos", nSubRootinos);
    inputTree->SetBranchAddress("nSubPhotons", nSubPhotons);
    inputTree->SetBranchAddress("nSubChargedPi", nSubChargedPi);
    inputTree->SetBranchAddress("subRootinos.", &subRootinos);
    inputTree->SetBranchAddress("subPhotons.", &subPhotons);
    inputTree->SetBranchAddress("subChargedPi.", &subChargedPi);
}

void    GTreeMeson::SetBranches()
{
    GTreeParticle::SetBranches();
    outputTree->Branch("nSubParticles", nSubParticles, "nSubParticles[nParticles]/b");
    outputTree->Branch("nSubRootinos", nSubRootinos,"nSubRootinos[nParticles]/b");
    outputTree->Branch("nSubPhotons", nSubPhotons,"nSubParticles[nParticles]/b");
    outputTree->Branch("nSubChargedPi", nSubChargedPi,"nSubParticles[nParticles]/b");
    outputTree->Branch("subRootinos.", &subRootinos, 32, 0);
    outputTree->Branch("subPhotons.", &subPhotons, 32, 0);
    outputTree->Branch("subChargedPi.", &subChargedPi, 32, 0);
}

void    GTreeMeson::AddParticle(const Int_t _NSubRootinos, Int_t* subRootinos_index, TLorentzVector** subRootinos_list, const Int_t _NSubPhotons, Int_t* subPhotons_index, TLorentzVector** subPhotons_list, const Int_t _NSubChargedPi, Int_t* subChargedPi_index, TLorentzVector** subChargedPi_list)
{
    new((*subRootinos)[nParticles]) TClonesArray("TLorentzVector", _NSubRootinos);
    new((*subPhotons)[nParticles]) TClonesArray("TLorentzVector", _NSubPhotons);
    new((*subChargedPi)[nParticles]) TClonesArray("TLorentzVector", _NSubChargedPi);
    apparatus[nParticles]    = GTreeRawParticle::APPARATUS_NONE;
    time[nParticles]         = 0;
    clusterSize[nParticles]  = 0;
    vetoEnergy[nParticles]          = 0;
    MWPC0Energy[nParticles]        = 0;
    MWPC1Energy[nParticles]        = 0;

    nSubParticles[nParticles]  = _NSubRootinos + _NSubPhotons + _NSubChargedPi;
    nSubRootinos[nParticles]   = _NSubRootinos;
    nSubPhotons[nParticles]    = _NSubPhotons;
    nSubChargedPi[nParticles]  = _NSubChargedPi;
    TLorentzVector  sum;
    for(int i=0; i<_NSubRootinos; i++)
    {
        sum += *subRootinos_list[i];
        new((*((TClonesArray*)subRootinos->At(nParticles)))[i]) TLorentzVector(*subRootinos_list[i]);
        apparatus[nParticles]    = apparatus[nParticles] | manager->rootinos->GetApparatus(subRootinos_index[i]);
        time[nParticles]         += manager->rootinos->GetTime(subRootinos_index[i]);
        clusterSize[nParticles]  += manager->rootinos->GetClusterSize(subRootinos_index[i]);
        vetoEnergy[nParticles]          += manager->rootinos->GetVetoEnergy(subRootinos_index[i]);
        MWPC0Energy[nParticles]        += manager->rootinos->GetMWPC0Energy(subRootinos_index[i]);
        MWPC1Energy[nParticles]        += manager->rootinos->GetMWPC1Energy(subRootinos_index[i]);
    }
    for(int i=0; i<_NSubPhotons; i++)
    {
        sum += *subPhotons_list[i];
        new((*((TClonesArray*)subPhotons->At(nParticles)))[i]) TLorentzVector(*subPhotons_list[i]);
        apparatus[nParticles]    = apparatus[nParticles] | manager->photons->GetApparatus(subPhotons_index[i]);
        time[nParticles]         += manager->photons->GetTime(subPhotons_index[i]);
        clusterSize[nParticles]  += manager->photons->GetClusterSize(subPhotons_index[i]);
        vetoEnergy[nParticles]          += manager->photons->GetVetoEnergy(subPhotons_index[i]);
        MWPC0Energy[nParticles]        += manager->photons->GetMWPC0Energy(subPhotons_index[i]);
        MWPC1Energy[nParticles]        += manager->photons->GetMWPC1Energy(subPhotons_index[i]);
    }
    for(int i=0; i<_NSubChargedPi; i++)
    {
        sum += *subChargedPi_list[i];
        new((*((TClonesArray*)subChargedPi->At(nParticles)))[i]) TLorentzVector(*subChargedPi_list[i]);
        apparatus[nParticles]    = apparatus[nParticles] | manager->chargedPi->GetApparatus(subChargedPi_index[i]);
        time[nParticles]         += manager->chargedPi->GetTime(subChargedPi_index[i]);
        clusterSize[nParticles]  += manager->chargedPi->GetClusterSize(subChargedPi_index[i]);
        vetoEnergy[nParticles]          += manager->chargedPi->GetVetoEnergy(subChargedPi_index[i]);
        MWPC0Energy[nParticles]        += manager->chargedPi->GetMWPC0Energy(subChargedPi_index[i]);
        MWPC1Energy[nParticles]        += manager->chargedPi->GetMWPC1Energy(subChargedPi_index[i]);
    }
    time[nParticles]         /= nSubParticles[nParticles];
    new((*particles)[nParticles]) TLorentzVector(sum);
    nParticles++;
    manager->countReconstructed++;
}


void    GTreeMeson::AddParticle(const Int_t subParticle_index0, const TLorentzVector& subParticle0, const Int_t pdg0, const Int_t subParticle_index1, const TLorentzVector& subParticle1, const Int_t pdg1)
{
    new((*subRootinos)[nParticles]) TClonesArray("TLorentzVector", 2);
    new((*subPhotons)[nParticles]) TClonesArray("TLorentzVector", 2);
    new((*subChargedPi)[nParticles]) TClonesArray("TLorentzVector", 2);

    nSubParticles[nParticles]  = 2;
    nSubRootinos[nParticles]   = 0;
    nSubPhotons[nParticles]    = 0;
    nSubChargedPi[nParticles]  = 0;

    if(pdg0 == manager->pdgDB->GetParticle("gamma")->PdgCode())
    {
        new((*((TClonesArray*)subPhotons->At(nParticles)))[nSubPhotons[nParticles]]) TLorentzVector(subParticle0);
        apparatus[nParticles]    = manager->photons->GetApparatus(subParticle_index0);
        time[nParticles]         = manager->photons->GetTime(subParticle_index0);
        clusterSize[nParticles]  = manager->photons->GetClusterSize(subParticle_index0);
        vetoEnergy[nParticles]          = manager->photons->GetVetoEnergy(subParticle_index0);
        MWPC0Energy[nParticles]        = manager->photons->GetMWPC0Energy(subParticle_index0);
        MWPC1Energy[nParticles]        = manager->photons->GetMWPC1Energy(subParticle_index0);
        nSubPhotons[nParticles]++;
    }
    else if(pdg0 == manager->pdgDB->GetParticle("pi+")->PdgCode())
    {
        new((*((TClonesArray*)subChargedPi->At(nParticles)))[nSubChargedPi[nParticles]]) TLorentzVector(subParticle0);
        apparatus[nParticles]    = manager->chargedPi->GetApparatus(subParticle_index0);
        time[nParticles]         = manager->chargedPi->GetTime(subParticle_index0);
        clusterSize[nParticles]  = manager->chargedPi->GetClusterSize(subParticle_index0);
        vetoEnergy[nParticles]          = manager->chargedPi->GetVetoEnergy(subParticle_index0);
        MWPC0Energy[nParticles]        = manager->chargedPi->GetMWPC0Energy(subParticle_index0);
        MWPC1Energy[nParticles]        = manager->chargedPi->GetMWPC1Energy(subParticle_index0);
        nSubChargedPi[nParticles]++;
    }
    else
    {
        new((*((TClonesArray*)subRootinos->At(nParticles)))[nSubRootinos[nParticles]]) TLorentzVector(subParticle0);
        apparatus[nParticles]    = manager->rootinos->GetApparatus(subParticle_index0);
        time[nParticles]         = manager->rootinos->GetTime(subParticle_index0);
        clusterSize[nParticles]  = manager->rootinos->GetClusterSize(subParticle_index0);
        vetoEnergy[nParticles]          = manager->rootinos->GetVetoEnergy(subParticle_index0);
        MWPC0Energy[nParticles]        = manager->rootinos->GetMWPC0Energy(subParticle_index0);
        MWPC1Energy[nParticles]        = manager->rootinos->GetMWPC1Energy(subParticle_index0);
        nSubRootinos[nParticles]++;
    }
    
    if(pdg1 == manager->pdgDB->GetParticle("gamma")->PdgCode())
    {
        new((*((TClonesArray*)subPhotons->At(nParticles)))[nSubPhotons[nParticles]]) TLorentzVector(subParticle1);
        apparatus[nParticles]    = apparatus[nParticles] | manager->photons->GetApparatus(subParticle_index1);
        time[nParticles]         += manager->photons->GetTime(subParticle_index1);
        clusterSize[nParticles]  += manager->photons->GetClusterSize(subParticle_index1);
        vetoEnergy[nParticles]          += manager->photons->GetVetoEnergy(subParticle_index1);
        MWPC0Energy[nParticles]        += manager->photons->GetMWPC0Energy(subParticle_index1);
        MWPC1Energy[nParticles]        += manager->photons->GetMWPC1Energy(subParticle_index1);
        nSubPhotons[nParticles]++;
    }
    else if(pdg1 == manager->pdgDB->GetParticle("pi+")->PdgCode())
    {
        new((*((TClonesArray*)subChargedPi->At(nParticles)))[nSubChargedPi[nParticles]]) TLorentzVector(subParticle1);
        apparatus[nParticles]    = apparatus[nParticles] | manager->chargedPi->GetApparatus(subParticle_index1);
        time[nParticles]         += manager->chargedPi->GetTime(subParticle_index1);
        clusterSize[nParticles]  += manager->chargedPi->GetClusterSize(subParticle_index1);
        vetoEnergy[nParticles]          += manager->chargedPi->GetVetoEnergy(subParticle_index1);
        MWPC0Energy[nParticles]        += manager->chargedPi->GetMWPC0Energy(subParticle_index1);
        MWPC1Energy[nParticles]        += manager->chargedPi->GetMWPC1Energy(subParticle_index1);
        nSubChargedPi[nParticles]++;
    }
    else
    {
        new((*((TClonesArray*)subRootinos->At(nParticles)))[nSubRootinos[nParticles]]) TLorentzVector(subParticle1);
        apparatus[nParticles]    = apparatus[nParticles] | manager->rootinos->GetApparatus(subParticle_index1);
        time[nParticles]         += manager->rootinos->GetTime(subParticle_index1);
        clusterSize[nParticles]  += manager->rootinos->GetClusterSize(subParticle_index1);
        vetoEnergy[nParticles]          += manager->rootinos->GetVetoEnergy(subParticle_index1);
        MWPC0Energy[nParticles]        += manager->rootinos->GetMWPC0Energy(subParticle_index1);
        MWPC1Energy[nParticles]        += manager->rootinos->GetMWPC1Energy(subParticle_index1);
        nSubRootinos[nParticles]++;
    }
    time[nParticles]         /= 2;
    new((*particles)[nParticles]) TLorentzVector(subParticle0 + subParticle1);
    nParticles++;
    manager->countReconstructed++;
}


void    GTreeMeson::Print() const
{
    GTree::Print();
    cout << "nParticles: " << nParticles << endl;
    for(int i=0; i<nParticles; i++)
    {
        GTreeParticle::PrintParticle(i);
        cout << "nSubParticles: " << (Int_t)nSubParticles[i] << "   nSubRootinos: " << (Int_t)nSubRootinos[i] << "   nSubPhotons: " << (Int_t)nSubPhotons[i] << "   nSubChargedPi: " << (Int_t)nSubChargedPi[i] << endl;
        for(int l=0; l<nSubRootinos[i]; l++)
            cout << "\t\tPx: " << SubRootinos(i, l).Px() << "   Py: " << SubRootinos(i, l).Py() << "   Pz: " << SubRootinos(i, l).Pz() << "   E: " << SubRootinos(i, l).E()<< endl;
        for(int l=0; l<nSubPhotons[i]; l++)
            cout << "\t\tPx: " << SubPhotons(i, l).Px() << "   Py: " << SubPhotons(i, l).Py() << "   Pz: " << SubPhotons(i, l).Pz() << "   E: " << SubPhotons(i, l).E()<< endl;
        for(int l=0; l<nSubChargedPi[i]; l++)
            cout << "\t\tPx: " << SubChargedPi(i, l).Px() << "   Py: " << SubChargedPi(i, l).Py() << "   Pz: " << SubChargedPi(i, l).Pz() << "   E: " << SubChargedPi(i, l).E()<< endl;
    }
}

UChar_t             nSubParticles[GTreeParticle_MaxEntries];
UChar_t             nSubRootinos[GTreeParticle_MaxEntries];
UChar_t             nSubPhotons[GTreeParticle_MaxEntries];
UChar_t             nSubChargedPi[GTreeParticle_MaxEntries];
TClonesArray*       subRootinos;
TClonesArray*       subPhotons;
TClonesArray*       subChargedPi;
