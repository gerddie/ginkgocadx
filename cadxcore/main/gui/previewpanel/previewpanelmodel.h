/*
 *  
 *  $Id: previewpanelmodel.h $
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
#pragma once
#include <list>
#include <api/autoptr.h>

class wxWindow;
namespace GNC {
	namespace GUI {
		class IPreviewLeaf;
		
		class EXTAPI IPreviewNode {
		public:
			typedef std::list<IPreviewNode* > TListNodes;
			IPreviewNode(){}
			IPreviewNode(const std::string& label) {
				this->label = label;
			}
			virtual ~IPreviewNode()
			{
				for (TListNodes::iterator it = children.begin(); it != children.end(); ++it) 
				{
					delete (*it);
				}
				children.clear();
			}
			std::string GetLabel() {
				return this->label;
			}
			void SetLabel(const std::string& label) {
				this->label = label;
			}

			void AddChild(IPreviewNode* child) {
				children.push_back(child);
			}

			TListNodes children;

			virtual void OnShowMenu(wxWindow* pParent) = 0;
			virtual void OnPreviewDClick(wxWindow* pParent) = 0;
			virtual void OnPreviewLDown(wxWindow* pParent) = 0;
			std::string label;
		};

		class EXTAPI IPreviewLeaf: public IPreviewNode 
		{
		public:
			IPreviewLeaf()
			{
			}
			IPreviewLeaf(const std::string& label, const std::string& l2) :IPreviewNode(label), label2(l2)
			{
			}

			virtual wxBitmap GetOverlayBitmap()
			{
				return wxBitmap();
			}

			std::string GetLabel2() {
				return this->label2;
			}
			void SetLabel2(const std::string& label) {
				this->label2 = label;
			}
			std::string GetLabel3() {
				return this->label3;
			}
			void SetLabel3(const std::string& label) {
				this->label3 = label;
			}
			long GetFilePk() {
				return this->file_pk;
			}
			void SetFilePk(long file_pk) {
				this->file_pk = file_pk;
			}
		protected:
			std::string label2;
			std::string label3;
			long file_pk;
		};
	};
};
