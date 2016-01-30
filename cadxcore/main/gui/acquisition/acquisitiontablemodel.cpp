/*
 *  
 *  $Id: openwithdialog.h $
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
#include "acquisitiontablemodel.h"
#include <main/controllers/controladorlog.h>
#include <api/dicom/dcmdictionary.h>
#include <set>
#include <wx/imaglist.h>
#include <resources/ginkgoresourcesmanager.h>
#include <api/dicom/imodelodicom.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/storedqueriescontroller.h>
#include <main/controllers/controladoreventos.h>
#include <commands/pacsdownloadcommand.h>
#include <commands/comandopacs.h>
#include <eventos/progresocomando.h>

#define ICONO_ESTUDIO 0
#define ICONO_SERIE 1
#define ICONO_HOMBRE 2
#define ICONO_MUJER 3
#define ICONO_OTRO 4

#define SIZE_ICONOS 16

#define LOGGER "ACQUISITIONTABLEMODEL"

GNC::GUI::AcquisitionNode::AcquisitionNode(AcquisitionNode* parent, const GNC::GCS::Ptr<GIL::DICOM::DicomDataset> info):
        pData(info), 
        pParent(parent)
	
{
	if (IsSeriesNode()) {
		uid = pData->getTag(GKDCM_SeriesInstanceUID);
		SetDateTime(pData->getTag(GKDCM_SeriesDate),pData->getTag(GKDCM_SeriesTime));
	} else {
		uid = pData->getTag(GKDCM_StudyInstanceUID);
		SetDateTime(pData->getTag(GKDCM_StudyDate),pData->getTag(GKDCM_StudyTime));
	}
	if (uid.empty()) {
		LOG_ERROR(LOGGER, "trying to insert a node without uid");
	}
}

GNC::GUI::AcquisitionNode::~AcquisitionNode()
{
	DestroyChildren();
}

bool GNC::GUI::AcquisitionNode::IsSeriesNode()
{
	return pParent != NULL;
}

bool GNC::GUI::AcquisitionNode::IsStudyNode()
{
	return pParent == NULL;
}

void GNC::GUI::AcquisitionNode::AddChild(AcquisitionNode* pChild)
{
	seriesMap[pChild->uid] = pChild;
}

const GNC::GCS::Ptr<GIL::DICOM::DicomDataset>& GNC::GUI::AcquisitionNode::GetData()
{
	return pData;
}

void GNC::GUI::AcquisitionNode::DestroyChildren()
{
	for (TMapIndex::iterator it = seriesMap.begin(); it !=  seriesMap.end(); ++it) {
		delete (*it).second;
	}
	seriesMap.clear();
}


GNC::GUI::AcquisitionNode* GNC::GUI::AcquisitionNode::GetParent()
{
	return pParent;
}


unsigned int GNC::GUI::AcquisitionNode::GetChildren(wxDataViewItemArray &array ) 
{
	if (IsSeriesNode()) {
		return 0;
	} else {
		for (TMapIndex::const_iterator it = seriesMap.begin(); it != seriesMap.end(); ++it) {
			array.Add( wxDataViewItem( (void*) (*it).second) );
		}
		return seriesMap.size();
	}
}

void GNC::GUI::AcquisitionNode::SetDateTime(const std::string& strDate, const std::string& strTime)
{
	wxDateTime fecha;
	wxDateTime hora;
	hora.ParseFormat( wxString::FromUTF8( strTime.c_str() ).GetData(), wxT("%H%M%S"), wxDefaultDateTime);
	fecha.ParseFormat( wxString::FromUTF8( strDate.c_str() ).GetData(), wxT("%Y%m%d"), wxDefaultDateTime);
	if(fecha.IsValid()){
		if (hora.IsValid()) {

			fecha.SetHour(hora.GetHour());
			fecha.SetMinute(hora.GetMinute());
			fecha.SetSecond(hora.GetSecond());

			datetime = fecha.Format(_("%m/%d/%Y %H:%M:%S"), wxDateTime::TimeZone(wxDateTime::GMT1)).ToUTF8();
		}
		else {
			datetime = fecha.Format(_("%m/%d/%Y 00:00:00"), wxDateTime::TimeZone(wxDateTime::GMT1)).ToUTF8();
		}
	} else {
		datetime = wxString(wxT("00/00/0000 00:00:00")).ToUTF8();;
	}
}


/////////////////////////////
GNC::GUI::AcquisitionTableModel::AcquisitionTableModel():
        m_pComandoPACS(NULL), 
        m_Mode(MM_SERIES)
{
	m_pImageList = new wxImageList(SIZE_ICONOS,SIZE_ICONOS,true);
	m_pImageList->Add(GinkgoResourcesManager::History::GetIcoEstudio());
	m_pImageList->Add(GinkgoResourcesManager::History::GetIcoSerie());
	m_pImageList->Add(GinkgoResourcesManager::History::GetIcoHombre());
	m_pImageList->Add(GinkgoResourcesManager::History::GetIcoMujer());
	m_pImageList->Add(GinkgoResourcesManager::History::GetIcoOtro());
	GNC::GCS::ControladorEventos::Instance()->Registrar(this, GNC::GCS::Events::EventoProgresoComando());
}


GNC::GUI::AcquisitionTableModel::~AcquisitionTableModel()
{
	ClearStudyMap();
	delete m_pImageList;
}

unsigned int GNC::GUI::AcquisitionTableModel::GetColumnCount() const
{
	return COL_ACQUISITION_MAX;
}


wxString GNC::GUI::AcquisitionTableModel::GetColumnType( unsigned int col ) const
{
	switch (col) {
	case COL_ICON:
		return wxT("wxIcon");
	default:
		return wxT("string");
	}
}

void GNC::GUI::AcquisitionTableModel::GetValue( wxVariant &variant, const wxDataViewItem &item, unsigned int col ) const
{
	if (!item.IsOk()) {
		return;
	}

	AcquisitionNode* pNode = (AcquisitionNode*)(item.GetID());
	if (pNode->IsSeriesNode()) {
		switch (col) {
			case COL_ICON:
				{
					variant << m_pImageList->GetIcon(ICONO_SERIE);
				}
				break;
			case COL_PATIENT_NAME:
				{
					//nothing to do
				}
				break;
			case COL_PATIENT_ID:
				{
					//nothing to do 
				}
				break;
			case COL_DESCRIPTION:
				variant = wxString::FromUTF8(pNode->GetData()->getTag(GKDCM_SeriesDescription).c_str());
				break;
			case COL_MODALITY:
				variant = wxString::FromUTF8(pNode->GetData()->getTag(GKDCM_Modality).c_str());
				break;
			case COL_DATETIME:
				{
					variant = wxString::FromUTF8(pNode->datetime.c_str());
				}
				break;
			case COL_ACCNUMBER:
				//nothing to do
				break;
			default:
				LOG_ERROR("AcquisitionTableModel", "wrong column id " << col);
				break;
		}
	} else {
		switch (col) {
			case COL_ICON:
				{
					int icono = 0;
					if(pNode->GetData()->getTag(GKDCM_PatientSex) == "M") {
						icono = ICONO_HOMBRE;
					} else if(pNode->GetData()->getTag(GKDCM_PatientSex) == "F") {
						icono = ICONO_MUJER;
					} else {
						icono = ICONO_OTRO;
					}
					variant << m_pImageList->GetIcon(icono);
				}
				break;
			case COL_PATIENT_NAME:
				variant = wxString::FromUTF8(pNode->GetData()->getTag(GKDCM_PatientName).c_str());
				break;
			case COL_PATIENT_ID:
				variant = wxString::FromUTF8(pNode->GetData()->getTag(GKDCM_PatientID).c_str());
				break;
			case COL_DESCRIPTION:
				variant = wxString::FromUTF8(pNode->GetData()->getTag(GKDCM_StudyDescription).c_str());
				break;
			case COL_MODALITY:
				variant = wxString::FromUTF8(pNode->GetData()->getTag(GKDCM_ModalitiesInStudy).c_str());
				break;
			case COL_DATETIME:
				{
					variant = wxString::FromUTF8(pNode->datetime.c_str());
				}
				break;
			case COL_ACCNUMBER:
				variant = wxString::FromUTF8(pNode->GetData()->getTag(GKDCM_AccessionNumber).c_str());
				break;
			default:
				LOG_ERROR("AcquisitionTableModel", "wrong column id " << col);
				break;
		}
	}
	
}



bool GNC::GUI::AcquisitionTableModel::SetValue( const wxVariant &/*variant*/,const wxDataViewItem &/*item*/, unsigned int /*col*/ )
{
	//model cant be modified...
	return false;
}

