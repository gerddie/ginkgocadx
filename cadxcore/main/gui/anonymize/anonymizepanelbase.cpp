///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "anonymizepanelbase.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNC::GUI;

AnonymizePanelBase::AnonymizePanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
        wxBoxSizer* bSizer61;
        bSizer61 = new wxBoxSizer( wxVERTICAL );

        m_pPanelCampos = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        m_pPanelCampos->SetToolTip( _("Common tags") );

        wxBoxSizer* bSizer10;
        bSizer10 = new wxBoxSizer( wxVERTICAL );

        wxGridSizer* gSizer1;
        gSizer1 = new wxGridSizer( 0, 2, 0, 0 );

        m_pCheckNombrePaciente = new wxCheckBox( m_pPanelCampos, wxID_ANY, _("Patient's name and surname"), wxDefaultPosition, wxDefaultSize, 0 );
        gSizer1->Add( m_pCheckNombrePaciente, 0, wxALL, 5 );

        m_pCheckIdPaciente = new wxCheckBox( m_pPanelCampos, wxID_ANY, _("Patient Id"), wxDefaultPosition, wxDefaultSize, 0 );
        gSizer1->Add( m_pCheckIdPaciente, 0, wxALL, 5 );

        m_pCheckInstituciones = new wxCheckBox( m_pPanelCampos, wxID_ANY, _("Institutions"), wxDefaultPosition, wxDefaultSize, 0 );
        gSizer1->Add( m_pCheckInstituciones, 0, wxALL, 5 );

        m_pReferringPhysician = new wxCheckBox( m_pPanelCampos, wxID_ANY, _("Referring physician"), wxDefaultPosition, wxDefaultSize, 0 );
        gSizer1->Add( m_pReferringPhysician, 0, wxALL, 5 );

        m_pCheckComentarios = new wxCheckBox( m_pPanelCampos, wxID_ANY, _("Comments"), wxDefaultPosition, wxDefaultSize, 0 );
        gSizer1->Add( m_pCheckComentarios, 0, wxALL, 5 );

        m_pCheckAtributosPrivadosGinkgo = new wxCheckBox( m_pPanelCampos, wxID_ANY, _("Ginkgo CADx tags  (Evaluation Maps...)"), wxDefaultPosition, wxDefaultSize, 0 );
        gSizer1->Add( m_pCheckAtributosPrivadosGinkgo, 0, wxALL, 5 );


        gSizer1->Add( 0, 0, 1, wxEXPAND, 5 );


        bSizer10->Add( gSizer1, 1, wxEXPAND, 5 );


        m_pPanelCampos->SetSizer( bSizer10 );
        m_pPanelCampos->Layout();
        bSizer10->Fit( m_pPanelCampos );
        bSizer61->Add( m_pPanelCampos, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

        m_panel5 = new SubTitledPanelSeparatorV( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        bSizer61->Add( m_panel5, 0, wxEXPAND, 5 );

        m_pTagsPanel = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        m_pTagsPanel->SetToolTip( _("Tags (double click to change value)") );

        wxBoxSizer* bSizer4;
        bSizer4 = new wxBoxSizer( wxHORIZONTAL );

        m_pTagsList = new wxPropertyGrid(m_pTagsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE|wxPG_HIDE_CATEGORIES|wxPG_HIDE_MARGIN|wxTAB_TRAVERSAL);
        bSizer4->Add( m_pTagsList, 1, wxEXPAND, 5 );

        m_button4 = new wxButton( m_pTagsPanel, wxID_ANY, _("Add new..."), wxDefaultPosition, wxDefaultSize, 0 );
        bSizer4->Add( m_button4, 0, wxALIGN_RIGHT|wxLEFT, 5 );


        m_pTagsPanel->SetSizer( bSizer4 );
        m_pTagsPanel->Layout();
        bSizer4->Fit( m_pTagsPanel );
        bSizer61->Add( m_pTagsPanel, 1, wxEXPAND, 5 );


        this->SetSizer( bSizer61 );
        this->Layout();

        // Connect Events
        m_pCheckNombrePaciente->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AnonymizePanelBase::OnCheckNombreApellidos ), NULL, this );
        m_pCheckIdPaciente->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AnonymizePanelBase::OnCheckIdPaciente ), NULL, this );
        m_pCheckInstituciones->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AnonymizePanelBase::OnCheckInstituciones ), NULL, this );
        m_pReferringPhysician->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AnonymizePanelBase::OnCheckPhysician ), NULL, this );
        m_pCheckComentarios->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AnonymizePanelBase::OnCheckComentarios ), NULL, this );
        m_button4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AnonymizePanelBase::OnAddNewTag ), NULL, this );
}

