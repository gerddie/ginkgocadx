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


#include "ipipelineproperties.h"
GNC::GCS::IPipelineProperties::IPipelineProperties()
{
}

GNC::GCS::IPipelineProperties::~IPipelineProperties()
{
}

void GNC::GCS::IPipelineProperties::GetFPDimensions(double dims[3])
{
	int idims[3] = {0, 0, 0};
	GetDimensions(idims);
	dims[0] = (double) idims[0];
	dims[1] = (double) idims[1];
	dims[2] = (double) idims[2];

}

/**Gets the slice output dimensions **/
void GNC::GCS::IPipelineProperties::GetFPOutputDimensions(double dims[3])
{
	int odims[3] = {0, 0, 0};
	GetOutputDimensions(odims);
	dims[0] = (double) odims[0];
	dims[1] = (double) odims[1];
	dims[2] = (double) odims[2];
}

//endregion

//------------------------------------------------------------------------------------------------
//region optional

/** Sets camera custom orientation **/
void GNC::GCS::IPipelineProperties::SetCustomOrientation(GNC::GCS::Vector3D&, GNC::GCS::Vector3D&, GNC::GCS::Vector3D&, double&)
{
}

/** Gets the output current slice number **/
int GNC::GCS::IPipelineProperties::GetCurrentSliceIndex() const
{
	return 0;
}
