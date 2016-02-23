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

#include <map>
#include "historytablemodel.h"
#include <main/controllers/controladorlog.h>
#include <set>
#include <wx/imaglist.h>
#include <resources/ginkgoresourcesmanager.h>

#define SIZE_ICONOS 16

#define ICONO_LOCAL_DATABASE 0
#define ICONO_LOCAL_LINKED 1
#define ICONO_WADO_LINKED 2
#define ICONO_LOCAL_DATABASE_RECENT 3
#define ICONO_LOCAL_LINKED_RECENT 4
#define ICONO_WADO_LINKED_RECENT 5

GNC::GUI::HistoryNode::HistoryNode(const GNC::GCS::IHistoryController::StudyModel& study): pParent(NULL)
{
	pStudy = GNC::GCS::Ptr<GNC::GCS::IHistoryController::StudyModel>(new GNC::GCS::IHistoryController::StudyModel(study));
}

GNC::GUI::HistoryNode::HistoryNode(const GNC::GCS::IHistoryController::SeriesModel& series, HistoryNode* parent):pParent(parent)
{
	pSeries = GNC::GCS::Ptr<GNC::GCS::IHistoryController::SeriesModel>(new GNC::GCS::IHistoryController::SeriesModel(series));
}

GNC::GUI::HistoryNode::HistoryNode(): pParent(NULL)
{
}

GNC::GUI::HistoryNode::~HistoryNode()
{
	for (TMapIndex::iterator it = seriesMap.begin(); it !=  seriesMap.end(); ++it) {
		delete (*it).second;
	}
}

bool GNC::GUI::HistoryNode::IsStudyModel() const
{
	return pStudy.IsValid();
}

bool GNC::GUI::HistoryNode::IsSeriesModel() const
{ 
	return pSeries.IsValid();
}

GNC::GUI::HistoryNode* GNC::GUI::HistoryNode::GetParent()
{
	return pParent;
}

//if creation datetime is < now - 2 h => si se ha recibido antes de dos horas
bool GNC::GUI::HistoryNode::IsRecent()
{
	wxDateTime creationDate;
	if (IsSeriesModel()) {
		creationDate.ParseFormat(wxString::FromUTF8( GetSeriesModel()->created_time.c_str() ).GetData(), wxT("%Y-%m-%dT%H:%M:%S"), wxDefaultDateTime);
	} else {
		creationDate.ParseFormat(wxString::FromUTF8( GetStudyModel()->created_time.c_str() ).GetData(), wxT("%Y-%m-%dT%H:%M:%S"), wxDefaultDateTime);
	}
	wxTimeSpan diff = wxDateTime::Now().Subtract(creationDate);
	return diff.GetHours() < 2;
}

void GNC::GUI::HistoryNode::UpdateStudyModel(const GNC::GCS::IHistoryController::StudyModel& studyModel, wxDataViewItemArray& itemsAdded, wxDataViewItemArray& itemsChanged)
{
	if (pStudy.IsValid()) {
		*pStudy = studyModel;
		//if it's expanded... retry...
		if (!seriesMap.empty()) {
			//insert or update series....
			GNC::GCS::ILocker lock(this);
			GNC::GCS::IHistoryController::SeriesModelList seriesList;
			GNC::GCS::IHistoryController::Instance()->GetSeriesFromStudy(GetStudyModel()->pk, seriesList);
			for(GNC::GCS::IHistoryController::SeriesModelList::const_iterator it = seriesList.begin(); it != seriesList.end(); ++it) {
				if (seriesMap.find((*it).pk) != seriesMap.end()) {
					seriesMap[(*it).pk]->UpdateSeriesModel((*it));
					itemsChanged.push_back(wxDataViewItem(seriesMap[(*it).pk]));
				} else {
					HistoryNode* seriesNode = new HistoryNode((*it), this);
					seriesMap[(*it).pk] = seriesNode;
					itemsAdded.push_back(wxDataViewItem(seriesMap[(*it).pk]));
				}
			}
		}
	}
}
			
void GNC::GUI::HistoryNode::UpdateSeriesModel(const GNC::GCS::IHistoryController::SeriesModel& seriesModel)
{
	if (pSeries.IsValid()) {
		*pSeries = seriesModel;
	}
}