AnonymizePanelBase::~AnonymizePanelBase()
{
        // Disconnect Events
        m_pCheckNombrePaciente->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AnonymizePanelBase::OnCheckNombreApellidos ), NULL, this );
        m_pCheckIdPaciente->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AnonymizePanelBase::OnCheckIdPaciente ), NULL, this );
        m_pCheckInstituciones->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AnonymizePanelBase::OnCheckInstituciones ), NULL, this );
        m_pReferringPhysician->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AnonymizePanelBase::OnCheckPhysician ), NULL, this );
        m_pCheckComentarios->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AnonymizePanelBase::OnCheckComentarios ), NULL, this );
        m_button4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AnonymizePanelBase::OnAddNewTag ), NULL, this );

}

AnonymizeDialogBase::AnonymizeDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
        this->SetSizeHints( wxSize( 800,600 ), wxDefaultSize );

        wxBoxSizer* bSizer8;
        bSizer8 = new wxBoxSizer( wxVERTICAL );

        m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        m_pHeader->SetToolTip( _("Anonymize") );

        bSizer8->Add( m_pHeader, 0, wxEXPAND, 5 );

        m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        m_pBodySizer = new wxBoxSizer( wxVERTICAL );


        m_pBody->SetSizer( m_pBodySizer );
        m_pBody->Layout();
        m_pBodySizer->Fit( m_pBody );
        bSizer8->Add( m_pBody, 1, wxEXPAND, 5 );

        m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        wxBoxSizer* bSizer10;
        bSizer10 = new wxBoxSizer( wxHORIZONTAL );


        bSizer10->Add( 0, 0, 1, wxEXPAND, 5 );

        m_pOverwrite = new wxButton( m_pFooter, wxID_ANY, _("Overwrite"), wxDefaultPosition, wxDefaultSize, 0 );
        bSizer10->Add( m_pOverwrite, 0, wxRIGHT, 5 );

        m_pCreateNew = new wxButton( m_pFooter, wxID_OK, _("Create new"), wxDefaultPosition, wxDefaultSize, 0 );
        bSizer10->Add( m_pCreateNew, 0, wxRIGHT, 5 );

        m_pCancel = new wxButton( m_pFooter, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
        m_pCancel->SetDefault();
        bSizer10->Add( m_pCancel, 0, 0, 5 );


        m_pFooter->SetSizer( bSizer10 );
        m_pFooter->Layout();
        bSizer10->Fit( m_pFooter );
        bSizer8->Add( m_pFooter, 0, wxEXPAND, 5 );


        this->SetSizer( bSizer8 );
        this->Layout();
        bSizer8->Fit( this );

        this->Centre( wxBOTH );

        // Connect Events
        m_pOverwrite->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AnonymizeDialogBase::OnOverwriteClick ), NULL, this );
        m_pCreateNew->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AnonymizeDialogBase::OnCreateNewClick ), NULL, this );
}

AnonymizeDialogBase::~AnonymizeDialogBase()
{
        // Disconnect Events
        m_pOverwrite->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AnonymizeDialogBase::OnOverwriteClick ), NULL, this );
        m_pCreateNew->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AnonymizeDialogBase::OnCreateNewClick ), NULL, this );

}

