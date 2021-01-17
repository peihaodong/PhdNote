rem 拷贝.h文件
call :FileCopy CAdoLx
call :FileCopy PhdIni
call :FileCopy PhdExcel
call :FileCopy PhdSocket
call :FileCopy PhdRegedit
call :FileCopy PhdXml
call :FileCopy PhdConver

pause
exit /b 0

rem 文件拷贝函数
:FileCopy
copy /y ..\PhdNote\%1.h ..\API_PhdNote\Debug\include\%1.h
copy /y ..\PhdNote\%1.h ..\API_PhdNote\Release\include\%1.h
goto :eof

