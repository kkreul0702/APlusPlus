#ifndef TABSFITPARTICLE_H
#define TABSFITPARTICLE_H

#include "TMatrixD.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TNamed.h"

class TAbsFitParticle : public TNamed {

public :

  TAbsFitParticle();
  TAbsFitParticle(const TString &name, const TString &title);
  virtual ~TAbsFitParticle();
  virtual TAbsFitParticle* clone( TString newname = "" ) const = 0;

  // returns derivative dP/dy with P=(p,E) and y=(par1, par2, par3, ...) 
  // the free parameters of the fit. The columns of the matrix contain 
  // (dP/dpar1, dP/dpar2, ...).
  virtual TMatrixD* getDerivative() = 0;
  virtual TMatrixD* transform(const TLorentzVector& vec) = 0;
  virtual TLorentzVector* calc4Vec( const TMatrixD* params ) = 0;
  virtual void setIni4Vec(const TLorentzVector* pini) = 0;

  virtual void applycorr(TMatrixD* corrMatrix);
  virtual void setCovMatrix(const TMatrixD* theCovMatrix);
  virtual void setCovMatrixCart(const TMatrixD* theCovMatrix);
  virtual const TMatrixD* getCovMatrix() const { return &_covMatrix; }
  virtual void setCovMatrixFit(const TMatrixD* theCovMatrixFit);
  virtual const TMatrixD* getCovMatrixFit() const { return &_covMatrixFit; }
  virtual const TMatrixD* getCovMatrixDeltaY();  
  virtual const TMatrixD* getPull();
  Int_t getNPar() const { return _nPar; }
  const TLorentzVector* getCurr4Vec() const { return &_pcurr; }
  const TLorentzVector* getIni4Vec() const { return &_pini; }
  const TMatrixD* getParIni() const { return &_iniparameters; }
  void  setParIni(const TMatrixD* parini);
  const TMatrixD* getParCurr() const { return &_parameters; }
  const TVector3* getu1()    { return &_u1; }
  const TVector3* getu2(){ return &_u2; }
  const TVector3* getu3()  { return &_u3; }

  void printParticle() const; 
  virtual void reset();

protected :

  void calcCovMatrixDeltaY();

  Int_t _nPar;        // Number of free parameters
  TVector3 _u1;       // base vector 1
  TVector3 _u2;       // base vector 2
  TVector3 _u3;       // base vector 3

  TMatrixD _covMatrix;        // covariance matrix
  TMatrixD _covMatrixFit;     // fitted covariance matrix
  TMatrixD _covMatrixDeltaY;  // V(deltaY) == V(y_meas) - V(y_fit)
  TMatrixD _pull;             // pull values for all parameters
  TMatrixD _iniparameters;    // initialized parameters (parameters values before the fit)
  TMatrixD _parameters;       // fitted parameters
  TLorentzVector _pini;       // measured 4vector
  TLorentzVector _pcurr;      // fitted 4vector
  
  ClassDef(TAbsFitParticle, 1)  // Abstract base class for fit particles
};

#endif
