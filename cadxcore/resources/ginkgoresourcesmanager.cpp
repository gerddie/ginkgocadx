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

#include "ginkgoresourcesmanager.h"

#include <wx/image.h>
#include <wx/filename.h>

static wxString bundleDir(wxString(wxT("resources")) + wxFileName::GetPathSeparator() + wxT("cadxcore"));

static wxString AcquisitionDir(wxT("acquisition"));
static wxString AngleBarDir(wxT("angle_bar"));
static wxString MagnifierBarDir(wxT("magnifier_bar"));
static wxString GridBarDir(wxT("grid_bar"));
static wxString WindowLevelBarDir(wxT("windowlevel_bar"));
static wxString MarkingBarDir(wxT("marking_bar"));
static wxString HistoryDir(wxT("history"));
static wxString ToolIconsDir(wxT("tool_icons"));
static wxString MenuIconsDir(wxT("menu_icons"));
static wxString LogosDir(wxT("logos"));
static wxString BigIconsDir(wxT("big_icons"));
static wxString CursorsDir(wxT("cursors"));
static wxString ProgressBarDir(wxT("progress_bar"));
static wxString WizardsDir(wxT("wizards"));
static wxString LayoutBarDir(wxT("layout_bar"));
static wxString MenuTabsDir(wxT("menu_tabs"));
static wxString CineBarDir(wxT("cine_bar"));


static void LoadPNG(wxBitmap& bmp, const wxString& section, const wxString& filename)
{
        wxString path = wxString::FromUTF8(GNC::GCS::IEntorno::Instance()->GetGinkgoResourcesDir().c_str()) + wxFileName::GetPathSeparator() + bundleDir + wxFileName::GetPathSeparator() + section + wxFileName::GetPathSeparator() + filename;
        if (!bmp.LoadFile(path, wxBITMAP_TYPE_PNG)) {
                LOG_ERROR("Core/Resources", _Std("Unable to load PNG bitmap resource") << ": " << std::string(path.ToUTF8()));
        }
}

static void LoadPNG(wxImage& img, const wxString& section, const wxString& filename)
{
        wxString path = wxString::FromUTF8(GNC::GCS::IEntorno::Instance()->GetGinkgoResourcesDir().c_str()) + wxFileName::GetPathSeparator() + bundleDir + wxFileName::GetPathSeparator() + section + wxFileName::GetPathSeparator() + filename;
        if (!img.LoadFile(path, wxBITMAP_TYPE_PNG)) {
                LOG_ERROR("Core/Resources", _Std("Unable to load PNG image resource") << ": " << std::string(path.ToUTF8()));
        }
}

static void LoadGIF(wxAnimation& anim, const wxString& section, const wxString& filename)
{
        wxString path = wxString::FromUTF8(GNC::GCS::IEntorno::Instance()->GetGinkgoResourcesDir().c_str()) + wxFileName::GetPathSeparator() + bundleDir + wxFileName::GetPathSeparator() + section + wxFileName::GetPathSeparator() + filename;
        if (!anim.LoadFile(path, wxANIMATION_TYPE_GIF)) {
                LOG_ERROR("Core/Resources", _Std("Unable to load GIF animation resource") << ": " << std::string(path.ToUTF8()));
        }
}

// Adquisicion

