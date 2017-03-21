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

#include <wx/file.h>
#include <wx/dir.h>
#include <wx/log.h>
#include <wx/busyinfo.h>
#include <wx/xml/xml.h>
#include <wx/sstream.h>
#include <wx/log.h>
#include <wx/menu.h>

#include <wx/tokenzr.h>

#include "panelconfiguracionpermisos.h"

#include <sstream>

#include <main/controllers/controladorextensiones.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/controladorpermisos.h>
#include <main/entorno.h>
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

#define SEPARADOR "|"

#define COL_NAME			  0
#define COL_VALUE			  1
#define COL_DEFAULT_VALUE     2

#define PERM_COL_NAME_IDX     0
#define PERM_COL_NAME_WIDTH   340
#define PERM_COL_ACTIVE_IDX   1
#define PERM_COL_ACTIVE_WIDTH 80
#define PERM_COL_XML	     2

#define COLOUR_ACTIVE wxColour(10,174,10)
#define COLOUR_INACTIVE wxColour(174,10,10)
#define COLOUR_SET_BY_USER wxColour(229,137,39)

namespace GNC
{
namespace GUI
{

class ProfileDialog: public ProfileDialogBase
{
        typedef struct TipoPermiso {
                std::string clave;
                bool valor;

                TipoPermiso() :
                        clave(),
                        valor(false)
                {
                }

                TipoPermiso(const std::string& _clave, bool _valor) :
                        clave(_clave),
                        valor(_valor)
                {
                }
        } TipoPermiso;
        typedef std::map <std::string, GNC::GCS::Permisos::EstadoPermiso> TipoMapaPermisos;

public:
        ProfileDialog(wxWindow* pParent, std::list<std::string>& listaNombres, bool isnew = false) :
                ProfileDialogBase(pParent),
                m_Ok(false),
                m_listaNombres(listaNombres)
        {

                if (isnew) {

                        long     idNombreAutomatico = 0;
                        std::string profile_template = _Std("Profile");

                        for (std::list<std::string>::iterator it = listaNombres.begin(); it != listaNombres.end(); ++it) {
                                std::istringstream is (*it);
                                std::string t_name;
                                long id = -1;
                                is >> t_name >> id;

                                // coverity complains that the scalar is tained, but it is checked here,
                                // sanitized. and it is used only as an id.
                                // coverity[TAINTED_SCALAR]
                                if (!is.fail() && t_name == profile_template && id > 0) {
                                        idNombreAutomatico = std::max<long>(id, idNombreAutomatico);
                                }

                        }
                        {
                                std::ostringstream os;
                                os << profile_template << " " << (idNombreAutomatico + 1);
                                m_pName->SetValue( wxString::FromUTF8( os.str().c_str() ) );
                        }
                }

                const GNC::ControladorPermisos::TipoMapaDefiniciones& definiciones = GNC::ControladorPermisos::Instance()->GetMapaDefiniciones();

                //cargamos los permisos "publicos" definidos
                std::string ultimaCategoria = "";
                wxPGProperty* cat = NULL;
                m_pGridPermisos->SetColumnCount(3);

                m_pGridPermisos->SetColumnProportion(COL_NAME, 3);
                m_pGridPermisos->SetColumnProportion(COL_VALUE, 4);
                m_pGridPermisos->SetColumnProportion(COL_DEFAULT_VALUE, 2);


                for(GNC::ControladorPermisos::TipoMapaDefiniciones::const_iterator it = definiciones.begin(); it != definiciones.end(); ++it) {
                        if(!(*it).second.privado) {
                                if ((*it).second.espacioNombres != ultimaCategoria) {
                                        ultimaCategoria = (*it).second.espacioNombres;
                                        cat = m_pGridPermisos->Append(new wxPropertyCategory(wxString::FromUTF8((*it).second.descripcionNamespace.c_str()),wxString::FromUTF8(ultimaCategoria.c_str())));
                                }
                                std::string clave = (*it).second.espacioNombres + SEPARADOR + (*it).second.id;
                                wxLongStringProperty* propValue = new wxLongStringProperty(wxString::FromUTF8((*it).second.descripcion.c_str()), wxString::FromUTF8(clave.c_str()), wxString::FromUTF8((*it).second.valorPorDefecto.c_str()));
                                wxPGProperty* prop = cat->AppendChild(propValue);
                                m_pGridPermisos->SetPropertyCell(prop, COL_DEFAULT_VALUE, _("Default value"));
                                SetPropertyActive(prop, (*it).second.activoPorDefecto);
                        }
                }

                m_pGridPermisos->GetRoot()->SetFlagRecursively(wxPG_PROP_NO_ESCAPE, true);
                m_pGridPermisos->Connect(wxEVT_PG_DOUBLE_CLICK, wxPropertyGridEventHandler( ProfileDialog::OnPgDoubleClick ), NULL, this );
                m_pGridPermisos->Connect(wxEVT_PG_RIGHT_CLICK, wxPropertyGridEventHandler( ProfileDialog::OnPgMenu ), NULL, this );
        }

