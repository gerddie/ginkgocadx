/*
 *  
 *  $Id: imodulecontroller.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
//#define _GINKGO_TRACE
#include <wx/string.h>
#include <wx/bitmap.h>

#include "imodulecontroller.h"

#include <main/controllers/configurationcontroller.h>
#include <export/tagsprivadoscomunes.h>
#include <resources/ginkgoresourcesmanager.h>


GNC::GCS::ModoControlador::ModoControlador(int _id, const std::string& _descripcion, const TipoListaModalidades& listaModalidades, const TipoListaUIDsImportacion& listaImportacion, wxBitmap* _bitmap, T_HangingProtocolSupportLevel supportLevel) :
id(_id),
descripcion(_descripcion),
m_listaModalidades(listaModalidades),
m_listImportationUIDs(listaImportacion),
bitmap(_bitmap),
m_hangingSupportLevel(supportLevel)
{
}

GNC::GCS::ModoControlador::~ModoControlador()
{
	m_listaModalidades.clear();
}

const GNC::GCS::ModoControlador::TipoListaModalidades GNC::GCS::ModoControlador::GetModalidades() {
	return m_listaModalidades;
}

const std::string& GNC::GCS::ModoControlador::GetDescripcion(){
	return descripcion;
}

wxBitmap* GNC::GCS::ModoControlador::GetBitmap(){
	return bitmap;
}

const int GNC::GCS::ModoControlador::GetId() const{
	return id;
}


bool GNC::GCS::ModoControlador::SupportsModalityFile(const std::string &modalidad, const std::string& transferSyntax) const{
	for(TipoListaModalidades::const_iterator it = m_listaModalidades.begin(); it!= m_listaModalidades.end(); ++it){
		if((*it) == modalidad){
			if (transferSyntax != "1.2.840.10008.1.2.4.100" && transferSyntax != "1.2.840.10008.1.2.4.101") {
				return true;
			} else {
				return false;
			}
		}
	}
	return false;
}

GNC::GCS::ModoControlador::T_HangingProtocolSupportLevel GNC::GCS::ModoControlador::GetHangingSupportLevel() const
{
	return m_hangingSupportLevel;
}

bool GNC::GCS::ModoControlador::SupportsImportationUID(const std::string &importationUID) const {
	for(TipoListaUIDsImportacion::const_iterator it = m_listImportationUIDs.begin(); it!= m_listImportationUIDs.end(); ++it){
		if((*it) == importationUID){
			return true;
		}
	}
	return false;
}



GNC::GCS::IdHL7::IdHL7()
{
}

GNC::GCS::IdHL7::IdHL7(const IdHL7& o)
{
	*this = o;
}

GNC::GCS::IdHL7::IdHL7(const std::string& e, const std::string& c, const std::string& d, const std::string& n, const std::string& t, const std::string& td, const std::string& v) :
etiqueta(e), codigo(c), descripcion(d), namespaceid(n), tipoobservacion(t), tipodatos(td), version(v)
{
}

GNC::GCS::IdHL7& GNC::GCS::IdHL7::operator = (const GNC::GCS::IdHL7& o)
{
	etiqueta        = o.etiqueta;
	codigo          = o.codigo;
	descripcion     = o.descripcion;
	namespaceid     = o.namespaceid;
	tipoobservacion = o.tipoobservacion;
	tipodatos       = o.tipodatos;
	version		 = o.version;
	return *this;
}

GNC::GCS::IdHL7::operator const std::string() const {
	return codigo + "^" + descripcion + "^" + namespaceid;
}


GNC::GCS::IModuleController::IModuleController(GNC::GCS::IEntorno* pEntorno, const std::string& uid, int priority) :
m_pEntorno(pEntorno), m_UID(uid), m_priority(priority)
{
}

/* Destructor */
GNC::GCS::IModuleController::~IModuleController()
{
}

void GNC::GCS::IModuleController::RegistrarConfiguracion()
{
	bool written = false;

	std::string key;
	std::string val;

	GNC::GCS::ConfigurationController * pConfig = GNC::GCS::ConfigurationController::Instance();

	for (TipoListaIdsHL7::iterator it = m_ListaInformes.begin(); it != m_ListaInformes.end(); ++it)
	{
		const IdHL7& i = *it;

		key = i.etiqueta;
		if ( !pConfig->readStringGeneral("/GinkgoCore/HCE/Plantillas",key, val)) {
			pConfig->writeStringGeneral("/GinkgoCore/HCE/Plantillas",key,(const std::string)i);
			written = true;
		}
	}

	if (written) {
		pConfig->Flush();
	}

}


void GNC::GCS::IModuleController::GetTagsPrivadosReplace(GNC::GCS::IModuleController::TMapaTagsPrivados& mapaTags)
{
	if (mapaTags.find(UID_TAGS_PRIVADOS_COMUNES) == mapaTags.end()) {
		mapaTags[UID_TAGS_PRIVADOS_COMUNES] = TListaIdsTagsPrivados();
	}

	mapaTags[UID_TAGS_PRIVADOS_COMUNES].push_back(GNC::GCS::IModuleController::TPrivateTagReemplazable(TAG_WIDGETS, GNC::GCS::IModuleController::TPrivateTagReemplazable::XML));
}

bool GNC::GCS::IModuleController::SupportsImportationUID(const std::string &)
{
	return false;
}

bool GNC::GCS::IModuleController::MakeThumbnail(const GNC::GCS::IHistoryController::FileModel& /*model*/, ImgProxy<UCHAR3>& /*thumbnail*/)
{
	return false;
}

GNC::GCS::IModuleController::TExtraLicensesMap GNC::GCS::IModuleController::GetExtraLicenses()
{
	GNC::GCS::IModuleController::TExtraLicensesMap map;
	return map;
}

const std::string& GNC::GCS::IModuleController::GetUID() const
{
	return m_UID;
}

bool GNC::GCS::IModuleController::SoportaImportacion() const
{
	return false;
}

//si importa series true, si importa estudios false
bool GNC::GCS::IModuleController::ImportaSeries() const
{
	return true;
}

const std::string& GNC::GCS::IModuleController::GetImporterDescription() const
{
	return m_ImporterDescription;
}

wxBitmap GNC::GCS::IModuleController::GetImporterIcon() const
{
	return GinkgoResourcesManager::BigIcons::GetIcoDefaultDicomizer();
}


void GNC::GCS::IModuleController::GetPasosImportacion(IWizard* , std::list<IPasoWizard*> &, wxWindow* , std::string &, GNC::GCS::Ptr<GIL::IModeloIntegracion> )
{

}

void GNC::GCS::IModuleController::GetPasosConfiguracion( std::list<IPasoConfiguracion*> &, wxWindow*, IDialogoConfiguracion* ) const
{

}

GNC::GCS::ModoControlador* GNC::GCS::IModuleController::GetModo(int id) {
	if (id >= (int)m_ListaModos.size()) {
		return NULL;
	}
	else {
		int top = id - 1;
		IteradorListaModos it = m_ListaModos.begin();
		for (int i = 0; i < top; ++i, ++it);
		return *it;
	}
}

GNC::GCS::IModuleController::ListaModos& GNC::GCS::IModuleController::GetListaModos(){
	return m_ListaModos;
}

int GNC::GCS::IModuleController::GetPriority()
{
	return m_priority;
}