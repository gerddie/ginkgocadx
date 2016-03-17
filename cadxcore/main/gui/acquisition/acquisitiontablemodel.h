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

#pragma once
#include <main/controllers/historycontroller.h>
#include <api/dicom/imodelodicom.h>
#include <api/observers/ieventsobserver.h>
#include <wx/dynarray.h>
#include <wx/dataview.h>
#include <prvext/prvext.h>
#include <wx/propiedades/wxpropiedades.h>
#include <api/ilock.h>
#include <api/autoptr.h>

#define COL_ICON			0
#define COL_PATIENT_NAME	1
#define COL_PATIENT_ID		2
#define COL_DESCRIPTION		3
#define COL_MODALITY		4
#define COL_DATETIME		5
#define COL_ACCNUMBER	6
#define COL_ACQUISITION_MAX	7

namespace GADAPI
{
class ComandoPACS;
}

class wxImageList;
namespace GNC
{
namespace GCS
{
class StoredQuery;
}
namespace GUI
{
class AcquisitionNode: GNC::GCS::ILockable
{
public:
        AcquisitionNode(AcquisitionNode* parent, const GNC::GCS::Ptr<GIL::DICOM::DicomDataset> pInfo);
        virtual ~AcquisitionNode();

        bool IsSeriesNode();
        bool IsStudyNode();

        void AddChild(AcquisitionNode* pChild);
        void DestroyChildren();

        const GNC::GCS::Ptr<GIL::DICOM::DicomDataset>& GetData();

        AcquisitionNode* GetParent();

        virtual unsigned int GetChildren(wxDataViewItemArray &array) ;

        void SetDateTime(const std::string& strDate, const std::string& strTime);

        typedef std::map<std::string, AcquisitionNode*> TMapIndex;
        TMapIndex seriesMap;
        std::string datetime;
        std::string uid;

        GNC::GCS::Ptr<GIL::DICOM::DicomDataset> pData;

        AcquisitionNode* pParent;
};


class AcquisitionTableModel: public wxDataViewModel, public GNC::GCS::IEventsObserver
{
public:
        typedef enum T_ModelMode {
                MM_SERIES,
                MM_STUDIES
        } T_ModelMode;

        AcquisitionTableModel();
        ~AcquisitionTableModel();

        //wxDataViewModel methods
        virtual unsigned int GetColumnCount() const;
        virtual wxString GetColumnType( unsigned int col ) const;
        virtual void GetValue( wxVariant &variant,
                               const wxDataViewItem &item, unsigned int col ) const;
        virtual bool SetValue( const wxVariant &variant,
                               const wxDataViewItem &item, unsigned int col );
        virtual bool IsEnabled( const wxDataViewItem &item, unsigned int col ) const;
        virtual wxDataViewItem GetParent( const wxDataViewItem &item ) const;
        virtual bool IsContainer( const wxDataViewItem &item ) const;
        bool 	HasValue (const wxDataViewItem &item, unsigned col) const;
        virtual bool HasContainerColumns (const wxDataViewItem &item) const ;
        virtual unsigned int GetChildren( const wxDataViewItem &parent,
                                          wxDataViewItemArray &array ) const;
        int Compare( const wxDataViewItem &item1, const wxDataViewItem &item2,
                     unsigned int column, bool ascending ) const;
        //
        void CancelCommand();
        void ClearResults();
        T_ModelMode GetModelMode();
        void SetModelMode(T_ModelMode mode);
        void DoSearch(const GNC::GCS::Ptr<GNC::GCS::StoredQuery>& pQuery);
        void DoExpand(AcquisitionNode* pNode, const std::string& modalities, const std::string& server);

        std::string GetModality(AcquisitionNode* pNode);
        void GetProperties(AcquisitionNode* pNode, GNC::GUI::wxPropiedades::TListaMapasPropiedades& properties);

        //region realizacion de la interfaz IComandoPACSNotificador
        virtual void ReloadModel(std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> >& results);
        virtual void ReloadModel(const std::string& studyUID, std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> >& results);
        //endregion
        virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt);

protected:
        void ClearStudyMap();


        GADAPI::ComandoPACS*           m_pComandoPACS;

        T_ModelMode m_Mode;
        typedef std::map<std::string, AcquisitionNode*> TMapIndex;
        TMapIndex studyMap;
        wxImageList* m_pImageList;
};
}
}
