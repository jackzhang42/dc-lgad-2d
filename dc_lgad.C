#include <iostream>
#include <fstream>
#include <cmath>

#include <TCanvas.h>
#include <TROOT.h>
#include <TApplication.h>

#include "Garfield/MediumSilicon.hh"
#include "Garfield/ComponentTcad2d.hh"
#include "Garfield/ComponentAnalyticField.hh"
#include "Garfield/Sensor.hh"
#include "Garfield/TrackHeed.hh"
#include "Garfield/AvalancheMC.hh"
#include "Garfield/ViewField.hh"
#include "Garfield/ViewDrift.hh"
#include "Garfield/ViewSignal.hh"
#include "Garfield/FundamentalConstants.hh"
#include "Garfield/Random.hh"

using namespace Garfield;

int main(int argc, char * argv[]) {
	
	TApplication app("app", &argc, argv);

	const double depth = 50.e-4;
	const double left = 175.e-4;
	const double right = 1125.e-4;
	const double width = 650.e-4;

	MediumSilicon si;
	ComponentTcad2d cmp;
  cmp.EnableDebugging();
  cmp.Initialise("n3397_des.grd", "n3397_des.dat");
	cmp.SetRangeZ(-width, width);
	int nRegions = cmp.GetNumberOfRegions();
	for (int i = 0; i < nRegions; ++i) {
		std::string region;
		bool active;
		cmp.GetRegion(i, region, active);
		if (region == "\"Silicon_1\"") cmp.SetMedium(i, &si);
	}

	ComponentAnalyticField wfieldAnalytic;
	wfieldAnalytic.AddPlaneX(0., 0., "top");
	wfieldAnalytic.AddPlaneX(depth, -100., "bot");
	wfieldAnalytic.AddStripOnPlaneX('z', 0., left, right, "strip");
	wfieldAnalytic.AddReadout("strip");

	ViewField vField;
	constexpr bool plotField = true;
	if (plotField) {
		vField.SetComponent(&cmp);
		vField.PlotContour("v");
	}


	app.Run(true);

}
