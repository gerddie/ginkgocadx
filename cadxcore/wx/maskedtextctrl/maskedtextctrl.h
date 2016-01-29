/////////////////////////////////////////////////////////////////////////////
// Name:        wxMaskedTextCtrl.h
// Purpose:     wxMaskedTextCtrl: masked edit control class
// Author:      Thomas Härtel
// Modified by:
// Created:     2002-11-14
// RCS-ID:      $Id: wxMaskedTextCtrl.h,v $
// Copyright:   (c) Thomas Härtel
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <api/api.h>
#include <wx/list.h>
#include <wx/datetime.h>
#include <wx/textctrl.h>

const wxChar chNULL = wxT('\0') ;
const wxChar chDECIMALNULL = wxT('0') ;
const wxChar chCR = wxT('\r') ;
const wxChar chLF = wxT('\n') ;
const wxChar chSPACE = wxT('_') ;
const wxChar chPERIOD = wxT('.') ;
const wxChar chCOMMA = wxT(',') ;
const wxChar chCOLON = wxT(':') ;
const wxChar chSLASH = wxT('/') ;

/////////////////////////////////////////////////////////////////////////////
// wxMaskData object

const wxChar chMaskPlaceholderDECIMALSEPARATOR = wxT('.');
const wxChar chMaskPlaceholderTHOUSANDSSEPARATOR = wxT(',');
const wxChar chMaskPlaceholderTIMESEPARATOR = wxT(':');
const wxChar chMaskPlaceholderDATESEPARATOR = wxT('/');
const wxChar chMaskPlaceholderDIGIT = wxT('#');
const wxChar chMaskPlaceholderALPHANUMERIC = wxT('A');
const wxChar chMaskPlaceholderALPHABETIC = wxT('?');
const wxChar chMaskPlaceholderALPHABETICUPPER = wxT('>');
const wxChar chMaskPlaceholderALPHABETICLOWER = wxT('<');
const wxChar chMaskPlaceholderCHARACTER = wxT('&');
const wxChar chMaskPlaceholderLITERALESCAPE = wxT('\\');
const wxChar chMaskPlaceholderDATEDAY = wxT('d');
const wxChar chMaskPlaceholderDATEMONTH = wxT('m');
const wxChar chMaskPlaceholderDATEYEAR = wxT('y');
const wxChar chMaskPlaceholderTIMEHOUR = wxT('h');
const wxChar chMaskPlaceholderTIMEMINUTE = wxT('n');
const wxChar chMaskPlaceholderTIMESECOND = wxT('s');

enum enumMaskDataType
{
	MaskDataTypeDECIMALSEPARATOR=0,  // Decimal separator. 
	MaskDataTypeTHOUSANDSSEPARATOR,  // Thousands separator. 
	MaskDataTypeTIMESEPARATOR     ,  // Time separator. 
	MaskDataTypeDATESEPARATOR     ,  // Date separator. 
	MaskDataTypeDIGIT             ,  // # digit placeholder (0-9). 
	MaskDataTypeALPHANUMERIC      ,  // A alphanumeric character placeholder (0-9 and a-Z). 
	MaskDataTypeALPHABETIC        ,  // ? Alphabetic placeholder (a-Z). 
	MaskDataTypeALPHAETICUPPER    ,  // > Alphabetic placeholder, but forced to Uppercase characters (A-Z). 
	MaskDataTypeALPHAETICLOWER    ,  // < Alphabetic placeholder, but forced to Lowercase characters (a-z). 
	MaskDataTypeCHARACTER         ,  // # character placeholder (ANSI characters 32-126 and 128-255). 
	MaskDataTypeLITERALESCAPE     ,  // All other symbols are displayed as themselves. 
	MaskDataTypeLITERAL           ,  // All other symbols are displayed as themselves. 
	MASKDATATYPECOUNT
};

enum enumMaskDataSubType
{
	MaskDataSubTypeNONE = 0,
	MaskDataSubTypeDATEDAY,
	MaskDataSubTypeDATEMONTH,
	MaskDataSubTypeDATEYEAR,
	MaskDataSubTypeTIMEHOUR,
	MaskDataSubTypeTIMEMINUTE,
	MaskDataSubTypeTIMESECOND,
	MASKDATASUBTYPECOUNT
};

