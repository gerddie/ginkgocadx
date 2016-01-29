rem @echo off

set TARGET="WIN32"
IF "%TARGET%"=="x64" (set ARCH=x64) ELSE (set ARCH=x86)
echo Installing %TARGET% (Rel \%ARCH%\)

set CORE_PATH=..\ginkgocadx\%TARGET%\ReleaseMonoliticOpenSource
set LANGUAGE_DIR=%CORE_PATH%\lang
set RESOURCES_DIR=%CORE_PATH%\resources
set CADXCORE_RESOURCES_DIR=%RESOURCES_DIR%\cadxcore
set VISUALIZATOR_RESOURCES_DIR=%RESOURCES_DIR%\visualizator
set COMPILATION=Internet

REM @echo off
set DLL_DIR=%CORE_PATH%
copy ..\..\lib\Windows-x86\ITK-3.20.0\lib\Release\*.dll %DLL_DIR%
copy ..\..\lib\Windows-x86\VTK-5.10.0\lib\release\*.dll %DLL_DIR%

set RELEASE_DIR=.\ginkgoRelease

set LANGUAGE_DIR_RELEASE=%RELEASE_DIR%\lang
set RESOURCES_DIR_RELEASE=%RELEASE_DIR%\resources
set DLL_DIR=%RELEASE_DIR%
set MPRESS_EXECUTABLE=..\..\utils\mpress.exe
set UPX_SENTENCE=..\..\utils\upx -9 
set NSIS_SENTENCE="..\..\utils\nsis\makensis.exe"
set ZIP_SENTENCE="..\..\utils\7z.exe"

mkdir %RESOURCES_DIR% 2>NUL
mkdir %CADXCORE_RESOURCES_DIR% 2>NUL
mkdir %VISUALIZATOR_RESOURCES_DIR% 2>NUL
xcopy /y /s ..\cadxcore\resources\files\*.* %CADXCORE_RESOURCES_DIR%
xcopy /y /s ..\visualizator\resources\files\*.* %VISUALIZATOR_RESOURCES_DIR%

mkdir %LANGUAGE_DIR% 2>NUL

mkdir %LANGUAGE_DIR%\de 2>NUL
copy ..\visualizator\lang\de\visualizator.mo %LANGUAGE_DIR%\de
copy ..\cadxcore\lang\de\cadxcore.mo %LANGUAGE_DIR%\de
copy ..\cadxcore\lang\de\wxstd.mo %LANGUAGE_DIR%\de

mkdir %LANGUAGE_DIR%\el 2>NUL
copy ..\visualizator\lang\el\visualizator.mo %LANGUAGE_DIR%\el
copy ..\cadxcore\lang\el\cadxcore.mo %LANGUAGE_DIR%\el
copy ..\cadxcore\lang\el\wxstd.mo %LANGUAGE_DIR%\el

mkdir %LANGUAGE_DIR%\es 2>NUL
copy ..\visualizator\lang\es\visualizator.mo %LANGUAGE_DIR%\es
copy ..\cadxcore\lang\es\cadxcore.mo %LANGUAGE_DIR%\es
copy ..\cadxcore\lang\es\wxstd.mo %LANGUAGE_DIR%\es

mkdir %LANGUAGE_DIR%\fr 2>NUL
copy ..\visualizator\lang\fr\visualizator.mo %LANGUAGE_DIR%\fr
copy ..\cadxcore\lang\fr\cadxcore.mo %LANGUAGE_DIR%\fr
copy ..\cadxcore\lang\fr\wxstd.mo %LANGUAGE_DIR%\fr

mkdir %LANGUAGE_DIR%\ja 2>NUL
copy ..\visualizator\lang\ja\visualizator.mo %LANGUAGE_DIR%\ja
copy ..\cadxcore\lang\ja\cadxcore.mo %LANGUAGE_DIR%\ja
copy ..\cadxcore\lang\ja\wxstd.mo %LANGUAGE_DIR%\ja

mkdir %LANGUAGE_DIR%\pl 2>NUL
copy ..\visualizator\lang\pl\visualizator.mo %LANGUAGE_DIR%\pl
copy ..\cadxcore\lang\pl\cadxcore.mo %LANGUAGE_DIR%\pl
copy ..\cadxcore\lang\pl\wxstd.mo %LANGUAGE_DIR%\pl

