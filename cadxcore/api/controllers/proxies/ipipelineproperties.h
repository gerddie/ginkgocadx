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
