void seedata()
{
	TFile *f=TFile::Open("muon_3GeV_10w_2cm.root","READ");
	
	TTree *t=(TTree*)f->Get("T");

	vector<double>	*Hit_X=0;
	vector<double>	*Hit_Y=0;
	vector<double>  *Hit_Z=0;
	vector<int>  *PDGID=0;
	vector<int>  *TrackID=0;
	vector<double>	*Hit_Time=0;
	vector<double>	*Hit_Energy=0;
	vector<int>	*Hit_PSDID=0;
	t->SetBranchAddress("Hit_X",&Hit_X);
	t->SetBranchAddress("Hit_Y",&Hit_Y);
	t->SetBranchAddress("Hit_Z",&Hit_Z);
	t->SetBranchAddress("PDGID",&PDGID);
	t->SetBranchAddress("TrackID",&TrackID);
	t->SetBranchAddress("Hit_Time",&Hit_Time);
	t->SetBranchAddress("Hit_Energy",&Hit_Energy);
	t->SetBranchAddress("Hit_PSDID",&Hit_PSDID);
	t->GetEntry(0);
//	cout<<Hit_X->size()<<endl<<Hit_Y->size()<<endl<<Hit_Z->size()<<endl<<
//	PDGID->size()<<endl<<Hit_PSDID->size()<<endl<<Hit_Energy->size();
	t->GetEntry(34434);
	for(int j=0;j<Hit_Energy->size();j++)	
		cout<<j<<":  "<<
			Hit_X->at(j)<<" "<<Hit_Y->at(j)<<"  "<<Hit_Z->at(j)<<" "<<Hit_PSDID->at(j)<<".........."
				<<PDGID->at(j)<<" "<<TrackID->at(j)<<" "
					<<Hit_Time->at(j)<<" "<<Hit_Energy->at(j)<<endl;

	f->Close();	
}
