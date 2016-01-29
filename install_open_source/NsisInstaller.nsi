;Ginkgo Installation script
;Start Menu Folder Selection Example Script
;Written by Javier Tovar


;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"
  !include "Sections.nsh"
  !include "Registry.nsh"
  !include "LogicLib.nsh" ; Logical Library
  !include "..\Versions.nsh"
  
;--------------------------------
;Include FileAssociation
!include "FileAssociation.nsh"

;--------------------------------
;General

  ;definicion de variables
  !define RELEASE_DIR ".\ginkgoRelease"
    
  ;Name and file
  Name "Ginkgo CADx"
  OutFile "Ginkgo_CADx_installer.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\MetaEmotion\GinkgoCADx"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKLM "Software\Metaemotion\Ginkgo CADx" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin
  
  !define MUI_ICON ginkgocadx_installer.ico
  ;!define MUI_UNICON  gnkvideo_uninstall.ico


;--------------------------------
;Language Selection Dialog Settings

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKLM" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\Ginkgo CADx" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"
  
;--------------------------------
;Variables

  Var StartMenuFolder

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  BrandingText "Ginkgo CADx ${VERSION_GINKGO}"
  
  !insertmacro MUI_PAGE_WELCOME
  !define MUI_LICENSEPAGE_CHECKBOX
  !insertmacro MUI_PAGE_LICENSE "GINKGO_CADX_LICENSE.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  
  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Ginkgo CADx" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Ginkgo CADx"
  
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
  
  !insertmacro MUI_PAGE_INSTFILES
  !define MUI_FINISHPAGE_RUN
  !define MUI_FINISHPAGE_RUN_FUNCTION "LaunchLink"
  !define MUI_FINISHPAGE_LINK "Ginkgo CADx website"
  !define MUI_FINISHPAGE_LINK_LOCATION http://ginkgo-cadx.com
  !insertmacro MUI_PAGE_FINISH
  
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;function launch
Function LaunchLink
  ExecShell "" "$INSTDIR\ginkgocadx.exe"
FunctionEnd

;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "Spanish" ;first language is the default language
  !insertmacro MUI_LANGUAGE "English" ;
  
;--------------------------------
;Reserve Files
  
  ;If you are using solid compression, files that are required before
  ;the actual installation should be stored first in the data block,
  ;because this will make your installer start faster.
  
  !insertmacro MUI_RESERVEFILE_LANGDLL


;--------------------------------
; Strings
  
  ;Microsoft Redistributable

  LangString DESC_SecGinkgo ${LANG_SPANISH} "Ginkgo CADx framework."
  LangString PRODUCT_NAME ${LANG_SPANISH} "Ginkgo CADx framework."
  LangString DESC_ELIMINARCONFIGURACION_DECISION ${LANG_SPANISH} "¿Desea eliminar la configuración de Ginkgo CADx y los estudios importados$\nen la base de datos por defecto?"
  LangString MSG_GINKGO_ALREADY_INSTALLED ${LANG_SPANISH} "Ginkgo CADx ya está instalado. $\n$\nHaga click en `Aceptar` para desinstalar la versión anterior o `Cancel` para cancelar esta actualización."
  LangString MSG_HA_HABIDO_ERRORES ${LANG_SPANISH} "Ha ocurrido un error al desinstalar Ginkgo CADx ¿Desea continuar instalando la nueva versión?"
  LangString MSG_GINKGO_IS_RUNNING ${LANG_SPANISH} "Ginkgo CADx Pro está en ejecución. Cierre los procesos y reintente la instalación."
  
  ;Microsoft Redistributable  
  LangString DESC_SecGinkgo ${LANG_ENGLISH} "Ginkgo CADx framework."
  LangString PRODUCT_NAME ${LANG_ENGLISH} "Ginkgo CADx framework."
  
  LangString DESC_ELIMINARCONFIGURACION_DECISION ${LANG_ENGLISH} "Would you like to delete Ginkgo CADx configuration and $\nimported studies stored in the default database?"
  LangString MSG_GINKGO_ALREADY_INSTALLED ${LANG_ENGLISH}  "Ginkgo CADx is already installed. $\n$\nClick `Accept` to remove the previous version or `Cancel` to cancel this upgrade."
  LangString MSG_HA_HABIDO_ERRORES ${LANG_ENGLISH} "There have been an error uninstalling Ginkgo CADx ¿Do you want to continue installing new version?."
  LangString MSG_GINKGO_IS_RUNNING ${LANG_ENGLISH} "Ginkgo CADx Pro is running. Close running processes and retry."
