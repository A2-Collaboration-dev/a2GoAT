#include "GTreeA2Geant.h"
#include <stdexcept>
#include "string.h"
#include "TString.h"

void GTreeA2Geant::SetBranchAdresses()
{
    if(inputTree->GetBranch("nhits")) inputTree->SetBranchAddress("nhits",&fnhits);
    if(inputTree->GetBranch("npart")) inputTree->SetBranchAddress("npart",&fnpart);
    if(inputTree->GetBranch("ntaps")) inputTree->SetBranchAddress("ntaps",&fntaps);
    if(inputTree->GetBranch("nvtaps")) inputTree->SetBranchAddress("nvtaps",&fnvtaps);
    if(inputTree->GetBranch("vhits")) inputTree->SetBranchAddress("vhits",&fvhits);
    if(inputTree->GetBranch("plab")) inputTree->SetBranchAddress("plab",plab);
    if(inputTree->GetBranch("tctaps")) inputTree->SetBranchAddress("tctaps",tctaps);
    if(inputTree->GetBranch("vertex")) inputTree->SetBranchAddress("vertex",fvertex);
    if(inputTree->GetBranch("beam")) inputTree->SetBranchAddress("beam",fbeam);
    if(inputTree->GetBranch("dircos")) inputTree->SetBranchAddress("dircos",dircos);
    if(inputTree->GetBranch("ecryst")) inputTree->SetBranchAddress("ecryst",ecryst);
    if(inputTree->GetBranch("tcryst")) inputTree->SetBranchAddress("tcryst",tcryst);
    if(inputTree->GetBranch("ectapfs")) inputTree->SetBranchAddress("ectapfs",ectapfs);
    if(inputTree->GetBranch("ectapsl")) inputTree->SetBranchAddress("ectapsl",ectapsl);
    if(inputTree->GetBranch("elab")) inputTree->SetBranchAddress("elab",elab);
    if(inputTree->GetBranch("eleak")) inputTree->SetBranchAddress("eleak",&feleak);
    if(inputTree->GetBranch("enai")) inputTree->SetBranchAddress("enai",&fenai);
    if(inputTree->GetBranch("etot")) inputTree->SetBranchAddress("etot",&fetot);
    if(inputTree->GetBranch("eveto")) inputTree->SetBranchAddress("eveto",eveto);
    if(inputTree->GetBranch("tveto")) inputTree->SetBranchAddress("tveto",tveto);
    if(inputTree->GetBranch("evtaps")) inputTree->SetBranchAddress("evtaps",evtaps);
    if(inputTree->GetBranch("icryst")) inputTree->SetBranchAddress("icryst",icryst);
    if(inputTree->GetBranch("ictaps")) inputTree->SetBranchAddress("ictaps",ictaps);
    if(inputTree->GetBranch("ivtaps")) inputTree->SetBranchAddress("ivtaps",ivtaps);
    if(inputTree->GetBranch("idpart")) inputTree->SetBranchAddress("idpart",idpart);
    if(inputTree->GetBranch("iveto")) inputTree->SetBranchAddress("iveto",iveto);
    if(inputTree->GetBranch("nmwpc")) inputTree->SetBranchAddress("nmwpc",&fnmwpc);
    if(inputTree->GetBranch("imwpc")) inputTree->SetBranchAddress("imwpc",imwpc);
    if(inputTree->GetBranch("mposx")) inputTree->SetBranchAddress("mposx",mposx);
    if(inputTree->GetBranch("mposy")) inputTree->SetBranchAddress("mposy",mposy);
    if(inputTree->GetBranch("mposz")) inputTree->SetBranchAddress("mposz",mposz);
    if(inputTree->GetBranch("emwpc")) inputTree->SetBranchAddress("emwpc",emwpc);
    if(inputTree->GetBranch("mc_evt_id")) inputTree->SetBranchAddress("mc_evt_id",&mc_evt_id);
    if(inputTree->GetBranch("mc_rnd_id")) inputTree->SetBranchAddress("mc_rnd_id",&mc_evt_id);
}

void GTreeA2Geant::SetBranches()
{
    outputTree = inputTree->CloneTree(0);
}

