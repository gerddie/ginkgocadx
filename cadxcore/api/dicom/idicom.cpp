#include "idicom.h"
#include "dcmtk/dcmdata/dcdict.h"
#include <dcmtk/dcmdata/dctk.h>
#include "dcmtk/dcmimgle/dcmimage.h"

GIL::DICOM::TipoTagPrivado::TipoTagPrivado()
{
}

GIL::DICOM::TipoTagPrivado::~TipoTagPrivado()
{
}


GIL::DICOM::TagPrivadoString::TagPrivadoString(const std::string& valor):Valor(valor)
{
}

GIL::DICOM::TagPrivadoString::~TagPrivadoString()
{
}

std::string GIL::DICOM::TagPrivadoString::ToString() {
	return Valor;
}

/*------------------------------*/
DcmElement* GIL::DICOM::TagPrivadoString::ToElement(unsigned int group, unsigned int element)
{
	OFCondition cond;
	DcmVR vr(EVR_LT);
	DcmTagKey key(group,element);
	DcmTag tag(key,vr);
	if (tag.error() != EC_Normal) {
		std::cerr << "error al almacenar los tags privados, tag desconocido: (" << group << ","<<element<<")" <<std::endl;
		return NULL;
	}

	DcmElement* pElement = newDicomElement(tag);

	if(pElement==NULL){
		std::cerr << "error al almacenar los tags privados, error al crear el elemento: (" << group << ","<<element<<") " <<std::endl;
		return NULL;
	}

	cond=pElement->putString(Valor.c_str());
			
	if (cond.bad()) {
		std::cerr << "error al almacenar los tags privados, error al escribir los datos: (" << group << ","<<element<<")" << cond.text() <<std::endl;
		return NULL;
	}

	return pElement;
}
/*------------------------------*/
GIL::DICOM::TagPrivadoUndefined::TagPrivadoUndefined()
{
	Valor = NULL;
	Size = 0;
}

GIL::DICOM::TagPrivadoUndefined::TagPrivadoUndefined(unsigned int size)
{
	Size = size;
	Valor = new unsigned char[Size];
}

GIL::DICOM::TagPrivadoUndefined::TagPrivadoUndefined(unsigned char* valor, unsigned int size)
{
	Size = size;
	Valor = new unsigned char[Size];
	memcpy(Valor,valor,Size*sizeof(unsigned char));
}

GIL::DICOM::TagPrivadoUndefined::~TagPrivadoUndefined() 
{
	if (Valor != NULL) {
		delete [] Valor;
	}
	Valor = NULL;
	Size = 0;
}

GIL::DICOM::TagPrivadoUndefined::TagPrivadoUndefined(const GIL::DICOM::TagPrivadoUndefined& otro) 
{
	this->Size = otro.Size;
	this->Valor = new unsigned char[Size];
	memcpy(otro.Valor,this->Valor,Size*sizeof(unsigned char));
}

GIL::DICOM::TagPrivadoUndefined& GIL::DICOM::TagPrivadoUndefined::operator = (const GIL::DICOM::TagPrivadoUndefined& otro) 
{
	this->Size = otro.Size;
	this->Valor = new unsigned char[Size];
	memcpy(otro.Valor,this->Valor,Size*sizeof(unsigned char));
	return *this;
}

void GIL::DICOM::TagPrivadoUndefined::Copiar(void* ptrCopia, int size) 
{
	if(this->Valor != NULL) {
		delete[] this->Valor;
	}
	this->Size = size;
	this->Valor = new unsigned char [Size];
	memcpy(this->Valor,ptrCopia,Size*sizeof(unsigned char));
}

std::string GIL::DICOM::TagPrivadoUndefined::ToString() 
{
	return std::string ((char*)Valor,Size);
}

unsigned char* GIL::DICOM::TagPrivadoUndefined::GetValor()
{
	return Valor;
}

unsigned int GIL::DICOM::TagPrivadoUndefined::GetSize()
{
	return Size;
}

