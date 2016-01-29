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
