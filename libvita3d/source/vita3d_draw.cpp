#include <math.h>
#include "vita3d.h"
#include "shared.h"

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float vita3d_rotation_x;
float vita3d_rotation_y;
float vita3d_rotation_z;
float vita3d_fov;
vita3d_color_vertex vita3d_scale;
vita3d_color_vertex vita3d_translation;
vita3d_color_vertex vita3d_camera_pos;
vita3d_color_vertex vita3d_camera_lookAt;

vita3d_color_vertex *vita3d_get_camera_pos()
{
	return &vita3d_camera_pos;
}

vita3d_color_vertex *vita3d_get_camera_look()
{
	return &vita3d_camera_lookAt;
}

void vita3d_set_camera_look(float x, float y, float z)
{
	vita3d_camera_lookAt.x = x;
	vita3d_camera_lookAt.y = y;
	vita3d_camera_lookAt.z = z;
}

void vita3d_set_camera_pos(float x, float y, float z)
{
	vita3d_camera_pos.x = x;
	vita3d_camera_pos.y = y;
	vita3d_camera_pos.z = z;
}

void vita3d_scale_object(float x, float y, float z)
{
	vita3d_scale.x = x;
	vita3d_scale.y = y;
	vita3d_scale.z = z;
}

void vita3d_rotate_object(float angle, vita3d_color_vertex axis)
{
	if(vita3d_color_vertex.x == 1.0f)
	{
		vita3d_rotation_x = angle;
	}
	else if(vita3d_color_vertex.y == 1.0f)
	{
		vita3d_rotation_y = angle;
	}
	else if(vita3d_color_vertex.z == 1.0f)
	{
		vita3d_rotation_z = angle;
	}
}

void vita3d_translate_object(float x, float y, float z)
{
	vita3d_translation.x = x;
	vita3d_translation.y = y;
	vita3d_translation.z = z;
}

void vita3d_draw_object(vita3d_primitive primitive, vita3d_color_vertex *vertices, uint16_t *indices, int vertexCount, int indexCount)
{
	//align memory and put 'vertices' in it
	vita3d_color_vertex *aVertices = (vita3d_color_vertex *)vita3d_pool_memalign(
		vertexCount * sizeof(vita3d_color_vertex),
		sizeof(vita3d_color_vertex));
		
	for(int i = 0; i < vertexCount; i++)
		aVertices[i] = vertices[i];
	
	//align memory and put 'indices' in it
	uint16_t *aIndices = (vita3d_color_vertex *)vita3d_pool_memalign(
		vertexCount * sizeof(uint16_t),
		sizeof(uint16_t));
		
	for(int i = 0; i < indexCount; i++)
		aIndices[i] = indices[i];
	
	
}

vita3d_color_vertex *vita3d_project_vertices(vita3d_color_vertex *vertices, unsigned int noVertices)
{
	vita3d_color_vertex *output = (vita3d_color_vertex *)vita3d_pool_memalign(
		noVertices * sizeof(vita3d_color_vertex),
		sizeof(vita3d_color_vertex));
	
	for(unsigned int i = 0; i < noVertices; i++)
	{
		float coords[16];
		for(int p = 0; p < 16; p++)
			coords[p] = 0;
		coords[12] = vertices[i].x;
		coords[13] = vertices[i].y;
		coords[14] = vertices[i].z;
		coords[15] = 1;
		
		glm::mat4 vertex = glm::make_mat4(coords); 
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(vita3d_translation.x, vita3d_translation.y, vita3d_translation.z));
		glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(vita3d_scale.x, vita3d_scale.y, vita3d_scale.z));
	
		glm::mat4 rotate(1.0f);
		const glm::vec3 directionX(1.0f, 0.0f, 0.0f);
		const glm::vec3 directionY(0.0f, 1.0f, 0.0f);
		const glm::vec3 directionZ(0.0f, 0.0f, 1.0f);
		
		rotate = glm::rotate((const glm::mat4&)rotate, (const float&) vita3d_rotation_x, directionY);
		rotate = glm::rotate((const glm::mat4&)rotate, (const float&) vita3d_rotation_y, directionX);
		rotate = glm::rotate((const glm::mat4&)rotate, (const float&) vita3d_rotation_z, directionZ);
		
		glm::mat4 modelMatrix = translation * rotate * scale * vertex;
		
		glm::mat4 viewMatrix = glm::lookAt(
			glm::vec3(vita3d_camera_pos.x, vita3d_camera_pos.y, vita3d_camera_pos.z),
			glm::vec3(vita3d_camera_lookAt.x, vita3d_camera_lookAt.y, vita3d_camera_lookAt.z),
			glm::vec3(0.0f, 1.0f, 0.0f));
			
		glm::mat4 projectionMatrix = glm::perspective(
			vita3d_fov,
			16.0f / 9.0f,
			0.1f,
			100.0f);
			
		//create screen coordinates
		glm::mat4 clipSpace = projectionMatrix * viewMatrix * modelMatrix;
		
		glm::vec3 ndcSpace;
		ndcSpace.x = clipSpace[3][0] / clipSpace[3][3];
		ndcSpace.y = clipSpace[3][1] / clipSpace[3][3];
		ndcSpace.z = clipSpace[3][2] / clipSpace[3][3];
		
		glm::vec2 windowSpacePos;
		windowSpacePos.x = ((ndcSpace.x + 1.0f) / 2.0f) * 960 + 0;
		windowSpacePos.y = ((ndcSpace.y + 1.0f) / 2.0f) * 544 + 0;
		
		//set output
		output[i].x = windowSpacePos.x;
		output[i].y = windowSpacePos.y;
		output[i].z = +0.5f;
		output[i].color = vertices[i].color;
	}
	
	return output;
}

