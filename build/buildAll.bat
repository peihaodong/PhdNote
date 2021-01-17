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
call :FileCopy CAdoLx
call :FileCopy PhdIni
call :FileCopy PhdExcel
call :FileCopy PhdSocket
call :FileCopy PhdRegedit
call :FileCopy PhdXml
call :FileCopy PhdConver

:error
pause

rem 头文件拷贝函数
:FileCopy
copy /y ..\PhdNote\%1.h ..\API_PhdNote\Debug\include\%1.h
copy /y ..\PhdNote\%1.h ..\API_PhdNote\Release\include\%1.h
goto :eof
