//////////////////////////////////////////////////////////////////////////
//
// 'MULTIDIMENSIONAL MODELS' RooFit tutorial macro #313
//
// Working with parameterized ranges to define non-rectangular regions
// for fitting and integration
//
//
//
// 07/2008 - Wouter Verkerke
//
/////////////////////////////////////////////////////////////////////////

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooPolynomial.h"
#include "RooProdPdf.h"
#include "TCanvas.h"
#include "RooPlot.h"
using namespace RooFit ;


class TestBasic313 : public RooFitTestUnit
{
public:
  TestBasic313(TFile* refFile, bool writeRef, Int_t verbose) : RooFitTestUnit("Integration over non-rectangular regions",refFile,writeRef,verbose) {} ;
  bool testCode() {

  // C r e a t e   3 D   p d f
  // -------------------------

  // Define observable (x,y,z)
  RooRealVar x("x","x",0,10) ;
  RooRealVar y("y","y",0,10) ;
  RooRealVar z("z","z",0,10) ;

  // Define 3 dimensional pdf
  RooRealVar z0("z0","z0",-0.1,1) ;
  RooPolynomial px("px","px",x,RooConst(0)) ;
  RooPolynomial py("py","py",y,RooConst(0)) ;
  RooPolynomial pz("pz","pz",z,z0) ;
  RooProdPdf pxyz("pxyz","pxyz",RooArgSet(px,py,pz)) ;



  // D e f i n e d   n o n - r e c t a n g u l a r   r e g i o n   R   i n   ( x , y , z )
  // -------------------------------------------------------------------------------------

  //
  // R = Z[0 - 0.1*Y^2] * Y[0.1*X - 0.9*X] * X[0 - 10]
  //

  // Construct range parameterized in "R" in y [ 0.1*x, 0.9*x ]
  RooFormulaVar ylo("ylo","0.1*x",x) ;
  RooFormulaVar yhi("yhi","0.9*x",x) ;
  y.setRange("R",ylo,yhi) ;

  // Construct parameterized ranged "R" in z [ 0, 0.1*y^2 ]
  RooFormulaVar zlo("zlo","0.0*y",y) ;
  RooFormulaVar zhi("zhi","0.1*y*y",y) ;
  z.setRange("R",zlo,zhi) ;



  // C a l c u l a t e   i n t e g r a l   o f   n o r m a l i z e d   p d f   i n   R
  // ----------------------------------------------------------------------------------

  // Create integral over normalized pdf model over x,y,z in "R" region
  RooAbsReal* intPdf = pxyz.createIntegral(RooArgSet(x,y,z),RooArgSet(x,y,z),"R") ;

  // Plot value of integral as function of pdf parameter z0
  RooPlot* frame = z0.frame(Title("Integral of pxyz over x,y,z in region R")) ;
  intPdf->plotOn(frame) ;


  regPlot(frame,"rf313_plot1") ;

  delete intPdf ;

  return true;
  }
} ;