bool GNC::GUI::AcquisitionTableModel::IsEnabled( const wxDataViewItem &/*item*/, unsigned int/* col*/ ) const
{
	return true;
}

wxDataViewItem GNC::GUI::AcquisitionTableModel::GetParent( const wxDataViewItem &item ) const
{
	if (!item.IsOk()) {
		return wxDataViewItem(0);
	}
	//if it's a study...
	AcquisitionNode* pNode = (AcquisitionNode*)(item.GetID());
	return wxDataViewItem( (void*) pNode->GetParent() );
}

bool GNC::GUI::AcquisitionTableModel::IsContainer( const wxDataViewItem &item ) const
{
	if (!item.IsOk()) {
		return true;
	}
	AcquisitionNode* pNode = (AcquisitionNode*)(item.GetID());
	return m_Mode == MM_SERIES && pNode->IsStudyNode();
}

bool GNC::GUI::AcquisitionTableModel::HasContainerColumns (const wxDataViewItem &/*item*/) const
{
	return true;
}

bool GNC::GUI::AcquisitionTableModel::HasValue (const wxDataViewItem &item, unsigned col) const
{
	if (!item.IsOk()) {
		return false;
	}
	AcquisitionNode* pNode = (AcquisitionNode*)(item.GetID());
	if (pNode->IsStudyNode()) {
		return true;
	} else {
		return col != COL_PATIENT_NAME && col != COL_PATIENT_ID && col != COL_ACCNUMBER;
	}
}


