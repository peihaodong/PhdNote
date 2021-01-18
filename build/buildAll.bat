echo off

Set Msbuild=c:\Windows\Microsoft.NET\Framework\v4.0.30319\MsBuild.exe 

rem 编译 Debug x86
"%MsBuild%" /p:Configuration=Debug;Platform=x86 PhdNote.msbuildproj
if errorlevel 1 goto error
rem 编译 Debug x64
"%MsBuild%" /p:Configuration=Debug;Platform=x64 PhdNote.msbuildproj
if errorlevel 1 goto error
rem 编译 Release x86
"%MsBuild%" /p:Configuration=Release;Platform=x86 PhdNote.msbuildproj
if errorlevel 1 goto error
rem 编译 Release x64
"%MsBuild%" /p:Configuration=Release;Platform=x64 PhdNote.msbuildproj
if errorlevel 1 goto error


rem 拷贝头文件
call :IncFileCopy PhdNoteDynamicLink

call :IncFileCopy CAdoLx
call :IncFileCopy PhdSocket
call :IncFileCopy PhdRegedit
call :IncFileCopy PhdExcel
call :IncFileCopy PhdIni
call :IncFileCopy PhdXml
call :IncFileCopy PhdConver

rem 拷贝dll文件和lib文件
xcopy /y ..\Debug\PhdNote.dll ..\API_PhdNote\Debug\bin-Win32\
xcopy /y ..\Debug\PhdNote.lib ..\API_PhdNote\Debug\lib-Win32\
xcopy /y ..\Release\PhdNote.dll ..\API_PhdNote\Release\bin-Win32\
xcopy /y ..\Release\PhdNote.lib ..\API_PhdNote\Release\lib-Win32\
xcopy /y ..\x64\Debug\PhdNote.dll ..\API_PhdNote\Debug\bin-x64\
xcopy /y ..\x64\Debug\PhdNote.lib ..\API_PhdNote\Debug\lib-x64\
xcopy /y ..\x64\Release\PhdNote.dll ..\API_PhdNote\Release\bin-x64\
xcopy /y ..\x64\Release\PhdNote.lib ..\API_PhdNote\Release\lib-x64\

:error
pause

rem 头文件拷贝函数
:IncFileCopy
copy /y ..\PhdNote\%1.h ..\API_PhdNote\Debug\include\%1.h
copy /y ..\PhdNote\%1.h ..\API_PhdNote\Release\include\%1.h
goto :eof
