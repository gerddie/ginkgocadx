#pragma once
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

#ifndef IEXCEPTION_H
#define IEXCEPTION_H

#include <string>
#include <exception>

#include <api/api.h>

namespace GNC
{
namespace GCS
{
class EXTAPI IException : public std::exception
{
public:
        IException(const std::string& msg, const std::string& component="GNC", bool fatal = true) throw();

        virtual ~IException() throw();

        const std::string& GetComponent() const;

        const std::string& GetCause() const;

        virtual const char* what() const throw();

        std::string GetFullCause() const;

        bool IsFatal()  const;

        operator std::string () const;

        inline const std::string str() const
        {
                return (std::string) *this;
        }

protected:
        std::string cause;
        std::string component;
        bool fatal;
};

class EXTAPI IRequestingException : public IException
{
public:
        IRequestingException(const std::string& msg, const std::string& component="GNC", bool fatal = true) throw();
        virtual ~IRequestingException() throw();

        virtual bool shouldExit() = 0;
};

}
}
#endif
