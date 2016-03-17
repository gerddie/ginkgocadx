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
#include <list>
#include <set>

#include <api/api.h>
#include <api/icommand.h>
#include <api/controllers/icontroladorvistas.h>
#include <api/controllers/ihistorycontroller.h>
#include <api/dicom/imodelodicom.h>
#include <wx/string.h>

namespace GNC
{
namespace GCS
{
class IVista;
}
}

namespace GADAPI
{
//lee un directorio en profundidad y lo incluye en el dicomdir
namespace LinkHistoryCommand
{
class LinkHistoryCommand;
class EXTAPI LinkHistoryCommandParams : public GNC::GCS::IComandoParams
{
public:
        LinkHistoryCommandParams(const GNC::GCS::Ptr<IModeloDicom>& pModelo);
        virtual ~LinkHistoryCommandParams();

        GNC::GCS::Ptr<IModeloDicom> m_pModelo;
        GNC::GCS::IHistoryController::TAddErrorList m_ErrorList;
        std::list<long>                             m_InsertedSeries;
        bool m_abrirDespuesDeCargar;
        bool m_informar;
};

class EXTAPI LinkHistoryCommand : public GNC::GCS::IComando
{
public:

        LinkHistoryCommand(LinkHistoryCommandParams* pParams);

protected:
        virtual void Execute();
        virtual void Update();

private:
        LinkHistoryCommandParams* m_pLinkParams;
};
}
}
