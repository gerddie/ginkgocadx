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

#include <vector>
#include <sstream>
#include <limits>

#include <main/controllers/modalitysettingscontroller.h>
#include <main/controllers/controladorextensiones.h>
#include <api/controllers/ipacscontroller.h>
#include <main/entorno.h>
#include <api/dicom/dcmdictionary.h>
#include <api/dicom/idicommanager.h>

#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>
#include <wx/tooltip.h>
#include <wx/pen.h>
#include <wx/pen.h>
#include <wx/sizer.h>
#include <wx/tokenzr.h>
#include <wx/msgdlg.h>
#include <wx/aui/auibar.h>
#include <wx/menu.h>
#include "defaultmodalitysettingsconfigurationpanel.h"
#include <resources/ginkgoresourcesmanager.h>

#define COL_LABEL_IDX				0
#define COL_LABEL_WIDTH				100
#define COL_WINDOW_IDX				1
#define COL_WINDOW_WIDTH			80
#define COL_LEVEL_IDX				2
#define COL_LEVEL_WIDTH				80

namespace GNC
{
namespace GUI
{

class ModalityPanel : public ModalityPanelBase
{

private:

        wxObjectDataPtr<wxDataViewListStore> m_pModel;
        typedef std::map<std::string, ModalityPanel*> TModalityMap;

public:



        class EditCornerAnnotationsDialog: public EditCornerAnnotationsDialogBase
        {
        public:
                EditCornerAnnotationsDialog(ModalityPanel* pParent, const GNC::GCS::Ptr<GNC::GCS::AnnotationSettings>& pAnnotationSettings): EditCornerAnnotationsDialogBase(pParent),
                        Parent(pParent)
                {
                        m_pHelpBitmap->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoHelp());
                        SetDefaultValue(pAnnotationSettings, true);
                        m_pMainPanel->Layout();
                        m_pBody->Layout();
                        Layout();
                }

                virtual void OnSetDefaultValues(wxCommandEvent& /*evt*/)
                {
                        GNC::GCS::Ptr<GNC::GCS::AnnotationSettings> pAnnotationSettings(new GNC::GCS::AnnotationSettings());
                        SetDefaultValue(pAnnotationSettings, false);
                }

                virtual void SetDefaultValue( const GNC::GCS::Ptr<GNC::GCS::AnnotationSettings>& pAnnotationSettings, bool createPanels)
                {
                        if (pAnnotationSettings.IsValid()) {
                                for (int i = 0; i < GNC::GCS::AnnotationSettings::TP_NumPos; ++i) {
                                        //create panel
                                        if (createPanels) {
                                                m_panelVector.push_back(new CornerAnnotationPanel(m_pMainPanel));
                                                m_pCornerAnnotationsSizer->Add(m_panelVector[i], 1,wxALL|wxEXPAND,5);
                                        }
                                        //set label
                                        switch (i) {
                                        case GNC::GCS::AnnotationSettings::TP_TopRight:
                                                m_panelVector[i]->m_pLabel->SetLabel(_("Top right corner annotation"));
                                                break;
                                        case GNC::GCS::AnnotationSettings::TP_TopLeft:
                                                m_panelVector[i]->m_pLabel->SetLabel(_("Top left corner annotation"));
                                                break;
                                        case GNC::GCS::AnnotationSettings::TP_BottomLeft:
                                                m_panelVector[i]->m_pLabel->SetLabel(_("Bottom left corner annotation"));
                                                break;
                                        case GNC::GCS::AnnotationSettings::TP_BottomRight:
                                                m_panelVector[i]->m_pLabel->SetLabel(_("Bottom right corner annotation"));
                                                break;
                                        }
                                        //set line content
                                        //reset values...
                                        m_panelVector[i]->m_pLine1->SetValue(wxEmptyString);
                                        m_panelVector[i]->m_pLine2->SetValue(wxEmptyString);
                                        m_panelVector[i]->m_pLine3->SetValue(wxEmptyString);
                                        m_panelVector[i]->m_pLine4->SetValue(wxEmptyString);
                                        wxString strAnnotations = wxString::FromUTF8(pAnnotationSettings->getAnnotation((GNC::GCS::AnnotationSettings::TAnnotationPosition)i).c_str());
                                        strAnnotations.Replace(wxT("${NEWLINE}"), wxT("\n"));
                                        wxStringTokenizer tknz(strAnnotations, wxT("\n"));
                                        int lineCount = 0;
                                        std::ostringstream lastLine;
                                        while(tknz.HasMoreTokens()) {
                                                switch (lineCount++) {
                                                case 0:
                                                        m_panelVector[i]->m_pLine1->SetValue(tknz.GetNextToken());
                                                        break;
                                                case 1:
                                                        m_panelVector[i]->m_pLine2->SetValue(tknz.GetNextToken());
                                                        break;
                                                case 2:
                                                        m_panelVector[i]->m_pLine3->SetValue(tknz.GetNextToken());
                                                        break;
                                                default:
                                                        if (lastLine.tellp() > 0) {
                                                                lastLine << "${NEWLINE}";
                                                        }
                                                        lastLine << std::string(tknz.GetNextToken().ToUTF8());
                                                }
                                        }
                                        m_panelVector[i]->m_pLine4->SetValue(wxString::FromUTF8(lastLine.str().c_str()));
                                }
                        }
                }