void vita3d_draw_pixel(float x, float y, float z, unsigned int color)
{
	vita3d_color_vertex *vertex = (vita3d_color_vertex *)vita3d_pool_memalign(
		1 * sizeof(vita3d_color_vertex), // 1 vertex
		sizeof(vita3d_color_vertex));

	uint16_t *index = (uint16_t *)vita3d_pool_memalign(
		1 * sizeof(uint16_t), // 1 index
		sizeof(uint16_t));
		
	vertex->x = x;
	vertex->y = y;
	vertex->z = z;
	vertex->color = color;

	*index = 0;
	
	sceGxmSetVertexProgram(_vita3d_context, _vita3d_colorVertexProgram);
	sceGxmSetFragmentProgram(_vita3d_context, _vita3d_colorFragmentProgram);

	void *vertexDefaultBuffer;
	sceGxmReserveVertexDefaultUniformBuffer(_vita3d_context, &vertexDefaultBuffer);
	sceGxmSetUniformDataF(vertexDefaultBuffer, _vita3d_colorWvpParam, 0, 16, _vita3d_perspective_matrix);

	sceGxmSetVertexStream(_vita3d_context, 0, vertex);
	sceGxmSetFrontPolygonMode(_vita3d_context, SCE_GXM_POLYGON_MODE_POINT);
	sceGxmDraw(_vita3d_context, SCE_GXM_PRIMITIVE_POINTS, SCE_GXM_INDEX_FORMAT_U16, index, 1);
	sceGxmSetFrontPolygonMode(_vita3d_context, SCE_GXM_POLYGON_MODE_TRIANGLE_FILL);
}

void vita3d_draw_line(float x0, float y0, float z0, float x1, float y1, float z1, unsigned int color)
{
	vita3d_color_vertex *vertices = (vita3d_color_vertex *)vita3d_pool_memalign(
		2 * sizeof(vita3d_color_vertex), // 2 vertices
		sizeof(vita3d_color_vertex));

	uint16_t *indices = (uint16_t *)vita3d_pool_memalign(
		2 * sizeof(uint16_t), // 2 indices
		sizeof(uint16_t));

	vertices[0].x = x0;
	vertices[0].y = y0;
	vertices[0].z = z0;
	vertices[0].color = color;

	vertices[1].x = x1;
	vertices[1].y = y1;
	vertices[1].z = z1;
	vertices[1].color = color;

	indices[0] = 0;
	indices[1] = 1;

	sceGxmSetVertexProgram(_vita3d_context, _vita3d_colorVertexProgram);
	sceGxmSetFragmentProgram(_vita3d_context, _vita3d_colorFragmentProgram);

	void *vertexDefaultBuffer;
	sceGxmReserveVertexDefaultUniformBuffer(_vita3d_context, &vertexDefaultBuffer);
	sceGxmSetUniformDataF(vertexDefaultBuffer, _vita3d_colorWvpParam, 0, 16, _vita3d_perspective_matrix);

	sceGxmSetVertexStream(_vita3d_context, 0, vertices);
	sceGxmSetFrontPolygonMode(_vita3d_context, SCE_GXM_POLYGON_MODE_LINE);
	sceGxmDraw(_vita3d_context, SCE_GXM_PRIMITIVE_LINES, SCE_GXM_INDEX_FORMAT_U16, indices, 2);
	sceGxmSetFrontPolygonMode(_vita3d_context, SCE_GXM_POLYGON_MODE_TRIANGLE_FILL);
}

