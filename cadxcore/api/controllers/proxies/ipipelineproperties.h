/*
*
*  $Id: ipipelineproperties.h $
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
#ifndef IPIPELINEPROPERTIES_H
#define IPIPELINEPROPERTIES_H
#include <api/api.h>
#include <api/autoptr.h>

//----------------------------------------------------------------------------------------------------
//region Forward Declarations

//endregion
namespace GNC {
	namespace GCS {
		class IGinkgoMatrix4x4;
		class Vector3D;

		//----------------------------------------------------------------------------------------------------
		/** Pipeline Properties Proxy  class **/
		class EXTAPI IPipelineProperties {
		public:
			
			IPipelineProperties();

			virtual ~IPipelineProperties();

			//------------------------------------------------------------------------------------------------
			//region mandatory

			/** Updates **/
			virtual void Update() = 0;

			/** Updates output information **/
			virtual void UpdateInformation() = 0;

			/** Gets original (3D Volume) direction cosines **/
			virtual const GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> GetDirection() = 0;
		
			/** Gets the original (3D Volume) origin **/
			virtual void GetOrigin(double origin[3]) = 0;
		
			/** Gets the original (3D Volume) spacing **/
			virtual void GetSpacing(double spacing[3]) = 0;
		
			/** Gets the original (3D Volume) dimensions **/
			virtual void GetDimensions(int dims[3]) = 0;

			/* Gets the slice direction cosines */
			virtual const GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> GetOutputDirection() = 0;
		
			/** Gets the slice output origin **/
			virtual void GetOutputOrigin(double origin[3]) = 0;
		
			/** Gets the slice output spacing **/
			virtual void GetOutputSpacing(double spacing[3]) = 0;
		
			/**Gets the slice output dimensions **/
			virtual void GetOutputDimensions(int currentDimensions[3]) = 0;


			//endregion

			//------------------------------------------------------------------------------------------------
			//region helpers
			
			/**Gets the original (3D Volume) dimensions as double floating point**/
			void GetFPDimensions(double dims[3]);

			/**Gets the slice output dimensions as double floating point**/
			void GetFPOutputDimensions(double currentDimensions[3]);

			//endregion

			//------------------------------------------------------------------------------------------------
			//region optional

			/** Sets camera custom orientation **/
			virtual void SetCustomOrientation(GNC::GCS::Vector3D& camera_focal_point, GNC::GCS::Vector3D& camera_position, GNC::GCS::Vector3D& camera_view_up, double& rotation);

			/** Gets the output current slice number **/
			virtual int GetCurrentSliceIndex() const;

			//endregion

		};
		//endregion
	}
}


#endif
