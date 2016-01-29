/*
*  
*  $Id: panelconfiguracionubicaciones.cpp $
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
#include <vector>
#include <sstream>
#include <limits>

#include <wx/msgdlg.h>
#include <wx/msgout.h>
#include <wx/filename.h>
#include <main/controllers/configurationcontroller.h>
#include <wx/valtext.h>
#include <wx/ginkgostyle/ginkgostyle.h>
#include <wx/mstream.h>
#include <wx/dirdlg.h>

#include <wx/file.h>
#include <wx/dir.h>
#include <wx/log.h>
#include <wx/xml/xml.h>
#include <wx/log.h>

#include <wx/tokenzr.h>

#include "panelconfiguracionubicaciones.h"
#include <api/ubication.h>

#include <sstream>
#include <api/imodelointegracion.h>
#include <main/controllers/controladorextensiones.h>
#include <main/controllers/controladorlog.h>
#include <main/entorno.h>
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

#define UB_COL_TITLE_IDX          0
#define UB_COL_TITLE_WIDTH        100
#define UB_COL_PATH_IDX           1
#define UB_COL_PATH_WIDTH         100
#define UB_COL_DESCRIPTION_IDX    2
#define UB_COL_DESCRIPTION_WIDTH  100
#define UB_COL_MONITORIZE_IDX     3
#define UB_COL_MONITORIZE_WIDTH   100
#define UB_COL_CLEAN_BEFORE_IDX   4
#define UB_COL_CLEAN_BEFORE_WIDTH 100
#define UB_COL_CLEAN_AFTER_IDX    5
#define UB_COL_CLEAN_AFTER_WIDTH  100

namespace GNC {
	namespace GUI {

		class DialogoAddLocationDialog: public DialogoAddLocationBase
		{

		public:

			typedef GNC::GCS::IEntorno::MapaUbicaciones UbicationsMapType;

			DialogoAddLocationDialog(wxWindow* pParent, const UbicationsMapType* const pUbicationsMap, const GIL::Ubicacion& u, bool isnew = false) : DialogoAddLocationBase(pParent), m_pUbicationsMap(pUbicationsMap)
			{
				m_Ubicacion = u;
				m_IsNew = isnew;
				m_Ok = false;

				m_pTETitle->SetValue( wxString::FromUTF8( m_Ubicacion.Titulo.c_str() ) );
				m_pTELocation->SetValue( wxString::FromUTF8( m_Ubicacion.Ruta.c_str() ) );
				m_pTEDesc->SetValue( wxString::FromUTF8( m_Ubicacion.Descripcion.c_str() ) );
				m_pMonitorize->SetValue(m_Ubicacion.Monitorize);
				m_pCleanBefore->SetValue(m_Ubicacion.CleanBefore);
				m_pCleanAfter->SetValue(m_Ubicacion.CleanAfter);

			}

			~DialogoAddLocationDialog()
			{

			}

			virtual void OnOpenDirClick( wxCommandEvent& /*event*/ ) {
				wxDirDialog dlg(this, _("Choose a directory"), wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
				if ( dlg.ShowModal() ) {
					wxString path = dlg.GetPath();
					if (path.size() > 0) {
						m_pTELocation->SetValue(path);
					}
				}
			}

			virtual void OnAceptarClick( wxCommandEvent& /*event*/ ) {
				if (Validar()) {
					m_Ok = true;
					EndModal(wxID_OK);
				}
			}

			bool Validar() {

				if(m_pTETitle->GetValue() != wxEmptyString && m_pTELocation->GetValue() != wxEmptyString) {

					std::string title (m_pTETitle->GetValue().ToUTF8());


					for (UbicationsMapType::const_iterator it = m_pUbicationsMap->begin(); it != m_pUbicationsMap->end(); ++it)
					{
						const std::string& ti = (*it).first;
						if ((ti == title && m_IsNew) || (!m_IsNew && ti == title && title != m_Ubicacion.Titulo) ){
							wxMessageBox(_("Location titles can't be repeated"),_("Error"),wxOK | wxICON_ERROR, this);
							return false;
						}
					}

					if (title.size() > 8)
					{
						wxMessageBox(_("Maximum length of title is 8 characters"),_("Error"),wxOK | wxICON_ERROR, this);
						return false;
					}

					if (!wxDir::Exists(m_pTELocation->GetValue()))
					{
						wxMessageBox(_("The selected directory can't be opened"),_("Error"),wxOK | wxICON_ERROR, this);
						return false;
					}

					if (m_IsNew) {
						m_Ubicacion = GIL::Ubicacion(
							title,
							std::string(m_pTELocation->GetValue().ToUTF8()),
							std::string(m_pTEDesc->GetValue().ToUTF8()),
							m_pMonitorize->GetValue(),
							m_pCleanBefore->GetValue(),
							m_pCleanAfter->GetValue());
					}
					else {
						m_Ubicacion.Update(
							std::string(m_pTELocation->GetValue().ToUTF8()),
							std::string(m_pTEDesc->GetValue().ToUTF8()),
							m_pMonitorize->GetValue(),
							m_pCleanBefore->GetValue(),
							m_pCleanAfter->GetValue());
						m_Ubicacion.Titulo = title;
					}

					return true;
				}
				else{
					wxMessageBox(_("You must fill both title and location fields to continue") ,_("Error"),wxOK | wxICON_ERROR, this);
					return false;
				}
			}

			bool                           m_Ok;
			bool                           m_IsNew;
			GIL::Ubicacion                 m_Ubicacion;
			const UbicationsMapType* const m_pUbicationsMap;
		};


		////////////////-------------------------------------------------------------------------------------/////////////////////////////

		PanelConfiguracionUbicaciones::PanelConfiguracionUbicaciones(wxWindow* pParent, IDialogoConfiguracion* pDialogo): PanelConfiguracionUbicacionesBase(pParent), IPasoConfiguracion(pDialogo)
		{

			m_pModel = new wxDataViewListStore();
			m_pListUbicaciones->AssociateModel(m_pModel.get());

			m_pListUbicaciones->AppendColumn( new wxDataViewColumn( _("Title"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ), UB_COL_TITLE_IDX, UB_COL_TITLE_WIDTH, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
			m_pListUbicaciones->AppendColumn( new wxDataViewColumn( _("Path"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ), UB_COL_PATH_IDX, UB_COL_PATH_WIDTH, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
			m_pListUbicaciones->AppendColumn( new wxDataViewColumn( _("Description"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ), UB_COL_DESCRIPTION_IDX, UB_COL_DESCRIPTION_WIDTH, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
			m_pListUbicaciones->AppendColumn( new wxDataViewColumn( _("Monitorize"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ), UB_COL_MONITORIZE_IDX, UB_COL_MONITORIZE_WIDTH, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));			
			m_pListUbicaciones->AppendColumn( new wxDataViewColumn( _("Clean before"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ), UB_COL_CLEAN_BEFORE_IDX, UB_COL_CLEAN_BEFORE_WIDTH, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
			m_pListUbicaciones->AppendColumn( new wxDataViewColumn( _("Clean after"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ), UB_COL_CLEAN_AFTER_IDX, UB_COL_CLEAN_AFTER_WIDTH, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
			
			m_pListUbicaciones->Connect(wxEVT_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler(PanelConfiguracionUbicaciones::OnUbicationsSelectionChanged), NULL, this);
			m_pListUbicaciones->Connect(wxEVT_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler(PanelConfiguracionUbicaciones::OnUbicationsDClick), NULL, this);

			Recargar();
			m_pUbicaciones->Layout();
			Layout();
		}

		PanelConfiguracionUbicaciones::~PanelConfiguracionUbicaciones()
		{

		}

		void PanelConfiguracionUbicaciones::Recargar()
		{
			GNC::GCS::ConfigurationController::TListGroups locations;
			GNC::GCS::ConfigurationController::Instance()->readGroupGeneral("/GinkgoCore/Locations", locations);

			wxVector<wxVariant> data;
			m_pListUbicaciones->DeleteAllItems();
			for (UbicationsMapType::iterator it = m_Ubicaciones.begin(); it != m_Ubicaciones.end(); ++it) {
				delete (*it).second;
			}
			m_Ubicaciones.clear();

			for (GNC::GCS::ConfigurationController::TListGroups::iterator it = locations.begin(); it != locations.end(); ++it) {
				GNC::GCS::ConfigurationController::TListGroups::value_type& item = (*it);
				data.clear();

				std::string title, path, descr;
				bool monitorize, cleanbefore, cleanafter;

				item.readStringValue("Title", title);
				item.readStringValue("Path", path);
				item.readStringValue("Description", descr);
				item.readBoolValue("Monitorize", monitorize, false);
				item.readBoolValue("CleanBefore", cleanbefore, false);
				item.readBoolValue("CleanAfter", cleanafter, false);

				m_Ubicaciones[title] = new GIL::Ubicacion(title, path, descr, monitorize, cleanbefore, cleanafter);
				data.push_back(wxString::FromUTF8(title.c_str()));
				data.push_back(wxString::FromUTF8(path.c_str()));
				data.push_back(wxString::FromUTF8(descr.c_str()));
				if (monitorize) {
					data.push_back(_("Yes"));
				}
				else {
					data.push_back(_("No"));
				}
				if (cleanbefore) {
					data.push_back(_("Yes"));
				}
				else {
					data.push_back(_("No"));
				}
				if (cleanafter) {
					data.push_back(_("Yes"));
				}
				else {
					data.push_back(_("No"));
				}
				m_pModel->AppendItem(data);
			}
		}

		//region "Metodos de IPasoConfiguracion"
		wxWindow* PanelConfiguracionUbicaciones::GetPanel()
		{
			return this;
		}

		std::string PanelConfiguracionUbicaciones::GetTitle()
		{
			return _Std("Locations");
		}

		std::string PanelConfiguracionUbicaciones::GetCabecera()
		{
			return _Std("Location Settings");
		}

		bool PanelConfiguracionUbicaciones::Validar()
		{
			bool ok = true;

			return ok;
		}

		bool PanelConfiguracionUbicaciones::Guardar()
		{
			GNC::GCS::IEntorno::MapaUbicaciones& mapa = GNC::Entorno::Instance()->GetUbicaciones();
			for (GNC::GCS::IEntorno::MapaUbicaciones::iterator it = mapa.begin(); it != mapa.end(); ++it) {
				delete (*it).second;
			}
			mapa.clear();

			GNC::GCS::ConfigurationController::Instance()->deleteEntryGeneral("/GinkgoCore/Locations");
			GNC::GCS::ConfigurationController::TListGroups locations;

			for (GNC::GCS::IEntorno::MapaUbicaciones::iterator it = m_Ubicaciones.begin(); it != m_Ubicaciones.end(); ++it) {
				GIL::Ubicacion* u = (*it).second;
				mapa[u->Titulo] = u;
				GNC::GCS::ConfigurationController::TMapValues location;
				location["Title"] = u->Titulo;
				location["Path"] = u->Ruta;
				location["Description"] = u->Descripcion;
				location.insertBool("Monitorize", u->Monitorize);
				location.insertBool("CleanBefore", u->CleanBefore);
				location.insertBool("CleanAfter", u->CleanAfter);
				locations.push_back(location);
			}

			GNC::GCS::ConfigurationController::Instance()->writeGroupGeneral("/GinkgoCore/Locations", locations, "Location");

			return true;
		}
		//endregion

		void PanelConfiguracionUbicaciones::OnUbicationsDClick( wxDataViewEvent&)
		{
			wxCommandEvent evt;
			OnEditarClick(evt);
		}

		void PanelConfiguracionUbicaciones::OnUbicationsSelectionChanged( wxDataViewEvent& event)
		{			
			if (event.GetSelection() >= 0) {
				m_pBotonEdit->Enable(true);
				m_pBotonDelete->Enable(true);
			}
			else {
				m_pBotonEdit->Enable(false);
				m_pBotonDelete->Enable(false);
			}

		}

		void PanelConfiguracionUbicaciones::OnNuevoClick(wxCommandEvent &){

			DialogoAddLocationDialog dlg(this, &m_Ubicaciones, GIL::Ubicacion(), true);
			dlg.ShowModal();
			if (dlg.m_Ok) {
				GIL::Ubicacion& tuple = dlg.m_Ubicacion;
				m_Ubicaciones[tuple.Titulo] = new GIL::Ubicacion(tuple);

				wxVector<wxVariant> data;
				data.push_back(wxString::FromUTF8(tuple.Titulo.c_str()));
				data.push_back(wxString::FromUTF8(tuple.Ruta.c_str()));
				data.push_back(wxString::FromUTF8(tuple.Descripcion.c_str()));
				if (tuple.Monitorize) {
					data.push_back(_("Yes"));
				}
				else {
					data.push_back(_("No"));
				}
				if (tuple.CleanBefore) {
					data.push_back(_("Yes"));
				}
				else {
					data.push_back(_("No"));
				}
				if (tuple.CleanBefore) {
					data.push_back(_("Yes"));
				}
				else {
					data.push_back(_("No"));
				}
				m_pModel->AppendItem(data);
				OnPropiedadCambiada();
			}
		}

		void PanelConfiguracionUbicaciones::OnEditarClick(wxCommandEvent &){

			wxDataViewItem selection = m_pListUbicaciones->GetSelection();
			if (selection.IsOk()) {
				wxVariant val;
				m_pModel->GetValue(val, selection, UB_COL_TITLE_IDX);
				std::string idSeleccionado(val.GetString().ToUTF8());

				GNC::GCS::IEntorno::MapaUbicaciones::iterator it = m_Ubicaciones.find(idSeleccionado);
				if (it != m_Ubicaciones.end()) {
					GIL::Ubicacion *pSel = (*it).second;
					if (pSel != NULL) {
						DialogoAddLocationDialog dlg(this, &m_Ubicaciones, (*pSel), false);
						dlg.ShowModal();
						if (dlg.m_Ok) {
							GIL::Ubicacion& tuple = dlg.m_Ubicacion;

							val = wxString::FromUTF8(tuple.Titulo.c_str());
							m_pModel->SetValue(val, selection, UB_COL_TITLE_IDX);

							val = wxString::FromUTF8(tuple.Ruta.c_str());
							m_pModel->SetValue(val, selection, UB_COL_PATH_IDX);

							val = wxString::FromUTF8(tuple.Descripcion.c_str());
							m_pModel->SetValue(val, selection, UB_COL_DESCRIPTION_IDX);

							m_pModel->SetValue((tuple.Monitorize ? _("Yes") : _("No")), selection, UB_COL_MONITORIZE_IDX);
							m_pModel->SetValue((tuple.CleanBefore ? _("Yes") : _("No")), selection, UB_COL_CLEAN_BEFORE_IDX);
							m_pModel->SetValue((tuple.CleanAfter ? _("Yes") : _("No")), selection, UB_COL_CLEAN_AFTER_IDX);

							(*m_Ubicaciones[tuple.Titulo]) = tuple;
							m_pModel->ItemChanged(selection);

							OnPropiedadCambiada();
						}
					}
				}					
			}
		}

		void PanelConfiguracionUbicaciones::OnEliminarClick(wxCommandEvent &){

			wxDataViewItem selection = m_pListUbicaciones->GetSelection();

			if(selection.IsOk()){
				wxVariant val;
				m_pModel->GetValue(val, selection, UB_COL_TITLE_IDX);
				std::string idSeleccionado(val.GetString().ToUTF8());

				int answer = wxMessageBox(_("Confirm the removal of the node") + wxT(" ") + wxString::FromUTF8(idSeleccionado.c_str()), _("Data modified"), wxYES_NO | wxCANCEL, this);
				if(answer == wxYES) {
					GNC::GCS::IEntorno::MapaUbicaciones::iterator it = m_Ubicaciones.find(idSeleccionado);
					if (it != m_Ubicaciones.end()) {
						GIL::Ubicacion *pSel = (*it).second;
						if (pSel != NULL) {

							bool notFound = true;
							for (unsigned int i = 0; notFound && i < m_pModel->GetCount(); ++i) {
								wxDataViewItem item = m_pModel->GetItem(i);

								wxVariant val2;
								m_pModel->GetValueByRow(val2, i, UB_COL_TITLE_IDX);
								if (val == val2) {
									notFound = false;
									m_pModel->DeleteItem(i);
								}
							}
							delete pSel;
							m_Ubicaciones.erase(it);
						}
						m_pListUbicaciones->Refresh();
						wxDataViewEvent evt;
						OnUbicationsSelectionChanged(evt);	
						OnPropiedadCambiada();
					}
				}
			}

		}


		void PanelConfiguracionUbicaciones::OnHelpClick(wxMouseEvent &)
		{
		}

		void PanelConfiguracionUbicaciones::OnSize(wxSizeEvent & event){
			event.Skip(true);
		}
	}
}
