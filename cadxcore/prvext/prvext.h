#pragma once
/*
 *  
 *  $Id: prvext.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
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

namespace GNC {
	namespace GCS {
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
		
		AsignarFechaCompilacion(__DATE__);
	}
	PrivateExtension(const std::string& sid, const std::string& provider, const std::string& description, const char* updateurl, const wxBitmap& bitmapIcon) : icon(bitmapIcon)
	{
		this->sid = sid;
		this->provider = provider;
		this->description = description;
		this->updateurl = updateurl;
		
		AsignarFechaCompilacion(__DATE__);
	}

	PrivateExtension(const PrivateExtension& o)
	{
		*this = o;
	}

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

	void AsignarFechaCompilacion(const std::string& fecha)
	{
		this->fecha_compilacion = fecha;
	}

	void AsignPath(const std::string& path) 
	{
		this->path = path;
	}

	const std::string& GetPath() const
	{
		return this->path;
	}

	const std::string& GetFechaCompilacion() const
	{
		return this->fecha_compilacion;
	}

	const std::string& GetUpdateURL() const
	{
		return updateurl;
	}

	wxBitmap& GetIcon() {
		return icon;
	}

	PrivateExtension& operator=(const PrivateExtension& o)
	{
		this->sid = o.sid;
		this->description = o.description;
		this->fecha_compilacion = o.fecha_compilacion;
		this->updateurl = o.updateurl;

		this->icon = o.icon;
		return *this;
	}

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
		}
		else {
			return os << "NULL";
		}
	}

protected:
	std::string sid;
	std::string provider;
	std::string description;
	std::string fecha_compilacion;
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