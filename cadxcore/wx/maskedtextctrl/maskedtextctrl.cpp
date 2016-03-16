/////////////////////////////////////////////////////////////////////////////
// Name:        wxMaskedTextCtrl.cpp
// Purpose:     wxMaskedTextCtrl: masked edit control class
// Author:      Thomas H‰rtel
// Modified by:
// Created:     2002-11-14
// RCS-ID:      $Id: wxMaskedTextCtrl.cpp,v $
// Copyright:   (c) Thomas H‰rtel
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
// For compilers that support precompilation, includes "wx/wx.h".
#if defined(__WXGTK__) || defined(__WXMOTIF__)
        #include "wx/wx.h"
#endif


#include "wx/wxprec.h"
#include "maskedtextctrl.h"
#include <api/controllers/icontroladorlog.h>

IMPLEMENT_DYNAMIC_CLASS(wxMaskData, wxObject)
IMPLEMENT_DYNAMIC_CLASS(wxMaskedTextCtrl, wxTextCtrl)

BEGIN_EVENT_TABLE(wxMaskedTextCtrl, wxTextCtrl)
    EVT_KEY_DOWN(wxMaskedTextCtrl::OnKeyDown)
    EVT_CHAR(wxMaskedTextCtrl::OnChar)

	EVT_SET_FOCUS(wxMaskedTextCtrl::OnSetFocus)
	EVT_KILL_FOCUS(wxMaskedTextCtrl::OnKillFocus)
	EVT_LEFT_DOWN(wxMaskedTextCtrl::OnLeftButtonDown)

	EVT_UPDATE_UI(wxID_CUT, wxMaskedTextCtrl::OnCut)
    EVT_UPDATE_UI(wxID_COPY, wxMaskedTextCtrl::OnCopy)
    EVT_UPDATE_UI(wxID_PASTE, wxMaskedTextCtrl::OnPaste)
END_EVENT_TABLE()



wxMaskData::wxMaskData() :
	m_eType(MaskDataTypeLITERAL),
    m_eSubType(MaskDataSubTypeNONE),
	m_chValue(chNULL)
{
}

wxMaskData::~wxMaskData()
{
}

void wxMaskData::operator=(const wxMaskData& src)
{
	m_eType = src.m_eType;
	m_chValue = src.m_chValue;
}

bool wxMaskData::IsInputData()
{

	bool bIsInputData=FALSE;
	switch(m_eType)
	{
		// These are the input types.
		case MaskDataTypeDIGIT:
		case MaskDataTypeALPHANUMERIC:
		case MaskDataTypeALPHABETIC:
		case MaskDataTypeALPHAETICUPPER:
		case MaskDataTypeALPHAETICLOWER:
		case MaskDataTypeCHARACTER:
			bIsInputData=TRUE;
			break;
		default:
			break;
	}
	return bIsInputData;
}

bool wxMaskData::IsValidInput(wxChar chNewChar)
{
	bool bIsValidInput=FALSE;
	switch(m_eType)
	{
		// These are the input types.
		case MaskDataTypeDIGIT:
			bIsValidInput=wxIsdigit(chNewChar) != 0;
			break;

		case MaskDataTypeALPHANUMERIC:
			bIsValidInput=wxIsalnum(chNewChar) != 0;
			break;

		case MaskDataTypeALPHABETIC:
		case MaskDataTypeALPHAETICUPPER:
		case MaskDataTypeALPHAETICLOWER:
			bIsValidInput=wxIsalpha(chNewChar) != 0;
			break;

		case MaskDataTypeCHARACTER:
			if((chNewChar >=  32) && (chNewChar <= 126))
				bIsValidInput=TRUE;
			if((chNewChar >= 128) && (chNewChar <= 255))
				bIsValidInput = TRUE;
			break;
		default:
			break;
	}
	return bIsValidInput;
}

wxChar wxMaskData::PreProcessChar(wxChar chNewChar)
{
	wxChar chProcessedChar=chNewChar;
	switch(m_eType)
	{
		case MaskDataTypeALPHAETICUPPER:
			chProcessedChar=(wxChar) wxToupper(chNewChar);
			break;

		case MaskDataTypeALPHAETICLOWER:
			chProcessedChar=(wxChar) wxTolower(chNewChar);
			break;
		default:
			break;
	}
	return chProcessedChar;
}


wxMaskedTextCtrl::wxMaskedTextCtrl(wxWindow *parent, wxWindowID id, const wxString& mask, const wxString &value, const wxPoint &pos, const wxSize &size, int style = 0)
        : wxTextCtrl(parent, id, value, pos, size, style), m_bInsertMode(TRUE), m_chPromptSymbol(chSPACE),
			m_chIntlDecimal(chPERIOD),
			m_chIntlThousands(chCOMMA),
			m_chIntlTime(chCOLON),
			m_chIntlDate(chSLASH),
			m_bAutoTab(FALSE),
			m_bBackwardLocationRight(TRUE),
			m_bNeedValidation(TRUE),
			m_bValidation(FALSE),
			m_dtDateTime(wxInvalidDateTime),
			m_dtMinDateTime(wxInvalidDateTime),
			m_dtMaxDateTime(wxInvalidDateTime)
{
	wxString str = mask;
	SetMask(str);

	if(m_listData.GetCount() != 0)
		UpdateControl();
}

wxMaskedTextCtrl::~wxMaskedTextCtrl()
{
	DeleteContents();
}

void wxMaskedTextCtrl::DeleteContents()
{
	if(m_listData.GetCount() == 0)
		return;

	m_listData.DeleteContents(TRUE);

	m_bNeedValidation = FALSE;
	m_bValidation = TRUE;
}

wxString wxMaskedTextCtrl::GetMask()
{
	wxString csMask;
	wxMaskData* pobjData=NULL;
	for(unsigned long pos = 0; pos < m_listData.GetCount();pos++)
	{
		pobjData= (wxMaskData *) (m_listData.Item(pos))->GetData();
		switch(pobjData->m_eType)
		{
			case MaskDataTypeDECIMALSEPARATOR:
				csMask += chMaskPlaceholderDECIMALSEPARATOR;
				break;

			case MaskDataTypeTHOUSANDSSEPARATOR:
				csMask += chMaskPlaceholderTHOUSANDSSEPARATOR;
				break;

			case MaskDataTypeTIMESEPARATOR:
				csMask += chMaskPlaceholderTIMESEPARATOR;
				break;
			case MaskDataTypeDATESEPARATOR:
				csMask += chMaskPlaceholderDATESEPARATOR;
				break;

			case MaskDataTypeALPHANUMERIC:
				csMask += chMaskPlaceholderALPHANUMERIC;
				break;

			case MaskDataTypeALPHABETIC:
				csMask += chMaskPlaceholderALPHABETIC;
				break;

			case MaskDataTypeALPHAETICUPPER:
				csMask += chMaskPlaceholderALPHABETICUPPER;
				break;

			case MaskDataTypeALPHAETICLOWER:
				csMask += chMaskPlaceholderALPHABETICLOWER;
				break;

			case MaskDataTypeCHARACTER:
				csMask += chMaskPlaceholderCHARACTER;
				break;

			case MaskDataTypeDIGIT:
				switch(pobjData->m_eSubType)
				{
					case MaskDataSubTypeDATEDAY:
						csMask += chMaskPlaceholderDATEDAY;
						break;

					case MaskDataSubTypeDATEMONTH:
						csMask += chMaskPlaceholderDATEMONTH;
						break;

					case MaskDataSubTypeDATEYEAR:
						csMask += chMaskPlaceholderDATEYEAR;
						break;

					case MaskDataSubTypeTIMEHOUR:
						csMask += chMaskPlaceholderTIMEHOUR;
						break;

					case MaskDataSubTypeTIMEMINUTE:
						csMask += chMaskPlaceholderTIMEMINUTE;
						break;

					case MaskDataSubTypeTIMESECOND:
						csMask += chMaskPlaceholderTIMESECOND;
						break;

					default:
						csMask += chMaskPlaceholderDIGIT;
						break;
				}
				break;

			case MaskDataTypeLITERALESCAPE:
				// Need to add the escape to things that were escaped.
				csMask += chMaskPlaceholderLITERALESCAPE;
				csMask += pobjData->m_chValue;
				break;

			default:
				// Literals and everything else is kept the same.
				csMask += pobjData->m_chValue;
				break;
		}
	}
	return csMask;
}

