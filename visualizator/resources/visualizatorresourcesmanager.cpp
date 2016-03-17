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


#include <api/globals.h>
#include <api/ientorno.h>
#include <api/controllers/icontroladorlog.h>
#include <api/internationalization/internationalization.h>

#include "visualizatorresourcesmanager.h"

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/animate.h>
#include <wx/filename.h>

static wxString bundleDir(wxString(wxT("resources")) + wxFileName::GetPathSeparator() + wxT("visualizator"));

static wxString ToolIconsDir(wxT("tool_icons"));
static wxString LogosDir(wxT("logos"));
static wxString T3DBarDir(wxT("3d_bar"));
static wxString SynchronizationBarDir(wxT("synchronization_bar"));

static void LoadPNG(wxBitmap& bmp, const wxString& section, const wxString& filename)
{
        wxString path = wxString::FromUTF8(GNC::GCS::IEntorno::Instance()->GetGinkgoResourcesDir().c_str()) + wxFileName::GetPathSeparator() + bundleDir + wxFileName::GetPathSeparator() + section + wxFileName::GetPathSeparator() + filename;
        if (!bmp.LoadFile(path, wxBITMAP_TYPE_PNG)) {
                LOG_ERROR("Visualizator/Resources", _Std("Unable to load PNG bitmap resource") << ": " << std::string(path.ToUTF8()));
        }
}
/*
static void LoadPNG(wxImage& img, const wxString& section, const wxString& filename) {
	wxString path = wxString::FromUTF8(GNC::GCS::IEntorno::Instance()->GetGinkgoResourcesDir().c_str()) + wxFileName::GetPathSeparator() + bundleDir + wxFileName::GetPathSeparator() + section + wxFileName::GetPathSeparator() + filename;
	if (!img.LoadFile(path, wxBITMAP_TYPE_PNG)) {
		LOG_ERROR("Visualizator/Resources", _Std("Unable to load PNG image resource") << ": " << std::string(path.ToUTF8()));
	}
}

static void LoadGIF(wxAnimation& anim, const wxString& section, const wxString& filename) {
	wxString path = wxString::FromUTF8(GNC::GCS::IEntorno::Instance()->GetGinkgoResourcesDir().c_str()) + wxFileName::GetPathSeparator() + bundleDir + wxFileName::GetPathSeparator() + section + wxFileName::GetPathSeparator() + filename;
	if (!anim.LoadFile(path, wxANIMATION_TYPE_GIF)) {
		LOG_ERROR("Visualizator/Resources", _Std("Unable to load GIF animation resource") << ": " << std::string(path.ToUTF8()));
	}
}
*/

// Tool Icons
wxBitmap GNKVisualizator::ResourcesManager::ToolIcons::GetIcoCapas()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_capas.png"));
        return bmp;
}
wxBitmap GNKVisualizator::ResourcesManager::ToolIcons::GetIcoReset()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_reset.png"));
        return bmp;
}
wxBitmap GNKVisualizator::ResourcesManager::ToolIcons::GetIcoMenuImagen()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_menu_imagen.png"));
        return bmp;
}
wxBitmap GNKVisualizator::ResourcesManager::ToolIcons::GetIcoECGMetadata()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_ecg_metadata.png"));
        return bmp;
}

// Logos
wxBitmap GNKVisualizator::ResourcesManager::Logos::GetIcoVisualizator()
{
        wxBitmap bmp;
        LoadPNG(bmp, LogosDir, wxT("logo_visualizator.png"));
        return bmp;
}

// 3D Bar
wxBitmap GNKVisualizator::ResourcesManager::T3DBar::GetIco3DArrow()
{
        wxBitmap bmp;
        LoadPNG(bmp, T3DBarDir, wxT("ico_cursor3d.png"));
        return bmp;
}
wxBitmap GNKVisualizator::ResourcesManager::T3DBar::GetIcoStereo()
{
        wxBitmap bmp;
        LoadPNG(bmp, T3DBarDir, wxT("ico_stereo.png"));
        return bmp;
}
wxBitmap GNKVisualizator::ResourcesManager::T3DBar::GetIcoPin()
{
        wxBitmap bmp;
        LoadPNG(bmp, T3DBarDir, wxT("ico_plegar.png"));
        return bmp;
}
wxBitmap GNKVisualizator::ResourcesManager::T3DBar::GetIcoDemo()
{
        wxBitmap bmp;
        LoadPNG(bmp, T3DBarDir, wxT("ico_demo.png"));
        return bmp;
}

// Synchronization Bar
wxBitmap GNKVisualizator::ResourcesManager::SynchronizationBar::GetIcoDessincronizar()
{
        wxBitmap bmp;
        LoadPNG(bmp, SynchronizationBarDir, wxT("ico_dessincronizar.png"));
        return bmp;
}
