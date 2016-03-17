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
#include <api/autoptr.h>

#include <api/icommand.h>

#include <main/gui/export/datospersistentesexportacion.h>

namespace GNC
{
namespace GCS
{
class IVista;
}
}

namespace GADAPI
{
class ComandoExportacion;

class ComandoExportacionParams : public GNC::GCS::IComandoParams
{
public:
        ComandoExportacionParams(GNC::GCS::Ptr<GNC::GUI::TipoWizardExportacion>& pDatosPersistentes);

        virtual ~ComandoExportacionParams();

        GNC::GCS::Ptr<GNC::GUI::TipoWizardExportacion> m_pDatosPersistentes;
        std::list<std::string> m_outputPaths;
        std::string m_Error;
        bool m_hasError;
        std::string m_tempDir;
};

class ComandoExportacion : public GNC::GCS::IComando
{
public:

        ComandoExportacion(ComandoExportacionParams* pParams);

protected:

        virtual void Execute();
        virtual void Update();

protected:
        void ExportarDICOM();
        wxString GetFichero(const wxString& extension);

        ComandoExportacionParams* m_pExportacionParams;
};
}
