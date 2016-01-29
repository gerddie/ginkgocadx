
#include "icontroladorcarga.h"
GNC::GCS::ControladorCargaException::ControladorCargaException(const std::string& msg, const std::string& componente, bool fatal) : GNC::GCS::IException(msg, componente, fatal){
}

GNC::GCS::ControladorCargaException::~ControladorCargaException() throw()
{
}

GNC::GCS::IControladorCarga::IControladorCarga()
{
}

GNC::GCS::IControladorCarga::~IControladorCarga()
{
}

GNC::GCS::IStreamingLoader::IStreamingLoader()
{
}

GNC::GCS::IStreamingLoader::~IStreamingLoader()
{
}