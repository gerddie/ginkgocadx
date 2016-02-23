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
//#define _GINKGO_TRACE
#include <api/globals.h>
#include <api/icommand.h>
#include <api/ivista.h>
#include <api/dicom/imodelodicom.h>
#include <api/controllers/ipacscontroller.h>

#include <main/controllers/dcmtk/dicomservers.h>



namespace GIL{
	class IModeloIntegracion;
	namespace DICOM{
		class IPACSController;
	}
}

namespace GADAPI {
	class PACSDownloadCommandParams : public GNC::GCS::IComandoParams {

	public:
		//download only specifying 
		PACSDownloadCommandParams(const std::string& serverSeleccionado, const GIL::DICOM::DicomDataset& base, bool link = false, bool silent = false);
		virtual ~PACSDownloadCommandParams();

	public:
		std::string              m_serverSeleccionado;
		bool					 m_link;
		bool					 m_silent;
		std::string              m_error;
		GNC::GCS::Ptr<IModeloDicom> m_pModelo;
		GIL::DICOM::DicomDataset m_base;
	};



	class EXTAPI PACSDownloadCommand : public GNC::GCS::IComando {
	public:
		PACSDownloadCommand(PACSDownloadCommandParams* pParams);

    protected:
		virtual void Execute();
		virtual void Update();

    public:

		virtual bool NotificarProgreso(float progresoNormalizado, const std::string &texto);

		void LiberarRecursos();

		PACSDownloadCommandParams* m_pPACSParams;
	};
}
