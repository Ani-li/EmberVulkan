#include <memory>
#include <vector>

#include "components/Setup/eb_setup.hpp"
#include "components/Renderer/eb_renderer.hpp"
#include "components/Camera/eb_camera.hpp"
#include "components/ShaderCompiler/eb_shaderCompiler.hpp"
using namespace eb;

static constexpr int WIDTH = 800;
static constexpr int HEIGHT = 800;

void run()
{
	EbShaderCompiler::compileShader();
	///////////////
	EbSetup ebSetup{ WIDTH, HEIGHT, "Hello Vulkan!" };
	EbRenderer ebRenderer{ ebSetup };
	EbCamera camera{ ebSetup.Init_window };

	camera.setPosition(glm::vec3(0.0f, 0.0f, -3.0f));
	//camera.setOrthographicProjection(-ebSetup.Init_windowAspectRatio, ebSetup.Init_windowAspectRatio, -1, 1, -1,1);
	camera.setPerspectiveProjection(glm::radians(50.0f), ebSetup.Init_windowAspectRatio, 0.1f, 10.0f);
	camera.setTarget(glm::vec3(0.0f, 0.0f, 0.0f));

	while (!ebSetup.windowShouldClose()) {
		glfwPollEvents();
		camera.controlView();
		ebRenderer.render(camera);
	}
	vkDeviceWaitIdle(ebSetup.Init_device);
}

int main()
{
	try {
		run();
	}
	catch (const std::exception& e) {
		throw std::runtime_error(e.what());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