mkdir %LANGUAGE_DIR%\pt 2>NUL
copy ..\visualizator\lang\pt\visualizator.mo %LANGUAGE_DIR%\pt
copy ..\cadxcore\lang\pt\cadxcore.mo %LANGUAGE_DIR%\pt
copy ..\cadxcore\lang\pt\wxstd.mo %LANGUAGE_DIR%\pt

mkdir %LANGUAGE_DIR%\pt_BR 2>NUL
copy ..\visualizator\lang\pt_BR\visualizator.mo %LANGUAGE_DIR%\pt_BR
copy ..\cadxcore\lang\pt_BR\cadxcore.mo %LANGUAGE_DIR%\pt_BR
copy ..\cadxcore\lang\pt_BR\wxstd.mo %LANGUAGE_DIR%\pt_BR

mkdir %LANGUAGE_DIR%\sr 2>NUL
copy ..\visualizator\lang\sr\visualizator.mo %LANGUAGE_DIR%\sr
copy ..\cadxcore\lang\sr\cadxcore.mo %LANGUAGE_DIR%\sr

mkdir %LANGUAGE_DIR%\tr 2>NUL
copy ..\visualizator\lang\tr\visualizator.mo %LANGUAGE_DIR%\tr
copy ..\cadxcore\lang\tr\cadxcore.mo %LANGUAGE_DIR%\tr
copy ..\cadxcore\lang\tr\wxstd.mo %LANGUAGE_DIR%\tr

mkdir %LANGUAGE_DIR%\zh_CN 2>NUL
copy ..\visualizator\lang\zh_CN\visualizator.mo %LANGUAGE_DIR%\zh_CN
copy ..\cadxcore\lang\zh_CN\cadxcore.mo %LANGUAGE_DIR%\zh_CN
copy ..\cadxcore\lang\zh_CN\wxstd.mo %LANGUAGE_DIR%\zh_CN

REM A PARTIR DE AQUI GENERAMOS LA VERSION RELEASE

mkdir %RELEASE_DIR% 2>NUL
REM LANGUAGE
mkdir  %LANGUAGE_DIR_RELEASE% 2>NUL

mkdir %LANGUAGE_DIR_RELEASE%\de 2>NUL
copy ..\visualizator\lang\de\visualizator.mo %LANGUAGE_DIR_RELEASE%\de
copy ..\cadxcore\lang\de\cadxcore.mo %LANGUAGE_DIR_RELEASE%\de
copy ..\cadxcore\lang\de\wxstd.mo %LANGUAGE_DIR_RELEASE%\de

mkdir %LANGUAGE_DIR_RELEASE%\el 2>NUL
copy ..\visualizator\lang\el\visualizator.mo %LANGUAGE_DIR_RELEASE%\el
copy ..\cadxcore\lang\el\cadxcore.mo %LANGUAGE_DIR_RELEASE%\el
copy ..\cadxcore\lang\el\wxstd.mo %LANGUAGE_DIR_RELEASE%\el

mkdir %LANGUAGE_DIR_RELEASE%\es 2>NUL
copy ..\visualizator\lang\es\visualizator.mo %LANGUAGE_DIR_RELEASE%\es
copy ..\cadxcore\lang\es\cadxcore.mo %LANGUAGE_DIR_RELEASE%\es
copy ..\cadxcore\lang\es\wxstd.mo %LANGUAGE_DIR_RELEASE%\es

mkdir %LANGUAGE_DIR_RELEASE%\pl 2>NUL
copy ..\visualizator\lang\pl\visualizator.mo %LANGUAGE_DIR_RELEASE%\pl
copy ..\cadxcore\lang\pl\cadxcore.mo %LANGUAGE_DIR_RELEASE%\pl
copy ..\cadxcore\lang\pl\wxstd.mo %LANGUAGE_DIR_RELEASE%\pl

mkdir %LANGUAGE_DIR_RELEASE%\ja 2>NUL
copy ..\visualizator\lang\ja\visualizator.mo %LANGUAGE_DIR_RELEASE%\ja
copy ..\cadxcore\lang\ja\cadxcore.mo %LANGUAGE_DIR_RELEASE%\ja
copy ..\cadxcore\lang\ja\wxstd.mo %LANGUAGE_DIR_RELEASE%\ja

