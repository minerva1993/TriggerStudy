void aa(){
TFile *_file0 = TFile::Open("mc_TEST.root");
  TH1F *h0=(TH1F*)_file0->Get("hitChi2EE");
  TH1F *h1=(TH1F*)_file0->Get("hitChi2EB");

TCanvas *MyC=new TCanvas("MyC","chi square",600,600);

  h0->GetXaxis()->SetRangeUser(0,30);
  h0->GetYaxis()->SetRangeUser(0,20000);
  h0->Draw();
  h0->SetStats(0);

  h1->Draw("Same");
  h1->SetLineColor(2);
  h1->SetStats(0);

  h1->SetTitle("chi2 for EE and EB;chi2;Events");

TLegend *leg = new TLegend(0.7,0.75,0.9,0.9);
  leg->AddEntry(h0,"hitChi2EE","l");
  leg->AddEntry(h1,"hitChi2EB","l");
  leg->AddEntry((TObject*)0,"n=100 in hlt run","");
  leg->Draw();

  MyC->Print("both.pdf");

cout << "for EE" << endl;
cout << "ratio of chi2 > 1 is " <<  h0->Integral(1,66) / h0->Integral(0,66) << endl;
cout << "ratio of chi2 > 2 is " <<  h0->Integral(2,66) / h0->Integral(0,66) << endl;
cout << "ratio of chi2 > 5 is " <<  h0->Integral(5,66) / h0->Integral(0,66) << endl;
cout << "ratio of chi2 > 7 is " <<  h0->Integral(7,66) / h0->Integral(0,66) << endl;
cout << "ratio of chi2 > 10 is " <<  h0->Integral(10,66) / h0->Integral(0,66) << endl;
cout << "for EB" << endl;
cout << "ratio of chi2 > 1 is " <<  h1->Integral(1,66) / h1->Integral(0,66) << endl;
cout << "ratio of chi2 > 2 is " <<  h1->Integral(2,66) / h1->Integral(0,66) << endl;
cout << "ratio of chi2 > 5 is " <<  h1->Integral(5,66) / h1->Integral(0,66) << endl;
cout << "ratio of chi2 > 7 is " <<  h1->Integral(7,66) / h1->Integral(0,66) << endl;
cout << "ratio of chi2 > 10 is " <<  h1->Integral(10,66) / h1->Integral(0,66) << endl;
cout << "ratio of chi2 > 15 is " <<  h1->Integral(15,66) / h1->Integral(0,66) << endl;

}


