/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once
#include <commands/dicomizecommand.h>


namespace GADAPI
{
class SecondaryCaptureDicomizeCommandParameters : public ::GADAPI::DicomizeCommandParams
{
public:
        SecondaryCaptureDicomizeCommandParameters(GNC::GCS::IVista* pView, const std::string& screenShot);

        GNC::GCS::IVista* m_pView;
        std::string m_originalImagePath;
        std::string m_screenShot;
};

class SecondaryCaptureDicomizeCommand : public ::GADAPI::DicomizeCommand
{
public:
        SecondaryCaptureDicomizeCommand(::GADAPI::SecondaryCaptureDicomizeCommandParameters* pParams);

protected:
        virtual void Update();
        virtual bool ImportarFicherosOriginales();

        SecondaryCaptureDicomizeCommandParameters* m_pSecondaryParams;
};
}
