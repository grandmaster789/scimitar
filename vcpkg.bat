@echo off

if /I [%1]==[clean] goto clean
if /I [%1]==[build] goto build
if /I [%1]==[restore] goto restore

echo Usage: vcpkg (build / restore / clean)
goto exit

:build

cd dependencies

if not exist vcpkg.windows git clone https://github.com/Microsoft/vcpkg.git vcpkg.windows
cd vcpkg.windows

if not exist vcpkg.exe call bootstrap-vcpkg.bat || goto error

vcpkg.exe install ^
	concurrentqueue:x64-windows ^
	gtest:x64-windows ^
	glfw3:x64-windows ^
	ms-gsl:x64-windows ^
	cppcoro:x64-windows ^
	nlohmann-json:x64-windows ^
	tinyobjloader:x64-windows ^
	fmt:x64-windows ^
	--clean-after-build

rem NOTE you'll need to install the Vulkan SDK separately, I did not find a way to use vcpkg for that...
rem The buildtrees folder holds a *lot* of data, and can safely be removed while keeping the installed libraries intact

if exist buildtrees rd buildtrees /s /q

cd ..
cd ..

goto exit

:clean

cd dependencies
if exist vcpkg.windows (
	cd vcpkg.windows

	rem This removes all package-related files and installed libaries and retains the vcpkg executable

	if exist buildtrees rd buildtrees /s /q
	if exist packages rd packages /s /q
	if exist installed rd installed /s /q
	if exist downloads rd downloads /s /q

	cd ..
)
cd ..

goto exit

:restore

cd dependencies

if not exist vcpkg.windows git clone https://github.com/Microsoft/vcpkg.git vcpkg.windows
cd vcpkg.windows

powershell -Command Expand-Archive -Force ..\vcpkg.windows.zip .

cd ..
cd ..

goto exit

:exit
