echo OFF

echo Please confirm you have cmake and git installed or this script won't work.

echo Forking deps
git clone https://github.com/glfw/glfw dep/GLFW
git clone https://github.com/g-truc/glm dep/GLM
git clone https://github.com/nlohmann/json dep/json
git clone https://github.com/Perlmint/glew-cmake dep/GLEW
git clone https://github.com/nothings/stb dep/stb

echo building GLEW
cd dep\GLEW
cmake-testbuild.sh
cd ..\..

echo copying GLEW
md lib\GLEW\Debug
md lib\GLEW\Release
copy dep\GLEW\out\lib\libglew32d.lib lib\GLEW\Debug\libglew32.lib
copy dep\GLEW\out\lib\libglew32.lib lib\GLEW\Release\libglew32.lib

echo Building GLFW
cmake -S dep/GLFW -B dep/GLFW/bin
cmake --build dep/GLFW/bin --config Debug
cmake --build dep/GLFW/bin --config Release

echo Copying GLFW
md lib\GLFW\Debug
md lib\GLFW\Release
copy dep\GLFW\bin\src\Debug\glfw3.lib lib\GLFW\Debug\glfw3.lib
copy dep\GLFW\bin\src\Release\glfw3.lib lib\GLFW\Release\glfw3.lib
xcopy dep\GLFW\include\* inc\ /i /y /e

echo copying GLM
md inc\GLM
xcopy dep\GLM\glm\* inc\GLM /i /y /e

echo copying stb
md inc\stb
copy dep\stb\stb_image.h inc\stb\stb_image.h

echo copying json
md inc\json
xcopy dep\json\include\nlohmann\* inc\nlohmann /i /y /e

type env\VS_PROJECT > Valkyrie-Engine.vcxproj
type env\VS_SOLUTION > Valkyrie-Engine.sln
