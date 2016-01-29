
#############################################################################################

MACRO(PACKAGE_RULES TPROJECT ARCH DEST)
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/ginkgocadx-launcher.in ${DEST}/ginkgocadx)
	ADD_CUSTOM_COMMAND(
		TARGET ${TPROJECT}
		POST_BUILD
		COMMAND echo "\\>\\>\\> Purging dist/"
		COMMAND rm -f \"${DEST}/ginkgocadx-${ARCH}\" "${DEST}/*.so*"
		COMMAND rm -rf \"${DEST}/Plugins\" \"${DEST}/lang\" \"${DEST}/resources\"
		COMMAND echo "\\>\\>\\> Making subdirs Plugins, resources, resources/cadxcore, lang, lang/el, lang/es, lang/ja, lang/de, lang/tr, lang/pl, lang/pt, lang/pt_BR, lang/fr, lang/sr"
		COMMAND mkdir \"${DEST}/resources\"
		COMMAND chmod 755 \"${DEST}/resources\"
		COMMAND mkdir \"${DEST}/resources/cadxcore\"
		COMMAND chmod 755 \"${DEST}/resources/cadxcore\"
		COMMAND mkdir -p \"${DEST}/Plugins\"
		COMMAND mkdir -p \"${DEST}/lang/el\" \"${DEST}/lang/es\" \"${DEST}/lang/ja\" \"${DEST}/lang/de\" \"${DEST}/lang/tr\" \"${DEST}/lang/pl\" \"${DEST}/lang/pt\" \"${DEST}/lang/pt_BR\" \"${DEST}/lang/fr\" \"${DEST}/lang/sr\"
		COMMAND echo "\\>\\>\\> Copying dicom conformance to dist/"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/resources/conformance.xml" "${DEST}/"
		COMMAND echo "\\>\\>\\> Copying dicom test card to dist/"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/resources/test_card.dcm" "${DEST}/"
		COMMAND echo "\\>\\>\\> Copying cadxcore resources to dist/"
		COMMAND cp -r "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/resources/files/*" "${DEST}/resources/cadxcore/"
		COMMAND echo "\\>\\>\\> Copying ginkgo binary to dist/"
		COMMAND cp "${CMAKE_CURRENT_BINARY_DIR}/ginkgocadx/ginkgocadx-${ARCH}" "${DEST}/ginkgocadx-${ARCH}"
		COMMAND echo "\\>\\>\\> changing RPATH of ginkgo"
		COMMAND chrpath "${DEST}/ginkgocadx-${ARCH}" -r "." ">/dev/null"
		COMMAND echo "\\>\\>\\> Stripping ginkgo"
		COMMAND strip -s "${DEST}/ginkgocadx-${ARCH}"
		COMMAND echo "\\>\\>\\> Applying perms"
		COMMAND chmod +x "${DEST}/ginkgocadx-${ARCH}" "${DEST}/ginkgocadx"

		COMMAND echo "\\>\\>\\> Copying libs to dist \\(${ARCH}\\)/"
		COMMAND cp -a "${CMAKE_CURRENT_BINARY_DIR}/../../lib/${OS}-${ARCH}/*/lib/${DTYPE}/*.so*" \"${DEST}\"
		COMMAND echo "\\>\\>\\> Applying perms"
		COMMAND chmod -x "${DEST}/*so*"

		#COMMAND echo "\\>\\>\\> changing RPATH of libCADxCore.so"
		#COMMAND chrpath "${DEST}/libCADxCore.so" -r "." ">/dev/null"
		#COMMAND echo "\\>\\>\\> Stripping CADxCore"
		#COMMAND strip -s "${DEST}/libCADxCore.so"
		COMMAND echo "\\>\\>\\> Installing translations"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/el/cadxcore.mo" "${DEST}/lang/el"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/el/wxstd.mo" "${DEST}/lang/el"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/es/cadxcore.mo" "${DEST}/lang/es"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/es/wxstd.mo" "${DEST}/lang/es"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/ja/cadxcore.mo" "${DEST}/lang/ja"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/ja/wxstd.mo" "${DEST}/lang/ja"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/de/cadxcore.mo" "${DEST}/lang/de"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/de/wxstd.mo" "${DEST}/lang/de"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/tr/cadxcore.mo" "${DEST}/lang/tr"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/tr/wxstd.mo" "${DEST}/lang/tr"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/pl/cadxcore.mo" "${DEST}/lang/pl"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/pl/wxstd.mo" "${DEST}/lang/pl"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/pt/cadxcore.mo" "${DEST}/lang/pt"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/pt/wxstd.mo" "${DEST}/lang/pt"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/pt_BR/cadxcore.mo" "${DEST}/lang/pt_BR"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/pt_BR/wxstd.mo" "${DEST}/lang/pt_BR"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/fr/cadxcore.mo" "${DEST}/lang/fr"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/fr/wxstd.mo" "${DEST}/lang/fr"
		COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/cadxcore/lang/sr/cadxcore.mo" "${DEST}/lang/sr"
	)

	IF(BUILD_VISUALIZATION_EXT)
		ADD_CUSTOM_COMMAND(
   	   TARGET ${TPROJECT}
      	POST_BUILD
			#COMMAND echo "\\>\\>\\> Copying visualizator libs to dist/Plugins \\(${ARCH}\\)"
			#COMMAND cp -a ${CMAKE_CURRENT_BINARY_DIR}/visualizator/libvisualizator.so* "${DEST}/Plugins"
			#COMMAND cp -a ${CMAKE_CURRENT_BINARY_DIR}/visualizator/visualizator.inf "${DEST}/Plugins"
			#COMMAND echo "\\>\\>\\> changing RPATH of Plugins/libvisualizator.so"
			#COMMAND chrpath "${DEST}/Plugins/libvisualizator.so" -d ">/dev/null"
			#COMMAND echo "\\>\\>\\> Stripping visualizator libs"
			#COMMAND strip -s "${DEST}/Plugins/libvisualizator.so"
			COMMAND echo "\\>\\>\\> Making subdirs resources/visualizator"
			COMMAND chmod 755 \"${DEST}/resources\"
			COMMAND mkdir \"${DEST}/resources/visualizator\"
			COMMAND chmod 755 \"${DEST}/resources\"
			COMMAND chmod 755 \"${DEST}/resources/visualizator\"
   	   COMMAND echo "\\>\\>\\> Copying visualizator resources to dist/"
	      COMMAND cp -r "${CMAKE_CURRENT_SOURCE_DIR}/visualizator/resources/files/*" "${DEST}/resources/visualizator/"
			COMMAND echo "\\>\\>\\> Installing translations"
			COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/visualizator/lang/el/visualizator.mo" "${DEST}/lang/el"
			COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/visualizator/lang/es/visualizator.mo" "${DEST}/lang/es"
			COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/visualizator/lang/ja/visualizator.mo" "${DEST}/lang/ja"
			COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/visualizator/lang/de/visualizator.mo" "${DEST}/lang/de"
			COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/visualizator/lang/tr/visualizator.mo" "${DEST}/lang/tr"
			COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/visualizator/lang/pl/visualizator.mo" "${DEST}/lang/pl"
			COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/visualizator/lang/pt/visualizator.mo" "${DEST}/lang/pt"
			COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/visualizator/lang/pt_BR/visualizator.mo" "${DEST}/lang/pt_BR"
			COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/visualizator/lang/fr/visualizator.mo" "${DEST}/lang/fr"
			COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/visualizator/lang/sr/visualizator.mo" "${DEST}/lang/sr"
		)
	ENDIF()

	IF(BUILD_OCT_EXT)
		ADD_CUSTOM_COMMAND(
   	   TARGET ${TPROJECT}
      	POST_BUILD
			COMMAND echo "\\>\\>\\> Copying octextension libs to dist/Plugins \\(${ARCH}\\)"
			COMMAND cp -a ${CMAKE_CURRENT_BINARY_DIR}/octextension/liboctextension.so* "${DEST}/Plugins"
			COMMAND cp -a ${CMAKE_CURRENT_BINARY_DIR}/octextension/octextension.inf "${DEST}/Plugins"
			COMMAND echo "\\>\\>\\> changing RPATH of Plugins/liboctextension.so"
			COMMAND chrpath "${DEST}/Plugins/liboctextension.so" -d ">/dev/null"
			COMMAND echo "\\>\\>\\> Stripping octextension libs"
			COMMAND strip -s "${DEST}/Plugins/liboctextension.so"
			COMMAND echo "\\>\\>\\> Installing translations"
			COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/octextension/lang/es/octextension.mo" "${DEST}/lang/es"
		)
	ENDIF()


	IF(BUILD_DERMATHOLOGY_EXT)
		ADD_CUSTOM_COMMAND(
         TARGET ${TPROJECT}
         POST_BUILD
			COMMAND echo "\\>\\>\\> Copying dermatologia libs to dist/Plugins \\(${ARCH}\\)"
			COMMAND cp -a ${CMAKE_CURRENT_BINARY_DIR}/dermatologia/libdermatologia.so* "${DEST}/Plugins"
			COMMAND cp -a ${CMAKE_CURRENT_BINARY_DIR}/dermatologia/dermatologia.inf "${DEST}/Plugins"
			COMMAND echo "\\>\\>\\> changing RPATH of Plugins/libdermatologia.so"
			COMMAND chrpath "${DEST}/Plugins/libdermatologia.so" -d ">/dev/null"
			COMMAND echo "\\>\\>\\> Stripping dermatologia libs"
			COMMAND strip -s "${DEST}/Plugins/libdermatologia.so"
			COMMAND echo "\\>\\>\\> Installing translations"
			COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/dermatologia/lang/es/derma.mo" "${DEST}/lang/es"	
		)
	ENDIF()

	IF(BUILD_OPHTALMOLOGY_EXT)
		ADD_CUSTOM_COMMAND(
         TARGET ${TPROJECT}
         POST_BUILD
			COMMAND echo "\\>\\>\\> Copying oftalmologia libs to dist/Plugins \\(${ARCH}\\)"
			COMMAND cp -a ${CMAKE_CURRENT_BINARY_DIR}/oftalmologia/liboftalmologia.so* "${DEST}/Plugins"
			COMMAND cp -a ${CMAKE_CURRENT_BINARY_DIR}/oftalmologia/oftalmologia.inf "${DEST}/Plugins"
			COMMAND echo "\\>\\>\\> changing RPATH of Plugins/liboftalmologia.so"
			COMMAND chrpath "${DEST}/Plugins/liboftalmologia.so" -d ">/dev/null"
			COMMAND echo "\\>\\>\\> Stripping oftalmologia libs"
			COMMAND strip -s "${DEST}/Plugins/liboftalmologia.so"
	
			COMMAND echo "\\>\\>\\> Installing translations"
			COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/oftalmologia/lang/es/oftal.mo" "${DEST}/lang/es"
		)
	ENDIF()

   IF(BUILD_SCANNER_EXT)
		ADD_CUSTOM_COMMAND(
         TARGET ${TPROJECT}
         POST_BUILD
         COMMAND echo "\\>\\>\\> Copying scanne libs to dist/Plugins \\(${ARCH}\\)"
         COMMAND cp -a ${CMAKE_CURRENT_BINARY_DIR}/scannerextension/libscannerextension.so* "${DEST}/Plugins"
			COMMAND cp -a ${CMAKE_CURRENT_BINARY_DIR}/scannerextension/scannerextension.inf "${DEST}/Plugins"
         COMMAND echo "\\>\\>\\> changing RPATH of Plugins/libscannerextension.so"
         COMMAND chrpath "${DEST}/Plugins/libscannerextension.so" -d ">/dev/null"
         COMMAND echo "\\>\\>\\> Stripping scannerextension libs"
         COMMAND strip -s "${DEST}/Plugins/libscannerextension.so"
   
         COMMAND echo "\\>\\>\\> Installing translations"
         COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/scannerextension/lang/es/scannerextension.mo" "${DEST}/lang/es"
      )
   ENDIF()


	IF(BUILD_DERMATHOLOGY_EXT OR BUILD_OPHTALMOLOGY_EXT OR BUILD_SCANNER_EXT)
		ADD_CUSTOM_COMMAND(
         TARGET ${TPROJECT}
         POST_BUILD
			COMMAND echo "\\>\\>\\> Installing translations"
			COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/lib_activation/lang/es/lib_activation.mo" "${DEST}/lang/es"
		)
	ENDIF()


ENDMACRO(PACKAGE_RULES)

#############################################################################################

MACRO(ADD_GINKGO_DEFINITIONS)
	
	ADD_GLOBAL_GINKGO_DEFINITIONS()
	
	ADD_DEFINITIONS(-DLINUX -DMONOLITIC)
	IF(CUSTOM_PACKAGE)
		ADD_DEFINITIONS(-DCUSTOM_PACKAGE)
	ENDIF()	
	
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

MACRO (SET_CUSTOM_libcurl_PACKAGE)
	SET(libcurl_FOUND TRUE)
	SET(libcurl_defines -DCURL_STATICLIB)
        SET(libcurl_INCLUDE_DIR
                ${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/libcurl-7.28.1/include
        )
        SET(libcurl_LIBRARIES curl)
	SET(libcurl_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/libcurl-7.28.1/lib/${DTYPE})
ENDMACRO()


MACRO(SET_CUSTOM_wxWidgets_PACKAGE)
	SET(wxWidgets_FOUND TRUE)
	SET(wxWidgets_defines -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -DHAVE_CONFIG_H -D_REENTRANT -DNDEBUG -DwxDEBUG_LEVEL=0)
	SET(wxWidgets_INCLUDE_DIR
		${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/WX-3.0.1/gtk2-unicode-${DTYPE}-3.0
		${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/WX-3.0.1/include
	)
	SET(wxWidgets_LIBRARIES wx_gtk2u-3.0 wx_gtk2u_gl-3.0)
	
	SET(wxWidgets_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/WX-3.0.1/lib/${DTYPE})
ENDMACRO()

MACRO(SET_CUSTOM_VTK_PACKAGE)
	SET(VTK_FOUND TRUE)
	SET(VTK_defines -DVTK_DEBUG_LEAKS)
	SET(VTK_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/VTK-5.6.1/include)
	SET(VTK_LIBRARIES
		vtkCommon vtkGraphics vtkFiltering vtkHybrid vtkIO vtkRendering vtkImaging
		vtkGenericFiltering vtkWidgets vtkftgl vtkVolumeRendering vtkexoIIc vtkDICOMParser
		vtkjpeg vtktiff vtkpng vtkzlib vtkexpat vtksys vtkfreetype
	)
	SET(VTK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/VTK-5.6.1/lib/${DTYPE})
ENDMACRO()

MACRO(SET_CUSTOM_ITK_PACKAGE)
	SET(ITK_FOUND TRUE)
	SET(ITK_defines)
	SET(ITK_INCLUDE_DIR
		${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/ITK-3.20.0/include
		${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/ITK-3.20.0/include/gdcm/src
		${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/ITK-3.20.0/include
		${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/ITK-3.20.0/include/Common
		${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/ITK-3.20.0/include/IO
		${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/ITK-3.20.0/include/BasicFilters
		${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/ITK-3.20.0/include/Algorithms
		${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/ITK-3.20.0/include/Numerics
		${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/ITK-3.20.0/include/Numerics/Statistics
		${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/ITK-3.20.0/include/Utilities
		${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/ITK-3.20.0/include/Utilities/vxl/core
		${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/ITK-3.20.0/include/Utilities/vxl/vcl
		${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/ITK-3.20.0/include/gdcm/src
	)
	SET(ITK_LIBRARIES
		ITKCommon ITKIO ITKMetaIO ITKAlgorithms itkgdcm itksys itkgdcm
		itkjpeg8 itkjpeg12 itkjpeg16 itktiff itkpng itkzlib itkopenjpeg
		ITKDICOMParser itkv3p_netlib itkvcl itkvnl itkvnl_algo itkvnl_inst
		ITKniftiio ITKStatistics ITKBasicFilters ITKNrrdIO ITKMetaIO ITKznz
	)
	SET(ITK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/ITK-3.20.0/lib/${DTYPE})
ENDMACRO()

MACRO(SET_CUSTOM_DCMTK_PACKAGE)
	SET(DCMTK_FOUND TRUE)
	SET(DCMTK_defines)
	SET(DCMTK_INCLUDE_DIR
		${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/DCMTK-3.6.0/include
	)
	SET(DCMTK_LIBRARIES
		dcmdata dcmtls dcmnet ofstd oflog dcmjpeg dcmimage
		dcmimgle dcmsr ijg8 ijg12 ijg16
	)
	SET(DCMTK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS}-${ARCH}/DCMTK-3.6.0/lib/${DTYPE})
ENDMACRO()
