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

#include <algorithm>
#include <sstream>
#include <cmath>
#include <cstring>
#include <cairo/cairo.h>

//#define _GINKGO_TRACE
#include <api/globals.h>
#include <api/helpers/helpertexto.h>
#include <api/iwidgetsmanager.h>
#include <api/controllers/icontroladorlog.h>
#include <api/ievento.h>
#include <api/icontexto.h>
#include <api/math/geometry3d.h>
#include <api/westilo.h>
#include <api/dicom/dcmdictionary.h>
#include <api/iannotator.h>

#include <eventos/modificacionimagen.h>

#include <main/entorno.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/modalitysettingscontroller.h>
#include <api/controllers/icontroladorpermisos.h>

#include "wanotacionesquina.h"

#include <vtkgl.h>
#include <vtkImageData.h>
#include <vtkImageActor.h>
#include <vtkPointData.h>
#include <vtk/vtkginkgoimageviewer.h>
#include "striptexthelper.h"

namespace GNC {
	const char* PatientPositionOrientation[10*3*3] = {
		"",  "",  "",    "", "",  "",    "", "",  "", // N/A
		"R", "", "L",   "A", "", "P",   "I", "", "S", // HFS
		"L", "", "R",   "P", "", "A",   "I", "", "S", // HFP
		"A", "", "P",   "L", "", "R",   "I", "", "S", // HFDR
		"P", "", "A",   "R", "", "L",   "I", "", "S", // HFDL
		"P", "", "A",   "L", "", "R",   "S", "", "I", // FFDR
		"A", "", "P",   "R", "", "L",   "S", "", "I", // FFDL
		"L", "", "R",   "A", "", "P",   "S", "", "I", // FFS
		"R", "", "L",   "P", "", "A",   "S", "", "I", // FFP
		"?", "", "?",   "?", "", "?",   "?", "", "?", // Unknown
	};	
}

namespace GNC {
	namespace GCS {
		namespace Widgets {

			//----------------------------------------------------------------------------------------------------
			class OrientationMarks
			{
			public:
				typedef enum TOrientationPosition {
					TP_Top,
					TP_Right,
					TP_Bottom,
					TP_Left,
					TP_NumPosiciones
				} TPosicionAnotacion;
				OrientationMarks() {
					m_FontSizeProp = GNC::GCS::IControladorPermisos::Instance()->Get("style", "cornerAnnotation.fontSize").ObtenerValor<float>();
					m_StripTextRenderer.Update("RLAPISHF", GNC::GCS::GLHelper::TColor(ANNOTATOR_TEXT_COLOR), m_FontSizeProp, ANNOTATION_TEXT_WITH_SHADOW);
				}

				~OrientationMarks()
				{
				}

				bool SetAnotation(TOrientationPosition index, const std::string& str)
				{
					if (m_TextoAnotacion[index] != str) {
						m_TextoAnotacion[index] = str;
						return true;
					}
					else {
						return false;
					}
				}


				void Render(GNC::GCS::Contexto3D* /*c*/, GNC::GCS::Vector m_RectViewport[2])
				{
					GNC::GCS::Vector pos;
					//
					pos = GNC::GCS::Vector( (0.5 * m_RectViewport[1].x), 2.0).Redondeado();
					m_StripTextRenderer.Render(m_TextoAnotacion[OrientationMarks::TP_Top], pos, true);

					pos = GNC::GCS::Vector(10.0, (0.5 * m_RectViewport[1].y)).Redondeado();
					m_StripTextRenderer.Render(m_TextoAnotacion[OrientationMarks::TP_Left], pos, true);

					pos = GNC::GCS::Vector((0.5 * m_RectViewport[1].x), m_RectViewport[1].y - m_StripTextRenderer.GetTextHeigh() - 10.0 ).Redondeado();
					m_StripTextRenderer.Render(m_TextoAnotacion[OrientationMarks::TP_Bottom], pos, true);

					pos = GNC::GCS::Vector( m_RectViewport[1].x - 4.0,  (0.5 * m_RectViewport[1].y) ).Redondeado();
					m_StripTextRenderer.Render(m_TextoAnotacion[OrientationMarks::TP_Right], pos, false);
					//
				}
				void LiberarRecursos()
				{
					m_StripTextRenderer.Destroy();
				}

				std::string m_TextoAnotacion[4];
				GNC::GCS::Widgets::StripTextRenderer m_StripTextRenderer;
				float m_FontSizeProp;
			};

			/**
			annotationn items
			*/
			class AnnotationItem 
			{
			public:
				AnnotationItem(): isModified(true), value("")
				{
				}

				AnnotationItem(const std::string& value): isModified(true)
				{
					this->value = value;
				}

				AnnotationItem(const AnnotationItem& other) {
					(*this) = other;
				}

				AnnotationItem& operator = (const AnnotationItem& o)
				{
					this->isModified = o.isModified;
					this->value = o.value;
					return *this;
				}

				void setValue(const std::string& value)
				{
					if (value != this->value) {
						isModified = true;
					}
				}

			protected:
				bool isModified;
				std::string value;

			};

			class Anotaciones {
			public:
				typedef std::map<std::string, AnnotationItem> TAnnotationItemDictionary;

				Anotaciones(GNC::GCS::IWidgetsRenderer* pRenderer)
				{
					m_Correcta            = false;
					m_RecalcularTamOptimo = false;
					m_pRenderer           = pRenderer;
					m_Modificada          = false;
					for (int i = 0; i < GNC::GCS::AnnotationSettings::TP_NumPos; i++)
					{
						m_TexturaModificada[i] = false;
					}

					m_FontSizeProp = GNC::GCS::IControladorPermisos::Instance()->Get("style", "cornerAnnotation.fontSize").ObtenerValor<float>();

					m_Alineacion[GNC::GCS::AnnotationSettings::TP_TopLeft] = GNC::GCS::Widgets::HelperTexto::TA_Izquierda;
					m_Alineacion[GNC::GCS::AnnotationSettings::TP_TopRight] = GNC::GCS::Widgets::HelperTexto::TA_Derecha;
					m_Alineacion[GNC::GCS::AnnotationSettings::TP_BottomLeft] = GNC::GCS::Widgets::HelperTexto::TA_Izquierda;
					m_Alineacion[GNC::GCS::AnnotationSettings::TP_BottomRight] = GNC::GCS::Widgets::HelperTexto::TA_Derecha;

					m_pFontOptions = cairo_font_options_create();
					m_TexturaMetrica.Redimensionar(2, 2);
					cairo_get_font_options(m_TexturaMetrica.cr, m_pFontOptions);
					cairo_font_options_set_antialias (m_pFontOptions, CAIRO_ANTIALIAS_NONE);
					cairo_set_font_options(m_TexturaMetrica.cr, m_pFontOptions);
				}