class EXTAPI wxMaskData : public wxObject
{
	DECLARE_DYNAMIC_CLASS(wxMaskData)
	
public:
	wxMaskData();
	virtual ~wxMaskData();

// Attributes
public:
	enumMaskDataType m_eType;
	enumMaskDataSubType m_eSubType;
	wxChar m_chValue;
	
// Operations
public:
	void operator=(const wxMaskData& src);
	bool IsInputData();
	bool IsValidInput(wxChar chNewChar);
	wxChar PreProcessChar(wxChar chNewChar);
};

class EXTAPI wxMaskedTextCtrl : public wxTextCtrl
{
DECLARE_DYNAMIC_CLASS(wxMaskedTextCtrl)

// Member functions ---------------------------------------------------------
public:
	wxMaskedTextCtrl();
	wxMaskedTextCtrl(wxWindow *parent, wxWindowID id, const wxString& mask, const wxString &value, const wxPoint &pos, const wxSize &size, int style);
	virtual ~wxMaskedTextCtrl();

	wxString GetMask();
	void SetMask(wxString& mask);
	wxChar GetPromptSymbol();
	void SetPromptSymbol(wxChar chNewPromptSymbol);
	wxChar GetDecimalSeperator();
	void SetDecimalSeperator(wxChar chNewDecimalSeperator);
	wxChar GetThousandsSeperator();
	void SetThousandsSeperator(wxChar chNewThousandsSeperator);
	wxChar GetTimeSeperator();
	void SetTimeSeperator(wxChar chNewTimeSeperator);
	wxChar GetDateSeperator();
	void SetDateSeperator(wxChar chNewDateSeperator);
	bool GetInsertMode();
	void SetInsertMode(bool bInsertMode);
	bool GetAutoTab();
	void SetAutoTab(bool bAutoTab);
	bool GetBackwardLocationRight();
	void SetBackwardLocationRight(bool bRight=TRUE);
	wxDateTime GetMinDateTime(void);
	void SetMinDateTime(wxDateTime& minDateTime);
	wxDateTime GetMaxDateTime(void);
	void SetMaxDateTime(wxDateTime& maxDateTime);
	void SetDateTimeRange(wxDateTime& minDateTime, wxDateTime& maxDateTime);

	void Clear();
	void SetValue(const wxString& value);
	wxDateTime GetDateTimeValue();
	void SetDateTimeValue(wxDateTime& value);
	
// Internal Member functions ------------------------------------------------
protected:
	// Event Handler
	void OnKeyDown(wxKeyEvent& event);
	void OnChar(wxKeyEvent& event);
	void OnSetFocus(wxFocusEvent& event);
	void OnKillFocus(wxFocusEvent& event);
	void OnLeftButtonDown(wxMouseEvent& event);
	void OnCut(wxUpdateUIEvent& event);
	void OnCopy(wxUpdateUIEvent& event);
	void OnPaste(wxUpdateUIEvent& event);
	DECLARE_EVENT_TABLE();
	
	wxString ShowMask();
	wxString GetInputData();
	wxString GetInputData(wxString& value);
	bool SetInputData(const wxString& value, int nBeginPos=0, bool bAllowPrompt=TRUE);	
	void EmptyData(bool bOnlyInput=FALSE);
	bool IsInputEmpty() const;
	bool IsEmpty() const;
	virtual void ValidationError();
	bool Validate();
	bool IsValid(void);
	int RPtoLP(int nRealPos);
	int LPtoRP(int nLogicalPos);

	void DeleteContents();
	bool IsInputData(int nPosition);
	int DeleteRange(int nSelectionStart, int nSelectionEnd);
	int InsertAt(int nSelectionStart, wxChar chNewChar);
	int SetAt(int nSelectionStart, wxChar chNewChar);
	int GetNextInputLocation(int nSelectionStart);
	int GetPreviousInputLocation(int nSelectionStart);
	int GetEmptyInputLocation(int nSelectionStart);

	void UpdateControl(int nSelectionStart=0);  // UpdateInsertionPoint is automatic. 
	void UpdateInsertionPointForward(int nSelectionStart);
	void UpdateInsertionPointBackward(int nSelectionStart);

// Data members -------------------------------------------------------------
	wxList m_listData;

	bool m_bInsertMode;
	wxChar m_chPromptSymbol;
	wxChar m_chIntlDecimal;
	wxChar m_chIntlThousands;
	wxChar m_chIntlTime;
	wxChar m_chIntlDate;
	bool m_bAutoTab;	
	bool m_bBackwardLocationRight;
	bool m_bNeedValidation;
	bool m_bValidation;

	wxDateTime m_dtDateTime;
	wxDateTime m_dtMinDateTime;
	wxDateTime m_dtMaxDateTime;
};
