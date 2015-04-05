#ifndef image_h
#define image_h

#include "common_header.h"
#include "GL/glew.h"
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

enum enum_image_format {
	IMG_FORMAT_NULL = 0,
	IMG_FORMAT_RGBA,
	IMG_FORMAT_BGRA,
	IMG_FORMAT_RGB,
	IMG_FORMAT_BGR,
};

class image_data {
public:
	uint width;
	uint height;
	enum enum_image_format format;
	uchar *pixels;

	image_data()
		:width(0), height(0), format(IMG_FORMAT_NULL), pixels(nullptr)
	{
		
	}

	~image_data() {
		if (pixels != nullptr) {
			delete[] pixels;
			pixels = nullptr;
		}
	}

	GLenum get_gl_format() {
		switch (format) {
		case IMG_FORMAT_NULL:
			break;
		case IMG_FORMAT_RGBA:
			return GL_RGBA;
		case IMG_FORMAT_BGRA:
			return GL_BGRA;
		case IMG_FORMAT_RGB:
			return GL_RGB;
		case IMG_FORMAT_BGR:
			return GL_BGR;
		default:
			break;
		}
		assert(false);
	}
};

void img_init_decoder();
void img_dispose_decoder();
int img_load_image(const char *path, image_data *data);

#ifdef __cplusplus
}
#endif

#endif