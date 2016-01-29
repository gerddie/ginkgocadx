/*
 *  
 *  $Id: iexception.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */

#include <api/iexception.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::IException::IException(const std::string& msg, const std::string& component, bool fatal) throw()
{
	this->cause = msg;
	this->component = component;
	this->fatal = fatal;
}

GNC::GCS::IException::~IException() throw()
{
}

const std::string& GNC::GCS::IException::GetComponent() const
{
	return component;
}

const std::string& GNC::GCS::IException::GetCause() const {
	return cause;
}

const char* GNC::GCS::IException::what() const throw()
{
	return cause.c_str();
}

std::string GNC::GCS::IException::GetFullCause() const {
	return *this;
}

bool GNC::GCS::IException::IsFatal()  const {
	return fatal;
}

GNC::GCS::IException::operator std::string() const
{
	return _Std("Exception in component") + std::string(" ") + GetComponent() + std::string(" :\n")  + GetCause();
}

//=========================

GNC::GCS::IRequestingException::IRequestingException(const std::string& msg, const std::string& component, bool fatal) throw() : IException(msg, component, fatal)
{

}

GNC::GCS::IRequestingException::~IRequestingException() throw()
{

}