				~Anotaciones()
				{
					cairo_font_options_destroy(m_pFontOptions);
					m_pFontOptions = NULL;
					//std::cout << "Destruyendo recursos de " << m_pRenderer << std::endl;
				}

				void Recalculate(GNC::GCS::Contexto3D* c, const GNC::GCS::Ptr<GNC::GCS::IAnnotator>& pAnnotator)
				{
					if (!pAnnotator.IsValid()) {
						m_Correcta = false;
						return;
					}
					//firstly get modality to get the pattern...
					const std::string modality = pAnnotator->GetTagValue(c, GKDCM_Modality);

					GNC::GCS::Ptr<GNC::GCS::DefaultModalitySettings> pModalitySettings = GNC::GCS::ModalitySettingsController::Instance()->getModalitySettings(modality);
					if (!pModalitySettings.IsValid()) {
						pModalitySettings = GNC::GCS::ModalitySettingsController::Instance()->getModalitySettings("OT");
						if (!pModalitySettings.IsValid()) {
							LOG_WARN("WCornerAnnotations", "settings for default modality OT not found");
							m_Correcta = false;
							return;
						}
					}

					const GNC::GCS::Ptr<GNC::GCS::AnnotationSettings>& pAnnotationSettings = pModalitySettings->getAnnotationSettings();

					for (int i = 0; i < GNC::GCS::AnnotationSettings::TP_NumPos; ++i) {
						//recalculate annotation
						SetAnotation((GNC::GCS::AnnotationSettings::TAnnotationPosition)i, pAnnotationSettings->getAnnotation((GNC::GCS::AnnotationSettings::TAnnotationPosition)i), c, pAnnotator);
					}

					m_Correcta = true;
				}


				void Redimensionar(double* viewport)
				{
					bool redimensionado = false;
					if (m_RectViewport[0].x != viewport[0])
					{
						m_RectViewport[0].x = viewport[0];
						redimensionado = true;
					}
					if (m_RectViewport[0].y != viewport[1])
					{
						m_RectViewport[0].y = viewport[1];
						redimensionado = true;
					}
					if (m_RectViewport[1].x != viewport[2])
					{
						m_RectViewport[1].x = viewport[2];
						redimensionado = true;
					}
					if (m_RectViewport[1].y != viewport[3])
					{
						m_RectViewport[1].y = viewport[3];
						redimensionado = true;
					}
					if (redimensionado) {
						m_TamViewPort = (m_RectViewport[1] - m_RectViewport[0]).ValorAbsoluto() * PROPVIEWPORT;
						m_RecalcularTamOptimo = true;
					}
				}

			public:

				void ModificarTextura(int i, bool modificada)
				{
					m_TexturaModificada[i] = modificada;
					if (modificada) {
						Modificar(true);
						m_RecalcularTamOptimo = true;
					}

				}

				void Modificar(bool modificada)
				{
					m_Modificada = modificada;
					if (m_Modificada) {
						m_pRenderer->Modificar(true);
					}
				}

				bool EstaModificada()
				{
					return m_Modificada;
				}

				bool SetAnotation(GNC::GCS::AnnotationSettings::TAnnotationPosition index, const std::string& str, GNC::GCS::Contexto3D* c, const GNC::GCS::Ptr<GNC::GCS::IAnnotator>& pAnnotator)
				{
					//parse annotation...
					std::ostringstream annotationStream;
					unsigned initialPos = 0;
					int defValuePos,formatPos = 0;
					std::ostringstream::pos_type lastEndl = 0;
					std::string tmp;
					for (unsigned curPos = 0; curPos < str.length(); ++curPos) {
						if (str[curPos] == '$') {
							//copy from initial position...
							annotationStream << str.substr(initialPos, curPos - initialPos);
							initialPos = curPos;
							if (curPos + 1 < str.length() && str[curPos+1] == '{') {
								//search for '}'
								defValuePos = -1;
								formatPos = -1;
								bool isTag = false;
								for (;curPos < str.length(); ++curPos) {
									if (str[curPos] == '}') {
										//key found...
										if (pAnnotator.IsValid()) {
											if (isTag) {
												if (defValuePos > 0) {
													if (formatPos > 0)
													{
														std::string lowerTag = str.substr(initialPos + 2, defValuePos - initialPos - 3);
														std::transform(lowerTag.begin(), lowerTag.end(), lowerTag.begin(), ::tolower);
														annotationStream << pAnnotator->GetTagValue(c, lowerTag, str.substr(defValuePos, formatPos - defValuePos), str.substr(formatPos, curPos - formatPos));
													}
													else
													{
														std::string lowerTag = str.substr(initialPos + 2, defValuePos - initialPos - 3);
														std::transform(lowerTag.begin(), lowerTag.end(), lowerTag.begin(), ::tolower);
														annotationStream << pAnnotator->GetTagValue(c, lowerTag, str.substr(defValuePos, curPos - defValuePos));
													}
												} else {
													std::string lowerTag = str.substr(initialPos + 2, curPos - initialPos -2);
													std::transform(lowerTag.begin(), lowerTag.end(), lowerTag.begin(), ::tolower);
													annotationStream << pAnnotator->GetTagValue(c, lowerTag);
												}
											} else {
												tmp = pAnnotator->GetAnnotationValue(c, str.substr(initialPos, curPos - initialPos + 1));
												if (tmp == "\n") {
													if (annotationStream.tellp() != lastEndl) {
														annotationStream << tmp;
														lastEndl = annotationStream.tellp();
													}
												} else {
													annotationStream << tmp;
												}
											}											
										} else {
											//if no anotator..
											//copy from initial position...
											annotationStream << str.substr(initialPos, curPos - initialPos + 1);
										}
										break;
									} else if (str[curPos] == ':') {
										//this could be defvalue or format
										if (defValuePos > 0)
										{
											formatPos = curPos + 1;
										} 
										else
										{
											defValuePos = curPos + 1;
										}
									} else if (str[curPos] == '|') {
										isTag = true;
									}
								}
								initialPos = curPos + 1;
							}
						}						
					}
					if (initialPos < str.length()) {
						annotationStream << str.substr(initialPos, str.length() - initialPos);
					}

					std::string ann = annotationStream.str();

					if (m_TextoAnotacion[index] != ann) {
						m_TextoAnotacion[index] = ann;
						ModificarTextura(index, true);
						return true;
					}
					else {
						return false;
					}
				}

