/*
*  
*  $Id: comandopacs.h $
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
