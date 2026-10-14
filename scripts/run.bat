if not exist ..\build\ mkdir ..\build
cd ..\build
cmake -G "Visual Studio 18 2026" ..
if %ERRORLEVEL% EQU 0 start ..\build\DataStructure.slnx