Double_t GTreeA2Geant::sumArray(const Float_t * const data, const Int_t size)
{
    Double_t sum(0.0);

    for( Int_t i=0; i<size; ++i )
        sum += data[i];

    return sum;
}

void GTreeA2Geant::buildPattern(GTreeA2Geant::hitvector &pattern, const Int_t * const indices, const Float_t * const data, const Int_t nhits, const Int_t patternsize)
{

    pattern.assign(patternsize, 0.0);

    for(Int_t i=0; i < nhits; ++i ) {
        pattern.at(i) = data[ indices[i] ];
    }
}

GTreeA2Geant::GTreeA2Geant(GTreeManager *Manager):
    GTree(Manager, TString(GEANT_TREE_NAME)),
    fnhits(0),
    fnpart(0),
    fntaps(0),
    fnvtaps(0),
    fvhits(0),
    plab(),
    tctaps(),
    fvertex(),
    fbeam(),
    dircos(),
    ecryst(),
    tcryst(),
    ectapfs(),
    ectapsl(),
    elab(),
    feleak(0),
    fenai(0),
    fetot(0),
    eveto(),
    tveto(),
    evtaps(),
    icryst(),
    ictaps(),
    ivtaps(),
    idpart(),
    iveto(),
    fnmwpc(0),
    imwpc(),
    mposx(),
    mposy(),
    mposz(),
    emwpc(),
    mc_evt_id(0),
    mc_rnd_id(0)
{
}

GTreeA2Geant::~GTreeA2Geant()
{
}

TLorentzVector GTreeA2Geant::GetBeam() const
{
    double x = fbeam[0], y = fbeam[1], z = fbeam[2], t = fbeam[3];

    return TLorentzVector(t*x, t*y, t*z, t);
}

TVector3 GTreeA2Geant::GetVertex() const
{
    return TVector3(fvertex);
}

Double_t GTreeA2Geant::GetCBESum() const
{
    return fetot;
}

Double_t GTreeA2Geant::GetPIDESum() const
{
    return sumArray(GetPIDHitEnergies(), GetNPIDHits());
}

Double_t GTreeA2Geant::GetTAPSESum() const
{
    return sumArray(ectapsl, fntaps);
}

Double_t GTreeA2Geant::GetTAPSVetoESum() const
{
    return sumArray(evtaps, fnvtaps);
}


Int_t GTreeA2Geant::GetNCBHits() const
{
    return fnhits;
}

Int_t GTreeA2Geant::GetCBHitIndex(const UInt_t n) const throw (std::out_of_range)
{
    if( n >= (UInt_t)GetNCBHits() )
        throw std::out_of_range(Form("%s: CB Hit index out of bounds. (%d/%d)", __func__, n, GetNCBHits()));

    return icryst[n];
}

Float_t GTreeA2Geant::GetCBHitEnergy(const UInt_t n) const throw (std::out_of_range)
{
    if( n >= (UInt_t)GetNCBHits() )
        throw std::out_of_range(Form("%s: CB Hit index out of bounds. (%d/%d)", __func__, n, GetNCBHits()));

    return ecryst[n];
}

const Int_t * const GTreeA2Geant::GetCBHitIndices() const
{
    return icryst;
}

const Float_t * const GTreeA2Geant::GetCBHitEnergies() const
{
    return ecryst;
}


Int_t GTreeA2Geant::GetNTAPSHits() const
{
    return fntaps;
}

const Int_t * const GTreeA2Geant::GetTAPSHitIndices() const
{
    return ictaps;
}

Int_t GTreeA2Geant::GetTAPSHitIndex(const UInt_t n) const throw (std::out_of_range)
{
    if( n >= (UInt_t)GetNTAPSHits() )
        throw std::out_of_range(Form("%s: TAPS Hit index out of bounds. (%d/%d)", __func__, n, GetNTAPSHits()));

    return ictaps[n];
}