AnonymizeAddTagDialogBase::AnonymizeAddTagDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
        this->SetSizeHints( wxSize( 550,280 ), wxDefaultSize );

        wxBoxSizer* bSizer6;
        bSizer6 = new wxBoxSizer( wxVERTICAL );

        m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        wxBoxSizer* bSizer7;
        bSizer7 = new wxBoxSizer( wxVERTICAL );

        m_pTitled = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        m_pTitled->SetToolTip( _("Add DICOM tag") );

        wxBoxSizer* bSizer8;
        bSizer8 = new wxBoxSizer( wxVERTICAL );

        wxArrayString m_pTagChoices;
        m_pTag = new wxChoice( m_pTitled, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pTagChoices, 0 );
        m_pTag->SetSelection( 0 );
        m_pTag->SetMaxSize( wxSize( 470,-1 ) );

        bSizer8->Add( m_pTag, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );

        wxFlexGridSizer* fgSizer1;
        fgSizer1 = new wxFlexGridSizer( 0, 3, 0, 0 );
        fgSizer1->AddGrowableCol( 2 );
        fgSizer1->SetFlexibleDirection( wxBOTH );
        fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

        m_staticText1 = new wxStaticText( m_pTitled, wxID_ANY, _("Group"), wxDefaultPosition, wxDefaultSize, 0 );
        m_staticText1->Wrap( -1 );
        fgSizer1->Add( m_staticText1, 0, wxALL, 5 );

        m_staticText2 = new wxStaticText( m_pTitled, wxID_ANY, _("Element"), wxDefaultPosition, wxDefaultSize, 0 );
        m_staticText2->Wrap( -1 );
        fgSizer1->Add( m_staticText2, 0, wxALL, 5 );

        m_staticText3 = new wxStaticText( m_pTitled, wxID_ANY, _("Value"), wxDefaultPosition, wxDefaultSize, 0 );
        m_staticText3->Wrap( -1 );
        fgSizer1->Add( m_staticText3, 0, wxALL, 5 );

        m_pTextGroup = new wxTextCtrl( m_pTitled, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
        m_pTextGroup->SetMaxLength( 4 );
        fgSizer1->Add( m_pTextGroup, 0, wxALL, 5 );

        m_pTextElement = new wxTextCtrl( m_pTitled, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
        m_pTextElement->SetMaxLength( 4 );
        fgSizer1->Add( m_pTextElement, 0, wxALL, 5 );

        m_pTextValue = new wxTextCtrl( m_pTitled, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
        m_pTextValue->SetMaxLength( 0 );
        fgSizer1->Add( m_pTextValue, 0, wxALL|wxEXPAND, 5 );


        bSizer8->Add( fgSizer1, 1, wxEXPAND, 5 );


        m_pTitled->SetSizer( bSizer8 );
        m_pTitled->Layout();
        bSizer8->Fit( m_pTitled );
        bSizer7->Add( m_pTitled, 1, wxEXPAND | wxALL, 5 );


        m_pBody->SetSizer( bSizer7 );
        m_pBody->Layout();
        bSizer7->Fit( m_pBody );
        bSizer6->Add( m_pBody, 1, wxEXPAND, 5 );

        m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        wxBoxSizer* bSizer10;
        bSizer10 = new wxBoxSizer( wxHORIZONTAL );


        bSizer10->Add( 0, 0, 1, wxEXPAND, 5 );

        m_button5 = new wxButton( m_pFooter, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
        bSizer10->Add( m_button5, 0, wxRIGHT, 5 );

        m_button6 = new wxButton( m_pFooter, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
        bSizer10->Add( m_button6, 0, 0, 5 );


        m_pFooter->SetSizer( bSizer10 );
        m_pFooter->Layout();
        bSizer10->Fit( m_pFooter );
        bSizer6->Add( m_pFooter, 0, wxEXPAND, 5 );


        this->SetSizer( bSizer6 );
        this->Layout();

        this->Centre( wxBOTH );

        // Connect Events
        m_pTag->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AnonymizeAddTagDialogBase::OnTagSelection ), NULL, this );
        m_button5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AnonymizeAddTagDialogBase::OnOkClick ), NULL, this );
}

AnonymizeAddTagDialogBase::~AnonymizeAddTagDialogBase()
{
        // Disconnect Events
        m_pTag->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AnonymizeAddTagDialogBase::OnTagSelection ), NULL, this );
        m_button5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AnonymizeAddTagDialogBase::OnOkClick ), NULL, this );

}
