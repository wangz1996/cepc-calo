class Fitting
{
	TString fname="output_hcal_muon_3GeV_2cm_100w.root";
	TF1 *ffit[40];
	TH1D *hEnLoss;
	TH1D *hlayer[40];
	TFitResultPtr pfit[40];

	int N_layer=40;	
	
public:
	Fitting();
	void Setffit(TF1 *ffit);	
	void SethEnLoss(TH1D *hEnLoss);
	void Sethlayer(TH1D *hlayer);
	TLegend *legend(TH1D *data,TF1 *fit);
	void Plotting();
};
Fitting::Fitting()
{
	vector<double>  MPV(40);
        vector<double>  Sigma(40);

	hEnLoss=new TH1D("hEnLoss","hEnLoss",40,-0.5,39.5);	
	SethEnLoss(hEnLoss);
	
	TFile *f=TFile::Open(fname, "READ");
	TDirectory *dir=(TDirectory*)f->Get("EnergyPerLayer");
	dir->cd();
	for(int i=0;i<N_layer;i++)
	{
		TString dirname=TString("layer_")+TString(to_string(i));
		TString histname=TString("hlayer_")+TString(to_string(i));
		TString fitname=TString("ffit_")+TString(to_string(i));		
		cout<<histname<<endl;
		
		ffit[i]=new TF1(fitname,"[0]*TMath::Landau(x,[1],[2])");
                Setffit(ffit[i]);

		TDirectory *dirlayer=(TDirectory*)dir->Get(dirname);
		dirlayer->cd();
		TH1D *h=(TH1D*)dirlayer->Get(histname);
		hlayer[i]=(TH1D*)h->Clone();
		hlayer[i]->SetDirectory(0);
		Sethlayer(hlayer[i]);

//		pfit[i]=hlayer[i]->Fit(ffit[i],"QS","",0.4,1.5);
		pfit[i]=hlayer[i]->Fit(ffit[i],"QS");
	
		double EnMPV=ffit[i]->GetParameter(1);
                double EnSigma=ffit[i]->GetParameter(2);
		
		cout<<EnMPV<<endl;
		MPV[i]=EnMPV;	
		Sigma[i]=ffit[i]->GetParError(1);;


	}
	double MIP=*min_element(MPV.begin(),MPV.end());
//	double MIP=1;
	for(int i=0;i<N_layer;i++)
	{
		hEnLoss->SetBinContent(i+1,MPV[i]/MIP);
        	hEnLoss->SetBinError(i+1,Sigma[i]/MIP);
	}
	
	f->Close();	
}
void Fitting::Setffit(TF1 *ffit)
{
	ffit->SetParNames("Norm","MPV","#sigma");
	ffit->SetLineColor(kViolet+0);
	ffit->SetLineWidth(2);
        ffit->SetParameter(0,1500);
        ffit->SetParameter(1,0.47);
        ffit->SetParameter(2,0.03);
}

void Fitting::SethEnLoss(TH1D *hEnLoss)
{
	hEnLoss->SetTitle("AHCAL Energy Loss per Layer of 3GeV #mu+");
        hEnLoss->GetXaxis()->SetTitle("Layer");
        hEnLoss->GetYaxis()->SetTitle("MPV of Landau Fitting[MIP]");
        hEnLoss->GetXaxis()->CenterTitle();
        hEnLoss->GetYaxis()->CenterTitle();
        hEnLoss->GetXaxis()->SetTitleSize(0.04);
        hEnLoss->GetXaxis()->SetTitleFont(32);
        hEnLoss->GetYaxis()->SetTitleSize(0.04);
        hEnLoss->GetYaxis()->SetTitleFont(30);
        hEnLoss->SetAxisRange(0.99,1.02,"y");
        
	hEnLoss->SetMarkerStyle(21);
        hEnLoss->SetMarkerColor(kGreen+1);
        hEnLoss->SetMarkerSize(1);
 
	hEnLoss->SetLineWidth(4);
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
TLegend *Fitting::legend(TH1D *data,TF1 *fit)
{
	TLegend *leg=new TLegend(.6,.6,.8,.8);
        leg->SetFillColor(0);
        fit->SetFillColor(0);
        leg->AddEntry(fit,"Fitted Landau");
        leg->AddEntry(data,"Simulation Results");
	return leg;
}
void Fitting::Plotting()
{
	auto c=new TCanvas("c","c",1500,1000);
	gStyle->SetErrorX(0);
	gStyle->SetOptStat("");
	gStyle->SetOptFit(0);
	gStyle->SetStatBorderSize(0);
        gStyle->SetStatX(.89); 
	gStyle->SetStatY(.49);
	gStyle->SetStatFont(32);	

	gStyle->SetLegendBorderSize(0);
	gStyle->SetLegendTextSize(0.04);
	gStyle->SetLegendFont(32);	

	c->SetGrid();
        c->Divide(2,2);

        c->cd(1);
        hEnLoss->Draw();	
	TLatex *latex=new TLatex(22,1.01,"AHCAL");
        latex->SetTextSize(0.15);
        latex->Draw("Same");
	
	c->cd(2);
        hlayer[0]->Draw();
	legend(hlayer[0],ffit[0])->Draw("Same");
        
	c->cd(3);
        hlayer[19]->Draw();
	legend(hlayer[19],ffit[19])->Draw("Same");
//	pfit->Print();

        c->cd(4);
        hlayer[39]->Draw();
	legend(hlayer[0],ffit[39])->Draw("Same");
//	pfit->Print();
	c->Print("AHCALgetMPV.jpg");
}
void fitting()
{
	Fitting f;
	f.Plotting();
}
