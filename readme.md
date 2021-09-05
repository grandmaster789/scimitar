# Scimitar GUI project
This was built as a reasonably high performance GUI framework, based on Vulkan. It should be cross-platform but it was mainly developed for Windows.

The project tries to use preprocessor definitions to figure out the capabilities of the system it's being build on. If it ever gets to the point where I would want to distribute this in a more binary format I'll have to think of something else but for the foreseeable future I feel this is a very reasonable approach.

Notable features:
- Vulkan
- heavy use of C++20 (concepts, constexpr, coroutines)

