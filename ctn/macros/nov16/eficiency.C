void eficiency() {


	//ROOT ENVIRONMENT
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(1);
	gStyle->SetOptFit(0);

	//cout<< "estoy aqui 1"<<endl;

	//HISTOGRAMS DEFINITION------------------------------------------------------------

	const Int_t n=19;
	TH1F* h [n]; // creamos un array de histogramas
	//TH1F* h   = new TH1F("h","HPGe Energy",8000,0.0,11.0);
	char namehist [250]; // creamos un char
	char titlehist [250]; 
	Double_t efy[n];
	Double_t x[n]={1., 1.5, 2., 2.5, 3., 3.5, 4., 4.5, 5., 5.5, 6., 6.5, 7., 7.5, 8., 8.5, 9., 9.5, 10.};

	//INPUT FILE --------------------------------------------------------------------- 
	for (Int_t k=0; k<n ; k++)	// loop to all generated files MODIFICADO SON 20!!
	{
	
		cout<<"HISTOGRAM: "<< k <<endl;

		//char inputFile[250] = sprintf(file1, "outsim_%d.root", i);
		char file1[250];
		sprintf(file1, "outsim_%d.root", k+1);
		TFile *file2 = TFile::Open(file1);

		sprintf(namehist, "histo_%d", k);      //crea el nombre histo_1, histo_2 ...
		sprintf(titlehist, "Photon Energy of %.2f MeV", x[k]);
		//format(x, '.2f')  .prec=2
		h[k] = new TH1F(namehist,titlehist,8000,0.0,11.0);// asignamos los nombres histo_1, histo_2... a cada histograma del array

		//READING TREE---------------------------------------------------------------------
		TTree* tree = (TTree*)file2->Get("ensartree");


		//cout<< "estoy aqui 3"<<endl;

		//Crystal Points (input)------------------------------------------------------------

		TClonesArray* hpgeHitCA;
		EnsarHPGeDetHit** hpgeHit;
		hpgeHitCA = new TClonesArray("EnsarHPGeDetHit",5);
		TBranch *branchEnsarHPGeDetHit = tree ->GetBranch("HPGeDetHit");
		branchEnsarHPGeDetHit->SetAddress(&hpgeHitCA );


		Int_t hpgeHitsPerEvent = 0;
		Double_t energy = 0.0;
		Int_t nfound = 0;
		Double_t xp = 0.0;


		//TREE ENTRIES--------------------------------------------------------------------
		Long64_t nevents = tree->GetEntries();


		//LOOP IN THE EVENTS--------------------------------------------------------------
		for(int i=0;i<nevents;i++)
		{
			hpgeHitCA->Clear();
			tree->GetEvent(i);

			hpgeHitsPerEvent = hpgeHitCA->GetEntries();

			//cout<< "estoy aqui 4"<<endl;

			if(hpgeHitsPerEvent>0) 
			{
			
				//cout<< "estoy aqui 5"<<endl;
				hpgeHit = new EnsarHPGeDetHit*[hpgeHitsPerEvent];
				for(Int_t j=0;j<hpgeHitsPerEvent;j++)
				{
			
					hpgeHit[j] = new EnsarHPGeDetHit;
					hpgeHit[j] = (EnsarHPGeDetHit*) hpgeHitCA->At(j);


					energy = hpgeHit[j]->GetEnergy()*1000; //energy MeV
					h[k]->Fill(energy); // rellenamos el array de histogramas	
					//cout<<"El valor de la energia es de: "<< energy <<endl;		
				}
			}
			if(hpgeHitsPerEvent) delete[] hpgeHit;	 
		}
		

			//-------------    AJUSTE GENERAL   --------------------------------------------------------------------------------	
			
			/*//Find the numer of peaks on each spectrum
			TSpectrum* spec= new TSpectrum(); 
			nfound = spec-> Search(h[k],5.9,"",0.8);
			//nfound = spec-> Search(h[k],2,"",0.05); by default
			printf("Found %d candidate peaks to fit\n",nfound);// numero de picos totales que encuentra en cada espectro


			//Get position on x axis for each peak
			Double_t *xpeaks = spec->GetPositionX(); // nos devuelve un array con la posicion de cada pico, si solo tiene un pico Dim=1, si tiene 3 Dim=3
			if(nfound==1) {
				xp = xpeaks[0];//solo existe un pico
			} else if(nfound==2) {
				xp = xpeaks[1];// hay dos picos, cojemos el último
			} else {
				xp = xpeaks[2];// hay tres picos, cojemos el último 
			}

			Int_t bin   = h[k]->GetXaxis()->FindBin(xp);  //bin position of the peak
			//Double_t yp = h[k]->GetBinContent(bin);       //number of counts in the bin of the peak
			printf("Found a useful peak to fit in %f MeV \n",xp);

			Int_t binX1 = h[k]->GetXaxis()->FindBin(xp-0.01*xp); //bin position of the gaussian extrem
			Int_t binX2 = h[k]->GetXaxis()->FindBin(xp+0.01*xp); //bin position of the gaussian extrem
			Double_t integral= h[k]->Integral (binX1, binX2);*/
			//------------------------------------------------------------------------------------------------------------------------			
			
			
			//--------------  AJUSTE PARA DIFERENTES ENRGÍAS (no va bien) ------------------------------------------------------------
			/*//Find the photopeak of each histogram
			TSpectrum* spec= new TSpectrum();
			
			
			if(k<11){
			
				nfound = spec-> Search(h[k],5.9,"",0.8);
				printf("Found %d candidate peaks to fit\n",nfound);
				Double_t *xpeaks = spec->GetPositionX();
				xp = xpeaks[0];
				
			} else if (k<15 && k>10){
			
				nfound = spec-> Search(h[k],5,"",0.4);
				printf("Found %d candidate peaks to fit\n",nfound);
				Double_t *xpeaks = spec->GetPositionX();
				xp = xpeaks[1];
			} else if (k>=15){
			
				nfound = spec-> Search(h[k],7,"",0.35);
				printf("Found %d candidate peaks to fit\n",nfound);
				Double_t *xpeaks = spec->GetPositionX();
				xp = xpeaks[2];
				if (k==16){xp = xpeaks[3];}
				if (k==18){xp = xpeaks[3];}
			} else{
				cout<<"error"<<endl;			
			}*/
			
			
			//------ AJUSTE GENERAL (seleccionando los photopicos)------------------------------------------------------------------
			
			
			TSpectrum* spec= new TSpectrum();
			nfound = spec-> Search(h[k],5,"",0.35);
			Double_t *xpeaks = spec->GetPositionX();
			
			for(Int_t i=0; i<nfound; i++)
			{
				if (  xpeaks[i]>(x[k]-(0.05*x[k]))  &&   xpeaks[i]<(x[k]+(0.05*x[k]))  ) 
				{
				   xp=xpeaks[i];
				}
			}
			
			
			
			Int_t bin   = h[k]->GetXaxis()->FindBin(xp);  //bin position of the peak
			Double_t yp = h[k]->GetBinContent(bin);       //number of counts in the bin of the peak
			printf("Found a useful peak to fit in %f MeV \n",xp);
			cout<< "The number of counts in this peak is:"<< yp <<endl;

			Int_t binX1 = h[k]->GetXaxis()->FindBin(xp-0.01*xp); //bin position of the gaussian extrem
			Int_t binX2 = h[k]->GetXaxis()->FindBin(xp+0.01*xp); //bin position of the gaussian extrem
			Double_t integral= h[k]->Integral (binX1, binX2);
			
			
			efy[k]=integral/100000;
			
		
	}

	cout<<"All photopeaks have been found!"<<endl;
	
	//-----Eficiency of photopeaks------------	
	
   	
   	        //Int_t K=0;
   		//for (Float_t i=1;i<=10;i=i+0.5) 
   		//{
   		//      cout << " K = " << K << " and i = " << i << endl;
     		//      y[K] = i;
   		//      K++;
    	 	//}

  	 TGraph *gr  = new TGraph(n,x,efy); // create eficiency graph



	//----- Draw histograms and efficiency-----
	TCanvas* c1 = new TCanvas("c1","Energy in the HPGe",0,0,400,400); // en el origen y CUADRADA
	c1->Divide(5,4);           // divide la canvas en 20 partes
	c1->SetFillColor(0);
	c1->SetFrameFillColor(0);

	for(Int_t j=0; j<n; j++)
	{

		c1->cd(j+1);
		h[j]->Draw();
		h[j]->GetXaxis()->SetTitle("Energy (MeV/c^{2})");
		h[j]->GetYaxis()->SetTitle("Counts");
		h[j]->SetLineColor(j);        // differents colours
		if(j==0) h[j]->SetLineColor(j+20);
		if(j==10) h[j]->SetLineColor(j+20);
	}
	
	c1->cd(20);
	gr->Draw("AC*");
	gr->GetXaxis()->SetTitle("Photon energy (MeV/c^{2})");
	gr->GetYaxis()->SetTitle("Efficiency");
	gr->SetLineColor(8); 


}

