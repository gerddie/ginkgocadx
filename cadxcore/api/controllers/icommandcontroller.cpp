#include "icommandcontroller.h"
#include <main/controllers/commandcontroller.h>

GNC::GCS::CommandControllerException::CommandControllerException(const std::string& msg, const std::string& componente, bool fatal) throw() : GNC::GCS::IException(msg, componente, fatal)
{
}

GNC::GCS::ICommandController::ICommandController()
{
}

GNC::GCS::ICommandController::~ICommandController()
{
}

GNC::GCS::ICommandController* GNC::GCS::ICommandController::Instance()
{
	return GNC::CommandController::Instance();

}

void GNC::GCS::ICommandController::FreeInstance()
{
	GNC::CommandController::FreeInstance();

}