wxBitmap GinkgoResourcesManager::Acquisition::GetIcoAdquireFile()
{
        wxBitmap bmp;
        LoadPNG(bmp, AcquisitionDir, wxT("ico_acquire_file.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::Acquisition::GetIcoAdquirePACS()
{
        wxBitmap bmp;
        LoadPNG(bmp, AcquisitionDir, wxT("ico_acquire_pacs.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::Acquisition::GetIcoDownload()
{
        wxBitmap bmp;
        LoadPNG(bmp, AcquisitionDir, wxT("ico_download.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::Acquisition::GetIcoClear()
{
        wxBitmap bmp;
        LoadPNG(bmp, AcquisitionDir, wxT("ico_clear.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::Acquisition::GetIcoRemoveDownload()
{
        wxBitmap bmp;
        LoadPNG(bmp, AcquisitionDir, wxT("ico_remove_download.png"));
        return bmp;
}

// Angle Bar

wxBitmap GinkgoResourcesManager::AngleBar::GetIcoAnguloCobb()
{
        wxBitmap bmp;
        LoadPNG(bmp, AngleBarDir, wxT("ico_angulo_cobb.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::AngleBar::GetIcoAnguloNormal()
{
        wxBitmap bmp;
        LoadPNG(bmp, AngleBarDir, wxT("ico_angulo_normal.png"));
        return bmp;
}

// Magnifier Bar
wxBitmap GinkgoResourcesManager::MagnifierBar::GetIcoZoomIn()
{
        wxBitmap bmp;
        LoadPNG(bmp, MagnifierBarDir, wxT("ico_zoom_in.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MagnifierBar::GetIcoZoomOut()
{
        wxBitmap bmp;
        LoadPNG(bmp, MagnifierBarDir, wxT("ico_zoom_out.png"));
        return bmp;
}

// Barra Grid Bar
wxBitmap GinkgoResourcesManager::GridBar::GetIcoVerRejilla()
{
        wxBitmap bmp;
        LoadPNG(bmp, GridBarDir, wxT("ico_ver_rejilla.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::GridBar::GetIcoOcultarRejilla()
{
        wxBitmap bmp;
        LoadPNG(bmp, GridBarDir, wxT("ico_ocultar_rejilla.png"));
        return bmp;
}

// WindowLevel Bar
wxBitmap GinkgoResourcesManager::WindowLevelBar::GetIcoReset()
{
        wxBitmap bmp;
        LoadPNG(bmp, WindowLevelBarDir, wxT("ico_reset_wl.png"));
        return bmp;
}

// Marking Bar
wxBitmap GinkgoResourcesManager::MarkingBar::GetIcoElipse()
{
        wxBitmap bmp;
        LoadPNG(bmp, MarkingBarDir, wxT("ico_elipse_marcado.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MarkingBar::GetIcoFlecha()
{
        wxBitmap bmp;
        LoadPNG(bmp, MarkingBarDir, wxT("ico_flecha_marcado.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MarkingBar::GetIcoPoligono()
{
        wxBitmap bmp;
        LoadPNG(bmp, MarkingBarDir, wxT("ico_poligono_marcado.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MarkingBar::GetIcoFreeForm()
{
        wxBitmap bmp;
        LoadPNG(bmp, MarkingBarDir, wxT("ico_free_form.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MarkingBar::GetIcoPunto()
{
        wxBitmap bmp;
        LoadPNG(bmp, MarkingBarDir, wxT("ico_punto_marcado.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MarkingBar::GetIcoRectangulo()
{
        wxBitmap bmp;
        LoadPNG(bmp, MarkingBarDir, wxT("ico_rectangulo_marcado.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MarkingBar::GetIcoTrapezoide()
{
        wxBitmap bmp;
        LoadPNG(bmp, MarkingBarDir, wxT("ico_rectangulo_marcado.png"));
        return bmp;
}

// History
wxBitmap GinkgoResourcesManager::History::GetIcoEstudio()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_estudio.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::History::GetIcoSerie()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_serie.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::History::GetIcoImagen()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_imagen.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::History::GetIcoHistorialOpen()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_historial_open.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::History::GetIcoHombre()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_hombre.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::History::GetIcoMujer()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_mujer.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::History::GetIcoOtro()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_otro.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::History::GetIcoFicheroClave()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_fichero_clave.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::History::GetIcoPlegar()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_plegar.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::History::GetIcoDesplegar()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_desplegar.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::History::GetMoviePreview()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_movie_preview.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::History::GetPDFPreview()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_pdf_preview.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::History::GetIcoUnknownFile()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_unknown_file.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::History::GetIcoCleanAll()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_clean_all.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::History::GetIcoAllPatients()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_all_patients.png"));
        return bmp;
}

wxBitmap GinkgoResourcesManager::History::GetIcoLocalLinked()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_local_linked.png"));
        return bmp;
}

wxBitmap GinkgoResourcesManager::History::GetIcoLocalFile()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_local_file.png"));
        return bmp;
}

wxBitmap GinkgoResourcesManager::History::GetIcoWadoLinkedRecent()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_wado_linked_recent.png"));
        return bmp;
}

wxBitmap GinkgoResourcesManager::History::GetIcoLocalLinkedRecent()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_local_linked_recent.png"));
        return bmp;
}

wxBitmap GinkgoResourcesManager::History::GetIcoLocalFileRecent()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_local_file_recent.png"));
        return bmp;
}

wxBitmap GinkgoResourcesManager::History::GetIcoWadoLinked()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_wado_linked.png"));
        return bmp;
}

wxBitmap GinkgoResourcesManager::History::GetIcoWadoLinkedPreview()
{
        wxBitmap bmp;
        LoadPNG(bmp, HistoryDir, wxT("ico_wado_linked_preview.png"));
        return bmp;
}

// Tool Icons
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoAngulo()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_angulo.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoAnotacion()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_anotacion.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoCruz()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_cruz.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoElevacion()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_elevacion.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoFlecha()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_flecha.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoLongitud()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_longitud.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoVarita()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_varita.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoLazoChroma()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_lazo_chroma.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoWindowLevel()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_wl.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoZoom()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_zoom.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoSinOpciones()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_sin_opciones.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoDebug()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_debug.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoMarcado()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_marcado.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoRejilla()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_rejilla.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoMostrarOcultarTags()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_mostrar_ocultar_tags.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoDesencajar()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_desencajar.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoCornerAnnotations()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_corner_annotations.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoInforme()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_informe.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoSlider()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_slider.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoLupa()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_lupa.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoPan()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_pan.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoMouseR()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_mouse_r.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoMouseL()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_mouse_l.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoMouseRL()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_mouse_rl.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoAnonymize()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_anonymize.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoLayout()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_layout.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoRotateLeft()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_rotate_left.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoRotateRight()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_rotate_right.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoFlipVertical()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_flip_vertical.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoFlipHorizontal()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_flip_horizontal.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoInvert()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_invert.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoTrackball()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_trackball.png"));
        return bmp;
}

wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoDownloadCloud()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_download_cloud.png"));
        return bmp;
}

wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoSearchCloud()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_search_cloud.png"));
        return bmp;
}

wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoCopyToClipboard()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_copy.png"));
        return bmp;
}

wxBitmap GinkgoResourcesManager::ToolIcons::GetIcoBrightnessContrast()
{
        wxBitmap bmp;
        LoadPNG(bmp, ToolIconsDir, wxT("ico_brightness_contrast.png"));
        return bmp;
}

// Menu Icons
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoAbrir()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_abrir.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoOpenDir()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_open_dir.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoRemovableUnit()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_open_disc.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoOpenFile()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_open_file.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoCerrarTab()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_cerrar_tab.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoConfiguracion()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_configuracion.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoEditor()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_editor.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoExportar()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_exportar.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoFiltrado()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_filtrado.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoGuardar()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_guardar.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoImportar()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_importar.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoDicomDir()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_dicom_dir.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoReset()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_reset.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoSalir()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_salir.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoImpresora()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_impresora.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoVistaPreliminar()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_vista_preliminar.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoCalibrarImagen()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_calibrar_imagen.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoEliminar()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_eliminar_widget.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoEliminarTodo()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_eliminar_todo.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoEditar()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_editar.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoSendToPACS()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_send_to_pacs.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoEliminarWidget()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_eliminar_widget.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoPantallaCompleta()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_pantalla_completa.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoUser()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_user.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoHelp()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_help.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoSupport()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_support.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoObtainExtensions()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_obtain_extensions.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoTick()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_tick.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoCross()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_cross.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuIcons::GetIcoPaste()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuIconsDir, wxT("ico_paste.png"));
        return bmp;
}

// Logos
wxBitmap GinkgoResourcesManager::Logos::GetLogoGinkgo48x48()
{
        wxBitmap bmp;
        LoadPNG(bmp, LogosDir, wxT("ginkgo48x48.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::Logos::GetLogoGinkgo16x16()
{
        wxBitmap bmp;
        LoadPNG(bmp, LogosDir, wxT("ico_ginkgo16x16.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::Logos::GetLogoGinkgo32x32()
{
        wxBitmap bmp;
        LoadPNG(bmp, LogosDir, wxT("ico_ginkgo32x32.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::Logos::GetLogoSplash()
{
        wxBitmap bmp;
        LoadPNG(bmp, LogosDir, wxT("ico_logo_splash.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::Logos::GetExtensionsLogo()
{
        wxBitmap bmp;
        LoadPNG(bmp, LogosDir, wxT("extensions_logo.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::Logos::GetExtensionsErrorLogo()
{
        wxBitmap bmp;
        LoadPNG(bmp, LogosDir, wxT("ico_extension_error.png"));
        return bmp;
}

// Big Icons
wxBitmap GinkgoResourcesManager::BigIcons::GetIcoBigBook()
{
        wxBitmap bmp;
        LoadPNG(bmp, BigIconsDir, wxT("ico_history_big.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::BigIcons::GetIcoBigDicomize()
{
        wxBitmap bmp;
        LoadPNG(bmp, BigIconsDir, wxT("ico_dicomize_big.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::BigIcons::GetIcoBigQR()
{
        wxBitmap bmp;
        LoadPNG(bmp, BigIconsDir, wxT("ico_qr_big.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::BigIcons::GetIcoBigAcquire()
{
        wxBitmap bmp;
        LoadPNG(bmp, BigIconsDir, wxT("ico_acquire_big.png"));
        return bmp;
}

wxBitmap GinkgoResourcesManager::BigIcons::GetIcoBigGinkgoCloudSearch()
{
        wxBitmap bmp;
        LoadPNG(bmp, BigIconsDir, wxT("ico_search_ginkgocloud.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::BigIcons::GetIcoDefaultDicomizer()
{
        wxBitmap bmp;
        LoadPNG(bmp, BigIconsDir, wxT("ico_default_dicomizer.png"));
        return bmp;
}

// Cursors
wxImage GinkgoResourcesManager::Cursors::GetCursorTransparente()
{
        wxImage img;
        LoadPNG(img, CursorsDir, wxT("ico_cursor_transparente.png"));
        return img;
}
wxImage GinkgoResourcesManager::Cursors::GetCursorLupa()
{
        wxImage img;
        LoadPNG(img, CursorsDir, wxT("ico_cursor_lupa.png"));
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 16);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 16);
        return img;
}
wxImage GinkgoResourcesManager::Cursors::GetCursorCrearLinea()
{
        wxImage img;
        LoadPNG(img, CursorsDir, wxT("ico_cursor_crear_linea.png"));
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 16);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 16);
        return img;
}
wxImage GinkgoResourcesManager::Cursors::GetCursorManoAbierta()
{
        wxImage img;
        LoadPNG(img, CursorsDir, wxT("ico_cursor_mano_abierta.png"));
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 16);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 16);
        return img;
}
wxImage GinkgoResourcesManager::Cursors::GetCursorManoCerrada()
{
        wxImage img;
        LoadPNG(img, CursorsDir, wxT("ico_cursor_mano_cerrada.png"));
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 16);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 16);
        return img;
}
wxImage GinkgoResourcesManager::Cursors::GetCursorSeleccionar()
{
        wxImage img;
        LoadPNG(img, CursorsDir, wxT("ico_cursor_seleccionar.png"));
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 16);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 16);
        return img;
}
wxImage GinkgoResourcesManager::Cursors::GetCursorCrearAnguloCobb()
{
        wxImage img;
        LoadPNG(img, CursorsDir, wxT("ico_cursor_crear_angulo_cobb.png"));
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 16);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 16);
        return img;
}
wxImage GinkgoResourcesManager::Cursors::GetCursorCrearAnguloNormal()
{
        wxImage img;
        LoadPNG(img, CursorsDir, wxT("ico_cursor_crear_angulo_normal.png"));
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 16);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 16);
        return img;
}
wxImage GinkgoResourcesManager::Cursors::GetCursorCrearAnotacion()
{
        wxImage img;
        LoadPNG(img, CursorsDir, wxT("ico_cursor_crear_anotacion.png"));
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 16);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 16);
        return img;
}
wxImage GinkgoResourcesManager::Cursors::GetCursorCrearElipse()
{
        wxImage img;
        LoadPNG(img, CursorsDir, wxT("ico_cursor_crear_elipse.png"));
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 16);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 16);
        return img;
}
wxImage GinkgoResourcesManager::Cursors::GetCursorCrearPoligono()
{
        wxImage img;
        LoadPNG(img, CursorsDir, wxT("ico_cursor_crear_poligono.png"));
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 16);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 16);
        return img;
}
wxImage GinkgoResourcesManager::Cursors::GetCursorCrearPunto()
{
        wxImage img;
        LoadPNG(img, CursorsDir, wxT("ico_cursor_crear_punto.png"));
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 16);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 16);
        return img;
}
wxImage GinkgoResourcesManager::Cursors::GetCursorCrearRectangulo()
{
        wxImage img;
        LoadPNG(img, CursorsDir, wxT("ico_cursor_crear_rectangulo.png"));
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 16);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 16);
        return img;
}
wxImage GinkgoResourcesManager::Cursors::GetCursorWindowLevel()
{
        wxImage img;
        LoadPNG(img, CursorsDir, wxT("ico_cursor_window_level.png"));
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 4);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 26);
        return img;
}
wxImage GinkgoResourcesManager::Cursors::GetCursorAnclaSimple()
{
        wxImage img;
        LoadPNG(img, CursorsDir, wxT("ico_cursor_ancla_simple.png"));
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 16);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 16);
        return img;
}
wxImage GinkgoResourcesManager::Cursors::GetCursorAnclaCompuesta()
{
        wxImage img;
        LoadPNG(img, CursorsDir, wxT("ico_cursor_ancla_compuesta.png"));
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 16);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 16);
        return img;
}

wxImage GinkgoResourcesManager::Cursors::GetRotationCursor()
{
        wxImage img;
        LoadPNG(img, CursorsDir, wxT("ico_rotate_cursor.png"));
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 16);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 16);
        return img;
}

// Progres Bar
wxAnimation GinkgoResourcesManager::ProgressBar::GetAniLoading()
{
        wxAnimation animation;
        LoadGIF(animation, ProgressBarDir, wxT("ico_loading.gif"));
        return animation;
}
wxBitmap GinkgoResourcesManager::ProgressBar::GetIcoStopped()
{
        wxBitmap bmp;
        LoadPNG(bmp, ProgressBarDir, wxT("ico_stopped.png"));
        return bmp;
}

// Wizards
wxBitmap GinkgoResourcesManager::Wizards::GetIcoBarraSuperior()
{
        wxBitmap bmp;
        LoadPNG(bmp, WizardsDir, wxT("ico_barra_superior.png"));
        return bmp;
}

// Layout bar
wxBitmap GinkgoResourcesManager::LayoutBar::GetIcoAddColumn()
{
        wxBitmap bmp;
        LoadPNG(bmp, LayoutBarDir, wxT("ico_add_columna.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::LayoutBar::GetIcoAddRow()
{
        wxBitmap bmp;
        LoadPNG(bmp, LayoutBarDir, wxT("ico_add_fila.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::LayoutBar::GetIcoDelColumn()
{
        wxBitmap bmp;
        LoadPNG(bmp, LayoutBarDir, wxT("ico_del_columna.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::LayoutBar::GetIcoDelRow()
{
        wxBitmap bmp;
        LoadPNG(bmp, LayoutBarDir, wxT("ico_del_fila.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::LayoutBar::GetIcoLayout1x1()
{
        wxBitmap bmp;
        LoadPNG(bmp, LayoutBarDir, wxT("ico_layout_1x1.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::LayoutBar::GetIcoLayout1x2()
{
        wxBitmap bmp;
        LoadPNG(bmp, LayoutBarDir, wxT("ico_layout_1x2.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::LayoutBar::GetIcoLayout2x1()
{
        wxBitmap bmp;
        LoadPNG(bmp, LayoutBarDir, wxT("ico_layout_2x1.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::LayoutBar::GetIcoLayout2x2()
{
        wxBitmap bmp;
        LoadPNG(bmp, LayoutBarDir, wxT("ico_layout_2x2.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::LayoutBar::GetIcoLayout3x3()
{
        wxBitmap bmp;
        LoadPNG(bmp, LayoutBarDir, wxT("ico_layout_3x3.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::LayoutBar::GetIcoLayout4x4()
{
        wxBitmap bmp;
        LoadPNG(bmp, LayoutBarDir, wxT("ico_layout_4x4.png"));
        return bmp;
}

wxBitmap GinkgoResourcesManager::LayoutBar::GetIcoLayout1x2Blue()
{
        wxBitmap bmp;
        LoadPNG(bmp, LayoutBarDir, wxT("ico_layout_1x2_blue.png"));
        return bmp;
}

wxBitmap GinkgoResourcesManager::LayoutBar::GetIcoLayout2x1Blue()
{
        wxBitmap bmp;
        LoadPNG(bmp, LayoutBarDir, wxT("ico_layout_2x1_blue.png"));
        return bmp;
}

// Menu Tabs
wxBitmap GinkgoResourcesManager::MenuTabs::GetIcoCerrarTab()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuTabsDir, wxT("ico_cerrar_tab_32.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuTabs::GetIcoPrimerPlano()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuTabsDir, wxT("ico_primer_plano_tab.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuTabs::GetIcoGrid2Col()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuTabsDir, wxT("ico_grid_2_col.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuTabs::GetIcoGrid3Col()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuTabsDir, wxT("ico_grid_3_col.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::MenuTabs::GetIcoReagrupar()
{
        wxBitmap bmp;
        LoadPNG(bmp, MenuTabsDir, wxT("ico_reagrupar.png"));
        return bmp;
}

// Cine bar
wxBitmap GinkgoResourcesManager::CineBar::GetIcoPlay()
{
        wxBitmap bmp;
        LoadPNG(bmp, CineBarDir, wxT("ico_play.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::CineBar::GetIcoBucle()
{
        wxBitmap bmp;
        LoadPNG(bmp, CineBarDir, wxT("ico_bucle.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::CineBar::GetIcoNoBucle()
{
        wxBitmap bmp;
        LoadPNG(bmp, CineBarDir, wxT("ico_no_bucle.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::CineBar::GetIcoStop()
{
        wxBitmap bmp;
        LoadPNG(bmp, CineBarDir, wxT("ico_stop.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::CineBar::GetIcoPause()
{
        wxBitmap bmp;
        LoadPNG(bmp, CineBarDir, wxT("ico_pause.png"));
        return bmp;
}
wxBitmap GinkgoResourcesManager::CineBar::GetIcoCine()
{
        wxBitmap bmp;
        LoadPNG(bmp, CineBarDir, wxT("ico_cine.png"));
        return bmp;
}
