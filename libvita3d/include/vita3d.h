#ifndef VITA2D_H
#define VITA2D_H

#include <psp2/gxm.h>
#include <psp2/types.h>
#include <psp2/kernel/sysmem.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RGBA8(r,g,b,a) ((((a)&0xFF)<<24) | (((b)&0xFF)<<16) | (((g)&0xFF)<<8) | (((r)&0xFF)<<0))

enum vita3d_primitive
{
	VITA3D_TRIANGLES,
	VITA3D_TRIANGLE_STRIP,
	VITA3D_TRIANGLE_FAN,
	VITA3D_LINES
};

typedef struct vita3d_clear_vertex {
	float x;
	float y;
} vita2d_clear_vertex;

typedef struct vita3d_color_vertex {
	float x;
	float y;
	float z;
	unsigned int color;
} vita2d_color_vertex;

typedef struct vita3d_texture_vertex {
	float x;
	float y;
	float z;
	float u;
	float v;
} vita2d_texture_vertex;

typedef struct vita3d_texture {
	SceGxmTexture gxm_tex;
	SceUID data_UID;
	SceUID palette_UID;
} vita2d_texture;

typedef struct vita3d_font vita2d_font;
typedef struct vita3d_pgf vita2d_pgf;

//cool 3d fuctions to draw your own geometry!
void vita3d_draw_object(vita3d_primitive primitive, vita3d_color_vertex *vertices, uint16_t *indices, int vertexCount);
void vita3d_translate_object(float x, float y, float z);
void vita3d_rotate_object(float angle, vita3d_color_vertex axis);
void vita3d_scale_object(float x, float y, float z);
void vita3d_set_camera_pos(float x, float y, float z);
void vita3d_set_camera_look(float x, float y, float z);
vita3d_color_vertex *vita3d_get_camera_pos();
vita3d_color_vertex *vita3d_get_camera_look();

int vita3d_init();
int vita3d_init_advanced(unsigned int temp_pool_size);
void vita3d_wait_rendering_done();
int vita3d_fini();

void vita3d_clear_screen();
void vita3d_swap_buffers();

void vita3d_start_drawing();
void vita3d_end_drawing();

int vita3d_common_dialog_update();

void vita3d_set_clear_color(unsigned int color);
void vita3d_set_vblank_wait(int enable);
void *vita3d_get_current_fb();

void vita3d_set_region_clip(SceGxmRegionClipMode mode, unsigned int x_min, unsigned int y_min, unsigned int x_max, unsigned int y_max);

void *vita3d_pool_malloc(unsigned int size);
void *vita3d_pool_memalign(unsigned int size, unsigned int alignment);
unsigned int vita3d_pool_free_space();
void vita3d_pool_reset();

void vita2d_draw_pixel(float x, float y, unsigned int color);
void vita2d_draw_line(float x0, float y0, float x1, float y1, unsigned int color);
void vita2d_draw_rectangle(float x, float y, float w, float h, unsigned int color);
void vita2d_draw_fill_circle(float x, float y, float radius, unsigned int color);

void vita3d_draw_pixel(float x, float y, float z, unsigned int color);
void vita3d_draw_line(float x0, float y0, float z0, float x1, float y1, float z1, unsigned int color);
void vita3d_draw_rectangle(float x, float y, float z, float w, float h, float d, unsigned int color, float rotation, bool outline);

vita3d_texture *vita3d_create_empty_texture(unsigned int w, unsigned int h);
vita3d_texture *vita3d_create_empty_texture_format(unsigned int w, unsigned int h, SceGxmTextureFormat format);
void vita3d_free_texture(vita2d_texture *texture);

unsigned int vita3d_texture_get_width(const vita3d_texture *texture);
unsigned int vita3d_texture_get_height(const vita3d_texture *texture);
unsigned int vita3d_texture_get_stride(const vita3d_texture *texture);
SceGxmTextureFormat vita3d_texture_get_format(const vita3d_texture *texture);
void *vita3d_texture_get_datap(const vita3d_texture *texture);
void *vita3d_texture_get_palette(const vita3d_texture *texture);
SceGxmTextureFilter vita3d_texture_get_min_filter(const vita3d_texture *texture);
SceGxmTextureFilter vita3d_texture_get_mag_filter(const vita3d_texture *texture);
void vita3d_texture_set_filters(vita3d_texture *texture, SceGxmTextureFilter min_filter, SceGxmTextureFilter mag_filter);

