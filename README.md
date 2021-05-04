## Experimental C++ 'Game Engine'

[![](https://i.imgur.com/ZS8kvhm.png)](https://i.imgur.com/ZS8kvhm.png)

This project started as a small C 'Game Engine' to make me more familiar with OpenGL, GLSL, HLSL, SPIR-V and [SPIRV-Cross](https://github.com/KhronosGroup/SPIRV-Cross "SPIRV-Cross"). Turns out i'm incredibly incompetent at keeping things simple and now this is a C++ project ?

This 'Engine' swallows SPIR-V as a shading language and automagically converts it to GLSL for rendering. The additional reflection provided by SPIRV-Cross allows the shaders to be modified at load-time to be compliant with the (currently inexistant) material system.
In theory this would also allow for the Renderer to use a different API than OpenGL for rendering without having to tweak any shader code or anything, but if I don't want to be absorbed by the dark abyss of overengineering and being able to live with the privilege of seeing the light of day and having a decent education while eating and taking showers, I have all the reasons in the world to not even think about implementing it.