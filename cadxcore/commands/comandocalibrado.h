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
#include <api/ientorno.h>
#include <exception>
#include "inotificadorcalibrado.h"
//#include <AVL.h>
//
//
//namespace GNC{
//	namespace GUI{
//		struct TPuntoCalibrado;
//	}
//}
//
//namespace GADAPI {
//
//	class CalibradoParamsException : public GNC::GCS::IException {
//	public:
//		CalibradoParamsException(std::string& message) throw() {m_message=message;}
//		 ~CalibradoParamsException() throw() {}
//		 std::string GetMensage(){return m_message;}
//	protected:
//		 std::string m_message;
//	};
//
//
//	class ComandoCalibradoParams : public IComandoParams {
//	public:
//		typedef MEF_ImagenRGBLight TImagenCalibrado;
//		TImagenCalibrado m_imagen;
//		double spacing[3];
//		std::list<GNC::GUI::TPuntoCalibrado> m_Puntos;
//		INotificadorCalibrado* m_pNotificador;
//
//		ComandoCalibradoParams(vtkImageData* pImagenOriginal,int slice,INotificadorCalibrado* pNotificador) ;
//
//		~ComandoCalibradoParams();
//
//	};
//
//	class ComandoCalibrado : public IComando {
//	public:
//		ComandoCalibrado(ComandoCalibradoParams* pParams);
//		~ComandoCalibrado();
//
//		void Execute();
//
//		void Abort();
//		void Update();
//
//		void SetNotificadorProgreso(GNC::INotificadorProgreso* pNotificador);
//		void SetNotificadorProgresoAlternativo(GNC::INotificadorProgreso* pNotificador);
//
//		void NotificarProgreso(float progresoNormalizado,const std::string &texto);
//
//		void LiberarRecursos();
//
//	protected:
//		ComandoCalibradoParams* m_pCalibradoParams;
//		GNC::INotificadorProgreso* m_pNotificadorProgresoAlternativo;
//	};
//}
