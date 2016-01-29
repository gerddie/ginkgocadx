#include "icontroladorlog.h"
#include <main/controllers/controladorlog.h>

GNC::GCS::IControladorLog* GNC::GCS::IControladorLog::Instance()
{
	return GNC::ControladorLog::Instance();
}

void GNC::GCS::IControladorLog::FreeInstance()
{
	GNC::ControladorLog::FreeInstance();
}

GNC::GCS::IControladorLog::IControladorLog()
{
}

GNC::GCS::IControladorLog::~IControladorLog()
{
}
