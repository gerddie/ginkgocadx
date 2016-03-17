///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __PANELESCONFIGURACIONGINKGOBASE_H__
#define __PANELESCONFIGURACIONGINKGOBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class BodyPanel;
class FooterPanel;
class GinkgoBoxPanel;
class HeaderPanel;
class PanelScrollable;
class SubTitledPanel;
class SubTitledPanelSeparatorV;
class SubTitledPanelSinBorder;
class TitledPanel;
class TitledPanelSinBorder;

#include "wx/ginkgostyle/ginkgostyle.h"
#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/dataview.h>
#include <wx/notebook.h>
#include <wx/scrolwin.h>
#include <wx/propgrid/propgrid.h>
#include <wx/listctrl.h>
#include <wx/valtext.h>
#include <wx/aui/auibook.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class DialogoConfiguracionBase
///////////////////////////////////////////////////////////////////////////////
class DialogoConfiguracionBase : public wxDialog
{
private:

protected:
        HeaderPanel* m_pHeader;
        BodyPanel* m_pImagePanel;
        wxSplitterWindow* m_splitter2;
        wxPanel* m_panel1;
        wxTreeCtrl* m_pTreeNavegacion;
        TitledPanelSinBorder* m_pTitledPanelConfig;
        PanelScrollable* m_pPanelConfig;
        wxBoxSizer* m_pSizerPaneles;
        FooterPanel* m_panel3;
        wxButton* m_button12;
        wxButton* m_button13;
        wxButton* m_button8;
        wxButton* m_button9;
        wxButton* m_pBAplicar;

        // Virtual event handlers, overide them in your derived class
        virtual void OnCambioNodoNavegacion( wxTreeEvent& event )
        {
                event.Skip();
        }
        virtual void OnExportarClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnImportarClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnAceptarClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnCancelarClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnAplicarClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        DialogoConfiguracionBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 860,565 ), long style = wxDEFAULT_DIALOG_STYLE );
        ~DialogoConfiguracionBase();

        void m_splitter2OnIdle( wxIdleEvent& )
        {
                m_splitter2->SetSashPosition( 245 );
                m_splitter2->Disconnect( wxEVT_IDLE, wxIdleEventHandler( DialogoConfiguracionBase::m_splitter2OnIdle ), NULL, this );
        }

};

///////////////////////////////////////////////////////////////////////////////
/// Class PanelConfiguracionEstacionBase
///////////////////////////////////////////////////////////////////////////////
class PanelConfiguracionEstacionBase : public wxPanel
{
private:

protected:
        SubTitledPanel* m_panelPhysician;
        wxStaticText* m_staticText1;
        wxTextCtrl* m_pMedicoNombre;
        wxStaticText* m_staticText11;
        wxTextCtrl* m_pMedicoId;
        SubTitledPanelSeparatorV* m_pSep11;
        SubTitledPanel* m_panelServicio;
        wxStaticText* m_staticText39;
        wxTextCtrl* m_pServicioNombre;
        wxStaticText* m_staticText12;
        wxTextCtrl* m_pServicioId;
        SubTitledPanelSeparatorV* m_pSep1;
        SubTitledPanel* m_pPanelInstitucion;
        wxStaticText* m_staticText13;
        wxTextCtrl* m_pCentroNombre;
        wxStaticText* m_staticText111;
        wxTextCtrl* m_pCentroId;
        SubTitledPanelSeparatorV* m_pSep12;
        SubTitledPanel* m_pPanelInstitucion1;
        wxStaticText* m_staticText131;
        wxComboBox* m_pComboCharset;

