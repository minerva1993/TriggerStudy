void aa(){
TFile *_file0 = TFile::Open("mc_TEST.root");
//  TH1F *h0=(TH1F*)_file0->Get("hitChi2EE");
  TH1F *h1=(TH1F*)_file0->Get("hitChi2EB");

 TCanvas *MyC=new TCanvas("MyC","chi square",600,600);

  h1->GetXaxis()->SetRangeUser(0,50);
  h1->GetYaxis()->SetRangeUser(0,20000);
  h1->Draw();
//  h1->Draw("Same");

  MyC->Print("EB.pdf");

}