void vita3d_draw_rectangle(float x, float y, float z, float w, float h, float d, unsigned int color, float rotation, bool outlined)
{
	vita3d_color_vertex *vertices = (vita3d_color_vertex *)vita3d_pool_memalign(
		8 * sizeof(vita3d_color_vertex), // 8 vertices
		sizeof(vita3d_color_vertex));

	uint16_t *indices = (uint16_t *)vita3d_pool_memalign(
		36 * sizeof(uint16_t), // 36 indices
		sizeof(uint16_t));
	
	uint16_t *outline = (uint16_t *)vita3d_pool_memalign(
		24 * sizeof(uint16_t),
		sizeof(uint16_t));
	
	vertices[0].x = x;
	vertices[0].y = y;
	vertices[0].z = z + d;
	vertices[0].color = color;

	vertices[1].x = x + w;
	vertices[1].y = y;
	vertices[1].z = z + d;
	vertices[1].color = color;

	vertices[2].x = x;
	vertices[2].y = y + h;
	vertices[2].z = z + d;
	vertices[2].color = color;

	vertices[3].x = x + w;
	vertices[3].y = y + h;
	vertices[3].z = z + d;
	vertices[3].color = color;
	
	vertices[4].x = x;
	vertices[4].y = y;
	vertices[4].z = z;
	vertices[4].color = color;
	
	vertices[5].x = x + w;
	vertices[5].y = y;
	vertices[5].z = z;
	vertices[5].color = color;
	
	vertices[6].x = x;
	vertices[6].y = y + h;
	vertices[6].z = z;
	vertices[6].color = color;
	
	vertices[7].x = x + w;
	vertices[7].y = y + h;
	vertices[7].z = z;
	vertices[7].color = color;

	indices[0] = 4; //7
	indices[1] = 5; //4
	indices[2] = 7; //5
	indices[3] = 7; //7
	indices[4] = 6; //6
	indices[5] = 4; //7
	
	indices[6] = 0;
	indices[7] = 1;
	indices[8] = 5;
	indices[9] = 5;
	indices[10] = 4;
	indices[11] = 0;
	
	indices[12] = 2;
	indices[13] = 3;
	indices[14] = 1;
	indices[15] = 1;
	indices[16] = 0;
	indices[17] = 2;
	
	indices[18] = 6;
	indices[19] = 7;
	indices[20] = 3;
	indices[21] = 3;
	indices[22] = 2;
	indices[23] = 6;
	
	indices[24] = 0;
	indices[25] = 4;
	indices[26] = 6;
	indices[27] = 6;
	indices[28] = 2;
	indices[29] = 0;
	
	indices[30] = 5;
	indices[31] = 1;
	indices[32] = 3;
	indices[33] = 3;
	indices[34] = 7;
	indices[35] = 5;
	
	outline[0] = 4;
	outline[1] = 6;
	
	outline[2] = 6;
	outline[3] = 7;
	
	outline[4] = 7;
	outline[5] = 5;
	
	outline[6] = 5;
	outline[7] = 4;
	
	outline[8] = 4;
	outline[9] = 0;
	
	outline[10] = 5;
	outline[11] = 1;
	
	outline[12] = 7;
	outline[13] = 3;
	
	outline[14] = 6;
	outline[15] = 2;
	
	outline[16] = 2;
	outline[17] = 3;
	
	outline[18] = 3;
	outline[19] = 1;
	
	outline[20] = 1;
	outline[21] = 0;
	
	outline[22] = 0;
	outline[23] = 2;
	
	vita3d_color_vertex *output = (vita3d_color_vertex *)vita3d_pool_memalign(
		8 * sizeof(vita3d_color_vertex),
		sizeof(vita3d_color_vertex));
	
	//setup to call vita3d_project_vertices()
	vita3d_color_vertex rotDirect;
	rotDirect.x = 0.0f;
	rotDirect.y = 1.0f;
	rotDirect.z = 0.0f;
	
	vita3d_color_vertex scale;
	scale.x = 10.0f;
	scale.y = 10.0f;
	scale.z = 10.0f;
	
	vita3d_color_vertex translation;
	translation.x = 0;
	translation.y = 0;
	translation.z = 0;
	
	vita3d_color_vertex camera_pos;
	camera_pos.x = 0.0f;
	camera_pos.y = 0.0f;
	camera_pos.z = -15.0f;
	
	vita3d_color_vertex lookAt;
	lookAt.x = 0;
	lookAt.y = 0;
	lookAt.z = 0;
	
	vita3d_rotation = rotation;
	vita3d_fov = 80.0f;
	vita3d_rotation_axis = rotDirect;
	vita3d_scale = scale;
	vita3d_translation = translation;
	vita3d_camera_pos = camera_pos;
	vita3d_camera_lookAt = lookAt;
	
	output = vita3d_project_vertices(vertices, 8);
	
	/*
	for(unsigned int i = 0; i < 8; i++)
	{
		float coords[16];
		for(int p = 0; p < 16; p++)
			coords[p] = 0;
		coords[12] = vertices[i].x;
		coords[13] = vertices[i].y;
		coords[14] = vertices[i].z;
		coords[15] = 1;
		
		glm::mat4 vertex = glm::make_mat4(coords); 
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(10.0f, 10.0f, 10.0f));
	
		glm::mat4 rotate(1.0f);
		const glm::vec3 direction(0.0f, 1.0f, 0.0f);
		rotate = glm::rotate((const glm::mat4&)rotate, (const float&) rotation, direction);
		
		glm::mat4 modelMatrix = translation * rotate * scale * vertex;
		
		glm::mat4 viewMatrix = glm::lookAt(
			glm::vec3(0.0f, 0.0f, -15.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
			
		glm::mat4 projectionMatrix = glm::perspective(
			80.0f,
			16.0f / 9.0f,
			0.1f,
			100.0f);
			
		glm::mat4 clipSpace = projectionMatrix * viewMatrix * modelMatrix;
		
		glm::vec3 ndcSpace;
		ndcSpace.x = clipSpace[3][0] / clipSpace[3][3];
		ndcSpace.y = clipSpace[3][1] / clipSpace[3][3];
		ndcSpace.z = clipSpace[3][2] / clipSpace[3][3];
		
		glm::vec2 windowSpacePos;
		windowSpacePos.x = ((ndcSpace.x + 1.0f) / 2.0f) * 960 + 0;
		windowSpacePos.y = ((ndcSpace.y + 1.0f) / 2.0f) * 544 + 0;
		
		output[i].x = windowSpacePos.x;
		output[i].y = windowSpacePos.y;
		output[i].z = +0.5f;
		output[i].color = vertices[i].color;
	}
	*/
	
	//turn on backface culling
	SceGxmCullMode cull = SCE_GXM_CULL_CW;
	sceGxmSetCullMode(_vita3d_context, cull);
	
	//set shaders
	sceGxmSetVertexProgram(_vita3d_context, _vita3d_colorVertexProgram);
	sceGxmSetFragmentProgram(_vita3d_context, _vita3d_colorFragmentProgram);
	
	//pass ortho matrix to shader (this IS required)
	void *vertexDefaultBuffer;
	sceGxmReserveVertexDefaultUniformBuffer(_vita3d_context, &vertexDefaultBuffer);
	sceGxmSetUniformDataF(vertexDefaultBuffer, _vita3d_colorWvpParam, 0, 16, _vita3d_ortho_matrix);

	//draw cube
	sceGxmSetVertexStream(_vita3d_context, 0, output);
	sceGxmDraw(_vita3d_context, SCE_GXM_PRIMITIVE_TRIANGLES, SCE_GXM_INDEX_FORMAT_U16, indices, 36);
	
	//if specified, draw outline
	if(outlined)
	{
		vita3d_color_vertex *outlineColor = (vita3d_color_vertex *)vita3d_pool_memalign(
			8 * sizeof(vita3d_color_vertex),
			sizeof(vita3d_color_vertex));
		
		for(int i = 0; i < 8; i++)
		{
			outlineColor[i].x = output[i].x;
			outlineColor[i].y = output[i].y;
			outlineColor[i].z = +0.5f;
			outlineColor[i].color = RGBA8(255, 255, 255, 255);
		}
			
		
		sceGxmSetVertexStream(_vita3d_context, 0, outlineColor);
		
		sceGxmSetFrontPolygonMode(_vita3d_context, SCE_GXM_POLYGON_MODE_LINE);
		sceGxmDraw(_vita3d_context, SCE_GXM_PRIMITIVE_LINES, SCE_GXM_INDEX_FORMAT_U16, outline, 24);
		sceGxmSetFrontPolygonMode(_vita3d_context, SCE_GXM_POLYGON_MODE_TRIANGLE_FILL);
		
		//for(int i = 0; i < 8; i++)
			
	}
}

