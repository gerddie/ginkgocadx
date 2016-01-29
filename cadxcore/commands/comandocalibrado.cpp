/*
 *  $Id: comandocalibrado.cpp $
 *  Proyecto Ginkgo
 *
 *  Copyright 2008 MetaEmotion S.L. All rights reserved.
 *
 */
//#define _GINKGO_TRACE
//#define DUMP_IMAGENES
//#include <api/globals.h>
//#include <api/controllers/ieventscontroller.h>
//#include <main/dialogos/calibrado/datospersistentescalibrado.h>
//
//#include <main/controllers/configurationcontroller.h>
//#include <wx/filename.h>
//#include <wx/file.h>
//#include <wx/dir.h>
//#include <main/controllers/configurationcontroller.h>
//
//#ifdef __DEPRECATED
//#undef __DEPRECATED
//#endif
//
//#include <vtkSmartPointer.h>
//#include <vtkPointData.h>
//#include <vtkImageData.h>
//
//#include "comandocalibrado.h"
//#include "accioncalibrado.h"
//
//#include <vector>
//#include <sstream>
//
//
//#define IDC_CALIBRADO           72
//
//// Singleton de persistencia
//namespace GADAPI {
//
//	ComandoCalibradoParams::ComandoCalibradoParams(vtkImageData* pImagenOriginal, int slice,INotificadorCalibrado* pNotificador)
//	{
//		m_pNotificador = pNotificador;
//		//se pilla la conexion d filtrado y se transforma a una lista de MEF
//		{
//				pImagenOriginal->GetSpacing(spacing);
//				int numComponentes = pImagenOriginal->GetNumberOfScalarComponents();
//				int* dims = pImagenOriginal->GetDimensions();
//				if(slice>dims[2]){
//					std::string str ("No se puede realizar el procesamiento de esta imagen");
//					throw CalibradoParamsException(str);
//				}
//				double tval[3] = {0.0f, 0.0f, 0.0f};
//				vtkDataArray* scalars = pImagenOriginal->GetPointData()->GetScalars();
//
//				unsigned long off = 0;
//				if (numComponentes == 1) {
//
//				/*	TImagenMEFIntensidad2D* entrada = new TImagenMEFIntensidad2D(dims[0], dims[1]);
//
//					if (entrada->Altura < 1 || entrada->Anchura < 1) {
//						std::cerr << "Dimensiones de la imagen incorrectas" << std::endl;
//						return;
//					}
//					off = dims[0] * dims[1] * slice;
//					for (int y = entrada->Altura -1; y >= 0; --y) {
//						for (int x = 0; x < entrada->Anchura; ++x) {
//							scalars->GetTuple(off++, tval);
//							entrada->Pixel(x, y) = tval[0];
//						}
//					}*/
//					std::cerr << "De momento solo soportamos imagenes rgb" << std::endl;
//					std::string str ("Error al aplicar el filtro: Numero de componentes no soportado.\nSolo se soportan imagenes RGB.");
//					throw CalibradoParamsException(str);
//					return;
//				}
//				else if (numComponentes == 3) {
//					m_imagen.Redimensionar(dims[0], dims[1]);
//
//					if (m_imagen.Altura < 1 || m_imagen.Anchura < 1) {
//						std::cerr << "Dimensiones de la imagen incorrectas" << std::endl;
//						std::string str("Dimensiones de la imagen incorrectas.");
//						throw CalibradoParamsException(str);
//					}
//
//					if(dims[2]>1){
//						off = dims[0]*dims[1]*slice;
//					} else {
//						off = 0;
//					}
//					for (int y = m_imagen.Altura -1; y >= 0; --y) {
//						for (int x = 0; x < m_imagen.Anchura; ++x) {
//							scalars->GetTuple(off++, tval);
//							m_imagen.Pixel(x, y).r = tval[0];
//							m_imagen.Pixel(x, y).g = tval[1];
//							m_imagen.Pixel(x, y).b = tval[2];
//						}
//					}
//
//				}
//				else {
//					std::cerr << "Error al aplicar el filtro: Numero de componentes no soportado. Accion ignorada" << std::endl;
//					//lanzar excepcion
//					std::string str("Error al aplicar el filtro: Numero de componentes no soportado.\nSolo se soportan imagenes RGB.");
//					throw CalibradoParamsException(str);
//				}
//			}
//
//		GTRACE("<< ComandoCalibradoParams::ComandoCalibradoParams(): " << this);
//	};
//
//	ComandoCalibradoParams::~ComandoCalibradoParams()
//	{
//		GTRACE(">> ComandoCalibradoParams::~ComandoCalibradoParams(): " << this);
//		m_Puntos.clear();
//		m_pNotificador = NULL;
//		GTRACE("<< ComandoCalibradoParams::~ComandoCalibradoParams(): " << this);
//	};
//
//	ComandoCalibrado::ComandoCalibrado(ComandoCalibradoParams* pParams) : IComando(pParams)
//	{
//		GTRACE(">> ComandoCalibrado::ComandoCalibrado(): " << this);
//		m_pCalibradoParams = pParams;
//		m_pNotificadorProgresoAlternativo=NULL;
//		SetId(IDC_CALIBRADO);
//		GTRACE("<< ComandoCalibrado::ComandoCalibrado(): " << this);
//	}
//
//	ComandoCalibrado::~ComandoCalibrado() {
//		delete m_pCalibradoParams;
//		m_pNotificadorProgresoAlternativo=NULL;
//		GTRACE(">> ComandoCalibrado::~ComandoCalibrado(): " << this);
//		GTRACE("<< ComandoCalibrado::~ComandoCalibrado(): " << this);
//	}
//
//	void ComandoCalibrado::Execute()
//	{
//		//se pillan las imagenes y se cargan con MEF
//		//se pegan...
//		//m_pCalibradoParams->m_pPuntos
//		MEF_BufferLight<TPuntoAccionCalibrado> puntos;
//		Calibrado(m_pCalibradoParams->m_imagen, puntos,this);
//
//		if (puntos.NumElementos == 4) {
//			for (unsigned int i = 0; i < 4; ++i) {
//				m_pCalibradoParams->m_Puntos.push_back(GNC::GUI::TPuntoCalibrado(puntos.data[i].x * m_pCalibradoParams->spacing[0], (puntos.data[i].y - m_pCalibradoParams->m_imagen.Altura) * m_pCalibradoParams->spacing[1]));
//			}
//		}
//
//	}
//
//	void ComandoCalibrado::Update()
//	{
//		m_pCalibradoParams->m_pNotificador->SetPuntosCalibrado(m_pCalibradoParams->m_Puntos);
//	}
//
//	void ComandoCalibrado::SetNotificadorProgreso(GNC::INotificadorProgreso* pNotificador) {
//		m_pNotificadorProgreso = pNotificador;
//	}
//
//	void ComandoCalibrado::SetNotificadorProgresoAlternativo(GNC::INotificadorProgreso* pNotificador) {
//		m_pNotificadorProgresoAlternativo = pNotificador;
//	}
//
//	void ComandoCalibrado::NotificarProgreso(float progresoNormalizado,const std::string &texto){
//		if(m_pNotificadorProgreso!=NULL){
//			m_pNotificadorProgreso->NotificarProgreso(progresoNormalizado,texto);
//		}
//		if(m_pNotificadorProgresoAlternativo != NULL){
//			m_pNotificadorProgresoAlternativo->NotificarProgreso(progresoNormalizado,texto);
//		}
//	}
//
//	void ComandoCalibrado::Abort()
//	{
//	}
//
//	void ComandoCalibrado::LiberarRecursos()
//	{
//	}
//
//}