        ~ProfileDialog()
        {

        }

        void GetParametros(std::string &nombre, std::string &xml)
        {
                nombre = std::string(m_pName->GetValue().ToUTF8());

                wxXmlDocument doc;
                wxXmlNode* root = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("permisos"));
                doc.SetRoot(root);

                //transformamos lo que esta viendo el usuario a un xml...
                for(wxPropertyGridIterator it = m_pGridPermisos->GetIterator(); !it.AtEnd(); it.Next()) {
                        wxPGProperty* pProperty = it.GetProperty();

                        wxString clave = pProperty->GetName();

                        wxStringTokenizer tkz(clave, wxString::FromUTF8(SEPARADOR));
                        std::string espacioNombres ="", id="";
                        if ( tkz.HasMoreTokens() ) {
                                espacioNombres = tkz.GetNextToken().ToUTF8();
                                if (tkz.HasMoreTokens()) {
                                        id = tkz.GetNextToken().ToUTF8();
                                }
                        }

                        if (id != "" && espacioNombres != "") {

                                std::string stdclave( espacioNombres + "." + id );

                                std::string accion;

                                if (IsPropertyActive(pProperty)) {
                                        accion = "activar";
                                } else {
                                        accion = "desactivar";
                                }

                                std::string val(pProperty->GetValueAsString().ToUTF8());

                                std::string activopordefecto = "true";

                                GNC::ControladorPermisos::TipoMapaDefiniciones mapa = GNC::ControladorPermisos::Instance()->GetMapaDefiniciones();
                                GNC::ControladorPermisos::TipoMapaDefiniciones::iterator it = mapa.find(stdclave);
                                GNC::ControladorPermisos::TipoDefinicionPermiso def;

                                if (it != mapa.end()) {
                                        def = (*it).second;
                                }

                                if (!def.activoPorDefecto) {
                                        activopordefecto = "false";
                                }

                                wxXmlNode* permiso = new wxXmlNode(root, wxXML_ELEMENT_NODE, wxT("permiso"));
                                permiso->AddAttribute(wxT("id"), wxString::FromUTF8(id.c_str()));
                                permiso->AddAttribute(wxT("namespace"), wxString::FromUTF8(espacioNombres.c_str()));
                                permiso->AddAttribute(wxT("accion"), wxString::FromUTF8(accion.c_str()));
                                permiso->AddAttribute(wxT("activopordefecto"), wxString::FromUTF8(activopordefecto.c_str()));


                                wxXmlNode* data = new wxXmlNode(permiso, wxXML_ELEMENT_NODE, wxT("default_value"));
                                data = new wxXmlNode(data, wxXML_CDATA_SECTION_NODE, wxT("default_value"));
                                data->SetContent(wxString::FromUTF8(def.valorPorDefecto.c_str()));
                                data = new wxXmlNode(permiso, wxXML_ELEMENT_NODE, wxT("value"));
                                data = new wxXmlNode(data, wxXML_CDATA_SECTION_NODE, wxT("value"));
                                data->SetContent(wxString::FromUTF8(val.c_str()));
                        }
                }
                wxMemoryOutputStream out;
                doc.SetFileEncoding(wxT("UTF-8"));
                doc.Save(out,0);
                char* vectorWidgets = new char[out.GetLength()];
                out.CopyTo(vectorWidgets,out.GetLength());
                xml = std::string(vectorWidgets, out.GetLength());
                delete[] vectorWidgets;
        }

        virtual void OnPgDoubleClick(wxPropertyGridEvent &event)
        {
                if (event.GetProperty() == NULL || event.GetProperty()->IsCategory())
                        return;
                wxPGProperty* prop = event.GetProperty();
                SetPropertyActive(prop, !IsPropertyActive(prop));
        }

        bool IsPropertyActive(wxPGProperty* pProp)
        {
                wxColour col = pProp->GetCell(COL_NAME).GetFgCol();
                return col == COLOUR_ACTIVE;
        }

