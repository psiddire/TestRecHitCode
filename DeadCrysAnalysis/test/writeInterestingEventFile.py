import ROOT
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--input', dest='infilename', action='store',help='input file.root', required=True)
parser.add_argument('--output', dest='outtxt', action='store', help='output file.txt', required=True)
args = parser.parse_args()

infile=ROOT.TFile.Open(args.infilename, "READ")

ntuple=infile.Get("zskimtree/TreeR")

outfile=open(args.outtxt, "w")

for row in ntuple:
    run=row.run
    lumi=row.lumi
    evt=row.evt
    #e1=row.E1
    #e2=row.E2
    #e3=row.E3
    #e4=row.E4
    #e6=row.E6
    #e7=row.E7
    #e8=row.E8
    #e9=row.E9
    
    #minE = min(e1,e2,e3,e4,e6,e7,e8,e9)
    #if minE>0.70:
    outfile.write("%d:%d:%d\n" %(run, lumi, evt))
    
outfile.close()


    

