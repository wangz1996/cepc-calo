class EnergyLoss
{
	TString rootname="hcal_muon_3GeV_2cm_100w.root";
	

	TH1D *hlayer[40];

	vector<int>	*hcal_psdid;
	vector<double>	*hcal_energy;
	
	int N_layer=40;
	int N_x=18;
	int N_y=18;
	int N_chip=9;
	int N_channel=36;

public:
	EnergyLoss();
	void ReadData();
	void StoreProcessedData();		
};
EnergyLoss::EnergyLoss()
{
	hcal_psdid=0;
	hcal_energy=0;

	for(int i=0;i<N_layer;i++)
	{
		TString hlayername="hlayer_"+TString(to_string(i));
		TString hlayerTitle="HCAL Energy Loss Distribution of Layer_"+TString(to_string(i));
		hlayer[i]=new TH1D(hlayername,hlayerTitle,1500,0,1.5);
	}
}
void EnergyLoss::ReadData()
{
	TFile *f=TFile::Open(rootname, "READ");
	TTree *t=(TTree*)f->Get("T");
	t->SetBranchAddress("hcal_psdid",&hcal_psdid);
	t->SetBranchAddress("hcal_energy",&hcal_energy);
		
	int N=t->GetEntries();
	for(int i=0;i<N;i++)
	{
		t->GetEntry(i);
		vector<double>	supEnergy(N_layer,0.);
		int count=0;	
		for(int j=0;j<hcal_psdid->size();j++)
		{
			int psdid=hcal_psdid->at(j);
			double energy=hcal_energy->at(j);

			if(psdid%10000==909)
				supEnergy[psdid/10000]+=energy;
		}
		for(int k=0;k<N_layer;k++)
		{
			if(supEnergy[k]!=0)
				hlayer[k]->Fill(supEnergy[k]);
		}
	}				
}
void EnergyLoss::StoreProcessedData()
{
	TFile *f=new TFile("output_"+rootname,"RECREATE");


	TString dirname0="EnergyPerLayer";
	f->mkdir(dirname0);
	
	for(int i=0;i<N_layer;i++)
	{
	
		TString dirname1=dirname0+"/layer_";
		dirname1+=to_string(i);
		f->mkdir(dirname1);
		f->cd(dirname1);
		
		hlayer[i]->Write();
	}
	f->Close();
}
void co()
{
	EnergyLoss el;
	el.ReadData();
	el.StoreProcessedData();
}