        void SetPropertyActive(wxPGProperty* pProp, bool value)
        {
                if (value) {
                        m_pGridPermisos->SetPropertyCell(pProp, COL_NAME, pProp->GetLabel(), GinkgoResourcesManager::MenuIcons::GetIcoTick(), COLOUR_ACTIVE);
                } else {
                        m_pGridPermisos->SetPropertyCell(pProp, COL_NAME, pProp->GetLabel(), GinkgoResourcesManager::MenuIcons::GetIcoCross(), COLOUR_INACTIVE);
                }
                m_pGridPermisos->RefreshProperty(pProp);
        }

        virtual void OnPgMenu(wxPropertyGridEvent &event)
        {
                if (event.GetProperty() == NULL || event.GetProperty()->IsCategory())
                        return;
                wxPGProperty* prop = event.GetProperty();
                bool isActive = IsPropertyActive(prop);
                wxMenu menu;
                wxMenuItem* pActive = menu.Append(0, _("Active"), _("Set property active"), wxITEM_CHECK);
                wxMenuItem* pInactive = menu.Append(1, _("Inactive"), _("Set property inactive"), wxITEM_CHECK);
                wxMenuItem* pDefault = menu.Append(2, _("Default value"), _("Set default value"), wxITEM_NORMAL);
                if (isActive) {
                        pActive->Check(true);
                        pInactive->Check(false);
                } else {
                        pActive->Check(false);
                        pInactive->Check(true);
                }
                menu.Connect(pActive->GetId(),wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ProfileDialog::OnActiveSelectedProperty ), NULL, this );
                menu.Connect(pInactive->GetId(),wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ProfileDialog::OnDesactiveSelectedProperty ), NULL, this );
                menu.Connect(pDefault->GetId(),wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ProfileDialog::OnSetDefaultSelected ), NULL, this );
                m_pGridPermisos->PopupMenu(&menu);
        }

        void OnActiveSelectedProperty(wxCommandEvent&)
        {
                wxPGProperty* prop = m_pGridPermisos->GetSelectedProperty();
                if (prop != NULL) {
                        SetPropertyActive(prop, true);
                }
        }

        void OnDesactiveSelectedProperty(wxCommandEvent&)
        {
                wxPGProperty* prop = m_pGridPermisos->GetSelectedProperty();
                if (prop != NULL) {
                        SetPropertyActive(prop, false);
                }
        }

        void OnSetDefaultSelected(wxCommandEvent&)
        {
                wxPGProperty* prop = m_pGridPermisos->GetSelectedProperty();
                if (prop != NULL) {
                        wxString clave = prop->GetName();

                        wxStringTokenizer tkz(clave, wxString::FromUTF8(SEPARADOR));
                        std::string espacioNombres ="", id="";
                        if ( tkz.HasMoreTokens() ) {
                                espacioNombres = tkz.GetNextToken().ToUTF8();
                                if (tkz.HasMoreTokens()) {
                                        id = tkz.GetNextToken().ToUTF8();
                                }
                        }
                        if (id != "" && espacioNombres != "") {
                                std::string stdclave( espacioNombres + "." + id );
                                std::string val(prop->GetValueAsString().ToUTF8());

                                GNC::ControladorPermisos::TipoMapaDefiniciones mapa = GNC::ControladorPermisos::Instance()->GetMapaDefiniciones();
                                GNC::ControladorPermisos::TipoMapaDefiniciones::iterator it = mapa.find(stdclave);
                                GNC::ControladorPermisos::TipoDefinicionPermiso def;

                                if (it != mapa.end()) {
                                        def = (*it).second;
                                }

                                prop->SetValue(wxString::FromUTF8(def.valorPorDefecto.c_str()));
                                m_pGridPermisos->SetPropertyCell(prop, COL_DEFAULT_VALUE, _("Default value"), wxNullBitmap, wxColour(0,0,0));
                                SetPropertyActive(prop, def.activoPorDefecto);
                        }
                }
        }

        void SetParametros(const std::string &nombre, const std::string &xml)
        {
                m_nombreEditar = nombre;
                m_pName->SetValue(wxString::FromUTF8(nombre.c_str()));

                TipoMapaPermisos mapa = GetPermisosFromXml(xml);

                for(wxPropertyGridIterator it = m_pGridPermisos->GetIterator(); !it.AtEnd(); it.Next()) {
                        wxPGProperty* pProperty = it.GetProperty();
                        if(pProperty != NULL) {

                                TipoMapaPermisos::iterator itp = mapa.find(std::string(pProperty->GetName().ToUTF8()));
                                if (itp != mapa.end()) {
                                        GNC::GCS::Permisos::EstadoPermiso& perm = (*itp).second;
                                        SetPropertyActive(pProperty, perm);

                                        pProperty->SetValue( wxString::FromUTF8(perm.GetRawValue().c_str()) );
                                        if (perm.TieneValorPorDefecto()) {
                                                m_pGridPermisos->SetPropertyCell(pProperty, COL_DEFAULT_VALUE, _("Default value"), wxNullBitmap, wxColour(0,0,0));
                                        } else {
                                                m_pGridPermisos->SetPropertyCell(pProperty, COL_DEFAULT_VALUE, _("Set by user"), wxNullBitmap, COLOUR_SET_BY_USER);
                                        }
                                }

                        }
                }
                //free memory
        }

        TipoMapaPermisos GetPermisosFromXml(const std::string& stdCadena)
        {
                TipoMapaPermisos permisos;
                try {
                        GNC::ControladorPermisos::TipoMapaPermisosNamespace mapaPermisosNamespace = GNC::ControladorPermisos::Instance()->GetPermisosFromXML(stdCadena);
                        for( GNC::ControladorPermisos::TipoIteradorMapaPermisosNamespace itNames = mapaPermisosNamespace.begin(); itNames != mapaPermisosNamespace.end(); ++itNames ) {
                                for( GNC::ControladorPermisos::TipoIteradorMapaPermisos itPermisos = (*itNames).second->begin(); itPermisos != (*itNames).second->end(); ++itPermisos) {
                                        permisos[itNames->first + SEPARADOR + itPermisos->first] = itPermisos->second;
                                }
                                delete (*itNames).second;
                        }
                } catch (GNC::GCS::ControladorPermisosException& ce) {
                        LOG_WARN("GestionPermisos", "Existe en la configuración un xml de permisos mal formado " << ce.GetCause());
                }
                return permisos;
        }

        virtual void OnCancelClick(wxCommandEvent &)
        {
                EndModal(wxID_CANCEL);
        }

        virtual void OnOkClick(wxCommandEvent &)
        {
                if (Validar()) {
                        m_Ok=true;
                        EndModal(wxID_OK);
                }
        }

        bool Validar()
        {
                if(m_pName->GetValue() != wxEmptyString) {
                        //no puede duplicar un nombre que esté en la lista y que no sea el mismo
                        std::string nombre (m_pName->GetValue().ToUTF8());
                        if (nombre != m_nombreEditar) {
                                for (std::list<std::string>::iterator it = m_listaNombres.begin(); it != m_listaNombres.end(); ++it) {
                                        if ((*it) == nombre) {
                                                wxMessageBox(_("Profile name can't be repeated"),_("Info"),wxOK | wxICON_INFORMATION, this);
                                                return false;
                                        }
                                }
                        }
                        return true;
                } else {
                        wxMessageBox(_("You must fill in name field to continue"),_("Info"),wxOK | wxICON_INFORMATION, this);
                }
                return false;
        }

        bool m_Ok;
        std::list<std::string> m_listaNombres;
        std::string m_nombreEditar;
};


