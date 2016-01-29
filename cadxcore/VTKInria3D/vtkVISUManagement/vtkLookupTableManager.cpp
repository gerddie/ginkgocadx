/*=========================================================================

Program:   vtkINRIA3D
Module:    $Id: vtkLookupTableManager.cpp $
Language:  C++
Author:    $Author: ntoussaint $
Date:      $Date: 2008-05-20 18:57:09 +0200 (mar, 20 may 2008) $
Version:   $Revision: 842 $

Copyright (c) 2007 INRIA - Asclepios Project. All rights reserved.
See Copyright.txt for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <api/internationalization/internationalization.h>
#include "vtkLookupTableManager.h"
#include <vtkObjectFactory.h>

vtkCxxRevisionMacro(vtkLookupTableManager, "$Revision: 842 $");
vtkStandardNewMacro(vtkLookupTableManager);
#include "lut/BlackBody.h"
#include "lut/Cardiac.h"
#include "lut/VRBones.h"
#include "lut/VRMusclesBones.h"
#include "lut/VRRedVessels.h"
#include "lut/GrayRainbow.h"
#include "lut/Stern.h"
#include "lut/Spectrum.h"
#include "lut/HotMetal.h"
#include "lut/HotIron.h"
#include "lut/HotGreen.h"
#include "lut/GEColor.h"
#include "lut/PERFUSION.h"
#include "lut/Flow.h"
#include "lut/LONI.h"
#include "lut/LONI2.h"
#include "lut/Asymmetry.h"
#include <vtkMath.h>
#include <cmath>

#include <time.h>

vtkLookupTableManager::vtkLookupTableManager() {}

vtkLookupTableManager::~vtkLookupTableManager() {}

std::list<std::string> vtkLookupTableManager::GetAvailableLookupTables() {
    std::string lutNames[]={_Std("B/W"),_Std("B/W Inverse"),_Std("Barten"), _Std("Black Body"), _Std("Cardiac"), _Std("VR Bones"), _Std("VR Muscles & Bones"), _Std("VR Red Vessels"), _Std("Gray Rainbow"), _Std("Stern"), _Std("Spectrum"), _Std("Hot Metal"), _Std("Hot Iron"), _Std("Hot Green"), _Std("GE Color"), _Std("PERFusion"), _Std("PET"), _Std("Flow"), _Std("LONI"), _Std("LONI2"), _Std("Asymmetry"), _Std("P-Value"), _Std("ROI")};
    std::list<std::string> v_lutNames;
    for ( int i=0; i<22; i++) {
        v_lutNames.push_back(lutNames[i]);
    }

    return v_lutNames;
}


vtkLookupTable* vtkLookupTableManager::GetLookupTable(const int& n) {
	vtkLookupTable* lut = 0;
	switch (n) {
		case LUT_LINEAR:
			lut = vtkLookupTableManager::GetLinearLookupTable();
			break;
		case LUT_LINEAR_INV:
			lut = vtkLookupTableManager::GetLinearInverseLookupTable();
			break;

		case LUT_BARTEN:
			lut = vtkLookupTableManager::GetBartenLookupTable();
			break;

		case LUT_BLACKBODY:
			lut = vtkLookupTableManager::GetBlackBodyLookupTable();
			break;

		case LUT_CARDIAC:
			lut = vtkLookupTableManager::GetCardiacLookupTable();
			break;

		case LUT_VRBONES:
			lut = vtkLookupTableManager::GetVRBonesLookupTable();
			break;

		case LUT_VRMUSCLESBONES:
			lut = vtkLookupTableManager::GetVRMusclesBonesLookupTable();
			break;

		case LUT_VRREDVESSELS:
			lut = vtkLookupTableManager::GetVRRedVesselsLookupTable();
			break;

		case LUT_GRAYRAINBOW:
			lut = vtkLookupTableManager::GetGrayRainbowLookupTable();
			break;

		case LUT_STERN:
			lut = vtkLookupTableManager::GetSternLookupTable();
			break;

		case LUT_SPECTRUM:
			lut = vtkLookupTableManager::GetSpectrumLookupTable();
			break;

		case LUT_HOTMETAL:
			lut = vtkLookupTableManager::GetHotMetalLookupTable();
			break;

		case LUT_HOTIRON:
			lut = vtkLookupTableManager::GetHotIronLookupTable();
			break;

		case LUT_HOTGREEN:
			lut = vtkLookupTableManager::GetHotGreenLookupTable();
			break;

		case LUT_GECOLORS:
			lut = vtkLookupTableManager::GetGEColorLookupTable();
			break;

		case LUT_PERFUSION:
			lut = vtkLookupTableManager::GetPERFusionLookupTable();
			break;

		case LUT_PET:
			lut = vtkLookupTableManager::GetPETLookupTable();
			break;

		case LUT_FLOW:
			lut = vtkLookupTableManager::GetFlowLookupTable();
			break;

		case LUT_LONI:
			lut = vtkLookupTableManager::GetLONILookupTable();
			break;

		case LUT_LONI2:
			lut = vtkLookupTableManager::GetLONI2LookupTable();
			break;

		case LUT_ASYMETRY:
			lut = vtkLookupTableManager::GetAsymmetryLookupTable();
			break;

		case LUT_PVALUE:
			lut = vtkLookupTableManager::GetPValueLookupTable();
			break;

		case LUT_ROI:
			lut = vtkLookupTableManager::GetROILookupTable();
			break;

		default:
			break;
	}

	return lut;
}

vtkLookupTable* vtkLookupTableManager::GetBartenLookupTable() {

	double tablaBarten[1024];

	const double a = -1.3011877;
	const double b = -2.5840191E-2;
	const double c = 8.0242636E-2;
	const double d = -1.0320229E-1;
	const double e = 1.3646699E-1;
	const double f = 2.8745620E-2;
	const double g = -2.5468404E-2;
	const double h = -3.1978977E-3;
	const double k = 1.2992634E-4;
	const double m = 1.3635334E-3;

	double logaritmoJ;
	double minimo = 3.402823466E+38;
	double maximo = -3.402823466E+38;
	int indice;
	for ( int j=1; j<=1024; j++) {
		indice = j-1;
		logaritmoJ = std::log((double)j);

		tablaBarten[indice] = ( a + c * logaritmoJ + e * std::pow(logaritmoJ,(double)2.0f) + g * std::pow(logaritmoJ,(double)3.0f) + m * std::pow(logaritmoJ,(double)4.0f) ) /
			( 1.0f + b * logaritmoJ + d * std::pow(logaritmoJ,(double)2.0f) + f * std::pow(logaritmoJ,(double)3.0f) + h * std::pow(logaritmoJ,(double)4.0f) + k * std::pow(logaritmoJ,(double)5.0f) );

		if(maximo < tablaBarten[indice])
			maximo = tablaBarten[indice];
		if(minimo > tablaBarten[indice])
			minimo = tablaBarten[indice];
	}

	const int numeroValores = 1024;
	vtkLookupTable* lut = vtkLookupTable::New();
	lut->SetNumberOfTableValues(numeroValores);
	lut->Build();

	for (int i = 0; i<numeroValores; i++) {
		int posicion = i;
		double v = (tablaBarten[posicion] - minimo) / (maximo - minimo);		
		lut->SetTableValue(i, v, v, v, 1.0);
	}

	return lut;
}

vtkLookupTable* vtkLookupTableManager::GetLinearLookupTable()
{
	const int numeroValores = 1024;
	vtkLookupTable* lut = vtkLookupTable::New();
	lut->SetNumberOfTableValues(numeroValores);
	lut->Build();
	
	for (int i = 0; i < numeroValores; i++)
	{		
		double v = ((float) i) / 1024.0f;
		lut->SetTableValue(i, v, v, v, 1.0);
	}

	return lut;
}

vtkLookupTable* vtkLookupTableManager::GetLinearInverseLookupTable()
{
	const int numeroValores = 1024;
	vtkLookupTable* lut = vtkLookupTable::New();
	lut->SetNumberOfTableValues(numeroValores);
	lut->Build();
	
	for (int i = 0; i < numeroValores; i++)
	{		
		double v = ((float) numeroValores - i - 1) / 1024.0f;
		lut->SetTableValue(i, v, v, v, 1.0);
	}

	return lut;
}

vtkLookupTable* vtkLookupTableManager::GetSpectrumLookupTable() {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(256);
    lut->Build();

    for ( int i=0; i<256; i++) {
        lut->SetTableValue(i, (double)Spectrum[i]/255.0, (double)Spectrum[256+i]/255.0, (double)Spectrum[256*2+i]/255.0, 1.0);
    }

    return lut;
}


vtkLookupTable* vtkLookupTableManager::GetHotMetalLookupTable() {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(256);
    lut->Build();

    for ( int i=0; i<256; i++) {
        lut->SetTableValue(i, (double)HotMetal[i]/255.0, (double)HotMetal[256+i]/255.0, (double)HotMetal[256*2+i]/255.0, 1.0);
    }

    return lut;
}


vtkLookupTable* vtkLookupTableManager::GetGEColorLookupTable() {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(256);
    lut->Build();

    for ( int i=0; i<256; i++) {
        lut->SetTableValue(i, (double)GEColor[i]/255.0, (double)GEColor[256+i]/255.0, (double)GEColor[256*2+i]/255.0, 1.0);
    }

    return lut;
}


vtkLookupTable* vtkLookupTableManager::GetFlowLookupTable() {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(256);
    lut->Build();

    for ( int i=0; i<256; i++) {
        lut->SetTableValue(i, (double)Flow[i]/255.0, (double)Flow[256+i]/255.0, (double)Flow[256*2+i]/255.0, 1.0);
    }

    return lut;
}


vtkLookupTable* vtkLookupTableManager::GetLONILookupTable() {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(203);
    lut->Build();

    for ( int i=0; i<203; i++) {
        lut->SetTableValue(i, (double)LONI[i][0], (double)LONI[i][1], (double)LONI[i][2], 1.0);
    }

    return lut;
}


vtkLookupTable* vtkLookupTableManager::GetLONI2LookupTable() {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(120);
    lut->Build();

    for ( int i=0; i<120; i++) {
        lut->SetTableValue(i, (double)LONI2[i][0], (double)LONI2[i][1], (double)LONI2[i][2], 1.0);
    }

    return lut;
}


vtkLookupTable* vtkLookupTableManager::GetAsymmetryLookupTable() {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(256);
    lut->Build();

    for ( int i=0; i<256; i++) {
        lut->SetTableValue(i, (double)AsymmetryLUT[i][0], (double)AsymmetryLUT[i][1], (double)AsymmetryLUT[i][2], 1.0);
    }

    return lut;
}


vtkLookupTable* vtkLookupTableManager::GetPValueLookupTable() {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetTableRange (0, 1);
    lut->SetSaturationRange (1.0, 1.5);
    lut->SetHueRange (0.666, 0.8333);
    //lut->SetHueRange (0.666, 0.0);
    lut->SetValueRange (0.5, 1.0);
    lut->Build();

    return lut;
}


// Nuevos

vtkLookupTable* vtkLookupTableManager::GetBlackBodyLookupTable(void) {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(256);
    lut->Build();

    for ( int i=0; i<256; i++) {
        lut->SetTableValue(i, (double)BlackBody[i]/255.0, (double)BlackBody[256+i]/255.0, (double)BlackBody[256*2+i]/255.0, /*(double)(i)/255.0*/ log (1.0+(double)(i)/255.0*9.0)/log (10.0) );
    }

    return lut;
}

