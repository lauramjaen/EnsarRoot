void combi_hist(){
char histoA[100];
char histoB[100];
char histoC[100];
char title[150];
char name[150];

TCanvas *c1 = new TCanvas("c1","Effect of the area on the background simulation",200,10, 600,400);
c1->cd();
//c1->Divide(1,4);

//ROOT A
sprintf(histoA,"h3A"); //name of the histogram 
TFile *fA = new TFile ("Background_A.root");//name root
TH1F * histA = (TH1F*)fA->Get(histoA);
c1->cd();
histA->SetLineColor(kOrange);
//hist1->SetAxisRange(300, 1000, "X");
//sprintf(title,"", E_p, CryId);
//hist1->SetTitle(title);
histA->Draw("");

//ROOT B
sprintf(histoB,"h3B");
TFile *fB=new TFile("Background_B.root");
TH1F* histB = (TH1F*)fB->Get(histoB);
c1->cd();
histB->SetLineColor(kViolet);
histB->Draw("same");

//ROOT C
sprintf(histoC,"h3C");
TFile *fC=new TFile("Background_C.root");
TH1F* histC = (TH1F*)fC->Get(histoC);
c1->cd();
histC->SetLineColor(kGreen);
histC->Draw("same");

auto legend1 = new TLegend(0.70,0.40,0.89,0.9);
legend1->SetHeader("Comparison of backgrounds"); // option "C" allows to center the header
legend1->AddEntry(histA,"background areaA (15x15)","L");
legend1->AddEntry(histB,"background areaB (20x20)","L");
legend1->AddEntry(histC,"background areaC (10.4x10.4)","L");
legend1->Draw();

sprintf(name,"background_in_areas.pdf");
c1->SaveAs(name);
}