const GNC::GCS::Ptr<GNC::GCS::IHistoryController::StudyModel>& GNC::GUI::HistoryNode::GetStudyModel()
{
	return pStudy;
}

const GNC::GCS::Ptr<GNC::GCS::IHistoryController::SeriesModel>& GNC::GUI::HistoryNode::GetSeriesModel()
{
	return pSeries;
}

unsigned int GNC::GUI::HistoryNode::GetChildren(wxDataViewItemArray &array ) 
{
	if (IsSeriesModel()) {
		return 0;
	} else if (IsStudyModel()) {
		ReloadSeries();
		GNC::GCS::ILocker lock(this);
		for (TMapIndex::const_iterator it = seriesMap.begin(); it != seriesMap.end(); ++it) {
			array.Add( wxDataViewItem( (void*) (*it).second) );
		}
		return seriesMap.size();
	} else {
		return 0;
	}
}

void GNC::GUI::HistoryNode::GetSeriesModel(SeriesModelList& seriesList)
{
	if (IsSeriesModel()) {
		seriesList.push_back(GetSeriesModel());
	} else if (IsStudyModel()) {
		ReloadSeries();
		GNC::GCS::ILocker lock(this);
		for (TMapIndex::const_iterator it = seriesMap.begin(); it != seriesMap.end(); ++it) {
			seriesList.push_back((*it).second->GetSeriesModel());
		}
	}

}

void GNC::GUI::HistoryNode::ReloadSeries()
{
	if (IsSeriesModel()) {
		return;
	} else {
		if (seriesMap.empty()) {
			GNC::GCS::ILocker lock(this);
			GNC::GCS::IHistoryController::SeriesModelList seriesList;
			GNC::GCS::IHistoryController::Instance()->GetSeriesFromStudy(GetStudyModel()->pk, seriesList);
			for(GNC::GCS::IHistoryController::SeriesModelList::const_iterator it = seriesList.begin(); it != seriesList.end(); ++it) {
				HistoryNode* seriesNode = new HistoryNode((*it), this);
				seriesMap[(*it).pk] = seriesNode;
			}
		}
	}
}

void GNC::GUI::HistoryNode::DeleteSeries(long seriesPk)
{
	GNC::GCS::ILocker lock(this);
	if (seriesMap.find(seriesPk) != seriesMap.end()) {
		delete seriesMap.find(seriesPk)->second;
		seriesMap.erase(seriesMap.find(seriesPk));
	}

}

/////////////////////////////
GNC::GUI::HistoryTableModel::HistoryTableModel()
{
	m_pImageList = new wxImageList(SIZE_ICONOS,SIZE_ICONOS,true);
	wxBitmap bmp;
	bmp = GinkgoResourcesManager::History::GetIcoLocalFile();
	if (bmp.IsOk()) {
		m_pImageList->Add(bmp);
	}
	bmp = GinkgoResourcesManager::History::GetIcoLocalLinked();
	if (bmp.IsOk()) {
		m_pImageList->Add(bmp);
	}
	bmp = GinkgoResourcesManager::History::GetIcoWadoLinked();
	if (bmp.IsOk()) {
		m_pImageList->Add(bmp);
	}	
	bmp = GinkgoResourcesManager::History::GetIcoLocalFileRecent();
	if (bmp.IsOk()) {
		m_pImageList->Add(bmp);
	}
	bmp = GinkgoResourcesManager::History::GetIcoLocalLinkedRecent();
	if (bmp.IsOk()) {
		m_pImageList->Add(bmp);
	}
	bmp = GinkgoResourcesManager::History::GetIcoWadoLinkedRecent();
	if (bmp.IsOk()) {
		m_pImageList->Add(bmp);
	}
}


GNC::GUI::HistoryTableModel::~HistoryTableModel()
{
	ClearStudyMap();
	delete m_pImageList;
}

void GNC::GUI::HistoryTableModel::ClearStudyMap()
{
	for (TMapIndex::iterator it = studyMap.begin(); it !=  studyMap.end(); ++it) {
		delete (*it).second;
	}
	studyMap.clear();
}

unsigned int GNC::GUI::HistoryTableModel::GetColumnCount() const
{
	return COL_HISTORY_MAX;
}