////////////////-------------------------------------------------------------------------------------/////////////////////////////

PanelConfiguracionPermisos::PanelConfiguracionPermisos(wxWindow* pParent,IDialogoConfiguracion* pDialogo): PanelConfiguracionPermisosBase(pParent), IPasoConfiguracion(pDialogo)
{
        m_pModel = new wxDataViewListStore();
        m_pListProfiles->AssociateModel(m_pModel.get());

        m_pListProfiles->AppendColumn( new wxDataViewColumn( _("Name"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ), PERM_COL_NAME_IDX, PERM_COL_NAME_WIDTH, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
        m_pListProfiles->AppendColumn( new wxDataViewColumn( _("Active"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ), PERM_COL_ACTIVE_IDX, PERM_COL_ACTIVE_WIDTH, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));

        Recargar();
        m_permisosCambiados = false;

        m_pListProfiles->Connect(wxEVT_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( PanelConfiguracionPermisos::OnListaPermisosChangeSelection ), NULL, this );
        m_pListProfiles->Connect(wxEVT_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( PanelConfiguracionPermisos::OnListaPermisosDClick ), NULL, this );

}

PanelConfiguracionPermisos::~PanelConfiguracionPermisos()
{
}

void PanelConfiguracionPermisos::Recargar()
{
        GNC::GCS::ConfigurationController::TListGroups profiles;
        GNC::GCS::ConfigurationController::Instance()->readGroupGeneral("/GinkgoCore/Profiles", profiles);

        std::string strTmp, xml;
        bool apply;
        bool primero =true;
        m_pListProfiles->DeleteAllItems();

        wxVector<wxVariant> data;

        for (GNC::GCS::ConfigurationController::TListGroups::iterator it = profiles.begin(); it != profiles.end(); ++it) {
                GNC::GCS::ConfigurationController::TListGroups::value_type tuple = (*it);
                data.clear();

                tuple.readStringValue("Name", strTmp);
                data.push_back(wxString::FromUTF8(strTmp.c_str()));
                tuple.readBoolValue("Apply", apply, false);
                if(apply) {
                        data.push_back(_("Yes"));
                } else {
                        data.push_back(_("No"));
                }
                tuple.readStringValue("XML", xml);
                data.push_back(wxString::FromUTF8(xml.c_str()));

                m_pModel->AppendItem(data);

                if(primero) {
                        if(m_pModel->GetCount() > 0) {
                                wxDataViewItem item = m_pModel->GetItem(0);
                                if (item.IsOk()) {
                                        m_pListProfiles->SelectRow(0);
                                }
                        }
                        m_pBEdit->Enable(true);
                        m_pBDelete->Enable(true);
                        m_pBActive->Enable(true);
                        primero = false;
                }
        }
}

//region "Metodos de IPasoConfiguracion"
wxWindow* PanelConfiguracionPermisos::GetPanel()
{
        return this;
}

std::string PanelConfiguracionPermisos::GetTitle()
{
        return _Std("Profiles");
}

std::string PanelConfiguracionPermisos::GetCabecera()
{
        return _Std("Profiles Settings");
}

bool PanelConfiguracionPermisos::Validar()
{
        bool ok = true;
        //si modificado... los cambios se recargaran ahora pero puede haber incongruencias
        if(m_permisosCambiados) {
                GNC::ControladorPermisos::FreeInstance();
                wxMessageBox(_("Ginkgo CADx must restart for the permission changes to take effect"),_("Info"));
        }
        return ok;
}

bool PanelConfiguracionPermisos::Guardar()
{
        GNC::GCS::ConfigurationController::Instance()->deleteEntryGeneral("/GinkgoCore/Profiles");
        GNC::GCS::ConfigurationController::TListGroups profiles;

        for (unsigned int i = 0; i< m_pModel->GetItemCount(); i++) {
                GNC::GCS::ConfigurationController::TMapValues profile;
                wxVariant value;
                m_pModel->GetValueByRow(value, i, PERM_COL_NAME_IDX);
                profile["Name"] = value.GetString().ToUTF8();
                m_pModel->GetValueByRow(value, i, PERM_COL_ACTIVE_IDX);
                profile.insertBool("Apply", value.GetString().CompareTo(_("Yes")) == 0);
                m_pModel->GetValueByRow(value, i, PERM_COL_XML);
                profile["XML"] = value.GetString().ToUTF8();
                profiles.push_back(profile);
        }
        GNC::GCS::ConfigurationController::Instance()->writeGroupGeneral("/GinkgoCore/Profiles", profiles, "Profile");

        return true;
}
//endregion


void PanelConfiguracionPermisos::OnListaPermisosDClick(wxDataViewEvent& )
{
        wxCommandEvent evt;
        OnEditarClick(evt);
}

void PanelConfiguracionPermisos::OnListaPermisosChangeSelection(wxDataViewEvent& event)
{
        if(event.GetItem().IsOk()) {
                m_pBEdit->Enable(true);
                m_pBDelete->Enable(true);
                m_pBActive->Enable(true);
        } else {
                m_pBEdit->Enable(false);
                m_pBDelete->Enable(false);
                m_pBActive->Enable(false);
        }
}

void PanelConfiguracionPermisos::OnSetActive(wxCommandEvent &)
{
        wxDataViewItem selection = m_pListProfiles->GetSelection();
        if (selection.IsOk()) {
                wxDataViewItemArray items;
                for (unsigned int i = 0; i < m_pModel->GetCount(); ++i) {
                        wxDataViewItem item = m_pModel->GetItem(i);
                        m_pModel->SetValue(_("No"), item, PERM_COL_ACTIVE_IDX);
                        items.Add(item);
                }
                m_pModel->SetValue(_("Yes"), selection, PERM_COL_ACTIVE_IDX);
                m_pModel->ItemsChanged(items);

                OnPropiedadCambiada();
        }
}

void PanelConfiguracionPermisos::OnNuevoClick(wxCommandEvent &)
{
        //se le pasa la lista de identificadores para que no duplique

        std::list<std::string> listaNombres;
        wxVariant val;
        for (unsigned int i = 0; i < m_pModel->GetCount(); ++i) {
                m_pModel->GetValueByRow(val, i, PERM_COL_NAME_IDX);
                listaNombres.push_back(std::string(val.GetString().ToUTF8()));
        }

        ProfileDialog dialogo(this, listaNombres, true);
        dialogo.ShowModal();
        if(dialogo.m_Ok) {
                std::string name, xml;
                dialogo.GetParametros(name, xml);
                bool apply = true;
                if(m_pModel->GetCount() > 0) {
                        wxDataViewItem item = m_pModel->GetItem(0);
                        if (item.IsOk()) {
                                apply = false;
                        }
                }

                wxVector<wxVariant> data;
                data.push_back(wxString::FromUTF8(name.c_str()));
                if (apply) {
                        data.push_back(_("Yes"));
                } else {
                        data.push_back(_("No"));
                }
                data.push_back(wxString::FromUTF8(xml.c_str()));
                m_pModel->AppendItem(data);


                wxVariant val;
                bool notFound = true;
                for (unsigned int i = 0; notFound && i < m_pModel->GetCount(); ++i) {
                        m_pModel->GetItem(i);
                        m_pModel->GetValueByRow(val, i, PERM_COL_NAME_IDX);
                        std::string stdVal(val.GetString().ToUTF8());
                        if (stdVal == name) {
                                notFound = false;
                                m_pListProfiles->SelectRow(i);
                        }
                }

                OnPropiedadCambiada();
        }
}

void PanelConfiguracionPermisos::OnEditarClick(wxCommandEvent &)
{

        wxDataViewItem selection = m_pListProfiles->GetSelection();

        if (selection.IsOk()) {
                wxVariant val;
                //se le pasa la lista de identificadores para que no duplique
                std::list<std::string> listaNombres;
                for (unsigned int i = 0; i < m_pModel->GetCount(); ++i) {
                        m_pModel->GetValueByRow(val, i, PERM_COL_NAME_IDX);
                        listaNombres.push_back(std::string(val.GetString().ToUTF8()));
                }

                ProfileDialog dialogo(this, listaNombres);

                m_pModel->GetValue(val, selection, PERM_COL_NAME_IDX);
                std::string name(val.GetString().ToUTF8());
                m_pModel->GetValue(val, selection, PERM_COL_XML);
                std::string xml(val.GetString().ToUTF8());

                dialogo.SetParametros(name, xml);

                dialogo.ShowModal();
                if(dialogo.m_Ok) {
                        dialogo.GetParametros( name, xml);
                        m_pModel->SetValue(wxVariant(wxString::FromUTF8(name.c_str())), selection, PERM_COL_NAME_IDX);
                        m_pModel->SetValue(wxVariant(wxString::FromUTF8(xml.c_str())), selection, PERM_COL_XML);
                        m_pModel->ItemChanged(selection);
                        OnPropiedadCambiada();
                }
        }
}

void PanelConfiguracionPermisos::OnEliminarClick(wxCommandEvent &)
{

        wxDataViewItem selection = m_pListProfiles->GetSelection();

        if(selection.IsOk()) {

                wxVariant val;
                m_pModel->GetValue(val, selection, PERM_COL_NAME_IDX);
                std::string idSeleccionado(val.GetString().ToUTF8());


                int answer = wxMessageBox(_("Confirm the removal of the profile ") + wxString::FromUTF8(idSeleccionado.c_str()), _("Data modified"), wxYES_NO | wxCANCEL, this);
                if(answer == wxYES) {
                        m_pModel->DeleteItem(m_pListProfiles->GetSelectedRow());
                        OnPropiedadCambiada();
                }
        }
}

void PanelConfiguracionPermisos::OnPropiedadCambiada()
{
        m_permisosCambiados = true;
        IPasoConfiguracion::OnPropiedadCambiada();
}

void PanelConfiguracionPermisos::OnTextoCambiado(wxCommandEvent & )
{
}

void PanelConfiguracionPermisos::OnSize(wxSizeEvent & event)
{
        event.Skip(true);
}
}
}