vtkLookupTable* vtkLookupTableManager::GetCardiacLookupTable(void) {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(256);
    lut->Build();

    for ( int i=0; i<256; i++) {
        lut->SetTableValue(i, (double)Cardiac[i]/255.0, (double)Cardiac[256+i]/255.0, (double)Cardiac[256*2+i]/255.0, /*(double)(i)/255.0*/ log (1.0+(double)(i)/255.0*9.0)/log (10.0) );
    }

    return lut;
}

vtkLookupTable* vtkLookupTableManager::GetVRBonesLookupTable(void) {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(256);
    lut->Build();

    for ( int i=0; i<256; i++) {
        lut->SetTableValue(i, (double)VRBones[i]/255.0, (double)VRBones[256+i]/255.0, (double)VRBones[256*2+i]/255.0, /*(double)(i)/255.0*/ log (1.0+(double)(i)/255.0*9.0)/log (10.0) );
    }

    return lut;
}

vtkLookupTable* vtkLookupTableManager::GetVRMusclesBonesLookupTable(void) {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(256);
    lut->Build();

    for ( int i=0; i<256; i++) {
        lut->SetTableValue(i, (double)VRMusclesBones[i]/255.0, (double)VRMusclesBones[256+i]/255.0, (double)VRMusclesBones[256*2+i]/255.0, /*(double)(i)/255.0*/ log (1.0+(double)(i)/255.0*9.0)/log (10.0) );
    }

    return lut;
}

