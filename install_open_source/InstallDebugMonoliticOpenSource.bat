@echo off
set LANGUAGE_DIR=..\ginkgocadx\DebugMonoliticOS\lang
set RESOURCES_DIR=..\ginkgocadx\DebugMonoliticOS\resources
set CADXCORE_RESOURCES_DIR=%RESOURCES_DIR%\cadxcore
set VISUALIZATOR_RESOURCES_DIR=%RESOURCES_DIR%\visualizator
set CADXPRO_RESOURCES_DIR=%RESOURCES_DIR%\cadxpro
set DLL_DIR=..\ginkgocadx\DebugMonoliticOS

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

REM DLL's
copy ..\..\lib\Windows-x86\ITK-3.20.0\lib\Debug\*.dll %DLL_DIR%
copy ..\..\lib\Windows-x86\VTK-5.10.0\lib\Debug\*.dll %DLL_DIR%
copy ..\..\lib\Windows-x86\WX-3.0.0\lib\Debug\*.dll %DLL_DIR%