mkdir %LANGUAGE_DIR_RELEASE%\pt 2>NUL
copy ..\visualizator\lang\pt\visualizator.mo %LANGUAGE_DIR_RELEASE%\pt
copy ..\cadxcore\lang\pt\cadxcore.mo %LANGUAGE_DIR_RELEASE%\pt
copy ..\cadxcore\lang\pt\wxstd.mo %LANGUAGE_DIR_RELEASE%\pt

mkdir %LANGUAGE_DIR_RELEASE%\pt_BR 2>NUL
copy ..\visualizator\lang\pt_BR\visualizator.mo %LANGUAGE_DIR_RELEASE%\pt_BR
copy ..\cadxcore\lang\pt_BR\cadxcore.mo %LANGUAGE_DIR_RELEASE%\pt_BR
copy ..\cadxcore\lang\pt_BR\wxstd.mo %LANGUAGE_DIR_RELEASE%\pt_BR

mkdir %LANGUAGE_DIR_RELEASE%\fr 2>NUL
copy ..\visualizator\lang\fr\visualizator.mo %LANGUAGE_DIR_RELEASE%\fr
copy ..\cadxcore\lang\fr\cadxcore.mo %LANGUAGE_DIR_RELEASE%\fr
copy ..\cadxcore\lang\fr\wxstd.mo %LANGUAGE_DIR_RELEASE%\fr

mkdir %LANGUAGE_DIR_RELEASE%\sr 2>NUL
copy ..\visualizator\lang\sr\visualizator.mo %LANGUAGE_DIR_RELEASE%\sr
copy ..\cadxcore\lang\sr\cadxcore.mo %LANGUAGE_DIR_RELEASE%\sr

mkdir %LANGUAGE_DIR_RELEASE%\tr 2>NUL
copy ..\visualizator\lang\tr\visualizator.mo %LANGUAGE_DIR_RELEASE%\tr
copy ..\cadxcore\lang\tr\cadxcore.mo %LANGUAGE_DIR_RELEASE%\tr
copy ..\cadxcore\lang\tr\wxstd.mo %LANGUAGE_DIR_RELEASE%\tr

mkdir %RESOURCES_DIR_RELEASE% 2>NUL
xcopy /s %RESOURCES_DIR%\*.* %RESOURCES_DIR_RELEASE% 1>NUL

REM DLL's
mkdir %DLL_DIR% 2>NUL
copy ..\..\lib\Windows-x86\ITK-3.20.0\lib\Release\*.dll %DLL_DIR%
copy ..\..\lib\Windows-x86\VTK-5.10.0\lib\release\*.dll %DLL_DIR%
copy ..\..\lib\Windows-x86\Microsoft.crt\bin\*.dll %DLL_DIR%
copy ..\..\lib\Windows-x86\Microsoft.crt\bin\*.manifest %DLL_DIR%

REM EJECUTABLE
copy %CORE_PATH%\ginkgocadx.exe %RELEASE_DIR%\
copy %CORE_PATH%\conformance.xml %RELEASE_DIR%\
copy GINKGO_CADX_LICENSE.txt %RELEASE_DIR%\
copy LGPL3.txt %RELEASE_DIR%\

REM mpress (lo desactivo porque da alerta)
REM %MPRESS_EXECUTABLE% %RELEASE_DIR%\Ginkgo_CADx.exe

REM UPX para las dlls
REM %UPX_SENTENCE% %RELEASE_DIR%\cadxcore.dll

REM No se les pasa UPX

%NSIS_SENTENCE% NsisInstaller.nsi

REM compress folder
echo "comprimiendo directorio..."
move %RELEASE_DIR% GinkgoCADx
%ZIP_SENTENCE% a -tzip GinkgoCADx.zip GinkgoCADx
rmdir /S /Q GinkgoCADx

REM echo "moviendo fichero comprimido..."
REM move GinkgoCADx.zip ..\..\..\dist\0-ESTABLE\%COMPILATION%\
REM echo "moviendo fichero ejecutable..."
REM move Ginkgo_CADx-*.exe ..\..\..\dist\0-ESTABLE\%COMPILATION%\