        // Virtual event handlers, overide them in your derived class
        virtual void OnTextoCambiado( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        PanelConfiguracionEstacionBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
        ~PanelConfiguracionEstacionBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class PanelConfiguracionGeneralBase
///////////////////////////////////////////////////////////////////////////////
class PanelConfiguracionGeneralBase : public wxPanel
{
private:

protected:
        SubTitledPanel* m_pLanguage;
        wxStaticText* m_staticText34;
        wxComboBox* m_pComboLanguage;
        SubTitledPanelSeparatorV* m_pSep3;
        SubTitledPanel* m_pUpdate;
        wxCheckBox* m_pCheckForUpdates;
        wxCheckBox* m_pCheckAnonUsageStats;
        SubTitledPanelSeparatorV* m_pSep31;
        SubTitledPanel* m_pProxy;
        wxCheckBox* m_pEnableProxy;
        wxButton* m_pProxySettingsButton;

        // Virtual event handlers, overide them in your derived class
        virtual void OnIdiomaCambiado( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnCheckForUpdates( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnCheckEnableProxySettings( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnProxySettings( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        PanelConfiguracionGeneralBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
        ~PanelConfiguracionGeneralBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class ProxySettingsDialogBase
///////////////////////////////////////////////////////////////////////////////
class ProxySettingsDialogBase : public wxDialog
{
private:

protected:
        HeaderPanel* m_pHeader;
        BodyPanel* m_pBody;
        SubTitledPanel* m_panel64;
        wxStaticText* m_pstaticText1;
        wxTextCtrl* m_pHost;
        wxStaticText* m_pStaticText22;
        wxTextCtrl* m_pPort;
        SubTitledPanelSeparatorV* m_panel641;
        SubTitledPanel* m_panel642;
        wxCheckBox* m_pCheckAuthentication;
        wxStaticText* m_pstaticText12;
        wxTextCtrl* m_pUsername;
        wxStaticText* Port2;
        wxTextCtrl* m_pPassword;
        SubTitledPanelSeparatorV* m_panel6411;
        SubTitledPanel* m_panel6421;
        wxStaticText* m_staticText115;
        wxTextCtrl* m_pExceptions;
        wxStaticText* m_staticText116;
        FooterPanel* m_pFooter;
        wxButton* m_button37;
        wxButton* m_button38;

        // Virtual event handlers, overide them in your derived class
        virtual void OnCheckAuthentication( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnOkClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnCancelClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        ProxySettingsDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Proxy Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,570 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
        ~ProxySettingsDialogBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class LocalDatabaseConfigurationPanelBase
///////////////////////////////////////////////////////////////////////////////
class LocalDatabaseConfigurationPanelBase : public wxPanel
{
private:

protected:
        SubTitledPanel* m_pPanelDicomDir;
        wxStaticText* m_staticTextExplicacion;
        wxStaticText* m_staticText32;
        wxTextCtrl* m_pDICOMDir;
        wxButton* m_buttonSeleccionarDICOMDir;
        wxStaticText* m_pstaticTextAdvertencia;
        wxCheckBox* m_pCheckDeleteOlder;
        wxChoice* m_pChoiceDeleteOlder;
        wxCheckBox* m_pCheckHDFreeSpace;
        wxSpinCtrl* m_pSpinHDSpace;
        wxStaticText* m_staticText65;
        wxCheckBox* m_pCheckDicomDirSize;
        wxSpinCtrl* m_pSpinDicomDirSize;
        wxStaticText* m_staticText651;
        wxStaticText* m_staticText68;
        wxButton* m_button66;

        // Virtual event handlers, overide them in your derived class
        virtual void OnSize( wxSizeEvent& event )
        {
                event.Skip();
        }
        virtual void OnTextoCambiado( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnSeleccionarDirectorio( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnCheckOlderStudies( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnOptionModified( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnCheckHDSpace( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnSpinCtrl( wxSpinEvent& event )
        {
                event.Skip();
        }
        virtual void OnCheckDicomDirSize( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnResetToDefault( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        LocalDatabaseConfigurationPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
        ~LocalDatabaseConfigurationPanelBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class PanelConfiguracionPACSBase
///////////////////////////////////////////////////////////////////////////////
class PanelConfiguracionPACSBase : public wxPanel
{
private:

protected:
        SubTitledPanel* m_pPACSLocal;
        wxStaticText* m_staticTextExplicacion;
        wxStaticText* m_staticText1;
        wxTextCtrl* m_pAETLocal;
        wxStaticText* m_pAETReal;
        wxStaticBitmap* m_pHelpBitmap;
        wxStaticText* m_staticText11;
        wxTextCtrl* m_pPuertoLocal;
        wxButton* m_pBAdvanced;
        SubTitledPanelSeparatorV* m_pSep;
        SubTitledPanel* m_pPACSServers;
        wxDataViewListCtrl* m_pListPacs;
        wxButton* m_button3;
        wxButton* m_pEditButton;
        wxButton* m_pDelButton;
        wxButton* m_pDefaultButton;

        // Virtual event handlers, overide them in your derived class
        virtual void OnSize( wxSizeEvent& event )
        {
                event.Skip();
        }
        virtual void OnTextoCambiado( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnHelpClick( wxMouseEvent& event )
        {
                event.Skip();
        }
        virtual void OnAdvancedLocal( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnNuevoClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnEditarClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnEliminarClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnSetDefaultClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        PanelConfiguracionPACSBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
        ~PanelConfiguracionPACSBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogoServidorPACSBase
///////////////////////////////////////////////////////////////////////////////
class DialogoServidorPACSBase : public wxDialog
{
private:

protected:
        HeaderPanel* m_pHeader;
        wxNotebook* m_notebook1;
        BodyPanel* m_panel6;
        SubTitledPanel* m_panel64;
        wxStaticText* m_pstaticText1;
        wxTextCtrl* m_pId;
        wxStaticText* m_pstaticText2;
        wxTextCtrl* m_pAET;
        wxStaticText* m_pstaticText3;
        wxTextCtrl* m_pHost;
        wxStaticText* m_pstaticText4;
        wxTextCtrl* m_pPuerto;
        wxStaticText* m_staticText33;
        wxSpinCtrl* m_pPDU;
        wxStaticText* m_staticText48;
        wxChoice* m_pRetrieveMethod;
        wxStaticText* m_staticText59;
        wxTextCtrl* m_pWadoURL;
        wxStaticText* m_staticText60;
        wxChoice* m_pRetrieveMode;
        wxStaticText* m_staticText57;
        wxCheckBox* m_pReuseConnection;
        BodyPanel* m_pWorkListPanel;
        SubTitledPanel* m_panel83;
        wxStaticText* m_staticText611;
        wxCheckBox* m_pCheckDICOMMPPS;
        wxCheckBox* m_pCheckHL7;
        wxPanel* m_pPanelHL7;
        wxStaticBoxSizer* m_pSizerHL7;
        wxCheckBox* m_pCheckAbort;
        wxTextCtrl* m_pTextAbort;
        wxCheckBox* m_pCheckCompleted;
        wxTextCtrl* m_pTextCompleted;
        wxCheckBox* m_pCheckInProgress;
        wxTextCtrl* m_pTextProgress;
        wxButton* m_button62;
        BodyPanel* m_pPanelSeguridad;
        SubTitledPanel* m_pSSLTLS;
        wxCheckBox* m_pVerifyServer;
        wxCheckBox* m_pUseSSL;
        wxStaticText* m_staticText55;
        wxButton* m_pBCert;
        SubTitledPanelSeparatorV* m_panelSep;
        SubTitledPanel* m_panelUserPass;
        wxStaticText* m_staticText61;
        wxTextCtrl* m_pUser;
        wxStaticText* m_staticText62;
        wxTextCtrl* m_pPassword;
        SubTitledPanelSeparatorV* m_panelSep1;
        SubTitledPanel* m_pPanelDefaultCharset;
        wxStaticText* m_staticText131;
        wxComboBox* m_pComboCharset;
        FooterPanel* m_pFooter;
        wxButton* m_button20;
        wxButton* m_button37;
        wxButton* m_button38;

        // Virtual event handlers, overide them in your derived class
        virtual void OnRetrieveMethodChoice( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnChecWL( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnMessageDefaultValues( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnAdvancedConfiguration( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnTextoCambiado( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnProbarClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnOkClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnCancelClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        DialogoServidorPACSBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("DICOM Node"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 450,550 ), long style = wxDEFAULT_DIALOG_STYLE );
        ~DialogoServidorPACSBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogoLocalPACSBase
///////////////////////////////////////////////////////////////////////////////
class DialogoLocalPACSBase : public wxDialog
{
private:

protected:
        BodyPanel* m_panel51;
        TitledPanel* m_panel67;
        wxCheckBox* m_pVerifyServer;
        wxCheckBox* m_pUseTLS;
        wxStaticText* m_staticText55;
        wxButton* m_pBCert;
        FooterPanel* m_pFooter;
        wxButton* m_button8;
        wxButton* m_button9;

        // Virtual event handlers, overide them in your derived class
        virtual void OnEditCertificate( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnAceptarClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        DialogoLocalPACSBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Advanced Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 346,300 ), long style = wxDEFAULT_DIALOG_STYLE );
        ~DialogoLocalPACSBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class PanelConfiguracionHCEBase
///////////////////////////////////////////////////////////////////////////////
class PanelConfiguracionHCEBase : public wxPanel
{
private:

protected:
        SubTitledPanel* m_pHCE;
        wxStaticText* m_pstIdOrganizacionReceptora;
        wxTextCtrl* m_pIdOrganizacionReceptora;
        wxStaticText* m_pstIdSistemaPeticionario;
        wxTextCtrl* m_pIdSistemaPeticionario;
        wxStaticText* m_pstIdAplicacionRellenadora;
        wxTextCtrl* m_pIdAplicacionRellenadora;
        wxStaticText* m_pstIdAplicacionPACS;
        wxTextCtrl* m_pIdGinkgoAplication;
        SubTitledPanelSeparatorV* m_pSep;
        SubTitledPanel* m_pIntegrationEngine;
        wxStaticText* m_staticText121;
        wxTextCtrl* m_pHostnameMI;
        wxStaticText* m_staticText60;
        wxTextCtrl* m_pPuertoMI;
        wxStaticText* m_staticText38;
        wxChoice* m_pProtocolo;
        wxStaticText* m_staticText39;
        wxChoice* m_pFormato;
        wxButton* m_button15;

        // Virtual event handlers, overide them in your derived class
        virtual void OnSize( wxSizeEvent& event )
        {
                event.Skip();
        }
        virtual void OnTextoCambiado( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnTestClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        PanelConfiguracionHCEBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 556,438 ), long style = wxTAB_TRAVERSAL );
        ~PanelConfiguracionHCEBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class PanelConfiguracionExtensionesBase
///////////////////////////////////////////////////////////////////////////////
class PanelConfiguracionExtensionesBase : public wxPanel
{
private:

protected:
        SubTitledPanel* m_pExtensions;
        wxScrolledWindow* m_pScrollPanelExtensions;

        // Virtual event handlers, overide them in your derived class
        virtual void OnEraseBackground( wxEraseEvent& event )
        {
                event.Skip();
        }


public:

        PanelConfiguracionExtensionesBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
        ~PanelConfiguracionExtensionesBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class ExtensionPanelBase
///////////////////////////////////////////////////////////////////////////////
class ExtensionPanelBase : public GinkgoBoxPanel
{
private:

protected:
        wxStaticBitmap* m_pThumbnail;
        wxStaticText* m_pLabelTitle;

public:

        ExtensionPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
        ~ExtensionPanelBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogoEditarCertificadoBase
///////////////////////////////////////////////////////////////////////////////
class DialogoEditarCertificadoBase : public wxDialog
{
private:

protected:
        BodyPanel* m_pBody;
        SubTitledPanel* m_pPanelDefault;
        wxCheckBox* m_pCheckDefault;
        SubTitledPanelSeparatorV* m_pSepDefault;
        SubTitledPanel* m_pPanelPublic;
        wxStaticText* m_pLabel;
        wxTextCtrl* m_pTextoCertificado;
        wxButton* m_button62;
        SubTitledPanelSeparatorV* m_panel60;
        SubTitledPanel* m_pPanelPrivate;
        wxStaticText* m_pLabel11;
        wxTextCtrl* m_pTextoPrivateKey;
        wxButton* m_button621;
        FooterPanel* m_pFooter;
        wxButton* m_button48;
        wxButton* m_button8;
        wxButton* m_button9;

        // Virtual event handlers, overide them in your derived class
        virtual void OnCheckDefault( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnLoadPublicClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnLoadPrivateClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnResetToDefault( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnAceptarClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        DialogoEditarCertificadoBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Certificate"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 450,550 ), long style = wxDEFAULT_DIALOG_STYLE );
        ~DialogoEditarCertificadoBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class ProfileDialogBase
///////////////////////////////////////////////////////////////////////////////
class ProfileDialogBase : public wxDialog
{
private:

protected:
        BodyPanel* m_pBasePanel;
        TitledPanel* m_pTitledPanel;
        wxStaticText* m_staticText47;
        wxTextCtrl* m_pName;
        wxPropertyGrid* m_pGridPermisos;
        FooterPanel* m_pFooterPanel;
        wxButton* m_button28;
        wxButton* m_button29;

        // Virtual event handlers, overide them in your derived class
        virtual void OnOkClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnCancelClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        ProfileDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Edit profile"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 770,540 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
        ~ProfileDialogBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class PanelConfiguracionPermisosBase
///////////////////////////////////////////////////////////////////////////////
class PanelConfiguracionPermisosBase : public wxPanel
{
private:

protected:
        SubTitledPanel* m_pBasePanel;
        wxDataViewListCtrl* m_pListProfiles;
        wxButton* m_button3;
        wxButton* m_pBEdit;
        wxButton* m_pBDelete;
        wxButton* m_pBActive;

        // Virtual event handlers, overide them in your derived class
        virtual void OnSize( wxSizeEvent& event )
        {
                event.Skip();
        }
        virtual void OnNuevoClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnEditarClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnEliminarClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnSetActive( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        PanelConfiguracionPermisosBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 501,160 ), long style = wxTAB_TRAVERSAL );
        ~PanelConfiguracionPermisosBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class PanelConfiguracionUbicacionesBase
///////////////////////////////////////////////////////////////////////////////
class PanelConfiguracionUbicacionesBase : public wxPanel
{
private:

protected:
        SubTitledPanel* m_pUbicaciones;
        wxStaticText* m_staticTextExplicacion;
        wxDataViewListCtrl* m_pListUbicaciones;
        wxButton* m_pBotonNuevo;
        wxButton* m_pBotonEdit;
        wxButton* m_pBotonDelete;

        // Virtual event handlers, overide them in your derived class
        virtual void OnSize( wxSizeEvent& event )
        {
                event.Skip();
        }
        virtual void OnNuevoClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnEditarClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnEliminarClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        PanelConfiguracionUbicacionesBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
        ~PanelConfiguracionUbicacionesBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogoAddLocationBase
///////////////////////////////////////////////////////////////////////////////
class DialogoAddLocationBase : public wxDialog
{
private:

protected:
        BodyPanel* m_pBody;
        TitledPanel* m_pPanel;
        wxStaticText* m_pSTTitle;
        wxTextCtrl* m_pTETitle;
        wxStaticText* m_pSTLocation;
        wxTextCtrl* m_pTELocation;
        wxButton* m_pOpenDir;
        wxStaticText* m_pSTDesc;
        wxTextCtrl* m_pTEDesc;
        wxCheckBox* m_pMonitorize;
        wxCheckBox* m_pCleanAfter;
        wxCheckBox* m_pCleanBefore;
        FooterPanel* m_pFooter;
        wxButton* m_pAccept;
        wxButton* m_pCancel;

        // Virtual event handlers, overide them in your derived class
        virtual void OnOpenDirClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnAceptarClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        DialogoAddLocationBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Add/Edit Location"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 450,450 ), long style = wxDEFAULT_DIALOG_STYLE );
        ~DialogoAddLocationBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class PanelConfiguracionSeguridadBase
///////////////////////////////////////////////////////////////////////////////
class PanelConfiguracionSeguridadBase : public wxPanel
{
private:

protected:
        SubTitledPanel* m_pSecurity;
        wxStaticText* m_staticText47;
        wxChoice* m_pSecType;
        SubTitledPanelSeparatorV* m_pSep;
        SubTitledPanel* m_pPanelUsuarios;
        wxListCtrl* m_pListaUsuarios;
        wxButton* m_button3;
        wxButton* m_pBCambiarPassword;
        wxButton* m_pBDelete;
        SubTitledPanel* m_pPanelPassword;
        wxStaticText* m_pLabelPassword;
        wxButton* m_pButtonSetPassword;

        // Virtual event handlers, overide them in your derived class
        virtual void OnEraseBackground( wxEraseEvent& event )
        {
                event.Skip();
        }
        virtual void OnSecTypeChoice( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnListaUsuariosItemDeSelected( wxListEvent& event )
        {
                event.Skip();
        }
        virtual void OnListaUsuariosItemSelected( wxListEvent& event )
        {
                event.Skip();
        }
        virtual void OnNuevoClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnCambiarPasswordClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnEliminarClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnSetMainPassword( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        PanelConfiguracionSeguridadBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
        ~PanelConfiguracionSeguridadBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class UserDialogBase
///////////////////////////////////////////////////////////////////////////////
class UserDialogBase : public wxDialog
{
private:

protected:
        HeaderPanel* m_pHeader;
        BodyPanel* m_pBody;
        TitledPanel* m_pFormPanel;
        wxStaticText* m_pLabelUser;
        wxTextCtrl* m_pTCUsuario;
        wxStaticText* m_pstat2;
        wxTextCtrl* m_pTCPassword0;
        wxStaticText* m_pstat21;
        wxTextCtrl* m_pTCPassword1;
        FooterPanel* m_panel54;
        wxButton* m_button42;
        wxButton* m_button41;

        // Virtual event handlers, overide them in your derived class
        virtual void OnOkClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        UserDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Edit User"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 423,292 ), long style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER|wxSTAY_ON_TOP );
        ~UserDialogBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class PanelConfigurationAnonymizationBase
///////////////////////////////////////////////////////////////////////////////
class PanelConfigurationAnonymizationBase : public wxPanel
{
private:

protected:
        SubTitledPanel* m_pAnonimization;
        wxStaticText* m_staticText52;
        wxTextCtrl* m_pTextDefaultValue;

public:

        PanelConfigurationAnonymizationBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
        ~PanelConfigurationAnonymizationBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class AddWLDialogBase
///////////////////////////////////////////////////////////////////////////////
class AddWLDialogBase : public wxDialog
{
private:

protected:
        BodyPanel* m_pBody;
        TitledPanel* m_pPanelMessage;
        wxStaticText* m_staticText79;
        wxStaticText* m_staticText59;
        wxStaticText* m_staticText60;
        FooterPanel* m_pFooter;
        wxButton* m_pOkButton;
        wxButton* m_button5;

public:
        wxTextCtrl* m_pLabel;
        wxTextCtrl* m_pWindow;
        wxTextCtrl* m_pLevel;
        wxString windowValidator;
        wxString levelValidator;

        AddWLDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Add window/level"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
        ~AddWLDialogBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class EditCornerAnnotationsDialogBase
///////////////////////////////////////////////////////////////////////////////
class EditCornerAnnotationsDialogBase : public wxDialog
{
private:

protected:
        BodyPanel* m_pBody;
        TitledPanel* m_pMainPanel;
        wxStaticText* m_staticText84;
        wxStaticText* m_staticText85;
        wxStaticBitmap* m_pHelpBitmap;
        wxGridSizer* m_pCornerAnnotationsSizer;
        FooterPanel* m_pFooter;
        wxButton* m_button70;
        wxButton* m_pOkButton;
        wxButton* m_button5;

        // Virtual event handlers, overide them in your derived class
        virtual void OnSetDefaultValues( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        EditCornerAnnotationsDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Edit corner annotations"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 850,650 ), long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
        ~EditCornerAnnotationsDialogBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class ModalityPanelBase
///////////////////////////////////////////////////////////////////////////////
class ModalityPanelBase : public wxPanel
{
private:

protected:
        wxDataViewListCtrl*  m_pListWL;
        wxButton* m_button63;
        wxButton* m_button64;
        wxButton* m_button65;
        wxStaticText* m_staticText68;
        wxStaticText* m_staticText71;
        wxButton* m_button67;
        wxStaticText* m_staticText69;
        wxChoice* m_pChoiceInterpolate;

        // Virtual event handlers, overide them in your derived class
        virtual void OnAddWL( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnEditWL( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnDeleteWL( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnChoiceChanged( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnSetupCornerAnnotations( wxCommandEvent& event )
        {
                event.Skip();
        }


public:
        wxChoice* m_pDefaultWindowLevel;

        ModalityPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
        ~ModalityPanelBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class EditHangingProtocolDialogBase
///////////////////////////////////////////////////////////////////////////////
class EditHangingProtocolDialogBase : public wxDialog
{
private:

protected:
        HeaderPanel* m_pHeader;
        wxNotebook* m_pMainNoteBook;
        wxPanel* m_pGeneralPanel;
        wxStaticText* m_staticText87;
        wxTextCtrl* m_textName;
        wxStaticText* m_staticText91;
        wxTextCtrl* m_textOpeningMode;
        wxStaticBoxSizer* m_pSizerStudyCriteria;
        wxPanel* m_pWorkspacePanel;
        wxAuiNotebook* m_pNoteBookWindows;
        wxPanel* m_pAdddPanel;
        FooterPanel* m_pFooter;
        wxButton* m_button37;
        wxButton* m_button38;

        // Virtual event handlers, overide them in your derived class
        virtual void OnPageChanging( wxAuiNotebookEvent& event )
        {
                event.Skip();
        }
        virtual void OnPageClose( wxAuiNotebookEvent& event )
        {
                event.Skip();
        }
        virtual void OnOkClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnCancelClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        EditHangingProtocolDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Hanging protocol"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 744,641 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER );
        ~EditHangingProtocolDialogBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class HangingProtocolConfigurationPanelBase
///////////////////////////////////////////////////////////////////////////////
class HangingProtocolConfigurationPanelBase : public wxPanel
{
private:

protected:
        SubTitledPanel* m_pProtocols;
        wxDataViewListCtrl* m_pListHanging;
        wxButton* m_button3;
        wxButton* m_pBEdit;
        wxButton* m_pBDelete;
        wxButton* m_pBActive;

        // Virtual event handlers, overide them in your derived class
        virtual void OnNuevoClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnEditarClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnEliminarClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnSetActive( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        HangingProtocolConfigurationPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
        ~HangingProtocolConfigurationPanelBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class DefaultModalitySettingsConfigurationPanelBase
///////////////////////////////////////////////////////////////////////////////
class DefaultModalitySettingsConfigurationPanelBase : public wxPanel
{
private:

protected:
        wxSplitterWindow* m_splitter4;
        wxPanel* m_panel90;
        wxTreeCtrl* m_pWLList;
        SubTitledPanelSinBorder* m_pTitledPanelModalities;
        PanelScrollable* m_pModalityPanel;
        wxBoxSizer* m_pSizerModalities;

        // Virtual event handlers, overide them in your derived class
        virtual void OnTreeSelChanged( wxTreeEvent& event )
        {
                event.Skip();
        }


public:

        DefaultModalitySettingsConfigurationPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
        ~DefaultModalitySettingsConfigurationPanelBase();

        void m_splitter4OnIdle( wxIdleEvent& )
        {
                m_splitter4->SetSashPosition( 150 );
                m_splitter4->Disconnect( wxEVT_IDLE, wxIdleEventHandler( DefaultModalitySettingsConfigurationPanelBase::m_splitter4OnIdle ), NULL, this );
        }

};

///////////////////////////////////////////////////////////////////////////////
/// Class CornerAnnotationPanel
///////////////////////////////////////////////////////////////////////////////
class CornerAnnotationPanel : public wxPanel
{
private:

protected:

public:
        wxStaticText* m_pLabel;
        wxTextCtrl* m_pLine1;
        wxTextCtrl* m_pLine2;
        wxTextCtrl* m_pLine3;
        wxTextCtrl* m_pLine4;

        CornerAnnotationPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
        ~CornerAnnotationPanel();

};

///////////////////////////////////////////////////////////////////////////////
/// Class StudyWindowBase
///////////////////////////////////////////////////////////////////////////////
class StudyWindowBase : public wxPanel
{
private:

protected:
        SubTitledPanel* m_pConfigWindow;
        wxStaticText* m_staticText72;
        wxButton* m_button62;
        wxButton* m_pSetupPositionButton;
        SubTitledPanelSeparatorV* m_panel64111;
        SubTitledPanel* m_pLayoutPreview;
        wxBoxSizer* m_pSizerLayoutTools;
        wxStaticText* m_staticText56;
        wxSpinCtrl* m_pSpinRows;
        wxStaticText* m_staticText57;
        wxSpinCtrl* m_pSpinCols;
        wxBoxSizer* m_pPreviewSizer;
        wxBoxSizer* m_pSizerSeriesCriteria;
        wxStaticText* m_staticText65;
        SubTitledPanelSeparatorV* m_panel6411;

        // Virtual event handlers, overide them in your derived class
        virtual void OnSetDefaultWindowPosition( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnSetupWindowPositionClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnSpinRows( wxSpinEvent& event )
        {
                event.Skip();
        }
        virtual void OnSpinCols( wxSpinEvent& event )
        {
                event.Skip();
        }


public:

        StudyWindowBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
        ~StudyWindowBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class EditCriteriaPanelBase
///////////////////////////////////////////////////////////////////////////////
class EditCriteriaPanelBase : public wxPanel
{
private:

protected:
        wxDataViewListCtrl* m_pListCriteria;
        wxButton* m_button3;
        wxButton* m_pBDelete;

        // Virtual event handlers, overide them in your derived class
        virtual void OnAddClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnDeleteClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:
        wxBoxSizer* m_pGeneralSizer;
        wxBoxSizer* m_pButtonSizer;

        EditCriteriaPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,200 ), long style = wxTAB_TRAVERSAL );
        ~EditCriteriaPanelBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class SelectOpeningModeBase
///////////////////////////////////////////////////////////////////////////////
class SelectOpeningModeBase : public wxDialog
{
private:

protected:
        BodyPanel* m_pBody;
        TitledPanel* m_pPanelMessage;
        wxStaticText* m_pLabel;
        wxChoice* m_pOpeningMode;
        FooterPanel* m_pFooter;
        wxButton* m_pOkButton;
        wxButton* m_button5;

public:

        SelectOpeningModeBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Select opening mode"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
        ~SelectOpeningModeBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class AddTagDialogBase
///////////////////////////////////////////////////////////////////////////////
class AddTagDialogBase : public wxDialog
{
private:

protected:
        BodyPanel* m_pBody;
        TitledPanel* m_pPanelMessage;
        wxStaticText* m_staticText59;
        wxStaticText* m_staticText60;
        wxChoice* m_pChoiceTag;
        wxTextCtrl* m_pTextValue;
        FooterPanel* m_pFooter;
        wxButton* m_pOkButton;
        wxButton* m_button5;

public:

        AddTagDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Add tag"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
        ~AddTagDialogBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class SetupWindowPositionBase
///////////////////////////////////////////////////////////////////////////////
class SetupWindowPositionBase : public wxDialog
{
private:

protected:
        wxPanel* m_panel97;
        wxStaticText* m_staticText73;
        wxButton* m_button64;
        wxButton* m_button65;

public:

        SetupWindowPositionBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Setup window position"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,400 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER );
        ~SetupWindowPositionBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class SmartRetrieveConfigurationPanelBase
///////////////////////////////////////////////////////////////////////////////
class SmartRetrieveConfigurationPanelBase : public wxPanel
{
private:

protected:
        wxCheckBox* m_pCheckEnable;
        SubTitledPanel* m_pPanelQueries;
        wxDataViewListCtrl* m_pListQueries;

        // Virtual event handlers, overide them in your derived class
        virtual void OnCheckEnableBackgroundQueries( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        SmartRetrieveConfigurationPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
        ~SmartRetrieveConfigurationPanelBase();

};

#endif //__PANELESCONFIGURACIONGINKGOBASE_H__
