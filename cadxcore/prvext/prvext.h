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


#pragma once
#ifndef PRVEXT_H
#define PRVEXT_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <list>

#include <api/api.h>

#if defined(_WINDOWS)
#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>
#endif

#include <main/license.h>

#include <wx/bitmap.h>

class wxWindow;

namespace GNC
{
namespace GCS
{
class IEntorno;
class IModuleController;
}
}
class EXTAPI PrivateExtension
{
public:
        typedef std::list<GNC::GCS::IModuleController*> TListControllers;
        PrivateExtension(const std::string& sid, const std::string& provider, const std::string& description, const char* updateurl)
        {
                this->sid = sid;
                this->provider = provider;
                this->description = description;
                this->updateurl = updateurl;
        }
        PrivateExtension(const std::string& sid, const std::string& provider, const std::string& description, const char* updateurl, const wxBitmap& bitmapIcon) : icon(bitmapIcon)
        {
                this->sid = sid;
                this->provider = provider;
                this->description = description;
                this->updateurl = updateurl;
        }

        PrivateExtension(const PrivateExtension& o) = default;

        virtual ~PrivateExtension()
        {
        }

        virtual TListControllers InitializeLibrary(GNC::GCS::IEntorno* pEntorno) = 0;

        virtual bool IsActive() = 0;


        const std::string& GetSID() const
        {
                return this->sid;
        }

        const std::string& GetProvider() const
        {
                return this->provider;
        }

        const std::string& GetDescription() const
        {
                return this->description;
        }

        void AsignPath(const std::string& path)
        {
                this->path = path;
        }

        const std::string& GetPath() const
        {
                return this->path;
        }

        const std::string& GetUpdateURL() const
        {
                return updateurl;
        }

        wxBitmap& GetIcon()
        {
                return icon;
        }

        PrivateExtension& operator=(const PrivateExtension& o) = default;

        friend std::ostream& operator<<(std::ostream &os, const PrivateExtension& ext)
        {
                os << "SID=" << ext.sid.c_str() << std::endl;
                os << "DESCRIPTION=" << ext.description.c_str() << std::endl;
                os << "PROVIDER=" << ext.provider.c_str() << std::endl;
                os << "UPDATEURL=" << ext.updateurl.c_str();
                return os;
        }

        friend std::ostream& operator<<(std::ostream& os, PrivateExtension* ext)
        {
                if (ext != NULL) {
                        return os << (*ext);
                } else {
                        return os << "NULL";
                }
        }

protected:
        std::string sid;
        std::string provider;
        std::string description;
        std::string path;
        std::string updateurl;
        wxBitmap    icon;

private:
        int ParseSVNBuild(const char* str)
        {
                int num = -1;
                std::string beg;
                std::string end;
                std::stringstream ss(str);
                ss >> beg >> num >> end;
                return num;
        }

};

//define a function pointer type for convenience
#ifndef __PLUGIN_FUNCTION
#define __PLUGIN_FUNCTION
typedef PrivateExtension* ( *CreatePrivateExtension_function)();
#endif //__PLUGIN_FUNCTION

#endif
