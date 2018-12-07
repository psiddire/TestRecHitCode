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
    #zmass=row.ZMass
    #met=row.MET

    outfile.write("%d:%d:%d\n" %(run, lumi, evt))#, zmass, met))
    
outfile.close()


    