;--------------------------------
;URI Handling
 
; If ginkgocadx is the current URI handler for the specified protocol, remove it.
Function un.UnregisterURIHandler
  Exch $R0
 
  ;Unregister the URI handler
  DetailPrint "Unregistering $R0 URI Handler"
  DeleteRegKey HKCR "$R0"
 
  done:
  Pop $R0
FunctionEnd
 
Function RegisterURIHandler
  Exch $R0
  DetailPrint "Registering $R0 URI Handler"
  DeleteRegKey HKCR "$R0"
  WriteRegStr HKCR "$R0" "" "URL:$R0"
  WriteRegStr HKCR "$R0" "URL Protocol" ""
  WriteRegStr HKCR "$R0\DefaultIcon" "" "$INSTDIR\ginkgocadx.exe,0"
  WriteRegStr HKCR "$R0\shell" "" ""
  WriteRegStr HKCR "$R0\shell\Open" "" ""
  WriteRegStr HKCR "$R0\shell\Open\command" "" "$INSTDIR\ginkgocadx.exe %1"
  Pop $R0
FunctionEnd

;Installer Sections

Section "Ginkgo CADx" SecGinkgo

  
  ;ejecutable + dlls
  AllowSkipFiles off
   ;SetOutPath "$INSTDIR\Plugins"
   ;File /r "${RELEASE_DIR}\Plugins\"
   
   SetOutPath "$INSTDIR\lang\de"
   File "${RELEASE_DIR}\lang\de\*.*" 
   
   SetOutPath "$INSTDIR\lang\el"
   File "${RELEASE_DIR}\lang\el\*.*" 
   
   SetOutPath "$INSTDIR\lang\es"
   File "${RELEASE_DIR}\lang\es\*.*" 
   
   SetOutPath "$INSTDIR\lang\ja"
   File "${RELEASE_DIR}\lang\ja\*.*" 
   
   SetOutPath "$INSTDIR\lang\pl"
   File "${RELEASE_DIR}\lang\pl\*.*" 
   
   SetOutPath "$INSTDIR\lang\pt"
   File "${RELEASE_DIR}\lang\pt\*.*" 

   SetOutPath "$INSTDIR\lang\pt_BR"
   File "${RELEASE_DIR}\lang\pt_BR\*.*" 
   
   SetOutPath "$INSTDIR\lang\fr"
   File "${RELEASE_DIR}\lang\fr\*.*" 
   
   SetOutPath "$INSTDIR\lang\sr"
   File "${RELEASE_DIR}\lang\sr\*.*" 
   
   SetOutPath "$INSTDIR\lang\tr"
   File "${RELEASE_DIR}\lang\tr\*.*" 
   
   SetOutPath "$INSTDIR\resources"
   File /r "${RELEASE_DIR}\resources\"   
   
   SetOutPath "$INSTDIR"
   File  /oname=ginkgocadx.exe "${RELEASE_DIR}\ginkgocadx.exe" 
   File "${RELEASE_DIR}\*.dll" 
   File "${RELEASE_DIR}\*.manifest" 
   File "${RELEASE_DIR}\*.xml" 
   File "GINKGO_CADX_LICENSE.txt"
   File "LGPL3.txt"
   
   
  ;Store installation folder
  WriteRegStr HKLM "Software\Ginkgo CADx" "" $INSTDIR
  WriteRegStr HKLM "Software\Ginkgo CADx" "version" "${VERSION_GINKGO}"
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    SetShellVarContext all
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall Ginkgo CADx.lnk" "$INSTDIR\Uninstall.exe"

    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Ginkgo CADx website.lnk" "http://ginkgo-cadx.com" "" "$WINDIR\System32\SHELL32.dll" 13
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Ginkgo CADx.lnk" "$INSTDIR\ginkgocadx.exe"
    
    ${registerExtension} "$INSTDIR\ginkgocadx.exe" ".gxml" "Ginkgo Integration CADx file"
    ${registerExtension} "$INSTDIR\ginkgocadx.exe" ".gkxml" "Ginkgo Integration CADx file"
    ${registerExtension} "$INSTDIR\ginkgocadx.exe" ".dcm" "Ginkgo CADx file"
	
	Push "ginkgocadx"
    Call RegisterURIHandler
  
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

