#include "ubication.h"

GIL::Ubicacion::Ubicacion() :
	Monitorize(false),
	CleanBefore(false),
	CleanAfter(false)
{
}

GIL::Ubicacion::Ubicacion(const std::string& titulo, const std::string& ruta, const std::string& descripcion, bool monitorize, bool cleanbefore, bool cleanafter)
{
	this->Titulo = titulo;
	Update(ruta, descripcion, monitorize, cleanbefore, cleanafter);

}

GIL::Ubicacion::Ubicacion(const GIL::Ubicacion& otro)
{
	*this = otro;
}

GIL::Ubicacion::~Ubicacion()
{

}

void GIL::Ubicacion::Update(const std::string& ruta, const std::string& desc, bool monitorize, bool cleanbefore, bool cleanafter)
{
	this->Ruta        = ruta;
	this->Descripcion = desc;
	this->Monitorize  = monitorize;
	this->CleanBefore = cleanbefore;
	this->CleanAfter  = cleanafter;
}

GIL::Ubicacion& GIL::Ubicacion::operator=(const GIL::Ubicacion& otro)
{
	this->Titulo      = otro.Titulo;
	this->Ruta        = otro.Ruta;
	this->Descripcion = otro.Descripcion;
	this->Monitorize	= otro.Monitorize;
	this->CleanBefore	= otro.CleanBefore;
	this->CleanAfter	= otro.CleanAfter;

	return *this;
}		