unsigned int GNC::GUI::AcquisitionTableModel::GetChildren( const wxDataViewItem &parent, wxDataViewItemArray &array ) const
{
	AcquisitionNode *node = (AcquisitionNode*) parent.GetID();
    if (!node)
    {
		unsigned int count = studyMap.size();
		for (TMapIndex::const_iterator it = studyMap.begin(); it !=  studyMap.end(); ++it) {
			array.Add( wxDataViewItem( (void*) (*it).second) );
		}
		return count;        
	} else if (node->IsStudyNode()) {
		//return series from study...
		node->GetChildren(array);
		return array.Count();
	}
	return 0;
}

int GNC::GUI::AcquisitionTableModel::Compare( const wxDataViewItem &item1, const wxDataViewItem &item2,
                 unsigned int column, bool ascending ) const
{
	if (column == COL_DATETIME)
    {
        wxVariant value1, value2;
		GetValue( value1, item1, column );
        GetValue( value2, item2, column );

        wxString str1 = value1.GetString();
        wxString str2 = value2.GetString();

		wxDateTime date1,date2;
		date1.ParseFormat(str1, _("%m/%d/%Y %H:%M:%S"));
		date2.ParseFormat(str2, _("%m/%d/%Y %H:%M:%S"));
		int res = 0;
		if (date1.IsValid() && date2.IsValid()) {
			wxTimeSpan diff = date1.Subtract(date2);
			if (diff.IsPositive()) {
				res = 1;
			} else {
				res = -1;
			}
		} else if (date1.IsValid()) {
			res = 1;
		} else if (date2.IsValid()) {
			res =-1;
		} else {
			res = 0;
		}
		if (!ascending) {
			if (res == 1) {
				res = -1;
			} else if (res == -1) {
				res = 1;
			}
		}
		return res;
    }

    return wxDataViewModel::Compare( item1, item2, column, ascending );
}

