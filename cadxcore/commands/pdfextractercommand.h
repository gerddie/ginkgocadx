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
