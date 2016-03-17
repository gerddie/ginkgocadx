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

#include "controladorpermisos.h"
#include "controladorlog.h"
#include <wx/xml/xml.h>
#include <wx/sstream.h>

namespace GNC
{

ControladorPermisos* ControladorPermisos::m_pInstance = 0;

ControladorPermisos::ControladorPermisos()
{

}

ControladorPermisos::~ControladorPermisos()
{
        LimpiarPermisos();
}

ControladorPermisos* ControladorPermisos::Instance()
{
        if (m_pInstance == NULL) {
                m_pInstance = new ControladorPermisos();
        }
        return m_pInstance;
}

void ControladorPermisos::FreeInstance()
{
        if (m_pInstance != NULL) {
                delete m_pInstance;
                m_pInstance = 0;
        }
}

void ControladorPermisos::LimpiarPermisos()
{
        for(TipoIteradorMapaPermisosNamespace it = m_mapaPermisos.begin(); it!= m_mapaPermisos.end(); ++it) {
                (*it).second->clear();
                delete (*it).second;
        }
        m_mapaPermisos.clear();
}

//region "Realizacion de la interfaz IControladorPermisos

/** Carga el fichero XML de permisos **/
void ControladorPermisos::CargarXMLPermisos(const std::string& xmlPermisos, bool privados)
{
        wxStringInputStream input(wxString::FromUTF8(xmlPermisos.c_str()));
        wxXmlDocument doc(input);
        if(!doc.IsOk() || doc.GetRoot()->GetName() != wxT("permisos")) {
                throw GNC::GCS::ControladorPermisosException("xml mal formado");
        }

        wxString wxNamespace;
        wxString wxId;
        wxString wxAccion;
        wxString wxActivoPorDefecto;
        wxString wxValorPorDefecto;
        wxString wxValor;

        std::string stdNamespace;
        std::string stdId;
        std::string stdAccion;
        std::string stdValorPorDefecto;
        std::string stdValor;

        bool activoPorDefecto;
        bool activo;

        for(wxXmlNode* permiso = doc.GetRoot()->GetChildren(); permiso != NULL; permiso = permiso->GetNext()) {
                if(permiso->GetName() == wxT("permiso")) {
                        if(
                                permiso->GetAttribute(wxT("namespace"), &wxNamespace) &&
                                permiso->GetAttribute(wxT("id"), &wxId) &&
                                permiso->GetAttribute(wxT("accion"), &wxAccion) &&
                                permiso->GetAttribute(wxT("activopordefecto"), &wxActivoPorDefecto) ) {
                                if (!permiso->GetAttribute(wxT("valorpordefecto"), &wxValorPorDefecto) || !permiso->GetAttribute(wxT("valor"), &wxValor) ) {
                                        for (wxXmlNode* child = permiso->GetChildren(); child != NULL; child = child->GetNext()) {
                                                if (child->GetChildren() == NULL) {
                                                        continue;
                                                }
                                                if (child->GetName() == wxT("default_value")) {
                                                        wxValorPorDefecto = child->GetChildren()->GetContent();
                                                        wxValorPorDefecto.Trim();
                                                } else if (child->GetName() == wxT("value")) {
                                                        wxValor = child->GetChildren()->GetContent();
                                                        wxValor.Trim();
                                                }
                                        }
                                }

                                stdNamespace = wxNamespace.ToUTF8();
                                stdId = wxId.ToUTF8();
                                stdAccion = wxAccion.ToUTF8();
                                stdValorPorDefecto = wxValorPorDefecto.ToUTF8();
                                stdValor = wxValor.ToUTF8();

                                if (wxActivoPorDefecto == wxT("true")) {
                                        activoPorDefecto = true;
                                } else {
                                        activoPorDefecto = false;
                                }

                                //se busca la definicion del permiso
                                TipoMapaDefiniciones::iterator itDefinicion = m_mapaDefiniciones.find(stdNamespace + "." + stdId);
                                //Si el permiso no esta definido asumimos que es privado

                                bool esPrivado = true;
                                if (itDefinicion != m_mapaDefiniciones.end()) {
                                        esPrivado = itDefinicion->second.privado;
                                }

                                if((esPrivado && privados) || !esPrivado) {
                                        //se establece si es privado y la carga incluye privados, o no es privado
                                        TipoMapaPermisos* pMapaPermisos = GetMapaPermisos(stdNamespace);
                                        if(pMapaPermisos == NULL) {
                                                pMapaPermisos = new TipoMapaPermisos();
                                                m_mapaPermisos[stdNamespace] = pMapaPermisos;
                                        }

                                        if(stdAccion == "activar") {
                                                activo = true;
                                        } else {
                                                activo = false;
                                        }

                                        (*pMapaPermisos)[stdId] = GNC::GCS::Permisos::EstadoPermiso(stdValorPorDefecto, stdValor, activoPorDefecto, activo);
                                }
                        }
                }
        }
}

/** Resetea el estado y valor de todos los permisos a sus valores por defecto **/
void ControladorPermisos::ResetearValoresPorDefecto()
{
        for (TipoMapaPermisosNamespace::iterator it = m_mapaPermisos.begin(); it != m_mapaPermisos.end(); ++it) {
                TipoMapaPermisos* pNamespace = (*it).second;
                for (TipoMapaPermisos::iterator it2 = pNamespace->begin(); it2 != pNamespace->end(); ++it2) {
                        GNC::GCS::Permisos::EstadoPermiso& perm = (*it2).second;
                        perm.Reset();
                }
        }
}

/** Metodo recomendado para obtener permisos **/
GNC::GCS::Permisos::EstadoPermiso* ControladorPermisos::ObtenerPermiso(const std::string& stdNamespace, const std::string& idSujeto)
{
        TipoMapaPermisos* pNamespace = GetMapaPermisos(stdNamespace);
        if(pNamespace != NULL) {
                TipoMapaPermisos::iterator it = pNamespace->find(idSujeto);
                if (it == pNamespace->end()) {
                        return NULL;
                } else {
                        return &( (*it).second );
                }
        } else {
                return NULL;
        }
}

/** Metodo recomendado para comprobar permisos **/
GNC::GCS::Permisos::EstadoPermiso ControladorPermisos::Get(const std::string& stdNamespace, const std::string& idSujeto)
{
        std::string clave = stdNamespace + "." + idSujeto;
        TipoMapaPermisos* pNamespace = GetMapaPermisos(stdNamespace);
        if(pNamespace != NULL) {
                TipoMapaPermisos::iterator it = pNamespace->find(idSujeto);
                if (it != pNamespace->end()) {
                        return (*it).second;
                }
        }
        // Si no esta establecido el permiso, devolvemos un permiso por defecto.
        TipoMapaDefiniciones::iterator itd = m_mapaDefiniciones.find(clave);
        if ( itd != m_mapaDefiniciones.end() ) { // Devolvemos un permiso por defecto
                return (*itd).second.GenerarPermisoPorDefecto();
        } else {
                return GNC::GCS::Permisos::EstadoPermiso();
        }

}

/** Anyade una nueva definicion de permiso al conjunto **/
void ControladorPermisos::AddDefinicionPermiso(const std::string& espacioNombres, const std::string& descripcionNamespace, const std::string& id, const std::string& descripcion, bool privado, const std::string& valorPorDefecto, bool activoPorDefecto)
{
        std::string clave = espacioNombres + "." + id;
        m_mapaDefiniciones[clave] =	TipoDefinicionPermiso(espacioNombres, descripcionNamespace, id, descripcion, privado, valorPorDefecto, activoPorDefecto);
}
// endregion

GNC::ControladorPermisos::TipoMapaPermisos* ControladorPermisos::GetMapaPermisos(const std::string& stdNamespace)
{
        TipoIteradorMapaPermisosNamespace it = m_mapaPermisos.find(stdNamespace);
        if(it != m_mapaPermisos.end()) {
                return (*it).second;
        }
        return NULL;
}

const ControladorPermisos::TipoMapaDefiniciones& ControladorPermisos::GetMapaDefiniciones()
{
        return m_mapaDefiniciones;
}

ControladorPermisos::TipoMapaPermisosNamespace ControladorPermisos::GetPermisosFromXML(const std::string& xmlPermisos)
{
        TipoMapaPermisosNamespace mapaPermisos;
        wxStringInputStream input(wxString::FromUTF8(xmlPermisos.c_str()));
        wxXmlDocument doc(input);
        if(!doc.IsOk() || doc.GetRoot()->GetName() != wxT("permisos")) {
                throw GNC::GCS::ControladorPermisosException("xml mal formado");
        }

        wxString wxNamespace;
        wxString wxId;
        wxString wxAccion;
        wxString wxActivoPorDefecto;
        wxString wxValorPorDefecto;
        wxString wxValor;

        std::string stdNamespace;
        std::string stdId;
        std::string stdAccion;
        std::string stdValorPorDefecto;
        std::string stdValor;

        bool activoPorDefecto;
        bool activo;

        for(wxXmlNode* permiso = doc.GetRoot()->GetChildren(); permiso != NULL; permiso = permiso->GetNext()) {
                if(permiso->GetName() == wxT("permiso")) {
                        if(
                                permiso->GetAttribute(wxT("namespace"), &wxNamespace) &&
                                permiso->GetAttribute(wxT("id"), &wxId) &&
                                permiso->GetAttribute(wxT("accion"), &wxAccion) &&
                                permiso->GetAttribute(wxT("activopordefecto"), &wxActivoPorDefecto) ) {
                                if (!permiso->GetAttribute(wxT("valorpordefecto"), &wxValorPorDefecto) || !permiso->GetAttribute(wxT("valor"), &wxValor) ) {
                                        for (wxXmlNode* child = permiso->GetChildren(); child != NULL; child = child->GetNext()) {
                                                if (child->GetChildren() == NULL) {
                                                        continue;
                                                }
                                                if (child->GetName() == wxT("default_value")) {
                                                        wxValorPorDefecto = child->GetChildren()->GetContent();
                                                        wxValorPorDefecto.Trim();
                                                } else if (child->GetName() == wxT("value")) {
                                                        wxValor = child->GetChildren()->GetContent();
                                                        wxValor.Trim();
                                                }
                                        }
                                }

                                stdNamespace = wxNamespace.ToUTF8();
                                stdId = wxId.ToUTF8();
                                stdAccion = wxAccion.ToUTF8();
                                stdValorPorDefecto = wxValorPorDefecto.ToUTF8();
                                stdValor = wxValor.ToUTF8();

                                if (wxActivoPorDefecto == wxT("true")) {
                                        activoPorDefecto = true;
                                } else {
                                        activoPorDefecto = false;
                                }

                                //se busca la definicion del permiso
                                TipoMapaPermisos* pMapaPermisos = NULL;
                                TipoIteradorMapaPermisosNamespace itBuscar = mapaPermisos.find(stdNamespace);
                                if(itBuscar != mapaPermisos.end()) {
                                        pMapaPermisos = (*itBuscar).second;
                                }
                                if(pMapaPermisos == NULL) {
                                        pMapaPermisos = new TipoMapaPermisos();
                                        mapaPermisos[stdNamespace] = pMapaPermisos;
                                }
                                if(stdAccion == "activar") {
                                        activo = true;
                                } else {
                                        activo = false;
                                }

                                (*pMapaPermisos)[stdId] = GNC::GCS::Permisos::EstadoPermiso(stdValorPorDefecto, stdValor, activoPorDefecto, activo);
                        }
                }
        }
        return mapaPermisos;
}


}

