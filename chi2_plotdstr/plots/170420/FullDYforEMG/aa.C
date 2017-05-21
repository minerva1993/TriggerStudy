void aa(){
TFile *_file0 = TFile::Open("mc_TEST.root");
  TH1F *h0=(TH1F*)_file0->Get("hitChi2EE");
  TH1F *h1=(TH1F*)_file0->Get("hitChi2EB");

TCanvas *MyC=new TCanvas("MyC","chi square",600,600);
  MyC->SetLeftMargin(0.15);
  h0->GetXaxis()->SetRangeUser(0,30);
  h0->GetYaxis()->SetRangeUser(0,10000);
  h0->GetYaxis()->SetTitleOffset(2.0);
  h0->Draw();
  h0->SetStats(0);

  h1->Draw("Same");
  h1->SetLineColor(2);
  h1->SetStats(0);

  h0->SetTitle("chi2 for EE and EB;chi2;Events");

TLegend *leg = new TLegend(0.7,0.75,0.95,0.95);
  leg->AddEntry(h0,"hitChi2EE","l");
  leg->AddEntry(h1,"hitChi2EB","l");
  leg->AddEntry((TObject*)0,"n=100 in hlt run","");
  leg->Draw();

   

  MyC->Print("both.pdf");

  cout << "for EE" << endl;

  for (int i=1;i<16;i++){
      cout << "ratio of chi2 > " << i << " is " <<  h0->Integral(i,66) / h0->Integral(0,66) << endl;
  }; 

  cout << "for EB" << endl;

  for (int j=1;j<16;j++){
      cout << "ratio of chi2 > " << j << " is " <<  h1->Integral(j,66) / h1->Integral(0,66) << endl;
  }
}


