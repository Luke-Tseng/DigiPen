@echo off
setlocal EnableDelayedExpansion

echo CS 260 ASSIGNMENT 3 AUTOMATION: %1

mkdir .\results\%1 > NUL 2>NUL

echo Building Debug for %1...
msbuild .\%1\CS260_Assignment3.vcxproj -p:Configuration=Debug > .\results\%1\build-debug.log 

IF %ERRORLEVEL% NEQ 0 ( 
   echo DEBUG BUILD FAILURE - UNABLE TO TEST!
   EXIT /B %ERRORLEVEL%
)

echo Building Release for %1...
msbuild .\%1\CS260_Assignment3.vcxproj -p:Configuration=Release > .\results\%1\build-release.log 

IF %ERRORLEVEL% NEQ 0 ( 
   echo RELEASE BUILD FAILURE - UNABLE TO TEST!
   EXIT /B %ERRORLEVEL%
)

echo Starting Release Proxy for %1...
start "%1 Proxy" .\%1\Release\CS260_Assignment3.exe 8888

echo Testing for %1...

CALL :ExecuteTest %1 Valid-Google-NoSlash http://www.google.com
CALL :ExecuteTest %1 Valid-Google-Slash http://www.google.com/
CALL :ExecuteTest %1 Valid-Delay1-DNS http://cs260.meancat.com/delay
CALL :ExecuteTest %1 Valid-Delay2-DNS http://cs260.meancat.com/delay
CALL :ExecuteTest %1 Valid-Delay3-DNS http://cs260.meancat.com/delay
CALL :ExecuteTest %1 Valid-Delay4-DNS http://cs260.meancat.com/delay
CALL :ExecuteTest %1 Valid-Delay-IP http://34.213.232.150/delay

echo Testing for %1 - complete!

EXIT /B %ERRORLEVEL%

:ExecuteTest
start "%1 %2" cmd /c .\CS260_Assignment3_Client.exe %3 8888 ^> .\results\%1\%2.txt 2^> .\results\%1\%2-error.txt
EXIT /B 0
