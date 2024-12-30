@echo off
:: Definir variables
set ProjectName=NI2SUnreal
set UprojectFile=%ProjectName%.uproject
set BuildCommand=RunUAT BuildCookRun -project="%cd%\%UprojectFile%" -server -build -cook -stage -pak -archive

:: Ejecutar comando de compilación
echo Compilando el servidor dedicado para %ProjectName%...
%BuildCommand%

:: Esperar a que el proceso finalice
pause