void vita2d_draw_pixel(float x, float y, unsigned int color)
{
	vita3d_color_vertex *vertex = (vita3d_color_vertex *)vita3d_pool_memalign(
		1 * sizeof(vita3d_color_vertex), // 1 vertex
		sizeof(vita3d_color_vertex));

	uint16_t *index = (uint16_t *)vita3d_pool_memalign(
		1 * sizeof(uint16_t), // 1 index
		sizeof(uint16_t));

	vertex->x = x;
	vertex->y = y;
	vertex->z = +0.5f;
	vertex->color = color;

	*index = 0;

	sceGxmSetVertexProgram(_vita3d_context, _vita3d_colorVertexProgram);
	sceGxmSetFragmentProgram(_vita3d_context, _vita3d_colorFragmentProgram);

	void *vertexDefaultBuffer;
	sceGxmReserveVertexDefaultUniformBuffer(_vita3d_context, &vertexDefaultBuffer);
	sceGxmSetUniformDataF(vertexDefaultBuffer, _vita3d_colorWvpParam, 0, 16, _vita3d_ortho_matrix);

	sceGxmSetVertexStream(_vita3d_context, 0, vertex); //feed vertices into gpu
	sceGxmSetFrontPolygonMode(_vita3d_context, SCE_GXM_POLYGON_MODE_POINT); //set polygon mode to point
	sceGxmDraw(_vita3d_context, SCE_GXM_PRIMITIVE_POINTS, SCE_GXM_INDEX_FORMAT_U16, index, 1); //draw 1 vertex
	sceGxmSetFrontPolygonMode(_vita3d_context, SCE_GXM_POLYGON_MODE_TRIANGLE_FILL); //change polygon mode back to triangle fill
}