DcmElement* GIL::DICOM::TagPrivadoUndefined::ToElement(unsigned int group, unsigned int element)
{
	OFCondition cond;
	DcmVR vr(EVR_UN);
	DcmTagKey key(group,element);
	DcmTag tag(key,vr);
	if (tag.error() != EC_Normal) {
		std::cerr << "error al almacenar los tags privados, tag desconocido: (" << group << ","<<element<<")" <<std::endl;
		return NULL;
	}

	DcmElement* pElement = newDicomElement(tag);

	if(pElement==NULL){
		std::cerr << "error al almacenar los tags privados, error al crear el elemento: (" << group << ","<<element<<")" <<std::endl;
		return NULL;
	}

	cond=pElement->putUint8Array(Valor,Size);
			
	if (cond.bad()) {
		std::cerr << "error al almacenar los tags privados, error al escribir los datos: (" << group << ","<<element<<")" <<std::endl;
		return NULL;
	}

	return pElement;
}



GIL::DICOM::TipoPrivateTags::TipoPrivateTags(){

}

GIL::DICOM::TipoPrivateTags::TipoPrivateTags(const std::string& uidModulo){
	this->UIDModulo = uidModulo;
}

GIL::DICOM::TipoPrivateTags::~TipoPrivateTags() {
	Vaciar();
}

GIL::DICOM::TipoPrivateTags::ListaTags& GIL::DICOM::TipoPrivateTags::GetListaTags()
{
	return Tags;
}

void GIL::DICOM::TipoPrivateTags::Vaciar()
{
	for(ListaTags::iterator it=Tags.begin(); it!=Tags.end(); ++it){
		if((*it).second!=NULL){
			delete ((*it).second);
		}
	}
	Tags.clear();
}

void GIL::DICOM::TipoPrivateTags::DeleteTag(unsigned char tag){
	if(Tags.find(tag)!=Tags.end()){
		//se borra
		if (Tags[tag] != NULL) {
			delete Tags[tag];
		}
		Tags.erase(tag);
	}
}

bool GIL::DICOM::TipoPrivateTags::GetTag(unsigned char tag, std::string& valor) {
	if(Tags.find(tag)!=Tags.end()){
		valor = Tags[tag]->ToString();
		return true;
	} else {
		valor="";
		return false;
	}
}

GIL::DICOM::TagPrivadoUndefined* GIL::DICOM::TipoPrivateTags::GetTagUndefined(unsigned char tag)
{
	if(Tags.find(tag)!=Tags.end()){
		TagPrivadoUndefined* pTag = dynamic_cast<TagPrivadoUndefined*> (Tags[tag]);
		return pTag;
	} else {
		return NULL;
	}				
}

void GIL::DICOM::TipoPrivateTags::SetTag(unsigned char tag, const std::string& valor){
	if(Tags.find(tag) != Tags.end()) {
		delete Tags[tag];
	}
	Tags[tag] = new TagPrivadoString(valor);
}

void GIL::DICOM::TipoPrivateTags::SetTag(unsigned char tag, unsigned char* valor, int longitud) {
	if(Tags.find(tag) != Tags.end()) {
		delete Tags[tag];
	}
	Tags[tag] = new TagPrivadoUndefined(valor, longitud);
}

GIL::DICOM::TagPrivadoUndefined* GIL::DICOM::TipoPrivateTags::NewTagUndefined(unsigned char tag, int longitud) {
	if(Tags.find(tag) != Tags.end()) {
		delete Tags[tag];
	}
	TagPrivadoUndefined* pTag = new TagPrivadoUndefined(longitud);
	Tags[tag] = pTag;
	return pTag;
}


GIL::DICOM::TipoMetaInfo::~TipoMetaInfo()
{
}
			
/* Obtiene un tag y lo asigna en el parámetro de salida valor.
	Devuelve true si el tag existe y false si el tag no existe */
bool GIL::DICOM::TipoMetaInfo::getTag(const std::string& tag, std::string & valor)
{
	ListaTags::iterator it = tags.find(tag);
	bool found = false;
	if (it != tags.end()) {
		valor = (*it).second;
		found = true;
	}
	return found;
}
			
std::string GIL::DICOM::TipoMetaInfo::getTag(const std::string& tag)
{
	ListaTags::iterator it = tags.find(tag);
	if (it != tags.end()) {
		return (*it).second;
	}
	return "";
}
			
template<class T> T GIL::DICOM::TipoMetaInfo::getTagAs(const std::string& tag, const T& defaultValue)
{
	T ret = defaultValue;
	std::stringstream is(getTag(tag));
	is >> ret;
	return ret;
}

GIL::DICOM::I2DException::I2DException(const std::string& msg, const std::string& componente, bool fatal) throw()  : GNC::GCS::IException(msg, componente, fatal)
{
}
GIL::DICOM::I2DException::~I2DException() throw()
{
}