void GNC::GUI::HistoryTableModel::ReloadTree(const GNC::GCS::IHistoryController::StudyModelList& studyList, bool force)
{
	if (force) {
		wxDataViewItemArray toDeleteList;
		for (TMapIndex::iterator it = studyMap.begin(); it !=  studyMap.end(); ++it) {
			toDeleteList.push_back(wxDataViewItem((*it).second));
		}
		DeleteItems(toDeleteList);
		Cleared();
		//delete all and reload again...
		wxDataViewItemArray addList;
		ClearStudyMap();
		for (GNC::GCS::IHistoryController::StudyModelList::const_iterator it = studyList.begin(); it != studyList.end(); ++it) {
			HistoryNode* pNode = new HistoryNode((*it));
			studyMap[(*it).pk] = pNode;
			addList.push_back(wxDataViewItem((void*)pNode));
		}	
		ItemsAdded(wxDataViewItem(0), addList);
	} else {
		//check for changes...
		//remove nodes that doesn't come in this new list
		std::set<long> setOfpk;
		for(GNC::GCS::IHistoryController::StudyModelList::const_iterator it = studyList.begin(); it!= studyList.end(); ++it) {
			setOfpk.insert((*it).pk);
		}
		wxDataViewItemArray toDeleteList;
		for (TMapIndex::iterator itIndex = studyMap.begin(); itIndex != studyMap.end(); ++itIndex) {
			if (setOfpk.find((*itIndex).first) == setOfpk.end()) {
				toDeleteList.push_back(wxDataViewItem((void*)(*itIndex).second));
			}
		}
		
		ItemsDeleted(wxDataViewItem(0), toDeleteList);

		//delete nodes...
		for (wxDataViewItemArray::iterator it = toDeleteList.begin(); it != toDeleteList.end(); ++it) {
			HistoryNode* pNode = (HistoryNode*)(*it).GetID();
			studyMap.erase(studyMap.find(pNode->GetStudyModel()->pk));
			delete pNode;
		}

		//insert or update studies....
		wxDataViewItemArray itemsAdded;
		wxDataViewItemArray itemsChanged;
		for(GNC::GCS::IHistoryController::StudyModelList::const_iterator it = studyList.begin(); it!= studyList.end(); ++it) {
			if (studyMap.find((*it).pk) != studyMap.end()) {
				//update
				wxDataViewItemArray itemsAddedToStudy;
				studyMap[(*it).pk]->UpdateStudyModel((*it), itemsAddedToStudy, itemsChanged);
				if (!itemsAddedToStudy.empty()) {
					ItemsAdded(wxDataViewItem(studyMap[(*it).pk]), itemsAddedToStudy);
				}
			} else {
				//create
				HistoryNode* pNode = new HistoryNode((*it));
				studyMap[(*it).pk] = pNode;
				itemsAdded.push_back(wxDataViewItem(studyMap[(*it).pk]));
			}
		}
		ItemsChanged(itemsChanged);
		ItemsAdded(wxDataViewItem(NULL), itemsAdded);
	}	
}

void GNC::GUI::HistoryTableModel::DeleteItems(wxDataViewItemArray& selected)
{
	//first of all delete series Nodes...
	wxDataViewItemArray toDeleteList;
	for (wxDataViewItemArray::iterator it = selected.begin(); it != selected.end(); ++it) {
		if ((*it).IsOk()) {
			HistoryNode* pNode = (HistoryNode*) (*it).GetID();
			if (pNode->IsSeriesModel()) {
				ItemDeleted(wxDataViewItem(pNode->GetParent()), (*it));
				pNode->GetParent()->DeleteSeries(pNode->GetSeriesModel()->pk);
			} else {
				toDeleteList.push_back((*it));
			}
		}
	}
	//then delete studies Nodes...		
	ItemsDeleted(wxDataViewItem(0), toDeleteList);

	//delete nodes...
	for (wxDataViewItemArray::iterator it = toDeleteList.begin(); it != toDeleteList.end(); ++it) {
		HistoryNode* pNode = (HistoryNode*)(*it).GetID();
		studyMap.erase(studyMap.find(pNode->GetStudyModel()->pk));
		delete pNode;
	}
}

wxString GNC::GUI::HistoryTableModel::GetColumnType( unsigned int col ) const
{
	switch (col) {
	case COL_ICON:
		return wxT("wxIcon");
	case COL_AGE:
		return wxT("long");
	default:
		return wxT("string");
	}
}

