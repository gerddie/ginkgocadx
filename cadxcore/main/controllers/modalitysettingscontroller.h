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

namespace GNC {
	namespace GCS {
		class IVista;
		class Matcher;

		/**
		* Annotations settings
		*/
		class AnnotationSettings 
		{
		public:
			typedef enum TAnnotationPosition {
				TP_TopLeft = 0,
				TP_TopRight,
				TP_BottomLeft,
				TP_BottomRight,
				TP_NumPos
			} TAnnotationPosition;

			AnnotationSettings();
			AnnotationSettings(wxXmlNode* node);
			AnnotationSettings(const AnnotationSettings& other);
			~AnnotationSettings();
			/**
			serialize to XML
			*/
			wxXmlNode* serialize();

			std::string getAnnotation(TAnnotationPosition pos) const;
			void setAnnotation(TAnnotationPosition pos, const std::string& annotation);
			
		protected:
			std::string annotation[TP_NumPos];
		};
		
		/**
		* window level setting
		*/
		class WindowLevelSetting 
		{
		public:
			WindowLevelSetting();
			WindowLevelSetting(double window, double level, const std::string& label);
			WindowLevelSetting(wxXmlNode* node);
			WindowLevelSetting(const WindowLevelSetting& other);
			~WindowLevelSetting();
			/**
			serialize to XML
			*/
			wxXmlNode* serialize();

			double getWindow() const;
			double getLevel() const;
			const std::string& getLabel() const;

			std::string toString() const;

			void setWindow(double window);
			void setLevel(double level);
			void setLabel(const std::string& label);
			
		protected:
			double window;
			double level;
			std::string label;
		};

		/**
		* default modality settings
		*/
		class DefaultModalitySettings 
		{
		public:
			typedef enum TInterpolationMode{
				TIM_NEAREST_NEIGHBOUR,
				TIM_LINEAR,
				TIM_CUBIC
			}TInterpolationMode;
			typedef std::list<GNC::GCS::Ptr<WindowLevelSetting> > TListOfWLSettings;
			DefaultModalitySettings();
			DefaultModalitySettings(wxXmlNode* node);
			DefaultModalitySettings(const DefaultModalitySettings& other);
			~DefaultModalitySettings();
			/**
			serialize to XML
			*/
			wxXmlNode* serialize();

			static std::string getDefaultWindowLevelLabel();
			static std::string getAutocalculateWindowLevelLabel();

			const TListOfWLSettings& getWLSettings() const;
			const std::string& getModality() const;
			const std::string& getInitialWindowLevelLabel() const;
			TInterpolationMode getInterpolationMode() const;

			const GNC::GCS::Ptr<AnnotationSettings>& getAnnotationSettings() const;
			void setAnnotationSettings(const GNC::GCS::Ptr<AnnotationSettings>& pSettings);

			void addWindowLevel(const GNC::GCS::Ptr<WindowLevelSetting>& wlSetting);
			void clearWindowLevels();
			void setModality(const std::string& modality);
			void setInitialWindowLevelLabel(const std::string& label);
			void setInterpolationMode(TInterpolationMode mode);
		protected:
			std::string modality;
			std::string initialWindowLevelLabel;
			TInterpolationMode interpolationMode;
			TListOfWLSettings listOfWLSettings;
			GNC::GCS::Ptr<AnnotationSettings> annotationSettings;
		};

		
		/**
		modality settings controller class
		*/
		class ModalitySettingsController 
		{
		public:
			/**
			modality settings map
			*/
			typedef std::map<std::string, GNC::GCS::Ptr<DefaultModalitySettings> > TMapOfModalitySettings;

			/**
			Singleton method
			*/
			static ModalitySettingsController* Instance();

			/**
			Free singleton
			*/
			static void FreeInstance();

			const TMapOfModalitySettings& getModalitySettingsMap();

			void addModalitySettings(const GNC::GCS::Ptr<DefaultModalitySettings>& settings);

			void clearModalitySettings();

			GNC::GCS::Ptr<DefaultModalitySettings> getModalitySettings(const std::string& modality);

			/**
			stores modalitySettings list
			*/
			void StoreModalitySettingsConfiguration();
		protected:
			/**
			Creation method
			*/
			ModalitySettingsController();

			void LoadDefaultModalitySettings();

			/**
			Destruction method
			*/
			~ModalitySettingsController();

			/**
			Load from configuration
			*/
			void LoadFromConfiguration();

			/**
			singleton attribute
			*/
			static ModalitySettingsController* m_pInstance;

			/**
			visualization modality settings
			*/
			TMapOfModalitySettings modalitySettings;
		};
	}
}
