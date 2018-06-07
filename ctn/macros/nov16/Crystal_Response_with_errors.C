{
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////																		    
////		--- Simulation of the Lisbon Nov-2016 setup ---								
////														      	
////		This Macro analyze the Crystal Response of the Petals.				 				
////																       	
////		It studies the ratio of a correlated and an isotropic distribution of gammas in simulation					
////		versus the polar angle (theta) for each Crystal.													
////		The Crystals could be identified by their CrystalId:											
////				-Petal at 180º w.r.t.HPGe: CryId from 1 to 64											
////				-Petal at 90º w.r.t.HPGe: CryId from 65 to 128											
////		Then, it calculates the Normalized theoretical angular correlations function					
////		to check the agreement between them.															
////																	     
////		Usage:																							
////			1st: (OPTIONAL) change the data files & data file paths,									
////				  A2&A4 AngularCorrelations coefficients, Normalization of the function					
////			2nd: root -l Crystal_Response.C																
////															  	
////		This Macro reads:																				
////		 -the CryId and its Theta angle from the CrystalId_Theta.dat data file							
////		 -Num.gammas for each CryId of an isotropic distribution from the NumGammas_Cry_isotropic.dat	
////		 -Num.gammas for each CryId of an AngularCorr distribution from the NumGammas_Cry_AngularC.dat						
////															 	
////		Calculate:																						
////		 -Ratio gammas R= Num.gammas isotropic / Num.gammas with AngularCorr, for each CrystalId		
////		 -Normalized theoretical angular correlations function	
////		 -The relative error of the Ratio of gammas for each CryId												
////																	   
////		Graph representation: Theta angle vs Ratio gammas for each CrystalId							
////				      Normalized theoretical angular correlations function						
////																		     
//// **elisabet.galiana@usc.es																				
//// ** Universidad de Santiago de Compostela																
//// ** Dpto. Física de Partículas 																			
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  gROOT->SetStyle("Plain");
  gStyle->SetOptFit(111);
  gStyle->SetPalette(1);
  
  //OPEN & READ DATA FILES ------------------------------------
  
  //FILE 1 CrystalId and Theta angle
  ifstream *File1 = new ifstream("/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/nov16/CrystalId_Theta.dat");//change data file path
  
  const Int_t nLevel=128;
  Double_t CryId[nLevel];
  Double_t Theta[nLevel];
  
  
  File1->ignore(256,'\n');
  File1->ignore(256,'\n');
  File1->ignore(256,'\n');
  File1->ignore(256,'\n');
  
  for(Int_t i = 0; i < nLevel; i++){ 
    *File1 >> CryId[i] >> Theta[i];
    cout<<"CryId= "<< CryId[i]<<"  Theta= " << Theta[i]<<endl;
  }
  //fclose(File1);
  
  //FILE 2 Number of gammas have been arrived to each Crystal -> Isotropic distribution
  ifstream *File2 = new ifstream("/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/nov16/Co_Numgam_Iso.dat");//change data file path
  
  Double_t CryId2[nLevel];
  Double_t Gammas_iso[nLevel];
  
  
  File2->ignore(256,'\n');
  File2->ignore(256,'\n');
  File2->ignore(256,'\n');
  File2->ignore(256,'\n');
  
  for(Int_t i = 0; i < nLevel; i++){ 
    *File2 >> CryId2[i] >> Gammas_iso[i];
  }
  
  //FILE 3 Number of gammas have been arrived to each Crystal -> Angular Correlated distribution
  ifstream *File3 = new ifstream("/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/nov16/Co_Numgam_AngCor.dat");//change data file path

  Double_t CryId3[nLevel];
  Double_t Gammas_W[nLevel];
  
  
  File3->ignore(256,'\n');
  File3->ignore(256,'\n');
  File3->ignore(256,'\n');
  File3->ignore(256,'\n');
  
  for(Int_t i = 0; i < nLevel; i++){ 
    *File3 >> CryId3[i] >> Gammas_W[i];
  }
  
  
  //CALCULATION -----------------------------------------------------------------------------------------------------------
  //Ratio of gammas
  Double_t Gammas[nLevel];
  Double_t x, y, frac;
  x=0.;
  y=0.;
  Ratio=0.;
   
  for(Int_t i = 0; i < nLevel; i++){ 
    
    x= Gammas_iso[i];
    y= Gammas_W[i];
    Ratio=y/x;
    Gammas[i]=Ratio;
    
    cout<<"y="<<y<<" x="<<x<<endl;
    cout<<"Ratio= "<<Ratio<<endl;
  }
  
  //Ratio of gamma Error
  Double_t  Error_Gammas[nLevel];
  Double_t error_iso;
  Double_t error_W;
  Double_t W, iso, a, b;
  W=0.;
  iso=0.;
  a=0.;
  b=0.;
  
  for(Int_t i = 0; i < nLevel; i++){ 
    
    error_iso= sqrt(Gammas_iso[i]);
    error_W= sqrt(Gammas_W[i]);
    iso= Gammas_iso[i];
    W= Gammas_W[i];
    
    a= pow(((error_iso*W)/pow(iso,2)),2);
    b= pow(error_W/iso ,2);
    Error_Gammas[i]=(sqrt(a+b))/Gammas[i];
    
    cout<<"Error_R= "<<Error_Gammas[i]<<endl;
  }
  
  //FUNCTION ---------------------------------------------------------------------------------------------------------------
  //The Theoretical Angular Correlations function is	W=1+A2cos^2(theta)+A4cos^4(theta)
  // 						        where the A2, A4 Angular Correlations coefficients depend on 
  // 							the spin-parity of the transition in the gamma cascade
  
  //Normalized theoretical angular correlations function
  TF1 *fa1 = new TF1("fa1","(1+[0]*pow((TMath::Cos(x+ TMath::Pi()/2)),2)+[1]*pow((TMath::Cos(x+ TMath::Pi()/2)),4))/[2]",0,2.15);
  fa1->SetParameter(0, 0.95); //change 	A2, this parameter represents the A2,A4 Angular Correlations coefficients
  fa1->SetParameter(1, 0.95);  //change	A4
	fa1->SetParameter(2, 1.50667);  //change		normalization parameter
  
  
  //CHANGE THETA UNITS grad->rad 
  Double_t Theta_rad[nLevel];
  for(Int_t i = 0; i < nLevel; i++){ 
    Double_t T_rad, T_grad;
    T_grad=0.;
    T_rad=0.;
    
    T_grad=Theta[i];
    T_rad =T_grad*TMath::Pi()/180.;
	 	Theta_rad[i]=T_rad;
  }
  
  
    
  //GRAPH
  TGraph *gr = new TGraph(nLevel,Theta_rad,Gammas);
  gr->SetLineColor(2);
  gr->SetLineWidth(4);
  gr->SetMarkerColor(4);
  //gr->SetMarkerStyle(21);
  gr->SetTitle("Ratio gammas vs Theta angle for each CryId");
  gr->GetXaxis()->SetTitle("Theta (rad)");
  gr->GetYaxis()->SetTitle("Counts Gamma_W/Gamma_iso");
  
  //CANVAS
  TCanvas *c1 = new TCanvas("c1","A Simple",200,10,700,500);
  c1->SetFillColor(0);       
  c1->SetFrameFillColor(0); 
  gr->Draw("A*");
  fa1->Draw("same");
  
  TLegend *leg = new TLegend(0.5,0.7,0.8,0.8);
  leg->AddEntry(fa1, "Theoretical function", "l");
  leg->SetFillColor(0);
  leg->Draw();
  leg->AddEntry(gr, "Ratio gammas its error is the order of points", "p*");
  
}