void wxMaskedTextCtrl::SetMask(wxString& mask)
{
	if(mask.IsEmpty())
	{
		mask = wxT("");
	}
	DeleteContents();

	wxMaskData* pobjData = NULL;
	for(unsigned int i = 0;i < mask.Length(); i++)
	{
		wxChar chNew = mask[i];
		pobjData = new wxMaskData();
		if(pobjData)
		{
			m_listData.Append(pobjData);
			pobjData->m_eSubType = MaskDataSubTypeNONE;

			switch(chNew)
			{
				case chMaskPlaceholderDECIMALSEPARATOR:
					pobjData->m_eType = MaskDataTypeDECIMALSEPARATOR;
					pobjData->m_chValue = m_chIntlDecimal;
					break;

				case chMaskPlaceholderTHOUSANDSSEPARATOR:
					pobjData->m_eType = MaskDataTypeTHOUSANDSSEPARATOR;
					pobjData->m_chValue = m_chIntlThousands;
					break;

				case chMaskPlaceholderTIMESEPARATOR:
					pobjData->m_eType = MaskDataTypeTIMESEPARATOR;
					pobjData->m_chValue = m_chIntlTime;
					break;

				case chMaskPlaceholderDATESEPARATOR:
					pobjData->m_eType = MaskDataTypeDATESEPARATOR;
					pobjData->m_chValue = m_chIntlDate;
					break;

				case chMaskPlaceholderDIGIT:
					pobjData->m_eType = MaskDataTypeDIGIT;
					pobjData->m_chValue = m_chPromptSymbol;
					break;

				case chMaskPlaceholderALPHANUMERIC:
					pobjData->m_eType = MaskDataTypeALPHANUMERIC;
					pobjData->m_chValue = m_chPromptSymbol;
					break;

				case chMaskPlaceholderALPHABETIC:
					pobjData->m_eType = MaskDataTypeALPHABETIC;
					pobjData->m_chValue = m_chPromptSymbol;
					break;

				case chMaskPlaceholderALPHABETICUPPER:
					pobjData->m_eType = MaskDataTypeALPHAETICUPPER;
					pobjData->m_chValue = m_chPromptSymbol;
					break;

				case chMaskPlaceholderALPHABETICLOWER:
					pobjData->m_eType = MaskDataTypeALPHAETICLOWER;
					pobjData->m_chValue = m_chPromptSymbol;
					break;

				case chMaskPlaceholderCHARACTER:
					pobjData->m_eType = MaskDataTypeCHARACTER;
					pobjData->m_chValue = m_chPromptSymbol;
					break;

				default:
					if(chNew == chMaskPlaceholderLITERALESCAPE)
					{
						// It is the next character that is inserted.
						chNew = mask[++i];
						if(chNew)
						{
							pobjData->m_eType = MaskDataTypeLITERALESCAPE;
							pobjData->m_chValue = chNew;
							break;
						}
					}
					else if(chNew == chMaskPlaceholderDATEDAY)
					{
						// It is the next character that is inserted.
						wxChar chNext = (i < (mask.Length()-1) ? mask[i+1] : wxT('\0'));
						wxChar chBefore = (i > 0 ? mask[i-1] : wxT('\0'));
						if((chNext == chMaskPlaceholderDATEDAY || chBefore == chMaskPlaceholderDATEDAY) && chBefore != chNext)
						{
							pobjData->m_eType = MaskDataTypeDIGIT;
							pobjData->m_eSubType = MaskDataSubTypeDATEDAY;
							pobjData->m_chValue = m_chPromptSymbol;
							break;
						}
					}
					else if(chNew == chMaskPlaceholderDATEMONTH)
					{
						// It is the next character that is inserted.
						wxChar chNext = (i < (mask.Length()-1) ? mask[i+1] : wxT('\0'));
						wxChar chBefore = (i > 0 ? mask[i-1] : wxT('\0'));
						if((chNext == chMaskPlaceholderDATEMONTH || chBefore == chMaskPlaceholderDATEMONTH) && chBefore != chNext)
						{
							pobjData->m_eType = MaskDataTypeDIGIT;
							pobjData->m_eSubType = MaskDataSubTypeDATEMONTH;
							pobjData->m_chValue = m_chPromptSymbol;
							break;
						}
					}
					else if(chNew == chMaskPlaceholderDATEYEAR)
					{
						// It is the next character that is inserted.
						wxChar chNext = (i < (mask.Length()-1) ? mask[i+1] : wxT('\0'));
						wxChar chBefore = (i > 0 ? mask[i-1] : wxT('\0'));
						if(chNext == chMaskPlaceholderDATEYEAR || chBefore == chMaskPlaceholderDATEYEAR)
						{
							pobjData->m_eType = MaskDataTypeDIGIT;
							pobjData->m_eSubType = MaskDataSubTypeDATEYEAR;
							pobjData->m_chValue = m_chPromptSymbol;
							break;
						}
					}
					else if(chNew == chMaskPlaceholderTIMEHOUR)
					{
						// It is the next character that is inserted.
						wxChar chNext = (i < (mask.Length()-1) ? mask[i+1] : wxT('\0'));
						wxChar chBefore = (i > 0 ? mask[i-1] : wxT('\0'));
						if((chNext == chMaskPlaceholderTIMEHOUR || chBefore == chMaskPlaceholderTIMEHOUR) && chBefore != chNext)
						{
							pobjData->m_eType = MaskDataTypeDIGIT;
							pobjData->m_eSubType = MaskDataSubTypeTIMEHOUR;
							pobjData->m_chValue = m_chPromptSymbol;
							break;
						}
					}
					else if(chNew == chMaskPlaceholderTIMEMINUTE)
					{
						// It is the next character that is inserted.
						wxChar chNext = (i < (mask.Length()-1) ? mask[i+1] : wxT('\0'));
						wxChar chBefore = (i > 0 ? mask[i-1] : wxT('\0'));
						if((chNext == chMaskPlaceholderTIMEMINUTE || chBefore == chMaskPlaceholderTIMEMINUTE) && chBefore != chNext)
						{
							pobjData->m_eType = MaskDataTypeDIGIT;
							pobjData->m_eSubType = MaskDataSubTypeTIMEMINUTE;
							pobjData->m_chValue = m_chPromptSymbol;
							break;
						}
					}
					else if(chNew == chMaskPlaceholderTIMESECOND)
					{
						// It is the next character that is inserted.
						wxChar chNext = (i < (mask.Length()-1) ? mask[i+1] : wxT('\0'));
						wxChar chBefore = (i > 0 ? mask[i-1] : wxT('\0'));
						if((chNext == chMaskPlaceholderTIMESECOND || chBefore == chMaskPlaceholderTIMESECOND) && chBefore != chNext)
						{
							pobjData->m_eType = MaskDataTypeDIGIT;
							pobjData->m_eSubType = MaskDataSubTypeTIMESECOND;
							pobjData->m_chValue = m_chPromptSymbol;
							break;
						}
					}

					// Everything else is just a literal.
					pobjData->m_eType = MaskDataTypeLITERAL;
					pobjData->m_chValue = chNew;
					break;
			}
		}
	}
	wxTextCtrl::SetMaxLength(m_listData.GetCount());
	UpdateControl();
}

wxString wxMaskedTextCtrl::GetInputData()
{
	wxString csInputData;

	if(m_listData.GetCount() == 0)
	{
		csInputData = GetValue();
		return csInputData;
	}

	wxMaskData* pobjData=NULL;
	for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
	{
		pobjData= (wxMaskData *) (m_listData.Item(pos))->GetData();
		// Ignore everything that is not data.
		if(pobjData->IsInputData())
			csInputData += pobjData->m_chValue;
	}
	return csInputData;
}