void vita2d_draw_line(float x0, float y0, float x1, float y1, unsigned int color)
{
	vita3d_color_vertex *vertices = (vita3d_color_vertex *)vita3d_pool_memalign(
		2 * sizeof(vita3d_color_vertex), // 2 vertices
		sizeof(vita3d_color_vertex));

	uint16_t *indices = (uint16_t *)vita3d_pool_memalign(
		2 * sizeof(uint16_t), // 2 indices
		sizeof(uint16_t));

	vertices[0].x = x0;
	vertices[0].y = y0;
	vertices[0].z = +0.5f;
	vertices[0].color = color;

	vertices[1].x = x1;
	vertices[1].y = y1;
	vertices[1].z = +0.5f;
	vertices[1].color = color;

	indices[0] = 0;
	indices[1] = 1;

	sceGxmSetVertexProgram(_vita3d_context, _vita3d_colorVertexProgram);
	sceGxmSetFragmentProgram(_vita3d_context, _vita3d_colorFragmentProgram);

	void *vertexDefaultBuffer;
	sceGxmReserveVertexDefaultUniformBuffer(_vita3d_context, &vertexDefaultBuffer);
	sceGxmSetUniformDataF(vertexDefaultBuffer, _vita3d_colorWvpParam, 0, 16, _vita3d_ortho_matrix);

	sceGxmSetVertexStream(_vita3d_context, 0, vertices);
	sceGxmSetFrontPolygonMode(_vita3d_context, SCE_GXM_POLYGON_MODE_LINE);
	sceGxmDraw(_vita3d_context, SCE_GXM_PRIMITIVE_LINES, SCE_GXM_INDEX_FORMAT_U16, indices, 2);
	sceGxmSetFrontPolygonMode(_vita3d_context, SCE_GXM_POLYGON_MODE_TRIANGLE_FILL);
}

