{
   //
   // To see the output of this macro, click begin_html <a href="gif/graph.gif">here</a>. end_html
   //
   gROOT->Reset();
   c1 = new TCanvas("c1","A Simple Graph Example",200,10,700,500);

   c1->SetFillColor(42);
   c1->SetGrid();
   gStyle->SetBarOffset(0.25);
   gStyle->SetBarWidth(0.5);
   
   const Int_t n = 20;
   Double_t x[n], y[n];
   for (Int_t i=0;i<n;i++) {
     x[i] = i*0.1;
     y[i] = 10*sin(x[i]+0.2);
     printf(" i %i %f %f \n",i,x[i],y[i]);
   }
   gr = new TGraph(n,x,y);
   gr->SetLineColor(2);
   gr->SetLineWidth(4);
   gr->Draw("ABP");

   // TCanvas::Update() draws the frame, after which one can change it
   c1->Update();
   c1->GetFrame()->SetFillColor(21);
   c1->GetFrame()->SetBorderSize(2);
   c1->Modified();
}