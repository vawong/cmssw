import re
import ROOT
import argparse

class Hit:
    def __init__(self,_spot,_pulseType,_pulses):
        self.spot = _spot
        self.pulseType = _pulseType
        self.pulses = _pulses

def checkSameSpot(spot, allSpots):
    repeated = False
    if len(allSpots) == 0:
        return repeated
    else:
        for index in allSpots:
            if index == spot: repeated = True
    return repeated

def setBinLabel(hist):
    hist.GetXaxis().SetBinLabel(1,"TS0");
    hist.GetXaxis().SetBinLabel(2,"TS1");
    hist.GetXaxis().SetBinLabel(3,"TS2");
    hist.GetXaxis().SetBinLabel(4,"TS3");
    hist.GetXaxis().SetBinLabel(5,"TS4");
    hist.GetXaxis().SetBinLabel(6,"TS5");
    hist.GetXaxis().SetBinLabel(7,"TS6");
    hist.GetXaxis().SetBinLabel(8,"TS7");
    hist.GetXaxis().SetBinLabel(9,"TS8");
    hist.GetXaxis().SetBinLabel(10,"TS9");

def fillHist(hist,val):
    for i in range(0,9):
        hist.SetBinContent(i+1,val[i])


parser = argparse.ArgumentParser()
parser.add_argument('infile',help='Input printout text file of pulses')
args = parser.parse_args()

with open(args.infile) as f:
    allHits = []
    allSpots = []
    lines = f.readlines()

    # Dump info into Hit object
    for i in xrange(len(lines)):
        if '(iEta, iPhi, Depth)' in lines[i]:
            spot = [int(s) for s in re.findall(r'-?\d+',lines[i])]
            
            # Store different spots
            if not checkSameSpot(spot,allSpots):
                allSpots.append(spot)
            pulseType = [int(s) for s in re.findall(r'\b\d+\b',lines[i-1])]
            pulses = []
            for ts in range (2,11):
                pulses.append([float(s) for s in re.findall(r'-?\d+\.\d+',lines[i+ts])])
            myHit = Hit(spot,pulseType,pulses)
            allHits.append(myHit)

    
    # Iterate by different spots and draw histograms
    index = 0
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetLegendBorderSize(0);

    for mySpot in allSpots:
        ieta = mySpot[0]
        iphi = mySpot[1]
        depth = mySpot[2]
        index += 1 
        drawnDigi = False
        canvas = ROOT.TCanvas('canvas'+str(index),'',800,600)
        canvas.cd()
        for hit in allHits:
            if hit.spot == mySpot:
                if hit.pulseType == [1]: # standard m2
                    if not drawnDigi: 
                        digi = ROOT.TH1F('digi','',10,0.,10.);
                        setBinLabel(digi)
                        digiVal = []
                        for val in hit.pulses:
                            digiVal.append(val[1])
                        fillHist(digi,digiVal)
                        digi.SetLineWidth(3)
                        digi.SetLineColor(ROOT.kBlack)
                        digi.Draw("hist same")
                        drawnDigi = True

                    m2 = ROOT.TH1F('m2','',10,0.,10.);
                    setBinLabel(m2)
                    m2Val = []
                    for val in hit.pulses:
                        m2Val.append(val[0])
                    fillHist(m2,m2Val)
                    m2.SetLineWidth(3)
                    m2.SetLineColor(ROOT.kRed)
                    m2.Draw("hist same")
                
                elif hit.pulseType == [2]: # m2csv105
                    if not drawnDigi: 
                        digi = ROOT.TH1F('digi','',10,0.,10.);
                        setBinLabel(digi)
                        digiVal = []
                        for val in hit.pulses:
                            digiVal.append(val[1])
                        fillHist(digi,digiVal)
                        digi.SetLineWidth(3)
                        digi.SetLineColor(ROOT.kBlack)
                        digi.Draw("hist same")
                        drawnDigi = True
                    m2csv105 = ROOT.TH1F('m2csv105','',10,0.,10.);
                    setBinLabel(m2csv105)
                    m2csv105Val = []
                    for val in hit.pulses:
                        m2csv105Val.append(val[0])
                    fillHist(m2csv105,m2csv105Val)
                    m2csv105.SetLineWidth(3)
                    m2csv105.SetLineColor(ROOT.kBlue)
                    m2csv105.Draw("hist same")
                
                elif hit.pulseType == [3]: # m2csvlag
                    if not drawnDigi: 
                        digi = ROOT.TH1F('digi','',10,0.,10.);
                        setBinLabel(digi)
                        digiVal = []
                        for val in hit.pulses:
                            digiVal.append(val[1])
                        fillHist(digi,digiVal)
                        digi.SetLineWidth(3)
                        digi.SetLineColor(ROOT.kBlack)
                        digi.Draw("hist same")
                        drawnDigi = True
                    m2csvlag = ROOT.TH1F('m2csvlag','',10,0.,10.);
                    m2csvlagVal = []
                    for val in hit.pulses:
                        m2csvlagVal.append(val[0])
                    fillHist(m2csvlag,m2csvlagVal)
                    m2csvlag.SetLineWidth(3)
                    m2csvlag.SetLineColor(ROOT.kOrange)
                    m2csvlag.Draw("hist same")
        canvas.BuildLegend(0.65,0.6,0.88,0.88)
        text = ROOT.TLatex()
        text.SetTextFont(42)
        text.SetTextColor(1)
        text.SetTextAlign(12)
        text.SetTextSize(0.04)
        text.DrawLatex(0.5,0.95*m2.GetMaximum(),'iEta='+str(ieta));
        text.DrawLatex(0.5,0.85*m2.GetMaximum(),'iPhi='+str(iphi));
        text.DrawLatex(0.5,0.75*m2.GetMaximum(),'Depth='+str(depth));

        canvas.Draw()
        canvas.SaveAs('iEtaiPhiDepth_'+str(ieta)+'_'+str(iphi)+'_'+str(depth)+'.png')