				void RecalcularTamOptimo()
				{
					if (m_RecalcularTamOptimo) {
						m_RecalcularTamOptimo = false;
						m_TamCajaMaxima = m_TamViewPort;
						m_TamFuente = m_FontSizeProp;
						for (int i = 0; i < GNC::GCS::AnnotationSettings::TP_NumPos; ++i) {
							m_TamTexto[i].Asignar(0.0f, 0.0f);
							if (m_TextoAnotacion[i].size() > 0) {
								cairo_select_font_face(m_TexturaMetrica.cr, WIDGETS_TEXT_FONT_FAMILY, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
								cairo_set_font_size(m_TexturaMetrica.cr, m_FontSizeProp);
								m_TamTexto[i] = GNC::GCS::GLHelper::calcularBoundingBox(m_TexturaMetrica, m_TextoAnotacion[i], ANNOTATION_TEXT_WITH_SHADOW, -1.0f);
								m_TamCajaMaxima.AsignarMaximos(m_TamTexto[i]);
							}
						}
						if (!m_TamCajaMaxima.EsNulo()) {
							m_TamFuente = ((m_TamViewPort * m_FontSizeProp) / m_TamCajaMaxima).Redondear().ComponenteMinima(); // Calculo del tamanyo de fuente maxima
							m_TamFuente = std::max(TAMFUENTE_MIN, m_TamFuente);

							for (int i = 0; i < GNC::GCS::AnnotationSettings::TP_NumPos; ++i) {
								m_TamTexto[i] *=  (m_TamFuente / m_FontSizeProp);
								m_TamTexto[i].RedondearAlza();
								m_Texturas[i].Redimensionar(m_TamTexto[i].x, m_TamTexto[i].y);
								m_TexturaModificada[i] = true;
							}
						}
						else{
							for (int i = 0; i < GNC::GCS::AnnotationSettings::TP_NumPos; ++i) {
								m_TamTexto[i].RedondearAlza();
								m_Texturas[i].Redimensionar(m_TamTexto[i].x, m_TamTexto[i].y);
								m_TexturaModificada[i] = true;
							}
						}
					}
				}

				void Actualizar(const GNC::GCS::GLHelper::TColor& color)
				{
					RecalcularTamOptimo();

					for (int i = 0; i < GNC::GCS::AnnotationSettings::TP_NumPos; ++i)
					{
						const std::string& texto = m_TextoAnotacion[i];

						if ( m_TexturaModificada[i] && m_Texturas[i].EsValida() && (m_TextoAnotacion[i].size() > 0) )
						{
							cairo_select_font_face(m_Texturas[i].cr, WIDGETS_TEXT_FONT_FAMILY, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
							cairo_set_font_size(m_Texturas[i].cr, m_TamFuente);

							cairo_get_font_options(m_Texturas[i].cr, m_pFontOptions);
							cairo_font_options_set_antialias (m_pFontOptions, CAIRO_ANTIALIAS_NONE);
							cairo_set_font_options(m_Texturas[i].cr, m_pFontOptions);
							cairo_set_operator(m_Texturas[i].cr, CAIRO_OPERATOR_SOURCE);

							cairo_set_source_rgba (m_Texturas[i].cr, 1.0f, 1.0f, 0.0f, 0.0f);
							cairo_paint(m_Texturas[i].cr);
							m_TamTexto[i] = GNC::GCS::GLHelper::dibujarTexto(m_Texturas[i], texto, color, ANNOTATION_TEXT_WITH_SHADOW, m_TamCajaMaxima.x, m_Alineacion[i]);
							m_TexturaModificada[i] = false;
						}
					}
				}

				void Render(GNC::GCS::Contexto3D* c)
				{
					GTRACE("Anotaciones::Render(" << c->GetRenderer() << ")");
					GNC::GCS::Vector pos;

					pos = GNC::GCS::Vector(0.0, 0.0).Redondeado();
					m_Texturas[GNC::GCS::AnnotationSettings::TP_TopLeft].Actualizar();
					m_Texturas[GNC::GCS::AnnotationSettings::TP_TopLeft].Render(c, pos, false, false, 0);

					pos = GNC::GCS::Vector(m_RectViewport[1].x - m_TamTexto[GNC::GCS::AnnotationSettings::TP_TopRight].x, 0.0).Redondeado();
					m_Texturas[GNC::GCS::AnnotationSettings::TP_TopRight].Actualizar();
					m_Texturas[GNC::GCS::AnnotationSettings::TP_TopRight].Render(c, pos, false, false, 0);

					pos = GNC::GCS::Vector(0.0, m_RectViewport[1].y - m_TamTexto[GNC::GCS::AnnotationSettings::TP_BottomLeft].y).Redondeado();
					m_Texturas[GNC::GCS::AnnotationSettings::TP_BottomLeft].Actualizar();
					m_Texturas[GNC::GCS::AnnotationSettings::TP_BottomLeft].Render(c, pos, false, false, 0);

					pos = GNC::GCS::Vector(m_RectViewport[1].x - m_TamTexto[GNC::GCS::AnnotationSettings::TP_BottomRight].x, m_RectViewport[1].y - m_TamTexto[GNC::GCS::AnnotationSettings::TP_BottomRight].y).Redondeado();
					m_Texturas[GNC::GCS::AnnotationSettings::TP_BottomRight].Actualizar();
					m_Texturas[GNC::GCS::AnnotationSettings::TP_BottomRight].Render(c, pos, false, false, 0);

					Modificar(false);
				}

				void LiberarRecursos()
				{
					for (int i = 0; i < GNC::GCS::AnnotationSettings::TP_NumPos; i++)
					{
						m_Texturas[i].Destruir();
					}
				}

				GNC::GCS::Vector m_RectViewport[2];
				GNC::GCS::Vector m_TamViewPort;

				GNC::GCS::Vector m_TamCajaMaxima;

				GNC::GCS::TexturaCairo m_TexturaMetrica; // Contexto usado solamente para medir.

				bool m_TexturaModificada[GNC::GCS::AnnotationSettings::TP_NumPos];

				std::string m_TextoAnotacion[GNC::GCS::AnnotationSettings::TP_NumPos];

				GNC::GCS::Vector m_TamTexto[GNC::GCS::AnnotationSettings::TP_NumPos];

				GNC::GCS::TexturaCairo m_Texturas[GNC::GCS::AnnotationSettings::TP_NumPos];

				bool m_RecalcularTamOptimo;

				cairo_font_options_t* m_pFontOptions;

				bool m_Correcta;
				float m_TamFuente;
				float m_FontSizeProp;

				GNC::GCS::Widgets::HelperTexto::TAlineamiento m_Alineacion[GNC::GCS::AnnotationSettings::TP_NumPos];

				GNC::GCS::IWidgetsRenderer* m_pRenderer;
				TAnnotationItemDictionary m_annotationDicionary;
				bool m_Modificada;

			};

			//----------------------------------------------------------------------------------------------------
			//region "Textura de anotacion"
			class EstadoInterno {
			public:
				typedef std::map<GNC::GCS::IWidgetsRenderer*,Anotaciones*> MapaAnotaciones;
				typedef std::map<GNC::GCS::IWidgetsRenderer*,OrientationMarks*> OrientationMap;

				MapaAnotaciones       m_Anotaciones;
				OrientationMap		  m_Orientations;

				inline OrientationMarks* GetOrientation(GNC::GCS::IWidgetsRenderer* key)
				{
					OrientationMarks* res = NULL;

					OrientationMap::iterator it = m_Orientations.find(key);
					if(it != m_Orientations.end()) {
						res = (*it).second;
					}
					else {
						m_Orientations[key] = res = new OrientationMarks();
					}

					return res;
				}

				inline Anotaciones* Get(GNC::GCS::IWidgetsRenderer* key)
				{
					Anotaciones* res = NULL;

					MapaAnotaciones::iterator it = m_Anotaciones.find(key);
					if(it != m_Anotaciones.end()) {
						res = (*it).second;
					}
					else {
						m_Anotaciones[key] = res = new Anotaciones(key);
					}

					return res;
				}

				inline void Delete(GNC::GCS::IWidgetsRenderer* key)
				{
					{
						MapaAnotaciones::iterator it = m_Anotaciones.find(key);
						if(it != m_Anotaciones.end()) {
							(*it).second->LiberarRecursos();
							delete (*it).second;
							m_Anotaciones.erase(it);
						}
					}
					{
						OrientationMap::iterator it = m_Orientations.find(key);
						if(it != m_Orientations.end()) {
							(*it).second->LiberarRecursos();
							delete (*it).second;
							m_Orientations.erase(it);
						}
					}
				}

			};

			/**
			* Gets the orientation index given string. Default is zero.
			* strPos: The patient position string (HFS, HFP...)
			* defautlunknown: When orientation index is empty, set the default index to unknown ("?") or empty ("") values
			**/
			int GetPatientPositionOrientationIndex(const std::string& strPos, bool defaultunknown) {

				if (strcmp(strPos.c_str(), "HFS") == 0) {
					return 1;
				}
				else if (strcmp(strPos.c_str(), "HFP") == 0) {
					return 2;
				}
				else if (strcmp(strPos.c_str(), "HFDR") == 0) {
					return 3;
				}
				else if (strcmp(strPos.c_str(), "HFDL") == 0) {
					return 4;
				}
				else if (strcmp(strPos.c_str(), "FFDR") == 0) {
					return 5;
				}
				else if (strcmp(strPos.c_str(), "FFDL") == 0) {
					return 6;
				}
				else if (strcmp(strPos.c_str(), "FFS") == 0) {
					return 7;
				}
				else if(strcmp(strPos.c_str(), "FFP") == 0) {
					return 8;
				}
				if (defaultunknown) {
					return 9;
				}
				else {
					return 0;
				}
			}

			/**
			* Gets the patient orientation char ("R", "L", "S"...) from:
			* patientPosIndex: The patient position index: get if from GetPatientPositionOrientationIndex() call.
			* axis: The axis: 0==x, 1==y, 2==z
			* dir: The direction: -1 == decreasing, +1 == decreasing
			**/
			const char* GetPatientPositionOrientationChar(int patientPosIndex, int axis, int dir) {
				const int offPos = 9 * patientPosIndex;
				const int offAxis = 3 * axis;
				const int offDir = 1 + dir;

				return PatientPositionOrientation[offPos + offAxis + offDir];
			}
		}
	}
}


//region "Constructor y destructor"

GNC::GCS::Widgets::WAnotador::WAnotador(IWidgetsManager* pManager, long vid, const char* nombre, long gid) : GNC::GCS::Widgets::IWidget(pManager, vid, nombre, gid)
{
	GNC::GCS::ConfigurationController::Instance()->readBoolUser("/GinkgoCore/Tools/CornerAnotations", "IsShown", m_Oculto, true);
	m_TopLevel = true;
	m_ReservaRecursos = true;
	m_Estado = new EstadoInterno();
	m_color = GNC::GCS::GLHelper::TColor(ANNOTATOR_TEXT_COLOR);
}

GNC::GCS::Widgets::WAnotador::~WAnotador()
{
	delete m_Estado;
}

//endregion

void GNC::GCS::Widgets::WAnotador::LiberarRecursos(GNC::GCS::IWidgetsRenderer* renderer)
{
	m_Estado->Delete(renderer);
}


//region "Interfaz generica"

void GNC::GCS::Widgets::WAnotador::OnMouseEvents(GNC::GCS::Events::EventoRaton& /*evento*/)
{

}

void GNC::GCS::Widgets::WAnotador::OnKeyEvents(GNC::GCS::Events::EventoTeclado& /*evento*/)
{
}

bool GNC::GCS::Widgets::WAnotador::HitTest(float , float , const GNC::GCS::Vector&)
{
	return false;
}

bool GNC::GCS::Widgets::WAnotador::HitTest(GNC::GCS::Vector* , int )
{
	return false;
}

void GNC::GCS::Widgets::WAnotador::Render(GNC::GCS::Contexto3D* c)
{
	if(m_Oculto){
		return;
	}

	double viewport[4] = {0.0f, 0.0f, 0.0f, 0.0f}; // { x, y, ancho, alto }. Convenio de coordenadas: {x, y} == {bottom, left}, {ancho, alto} == {top, right}
	glGetDoublev(GL_VIEWPORT, viewport);

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0, viewport[2] , viewport[3] , 0, -1, 1 );

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	Anotaciones* pAnotaciones = m_Estado->Get(c->GetRenderer());
	OrientationMarks* pOrientation = m_Estado->GetOrientation(c->GetRenderer());


