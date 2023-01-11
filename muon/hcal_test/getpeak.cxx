class Fitting
{
	TString fname="output_hcal_muon_3GeV_2cm_100w.root";
	TH1D *hEnLoss;
	TH1D *hlayer[40];

	int N_layer=40;	
	
public:
	Fitting();
	void SethEnLoss(TH1D *hEnLoss);
	void Sethlayer(TH1D *hlayer);
	void Plotting();
};
Fitting::Fitting()
{
	vector<double> *peakEnergy=new vector<double>(40);
	hEnLoss=new TH1D("hEnLoss","hEnLoss",40,-0.5,39.5);	
	SethEnLoss(hEnLoss);
	
	TFile *f=TFile::Open(fname, "READ");
	TDirectory *dir=(TDirectory*)f->Get("EnergyPerLayer");
	dir->cd();
	for(int i=0;i<N_layer;i++)
	{
		TString dirname=TString("layer_")+TString(to_string(i));
		TString histname=TString("hlayer_")+TString(to_string(i));
		cout<<histname<<endl;
		
		TDirectory *dirlayer=(TDirectory*)dir->Get(dirname);
		dirlayer->cd();
		TH1D *h=(TH1D*)dirlayer->Get(histname);
		hlayer[i]=(TH1D*)h->Clone();                                                     
                hlayer[i]->SetDirectory(0);                                     
                Sethlayer(hlayer[i]);	
		
		peakEnergy->at(i)=h->GetMaximumBin()*h->GetBinWidth(0);
	}

	double minpeak=*min_element(peakEnergy->begin(),peakEnergy->end());
	for(int i=0;i<N_layer;i++)
		hEnLoss->Fill(i,peakEnergy->at(i)/minpeak);
	
	f->Close();	
}

void Fitting::SethEnLoss(TH1D *hEnLoss)
{
	hEnLoss->SetTitle("AHCAL Energy Loss per Layer of 3GeV #mu+");
        hEnLoss->GetXaxis()->SetTitle("Layer");
        hEnLoss->GetYaxis()->SetTitle("Peak Energy[MIP]");
        hEnLoss->GetXaxis()->CenterTitle();
        hEnLoss->GetYaxis()->CenterTitle();
        hEnLoss->GetXaxis()->SetTitleSize(0.04);
        hEnLoss->GetXaxis()->SetTitleFont(32);
        hEnLoss->GetYaxis()->SetTitleSize(0.04);
        hEnLoss->GetYaxis()->SetTitleFont(30);

	hEnLoss->SetAxisRange(0.9,1.2,"y");
      
	hEnLoss->SetMarkerStyle(21);
        hEnLoss->SetMarkerColor(kGreen+1);
        hEnLoss->SetMarkerSize(1);
 
	hEnLoss->SetLineWidth(0);
        hEnLoss->SetLineColor(kBlue);
}

void Fitting::Sethlayer(TH1D *hlayer)
{
	hlayer->StatOverflows(kTRUE);

        hlayer->SetFillColor(kCyan-4);
        hlayer->GetYaxis()->SetTitle("Frequency");
        hlayer->GetXaxis()->SetTitle("Energy Loss[MeV]");
        hlayer->GetXaxis()->CenterTitle();
        hlayer->GetYaxis()->CenterTitle();

        hlayer->SetTitleFont(32,"xy");
        hlayer->SetTitleSize(0.04,"xy");
        hlayer->SetLineWidth(0);

}
void Fitting::Plotting()
{
	auto c=new TCanvas("c","c",1500,1000);
	gStyle->SetErrorX(0);
	gStyle->SetOptStat("");

	c->SetGrid();
        c->Divide(2,2);

        c->cd(1);
        hEnLoss->Draw();	
	TLatex *text=new TLatex(20,1.1,"AHCAL");
	text->SetTextSize(0.15);
    	text->Draw("Same");	

	c->cd(2);
        hlayer[0]->Draw();
        
	c->cd(3);
        hlayer[19]->Draw();
//	pfit->Print();

        c->cd(4);
        hlayer[39]->Draw();
//	pfit->Print();
	c->Print("AHCALgetPeak.jpg");
}
void getpeak()
{
	Fitting f;
	f.Plotting();
}
