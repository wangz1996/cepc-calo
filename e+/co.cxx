class EnergyLoss
{
	TString rootname="ecal_e+_1GeV_0p25cm_10w.root";
	

	TH1D *hlayer[30];

        vector<int>  	*ecal_psdid;
        vector<double>  *ecal_energy;
	
	int N_layer=30;
	int N_x=42;
	int N_y=5;

public:
	EnergyLoss();
	void ReadData();
	void StoreProcessedData();		
};
EnergyLoss::EnergyLoss()
{
	ecal_psdid=0;
	ecal_energy=0;
	
	for(int i=0;i<N_layer;i++)
	{
		TString hlayername="hlayer_"+TString(to_string(i));
		TString hlayerTitle="ECAL Energy Loss Distribution of Layer_"+TString(to_string(i));
		hlayer[i]=new TH1D(hlayername,hlayerTitle,150,0,1.5);
	}
}
void EnergyLoss::ReadData()
{
	TFile *f=TFile::Open(rootname, "READ");
	TTree *t=(TTree*)f->Get("T");
        t->SetBranchAddress("ecal_psdid",&ecal_psdid);
        t->SetBranchAddress("ecal_energy",&ecal_energy);
	
	int N=t->GetEntries();

	for(int i=0;i<N;i++)
	{
		t->GetEntry(i);
		vector<double>  supEnergy(N_layer,0.);

		for(int j=0;j<ecal_psdid->size();j++)
		{
			int psdid=ecal_psdid->at(j);
			double energy=ecal_energy->at(j);
					
//			if(psdid%210==105)			
//				supEnergy[psdid/210]+=energy;
			hlayer[psdid/210]->Fill(energy);			
		
		}
/*		for(int k=0;k<N_layer;k++)
		{
			if(supEnergy[k]!=0)	
				hlayer[k]->Fill(supEnergy[k]);
		}
*/
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