vtkLookupTable* vtkLookupTableManager::GetVRRedVesselsLookupTable(void) {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(256);
    lut->Build();

    for ( int i=0; i<256; i++) {
        lut->SetTableValue(i, (double)VRRedVessels[i]/255.0, (double)VRRedVessels[256+i]/255.0, (double)VRRedVessels[256*2+i]/255.0, /*(double)(i)/255.0*/ log (1.0+(double)(i)/255.0*9.0)/log (10.0) );
    }

    return lut;
}

vtkLookupTable* vtkLookupTableManager::GetGrayRainbowLookupTable(void) {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(256);
    lut->Build();

    for ( int i=0; i<256; i++) {
        lut->SetTableValue(i, (double)GrayRainbow[i]/255.0, (double)GrayRainbow[256+i]/255.0, (double)GrayRainbow[256*2+i]/255.0, /*(double)(i)/255.0*/ log (1.0+(double)(i)/255.0*9.0)/log (10.0) );
    }

    return lut;
}

vtkLookupTable* vtkLookupTableManager::GetSternLookupTable(void) {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(256);
    lut->Build();

    for ( int i=0; i<256; i++) {
        lut->SetTableValue(i, (double)Stern[i]/255.0, (double)Stern[256+i]/255.0, (double)Stern[256*2+i]/255.0, /*(double)(i)/255.0*/ log (1.0+(double)(i)/255.0*9.0)/log (10.0) );
    }

    return lut;
}