Function .onInit
;test local
;Language selection dialog
  !insertmacro MUI_LANGDLL_DISPLAY
  
  ;para cuando queramos poner la imagen de bienvenida/fin MUI_WELCOMEFINISHPAGE_BITMAP 
FindProcDLL::FindProc "ginkgocadx.exe"
IntCmp $R0 1 0 notRunning
    MessageBox MB_OK|MB_ICONEXCLAMATION $(MSG_GINKGO_IS_RUNNING) /SD IDOK
    Abort
notRunning:

ReadRegStr $R0 HKLM \
  "Software\Ginkgo CADx" \
  ""
StrCmp $R0 "" testcurrentuser tryuninstall
  
;test current user
testcurrentuser:
ReadRegStr $R0 HKCU \
  "Software\Ginkgo CADx" \
  ""
StrCmp $R0 "" uninst_Done

 tryuninstall:
  MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION  $(MSG_GINKGO_ALREADY_INSTALLED) /SD IDOK IDOK uninst
  Abort
 
;Run the uninstaller
uninst:
ClearErrors
  ExecWait '$R0\uninstall.exe /S _?=$R0' $R2
  ;if uninstall status != 0... error
  ExecWait '$R0\uninstall.exe /S' 
  Sleep 1000
  
  StrCmp $R2 0 uninst_Done
  MessageBox MB_YESNO|MB_ICONEXCLAMATION $(MSG_HA_HABIDO_ERRORES) /SD IDYES IDYES uninst_Done
  Abort
  
   
uninst_Done:
  

;  !define MUI_LANGDLL_WINDOWTITLE "$LANGUAGE_DLL_TITLE"
;  !define MUI_LANGDLL_INFO "$LANGUAGE_DLL_INFO"
;  !insertmacro MUI_LANGDLL_DISPLAY
;  !undef MUI_LANGDLL_WINDOWTITLE
;  !undef MUI_LANGDLL_INFO
  InitPluginsDir
  SetOutPath "$PLUGINSDIR"
  ;File "Common\Plugins\*.*"
  File /r "${NSISDIR}\Plugins\*.*"
FunctionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall" 
;respetar configuracion...
  ;RMDir /r "$INSTDIR"
  ;RMDir /r "$INSTDIR\Plugins"
  RMDir /r "$INSTDIR\lang"
  RMDir /r "$INSTDIR\resources"
  
  Delete "$INSTDIR\*.exe"
  Delete "$INSTDIR\*.dll"
  Delete "$INSTDIR\*.manifest"
  Delete "$INSTDIR\*.xml"
  Delete "$INSTDIR\GINKGO_CADX_LICENSE.txt"
  Delete "$INSTDIR\LGPL3.txt"
  
  RMDir "$INSTDIR"
	
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    
  SetShellVarContext all
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall Ginkgo CADx.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Ginkgo CADx website.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Ginkgo CADx.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"
  
  ${unregisterExtension} ".gxml" "Ginkgo CADx Integration file"
  ${unregisterExtension} ".gkxml" "Ginkgo CADx Integration file"
  ${unregisterExtension} ".dcm" "Ginkgo CADx file"
  
  
  DeleteRegKey /ifempty HKLM "Software\Ginkgo CADx"
  ;unregister URI handler
  Push "ginkgocadx"
  Call un.UnregisterURIHandler
  
  ;se le pregunta si desea eliminar la configuración actual
  MessageBox MB_ICONEXCLAMATION|MB_YESNO|MB_DEFBUTTON2 "$(DESC_ELIMINARCONFIGURACION_DECISION)" /SD IDNO IDYES 0 IDNO enduninstall
  ; "Downloading Microsoft Visual C++ 2008 Redistributable Package"
  Delete "$PROFILE\Ginkgo.ini"
  Delete "$PROFILE\GinkgoUser.ini"
  
  ;remove ginkgocadx dir
  ReadRegStr $R0 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
 
  ${If} $R0 < 6
	;version less than windows vista
	RMDir /r "$APPDATA\ginkgocadx"
  ${Else}
	;windows vista, 7, 8...
    RMDir /r "$PROFILE\AppData\Roaming\ginkgocadx"
  ${EndIf}
 enduninstall:
	
SectionEnd



;--------------------------------
;Uninstaller Functions

Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE
  
FunctionEnd

;--------------------------------
;Descriptions

  ;Language strings
  

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN    
    !insertmacro MUI_DESCRIPTION_TEXT ${SecGinkgo} $(DESC_SecGinkgo)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