void vita2d_draw_texture(const vita3d_texture *texture, float x, float y);
void vita2d_draw_texture_rotate(const vita3d_texture *texture, float x, float y, float rad);
void vita2d_draw_texture_rotate_hotspot(const vita3d_texture *texture, float x, float y, float rad, float center_x, float center_y);
void vita2d_draw_texture_scale(const vita3d_texture *texture, float x, float y, float x_scale, float y_scale);
void vita2d_draw_texture_part(const vita3d_texture *texture, float x, float y, float tex_x, float tex_y, float tex_w, float tex_h);
void vita2d_draw_texture_part_scale(const vita3d_texture *texture, float x, float y, float tex_x, float tex_y, float tex_w, float tex_h, float x_scale, float y_scale);
void vita2d_draw_texture_scale_rotate_hotspot(const vita3d_texture *texture, float x, float y, float x_scale, float y_scale, float rad, float center_x, float center_y);
void vita2d_draw_texture_scale_rotate(const vita3d_texture *texture, float x, float y, float x_scale, float y_scale, float rad);

void vita2d_draw_texture_tint(const vita3d_texture *texture, float x, float y, unsigned int color);
void vita2d_draw_texture_tint_rotate(const vita3d_texture *texture, float x, float y, float rad, unsigned int color);
void vita2d_draw_texture_tint_rotate_hotspot(const vita3d_texture *texture, float x, float y, float rad, float center_x, float center_y, unsigned int color);
void vita2d_draw_texture_tint_scale(const vita3d_texture *texture, float x, float y, float x_scale, float y_scale, unsigned int color);
void vita2d_draw_texture_tint_part(const vita3d_texture *texture, float x, float y, float tex_x, float tex_y, float tex_w, float tex_h, unsigned int color);
void vita2d_draw_texture_tint_part_scale(const vita3d_texture *texture, float x, float y, float tex_x, float tex_y, float tex_w, float tex_h, float x_scale, float y_scale, unsigned int color);
void vita2d_draw_texture_tint_scale_rotate_hotspot(const vita3d_texture *texture, float x, float y, float x_scale, float y_scale, float rad, float center_x, float center_y, unsigned int color);
void vita2d_draw_texture_tint_scale_rotate(const vita3d_texture *texture, float x, float y, float x_scale, float y_scale, float rad, unsigned int color);

vita3d_texture *vita3d_load_PNG_file(const char *filename);
vita3d_texture *vita3d_load_PNG_buffer(const void *buffer);

vita3d_texture *vita3d_load_JPEG_file(const char *filename);
vita3d_texture *vita3d_load_JPEG_buffer(const void *buffer, unsigned long buffer_size);

vita3d_texture *vita3d_load_BMP_file(const char *filename);
vita3d_texture *vita3d_load_BMP_buffer(const void *buffer);

vita3d_font *vita3d_load_font_file(const char *filename);
vita3d_font *vita3d_load_font_mem(const void *buffer, unsigned int size);
void vita3d_free_font(vita3d_font *font);
int vita3d_font_draw_text(vita3d_font *font, int x, int y, unsigned int color, unsigned int size, const char *text);
int vita3d_font_draw_textf(vita3d_font *font, int x, int y, unsigned int color, unsigned int size, const char *text, ...);
void vita3d_font_text_dimensions(vita3d_font *font, unsigned int size, const char *text, int *width, int *height);
int vita3d_font_text_width(vita3d_font *font, unsigned int size, const char *text);
int vita3d_font_text_height(vita3d_font *font, unsigned int size, const char *text);

/* PGF functions are weak imports at the moment, they have to be resolved manually */
vita3d_pgf *vita3d_load_default_pgf();
void vita3d_free_pgf(vita3d_pgf *font);
int vita3d_pgf_draw_text(vita3d_pgf *font, int x, int y, unsigned int color, float scale, const char *text);
int vita3d_pgf_draw_textf(vita3d_pgf *font, int x, int y, unsigned int color, float scale, const char *text, ...);
void vita3d_pgf_text_dimensions(vita3d_pgf *font, float scale, const char *text, int *width, int *height);
int vita3d_pgf_text_width(vita3d_pgf *font, float scale, const char *text);
int vita3d_pgf_text_height(vita3d_pgf *font, float scale, const char *text);

#ifdef __cplusplus
}
#endif

#endif