wxString wxMaskedTextCtrl::GetInputData(wxString& value)
{
	wxString csInputData = value;
	unsigned int nSymbolCount = csInputData.Length();
	wxMaskData* pobjData = NULL;
	wxString sToExclude;
	int nStartPos = -1;
	int nEndPos = -1;
	unsigned int nIndex = 0;
	int nRemovedCount = 0;

	for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
	{
		pobjData = (wxMaskData *) (m_listData.Item(pos))->GetData();
		if(!pobjData->IsInputData())
		{
			if(nStartPos == -1)
			{
				nStartPos = nIndex;
				sToExclude.Empty();
			}
			sToExclude+=pobjData->m_chValue;
		}
		else
		{
			if(nStartPos != -1)
			{
				nEndPos = nIndex-1;
				if(csInputData.Mid(nStartPos-nRemovedCount, nEndPos-nStartPos+1) == sToExclude)
				{
					csInputData=csInputData.Left(nStartPos-nRemovedCount) + csInputData.Mid(nEndPos-nRemovedCount+1);
					nRemovedCount+=nEndPos-nStartPos+1;
				}
				nStartPos = -1;
			}
		}

		nIndex++;
		if(nIndex >= nSymbolCount)
			break;
	}

	return csInputData;
}

bool wxMaskedTextCtrl::SetInputData(const wxString& value, int nBeginPos/*=0*/, bool bAllowPrompt/*=TRUE*/)
{
	wxString csFullInput;

	m_bNeedValidation = TRUE;
	m_bValidation = FALSE;


	// Start with existing data and append the new data.
	csFullInput = GetInputData();
	csFullInput = csFullInput.Left(nBeginPos);

	if(bAllowPrompt)
		csFullInput += value;
	else
	{
		// If the prompt symbol is not valid, then
		// add the data one-by-one ignoring any prompt symbols.
		for(unsigned int i = 0;i < value.Length();i++)
		{
			if(value[i] != m_chPromptSymbol)
				csFullInput += value[i];
		}
	}

	bool bCompleteSuccess=TRUE;
	wxString pszReplaceData=csFullInput;
	wxMaskData* pobjData=NULL;
	unsigned int posReplaceData=0;

	for(unsigned long pos = 0; pos < m_listData.GetCount();pos++)
	{
		pobjData = (wxMaskData *) (m_listData.Item(pos))->GetData();

		// Ignore everything that is not data.
		if(pobjData->IsInputData())
		{
			// If we run out of replacement data, then use the prompt symbol.
			// Make sure we iterate through the entire list so that the
			// prompt symbol is applied to any empty areas.
			if(posReplaceData < pszReplaceData.Length())
			{
				// This inner while loop is so that we can re-apply input data
				// after an error.  This will allow us to skip over invalid
				// input data and try the next character.
				while(posReplaceData< pszReplaceData.Length())
				{
					wxChar chReplace = pszReplaceData[posReplaceData++];

					// Make sure to follow the input validation.
					// The prompt symbol is always valid at this level.
					// This allows the user to erase a string by overtyping a space.
					// On error, just skip the character being inserted.
					// This will allow the DeleteRange() function to have the remaining
					// characters validated.
					if((chReplace == m_chPromptSymbol) || pobjData->IsValidInput(chReplace))
					{
						pobjData->m_chValue = pobjData->PreProcessChar(chReplace);
						break;
					}
					else
						bCompleteSuccess = FALSE;
				}
			}
			else
				pobjData->m_chValue = m_chPromptSymbol;
		}
	}

	UpdateControl();

	return bCompleteSuccess;
}

wxChar wxMaskedTextCtrl::GetPromptSymbol()
{
	return m_chPromptSymbol;
}

void wxMaskedTextCtrl::SetPromptSymbol(wxChar chNewPromptSymbol)
{
	// The prompt symbol must be a valid edit box symbol.

	if(wxIsprint((int) chNewPromptSymbol))
	{
		// Just for the heck of it, if the prompt symbol changes,
		// go through and replace the existing prompts with the new prompt.
		wxMaskData* pobjData = NULL;
		for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
		{
			pobjData = (wxMaskData *) (m_listData.Item(pos))->GetData();
			if(pobjData->IsInputData())
			{
				if(pobjData->m_chValue == m_chPromptSymbol)
					pobjData->m_chValue = chNewPromptSymbol;
			}
		}
		m_chPromptSymbol = chNewPromptSymbol;
	}

	// Don't update the insertion point if just setting the prompt symbol.
	UpdateControl(-1);
}

wxChar wxMaskedTextCtrl::GetDecimalSeperator()
{
	return m_chIntlDecimal;
}

void wxMaskedTextCtrl::SetDecimalSeperator(wxChar chNewDecimalSeperator)
{
	// The decimal seperator must be a valid edit box symbol.

	if(wxIsprint((int) chNewDecimalSeperator))
	{
		// Just for the heck of it, if the decimal symbol changes,
		// go through and replace the existing symbols with the new symbol.
		wxMaskData* pobjData = NULL;
		for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
		{
			pobjData = (wxMaskData *) (m_listData.Item(pos))->GetData();
			if(pobjData->m_eType == MaskDataTypeDECIMALSEPARATOR)
				pobjData->m_chValue = chNewDecimalSeperator;
		}
		m_chIntlDecimal = chNewDecimalSeperator;
	}

	// Don't update the insertion point if just setting the decimal symbol.
	UpdateControl(-1);
}

wxChar wxMaskedTextCtrl::GetThousandsSeperator()
{
	return m_chIntlThousands;
}

void wxMaskedTextCtrl::SetThousandsSeperator(wxChar chNewThousandsSeperator)
{
	// The thousands seperator must be a valid edit box symbol.

	if(wxIsprint((int) chNewThousandsSeperator))
	{
		// Just for the heck of it, if the thousands symbol changes,
		// go through and replace the existing symbols with the new symbol.
		wxMaskData* pobjData = NULL;
		for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
		{
			pobjData = (wxMaskData *) (m_listData.Item(pos))->GetData();
			if(pobjData->m_eType == MaskDataTypeTHOUSANDSSEPARATOR)
				pobjData->m_chValue = chNewThousandsSeperator;
		}
		m_chIntlThousands = chNewThousandsSeperator;
	}

	// Don't update the insertion point if just setting the thousands symbol.
	UpdateControl(-1);
}

wxChar wxMaskedTextCtrl::GetTimeSeperator()
{
	return m_chIntlTime;
}

void wxMaskedTextCtrl::SetTimeSeperator(wxChar chNewTimeSeperator)
{
	// The time seperator must be a valid edit box symbol.

	if(wxIsprint((int) chNewTimeSeperator))
	{
		// Just for the heck of it, if the time symbol changes,
		// go through and replace the existing symbols with the new symbol.
		wxMaskData* pobjData = NULL;
		for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
		{
			pobjData = (wxMaskData *) (m_listData.Item(pos))->GetData();
			if(pobjData->m_eType == MaskDataTypeTIMESEPARATOR)
				pobjData->m_chValue = chNewTimeSeperator;
		}
		m_chIntlTime = chNewTimeSeperator;
	}

	// Don't update the insertion point if just setting the time symbol.
	UpdateControl(-1);
}

wxChar wxMaskedTextCtrl::GetDateSeperator()
{
	return m_chIntlDate;
}

void wxMaskedTextCtrl::SetDateSeperator(wxChar chNewDateSeperator)
{
	// The time seperator must be a valid edit box symbol.

	if(wxIsprint((int) chNewDateSeperator))
	{
		// Just for the heck of it, if the date symbol changes,
		// go through and replace the existing symbols with the new symbol.
		wxMaskData* pobjData=NULL;
		for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
		{
			pobjData = (wxMaskData *) (m_listData.Item(pos))->GetData();
			if(pobjData->m_eType == MaskDataTypeDATESEPARATOR)
				pobjData->m_chValue = chNewDateSeperator;
		}
		m_chIntlDate = chNewDateSeperator;
	}

	// Don't update the insertion point if just setting the date symbol.
	UpdateControl(-1);
}

void wxMaskedTextCtrl::EmptyData(bool bOnlyInput/*=FALSE*/)
{
	if(m_listData.GetCount()==0)
	{
		DeleteContents();
		return;
	}

	if(bOnlyInput)
	{
		// If emptying only the data, the iterate through the list
		// of data and replace input data with the prompt symbol.
		wxMaskData* pobjData = NULL;
		for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
		{
			pobjData = (wxMaskData *) (m_listData.Item(pos))->GetData();
			if(pobjData->IsInputData())
				pobjData->m_chValue=m_chPromptSymbol;
		}
	}
	else
		DeleteContents();

	UpdateControl();
}