void GNC::GUI::AcquisitionTableModel::CancelCommand()
{
	if(m_pComandoPACS != NULL)
	{
		GNC::CommandController::Instance()->AbortarComando(m_pComandoPACS, false);
	}
}

void GNC::GUI::AcquisitionTableModel::ClearResults()
{
	CancelCommand();

	wxDataViewItemArray toDeleteList;
	for (TMapIndex::iterator it = studyMap.begin(); it !=  studyMap.end(); ++it) {
		toDeleteList.push_back(wxDataViewItem((*it).second));
	}
	//then delete studies Nodes...		
	ItemsDeleted(wxDataViewItem(0), toDeleteList);

	//delete all and reload again...
	ClearStudyMap();
}

GNC::GUI::AcquisitionTableModel::T_ModelMode GNC::GUI::AcquisitionTableModel::GetModelMode()
{
	return m_Mode;
}

void GNC::GUI::AcquisitionTableModel::SetModelMode(T_ModelMode mode)
{
	m_Mode = mode;
}

void GNC::GUI::AcquisitionTableModel::DoSearch(const GNC::GCS::Ptr<GNC::GCS::StoredQuery>& pQuery)
{
	ClearResults();
	//insert fields that has to be returned...
	pQuery->addCondition(GKDCM_QueryRetrieveLevel,"STUDY");
	pQuery->addConditionIfNotExists(GKDCM_PatientName);
	pQuery->addConditionIfNotExists(GKDCM_PatientID);
	pQuery->addConditionIfNotExists(GKDCM_ModalitiesInStudy);
	pQuery->addConditionIfNotExists(GKDCM_PatientBirthDate);
	pQuery->addConditionIfNotExists(GKDCM_PatientSex);
	pQuery->addConditionIfNotExists(GKDCM_StudyDate);
	pQuery->addConditionIfNotExists(GKDCM_StudyTime);
	pQuery->addConditionIfNotExists(GKDCM_StudyID);
	pQuery->addConditionIfNotExists(GKDCM_StudyInstanceUID);
	pQuery->addConditionIfNotExists(GKDCM_StudyDescription);
	pQuery->addConditionIfNotExists(GKDCM_AccessionNumber);
	pQuery->addConditionIfNotExists(GKDCM_StationName);
	pQuery->addConditionIfNotExists(GKDCM_ReferringPhysicianName);

	GADAPI::ComandoPACSParams * pParams = new GADAPI::ComandoPACSParams(pQuery);
		

	m_pComandoPACS = new GADAPI::ComandoPACS(pParams);
	GNC::CommandController::Instance()->ProcessAsync(_Std("Exploring PACS..."),m_pComandoPACS, this);
}

void GNC::GUI::AcquisitionTableModel::DoExpand(AcquisitionNode* pNode, const std::string& modalities, const std::string& server)
{
	if (pNode->IsSeriesNode()) {
		return;
	}
	wxDataViewItemArray arr;
	pNode->GetChildren(arr);
	if (arr.empty() && !pNode->uid.empty()) {
		GNC::GCS::Ptr<GNC::GCS::StoredQuery> pQuery(new GNC::GCS::StoredQuery());
		pQuery->setPacsId(server);
		//insert fields that has to be returned...
		pQuery->addCondition(GKDCM_QueryRetrieveLevel,"SERIES");
		pQuery->addConditionIfNotExists(GKDCM_StudyInstanceUID, pNode->uid);
		pQuery->addCondition(GKDCM_Modality, modalities);
		pQuery->addConditionIfNotExists(GKDCM_SeriesInstanceUID);
		pQuery->addConditionIfNotExists(GKDCM_SeriesDescription);
		pQuery->addConditionIfNotExists(GKDCM_SeriesDate);
		pQuery->addConditionIfNotExists(GKDCM_SeriesTime);
		pQuery->addConditionIfNotExists(GKDCM_NumberOfSeriesRelatedInstances);		
		pQuery->addConditionIfNotExists(GKDCM_ReferringPhysicianName);
		
		GADAPI::ComandoPACSParams * pParams = new GADAPI::ComandoPACSParams(pQuery);
		m_pComandoPACS = new GADAPI::ComandoPACS(pParams);
		GNC::CommandController::Instance()->ProcessAsync(_Std("Exploring PACS..."),m_pComandoPACS, this);
	}
}

