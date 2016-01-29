/*
*  
*  $Id: comandocalibrado.h $
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