bool wxMaskedTextCtrl::IsInputEmpty() const
{
	if(m_listData.GetCount() == 0)
	{
		wxString csInputData;
		csInputData = GetValue();
		return csInputData.IsEmpty();
	}

	wxMaskData* pobjData=NULL;
	for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
	{
		pobjData = (wxMaskData *) (m_listData.Item(pos))->GetData();
		if (pobjData->IsInputData() && pobjData->m_chValue!=m_chPromptSymbol)
			return FALSE;
	}
	return TRUE;
}

bool wxMaskedTextCtrl::GetInsertMode()
{
	return m_bInsertMode;
}

void wxMaskedTextCtrl::SetInsertMode(bool bInsertMode)
{
	m_bInsertMode=bInsertMode;
}

bool wxMaskedTextCtrl::GetAutoTab()
{
	return m_bAutoTab;
}

void wxMaskedTextCtrl::SetAutoTab(bool bAutoTab)
{
	m_bAutoTab=bAutoTab;
}

void wxMaskedTextCtrl::SetBackwardLocationRight(bool bRight)
{
	m_bBackwardLocationRight = bRight;
}

bool wxMaskedTextCtrl::GetBackwardLocationRight()
{
	return m_bBackwardLocationRight;
}

wxString wxMaskedTextCtrl::ShowMask()
{
	wxString csShow;
	wxMaskData* pobjData=NULL;
	for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
	{
		pobjData = (wxMaskData *) (m_listData.Item(pos))->GetData();

		// There is no need to do any fancy string building because
		// all validation is done when characters are inserted into the list.
		// Literals and placeholders are converted properly at that time
		// so all we have to do here is get the value.
		if(pobjData->m_chValue != chNULL)
			csShow += pobjData->m_chValue;
	}
	return csShow;
}

bool wxMaskedTextCtrl::IsInputData(int nPosition)
{
	if(m_listData.GetCount() == 0)
	{
		return TRUE;
	}

	// We frequently need to know if a position refers to
	// input data or to a literal.
	bool bIsInputData=FALSE;
	if(nPosition >= 0 && nPosition < (int) m_listData.GetCount())
	{
		wxMaskData* pobjData = (wxMaskData*) (m_listData.Item(nPosition))->GetData();
		if(pobjData)
		{
			bIsInputData = pobjData->IsInputData();
		}
	}
	return bIsInputData;
}

int wxMaskedTextCtrl::DeleteRange(int nSelectionStart, int nSelectionEnd)
{
	// In order to delete properly, we must count the number of
	// input characters that are selected and only delete that many.
	// This is because the selection can include literals.
	int nCharIndex  =0;
	int nDeleteCount=0;
	wxString csInputData;
	wxMaskData* pobjData = NULL;

	m_bNeedValidation = TRUE;
	m_bValidation = FALSE;

	for(unsigned long pos = 0;pos < m_listData.GetCount(); pos++,nCharIndex++)
	{
		pobjData = (wxMaskData *) (m_listData.Item(pos))->GetData();
		// Ignore everything that is not data.
		// This is just like we do in GetInputData except that we
		// will ignore the input data within the selection range.
		if(pobjData->IsInputData())
		{
			if((nCharIndex < nSelectionStart) || (nCharIndex >= nSelectionEnd))
			{
				// The SetInputData() function will take care of validating
				// the shifted characters.
				csInputData += pobjData->m_chValue;
			}
			else
				nDeleteCount++;
		}
	}
	// Now apply the filtered data stream.
	SetInputData(csInputData);
	// return the deleted count so that an error can be generated
	// if none were deleted.
	return nDeleteCount;
}

int wxMaskedTextCtrl::InsertAt(int nSelectionStart, wxChar chNewChar)
{
	// Although we could have some complex, yet efficient, routine
	// that would error if inserting pushed an existing character
	// into an invalid region.  Instead, just save the current
	// state and restore it on error.
	wxString csPreviousInput=GetInputData();

	int nCharIndex = 0;
	int nInsertionPoint = -1;
	wxString csInputData;
	wxMaskData* pobjData = NULL;
	for(unsigned long pos = 0;pos < m_listData.GetCount(); pos++,nCharIndex++)
	{
		pobjData = (wxMaskData *) (m_listData.Item(pos))->GetData();
		// Ignore everything that is not data.
		// This is just like we do in GetInputData except that we
		// will ignore the input data within the selection range.
		if(pobjData->IsInputData())
		{
			// Wait until a valid insertion point and
			// only make sure to insert once.
			if((nInsertionPoint < 0) && (nCharIndex >= nSelectionStart))
			{
				csInputData += chNewChar;
				nInsertionPoint=nCharIndex;
			}
			csInputData += pobjData->m_chValue;
		}
	}
	// Now apply the filtered data stream and check if it was successful.
	if(!SetInputData(csInputData))
	{
		// If not successful, then restore the previous input and return -1.
		SetInputData(csPreviousInput);
		return -1;
	}
	return nInsertionPoint;
}

int wxMaskedTextCtrl::SetAt(int nSelectionStart, wxChar chNewChar)
{
	if(nSelectionStart >= 0)
	{
		m_bNeedValidation = TRUE;
		m_bValidation = FALSE;

		wxMaskData* pobjData=(wxMaskData*) (m_listData.Item(nSelectionStart))->GetData();
		if(pobjData)
		{
			if(pobjData->IsInputData())
			{
				if((chNewChar == m_chPromptSymbol) || pobjData->IsValidInput(chNewChar))
					pobjData->m_chValue=pobjData->PreProcessChar(chNewChar);
				else
					return -1;  // Input value is invalid or not allowed.
			}
		}
	}
	return nSelectionStart;
}

int wxMaskedTextCtrl::GetNextInputLocation(int nSelectionStart)
{
	// One of the functions of this edit control is that it skips over literals.
	// We need a function to help skip to the next position.
	int nNextInputLocation=nSelectionStart;
	if(nNextInputLocation < 0)
		nNextInputLocation = 0;

	wxMaskData* pobjData = NULL;
	while(nNextInputLocation < (int) m_listData.GetCount())
	{
		pobjData = (wxMaskData *) (m_listData.Item(nNextInputLocation))->GetData();
		if(pobjData->IsInputData())
			break;

		nNextInputLocation++;
	}

	return nNextInputLocation;
}

int wxMaskedTextCtrl::GetPreviousInputLocation(int nSelectionStart)
{
	// One of the functions of this edit control is that it skips over literals.
	// We need a function to help skip to the next position.
	int nNextInputLocation=nSelectionStart;
	if(nNextInputLocation < 0)
		nNextInputLocation = 0;
	// Need to determine if we moved to a previous location.
	// There will need to be some correction.
	int nInitialInputLocation=nNextInputLocation;

	wxMaskData* pobjData = NULL;
	for(wxNode* node = m_listData.Item(nNextInputLocation);node;nNextInputLocation--)
	{
		pobjData = (wxMaskData *) node->GetData();
		if(pobjData->IsInputData())
		{
			if(nInitialInputLocation != nNextInputLocation)
			{
				// If we find a valid previous location, then move to the right of it.
				// This backup, then move forward is typical when seeking backward.
				if(m_bBackwardLocationRight)
					nNextInputLocation++;
			}
			break;
		}
		node = node->GetPrevious();
	}
	// If there is no input data to the left of the selection,
	// then seek forward to the next location.
	if(nNextInputLocation < 0)
		return GetNextInputLocation(nSelectionStart);
	return nNextInputLocation;
}

int wxMaskedTextCtrl::GetEmptyInputLocation(int nSelectionStart)
{
	int nEmptyInputLocation=nSelectionStart;
	if(nEmptyInputLocation < 0)
		nEmptyInputLocation = 0;

	wxMaskData* pobjData=NULL;
	for(unsigned long pos = nEmptyInputLocation;;pos++,nEmptyInputLocation++)
	{
		pobjData = (wxMaskData *) (m_listData.Item(pos))->GetData();
		if(pobjData->IsInputData())
		{
			if(pobjData->m_chValue == m_chPromptSymbol)
				break;
		}
	}
	return nEmptyInputLocation;
}

void wxMaskedTextCtrl::UpdateControl(int nSelectionStart)
{
	// Update the edit control if it exists.
	wxString sText = ShowMask();
	wxTextCtrl::SetValue(sText);
	// We usually need to update the insertion point.
	if(nSelectionStart >= 0)
		UpdateInsertionPointForward(nSelectionStart);
}