vtkLookupTable* vtkLookupTableManager::GetHotIronLookupTable(void) {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(256);
    lut->Build();

    for ( int i=0; i<256; i++) {
        lut->SetTableValue(i, (double)HotIron[i]/255.0, (double)HotIron[256+i]/255.0, (double)HotIron[256*2+i]/255.0, /*(double)(i)/255.0*/ log (1.0+(double)(i)/255.0*9.0)/log (10.0) );
    }

    return lut;
}

vtkLookupTable* vtkLookupTableManager::GetHotGreenLookupTable(void) {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(256);
    lut->Build();

    for ( int i=0; i<256; i++) {
        lut->SetTableValue(i, (double)HotGreen[i]/255.0, (double)HotGreen[256+i]/255.0, (double)HotGreen[256*2+i]/255.0, /*(double)(i)/255.0*/ log (1.0+(double)(i)/255.0*9.0)/log (10.0) );
    }

    return lut;
}

vtkLookupTable* vtkLookupTableManager::GetPETLookupTable(void) {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(256);
    
	lut->Build();

	for ( int i = 0; i < 256; i++) {
		if (i < 128) {
			lut->SetTableValue(i, (double)(i << 1) / 255.0, 0.0, 0.0, /*(double)(i)/255.0*/ log (1.0+(double)(i)/255.0*9.0)/log (10.0) );
		}
		else if (i < 192) {
			lut->SetTableValue(i, 1.0, (double) ( (i-128) << 2 ) / 255.0, 0.0, /*(double)(i)/255.0*/ log (1.0+(double)(i)/255.0*9.0)/log (10.0) );
		}
		else {
			lut->SetTableValue(i, 1.0, 1.0, (double) ( (i-192) << 2 ) / 255.0, /*(double)(i)/255.0*/ log (1.0+(double)(i)/255.0*9.0)/log (10.0) );
		}        
    }

    return lut;
}

vtkLookupTable* vtkLookupTableManager::GetPERFusionLookupTable(void) {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(256);
    
	lut->Build();

	const int* R = PERFUSION;
	const int* G = R + 256;
	const int* B = G + 256;

    for ( int i = 0; i < 256; i++) {
        lut->SetTableValue(i, (double)R[i]/255.0, (double)G[i]/255.0, (double)B[i]/255.0, /*(double)(i)/255.0*/ log (1.0+(double)(i)/255.0*9.0)/log (10.0) );
    }

    return lut;
}

