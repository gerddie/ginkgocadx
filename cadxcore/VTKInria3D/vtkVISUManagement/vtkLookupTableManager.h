/*=========================================================================

Program:   vtkINRIA3D
Module:    $Id: vtkLookupTableManager.h $
Language:  C++
Author:    $Author: filus $
Date:      $Date: 2008-02-22 17:19:25 +0100 (vie, 22 feb 2008) $
Version:   $Revision: 700 $

Copyright (c) 2007 INRIA - Asclepios Project. All rights reserved.
See Copyright.txt for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _vtk_LookupTableManager_h_
#define _vtk_LookupTableManager_h_
#include <api/api.h>
#include <wx/arrstr.h>
#include "vtkINRIA3DConfigure.h"

#include <list>

#include <vtkObject.h>
#include <vtkLookupTable.h>

class EXTAPI vtkLookupTableManager: public vtkObject
{

public:

        //BTX
        enum LookupTableIds {
                LUT_LINEAR = 0,
                LUT_LINEAR_INV,
                LUT_BARTEN,
                LUT_BLACKBODY,      //
                LUT_CARDIAC,        //
                LUT_VRBONES,        //
                LUT_VRMUSCLESBONES, //
                LUT_VRREDVESSELS,   //
                LUT_GRAYRAINBOW,    //
                LUT_STERN,          //
                LUT_SPECTRUM,       //
                LUT_HOTMETAL,
                LUT_HOTIRON,        //
                LUT_HOTGREEN,       //
                LUT_GECOLORS,
                LUT_PERFUSION,
                LUT_PET,
                LUT_FLOW,
                LUT_LONI,
                LUT_LONI2,
                LUT_ASYMETRY,
                LUT_PVALUE,
                LUT_ROI
        };
        //ETX

        static vtkLookupTableManager* New();
        vtkTypeMacro (vtkLookupTableManager, vtkObject);

        static vtkLookupTable* GetBartenLookupTable (void);

        static vtkLookupTable* GetLinearLookupTable (void);

        static vtkLookupTable* GetLinearInverseLookupTable (void);

        static vtkLookupTable* GetSpectrumLookupTable (void);

        static vtkLookupTable* GetHotMetalLookupTable (void);

        static vtkLookupTable* GetGEColorLookupTable (void);

        static vtkLookupTable* GetFlowLookupTable (void);

        static vtkLookupTable* GetLONILookupTable (void);

        static vtkLookupTable* GetLONI2LookupTable (void);

        static vtkLookupTable* GetAsymmetryLookupTable (void);

        static vtkLookupTable* GetPValueLookupTable (void);

        static std::list<std::string> GetAvailableLookupTables(void);

        static vtkLookupTable* GetROILookupTable (void);

        static vtkLookupTable* GetOverlayLooupTable (void);

        // Nuevos

        static vtkLookupTable* GetBlackBodyLookupTable(void);
        static vtkLookupTable* GetCardiacLookupTable(void);
        static vtkLookupTable* GetVRBonesLookupTable(void);
        static vtkLookupTable* GetVRMusclesBonesLookupTable(void);
        static vtkLookupTable* GetVRRedVesselsLookupTable(void);
        static vtkLookupTable* GetGrayRainbowLookupTable(void);
        static vtkLookupTable* GetSternLookupTable(void);
        static vtkLookupTable* GetHotIronLookupTable(void);
        static vtkLookupTable* GetHotGreenLookupTable(void);

        // PERFUSION

        static vtkLookupTable* GetPERFusionLookupTable(void);

        // PET
        static vtkLookupTable* GetPETLookupTable(void);

        static vtkLookupTable* GetLookupTable(const int&);

protected:
        vtkLookupTableManager();
        ~vtkLookupTableManager();

private:
};


#endif
