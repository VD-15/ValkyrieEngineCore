echo OFF

echo Forking deps
git clone https://github.com/glfw/glfw dep/GLFW
git clone https://github.com/g-truc/glm dep/GLM
git clone https://github.com/nlohmann/json dep/json
git clone https://github.com/nigels-com/glew dep/GLEW
git clone https://github.com/nothings/stb dep/stb

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
xcopy dep\GLM\src\* inc\GLM /i /y /e

echo copying stb
md inc\stb
copy dep\stb\stb_image.h inc\stb\stb_image.h

echo copying json
md inc\json
xcopy dep\json\include\nlohmann\* inc\json /i /y /e

type env\VS_PROJECT > Valkyrie-Engine.vcxproj
type env\VS_SOLUTION > Valkyrie-Engine.sln