void vita2d_draw_rectangle(float x, float y, float w, float h, unsigned int color)
{
	vita3d_color_vertex *vertices = (vita3d_color_vertex *)vita3d_pool_memalign(
		4 * sizeof(vita3d_color_vertex), // 4 vertices
		sizeof(vita3d_color_vertex));

	uint16_t *indices = (uint16_t *)vita3d_pool_memalign(
		4 * sizeof(uint16_t), // 4 indices
		sizeof(uint16_t));

	vertices[0].x = x;
	vertices[0].y = y;
	vertices[0].z = +0.5f;
	vertices[0].color = color;

	vertices[1].x = x + w;
	vertices[1].y = y;
	vertices[1].z = +0.5f;
	vertices[1].color = color;

	vertices[2].x = x;
	vertices[2].y = y + h;
	vertices[2].z = +0.5f;
	vertices[2].color = color;

	vertices[3].x = x + w;
	vertices[3].y = y + h;
	vertices[3].z = +0.5f;
	vertices[3].color = color;

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;

	sceGxmSetVertexProgram(_vita3d_context, _vita3d_colorVertexProgram);
	sceGxmSetFragmentProgram(_vita3d_context, _vita3d_colorFragmentProgram);

	void *vertexDefaultBuffer;
	sceGxmReserveVertexDefaultUniformBuffer(_vita3d_context, &vertexDefaultBuffer);
	sceGxmSetUniformDataF(vertexDefaultBuffer, _vita3d_colorWvpParam, 0, 16, _vita3d_ortho_matrix);

	sceGxmSetVertexStream(_vita3d_context, 0, vertices);
	sceGxmDraw(_vita3d_context, SCE_GXM_PRIMITIVE_TRIANGLE_STRIP, SCE_GXM_INDEX_FORMAT_U16, indices, 4);
}

void vita2d_draw_fill_circle(float x, float y, float radius, unsigned int color)
{
	static const int num_segments = 100;

	vita3d_color_vertex *vertices = (vita3d_color_vertex *)vita3d_pool_memalign(
		(num_segments + 1) * sizeof(vita3d_color_vertex),
		sizeof(vita3d_color_vertex));

	uint16_t *indices = (uint16_t *)vita3d_pool_memalign(
		(num_segments + 2) * sizeof(uint16_t),
		sizeof(uint16_t));


	vertices[0].x = x;
	vertices[0].y = y;
	vertices[0].z = +0.5f;
	vertices[0].color = color;
	indices[0] = 0;

	float theta = 2 * M_PI / (float)num_segments;
	float c = cosf(theta);
	float s = sinf(theta);
	float t;

	float xx = radius;
	float yy = 0;
	int i;

	for (i = 1; i <= num_segments; i++) {
		vertices[i].x = x + xx;
		vertices[i].y = y + yy;
		vertices[i].z = +0.5f;
		vertices[i].color = color;
		indices[i] = i;

		t = xx;
		xx = c * xx - s * yy;
		yy = s * t + c * yy;
	}

	indices[num_segments + 1] = 1;

	sceGxmSetVertexProgram(_vita3d_context, _vita3d_colorVertexProgram);
	sceGxmSetFragmentProgram(_vita3d_context, _vita3d_colorFragmentProgram);

	void *vertexDefaultBuffer;
	sceGxmReserveVertexDefaultUniformBuffer(_vita3d_context, &vertexDefaultBuffer);
	sceGxmSetUniformDataF(vertexDefaultBuffer, _vita3d_colorWvpParam, 0, 16, _vita3d_ortho_matrix);

	sceGxmSetVertexStream(_vita3d_context, 0, vertices);
	sceGxmDraw(_vita3d_context, SCE_GXM_PRIMITIVE_TRIANGLE_FAN, SCE_GXM_INDEX_FORMAT_U16, indices, num_segments + 2);
}
