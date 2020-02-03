echo OFF

echo Forking deps
git clone https://github.com/glfw/glfw dep/GLFW
git clone https://github.com/g-truc/glm dep/GLM
git clone https://github.com/nlohmann/json dep/JSON
git clone https://github.com/nigels-com/glew dep/GLEW
git clone https://github.com/nothings/stb dep/stb

echo Building GLFW
cmake -S dep/GLFW -B dep/GLFW/bin
cmake --build dep/GLFW/bin --config Debug
cmake --build dep/GLFW/bin --config Release

echo Copying GLFW
xcopy dep\GLFW\src\Debug\glfw3.lib lib\GLFW\Debug\glfw3.lib
xcopy dep\GLFW\bin\src\Release\glfw3.lib lib\GLFW\Release\glfw3.lib
xcopy dep\GLFW\include\* inc\ /i /y

echo copying GLM
xcopy dep\GLM\src\* inc\GLM /i /y

echo copying stb
xcopy dep\stb\stb_image.h inc\stb\stb_image.h

type env\VS_PROJECT > Valkyrie-Engine.vcxproj
type env\VS_SOLUTION > Valkyrie-Engine.sln
