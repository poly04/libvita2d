#ifndef SHARED_H
#define SHARED_H

/* Shared with other .cpp */
extern float _vita3d_ortho_matrix[4*4];
extern float _vita3d_perspective_matrix[4*4];
extern SceGxmContext *_vita3d_context;
extern SceGxmVertexProgram *_vita3d_colorVertexProgram;
extern SceGxmFragmentProgram *_vita3d_colorFragmentProgram;
extern SceGxmVertexProgram *_vita3d_textureVertexProgram;
extern SceGxmFragmentProgram *_vita3d_textureFragmentProgram;
extern SceGxmFragmentProgram *_vita3d_textureTintFragmentProgram;
extern const SceGxmProgramParameter *_vita3d_colorWvpParam;
extern const SceGxmProgramParameter *_vita3d_textureWvpParam;
extern SceGxmProgramParameter *_vita3d_textureTintColorParam;


#endif
