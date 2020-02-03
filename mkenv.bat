echo OFF

echo Forking deps
git clone https://github.com/glfw/glfw dep/GLFW
git clone https://github.com/g-truc/glm dep/GLM
git clone https://github.com/nlohmann/json dep/JSON
git clone https://github.com/nigels-com/glew dep/GLEW

echo md lib\GLFW\Debug
echo md lib\GLFW\Release
echo md inc\GLFW

echo Building GLFW
cmake -S dep/GLFW -B dep/GLFW/bin
cmake --build dep/GLFW/bin --config Debug
cmake --build dep/GLFW/bin --config Release

echo Copying GLFW
move /Y dep\GLFW\bin\src\Debug\glfw3.lib lib\GLFW\Debug\glfw3.lib
move /Y dep\GLFW\bin\src\Release\glfw3.lib lib\GLFW\Release\glfw3.lib
xcopy dep\GLFW\include\* inc\ /i /e /y

type env\VS_PROJECT > Valkyrie-Engine.vcxproj
type env\VS_SOLUTION > Valkyrie-Engine.sln