Float_t GTreeA2Geant::GetTAPSHitEnergyLong(const UInt_t n) const throw (std::out_of_range)
{
    if( n >= (UInt_t)GetNTAPSHits() )
        throw std::out_of_range(Form("%s: TAPS Hit index out of bounds. (%d/%d)", __func__, n, GetNTAPSHits()));

    return ectapsl[n];
}

Float_t GTreeA2Geant::GetTAPSHitEnergyShort(const UInt_t n) const throw (std::out_of_range)
{
    if( n >= (UInt_t)GetNTAPSHits() )
        throw std::out_of_range(Form("%s: TAPS Hit index out of bounds. (%d/%d)", __func__, n, GetNTAPSHits()));

    return ectapfs[n];
}

const Float_t * const GTreeA2Geant::GetTAPSHitEnergiesLong() const
{
    return ectapsl;
}

const Float_t * const GTreeA2Geant::GetTAPSHitEnergiesShort() const
{
    return ectapfs;
}

Float_t GTreeA2Geant::GetTAPSHitTime(const UInt_t n) const throw (std::out_of_range)
{
    if( n >= (UInt_t)GetNTAPSHits() )
        throw std::out_of_range(Form("%s: TAPS Hit index out of bounds. (%d/%d)", __func__, n, GetNTAPSHits()));

    return tctaps[n];
}

const Float_t * const GTreeA2Geant::GetTAPSHitTimes() const
{
    return tctaps;
}

Int_t GTreeA2Geant::GetNTAPSVetoHits() const
{
    return fnvtaps;
}

Int_t GTreeA2Geant::GetTAPSVetoHitIndex(const UInt_t n) const throw (std::out_of_range)
{
    if( n >= (UInt_t)GetNTAPSVetoHits() )
        throw std::out_of_range(Form("%s: TAPS Hit index out of bounds. (%d/%d)", __func__, n, GetNTAPSVetoHits()));

    return ivtaps[n];
}

const Int_t * const GTreeA2Geant::GetTAPSVetoHitIndices() const
{
    return ivtaps;
}

Float_t GTreeA2Geant::GetTAPSVetoHitEnergy(const UInt_t n) const throw (std::out_of_range)
{
    if( n >= (UInt_t)GetNTAPSVetoHits() )
        throw std::out_of_range(Form("%s: TAPS Hit index out of bounds. (%d/%d)", __func__, n, GetNTAPSVetoHits()));

    return evtaps[n];
}

const Float_t * const GTreeA2Geant::GetTAPSVetoHitEnergies() const
{
    return evtaps;
}

Int_t GTreeA2Geant::GetNPIDHits() const
{
    return fvhits;
}

Int_t GTreeA2Geant::GetPIDHitIndex(const UInt_t n) const throw (std::out_of_range)
{
    if( n >= (UInt_t)GetNPIDHits() )
        throw std::out_of_range(Form("%s: PID Hit index out of bounds. (%d/%d)", __func__, n, GetNPIDHits()));

    return iveto[n];
}

Float_t GTreeA2Geant::GetPIDHitEnergy(const UInt_t n) const throw (std::out_of_range)
{
    if( n >= (UInt_t)GetNPIDHits() )
        throw std::out_of_range(Form("%s: PID Hit index out of bounds. (%d/%d)", __func__, n, GetNPIDHits()));

    return eveto[n];
}

const Int_t * const GTreeA2Geant::GetPIDHitIndices() const
{
    return iveto;
}

const Float_t * const GTreeA2Geant::GetPIDHitEnergies() const
{
    return eveto;
}

Float_t GTreeA2Geant::GetPIDHitTime(const UInt_t n) const throw (std::out_of_range)
{
    if( n >= (UInt_t)GetNPIDHits() )
        throw std::out_of_range(Form("%s: PID Hit index out of bounds. (%d/%d)", __func__, n, GetNPIDHits()));

    return tveto[n];
}

const Float_t * const GTreeA2Geant::GetPIDHitTimes() const
{
    return tveto;
}

Int_t GTreeA2Geant::GetNMWPCHits() const
{
    return fnmwpc;
}

const Int_t * const GTreeA2Geant::GetMWPCHitIndices() const
{
    return imwpc;
}

const Float_t * const GTreeA2Geant::GetMWPCHitEnergies() const
{
    return emwpc;
}