void wxMaskedTextCtrl::UpdateInsertionPointForward(int nSelectionStart)
{
	int nNewInsertionPoint=GetNextInputLocation(nSelectionStart);

	if(m_bAutoTab && (unsigned int) nNewInsertionPoint == m_listData.GetCount())
	{
		wxNavigationKeyEvent eventNav;
		eventNav.SetDirection(TRUE);
		eventNav.SetWindowChange(FALSE);
		eventNav.SetEventObject(this);

		GetParent()->GetEventHandler()->ProcessEvent(eventNav);
	}
	else
	{
		SetSelection(nNewInsertionPoint, nNewInsertionPoint);
	}
}

void wxMaskedTextCtrl::UpdateInsertionPointBackward(int nSelectionStart)
{
	int nNewInsertionPoint=GetPreviousInputLocation(nSelectionStart);
	SetSelection(nNewInsertionPoint, nNewInsertionPoint);
}

void wxMaskedTextCtrl::ValidationError()
{
	wxBell();
}


void wxMaskedTextCtrl::OnKeyDown(wxKeyEvent& event)
{
	// If there is no mask, then exit quickly performing the default operation.
	long keycode = event.GetKeyCode();

	if(m_listData.GetCount()== 0)
	{
		event.Skip();
		return;
	}

	// Keep the OnKeyDown processing to a minimum.  This is because the edit
	// control does lots of processing before OnChar() is sent and we want
	// to let it continue.
	bool bIsShiftKeyDown=event.ShiftDown();
	if(keycode == WXK_DELETE)
	{
		long nSelectionStart = 0;
		long nSelectionEnd = 0;
		GetSelection(&nSelectionStart, &nSelectionEnd);

		// Delete has two functions, it can delete the selection and
		// it can delete characters to the right.
		if(nSelectionStart==nSelectionEnd)
		{
			nSelectionEnd++; // Do the equivalent of a selection.
			if(DeleteRange(nSelectionStart, nSelectionEnd))
			{
				UpdateControl(nSelectionStart);
			}
			else	// Must be on a literal, so continue moving to right
					// and re-attempting the delete until we either delete
					// a character or run out of characters.
			{
				while (nSelectionEnd != (long) m_listData.GetCount())
				{
					nSelectionStart++;
					nSelectionEnd++; // Do the equivalent of a selection.
					if(DeleteRange(nSelectionStart, nSelectionEnd))
					{
						UpdateControl(nSelectionStart);
						break;
					}
				}
			}
		}
		else if(DeleteRange(nSelectionStart, nSelectionEnd))
		{
			UpdateControl(nSelectionStart);
		}
		else	// Must be on a literal, so continue moving to right
				// and reattempting the delete until we either delete
				// a character or run out of characters.
		{
			while (nSelectionEnd != (long) m_listData.GetCount())
			{
				nSelectionStart++;
				nSelectionEnd++; // Do the equivalent of a selection.
				if(DeleteRange(nSelectionStart, nSelectionEnd))
				{
					UpdateControl(nSelectionStart);
					break;
				}
			}
		}
	}
	else if(keycode == WXK_HOME && !bIsShiftKeyDown)
	{
		// If the shift key is not down, then HOME is a navigation and we need to
		// move the insertion point to the first available position.

		UpdateInsertionPointForward(0);
	}
	else if(keycode == WXK_LEFT && !bIsShiftKeyDown)
	{
		// If the shift key is not down, then LEFT is a navigation and we need to
		// move the insertion point to the previous available position.
		long nSelectionStart = 0;
		long nSelectionEnd = 0;
		GetSelection(&nSelectionStart, &nSelectionEnd);

		UpdateInsertionPointBackward(nSelectionStart-1);
	}
	else if(keycode == WXK_UP && !bIsShiftKeyDown)
	{
		// If the shift key is not down, then UP is a navigation and we need to
		// move the insertion point to the previous available position.
		long nSelectionStart = 0;
		long nSelectionEnd = 0;
		GetSelection(&nSelectionStart, &nSelectionEnd);

		UpdateInsertionPointBackward(nSelectionStart-1);
	}
	else if(keycode == WXK_RIGHT && !bIsShiftKeyDown)
	{
		// If the shift key is not down, then RIGHT is a navigation and we need to
		// move the insertion point to the next available position.
		long nSelectionStart = 0;
		long nSelectionEnd = 0;
		GetSelection(&nSelectionStart, &nSelectionEnd);

		UpdateInsertionPointForward(nSelectionStart+1);
	}
	else if(keycode == WXK_DOWN && !bIsShiftKeyDown)
	{
		// If the shift key is not down, then DOWN is a navigation and we need to
		// move the insertion point to the next available position.
		long nSelectionStart = 0;
		long nSelectionEnd = 0;
		GetSelection(&nSelectionStart, &nSelectionEnd);

		UpdateInsertionPointForward(nSelectionStart+1);
	}
	else if(keycode == WXK_INSERT)
	{
		// The standard CEdit control does not support over-typing.
		// This flag is used to manage over-typing internally.
		SetInsertMode(!GetInsertMode());
	}
	else
		event.Skip();
}

void wxMaskedTextCtrl::OnChar(wxKeyEvent& event)
{
	long keycode = event.GetKeyCode();
	// If there is no mask, then exit quickly performing the default operation.
	if(m_listData.GetCount()==0)
	{
		wxTextCtrl::OnChar(event);
		return;
	}

	long nSelectionStart = 0;
	long nSelectionEnd = 0;
	GetSelection(&nSelectionStart, &nSelectionEnd);

	// If character value is above 32, then it is ANSI or Extended.
	// Below 32 are control and navigation characters.
	if(keycode >= 32 && wxIsprint((int) keycode))
	{
		if(nSelectionStart == nSelectionEnd)
		{
			if(IsInputData(nSelectionStart))
			{
				int nActualInsertionPoint=nSelectionStart;
				if(m_bInsertMode)
					nActualInsertionPoint=InsertAt(nSelectionStart, (wxChar) keycode);
				else
					nActualInsertionPoint=SetAt(nSelectionStart, (wxChar) keycode);

				// InsertAt will return -1 if the character cannot be inserted here.
				if(nActualInsertionPoint >= 0)
					nSelectionStart=nActualInsertionPoint + 1;
				else
					ValidationError();

				UpdateControl(nSelectionStart);
			}
			else
			{
				// Beep if trying to type over a literal.
				ValidationError();
				UpdateInsertionPointForward(nSelectionStart);
			}
		}
		else
		{
			// First delete the remaining selection.
			// The function will return a valid count if
			// some input characters were deleted. We use
			// this value to determine if it makes sense to insert.
			if(DeleteRange(nSelectionStart, nSelectionEnd))
			{
				// InsertAt will place the character at the next available position,
				// then return that positition
				int nActualInsertionPoint=nSelectionStart;
				nActualInsertionPoint=InsertAt(nSelectionStart, (wxChar) keycode);

				// InsertAt will return -1 if the character cannot be inserted here.
				if(nActualInsertionPoint >= 0)
					nSelectionStart=nActualInsertionPoint + 1;
				else
					ValidationError();

				UpdateControl(nSelectionStart);
			}
			else  // Must be on a literal, so beep and move to a valid location.
			{
				ValidationError();
				UpdateInsertionPointForward(nSelectionStart);
			}
		}
	}
	else
	{
		if(keycode==WXK_BACK)
		{
			// Backspace performs two functions, if there is a selection,
			// then the backspace is the same as deleting the selection.
			// If there is no selection, then the backspace deletes the
			// first non-literal character to the left.
			if(nSelectionStart==nSelectionEnd)
			{
				if (nSelectionStart >= 1)
				{
					while (nSelectionStart >= 0)
					{
						nSelectionStart--; // Do the equivalent of a backspace.

						if (DeleteRange(nSelectionStart, nSelectionEnd))
						{
							UpdateControl(nSelectionStart);
							break;
						}

						nSelectionEnd--;
					}
				}
			}
			else if(DeleteRange(nSelectionStart, nSelectionEnd))
			{
				UpdateControl(nSelectionStart);
			}
			else	// Must be on a literal, so continue moving to left
					// and re-attempting the delete until we either delete
					// a character or run out of characters.
			{
				if (nSelectionStart >= 1)
				{
					while (nSelectionStart >= 0)
					{
						nSelectionStart--; // Do the equivalent of a backspace.

						if (DeleteRange(nSelectionStart, nSelectionEnd))
						{
							UpdateControl(nSelectionStart);
							break;
						}
						nSelectionEnd--;
					}
				}
			}
		}
		else
			// let edit control to do its job
			event.Skip();
	}
}