void GNC::GUI::HistoryTableModel::GetValue( wxVariant &variant, const wxDataViewItem &item, unsigned int col ) const
{
	if (!item.IsOk()) {
		return;
	}
	//if it's a study...
	HistoryNode* pNode = (HistoryNode*)(item.GetID());
	if (pNode->IsStudyModel()) {
		const GNC::GCS::Ptr<GNC::GCS::IHistoryController::StudyModel> pModel = pNode->GetStudyModel();
		switch (col) {
			case COL_ICON:
			{
				int icono = 0;
				if (pNode->IsRecent()) {
					switch(pModel->location)
					{
					case GNC::GCS::IHistoryController::TL_LocalDatabase:
						icono = ICONO_LOCAL_DATABASE_RECENT;
						break;
					case GNC::GCS::IHistoryController::TL_LocalLinked:
						icono = ICONO_LOCAL_LINKED_RECENT;
						break;
					case GNC::GCS::IHistoryController::TL_WadoLinked:
						icono = ICONO_WADO_LINKED_RECENT;
						break;
					}
				} else {
					switch(pModel->location)
					{
					case GNC::GCS::IHistoryController::TL_LocalDatabase:
						icono = ICONO_LOCAL_DATABASE;
						break;
					case GNC::GCS::IHistoryController::TL_LocalLinked:
						icono = ICONO_LOCAL_LINKED;
						break;
					case GNC::GCS::IHistoryController::TL_WadoLinked:
						icono = ICONO_WADO_LINKED;
						break;
					}
				}
				variant << m_pImageList->GetIcon(icono);
			}
			break;
			case COL_PATIENT_NAME:
				variant = wxString::FromUTF8(pModel->pat_name.c_str());
				break;
			case COL_PATIENT_ID:
				variant = wxString::FromUTF8(pModel->pat_id.c_str());
				break;
			case COL_AGE:
				variant = GetAge(pModel->pat_bithdate, pModel->study_datetime);
				break;
			case COL_MODALITY:
				{
					std::ostringstream ostr;
					for (std::list<std::string>::const_iterator it = pModel->mods_in_study.begin(); it != pModel->mods_in_study.end(); ++it)
					{
						if (it != pModel->mods_in_study.begin()) {
							ostr << "/";
						}
						ostr << (*it);
					}
					variant = wxString::FromUTF8(ostr.str().c_str());
				}
				break;
			case COL_DESCRIPTION:
				variant = wxString::FromUTF8(pModel->study_desc.c_str());
				break;
			case COL_DATE_TIME_ACQUIRED:
				{
					wxDateTime studyDateTime;
					studyDateTime.ParseFormat(wxString::FromUTF8( pModel->study_datetime.c_str() ).GetData(), wxT("%Y-%m-%dT%H:%M:%S"), wxDefaultDateTime);
					if(studyDateTime.IsValid()){
						variant = wxString(studyDateTime.Format(_("%m/%d/%Y %H:%M:%S"), wxDateTime::TimeZone(wxDateTime::GMT1)));
					}
					else {
						variant = _("00/00/0000 00:00:00");
					}
				}
				break;
			case COL_DATE_TIME_ADDED:
				{
					wxDateTime studyDateTime;
					studyDateTime.ParseFormat(wxString::FromUTF8( pModel->created_time.c_str() ).GetData(), wxT("%Y-%m-%dT%H:%M:%S"), wxDefaultDateTime);
					if(studyDateTime.IsValid()){
						variant = wxString(studyDateTime.Format(_("%m/%d/%Y %H:%M:%S"), wxDateTime::TimeZone(wxDateTime::GMT1)));
					}
					else {
						variant = _("00/00/0000 00:00:00");
					}
				}
				break;
			case COL_ACCNUMBER:
				variant = wxString::FromUTF8(pModel->study_acc_no.c_str());
				break;
			default:
				LOG_ERROR("HistoryTableModel", "wrong column id " << col);
				break;
		}
	} else if (pNode->IsSeriesModel()){
		const GNC::GCS::Ptr<GNC::GCS::IHistoryController::SeriesModel> pModel = pNode->GetSeriesModel();
		switch (col) {
			case COL_ICON:
			{
				int icono = 0;
				switch(pModel->location)
				{
				case GNC::GCS::IHistoryController::TL_LocalDatabase:
					icono = ICONO_LOCAL_DATABASE;
					break;
				case GNC::GCS::IHistoryController::TL_LocalLinked:
					icono = ICONO_LOCAL_LINKED;
					break;
				case GNC::GCS::IHistoryController::TL_WadoLinked:
					icono = ICONO_WADO_LINKED;
					break;
				}
				variant << m_pImageList->GetIcon(icono);
			}
			break;
			case COL_PATIENT_NAME:
				variant = wxString::FromUTF8(pModel->series_desc.c_str());
				break;
			case COL_PATIENT_ID:
				variant = wxEmptyString;
				break;
			case COL_AGE:
				//nothing to do... variant = wxString(pStudyModel->pat_name);
				break;
			case COL_MODALITY:
				variant = wxString::FromUTF8(pModel->series_modality.c_str());
				break;
			case COL_DESCRIPTION:
				variant = wxString::FromUTF8(pModel->series_desc.c_str());
				break;
			case COL_DATE_TIME_ACQUIRED:
				{
					wxDateTime seriesDateTime;
					seriesDateTime.ParseFormat(wxString::FromUTF8( pModel->series_datetime.c_str() ).GetData(), wxT("%Y-%m-%dT%H:%M:%S"), wxDefaultDateTime);
					if(seriesDateTime.IsValid()){
						variant = wxString(seriesDateTime.Format(_("%m/%d/%Y %H:%M:%S"), wxDateTime::TimeZone(wxDateTime::GMT1)));
					}
					else {
						variant = _("00/00/0000 00:00:00");
					}
				}
				break;
			case COL_DATE_TIME_ADDED:
				{
					wxDateTime seriesDateTime;
					seriesDateTime.ParseFormat(wxString::FromUTF8( pModel->created_time.c_str() ).GetData(), wxT("%Y-%m-%dT%H:%M:%S"), wxDefaultDateTime);
					if(seriesDateTime.IsValid()){
						variant = wxString(seriesDateTime.Format(_("%m/%d/%Y %H:%M:%S"), wxDateTime::TimeZone(wxDateTime::GMT1)));
					}
					else {
						variant = _("00/00/0000 00:00:00");
					}
				}
				break;
			case COL_ACCNUMBER:
				//nothing to do variant = wxString(pStudyModel->pat_name);
				break;
			default:
				LOG_ERROR("HistoryTableModel", "wrong column id " << col);
				break;
		}
	} else {
		LOG_ERROR("HistoryTableModel", "invalid node");
	}
}



