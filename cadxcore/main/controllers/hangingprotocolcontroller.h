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

#include <string>
#include <map>
#include <list>
#include <api/autoptr.h>
#include <api/dicom/idicom.h>
class wxXmlNode;
class wxSizer;
class wxWindow;
class wxSizerItem;

namespace GNC
{
namespace GCS
{
class IVista;
class Matcher;
/**
if series viewers implements this interface, getLayoutForSizer
*/
class IHangingSeriesViewer
{
public:
        virtual void getSeriesBase(GIL::DICOM::DicomDataset& seriesBase) = 0;
};
/**
contract to realize layout...
*/
class IHangingProtocolContract
{
public:
        virtual wxWindow* createSeriesSlot(const std::map<std::string, std::string>& properties, long pk = -1, const GNC::GCS::Ptr<GNC::GCS::Matcher>& pMatcher = NULL) = 0;
        virtual const std::list<long>& getOpeningSeries() = 0;
};
/**
Class that evaluates study and series matching, depending on matching criteria over tags
*/
class Matcher
{
public:
        Matcher();
        /**
        Deserialize from xml
        */
        Matcher(wxXmlNode* node);
        Matcher(const Matcher& other);
        ~Matcher();
        bool MatchesStudy(long idStudy);
        bool MatchesSeries(long idSeries);
        /**
        serialize to XML
        */
        wxXmlNode* serialize();
        /**
        getter and setter
        */
        std::map<std::string, std::string>& getMatchingCriteria();
        void setMatchingCriteria(const std::map<std::string, std::string>& properties);
protected:
        bool matches(const std::string& pathOfFile);
        std::map<std::string, std::string> matchingCriteria;
};

class HangingProtocol;
/**
Defines a specific layout, is recursive to allow splitted cells
*/
class Layout
{
public:
        Layout();
        /**
        deserialize from xml
        */
        Layout(wxXmlNode* layoutRoot);
        Layout(const Layout& other);
        ~Layout();
        /**
        Layout with a row and a column is a cell and cellList is empty
        */
        bool isACell();
        /**
        serialize to XML
        */
        wxXmlNode* serialize();

        void doLayout(IHangingProtocolContract* pContract, wxSizer* parentSizer);
        static GNC::GCS::Ptr<GNC::GCS::Layout> getLayoutFromSizer(wxSizer* pSizer);
protected:
        static GNC::GCS::Ptr<GNC::GCS::Layout> getLayoutFromSizerItem(wxSizerItem* pSizerItem);
public:

        /**
        getters and setters
        */
        int getRows();
        void setRows(int rows);
        int getColumns();
        void setColumns(int columns);
        std::list<GNC::GCS::Ptr<Layout> >& getCellList();
        void setCellList(const std::list<GNC::GCS::Ptr<Layout> >& cellList);
        const GNC::GCS::Ptr<Matcher>& getMatcher();
        void setMatcher(const GNC::GCS::Ptr<Matcher>& matcher);
        std::map<std::string, std::string>& getProperties();
        void setProperties(const std::map<std::string, std::string>& properties);
        GNC::GCS::HangingProtocol* getParent();
        void setParent(GNC::GCS::HangingProtocol* pH);
        void setDefaultPosition();
        void setWindowProperties(bool isMaximized, int xPos, int yPos, int width, int height);
        void copyWindowProperties(const GNC::GCS::Layout& other);
        void applyWindowProperties(wxWindow* pWindow);
        void applyWindowProperties(GNC::GCS::IVista* pView);
protected:
        int rows;
        int columns;
        /**
        pointer to parent
        */
        GNC::GCS::HangingProtocol* pParent;
        /**
        left to right and top down list of cells
        */
        std::list<GNC::GCS::Ptr<Layout> > cellList;
        /**
        if it's a cell series matching criteria
        */
        GNC::GCS::Ptr<Matcher> seriesMatcher;
        /*
        here modules can insert key/value properies as zoom level, window/level...
        */
        std::map<std::string, std::string> properties;
        /**
        if it's detached or not
        */
        bool isDetached;
        /**
        Window details
        */
        bool isMaximized;
        int xPos, yPos, width, height;
};

/**
HangingProtocol class, defines a hanging protocol
*/
class HangingProtocol
{
public:
        typedef std::list<GNC::GCS::Ptr<Layout> > TListOfLayouts;
        HangingProtocol();
        /**
        deserialize from xml
        */
        HangingProtocol(wxXmlNode* node);
        HangingProtocol(const HangingProtocol& other);
        ~HangingProtocol();
        /**
        serialize to XML
        */
        wxXmlNode* serialize();
        /**
        true if study matches with this hanging protocol
        */
        bool matches(long idStudy);

        /**
        Getters and setters
        */
        const std::string& getName();
        void setName(const std::string& name);
        const std::string& getDescription();
        void setDescription(const std::string& description);
        const std::string& getModuleSid();
        void setModuleSid(const std::string& name);
        int getOpeningMode();
        void setOpeningMode(int mode);
        const GNC::GCS::Ptr<Matcher>& getStudyMatcher();
        void setStudyMatcher(const GNC::GCS::Ptr<Matcher>& matcher);
        const TListOfLayouts& getLayouts();
        void addLayout(const GNC::GCS::Ptr<Layout>& layout);
        void clearLayouts();
        std::map<std::string, std::string>& getProperties();
        void setProperties(const std::map<std::string, std::string>& properties);
        bool isActive();
        void setActive(bool active);
protected:
        /**
        Unique name
        */
        std::string name;
        /**
        description
        */
        std::string description;
        /**
        Module that opens this hanging protocol
        */
        std::string moduleSid;
        /**opening mode
        */
        int openingMode;
        /**
        study matching
        */
        GNC::GCS::Ptr<Matcher> studyMatcher;
        /**
        layout of the protocol
        */
        TListOfLayouts layouts;
        /*
        here modules can insert key/value properies
        */
        std::map<std::string, std::string> properties;
        /*
        if it's active
        */
        bool active;
};

/**
Hanging protocol controller class
*/
class HangingProtocolController
{
public:
        /**
        Singleton method
        */
        static HangingProtocolController* Instance();

        /**
        Free singleton
        */
        static void FreeInstance();

        /**
        Get hanging protocol for specified study
        */
        GNC::GCS::Ptr<HangingProtocol> GetProtocolForStudy(long idStudy);

        /**
        stores hanging protocol list
        */
        void StoreHangingProtocolConfiguration();
        /**
        gets a copy of hanging protocols
        */
        std::list<GNC::GCS::Ptr<HangingProtocol> > getHangingProtocols();
        /**
        sets hanging protocol list...
        */
        void setHangingProtocols(std::list<GNC::GCS::Ptr<HangingProtocol> > listOfProtocols);
protected:
        /**
        Creation method
        */
        HangingProtocolController();

        /**
        Destruction method
        */
        ~HangingProtocolController();

        /**
        Load from configuration
        */
        void LoadFromConfiguration();

        /**
        singleton attribute
        */
        static HangingProtocolController* m_pInstance;

        /**
        protocol list
        */
        std::list<GNC::GCS::Ptr<HangingProtocol> > listOfProtocols;
};
}
}