void wxMaskedTextCtrl::OnSetFocus(wxFocusEvent& /*event*/)
{
	wxTextCtrl::SetFocus();
	// The default behavior is to highlight the entire string.
	// If this is the case, then move the insertion to the first input position.
	long nSelectionStart = 0;
	long nSelectionEnd = 0;
	GetSelection(&nSelectionStart, &nSelectionEnd);
	if((nSelectionStart == 0) && ((unsigned long) nSelectionEnd == GetValue().Length()))
	{
		// Only update the insertion point if the entire string is selected.
		// This will allow the mouse to be used to set the cursor without our interfering.
		UpdateInsertionPointForward(0);
	}
}

void wxMaskedTextCtrl::OnKillFocus(wxFocusEvent& /*event*/)
{
	Validate();
}


void wxMaskedTextCtrl::OnCut(wxUpdateUIEvent& event)
{
	if(m_listData.GetCount() == 0)
	{
		event.Skip();
		return;
	}

	long nSelectionStart = 0;
	long nSelectionEnd = 0;
	GetSelection(&nSelectionStart, &nSelectionEnd);

	// First do our version of the cut.
    if (nSelectionStart != nSelectionEnd) {
        int nDeleteCount =  DeleteRange(nSelectionStart, nSelectionEnd);
        if (nDeleteCount == 0)
            LOG_DEBUG("wx", "Deletecount zero even though the selection was not empty");
    }

	// Before updating, let the control do its normal thing.
	// This will save us the effort of filling the clipboard.
	wxTextCtrl::OnCut(event);

	// Now we update with our standard mask.
	UpdateControl(nSelectionStart);
/*	if(nDeleteCount==0)
	{
		// I don't think we want to beep if no input characters were cut.
		//ValidationError();
	}*/
}

void wxMaskedTextCtrl::OnCopy(wxUpdateUIEvent& event)
{
	// Just let copy do its thing and copy the selected text.
	event.Skip();
}

void wxMaskedTextCtrl::OnPaste(wxUpdateUIEvent& event)
{
	if(m_listData.GetCount() == 0)
	{
		event.Skip();
		return;
	}

	long nSelectionStart = 0;
	long nSelectionEnd = 0;
	GetSelection(&nSelectionStart, &nSelectionEnd);

	wxTextCtrl::OnPaste(event);


	// This is a real dump paste routine that expects SetInputData do
	// do the filtering.  There is probably no easy solution to this
	// task because anything can be pasted.  We could try and match
	// the literals, but maybe we will get to that later.
	wxString csNewString = GetValue();

	// It is very important that we do not allow the prompt character
	// in this scenario.  This is because we expect the pasted text
	// to contain lots of literals and spaces.
	SetInputData(csNewString, 0, FALSE);
	UpdateControl(-1);
	// Setting the insertion point after a paste is tricky because the
	// expected location is after the last valid pasted character.
	// Try and determine this location by setting the insertion point
	// to the first empty location after the specified starting point.
	int nNewInsertionPoint = GetEmptyInputLocation(nSelectionStart);
	SetSelection(nNewInsertionPoint, nNewInsertionPoint);
}

void wxMaskedTextCtrl::Clear()
{
	if(m_listData.GetCount() == 0)
		wxTextCtrl::Clear();
	else
	{
		long nSelectionStart = 0;
		long nSelectionEnd = 0;
		GetSelection(&nSelectionStart, &nSelectionEnd);

                // First do our version of the cut.
        if (nSelectionStart != nSelectionEnd) {
            int nDeleteCount = DeleteRange(nSelectionStart, nSelectionEnd);

            // if the selection is not empty there should be some debug message
            if (nDeleteCount != 0)
                LOG_DEBUG("wx", "Deletecount zero even though the selection was not empty");
        }

		// Now we update with our standard mask.
		UpdateControl(nSelectionStart);
	}
}

