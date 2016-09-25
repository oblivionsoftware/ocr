
/*
 * Copyright 2016 Jeff Upton
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <GL/glew.h>

#ifdef WIN32
	#include <GL/wglew.h>
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    #include <gl/GL.h>
    #include <gl/GLU.h>
#elif defined(__APPLE__)
    #include <OpenGL/OpenGL.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#ifndef APIENTRY
    #define APIENTRY
#endif

