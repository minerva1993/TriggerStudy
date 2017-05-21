void chi2_QCD50to80(){
TFile *_file0 = TFile::Open("mc_TEST_QCD50to80.root");
  TH1F *h0=(TH1F*)_file0->Get("hitChi2EE");
  TH1F *h1=(TH1F*)_file0->Get("hitChi2EB");

TCanvas *MyC=new TCanvas("MyC","chi square",400,400);
  MyC->SetLeftMargin(0.12);
  MyC->SetTopMargin(0.12);
  h0->GetXaxis()->SetRangeUser(0,50);
  h0->GetYaxis()->SetRangeUser(0,130000);
  h0->GetYaxis()->SetTitleOffset(1.3);
  h0->Draw();
  h0->SetStats(0);

  h1->Draw("Same");
  h1->SetLineColor(2);
  h1->SetStats(0);

  h0->SetTitle("chi2 for EE and EB (QCD, pt 50 to 80);chi2;Events");

TLegend *leg = new TLegend(0.67,0.77,0.93,0.91);
//  leg->SetHeader("DYJetsToLL","C");
  leg->AddEntry(h0,"hitChi2EE","l");
  leg->AddEntry(h1,"hitChi2EB","l");
  leg->AddEntry((TObject*)0,"n=1000 in hlt run","");
  leg->SetTextSize(0.03);
  leg->Draw();

   

  MyC->Print("plot_QCD50to80.pdf");

  cout << "for EE" << endl;

  for (int i=1;i<16;i++){
      cout << "ratio of chi2 > " << i << " is " <<  h0->Integral(i,66) / h0->Integral(0,66) << endl;
  }; 

  cout << "for EB" << endl;

  for (int j=1;j<16;j++){
      cout << "ratio of chi2 > " << j << " is " <<  h1->Integral(j,66) / h1->Integral(0,66) << endl;
  }
}


