
#############################################################################################

SET(Cairo_FOUND TRUE)
SET(Cairo_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/cairo-1.8.10/include)
SET(Cairo_LIBRARIES cairo-1.8.10)
SET(Cairo_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/cairo-1.8.10/lib/${DTYPE})
SET(Pixman_FOUND TRUE)
SET(Pixman_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/cairo-1.8.10/include)
SET(Pixman_LIBRARIES pixman-0.17.10)
SET(Pixman_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/pixman-0.17.10/lib/${DTYPE})

#############################################################################################

MACRO(PACKAGE_RULES PROJECT_NAME ARCH DEST)

ENDMACRO(PACKAGE_RULES)

#############################################################################################

MACRO(ADD_GINKGO_DEFINITIONS)
	
	ADD_GLOBAL_GINKGO_DEFINITIONS()
	
ENDMACRO(ADD_GINKGO_DEFINITIONS)

#############################################################################################

MACRO(ADD_EXTENSION_DEFINITIONS)
	
	ADD_GLOBAL_EXTENSION_DEFINITIONS()
		
ENDMACRO(ADD_EXTENSION_DEFINITIONS)

#############################################################################################

MACRO(ADD_CORE_DEFINITIONS)
	
	ADD_GLOBAL_CORE_DEFINITIONS()
	
ENDMACRO(ADD_CORE_DEFINITIONS)

#############################################################################################

MACRO(SET_CUSTOM_wxWidgets_PACKAGE)
	SET(wxWidgets_FOUND TRUE)
	SET(wxWidgets_defines -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXMAC__ -DHAVE_CONFIG_H -D_REENTRANT)
	SET(wxWidgets_INCLUDE_DIR
		${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/WX-2.8.11/mac-unicode-${DTYPE}-2.8
		${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/WX-2.8.11/include
	)
	IF (DEBUG)
		SET(wxWidgets_LIBRARIES wx_macud-2.8)
	ELSE()
		SET(wxWidgets_LIBRARIES wx_macu-2.8)
	ENDIF()
	
	SET(wxWidgets_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/WX-2.8.11/lib/${DTYPE})
ENDMACRO()

MACRO(SET_CUSTOM_VTK_PACKAGE)
	SET(VTK_FOUND TRUE)
	SET(VTK_defines -DVTK_DEBUG_LEAKS)
	SET(VTK_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/VTK-5.6.1/include)
	SET(VTK_LIBRARIES
		vtkCommon vtkGraphics vtkFiltering vtkHybrid vtkIO vtkRendering vtkImaging
		vtkGenericFiltering vtkWidgets vtkftgl vtkVolumeRendering vtkexoIIc vtkDICOMParser
		vtkjpeg vtktiff vtkpng vtkzlib vtkexpat vtksys vtkfreetype
	)
	SET(VTK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/VTK-5.6.1/lib/${DTYPE})
ENDMACRO()

MACRO(SET_CUSTOM_ITK_PACKAGE)
	SET(ITK_FOUND TRUE)
	SET(ITK_defines)
	SET(ITK_INCLUDE_DIR
		${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/ITK-3.20.0/include
		${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/ITK-3.20.0/include/gdcm/src
		${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/ITK-3.20.0/include
		${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/ITK-3.20.0/include/Common
		${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/ITK-3.20.0/include/IO
		${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/ITK-3.20.0/include/BasicFilters
		${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/ITK-3.20.0/include/Algorithms
		${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/ITK-3.20.0/include/Numerics
		${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/ITK-3.20.0/include/Numerics/Statistics
		${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/ITK-3.20.0/include/Utilities
		${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/ITK-3.20.0/include/Utilities/vxl/core
		${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/ITK-3.20.0/include/Utilities/vxl/vcl
		${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/ITK-3.20.0/include/gdcm/src
	)
	SET(ITK_LIBRARIES
		ITKCommon ITKIO ITKMetaIO ITKAlgorithms itkgdcm itksys itkgdcm
		itkjpeg8 itkjpeg12 itkjpeg16 itktiff itkpng itkzlib itkopenjpeg
		ITKDICOMParser itkv3p_netlib itkvcl itkvnl itkvnl_algo itkvnl_inst
		ITKniftiio ITKStatistics ITKBasicFilters ITKNrrdIO ITKMetaIO ITKznz
	)
	SET(ITK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/ITK-3.20.0/lib/${DTYPE})
ENDMACRO()

MACRO(SET_CUSTOM_DCMTK_PACKAGE)
	SET(DCMTK_FOUND TRUE)
	SET(DCMTK_defines)
	SET(DCMTK_INCLUDE_DIR
		${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/DCMTK-3.6.0/include
	)
	SET(DCMTK_LIBRARIES
		dcmdata dcmtls dcmnet ofstd oflog dcmjpeg dcmimage
		dcmimgle dcmsr ijg8 ijg12 ijg16
	)
	SET(DCMTK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../dll/${OS}-${ARCH}/DCMTK-3.6.0/lib/${DTYPE})
ENDMACRO()
