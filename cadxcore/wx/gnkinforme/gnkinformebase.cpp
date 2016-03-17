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


#include <sstream>

#include <wx/tokenzr.h>
#include <wx/ginkgostyle/ginkgostyle.h>

#include "gnkinformebase.h"

#include <api/dicom/idicommanager.h>
#include <api/internationalization/internationalization.h>

#include <main/controllers/pacscontroller.h>
#include <main/controllers/controladorvistas.h>
#include <main/controllers/controladoreventos.h>

#include <export/tagsprivadoscomunes.h>

#include <resources/ginkgoresourcesmanager.h>

#define COL_MEDICO 0
#define COL_FECHA 1

namespace GNC
{
namespace GUI
{
GnkInformeBase::GnkInformeBase( wxWindow* parent):GNKInformeBaseWx(parent)
{
        m_pListaConsolidaciones->InsertColumn(COL_MEDICO,_("Physician"),wxLIST_FORMAT_LEFT);
        m_pListaConsolidaciones->InsertColumn(COL_FECHA,_("Commit date"),wxLIST_FORMAT_LEFT);
        this->SetScrollRate( 5, 5 );
        m_cadenaConsolidaciones = "";
        m_estado = NO_INICIALIZADO;
        ActualizarEstado(NO_EXISTE);
        m_pPanelCuerpo->SetScrollRate(0,0);
}

GnkInformeBase::~GnkInformeBase()
{
        ;
}

void GnkInformeBase::CargarInforme(const std::string& pathFicheroInforme)
{
        GNC::GCS::Ptr<GIL::DICOM::TipoPrivateTags> pTagsPrivados = new GIL::DICOM::TipoPrivateTags(UID_TAGS_PRIVADOS_INFORME);
        GNC::GCS::Ptr<GIL::DICOM::DicomDataset> pBase = new GIL::DICOM::DicomDataset;
        GIL::DICOM::IDICOMManager* pDicomManager = GIL::DICOM::PACSController::Instance()->CrearInstanciaDeDICOMManager();
        if(pDicomManager->CargarFichero(pathFicheroInforme,*pBase)) {
                pDicomManager->CargarTagsPrivados(*pTagsPrivados);
                SetTags(pBase,pTagsPrivados);
                m_pathInforme = pathFicheroInforme;
                ActualizarEstado(ALMACENADO);
        }
        GIL::DICOM::PACSController::Instance()->LiberarInstanciaDeDICOMManager(pDicomManager);
}

void GnkInformeBase::SetTags(GNC::GCS::Ptr<GIL::DICOM::DicomDataset>& pBase, GNC::GCS::Ptr<GIL::DICOM::TipoPrivateTags> pTagsPrivadosComunes)
{
        if(pBase.IsValid()) {
                //titulo...
                GIL::DICOM::DicomDataset& base = (*pBase);
                std::string tag;
                //nombre paciente
                {
                        std::ostringstream os;
                        if(base.getTag("0010|0010",tag)) {
                                m_pNombrePaciente->SetLabel(wxString::FromUTF8(tag.c_str()).Upper());
                        } else {
                                m_pNombrePaciente->SetLabel(wxString(_("UNKNOW")).Upper());
                        }

                        if(base.getTag(std::string("0010|0020"),tag)) { //id del paciente
                                m_pIdPaciente->SetLabel(wxString::FromUTF8(tag.c_str()));
                        } else {
                                m_pIdPaciente->SetLabel(_("<unknown ID>"));
                        }
                }
                //sexo
                if(base.getTag(std::string("0010|0040"),tag)) { //sexo
                        if(tag.compare("M")==0) {
                                m_pBitmapSexo->SetBitmap(GinkgoResourcesManager::History::GetIcoHombre());
                        } else if(tag.compare("F")==0) {
                                m_pBitmapSexo->SetBitmap(GinkgoResourcesManager::History::GetIcoMujer());
                        } else if(tag.compare("O")==0) {
                                m_pBitmapSexo->SetBitmap(GinkgoResourcesManager::History::GetIcoOtro());
                        } else {
                                m_pBitmapSexo->SetBitmap(GinkgoResourcesManager::History::GetIcoOtro());
                        }
                } else {
                        m_pBitmapSexo->SetBitmap(GinkgoResourcesManager::History::GetIcoOtro());
                }

                //datos demograficos
                TListaDatosDemograficos listaDatosDemograficos;

                if(base.getTag("0010|1010",tag) && tag != "") { // edad
                        listaDatosDemograficos.push_back(TDatoDemografico(_Std("Age"),tag));
                }

                if(base.getTag("0010|0030",tag)) {//fecha nacimiento
                        wxDateTime fecha;
                        fecha.ParseFormat(wxString::FromUTF8(tag.c_str()),wxT("%Y%m%d"));
                        if(fecha.IsValid()) {
                                listaDatosDemograficos.push_back(TDatoDemografico(_Std("Birth date") , std::string(fecha.Format(_("%m/%d/%Y")).ToUTF8())));
                        }
                }
                if(base.getTag("0010|1020",tag) && tag != "") {//altura
                        listaDatosDemograficos.push_back(TDatoDemografico(_Std("Height"),tag+" cm"));
                }
                if(base.getTag("0010|1030",tag) && tag != "") { //peso
                        listaDatosDemograficos.push_back(TDatoDemografico(_Std("Weight"),tag+" kg"));
                }

                AddDatosDemograficos(listaDatosDemograficos);
        }

        //datos del informe
        if(pTagsPrivadosComunes.IsValid()) {
                if(pTagsPrivadosComunes->UIDModulo == UID_TAGS_PRIVADOS_INFORME) {
                        std::string tmp;
                        if(pTagsPrivadosComunes->GetTag((char)TAG_LISTA_COMENTARIOS,tmp))	{
                                m_comentarios = tmp;
                                m_pComentarios->SetLabel(wxString::FromUTF8(tmp.c_str()));
                        }
                        if(pTagsPrivadosComunes->GetTag((char)TAG_LISTA_COMPROMISOS,tmp)) {
                                m_cadenaConsolidaciones = tmp;
                                wxStringTokenizer tkzLinea(wxString::FromUTF8(tmp.c_str()), wxT("\n"));
                                while ( tkzLinea.HasMoreTokens() ) {
                                        wxString linea = tkzLinea.GetNextToken();
                                        wxStringTokenizer tkzClaveValor(linea,wxT("\t"));
                                        if ( tkzClaveValor.HasMoreTokens() ) {
                                                wxString medico = tkzClaveValor.GetNextToken();
                                                if(tkzClaveValor.HasMoreTokens()) {
                                                        wxString fecha = tkzClaveValor.GetNextToken();
                                                        long fila = m_pListaConsolidaciones->InsertItem(m_pListaConsolidaciones->GetItemCount(),medico);
                                                        m_pListaConsolidaciones->SetItem(fila,COL_FECHA,fecha);
                                                }
                                        }
                                }
                        }
                }
        }

        m_pListaConsolidaciones->SetColumnWidth(COL_MEDICO,300);
        m_pListaConsolidaciones->SetColumnWidth(COL_FECHA,200);
        m_pPanelReport->Layout();
        Layout();
}

void GnkInformeBase::OnConsolidarClick(wxCommandEvent& /*event*/)
{
        OnConsolidar();
}

void GnkInformeBase::ConsolidarComun(GNC::GCS::Ptr<GIL::DICOM::TipoPrivateTags>& tagsPrivadosComunes, const std::string& medico )
{
        tagsPrivadosComunes->UIDModulo = UID_TAGS_PRIVADOS_INFORME;
        //comentarios
        std::string comentarios(m_pComentarios->GetValue().ToUTF8());
        tagsPrivadosComunes->SetTag((char)TAG_LISTA_COMENTARIOS, comentarios);
        //el changelog
        std::ostringstream os;
        if(m_cadenaConsolidaciones != "") {
                os << m_cadenaConsolidaciones << std::endl;
        }

        wxString fecha = wxDateTime::Now().Format(wxT("%H:%M:%S  %d/%m/%Y"));
        std::string medicoConsolida = medico;
        if (medicoConsolida == "") {
                medicoConsolida = _Std("Unknown");
        }
        os << medicoConsolida;
        os << "\t";
        os << fecha.ToUTF8();

        m_cadenaConsolidaciones = os.str();
        tagsPrivadosComunes->SetTag((char)TAG_LISTA_COMPROMISOS,m_cadenaConsolidaciones);

        long fila = m_pListaConsolidaciones->InsertItem(m_pListaConsolidaciones->GetItemCount(),wxString::FromUTF8(medicoConsolida.c_str()));
        m_pListaConsolidaciones->SetItem(fila,COL_FECHA,fecha);

        ActualizarEstado(ALMACENADO);
}

void GnkInformeBase::OnComentariosText(wxCommandEvent &)
{
        if(!wxFileExists(FROMPATH(m_pathInforme))) {
                ActualizarEstado(NO_EXISTE);
        } else {
                ActualizarEstado(ALMACENADO_CON_MODIFICACIONES_PENDIENTES);
        }
}

void GnkInformeBase::ActualizarEstado(TEstado estado)
{
        if(m_estado != estado) {
                m_estado = estado;
                switch(m_estado) {
                case ALMACENADO:
                        m_pLabelStatus->SetLabel(_("Commited"));
                        m_pLabelStatus->SetForegroundColour(wxColour(0,174,0));
                        m_pBConsolidar->Enable(false);
                        break;
                case ALMACENADO_CON_MODIFICACIONES_PENDIENTES:
                        m_pLabelStatus->SetLabel(_("Modified"));
                        m_pLabelStatus->SetForegroundColour(wxColour(174,0,0));
                        m_pBConsolidar->Enable(true);
                        break;
                case NO_INICIALIZADO:
                case NO_EXISTE:
                        m_pLabelStatus->SetLabel(_("Not commited"));
                        m_pLabelStatus->SetForegroundColour(wxColour(232,82,0));
                        m_pBConsolidar->Enable(true);
                        break;
                }
                m_pPanelBoton->Layout();
        }
}

wxWindow* GnkInformeBase::GetParentPanelesCustom()
{
        return m_pPanelCuerpo;
}

void GnkInformeBase::AddDatosDemograficos(const TListaDatosDemograficos& datosDemograficos)
{
        for(TListaDatosDemograficos::const_iterator it = datosDemograficos.begin(); it != datosDemograficos.end(); ++it) {
                //primero mira a ver si ya esta insertado...
                wxSizerItemList& listaHijos = m_pSizerDatosDemograficos->GetChildren();
                bool found = false;
                int i = 0;
                for (wxSizerItemList::iterator itEtiquetas = listaHijos.begin(); itEtiquetas != listaHijos.end(); ++itEtiquetas, i++) {
                        if (i%2 != 0)
                                continue;
                        wxStaticText* pTextClave = dynamic_cast<wxStaticText*>((*itEtiquetas)->GetWindow());
                        if (pTextClave != NULL) {
                                if (pTextClave->GetLabel() == wxString::FromUTF8((*it).clave.c_str())) {
                                        //encontrada clave, se busca el valor y se asigna
                                        itEtiquetas++;
                                        wxStaticText* pTextValor = dynamic_cast<wxStaticText*>((*itEtiquetas)->GetWindow());
                                        if (pTextValor != NULL) {
                                                pTextValor->SetLabel(wxString::FromUTF8((*it).valor.c_str()));
                                                found = true;
                                                break;
                                        }
                                }
                        }
                }
                if (!found) {
                        wxStaticText* m_staticTextClave = new wxStaticText( m_pPanelDatosDemograficos, wxID_ANY, wxString::FromUTF8((*it).clave.c_str()));
                        m_staticTextClave->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
                        m_pSizerDatosDemograficos->Add( m_staticTextClave, 0, wxALL, 5 );

                        wxStaticText* m_staticTextValor = new wxStaticText( m_pPanelDatosDemograficos, wxID_ANY, wxString::FromUTF8((*it).valor.c_str()));
                        m_pSizerDatosDemograficos->Add( m_staticTextValor, 0, wxALL, 5 );
                }
        }
}

void GnkInformeBase::SetPanelesCustom(const TListaPanelesCustom& listaPaneles)
{
        for(TListaPanelesCustom::const_iterator it = listaPaneles.begin(); it != listaPaneles.end(); ++it) {
                wxWindow* pWindow = (*it);
                if(pWindow->GetParent() != GetParentPanelesCustom()) {
                        pWindow->Reparent(GetParentPanelesCustom());
                }
                m_pSizerCustom->Add(pWindow,0,wxEXPAND);
                SubTitledPanelSeparatorV* pSep = new SubTitledPanelSeparatorV(GetParentPanelesCustom());
                m_pSizerCustom->Add(pSep,0,wxEXPAND);

        }
}
}
}

