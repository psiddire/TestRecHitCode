import ROOT
infiles=['recHitTree_all.root']
path='/afs/cern.ch/work/t/taroni/private/DeadCh10XNoChanges/src/MyRecHitTree/DeadCrysAnalysis/test/'
outfile=ROOT.TFile.Open("minEnergy_all.root", "RECREATE")
hMinE=ROOT.TH1F("minE", "single cristal min E (GeV)", 1000, 0, 1);
outtext=open("checkingIntEvt700.txt", "w")
for infile in infiles:
    file0=ROOT.TFile.Open(path+infile, "READ")
    tree=file0.Get("deadCrysAnalyzer/TreeR")
    for row in tree:
        run=row.run
        lumi=row.lumi
        evt=row.evt
        e1=row.E1
        e2=row.E2
        e3=row.E3
        e4=row.E4
        e6=row.E6
        e7=row.E7
        e8=row.E8
        e9=row.E9
        minE = min(e1,e2,e3,e4,e6,e7,e8,e9)
        if minE>0.70:
            outtext.write("%d:%d:%d\n" %(run, lumi, evt))
        if minE>1.:
            minE=0.999
        hMinE.Fill(minE)
outtext.close()
outfile.cd()
hMinE.Write()

hSelEvents=ROOT.TH1F("selEvt", "number of events above threshold", 1000, 0, 1); 
for ibin in range(0, hMinE.GetXaxis().GetNbins()+1):    
    evts=hMinE.Integral(ibin, 1000)
    totevt=0
    hSelEvents.Fill(hSelEvents.GetBinCenter(ibin), evts)
    print ibin, hMinE.GetBinCenter(ibin), hSelEvents.GetBinCenter(ibin), evts, totevt

print 'number of recovered crystal if thr=0.28 GeV: %s' %hSelEvents.GetBinContent(hSelEvents.FindBin(0.28))
print 'number of recovered crystal if thr=0.7 GeV: %s' %hSelEvents.GetBinContent(hSelEvents.FindBin(0.7))

hSelEvents.Write()
outfile.Close()
