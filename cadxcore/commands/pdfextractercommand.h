/*
 *  
 *  $Id: PDFExtracterCommandParameters.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#pragma once
//#define _GINKGO_TRACE
#include <vector>
#include <api/globals.h>
#include <api/icommand.h>


namespace GADAPI {
	class PDFExtracterCommandParameters : public GNC::GCS::IComandoParams {
	public:
		PDFExtracterCommandParameters(const std::list<std::string>& rutas, const std::list<std::string>& dests = std::list<std::string>(), bool forOpening = true) ;

		virtual ~PDFExtracterCommandParameters() ;

	public:
		std::vector<std::string> m_pathsOriginal;
		std::vector<std::string> m_pathsExtracted;
		bool m_error;
		bool m_ForOpening;
	};


	class PDFExtracterCommand : public GNC::GCS:: IComando {
	public:
		PDFExtracterCommand(PDFExtracterCommandParameters* pParams);

		protected:
		virtual void Execute();

		virtual void Update();

		virtual void OnAbort();

		void LiberarRecursos();

		PDFExtracterCommandParameters* m_pExtracterParameters;
	};
}
