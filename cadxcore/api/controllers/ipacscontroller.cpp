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

#include <list>
#include <map>

#ifdef verify
#define MACRO_QUE_ESTORBA verify
#undef verify
#endif
#include <api/controllers/ipacscontroller.h>

#ifdef MACRO_QUE_ESTORBA
#define verify MACRO_QUE_ESTORBA
#endif

GIL::DICOM::PACSException::PACSException(const std::string& msg, const std::string& component, bool fatal) throw() : GNC::GCS::IException(msg, component, fatal)
{
}

GIL::DICOM::IPACSController::IPACSController()
{
}

GIL::DICOM::IPACSController::~IPACSController()
{
}

