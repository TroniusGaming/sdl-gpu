#ifndef _SDL_GPU_GLES_2_H__
#define _SDL_GPU_GLES_2_H__

#include "SDL_gpu.h"
#ifdef USING_SDL3
#include "SDL3/SDL_platform.h"
#else
#include "SDL_platform.h"
#endif

#if !defined(SDL_GPU_DISABLE_GLES) && !defined(SDL_GPU_DISABLE_GLES_2)

#ifdef __IPHONEOS__
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
#else
    #include "GLES2/gl2.h"
    #include "GLES2/gl2ext.h"
#endif

	#define glVertexAttribI1i glVertexAttrib1f
	#define glVertexAttribI2i glVertexAttrib2f
	#define glVertexAttribI3i glVertexAttrib3f
	#define glVertexAttribI4i glVertexAttrib4f
	#define glVertexAttribI1ui glVertexAttrib1f
	#define glVertexAttribI2ui glVertexAttrib2f
	#define glVertexAttribI3ui glVertexAttrib3f
    #define glVertexAttribI4ui glVertexAttrib4f
    #define glMapBuffer glMapBufferOES
    #define glUnmapBuffer glUnmapBufferOES
    #define GL_WRITE_ONLY GL_WRITE_ONLY_OES
#endif



#define GPU_CONTEXT_DATA ContextData_GLES_2
#define GPU_IMAGE_DATA ImageData_GLES_2
#define GPU_TARGET_DATA TargetData_GLES_2


#define GPU_DEFAULT_TEXTURED_VERTEX_SHADER_SOURCE \
"#version 100\n\
precision highp float;\n\
precision mediump int;\n\
\
attribute vec2 gpu_Vertex;\n\
attribute vec2 gpu_TexCoord;\n\
attribute mediump vec4 gpu_Color;\n\
uniform mat4 gpu_ModelViewProjectionMatrix;\n\
\
varying mediump vec4 color;\n\
varying vec2 texCoord;\n\
\
void main(void)\n\
{\n\
	color = gpu_Color;\n\
	texCoord = vec2(gpu_TexCoord);\n\
	gl_Position = gpu_ModelViewProjectionMatrix * vec4(gpu_Vertex, 0.0, 1.0);\n\
}"

// Tier 3 uses shader attributes to send position, texcoord, and color data for each vertex.
#define GPU_DEFAULT_UNTEXTURED_VERTEX_SHADER_SOURCE \
"#version 100\n\
precision highp float;\n\
precision mediump int;\n\
\
attribute vec2 gpu_Vertex;\n\
attribute mediump vec4 gpu_Color;\n\
uniform mat4 gpu_ModelViewProjectionMatrix;\n\
\
varying mediump vec4 color;\n\
\
void main(void)\n\
{\n\
	color = gpu_Color;\n\
	gl_Position = gpu_ModelViewProjectionMatrix * vec4(gpu_Vertex, 0.0, 1.0);\n\
}"


#define GPU_DEFAULT_TEXTURED_FRAGMENT_SHADER_SOURCE \
"#version 100\n\
#ifdef GL_FRAGMENT_PRECISION_HIGH\n\
precision highp float;\n\
#else\n\
precision mediump float;\n\
#endif\n\
precision mediump int;\n\
\
varying mediump vec4 color;\n\
varying vec2 texCoord;\n\
\
uniform sampler2D tex;\n\
\
void main(void)\n\
{\n\
    gl_FragColor = texture2D(tex, texCoord) * color;\n\
}"

#define GPU_DEFAULT_UNTEXTURED_FRAGMENT_SHADER_SOURCE \
"#version 100\n\
#ifdef GL_FRAGMENT_PRECISION_HIGH\n\
precision highp float;\n\
#else\n\
precision mediump float;\n\
#endif\n\
precision mediump int;\n\
\
varying mediump vec4 color;\n\
\
void main(void)\n\
{\n\
    gl_FragColor = color;\n\
}"




typedef struct ContextData_GLES_2
{
	SDL_Color last_color;
	GPU_bool last_use_texturing;
	unsigned int last_shape;
	GPU_bool last_use_blending;
	GPU_BlendMode last_blend_mode;
	GPU_Rect last_viewport;
	GPU_Camera last_camera;
	GPU_bool last_camera_inverted;
	
	GPU_bool last_depth_test;
	GPU_bool last_depth_write;
	GPU_ComparisonEnum last_depth_function;
	
	GPU_Image* last_image;
	float* blit_buffer;  // Holds sets of 4 vertices, each with interleaved position, tex coords, and colors (e.g. [x0, y0, z0, s0, t0, r0, g0, b0, a0, ...]).
	unsigned short blit_buffer_num_vertices;
	unsigned short blit_buffer_max_num_vertices;
	unsigned short* index_buffer;  // Indexes into the blit buffer so we can use 4 vertices for every 2 triangles (1 quad)
	unsigned int index_buffer_num_vertices;
	unsigned int index_buffer_max_num_vertices;
    
    // Tier 3 rendering
    unsigned int blit_VBO[2];  // For double-buffering
    unsigned int blit_IBO;
    GPU_bool blit_VBO_flop;
    
	GPU_AttributeSource shader_attributes[16];
	unsigned int attribute_VBO[16];
} ContextData_GLES_2;

typedef struct ImageData_GLES_2
{
    int refcount;
    GPU_bool owns_handle;
	Uint32 handle;
	Uint32 format;
} ImageData_GLES_2;

typedef struct TargetData_GLES_2
{
    int refcount;
	Uint32 handle;
	Uint32 format;
} TargetData_GLES_2;



#endif
