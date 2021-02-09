#include "renderer.h"

namespace scimitar {
	Renderer::Renderer():
		System("Renderer")
	{
		add_dependency("OS");
	}
}