const Float_t * const GTreeA2Geant::GetMWPCHitPosX() const
{
    return mposx;
}

const Float_t * const GTreeA2Geant::GetMWPCHitPosY() const
{
    return mposy;
}

const Float_t * const GTreeA2Geant::GetMWPCHitPosZ() const
{
    return mposz;
}

TVector3 GTreeA2Geant::GetMWPCVector(const UInt_t n) const throw(std::out_of_range)
{
    if( n >= (UInt_t)GetNMWPCHits() )
        throw std::out_of_range(Form("%s: MWPC Particle index out of bounds. (%d/%d)", __func__, n, GetNMWPCHits()));

    return TVector3(mposx[n], mposy[n], mposz[n]);
}

Float_t GTreeA2Geant::GetMWPCEnergy(const UInt_t n) const throw(std::out_of_range)
{
    if( n >= (UInt_t)GetNMWPCHits() )
        throw std::out_of_range(Form("%s: MWPC Particle index out of bounds. (%d/%d)", __func__, n, GetNMWPCHits()));

    return emwpc[n];
}

Int_t GTreeA2Geant::GetMWPCIndex(const UInt_t n) const throw(std::out_of_range)
{
    if( n >= (UInt_t)GetNMWPCHits() )
        throw std::out_of_range(Form("%s: MWPC Particle index out of bounds. (%d/%d)", __func__, n, GetNMWPCHits()));

    return imwpc[n];
}

void GTreeA2Geant::BuildCBHitPattern(GTreeA2Geant::hitvector &pattern) const
{
    try {
        buildPattern(pattern, icryst, ecryst, GetNCBHits(), 720);
    } catch (const std::out_of_range& oor) {
        std::cerr << "CB crystal index out of range " << oor.what() << std::endl;
    }
}

void GTreeA2Geant::BuildTAPSHitPattern(GTreeA2Geant::hitvector &pattern) const
{
    try {
        buildPattern(pattern, ictaps, ectapsl, GetNTAPSHits(), 438);
    } catch (const std::out_of_range& oor) {
        std::cerr << "TAPS crystal index out of range " << oor.what() << std::endl;
    }
}

UInt_t GTreeA2Geant::GetNTrueParticles() const
{
    return fnpart;
}

UInt_t GTreeA2Geant::GetTrueID(const UInt_t n) const throw(std::out_of_range)
{
    if( n >= GetNTrueParticles() )
        throw std::out_of_range(Form("%s: MC-True Particle index out of bounds. (%d/%d)", __func__, n, GetNTrueParticles()));

    return idpart[n];

}

TLorentzVector GTreeA2Geant::GetTrueVector(const UInt_t n) const throw(std::out_of_range)
{
    if( n >= GetNTrueParticles() )
        throw std::out_of_range(Form("%s: MC-True Particle index out of bounds. (%d/%d)", __func__, n, GetNTrueParticles()));

    TVector3 p(dircos[n][0], dircos[n][1], dircos[n][2]);
    p *= plab[n];

    TLorentzVector lv( p, elab[n]);

    return lv;
}

template <class T>
void zero( T& data ) {
    memset( data, 0, sizeof(data) );
}

void GTreeA2Geant::Clear()
{
    fnhits = 0;
    fnpart = 0;
    fntaps = 0;
    fnvtaps = 0;
    fvhits = 0;
    zero(plab);
    zero(tctaps);
    zero(fvertex);
    zero(fbeam);
    zero(dircos);
    zero(ecryst);
    zero(tcryst);
    zero(ectapfs);
    zero(ectapsl);
    zero(elab);
    feleak = 0;
    fenai = 0;
    fetot = 0;
    zero(eveto);
    zero(tveto);
    zero(evtaps);
    zero(icryst);
    zero(ictaps);
    zero(ivtaps);
    zero(idpart);
    zero(iveto);
    fnmwpc = 0;
    zero(imwpc);
    zero(mposx);
    zero(mposy);
    zero(mposz);
    zero(emwpc);
    mc_evt_id = 0;
    mc_rnd_id = 0;
}
