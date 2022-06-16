{
	TCanvas *can=new TCanvas("can","can",100,100,700,700);

	gStyle->SetOptTitle(1);
	gStyle->SetOptStat(1);

	can->SetFillColor(0);
	can->SetGrid();
	Float_t small=1e-5;

	can->Divide(1,2,small,small);
	can->ToggleEventStatus();
	
	can->cd(1);
	ngem_ev->Draw("tof/1000000>>time(1500,0,150)");
	can->cd(2);
	ngem_ev->Draw("pixelX:pixelY>>tt(128,0,128,128,0,128)","","colz");

}