bool GNC::GUI::HistoryTableModel::SetValue( const wxVariant &/*variant*/,const wxDataViewItem &/*item*/, unsigned int /*col*/ )
{
	//model cant be modified...
	return false;
}

bool GNC::GUI::HistoryTableModel::IsEnabled( const wxDataViewItem &/*item*/, unsigned int/* col*/ ) const
{
	return true;
}

wxDataViewItem GNC::GUI::HistoryTableModel::GetParent( const wxDataViewItem &item ) const
{
	if (!item.IsOk()) {
		return wxDataViewItem(0);
	}
	//if it's a study...
	HistoryNode* pNode = (HistoryNode*)(item.GetID());
	return wxDataViewItem( (void*) pNode->GetParent() );
}

bool GNC::GUI::HistoryTableModel::IsContainer( const wxDataViewItem &item ) const
{
	if (!item.IsOk()) {
		return true;
	}
	HistoryNode* pNode = (HistoryNode*)(item.GetID());
	return pNode->IsStudyModel();
}

bool GNC::GUI::HistoryTableModel::HasContainerColumns (const wxDataViewItem &/*item*/) const
{
	return true;
}

bool GNC::GUI::HistoryTableModel::HasValue (const wxDataViewItem &item, unsigned col) const
{
	if (!item.IsOk()) {
		return false;
	}
	//if it's a study...
	HistoryNode* pNode = (HistoryNode*)(item.GetID());
	return (pNode->IsStudyModel()) ||
		(pNode->IsSeriesModel() && (col!=COL_PATIENT_ID && col != COL_AGE && col!=COL_ACCNUMBER));
}


