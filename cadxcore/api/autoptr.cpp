#include <api/autoptr.h>
#include <api/internationalization/internationalization.h>

//----------------------------------------------------------------------------------------------------

GNC::GCS::NullPointerException::NullPointerException() throw() : GNC::GCS::IException(_Std("Attempted to dereference null pointer"))
{
}

GNC::GCS::NullPointerException::~NullPointerException() throw()
{
}

//----------------------------------------------------------------------------------------------------

GNC::PtrCounter::PtrCounter(unsigned c) : count(c)
{
}
//----------------------------------------------------------------------------------------------------
