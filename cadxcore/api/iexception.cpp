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

const std::string& GNC::GCS::IException::GetCause() const
{
        return cause;
}

const char* GNC::GCS::IException::what() const throw()
{
        return cause.c_str();
}

std::string GNC::GCS::IException::GetFullCause() const
{
        return *this;
}

bool GNC::GCS::IException::IsFatal()  const
{
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
