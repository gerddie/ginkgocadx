/*
 *  
 *  $Id: iventanaprogreso.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#pragma once
class wxString;

class IVentanaProgreso {
public:

    IVentanaProgreso() { }
    virtual ~IVentanaProgreso() { }

    virtual void ResetProgress() = 0;
    virtual void SetProgress(int progress, wxString text) = 0;
};

