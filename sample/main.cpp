#include <vita3d.h>

int main()
{
	vita3d_init();
	
	float angle = 0;
	
	while(true)
	{
		vita3d_start_drawing();
		vita3d_clear_screen();
		
		vita3d_draw_rectangle(-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, RGBA8(255, 0, 0, 255), angle, true);
		
		vita3d_end_drawing();
		vita3d_swap_buffers();
		
		angle += 0.01f;
	}
	
	vita3d_fini();
	
	return 0;
}