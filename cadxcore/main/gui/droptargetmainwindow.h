/*
 *
 *  $Id: droptargetmainwindow.h $
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

#include <wx/dnd.h>

/**
Drop target that executes dropped file for history and main window
*/
namespace GNC {
	namespace GUI {
		class DropTargetMainWindow: public wxFileDropTarget
		{
		public:
			DropTargetMainWindow();
			~DropTargetMainWindow();
			bool OnDropFiles(wxCoord /*x*/, wxCoord /*y*/, const wxArrayString& filenames);
		};
	}
}