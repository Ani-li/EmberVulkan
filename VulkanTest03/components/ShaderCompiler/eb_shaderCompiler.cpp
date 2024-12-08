#include "eb_shaderCompiler.hpp"

namespace eb
{
	bool EbShaderCompiler::compileShader()
	{
		int result = std::system("F:\\Project\\GraphicsProject\\VulkanTest03\\VulkanTest03\\shaders\\CompileShaderDebug.bat");

		if (result == 0) {
			std::cout << "Batch file executed successfully." << std::endl;
			return true;
		}
		else {
			std::cerr << "Failed to execute batch file. Error code: " << result << std::endl;
			return false;
		}
	}
}