std::string GNC::GUI::AcquisitionTableModel::GetModality(AcquisitionNode* pNode)
{
	std::string mod;
	if (pNode->IsSeriesNode()) {
		mod =pNode->GetData()->getTag(GKDCM_Modality);
	} else {
		mod =pNode->GetData()->getTag(GKDCM_ModalitiesInStudy);
	}
	return mod;
}

void GNC::GUI::AcquisitionTableModel::GetProperties(AcquisitionNode* pNode, GNC::GUI::wxPropiedades::TListaMapasPropiedades& properties)
{
	AcquisitionNode* pStudyNode;
	if (pNode->IsSeriesNode()) {
		pStudyNode = pNode->GetParent();
	} else {
		pStudyNode = pNode;
	}

	const GNC::GCS::Ptr<GIL::DICOM::DicomDataset>& dataSet = pStudyNode->GetData();
	{
		//patient		
		GNC::GUI::wxPropiedades::TMapaPropiedades mapaPropiedades;
		mapaPropiedades[_Std("Patient id")] = dataSet->getTag(GKDCM_PatientID);
		mapaPropiedades[_Std("Patient name")] = dataSet->getTag(GKDCM_PatientName);
		properties.push_back(mapaPropiedades);
	}
	{
		//study
		GNC::GUI::wxPropiedades::TMapaPropiedades mapaPropiedades;
		mapaPropiedades[_Std("Study Instance UID")] = dataSet->getTag(GKDCM_StudyInstanceUID);
		mapaPropiedades[_Std("Accession number")] = dataSet->getTag(GKDCM_AccessionNumber);
		mapaPropiedades[_Std("Study date")] = dataSet->getTag(GKDCM_StudyDate);
		mapaPropiedades[_Std("Study time")] = dataSet->getTag(GKDCM_StudyTime);
		properties.push_back(mapaPropiedades);
	}
	if (pNode->IsSeriesNode()) {
		//series
		const GNC::GCS::Ptr<GIL::DICOM::DicomDataset>& seriesDs = pNode->GetData();

		GNC::GUI::wxPropiedades::TMapaPropiedades mapaPropiedades;
		mapaPropiedades[_Std("Series Instance UID")] = seriesDs->getTag(GKDCM_SeriesInstanceUID);
		mapaPropiedades[_Std("Modality")] = seriesDs->getTag(GKDCM_Modality);
		mapaPropiedades[_Std("Series date")] = seriesDs->getTag(GKDCM_SeriesDate);
		mapaPropiedades[_Std("Series time")] = seriesDs->getTag(GKDCM_SeriesTime);
		properties.push_back(mapaPropiedades);
	}
}

