void chi2_EEEB(){

//read files
TFile *_fileDY = TFile::Open("mc_TEST_DY.root");
TFile *_fileTT = TFile::Open("mc_TEST_TT.root");
TFile *_file15 = TFile::Open("mc_TEST_QCD15to30.root");
TFile *_file30 = TFile::Open("mc_TEST_QCD30to50.root");
TFile *_file50 = TFile::Open("mc_TEST_QCD50to80.root");
TFile *_file80 = TFile::Open("mc_TEST_QCD80to120.root");
TFile *_file120 = TFile::Open("mc_TEST_QCD120to170.root");
//TFile *_fileFull = TFile::Open("mc_TEST_QCDFull.root");

//read histograms
TH1F *hDYEE=(TH1F*)_fileDY->Get("hitChi2EE");
TH1F *hDYEB=(TH1F*)_fileDY->Get("hitChi2EB");

TH1F *hTTEE=(TH1F*)_fileTT->Get("hitChi2EE");
TH1F *hTTEB=(TH1F*)_fileTT->Get("hitChi2EB");

TH1F *h15EE=(TH1F*)_file15->Get("hitChi2EE");
TH1F *h15EB=(TH1F*)_file15->Get("hitChi2EB");

TH1F *h30EE=(TH1F*)_file30->Get("hitChi2EE");
TH1F *h30EB=(TH1F*)_file30->Get("hitChi2EB");

TH1F *h50EE=(TH1F*)_file50->Get("hitChi2EE");
TH1F *h50EB=(TH1F*)_file50->Get("hitChi2EB");

TH1F *h80EE=(TH1F*)_file80->Get("hitChi2EE");
TH1F *h80EB=(TH1F*)_file80->Get("hitChi2EB");

TH1F *h120EE=(TH1F*)_file120->Get("hitChi2EE");
TH1F *h120EB=(TH1F*)_file120->Get("hitChi2EB");

TCanvas *EE_C=new TCanvas("EE_C","chi2 EE",400,400);
  EE_C->cd();
  EE_C->SetLeftMargin(0.15);
  EE_C->SetTopMargin(0.1);
  hDYEE->GetXaxis()->SetRangeUser(0,50);
  hDYEE->GetYaxis()->SetRangeUser(0,40000);
  hDYEE->GetYaxis()->SetTitleOffset(1.9);
  hDYEE->Draw();
  hDYEE->SetLineColor(1);
  hDYEE->SetStats(0);

  hTTEE->Draw("Same");
  hTTEE->SetLineColor(2);
  hTTEE->SetStats(0);

  h15EE->Draw("Same");
  h15EE->SetLineColor(3);
  h15EE->SetStats(0);

  h30EE->Draw("Same");
  h30EE->SetLineColor(4);
  h30EE->SetStats(0);

  h50EE->Draw("Same");
  h50EE->SetLineColor(8);
  h50EE->SetStats(0);

  h80EE->Draw("Same");
  h80EE->SetLineColor(6);
  h80EE->SetStats(0);

  h120EE->Draw("Same");
  h120EE->SetLineColor(7);
  h120EE->SetStats(0);

  hDYEE->SetTitle("chi2 for EE;chi2;Events");

TLegend *legEE = new TLegend(0.65,0.60,0.93,0.91);
  legEE->AddEntry(hDYEE,"DY","l");
  legEE->AddEntry(hTTEE,"TT","l");
  legEE->AddEntry(h15EE,"QCD Pt 15to30","l");
  legEE->AddEntry(h30EE,"QCD Pt 30to50","l");
  legEE->AddEntry(h50EE,"QCD Pt 50to80","l");
  legEE->AddEntry(h80EE,"QCD Pt 80to120","l");
  legEE->AddEntry(h120EE,"QCD Pt 120to170","l");
//  legEE->AddEntry((TObject*)0,"n=1000 in hlt run","");
  legEE->SetTextSize(0.03);
  legEE->Draw();

  EE_C->Print("plot_EE.pdf");

////////////

TCanvas *EB_C=new TCanvas("EB_C","chi2 EB",400,400);
  EB_C->cd();
  EB_C->SetLeftMargin(0.15);
  EB_C->SetTopMargin(0.1);
  hDYEB->GetXaxis()->SetRangeUser(0,50);
  hDYEB->GetYaxis()->SetRangeUser(0,200000);
  hDYEB->GetYaxis()->SetTitleOffset(1.9);
  hDYEB->Draw();
  hDYEB->SetLineColor(1);
  hDYEB->SetStats(0);

  hTTEB->Draw("Same");
  hTTEB->SetLineColor(2);
  hTTEB->SetStats(0);

  h15EB->Draw("Same");
  h15EB->SetLineColor(3);
  h15EB->SetStats(0);

  h30EB->Draw("Same");
  h30EB->SetLineColor(4);
  h30EB->SetStats(0);

  h50EB->Draw("Same");
  h50EB->SetLineColor(8);
  h50EB->SetStats(0);

  h80EB->Draw("Same");
  h80EB->SetLineColor(6);
  h80EB->SetStats(0);

  h120EB->Draw("Same");
  h120EB->SetLineColor(7);
  h120EB->SetStats(0);

  hDYEB->SetTitle("chi2 for EB;chi2;Events");

TLegend *legEB = new TLegend(0.65,0.60,0.93,0.91);
  legEB->AddEntry(hDYEB,"DY","l");
  legEB->AddEntry(hTTEB,"TT","l");
  legEB->AddEntry(h15EB,"QCD Pt 15to30","l");
  legEB->AddEntry(h30EB,"QCD Pt 30to50","l");
  legEB->AddEntry(h50EB,"QCD Pt 50to80","l");
  legEB->AddEntry(h80EB,"QCD Pt 80to120","l");
  legEB->AddEntry(h120EB,"QCD Pt 120to170","l");
//  legEE->AddEntry((TObject*)0,"n=1000 in hlt run","");
  legEB->SetTextSize(0.03);
  legEB->Draw();

    EB_C->Print("plot_EB.pdf");
/////DY
  cout << "for DYEE" << endl;

  for (int i=1;i<16;i++){
      cout << "ratio of chi2 > " << i << " is " <<  hDYEE->Integral(i,66) / hDYEE->Integral(0,66) << endl;
  }; 

  cout << "for DYEB" << endl;

  for (int j=1;j<16;j++){
      cout << "ratio of chi2 > " << j << " is " <<  hDYEB->Integral(j,66) / hDYEB->Integral(0,66) << endl;
  }

/////TT

  cout << "for TTEE" << endl;

  for (int i=1;i<16;i++){
      cout << "ratio of chi2 > " << i << " is " <<  hTTEE->Integral(i,66) / hTTEE->Integral(0,66) << endl;
  };

  cout << "for TTEB" << endl;

  for (int j=1;j<16;j++){
      cout << "ratio of chi2 > " << j << " is " <<  hTTEB->Integral(j,66) / hTTEB->Integral(0,66) << endl;
  }


/////15
  cout << "for 15EE" << endl;

  for (int i=1;i<16;i++){
      cout << "ratio of chi2 > " << i << " is " <<  h15EE->Integral(i,66) / h15EE->Integral(0,66) << endl;
  };

  cout << "for 15EB" << endl;

  for (int j=1;j<16;j++){
      cout << "ratio of chi2 > " << j << " is " <<  h15EB->Integral(j,66) / h15EB->Integral(0,66) << endl;
  }

/////30
  cout << "for 30EE" << endl;

  for (int i=1;i<16;i++){
      cout << "ratio of chi2 > " << i << " is " <<  h30EE->Integral(i,66) / h30EE->Integral(0,66) << endl;
  };

  cout << "for 30EB" << endl;

  for (int j=1;j<16;j++){
      cout << "ratio of chi2 > " << j << " is " <<  h30EB->Integral(j,66) / h30EB->Integral(0,66) << endl;
  }


/////50
  cout << "for 50EE" << endl;

  for (int i=1;i<16;i++){
      cout << "ratio of chi2 > " << i << " is " <<  h50EE->Integral(i,66) / h50EE->Integral(0,66) << endl;
  };

  cout << "for 50EB" << endl;

  for (int j=1;j<16;j++){
      cout << "ratio of chi2 > " << j << " is " <<  h50EB->Integral(j,66) / h50EB->Integral(0,66) << endl;
  }

/////80
  cout << "for 80EE" << endl;

  for (int i=1;i<16;i++){
      cout << "ratio of chi2 > " << i << " is " <<  h80EE->Integral(i,66) / h80EE->Integral(0,66) << endl;
  };

  cout << "for 80EB" << endl;

  for (int j=1;j<16;j++){
      cout << "ratio of chi2 > " << j << " is " <<  h80EB->Integral(j,66) / h80EB->Integral(0,66) << endl;
  }

/////120
  cout << "for 120EE" << endl;

  for (int i=1;i<16;i++){
      cout << "ratio of chi2 > " << i << " is " <<  h120EE->Integral(i,66) / h120EE->Integral(0,66) << endl;
  };

  cout << "for 120EB" << endl;

  for (int j=1;j<16;j++){
      cout << "ratio of chi2 > " << j << " is " <<  h120EB->Integral(j,66) / h120EB->Integral(0,66) << endl;
  }



}