vtkLookupTable* vtkLookupTableManager::GetROILookupTable() {
    vtkLookupTable* lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues (256);
    lut->SetTableValue (0, 0.0, 0.0, 0.0, 0.0); // erase color

    lut->SetTableValue (1, 1.0, 0.0, 0.0, 0.5);
    lut->SetTableValue (2, 0.0, 1.0, 0.0, 0.5);
    lut->SetTableValue (3, 0.0, 0.0, 1.0, 0.5);
    lut->SetTableValue (4, 1.0, 1.0, 0.0, 0.5);
    lut->SetTableValue (5, 0.0, 1.0, 1.0, 0.5);
    lut->SetTableValue (6, 1.0, 0.0, 1.0, 0.5);

    lut->SetTableValue (7, 1.0, 0.5, 0.0, 0.5);
    lut->SetTableValue (8, 0.0, 1.0, 0.5, 0.5);
    lut->SetTableValue (9, 0.5, 0.0, 1.0, 0.5);
    lut->SetTableValue (10, 1.0, 1.0, 0.5, 0.5);
    lut->SetTableValue (11, 0.5, 1.0, 1.0, 0.5);
    lut->SetTableValue (12, 1.0, 0.5, 1.0, 0.5);



    // Fill the rest of the labels with color ramps, code taken from SNAP
//   for (int i = 13; i < 256; i++)
//   {
//     if (i < 85)
//     {
//       lut->SetTableValue (i, ((84.0-i)/85.0 * 200.0 + 50.0)/255.0, (i/85.0 * 200.0 + 50.0)/255.0, 0, 0.5);
//     }
//     else if (i < 170)
//     {
//       lut->SetTableValue (i, 0, ((169.0-i)/85.0 * 200.0 + 50)/255.0, ((i-85)/85.0 * 200.0 + 50)/255.0, 0.5);
//     }
//     else
//     {
//       lut->SetTableValue (i, ((i-170)/85.0 * 200.0 + 50)/255.0, 0.0, ((255.0-i)/85.0 * 200.0 + 50)/255.0, 0.5);
//     }
//   }


    // Fill the rest with random colors

//   for (int i = 13; i < 256; i++)
//   {
//     srand (clock());

//     // put a random color
//     int i1 = 1 + (int) (100.0 * ((double)rand() / (RAND_MAX + 1.0)));
//     int i2 = 1 + (int) (100.0 * ((double)rand() / (RAND_MAX + 1.0)));
//     int i3 = 1 + (int) (100.0 * ((double)rand() / (RAND_MAX + 1.0)));
//     double rand_1 = (double)(i1)/(100.0);
//     double rand_2 = (double)(i2)/(100.0);
//     double rand_3 = (double)(i3)/(100.0);
//     double r = rand_1, g = rand_2, b = rand_3;
//     if (i1 < 33)
//       r = 1;
//     else if (i1 < 66)
//       g = 1;
//     else if (i1 < 100)
//       b = 1;

//     lut->SetTableValue (i, r, g, b, 0.5);
//   }

    for (int i = 12; i < 256; i++) {
        if (i%12 == 0)
            lut->SetTableValue (i, 1.0, 0.0, 0.0, 0.5);
        else if (i%12 == 1)
            lut->SetTableValue (i, 0.0, 1.0, 0.0, 0.5);
        else if (i%12 == 2)
            lut->SetTableValue (i, 0.0, 0.0, 1.0, 0.5);
        else if (i%12 == 3)
            lut->SetTableValue (i, 1.0, 1.0, 0.0, 0.5);
        else if (i%12 == 4)
            lut->SetTableValue (i, 0.0, 1.0, 1.0, 0.5);
        else if (i%12 == 5)
            lut->SetTableValue (i, 1.0, 0.0, 1.0, 0.5);
        else if (i%12 == 6)
            lut->SetTableValue (i, 1.0, 0.5, 0.0, 0.5);
        else if (i%12 == 7)
            lut->SetTableValue (i, 0.0, 1.0, 0.5, 0.5);
        else if (i%12 == 8)
            lut->SetTableValue (i, 0.5, 0.0, 1.0, 0.5);
        else if (i%12 == 9)
            lut->SetTableValue (i, 1.0, 1.0, 0.5, 0.5);
        else if (i%12 == 10)
            lut->SetTableValue (i, 0.5, 1.0, 1.0, 0.5);
        else if (i%12 == 11)
            lut->SetTableValue (i, 1.0, 0.5, 1.0, 0.5);
    }

    return lut;

}

vtkLookupTable* vtkLookupTableManager::GetOverlayLooupTable() 
{
	vtkLookupTable* lut = vtkLookupTable::New();
	lut->SetNumberOfTableValues(2);
	lut->Build();

	lut->SetTableValue(0, 0.0, 0.0, 0.0, 0.0);

	lut->SetTableValue(1, 1.0, 1.0, 1.0, 1.0);	 
	return lut;
}