//region realizacion de la interfaz IComandoPACSNotificador
void GNC::GUI::AcquisitionTableModel::ReloadModel(std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> >& results)
{	
	//RELOAD MODEL FROM IMODELODICOM...
	if (!studyMap.empty()) {
		wxDataViewItemArray toDeleteList;
		for (TMapIndex::iterator it = studyMap.begin(); it !=  studyMap.end(); ++it) {
			toDeleteList.push_back(wxDataViewItem((*it).second));
		}
		//then delete studies Nodes...		
		ItemsDeleted(wxDataViewItem(0), toDeleteList);

		//delete all and reload again...
		ClearStudyMap();
	}
	Cleared();

	wxDataViewItemArray addList;
	if(results.size() == 0) {
		//add a virtual item...
		GNC::GCS::Ptr<GIL::DICOM::DicomDataset> pDataset(new GIL::DICOM::DicomDataset());
		pDataset->tags[GKDCM_PatientName] = _Std("No results found");
		pDataset->tags[GKDCM_StudyInstanceUID] = "";
		pDataset->secuencias.push_back(GIL::DICOM::DicomDataset());//dummy
		AcquisitionNode* pNode = new AcquisitionNode(NULL, pDataset);
		studyMap[pNode->uid] = pNode;
		addList.push_back(wxDataViewItem((void*)pNode));
	}
	else {
		for (std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> >::iterator it = results.begin(); it != results.end(); ++it) {
			std::string studyUID = (*it)->getTag(GKDCM_StudyInstanceUID);
			if (studyMap.find(studyUID) == studyMap.end()) {
				AcquisitionNode* pNode = new AcquisitionNode(NULL, (*it));
				studyMap[pNode->uid] = pNode;
				addList.push_back(wxDataViewItem((void*)pNode));
			}
		}
	}

	ItemsAdded(wxDataViewItem(0), addList);
}

void GNC::GUI::AcquisitionTableModel::ReloadModel(const std::string& studyUID, std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> >& results)
{
	if (studyMap.find(studyUID) != studyMap.end()) {
		AcquisitionNode* pStudyNode = studyMap[studyUID];
		wxDataViewItemArray arr;
		pStudyNode->GetChildren(arr);
		if (!arr.empty()) {
			ItemsDeleted(wxDataViewItem(pStudyNode), arr);
			pStudyNode->DestroyChildren();
		}

		if(!results.empty()) {
			for (std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> >::iterator it = results.begin(); it != results.end(); ++it) {
				AcquisitionNode* pSeriesNode = new AcquisitionNode(pStudyNode, (*it));
				pStudyNode->AddChild(pSeriesNode);
			}
			arr.clear();
			pStudyNode->GetChildren(arr);
			if (!arr.empty()) {
				ItemsAdded(wxDataViewItem(pStudyNode), arr);
			}
		}
	}
}

//endregion

void GNC::GUI::AcquisitionTableModel::ProcesarEvento(GNC::GCS::Events::IEvent *evt)
{
	GNC::GCS::Events::EventoProgresoComando* pEvt = dynamic_cast<GNC::GCS::Events::EventoProgresoComando*> (evt);
	if (pEvt == NULL  || pEvt->GetComando() == NULL) {
		return;
	}
	
	switch (pEvt->GetTipo()) {
	case GNC::GCS::Events::EventoProgresoComando::TEP_Iniciado:
		break;
	case GNC::GCS::Events::EventoProgresoComando::TEP_Progreso:
		break;
	case GNC::GCS::Events::EventoProgresoComando::TEP_Unknown:
		break;
	case GNC::GCS::Events::EventoProgresoComando::TEP_Finalizado:
		GNC::GCS::IComando* pCmd = pEvt->GetComando();
		if (pCmd->GetOwner() == this) {
			if ( pCmd == m_pComandoPACS ) {
				m_pComandoPACS = NULL;
				GADAPI::ComandoPACSParams* pParams = dynamic_cast<GADAPI::ComandoPACSParams*>(pCmd->GetParams());
				if (pParams != NULL) {
					GNC::GCS::Ptr<GNC::GCS::Condition> cond = pParams->pQuery->getCondition(GKDCM_StudyInstanceUID);
					if (studyMap.empty() || !cond.IsValid() || cond->getValue().empty()) {
						ReloadModel(*pParams->m_apResults);
					} else {
						ReloadModel(cond->getValue(), *pParams->m_apResults);
					}
				}
			}
		}
		break;
	
	}
}

void GNC::GUI::AcquisitionTableModel::ClearStudyMap()
{
	for (TMapIndex::iterator it = studyMap.begin(); it !=  studyMap.end(); ++it) {
		delete (*it).second;
	}
	studyMap.clear();
}
