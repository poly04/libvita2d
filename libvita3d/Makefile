TARGET_LIB = libvita3d.a
OBJS       = source/vita3d.o source/vita3d_texture.o source/vita3d_draw.o source/utils.o \
             source/vita3d_image_png.o source/vita3d_image_jpeg.o source/vita3d_image_bmp.o \
             source/vita3d_font.o source/vita3d_pgf.o source/bin_packing_3d.o \
             source/texture_atlas.o source/int_htab.o
INCLUDES   = include
SHADERS    = shader/compiled/clear_v_gxp.o shader/compiled/clear_f_gxp.o \
             shader/compiled/color_v_gxp.o shader/compiled/color_f_gxp.o \
             shader/compiled/texture_v_gxp.o shader/compiled/texture_f_gxp.o \
             shader/compiled/texture_tint_f_gxp.o

PREFIX  ?= ${VITASDK}/arm-vita-eabi
CC      = arm-vita-eabi-gcc
CXX		= arm-vita-eabi-gcc
AR      = arm-vita-eabi-ar
CFLAGS  = -Wl,-q -Wall -O3 -I$(INCLUDES) -I$(VITASDK)/arm-vita-eabi/include/freetype2 -ffat-lto-objects -flto
CXXFLAGS = -Wl,-q -Wall -I$(INCLUDES) -I$(VITASDK)/arm-vita-eabi/include/freetype2 -ffat-lto-objects -flto
ASFLAGS = $(CFLAGS)

all: $(TARGET_LIB)

debug: CFLAGS += -DDEBUG_BUILD
debug: all

$(TARGET_LIB): $(SHADERS) $(OBJS)
	$(AR) -rc $@ $^

clean:
	rm -rf $(TARGET_LIB) $(OBJS)

install: $(TARGET_LIB)
	@mkdir -p $(DESTDIR)$(PREFIX)/lib/
	cp $(TARGET_LIB) $(DESTDIR)$(PREFIX)/lib/
	@mkdir -p $(DESTDIR)$(PREFIX)/include/
	cp include/vita3d.h $(DESTDIR)$(PREFIX)/include/
