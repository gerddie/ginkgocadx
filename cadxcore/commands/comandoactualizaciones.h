/*
*  
*  $Id: comandoactualizaciones.h $
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
#include <api/icommand.h>
#include <api/autoptr.h>

namespace GNC {
	namespace GCS {
		class IVista;
	}
}

namespace GNC {
	class ComandoComprobarActualizaciones;

	class ComandoComprobarActualizacionesParams : public GNC::GCS::IComandoParams {
	public:
		ComandoComprobarActualizacionesParams(const std::string& url, bool informar = false);

		virtual ~ComandoComprobarActualizacionesParams();

		std::string                     m_URL;
		bool                            m_Informar;

	};

	class ComandoComprobarActualizaciones : public GNC::GCS::IComando {
	public:

		ComandoComprobarActualizaciones(ComandoComprobarActualizacionesParams* pParams);

		const std::string& GetError() const;
		const std::string& GetVersion() const;
		const std::string& GetDescription() const;
		int GetRevision() const;

    protected:

		virtual void Execute();
		virtual void Update();

	protected:

		ComandoComprobarActualizacionesParams* m_pComprobarActualizacionesParams;
		std::string                            m_Error;

	private:
		std::string m_Version;
		int         m_Revision;
		std::string m_Description;
		std::string m_URL;
	};
}