bool wxMaskedTextCtrl::Validate()
{
	int day = 0, month = 0, year = 0, hour = 0, minute = 0, second = 0;
	bool bDay = FALSE, bMonth = FALSE, bYear = FALSE, bHour = FALSE, bMinute = FALSE, bSecond = FALSE;
	bool DateValid, TimeValid;
	int year_len = 0;
	unsigned long used = 0;
	long StartPosDay = -1, EndPosDay = -1, StartPosMonth = -1, EndPosMonth = -1, StartPosYear = -1, EndPosYear = -1;
	long StartPosHour = -1, EndPosHour = -1, StartPosMinute = -1, EndPosMinute = -1, StartPosSecond = -1, EndPosSecond = -1;

	if(!m_bNeedValidation)
		return m_bValidation;

	wxMaskData* pobjData=NULL;
	m_dtDateTime = wxInvalidDateTime;
	for(unsigned long pos = 0; pos < m_listData.GetCount();pos++)
	{
		pobjData= (wxMaskData *) (m_listData.Item(pos))->GetData();
		if(pobjData->m_eType == MaskDataTypeDIGIT)
		{
			if(pobjData->m_chValue != m_chPromptSymbol)
				used++;

			wxMaskData* pobjDataNext = NULL;

			switch(pobjData->m_eSubType)
			{
				case MaskDataSubTypeDATEDAY:
					bDay = TRUE;
					if(StartPosDay == -1)
					{
						StartPosDay = pos;
						EndPosDay = pos;
					}
					EndPosDay++;

					pobjDataNext = (wxMaskData *) (pos < (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
					if(pobjData->m_chValue != m_chPromptSymbol)
					{
						day = (day * 10) + (pobjData->m_chValue - 48);

						if(pobjDataNext != NULL)
						{
							if(pobjDataNext->m_eType == MaskDataTypeDIGIT && pobjDataNext->m_eSubType == MaskDataSubTypeDATEDAY)
							{
								day = (day * 10) + (pobjDataNext->m_chValue != m_chPromptSymbol ? (pobjDataNext->m_chValue - 48) : 0);
								EndPosDay++;
								pos++;
							}
						}
					}
					break;

				case MaskDataSubTypeDATEMONTH:
					bMonth = TRUE;
					if(StartPosMonth == -1)
					{
						StartPosMonth = pos;
						EndPosMonth = pos;
					}
					EndPosMonth++;

					pobjDataNext = (wxMaskData *) (pos < (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
					if(pobjData->m_chValue != m_chPromptSymbol)
					{
						month = (month * 10) + (pobjData->m_chValue - 48);

						if(pobjDataNext != NULL)
						{
							if(pobjDataNext->m_eType == MaskDataTypeDIGIT && pobjDataNext->m_eSubType == MaskDataSubTypeDATEMONTH)
							{
								month = (month * 10) + (pobjDataNext->m_chValue != m_chPromptSymbol ? (pobjDataNext->m_chValue - 48) : 0);
								EndPosMonth++;
								pos++;
							}
						}
					}
					break;

				case MaskDataSubTypeDATEYEAR:
					bYear = TRUE;
					if(StartPosYear == -1)
					{
						StartPosYear = pos;
						EndPosYear = pos;
					}
					EndPosYear++;

					if(pobjData->m_chValue != m_chPromptSymbol)
					{
						year = (year * 10) + (pobjData->m_chValue - 48);
						year_len++;
					}
					break;

				case MaskDataSubTypeTIMEHOUR:
					bHour = TRUE;
					if(StartPosHour == -1)
					{
						StartPosHour = pos;
						EndPosHour = pos;
					}
					EndPosHour++;

					pobjDataNext = (wxMaskData *) (pos < (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
					if(pobjData->m_chValue != m_chPromptSymbol)
					{
						hour = (hour * 10) + (pobjData->m_chValue - 48);

						if(pobjDataNext != NULL)
						{
							if(pobjDataNext->m_eType == MaskDataTypeDIGIT && pobjDataNext->m_eSubType == MaskDataSubTypeTIMEHOUR)
							{
								hour = (hour * 10) + (pobjDataNext->m_chValue != m_chPromptSymbol ? (pobjDataNext->m_chValue - 48) : 0);
								EndPosHour++;
								pos++;
							}
						}
					}
					break;

				case MaskDataSubTypeTIMEMINUTE:
					bMinute = TRUE;
					if(StartPosMinute == -1)
					{
						StartPosMinute = pos;
						EndPosMinute = pos;
					}
					EndPosMinute++;

					pobjDataNext = (wxMaskData *) (pos < (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
					if(pobjData->m_chValue != m_chPromptSymbol)
					{

						minute = (minute * 10) + (pobjData->m_chValue - 48);

						if(pobjDataNext != NULL)
						{
							if(pobjDataNext->m_eType == MaskDataTypeDIGIT && pobjDataNext->m_eSubType == MaskDataSubTypeTIMEMINUTE)
							{
								minute = (minute * 10) + (pobjDataNext->m_chValue != m_chPromptSymbol ? (pobjDataNext->m_chValue - 48) : 0);
								EndPosMinute++;
								pos++;
							}
						}
					}
					break;

				case MaskDataSubTypeTIMESECOND:
					bSecond = TRUE;
					if(StartPosSecond == -1)
					{
						StartPosSecond = pos;
						EndPosSecond = pos;
					}
					EndPosSecond++;

					pobjDataNext = (wxMaskData *) (pos < (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
					if(pobjData->m_chValue != m_chPromptSymbol)
					{
						second = (second * 10) + (pobjData->m_chValue - 48);

						if(pobjDataNext != NULL)
						{
							if(pobjDataNext->m_eType == MaskDataTypeDIGIT && pobjDataNext->m_eSubType == MaskDataSubTypeTIMESECOND)
							{
								second = (second * 10) + (pobjDataNext->m_chValue != m_chPromptSymbol ? (pobjDataNext->m_chValue - 48) : 0);
								EndPosSecond++;
								pos++;
							}
						}
					}
					break;
				default:
					break;
			}
		}
	}

	m_bNeedValidation = FALSE;

	if(used == 0)
		return (m_bValidation = TRUE);

	bool minValid = m_dtMinDateTime.IsValid();
	bool maxValid = m_dtMaxDateTime.IsValid();

	DateValid = TRUE;
	if(bDay || bMonth || bYear)
	{
		long pos;
		wxDateTime minDate = m_dtMinDateTime;
		wxDateTime maxDate = m_dtMaxDateTime;
		wxDateTime Date = wxDateTime(1, wxDateTime::Jan, (wxDateTime::Now()).GetYear(), 0, 0, 0, 0);

		if((bDay && day == 0) || (bMonth && month == 0))
			DateValid = FALSE;
		else if((bDay && day > 31) || (bMonth && month > 12))
			DateValid = FALSE;

		if(DateValid)
		{
			if(bYear && year_len < 3 && year < 100)
			{
				if(year_len == 0)
				{
					year = (wxDateTime::Now()).GetYear();
					year_len = 1;
				}
				else
					year += ((wxDateTime::Now()).GetYear() / 1000) * 1000;
			}

			if(bYear)
			{
				if(StartPosYear != -1)
				{
					wxString Year_Str = wxString::Format(wxT("%04d"), year);

					for(pos = StartPosYear; pos < EndPosYear;pos++)
						SetAt(pos, Year_Str[(int) (Year_Str.Length() + pos - EndPosYear)]);
				}

				Date.SetYear(year);

				if(minValid)
				{
					minDate.SetMonth(wxDateTime::Jan);
					minDate.SetDay(1);
				}

				if(maxValid)
				{
					maxDate.SetMonth(wxDateTime::Dec);
					maxDate.SetDay(31);
				}
			}
			else
			{
				if(minValid)
					minDate.SetYear(Date.GetYear());

				if(maxValid)
					maxDate.SetYear(Date.GetYear());
			}

			if(bMonth)
			{
				if(StartPosMonth != -1)
				{
					wxString Month_Str = wxString::Format(wxT("%02d"), month);

					for(pos = StartPosMonth; pos < EndPosMonth;pos++)
						SetAt(pos, Month_Str[(int) (pos - StartPosMonth)]);
				}

				Date.SetMonth(wxDateTime::Month(month-1));

				if(minValid)
					minDate.SetMonth(m_dtMinDateTime.GetMonth());

				if(maxValid)
				{
					maxDate.SetDay(1);
					maxDate.SetMonth(m_dtMaxDateTime.GetMonth());
					maxDate.SetDay(m_dtMaxDateTime.GetNumberOfDays(m_dtMaxDateTime.GetMonth(), m_dtMaxDateTime.GetYear()));
				}
			}
			else
			{
				if(minValid)
					minDate.SetMonth(m_dtMinDateTime.GetMonth());

				if(maxValid)
					maxDate.SetMonth(m_dtMaxDateTime.GetMonth());
			}

			if(bDay)
			{
				if(StartPosDay != -1)
				{
					wxString Day_Str = wxString::Format(wxT("%02d"), day);

					for(pos = StartPosDay; pos < EndPosDay;pos++)
						SetAt(pos, Day_Str[(int) (pos - StartPosDay)]);
				}

				if(day > Date.GetNumberOfDays(Date.GetMonth(), Date.GetYear()))
					DateValid = FALSE;

				if(DateValid)
				{
					Date.SetDay(day);

					if(minValid)
						minDate.SetDay(m_dtMinDateTime.GetDay());

					if(maxValid)
						maxDate.SetDay(m_dtMaxDateTime.GetDay());
				}
			}
			else
			{
				if(minValid)
					minDate.SetDay(1);

				if(maxValid)
					maxDate.SetDay(m_dtMaxDateTime.GetNumberOfDays(m_dtMaxDateTime.GetMonth(), m_dtMaxDateTime.GetYear()));
			}

			if(DateValid)
			{
				if(!Date.IsValid())
					DateValid = FALSE;

				if(minValid && m_bValidation)
				{
					if(Date.IsEarlierThan(minDate))
						DateValid = FALSE;
				}

				if(maxValid && m_bValidation)
				{
					if(Date.IsLaterThan(maxDate))
						DateValid = FALSE;
				}

				m_dtDateTime = Date;
			}
		}
	}

	TimeValid = TRUE;
	if(bHour || bMinute || bSecond)
	{
		long pos;
		wxDateTime minDate = m_dtMinDateTime;
		wxDateTime maxDate = m_dtMaxDateTime;
		wxDateTime Date;

		if(m_dtDateTime.IsValid())
			Date = m_dtDateTime;
		else
			Date = wxDateTime(1, wxDateTime::Jan, (wxDateTime::Now()).GetYear(), 0, 0, 0, 0);

		if((bHour && hour > 23) || (bMinute && minute > 59) || (bSecond && second > 59))
			TimeValid = FALSE;

		if(TimeValid)
		{
			if(bHour)
			{
				if(StartPosHour != -1)
				{
					wxString Hour_Str = wxString::Format(wxT("%02d"), hour);

					for(pos = StartPosHour; pos < EndPosHour;pos++)
						SetAt(pos, Hour_Str[(int) (pos - StartPosHour)]);
				}

				Date.SetHour(hour);
			}
			else
			{
				if(minValid)
					minDate.SetHour(0);

				if(maxValid)
					maxDate.SetHour(1);
			}

			if(bMinute)
			{
				if(StartPosMinute != -1)
				{
					wxString Minute_Str = wxString::Format(wxT("%02d"), minute);

					for(pos = StartPosMinute; pos < EndPosMinute;pos++)
						SetAt(pos, Minute_Str[(int) (pos - StartPosMinute)]);
				}

				Date.SetMinute(minute);
			}
			else
			{
				if(minValid)
					minDate.SetMinute(0);

				if(maxValid)
					maxDate.SetMinute(59);
			}

			if(bSecond)
			{
				if(StartPosSecond != -1)
				{
					wxString Second_Str = wxString::Format(wxT("%02d"), second);

					for(pos = StartPosSecond; pos < EndPosSecond;pos++)
						SetAt(pos, Second_Str[(int) (pos - StartPosSecond)]);
				}

				Date.SetSecond(second);
			}
			else
			{
				if(minValid)
					minDate.SetSecond(0);

				if(maxValid)
					maxDate.SetSecond(59);
			}

			if(!Date.IsValid())
				TimeValid = FALSE;

			if(TimeValid)
			{
				if(minValid)
				{
					if(Date.IsEarlierThan(minDate))
						TimeValid = FALSE;
				}

				if(maxValid)
				{
					if(Date.IsLaterThan(maxDate))
						TimeValid = FALSE;
				}

				m_dtDateTime.Set(hour, minute, second);
			}
		}
	}

	UpdateControl(-1);

	m_bValidation = DateValid & TimeValid;
	m_bNeedValidation = FALSE;

	return m_bValidation;
}

bool wxMaskedTextCtrl::IsValid(void)
{
	if(m_bNeedValidation)
		Validate();

	return m_bValidation;
}

void wxMaskedTextCtrl::OnLeftButtonDown(wxMouseEvent& event)
{
	if(FindFocus()!= this && IsInputEmpty())
	{
		SetFocus();
		UpdateInsertionPointForward(0);
	}
	else
		event.Skip();
}

bool wxMaskedTextCtrl::IsEmpty(void) const
{
	return IsInputEmpty();
}

wxDateTime wxMaskedTextCtrl::GetMinDateTime(void)
{
	return m_dtMinDateTime;
}

void wxMaskedTextCtrl::SetMinDateTime(wxDateTime& minDateTime)
{
	m_dtMinDateTime = minDateTime;
}

wxDateTime wxMaskedTextCtrl::GetMaxDateTime(void)
{
	return m_dtMaxDateTime;
}

void wxMaskedTextCtrl::SetMaxDateTime(wxDateTime& maxDateTime)
{
	m_dtMaxDateTime = maxDateTime;
}

void wxMaskedTextCtrl::SetDateTimeRange(wxDateTime& minDateTime, wxDateTime& maxDateTime)
{
	m_dtMinDateTime = minDateTime;
	m_dtMaxDateTime = maxDateTime;
}

void wxMaskedTextCtrl::SetValue(const wxString& value)
{
	if(m_listData.GetCount() == 0)
		wxTextCtrl::SetValue(value);
	else
		SetInputData(value, 0, TRUE);
}

void wxMaskedTextCtrl::SetDateTimeValue(wxDateTime& value)
{
	wxMaskData *pobjData = NULL, *pobjDataNext = NULL;
	wxString str_Value;
	long length;
	int year_len;

	if(m_listData.GetCount() == 0 && value.IsValid())
		return;

	for(long pos = 0;pos < (long) m_listData.GetCount();pos++)
	{
		pobjData = (wxMaskData *) (m_listData.Item(pos))->GetData();
		if(pobjData->m_eType == MaskDataTypeDIGIT)
		{
			if(pobjData->m_eSubType == MaskDataSubTypeDATEDAY)
			{
				pobjDataNext = (wxMaskData *) (pos < (long) (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
				if(pobjDataNext)
				{
					if(pobjData->m_eType == MaskDataTypeDIGIT && pobjData->m_eSubType == MaskDataSubTypeDATEDAY)
						str_Value = wxString::Format(wxT("%02d"), value.GetDay());
					else
						str_Value = wxString::Format(wxT("%d"), value.GetDay() % 10);
				}

			}
			else if(pobjData->m_eSubType == MaskDataSubTypeDATEMONTH)
			{
				pobjDataNext = (wxMaskData *) (pos < (long) (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
				if(pobjDataNext)
				{
					if(pobjData->m_eType == MaskDataTypeDIGIT && pobjData->m_eSubType == MaskDataSubTypeDATEMONTH)
						str_Value = wxString::Format(wxT("%02d"), value.GetMonth());
					else
						str_Value = wxString::Format(wxT("%d"), value.GetMonth() % 10);
				}
			}
			else if(pobjData->m_eSubType == MaskDataSubTypeDATEYEAR)
			{
				year_len = 1;
				for(long i = pos; i < (long) m_listData.GetCount();i++)
				{
					pobjDataNext = (wxMaskData *) (m_listData.Item(pos+1))->GetData();
					if(pobjDataNext->m_eType == MaskDataTypeDIGIT && pobjDataNext->m_eSubType == MaskDataSubTypeDATEYEAR)
						year_len++;
					else
						break;
				}
				str_Value = (wxString::Format(wxT("%04d"), value.GetYear())).Right(year_len);;
			}
			else if(pobjData->m_eSubType == MaskDataSubTypeTIMEHOUR)
			{
				pobjDataNext = (wxMaskData *) (pos < (long) (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
				if(pobjDataNext)
				{
					if(pobjData->m_eType == MaskDataTypeDIGIT && pobjData->m_eSubType == MaskDataSubTypeTIMEHOUR)
						str_Value = wxString::Format(wxT("%02d"), value.GetHour());
					else
						str_Value = wxString::Format(wxT("%d"), value.GetHour() % 10);
				}
			}
			else if(pobjData->m_eSubType == MaskDataSubTypeTIMEMINUTE)
			{
				pobjDataNext = (wxMaskData *) (pos < (long) (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
				if(pobjDataNext)
				{
					if(pobjData->m_eType == MaskDataTypeDIGIT && pobjData->m_eSubType == MaskDataSubTypeTIMEMINUTE)
						str_Value = wxString::Format(wxT("%02d"), value.GetMinute());
					else
						str_Value = wxString::Format(wxT("%d"), value.GetMinute() % 10);
				}
			}
			else if(pobjData->m_eSubType == MaskDataSubTypeTIMESECOND)
			{
				pobjDataNext = (wxMaskData *) (pos < (long) (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
				if(pobjDataNext)
				{
					if(pobjData->m_eType == MaskDataTypeDIGIT && pobjData->m_eSubType == MaskDataSubTypeTIMESECOND)
						str_Value = wxString::Format(wxT("%02d"), value.GetSecond());
					else
						str_Value = wxString::Format(wxT("%d"), value.GetSecond() % 10);
				}
			}

			length = str_Value.Length();
			if(pos + length > (long) m_listData.GetCount())
				length = (long) str_Value.Length() - pos;

			for(long i = pos; pos < (long) (i + length);pos++)
				SetAt(pos, str_Value[(int) (pos-i)]);
		}
	}
	UpdateControl();
}

wxDateTime wxMaskedTextCtrl::GetDateTimeValue()
{
	if(m_bNeedValidation)
		Validate();

	return m_dtDateTime;
}

int wxMaskedTextCtrl::RPtoLP(int nRealPos)
{
	// All wxMaskedTextCtrl functions that take cursor position as argument interpret it
	// as real position within edit control (taking into account all symbols including
	// literals). But sometimes we want to know which non-literal symbol is at
	// particular real position. In that case this function is really useful

	if(nRealPos < 0 || nRealPos >= (int) m_listData.GetCount())
		return -1;

	int nLogicalPos = -1;
	wxMaskData* pobjData = NULL;
	int nNextInputLocation = 0;
	for(unsigned long pos = nNextInputLocation;pos < m_listData.GetCount();pos++, nNextInputLocation++)
	{
		pobjData = (wxMaskData *) (m_listData.Item(pos))->GetData();
		if(pobjData->IsInputData())
		{
			nLogicalPos++;
		}
		if(nNextInputLocation == nRealPos)
		{
			return pobjData->IsInputData() ? nLogicalPos : -1;
		}
	}

	return -1;
}

int wxMaskedTextCtrl::LPtoRP(int nLogicalPos)
{
	// All wxMaskedTextCtrl functions that take cursor position as argument interpret it
	// as real position within edit control (taking into account all symbols including
	// literals). But sometimes we want to set cursor at position before or after
	// particular non-literal symbol. In that case this function is really useful

	if(nLogicalPos < 0 || nLogicalPos >= (int) m_listData.GetCount())
		return -1;

	int nRealPos = -1;
	int nNonLiterals = -1;
	wxMaskData* pobjData = NULL;
	int nNextInputLocation = 0;
	for(unsigned long pos = nNextInputLocation; pos < m_listData.GetCount();pos++, nNextInputLocation++)
	{
		pobjData= (wxMaskData *) (m_listData.Item(pos))->GetData();
		nRealPos++;
		if(pobjData->IsInputData())
		{
			nNonLiterals++;
			if(nNonLiterals == nLogicalPos)
			{
				return nRealPos;
			}
		}
	}

	return -1;
}