unsigned int GNC::GUI::HistoryTableModel::GetChildren( const wxDataViewItem &parent, wxDataViewItemArray &array ) const
{
	HistoryNode *node = (HistoryNode*) parent.GetID();
    if (!node)
    {
		unsigned int count = studyMap.size();
		for (TMapIndex::const_iterator it = studyMap.begin(); it !=  studyMap.end(); ++it) {
			array.Add( wxDataViewItem( (void*) (*it).second) );
		}
		return count;        
	} else if (node->IsStudyModel()) {
		//return series from study...
		node->GetChildren(array);
		return array.Count();
	} else {
		//series doesn't have children
		return 0;
	}
}

int GNC::GUI::HistoryTableModel::Compare( const wxDataViewItem &item1, const wxDataViewItem &item2,
                 unsigned int column, bool ascending ) const
{
	if ((column == COL_DATE_TIME_ACQUIRED || column == COL_DATE_TIME_ADDED) && item1.IsOk() && item2.IsOk())
    {
		HistoryNode *node1 = (HistoryNode*) item1.GetID();
		HistoryNode *node2 = (HistoryNode*) item2.GetID();
		wxDateTime date1, date2;
		if (column == COL_DATE_TIME_ACQUIRED) {
			if (node1->IsSeriesModel()) {
				date1.ParseFormat(wxString::FromUTF8( node1->GetSeriesModel()->series_datetime.c_str() ).GetData(), wxT("%Y-%m-%dT%H:%M:%S"), wxDefaultDateTime);
			} else {
				date1.ParseFormat(wxString::FromUTF8( node1->GetStudyModel()->study_datetime.c_str() ).GetData(), wxT("%Y-%m-%dT%H:%M:%S"), wxDefaultDateTime);
			}
			if (node2->IsSeriesModel()) {
				date2.ParseFormat(wxString::FromUTF8( node2->GetSeriesModel()->series_datetime.c_str() ).GetData(), wxT("%Y-%m-%dT%H:%M:%S"), wxDefaultDateTime);
			} else {
				date2.ParseFormat(wxString::FromUTF8( node2->GetStudyModel()->study_datetime.c_str() ).GetData(), wxT("%Y-%m-%dT%H:%M:%S"), wxDefaultDateTime);
			}
		} else {
			if (node1->IsSeriesModel()) {
				date1.ParseFormat(wxString::FromUTF8( node1->GetSeriesModel()->created_time.c_str() ).GetData(), wxT("%Y-%m-%dT%H:%M:%S"), wxDefaultDateTime);
			} else {
				date1.ParseFormat(wxString::FromUTF8( node1->GetStudyModel()->created_time.c_str() ).GetData(), wxT("%Y-%m-%dT%H:%M:%S"), wxDefaultDateTime);
			}
			if (node2->IsSeriesModel()) {
				date2.ParseFormat(wxString::FromUTF8( node2->GetSeriesModel()->created_time.c_str() ).GetData(), wxT("%Y-%m-%dT%H:%M:%S"), wxDefaultDateTime);
			} else {
				date2.ParseFormat(wxString::FromUTF8( node2->GetStudyModel()->created_time.c_str() ).GetData(), wxT("%Y-%m-%dT%H:%M:%S"), wxDefaultDateTime);
			}
		}
		
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


long GNC::GUI::HistoryTableModel::GetAge(const std::string& patBirthDate, const std::string& studyDateTime) const
{
	wxDateTime birthDate, studyDate;
	birthDate.ParseFormat(wxString::FromUTF8( patBirthDate.c_str() ).GetData(), wxT("%Y-%m-%d"), wxDefaultDateTime);
	studyDate.ParseFormat(wxString::FromUTF8( studyDateTime.c_str() ).GetData(), wxT("%Y-%m-%dT%H:%M:%S"), wxDefaultDateTime);
	if (!birthDate.IsValid() || !studyDate.IsValid()) {
		return -1;
	}
	if (studyDate.GetYear() == birthDate.GetYear()) {
		return 0;
	} else {
		if (studyDate.GetMonth() > birthDate.GetMonth()) {
			return studyDate.GetYear() - birthDate.GetYear();
		} else if (studyDate.GetMonth() < birthDate.GetMonth()) {
			return studyDate.GetYear() - birthDate.GetYear() -1;
		} else {
			if (studyDate.GetDay() >= birthDate.GetDay()) {
				return studyDate.GetYear() - birthDate.GetYear();
			} else {
				return studyDate.GetYear() - birthDate.GetYear() -1;
			}
		}
	}
}