	pAnotaciones->Redimensionar(viewport);

	pAnotaciones->Recalculate(c, m_pManager->GetAnnotator());

	RecalcularPosicion(pOrientation, c);


	pAnotaciones->Actualizar(m_color);

	pAnotaciones->Render(c);
	pOrientation->Render(c, pAnotaciones->m_RectViewport);


	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	glPopAttrib();
}

void GNC::GCS::Widgets::WAnotador::Seleccionar(bool )
{
}

void GNC::GCS::Widgets::WAnotador::Iluminar(bool )
{
}

void GNC::GCS::Widgets::WAnotador::Ocultar(bool oculto)
{
	if(oculto != m_Oculto){
		m_Oculto = oculto;
		Modificar(true);
	}
	GNC::GCS::ConfigurationController::Instance()->writeBoolUser("/GinkgoCore/Tools/CornerAnotations", "IsShown", m_Oculto);
}



//endregion

void GNC::GCS::Widgets::WAnotador::RecalcularPosicion(OrientationMarks* pAnotaciones, GNC::GCS::Contexto3D* c)
{
	std::ostringstream dirs[6]; // left, right, top, bottom, back, forward

	ComputeOrientation(c, dirs);
	pAnotaciones->SetAnotation(OrientationMarks::TP_Left, dirs[0].str());
	pAnotaciones->SetAnotation(OrientationMarks::TP_Right, dirs[1].str());
	pAnotaciones->SetAnotation(OrientationMarks::TP_Top, dirs[2].str());	
	pAnotaciones->SetAnotation(OrientationMarks::TP_Bottom, dirs[3].str());

}

void GNC::GCS::Widgets::WAnotador::ComputeOrientation(GNC::GCS::Contexto3D* c, std::ostringstream* dirs )
{
	GNC::GCS::Vector3D camPos;
	GNC::GCS::Vector3D camDir;
	GNC::GCS::Vector3D camUp;

	c->GetRenderer()->GetCamVectors(camPos.v, camDir.v, camUp.v);

	for (int i = 0; i < 3; ++i) {
		if (std::abs(camUp.v[i]) < 0.1) {
			camUp.v[i] = 0.0;
		}
		if (std::abs(camDir.v[i]) < 0.1) {
			camDir.v[i] = 0.0;
		}
	}

	GNC::GCS::Vector3D camCross = camDir.ProductoVectorial(camUp).Normalizado();

	for (int i = 0; i < 3; ++i) {
		if (std::abs(camCross.v[i]) < 0.1) {
			camCross.v[i] = 0.0;
		}
	}
	LOG_DEBUG("View", "---\nDir         : " << camDir);
	LOG_DEBUG("View", "Up          : " << camUp);

	GNC::GCS::Vector3D camOrient[3][2];
	camOrient[0][0] = camCross.Sign();            // izq
	camOrient[0][1] = -camOrient[0][0];           // der

	camOrient[1][0] = camUp.Stabilized().Sign();  // up
	camOrient[1][1] = -camOrient[1][0];           // down

	camOrient[2][0] = camDir.Stabilized().Sign(); // back
	camOrient[2][1] = -camOrient[2][0];           // front

	std::string patPosition;
	if (m_pManager->GetAnnotator().IsValid()) {
		patPosition = m_pManager->GetAnnotator()->GetTagValue(c, GKDCM_PatientPosition);
	}
	if (patPosition.empty()) {
		
		std::string viewPosition = m_pManager->GetAnnotator()->GetTagValue(c, GKDCM_ViewPosition);
		std::string patientOrientation = m_pManager->GetAnnotator()->GetTagValue(c, GKDCM_PatientOrientation);

		LOG_DEBUG("View", "Computing orientation from [ ViewPosition = " << viewPosition << ", PatientOrientation = " << patientOrientation << " ]");

		if (!viewPosition.empty() && !patientOrientation.empty()) {			

			if (viewPosition.find("CC") != std::string::npos) {
				LOG_DEBUG("View", "Warning: CC ViewPosition not supported yet");
				return;
			}

			std::string DynamicPatientPositionOrientation[] = {
				"",  "",  "",    "", "", "",   "", "", "",
			};
			char direction[2] = {'?', '?'}; // Annotations for each axis direction (columns, rows)

			std::istringstream pis(patientOrientation);

			if (pis.eof()) { // Read first char (pixel columns orientation)
				return;
			}
			direction[0] = (char) pis.get();
			if (pis.eof()) {
				return;
			}
			pis.get(); // Read separator
			if (pis.eof()) {
				return;
			}
			direction[1] = (char) pis.get(); // Read first char (pixel rows orientation)

			for (int i = 0; i < 2; ++i) { // Compute directions dinamically
				int curPos = 1 + i*3;

				switch (direction[i]) {
				case 'A':
					DynamicPatientPositionOrientation[curPos - 1] = "P";
					DynamicPatientPositionOrientation[curPos + 1] = "A";
					break;
				case 'P':
					DynamicPatientPositionOrientation[curPos - 1] = "A";
					DynamicPatientPositionOrientation[curPos + 1] = "P";
					break;
				case 'R':
					DynamicPatientPositionOrientation[curPos - 1] = "L";
					DynamicPatientPositionOrientation[curPos + 1] = "R";
					break;
				case 'L':
					DynamicPatientPositionOrientation[curPos - 1] = "R";
					DynamicPatientPositionOrientation[curPos + 1] = "L";
					break;
				case 'H':
					DynamicPatientPositionOrientation[curPos - 1] = "F";
					DynamicPatientPositionOrientation[curPos + 1] = "H";
					break;
				case 'F':
					DynamicPatientPositionOrientation[curPos - 1] = "H";
					DynamicPatientPositionOrientation[curPos + 1] = "F";
					break;
				default:
					break;
				}
			}

			if (viewPosition == "AP") {
				DynamicPatientPositionOrientation[6] = "A";
				DynamicPatientPositionOrientation[8] = "P";
			}
			else if (viewPosition == "PA") {
				DynamicPatientPositionOrientation[6] = "P";
				DynamicPatientPositionOrientation[8] = "A";
			}


			const int offPos = 0;
			const int offX = 0;
			const int offY = 3;
			const int offZ = 6;
			const int offSignNeg = 0;
			const int offSignZero = 1;
			const int offSignPos = 2;

			// Left X:
			if (camOrient[0][1][0] == 1.0) {
				dirs[0] << DynamicPatientPositionOrientation[offPos + offX + offSignPos];
			}
			else if (camOrient[0][1][0] == -1.0) {
				dirs[0] << DynamicPatientPositionOrientation[offPos + offX + offSignNeg];
			}
			else {
				dirs[0] << DynamicPatientPositionOrientation[offPos + offX + offSignZero];
			}
			// Left Y:
			if (camOrient[0][1][1] == 1.0) {
				dirs[0] << DynamicPatientPositionOrientation[offPos + offY + offSignPos];
			}
			else if (camOrient[0][1][1] == -1.0) {
				dirs[0] << DynamicPatientPositionOrientation[offPos + offY + offSignNeg];
			}
			else {
				dirs[0] << DynamicPatientPositionOrientation[offPos + offY + offSignZero];
			}
			// Left Z:
			if (camOrient[0][1][2] == 1.0) {
				dirs[0] << DynamicPatientPositionOrientation[offPos + offZ + offSignPos];
			}
			else if (camOrient[0][1][2] == -1.0) {
				dirs[0] << DynamicPatientPositionOrientation[offPos + offZ + offSignNeg];
			}
			else {
				dirs[0] << DynamicPatientPositionOrientation[offPos + offZ + offSignZero];
			}

			// Right X:
			if (camOrient[0][0][0] == 1.0) {
				dirs[1] << DynamicPatientPositionOrientation[offPos + offX + offSignPos];
			}
			else if (camOrient[0][0][0] == -1.0) {
				dirs[1] << DynamicPatientPositionOrientation[offPos + offX + offSignNeg];
			}
			else {
				dirs[1] << DynamicPatientPositionOrientation[offPos + offX + offSignZero];
			}
			// Right Y:
			if (camOrient[0][0][1] == 1.0) {
				dirs[1] << DynamicPatientPositionOrientation[offPos + offY + offSignPos];
			}
			else if (camOrient[0][0][1] == -1.0) {
				dirs[1] << DynamicPatientPositionOrientation[offPos + offY + offSignNeg];
			}
			else {
				dirs[1] << DynamicPatientPositionOrientation[offPos + offY + offSignZero];
			}
			// Right Z:
			if (camOrient[0][0][2] == 1.0) {
				dirs[1] << DynamicPatientPositionOrientation[offPos + offZ + offSignPos];
			}
			else if (camOrient[0][0][2] == -1.0) {
				dirs[1] << DynamicPatientPositionOrientation[offPos + offZ + offSignNeg];
			}
			else {
				dirs[1] << DynamicPatientPositionOrientation[offPos + offZ + offSignZero];
			}

			// Top X:
			if (camOrient[1][0][0] == 1.0) {
				dirs[2] << DynamicPatientPositionOrientation[offPos + offX + offSignPos];
			}
			else if (camOrient[1][0][0] == -1.0) {
				dirs[2] << DynamicPatientPositionOrientation[offPos + offX + offSignNeg];
			}
			else {
				dirs[2] << DynamicPatientPositionOrientation[offPos + offX + offSignZero];
			}
			// Top Y:
			if (camOrient[1][0][1] == 1.0) {
				dirs[2] << DynamicPatientPositionOrientation[offPos + offY + offSignPos];
			}
			else if (camOrient[1][0][1] == -1.0) {
				dirs[2] << DynamicPatientPositionOrientation[offPos + offY + offSignNeg];
			}
			else {
				dirs[2] << DynamicPatientPositionOrientation[offPos + offY + offSignZero];
			}
			// Top Z:
			if (camOrient[1][0][2] == 1.0) {
				dirs[2] << DynamicPatientPositionOrientation[offPos + offZ + offSignPos];
			}
			else if (camOrient[1][0][2] == -1.0) {
				dirs[2] << DynamicPatientPositionOrientation[offPos + offZ + offSignNeg];
			}
			else {
				dirs[2] << DynamicPatientPositionOrientation[offPos + offZ + offSignZero];
			}

			// Bottom X:
			if (camOrient[1][1][0] == 1.0) {
				dirs[3] << DynamicPatientPositionOrientation[offPos + offX + offSignPos];
			}
			else if (camOrient[1][1][0] == -1.0) {
				dirs[3] << DynamicPatientPositionOrientation[offPos + offX + offSignNeg];
			}
			else {
				dirs[3] << DynamicPatientPositionOrientation[offPos + offX + offSignZero];
			}
			// Bottom Y:
			if (camOrient[1][1][1] == 1.0) {
				dirs[3] << DynamicPatientPositionOrientation[offPos + offY + offSignPos];
			}
			else if (camOrient[1][1][1] == -1.0) {
				dirs[3] << DynamicPatientPositionOrientation[offPos + offY + offSignNeg];
			}
			else {
				dirs[3] << DynamicPatientPositionOrientation[offPos + offY + offSignZero];
			}
			// Bottom Z:
			if (camOrient[1][1][2] == 1.0) {
				dirs[3] << DynamicPatientPositionOrientation[offPos + offZ + offSignPos];
			}
			else if (camOrient[1][1][2] == -1.0) {
				dirs[3] << DynamicPatientPositionOrientation[offPos + offZ + offSignNeg];
			}
			else {
				dirs[3] << DynamicPatientPositionOrientation[offPos + offZ + offSignZero];
			}

			// Extended
			// Back X:
			if (camOrient[2][1][0] == 1.0) {
				dirs[4] << DynamicPatientPositionOrientation[offPos + offX + offSignPos];
			}
			else if (camOrient[2][1][0] == -1.0) {
				dirs[4] << DynamicPatientPositionOrientation[offPos + offX + offSignNeg];
			}
			else {
				dirs[4] << DynamicPatientPositionOrientation[offPos + offX + offSignZero];
			}
			// Back Y:
			if (camOrient[2][1][1] == 1.0) {
				dirs[4] << DynamicPatientPositionOrientation[offPos + offY + offSignPos];
			}
			else if (camOrient[2][1][1] == -1.0) {
				dirs[4] << DynamicPatientPositionOrientation[offPos + offY + offSignNeg];
			}
			else {
				dirs[4] << DynamicPatientPositionOrientation[offPos + offY + offSignZero];
			}
			// Back Z:
			if (camOrient[2][1][2] == 1.0) {
				dirs[4] << DynamicPatientPositionOrientation[offPos + offZ + offSignPos];
			}
			else if (camOrient[2][1][2] == -1.0) {
				dirs[4] << DynamicPatientPositionOrientation[offPos + offZ + offSignNeg];
			}
			else {
				dirs[4] << DynamicPatientPositionOrientation[offPos + offZ + offSignZero];
			}

			// Front X:
			if (camOrient[2][0][0] == 1.0) {
				dirs[5] << DynamicPatientPositionOrientation[offPos + offX + offSignPos];
			}
			else if (camOrient[2][0][0] == -1.0) {
				dirs[5] << DynamicPatientPositionOrientation[offPos + offX + offSignNeg];
			}
			else {
				dirs[5] << DynamicPatientPositionOrientation[offPos + offX + offSignZero];
			}
			// Front Y:
			if (camOrient[2][0][1] == 1.0) {
				dirs[5] << DynamicPatientPositionOrientation[offPos + offY + offSignPos];
			}
			else if (camOrient[2][0][1] == -1.0) {
				dirs[5] << DynamicPatientPositionOrientation[offPos + offY + offSignNeg];
			}
			else {
				dirs[5] << DynamicPatientPositionOrientation[offPos + offY + offSignZero];
			}
			// Front Z:
			if (camOrient[2][0][2] == 1.0) {
				dirs[5] << DynamicPatientPositionOrientation[offPos + offZ + offSignPos];
			}
			else if (camOrient[2][0][2] == -1.0) {
				dirs[5] << DynamicPatientPositionOrientation[offPos + offZ + offSignNeg];
			}
			else {
				dirs[5] << DynamicPatientPositionOrientation[offPos + offZ + offSignZero];
			}

			LOG_DEBUG("View", "Annotation:");
			LOG_DEBUG("View", "    " << dirs[2].str() << "   ");
			LOG_DEBUG("View", "  " << dirs[0].str() << "   " << dirs[1].str() << " ");
			LOG_DEBUG("View", "    " << dirs[3].str() << "   ");

		}
	}
	else {
		LOG_DEBUG("View", "Computing orientation from [ PatientPosition = " << patPosition << " ]");
		int patientPosIndex = GetPatientPositionOrientationIndex(patPosition, false);


		LOG_DEBUG("View", "Current orientation:");
		LOG_DEBUG("View", "Left = " << camOrient[0][1] << ", Right = " << camOrient[0][0]);
		LOG_DEBUG("View", "Up  = " << camOrient[1][0] << ", Down = " << camOrient[1][1]);
		LOG_DEBUG("View", "Back  = " << camOrient[2][0] << ", Front = " << camOrient[2][1]);

		const int offPos = 9 * patientPosIndex;
		const int offX = 0;
		const int offY = 3;
		const int offZ = 6;
		const int offSignNeg = 0;
		const int offSignZero = 1;
		const int offSignPos = 2;

		// Left X:
		if (camOrient[0][1][0] == 1.0) {
			dirs[0] << PatientPositionOrientation[offPos + offX + offSignPos];
		}
		else if (camOrient[0][1][0] == -1.0) {
			dirs[0] << PatientPositionOrientation[offPos + offX + offSignNeg];
		}
		else {
			dirs[0] << PatientPositionOrientation[offPos + offX + offSignZero];
		}
		// Left Y:
		if (camOrient[0][1][1] == 1.0) {
			dirs[0] << PatientPositionOrientation[offPos + offY + offSignPos];
		}
		else if (camOrient[0][1][1] == -1.0) {
			dirs[0] << PatientPositionOrientation[offPos + offY + offSignNeg];
		}
		else {
			dirs[0] << PatientPositionOrientation[offPos + offY + offSignZero];
		}
		// Left Z:
		if (camOrient[0][1][2] == 1.0) {
			dirs[0] << PatientPositionOrientation[offPos + offZ + offSignPos];
		}
		else if (camOrient[0][1][2] == -1.0) {
			dirs[0] << PatientPositionOrientation[offPos + offZ + offSignNeg];
		}
		else {
			dirs[0] << PatientPositionOrientation[offPos + offZ + offSignZero];
		}

		// Right X:
		if (camOrient[0][0][0] == 1.0) {
			dirs[1] << PatientPositionOrientation[offPos + offX + offSignPos];
		}
		else if (camOrient[0][0][0] == -1.0) {
			dirs[1] << PatientPositionOrientation[offPos + offX + offSignNeg];
		}
		else {
			dirs[1] << PatientPositionOrientation[offPos + offX + offSignZero];
		}
		// Right Y:
		if (camOrient[0][0][1] == 1.0) {
			dirs[1] << PatientPositionOrientation[offPos + offY + offSignPos];
		}
		else if (camOrient[0][0][1] == -1.0) {
			dirs[1] << PatientPositionOrientation[offPos + offY + offSignNeg];
		}
		else {
			dirs[1] << PatientPositionOrientation[offPos + offY + offSignZero];
		}
		// Right Z:
		if (camOrient[0][0][2] == 1.0) {
			dirs[1] << PatientPositionOrientation[offPos + offZ + offSignPos];
		}
		else if (camOrient[0][0][2] == -1.0) {
			dirs[1] << PatientPositionOrientation[offPos + offZ + offSignNeg];
		}
		else {
			dirs[1] << PatientPositionOrientation[offPos + offZ + offSignZero];
		}

		// Top X:
		if (camOrient[1][0][0] == 1.0) {
			dirs[2] << PatientPositionOrientation[offPos + offX + offSignPos];
		}
		else if (camOrient[1][0][0] == -1.0) {
			dirs[2] << PatientPositionOrientation[offPos + offX + offSignNeg];
		}
		else {
			dirs[2] << PatientPositionOrientation[offPos + offX + offSignZero];
		}
		// Top Y:
		if (camOrient[1][0][1] == 1.0) {
			dirs[2] << PatientPositionOrientation[offPos + offY + offSignPos];
		}
		else if (camOrient[1][0][1] == -1.0) {
			dirs[2] << PatientPositionOrientation[offPos + offY + offSignNeg];
		}
		else {
			dirs[2] << PatientPositionOrientation[offPos + offY + offSignZero];
		}
		// Top Z:
		if (camOrient[1][0][2] == 1.0) {
			dirs[2] << PatientPositionOrientation[offPos + offZ + offSignPos];
		}
		else if (camOrient[1][0][2] == -1.0) {
			dirs[2] << PatientPositionOrientation[offPos + offZ + offSignNeg];
		}
		else {
			dirs[2] << PatientPositionOrientation[offPos + offZ + offSignZero];
		}

		// Bottom X:
		if (camOrient[1][1][0] == 1.0) {
			dirs[3] << PatientPositionOrientation[offPos + offX + offSignPos];
		}
		else if (camOrient[1][1][0] == -1.0) {
			dirs[3] << PatientPositionOrientation[offPos + offX + offSignNeg];
		}
		else {
			dirs[3] << PatientPositionOrientation[offPos + offX + offSignZero];
		}
		// Bottom Y:
		if (camOrient[1][1][1] == 1.0) {
			dirs[3] << PatientPositionOrientation[offPos + offY + offSignPos];
		}
		else if (camOrient[1][1][1] == -1.0) {
			dirs[3] << PatientPositionOrientation[offPos + offY + offSignNeg];
		}
		else {
			dirs[3] << PatientPositionOrientation[offPos + offY + offSignZero];
		}
		// Bottom Z:
		if (camOrient[1][1][2] == 1.0) {
			dirs[3] << PatientPositionOrientation[offPos + offZ + offSignPos];
		}
		else if (camOrient[1][1][2] == -1.0) {
			dirs[3] << PatientPositionOrientation[offPos + offZ + offSignNeg];
		}
		else {
			dirs[3] << PatientPositionOrientation[offPos + offZ + offSignZero];
		}

		// Extended
		// Back X:
		if (camOrient[2][1][0] == 1.0) {
			dirs[4] << PatientPositionOrientation[offPos + offX + offSignPos];
		}
		else if (camOrient[2][1][0] == -1.0) {
			dirs[4] << PatientPositionOrientation[offPos + offX + offSignNeg];
		}
		else {
			dirs[4] << PatientPositionOrientation[offPos + offX + offSignZero];
		}
		// Back Y:
		if (camOrient[2][1][1] == 1.0) {
			dirs[4] << PatientPositionOrientation[offPos + offY + offSignPos];
		}
		else if (camOrient[2][1][1] == -1.0) {
			dirs[4] << PatientPositionOrientation[offPos + offY + offSignNeg];
		}
		else {
			dirs[4] << PatientPositionOrientation[offPos + offY + offSignZero];
		}
		// Back Z:
		if (camOrient[2][1][2] == 1.0) {
			dirs[4] << PatientPositionOrientation[offPos + offZ + offSignPos];
		}
		else if (camOrient[2][1][2] == -1.0) {
			dirs[4] << PatientPositionOrientation[offPos + offZ + offSignNeg];
		}
		else {
			dirs[4] << PatientPositionOrientation[offPos + offZ + offSignZero];
		}

		// Front X:
		if (camOrient[2][0][0] == 1.0) {
			dirs[5] << PatientPositionOrientation[offPos + offX + offSignPos];
		}
		else if (camOrient[2][0][0] == -1.0) {
			dirs[5] << PatientPositionOrientation[offPos + offX + offSignNeg];
		}
		else {
			dirs[5] << PatientPositionOrientation[offPos + offX + offSignZero];
		}
		// Front Y:
		if (camOrient[2][0][1] == 1.0) {
			dirs[5] << PatientPositionOrientation[offPos + offY + offSignPos];
		}
		else if (camOrient[2][0][1] == -1.0) {
			dirs[5] << PatientPositionOrientation[offPos + offY + offSignNeg];
		}
		else {
			dirs[5] << PatientPositionOrientation[offPos + offY + offSignZero];
		}
		// Front Z:
		if (camOrient[2][0][2] == 1.0) {
			dirs[5] << PatientPositionOrientation[offPos + offZ + offSignPos];
		}
		else if (camOrient[2][0][2] == -1.0) {
			dirs[5] << PatientPositionOrientation[offPos + offZ + offSignNeg];
		}
		else {
			dirs[5] << PatientPositionOrientation[offPos + offZ + offSignZero];
		}
	}

}

//region "Interfaz especifica"



void GNC::GCS::Widgets::WAnotador::SetTextColor(const GNC::GCS::GLHelper::TColor& color)
{
	m_color = color;
}
//endregion


//region "Estado interno"


void GNC::GCS::Widgets::WAnotador::OffscreenRender(GNC::GCS::Contexto3D* c)
{
	GNC::GCS::Vector RectViewport[2] = { GNC::GCS::Vector(0, 0), GNC::GCS::Vector(c->ancho, c->alto) };
	GNC::GCS::Vector TamViewPort = ((RectViewport[1] - RectViewport[0]).ValorAbsoluto() * PROPVIEWPORT).Redondear();

	GNC::GCS::Vector TamCajaMaxima = (RectViewport[1] - RectViewport[0]).ValorAbsoluto();

	const std::string        TextoAnotacion[WAnotador::TP_NumPosiciones] = {"","","",""};/////////////{ m_pAnnotator->GetTopLeftAnnotation(c), m_pAnnotator->GetTopRightAnnotation(c), m_pAnnotator->GetBottomLeftAnnotation(c), m_pAnnotator->GetBottomRightAnnotation(c) };
	GNC::GCS::Vector         TamTexto      [WAnotador::TP_NumPosiciones];
	GNC::GCS::Widgets::HelperTexto::TAlineamiento Alineacion[4] = { GNC::GCS::Widgets::HelperTexto::TA_Izquierda, GNC::GCS::Widgets::HelperTexto::TA_Derecha, GNC::GCS::Widgets::HelperTexto::TA_Izquierda, GNC::GCS::Widgets::HelperTexto::TA_Derecha };

	cairo_font_options_t* options;
	options = cairo_font_options_create ();

	cairo_select_font_face (c->cr, WIDGETS_TEXT_FONT_FAMILY, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(c->cr, std::max(m_FontSizeProp * c->RefRelacionMundoPantallaOffscreen().x, (double)8.0f));
	cairo_font_options_set_antialias (options, CAIRO_ANTIALIAS_NONE);
	cairo_set_font_options(c->cr, options);


	for (int i = 0; i < WAnotador::TP_NumPosiciones; ++i) {
		TamTexto[i].Asignar(0.0f, 0.0f);
		if (TextoAnotacion[i].size() > 0) {
			TamTexto[i] = GNC::GCS::Widgets::HelperTexto::calcularBoundingBox(c, TextoAnotacion[i], ANNOTATION_TEXT_WITH_SHADOW, TamCajaMaxima.x);
			TamCajaMaxima.AsignarMaximos(TamTexto[i]);
		}
	}

	GNC::GCS::Vector pos;


	// Anotacion TP_TopLeft
	pos.Asignar(0.0f, 0.0f).Redondear();
	cairo_save(c->cr);
	cairo_translate(c->cr, pos.x, pos.y );

	TamTexto[TP_TopLeft] = GNC::GCS::Widgets::HelperTexto::dibujarTexto(c, TextoAnotacion[TP_TopLeft], ANNOTATION_TEXT_WITH_SHADOW, TamCajaMaxima.x, Alineacion[TP_TopLeft]);
	cairo_restore(c->cr);

	// Anotacion TP_TopRight
	pos.Asignar(RectViewport[1].x - TamTexto[TP_TopRight].x - 5.0f, 0.0f).Redondear();
	cairo_save(c->cr);
	cairo_translate(c->cr, pos.x, pos.y );

	TamTexto[TP_TopRight] = GNC::GCS::Widgets::HelperTexto::dibujarTexto(c, TextoAnotacion[TP_TopRight], ANNOTATION_TEXT_WITH_SHADOW, TamCajaMaxima.x, Alineacion[TP_TopRight]);
	cairo_restore(c->cr);

	// Anotacion TP_BottomLeft
	pos.Asignar(0.0f, RectViewport[1].y - TamTexto[TP_BottomLeft].y).Redondear();
	cairo_save(c->cr);
	cairo_translate(c->cr, pos.x, pos.y );

	TamTexto[TP_BottomLeft] = GNC::GCS::Widgets::HelperTexto::dibujarTexto(c, TextoAnotacion[TP_BottomLeft], ANNOTATION_TEXT_WITH_SHADOW, TamCajaMaxima.x, Alineacion[TP_BottomLeft]);
	cairo_restore(c->cr);

	// Anotacion TP_BottomRight
	pos.Asignar(RectViewport[1].x - TamTexto[TP_BottomRight].x - 5.0f, RectViewport[1].y - TamTexto[TP_BottomRight].y).Redondear();
	cairo_save(c->cr);
	cairo_translate(c->cr, pos.x, pos.y );

	TamTexto[TP_BottomRight] = GNC::GCS::Widgets::HelperTexto::dibujarTexto(c, TextoAnotacion[TP_BottomRight], ANNOTATION_TEXT_WITH_SHADOW, TamCajaMaxima.x, Alineacion[TP_BottomRight]);
	cairo_restore(c->cr);


	cairo_font_options_destroy(options);
}