                virtual bool Validate()
                {
                        return true;
                }

                virtual void updateAnnotationSettings(const GNC::GCS::Ptr<GNC::GCS::AnnotationSettings>& pAnnotationSettings)
                {
                        for (int i = 0; i < GNC::GCS::AnnotationSettings::TP_NumPos; ++i) {
                                std::ostringstream ostr;
                                if (!m_panelVector[i]->m_pLine1->GetValue().IsEmpty()) {
                                        ostr << m_panelVector[i]->m_pLine1->GetValue().ToUTF8();
                                }
                                if (!m_panelVector[i]->m_pLine1->GetValue().IsEmpty()) {
                                        if (ostr.tellp() > 0) {
                                                ostr << "${NEWLINE}";
                                        }
                                        ostr << m_panelVector[i]->m_pLine2->GetValue().ToUTF8();
                                }
                                if (!m_panelVector[i]->m_pLine1->GetValue().IsEmpty()) {
                                        if (ostr.tellp() > 0) {
                                                ostr << "${NEWLINE}";
                                        }
                                        ostr << m_panelVector[i]->m_pLine3->GetValue().ToUTF8();
                                }
                                if (!m_panelVector[i]->m_pLine1->GetValue().IsEmpty()) {
                                        if (ostr.tellp() > 0) {
                                                ostr << "${NEWLINE}";
                                        }
                                        ostr << m_panelVector[i]->m_pLine4->GetValue().ToUTF8();
                                }
                                pAnnotationSettings->setAnnotation((GNC::GCS::AnnotationSettings::TAnnotationPosition)i, ostr.str());
                        }

                }

                ModalityPanel* Parent;
                std::vector<CornerAnnotationPanel*> m_panelVector;
        };

        class AddWindowLevelDialog: public AddWLDialogBase
        {
        public:
                AddWindowLevelDialog(ModalityPanel* pParent): AddWLDialogBase(pParent),
                        Parent(pParent),
                        EditMode(false)
                {
                }

                AddWindowLevelDialog(ModalityPanel* pParent, const wxString& label, const wxString& window, const wxString& level):
                        AddWLDialogBase(pParent),
                        Parent(pParent),
                        EditMode(true)
                {
                        m_pLabel->SetValue(label);
                        m_pWindow->SetValue(window);
                        m_pLevel->SetValue(level);
                }

                virtual bool Validate()
                {
                        if (m_pLabel->GetValue().empty() || m_pWindow->GetValue().empty() || m_pLevel->GetValue().empty()) {
                                wxMessageBox(_("You must fill in all fields"),_("Info"),wxOK | wxICON_INFORMATION, this);
                                return false;
                        } else {
                                double window, level;
                                if (m_pWindow->GetValue().ToDouble(&window) && m_pLevel->GetValue().ToDouble(&level)) {
                                        if (!EditMode && Parent->m_pDefaultWindowLevel->FindString(m_pLabel->GetValue()) > 0) {
                                                wxMessageBox(_("Window/level label in use"),_("Info"),wxOK | wxICON_INFORMATION, this);
                                                return false;
                                        }
                                        return true;
                                } else {
                                        wxMessageBox(_("Fill in valid numbers in window and level fields."),_("Info"),wxOK | wxICON_INFORMATION, this);
                                        return false;
                                }
                        }
                }

                ModalityPanel* Parent;
                bool EditMode;
        };

