/*
*  
*  $Id: comandoexportacion.h $
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
#include <api/autoptr.h>

#include <api/icommand.h>

#include <main/gui/export/datospersistentesexportacion.h>

namespace GNC {
	namespace GCS {
		class IVista;
	}
}

namespace GADAPI {
	class ComandoExportacion;

	class ComandoExportacionParams : public GNC::GCS::IComandoParams {
	public:
		ComandoExportacionParams(GNC::GCS::Ptr<GNC::GUI::TipoWizardExportacion>& pDatosPersistentes);

		virtual ~ComandoExportacionParams();

		GNC::GCS::Ptr<GNC::GUI::TipoWizardExportacion> m_pDatosPersistentes;
		std::list<std::string> m_outputPaths;
		std::string m_Error;
		bool m_hasError;
		std::string m_tempDir;
	};

	class ComandoExportacion : public GNC::GCS::IComando {
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