        ModalityPanel(wxWindow* pParent, DefaultModalitySettingsConfigurationPanel* pconfiguration, const GNC::GCS::Ptr<GNC::GCS::DefaultModalitySettings>& lo): ModalityPanelBase(pParent),
                modSettings(lo),
                pConfiguration(pconfiguration)
        {
                m_pModel = new wxDataViewListStore();
                m_pListWL->AssociateModel(m_pModel.get());
                m_pListWL->AppendColumn( new wxDataViewColumn( _("Label"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ), COL_LABEL_IDX, COL_LABEL_WIDTH, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
                m_pListWL->AppendColumn( new wxDataViewColumn( _("Window"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ), COL_WINDOW_IDX, COL_WINDOW_WIDTH, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
                m_pListWL->AppendColumn( new wxDataViewColumn( _("Level"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ), COL_LEVEL_IDX, COL_LEVEL_WIDTH, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));

        }

        void InitPanels()
        {

                wxVector<wxVariant> data;

                for (GNC::GCS::DefaultModalitySettings::TListOfWLSettings::const_iterator it = modSettings->getWLSettings().begin(); it != modSettings->getWLSettings().end(); ++it) {
                        const GNC::GCS::DefaultModalitySettings::TListOfWLSettings::value_type& tuple = (*it);
                        data.clear();

                        data.push_back(wxString::FromUTF8(tuple->getLabel().c_str()));
                        data.push_back(wxString::Format(wxT("%lf"), tuple->getWindow()));
                        data.push_back(wxString::Format(wxT("%lf"), tuple->getLevel()));

                        m_pListWL->AppendItem(data);
                }

                RebuildComboWL();

                m_pChoiceInterpolate->Append(_("Nearest neighbour"));
                m_pChoiceInterpolate->Append(_("Linear"));
                m_pChoiceInterpolate->Append(_("Cubic"));

                if (modSettings->getInterpolationMode() == GNC::GCS::DefaultModalitySettings::TIM_NEAREST_NEIGHBOUR) {
                        m_pChoiceInterpolate->Select(0);
                } else if (modSettings->getInterpolationMode() == GNC::GCS::DefaultModalitySettings::TIM_LINEAR) {
                        m_pChoiceInterpolate->Select(1);
                }  else if (modSettings->getInterpolationMode() == GNC::GCS::DefaultModalitySettings::TIM_CUBIC) {
                        m_pChoiceInterpolate->Select(2);
                }
        }

        void RebuildComboWL()
        {
                m_pDefaultWindowLevel->Clear();
                m_pDefaultWindowLevel->Append(wxString::FromUTF8(GNC::GCS::DefaultModalitySettings::getDefaultWindowLevelLabel().c_str()));
                m_pDefaultWindowLevel->Append(wxString::FromUTF8(GNC::GCS::DefaultModalitySettings::getAutocalculateWindowLevelLabel().c_str()));
                m_pDefaultWindowLevel->Select(0);
                if (modSettings->getInitialWindowLevelLabel() == GNC::GCS::DefaultModalitySettings::getAutocalculateWindowLevelLabel()) {
                        m_pDefaultWindowLevel->Select(1);
                }
                for (GNC::GCS::DefaultModalitySettings::TListOfWLSettings::const_iterator it = modSettings->getWLSettings().begin(); it != modSettings->getWLSettings().end(); ++it) {
                        int item = m_pDefaultWindowLevel->Append(wxString::FromUTF8((*it)->getLabel().c_str()));
                        if ((*it)->getLabel() == modSettings->getInitialWindowLevelLabel()) {
                                m_pDefaultWindowLevel->Select (item);
                        }
                }
        }

        const std::string& getModalityName()
        {
                return modSettings->getModality();
        }

        virtual void OnAddWL(wxCommandEvent& /*evt*/)
        {

                AddWindowLevelDialog dlg(this);
                if (dlg.ShowModal() == wxID_OK) {

                        m_pDefaultWindowLevel->Append(dlg.m_pLabel->GetValue());

                        wxVector<wxVariant> data;

                        data.push_back(dlg.m_pLabel->GetValue());
                        data.push_back(dlg.m_pWindow->GetValue());
                        data.push_back(dlg.m_pLevel->GetValue());

                        m_pModel->AppendItem(data);

                        pConfiguration->OnPropiedadCambiada();
                }

        }

        virtual void OnChoiceChanged(wxCommandEvent& )
        {
                pConfiguration->OnPropiedadCambiada();
        }

        virtual void OnEditWL(wxCommandEvent& /*evt*/)
        {

                wxDataViewItem selection = m_pListWL->GetSelection();
                if(selection.IsOk()) {
                        wxVariant oldLabel;
                        wxVariant oldWindow;
                        wxVariant oldLevel;

                        m_pModel->GetValue(oldLabel, selection, COL_LABEL_IDX);
                        m_pModel->GetValue(oldWindow, selection, COL_WINDOW_IDX);
                        m_pModel->GetValue(oldLevel, selection, COL_LEVEL_IDX);

                        AddWindowLevelDialog dlg(this, oldLabel, oldWindow, oldLevel);
                        if (dlg.ShowModal() == wxID_OK) {
                                int comboItem = m_pDefaultWindowLevel->FindString(oldLabel);
                                if (comboItem > 0) {
                                        m_pDefaultWindowLevel->SetString(comboItem, dlg.m_pLabel->GetValue());
                                }
                                m_pModel->SetValue(dlg.m_pLabel->GetValue(), selection, COL_LABEL_IDX);
                                m_pModel->SetValue(dlg.m_pWindow->GetValue(), selection, COL_WINDOW_IDX);
                                m_pModel->SetValue(dlg.m_pLevel->GetValue(), selection, COL_LEVEL_IDX);

                                m_pModel->ItemChanged(selection);

                                pConfiguration->OnPropiedadCambiada();
                        }
                }
        }

        virtual void OnDeleteWL(wxCommandEvent& /*evt*/)
        {

                wxDataViewItem selection = m_pListWL->GetSelection();
                if(selection.IsOk()) {
                        int answer = wxMessageBox(_("Do you confirm the removal of the preset?"), _("Data modified"), wxYES_NO | wxCANCEL, this);
                        if(answer == wxYES) {

                                wxVariant oldLabel;
                                m_pModel->GetValue(oldLabel, selection, COL_LABEL_IDX);

                                int comboItem = m_pDefaultWindowLevel->FindString(oldLabel.GetString());
                                if (comboItem > 0) {
                                        m_pDefaultWindowLevel->Delete(comboItem);
                                        if (m_pDefaultWindowLevel->GetSelection() < 0) {
                                                m_pDefaultWindowLevel->Select(0);
                                        }
                                }
                                bool notFound = true;
                                for (unsigned int i = 0; notFound && i < m_pModel->GetCount(); ++i) {
                                        wxDataViewItem item = m_pModel->GetItem(i);

                                        wxVariant val2;
                                        m_pModel->GetValue(val2, item, COL_LABEL_IDX);
                                        if (oldLabel == val2) {
                                                notFound = false;
                                                m_pModel->DeleteItem(i);
                                        }
                                }

                                pConfiguration->OnPropiedadCambiada();
                        }
                }
        }

        virtual void OnSetupCornerAnnotations(wxCommandEvent& /*evt*/)
        {
                EditCornerAnnotationsDialog dlg(this, modSettings->getAnnotationSettings());
                if (dlg.ShowModal() == wxID_OK) {
                        dlg.updateAnnotationSettings(modSettings->getAnnotationSettings());
                        pConfiguration->OnPropiedadCambiada();
                }
        }

        GNC::GCS::Ptr<GNC::GCS::DefaultModalitySettings> GetSettings()
        {
                modSettings->clearWindowLevels();
                modSettings->setInitialWindowLevelLabel(std::string(m_pDefaultWindowLevel->GetString(m_pDefaultWindowLevel->GetSelection()).ToUTF8()));

                for (unsigned int i = 0; i < m_pModel->GetCount(); ++i) {
                        wxDataViewItem item = m_pModel->GetItem(i);
                        double window;
                        double level;
                        wxVariant labelVal;
                        wxVariant windowVal;
                        wxVariant levelVal;

                        m_pModel->GetValue(labelVal, item, COL_LABEL_IDX);
                        m_pModel->GetValue(windowVal, item, COL_WINDOW_IDX);
                        m_pModel->GetValue(levelVal, item, COL_LEVEL_IDX);

                        if (windowVal.GetString().ToDouble(&window) && levelVal.GetString().ToDouble(&level)) {
                                GNC::GCS::Ptr<GNC::GCS::WindowLevelSetting> wlSettings(new GNC::GCS::WindowLevelSetting(window, level, std::string(labelVal.GetString().ToUTF8())));
                                modSettings->addWindowLevel(wlSettings);
                        }
                }

                modSettings->setInterpolationMode((GNC::GCS::DefaultModalitySettings::TInterpolationMode)m_pChoiceInterpolate->GetSelection());
                return modSettings;
        }

        GNC::GCS::Ptr<GNC::GCS::DefaultModalitySettings> modSettings;
        DefaultModalitySettingsConfigurationPanel* pConfiguration;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DefaultModalitySettingsConfigurationPanel::DefaultModalitySettingsConfigurationPanel(wxWindow* pParent,IDialogoConfiguracion* pDialogo): DefaultModalitySettingsConfigurationPanelBase(pParent), IPasoConfiguracion(pDialogo)
{

        Recargar();

        wxColour barColor = wxColour(200,200,200);
        wxFont fontBar = wxFont( 10, 70, 90, wxFONTWEIGHT_BOLD, false, wxEmptyString );
}

DefaultModalitySettingsConfigurationPanel::~DefaultModalitySettingsConfigurationPanel()
{
}

void DefaultModalitySettingsConfigurationPanel::Recargar()
{

        GNC::GCS::ModalitySettingsController::TMapOfModalitySettings modalitySettings = GNC::GCS::ModalitySettingsController::Instance()->getModalitySettingsMap();

        m_pWLList->DeleteAllItems();
        m_currentModality = NULL;
        for (TModalityMap::iterator it = modalityBrowser.begin(); it != modalityBrowser.end(); ++it) {
                (*it).second->Destroy();
        }
        modalityBrowser.clear();
        //default modality settings...
        wxTreeItemId root = m_pWLList->AddRoot(_("Modalities"));
        for (GNC::GCS::ModalitySettingsController::TMapOfModalitySettings::const_iterator it = modalitySettings.begin(); it != modalitySettings.end(); ++it) {
                if ((*it).second.IsValid()) {
                        ModalityPanel * mp = new ModalityPanel(m_pModalityPanel, this, (*it).second);
                        wxTreeItemId id = m_pWLList->AppendItem(root, wxString::FromUTF8(mp->getModalityName().c_str()));
                        modalityBrowser[id.m_pItem] = mp;
                        mp->InitPanels();
                        mp->Hide();
                        m_pSizerModalities->Add(mp, 1, wxEXPAND|wxALL, 5);
                        m_pModalityPanel->Layout();
                }
        }
        wxTreeItemIdValue cookie;
        m_pWLList->SelectItem(m_pWLList->GetFirstChild(root, cookie));
        LoadCurrent();
        //*/
        Layout();
}

//region "Metodos de IPasoConfiguracion"
wxWindow* DefaultModalitySettingsConfigurationPanel::GetPanel()
{
        return this;
}

std::string DefaultModalitySettingsConfigurationPanel::GetTitle()
{
        return _Std("Modality settings");
}

std::string DefaultModalitySettingsConfigurationPanel::GetCabecera()
{
        return _Std("Default modality settings");
}

bool DefaultModalitySettingsConfigurationPanel::Validar()
{
        bool ok = true;
        return ok;
}

bool DefaultModalitySettingsConfigurationPanel::Guardar()
{
        //get hanging protocols...

        GNC::GCS::ModalitySettingsController::Instance()->clearModalitySettings();

        for (TModalityMap::iterator it = modalityBrowser.begin(); it != modalityBrowser.end(); ++it) {
                GNC::GCS::ModalitySettingsController::Instance()->addModalitySettings((*it).second->GetSettings());
        }
        GNC::GCS::ModalitySettingsController::Instance()->StoreModalitySettingsConfiguration();
        return true;
}
//endregion


void DefaultModalitySettingsConfigurationPanel::OnTreeSelChanged(wxTreeEvent &)
{
        LoadCurrent();
}

void DefaultModalitySettingsConfigurationPanel::LoadCurrent()
{
        wxTreeItemId id = m_pWLList->GetSelection();
        if (id.IsOk()) {
                if(modalityBrowser.find(id.m_pItem) != modalityBrowser.end()) {
                        ModalityPanel* pCurrent = modalityBrowser[id.m_pItem];
                        if( pCurrent!=NULL && pCurrent!=m_currentModality) {
                                m_pTitledPanelModalities->Freeze();
                                if(m_currentModality!=NULL) {
                                        m_currentModality->Hide();
                                }
                                m_currentModality = pCurrent;
                                wxString modName = wxString::Format(_("%s settings"),wxString::FromUTF8(pCurrent->getModalityName().c_str()).c_str());
                                m_pTitledPanelModalities->SetToolTip(modName);
                                m_currentModality->Show();
                                m_pModalityPanel->SetScrollbars(20, 20, 50, 50, 0, 0, false);
                                m_pModalityPanel->AdjustScrollbars();
                                m_pTitledPanelModalities->Layout();
                                m_pTitledPanelModalities->Thaw();
                        }
                }
        }
}

void DefaultModalitySettingsConfigurationPanel::OnPropiedadCambiada()
{
        IPasoConfiguracion::OnPropiedadCambiada();
}

void DefaultModalitySettingsConfigurationPanel::OnSize(wxSizeEvent & event)
{
        event.Skip(true);
}
}
}
