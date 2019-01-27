// PNGOBJECT.CPP

#include "Valhalla.h"
#include <png.h>
#include <zlib.h>
#include <memory.h>

#pragma comment(lib, "libpng16.lib")
#pragma comment(lib, "zlib.lib")

namespace Valhalla
{
	PNGObject::~PNGObject()
	{
		delete[] m_Data;
	}

	bool PNGObject::Load( const std::wstring& fileName )
	{
		bool results = false;
		unsigned int sig_read = 0;

		if( fileName.length() > 0 )
		{
			FILE* filePointer;

			if( _wfopen_s( &filePointer, fileName.c_str(), L"rb" ) == 0 )
			{
				// Create and initialize the png_struct with the desired error handler functions.  
				// If you want to use the default stderr and longjump method, you can supply NULL for the last three parameters.  
				// We also supply the the compiler header file version, so that we know if the application was compiled with a 
				// compatible version of the library.  REQUIRED
				png_structp png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );

				png_infop info_ptr = NULL;

				if( png_ptr )
				{
					// Allocate/initialize the memory for image information.  REQUIRED.
					png_infop info_ptr = png_create_info_struct( png_ptr );

					if( info_ptr )
					{
						// Set error handling if you are using the setjmp/longjmp method (this is the normal method of doing things with libpng).
						// REQUIRED unless you set up your own error handlers in the png_create_read_struct() earlier.
						if( setjmp( png_jmpbuf( png_ptr ) ) == 0 )
						{
							// Set up the output control if you are using standard C streams 
							png_init_io( png_ptr, filePointer );

							// If we have already read some of the signature
							png_set_sig_bytes( png_ptr, sig_read );

							// If you have enough memory to read in the entire image at once, and you need to specify only transforms that can be controlled
							// with one of the PNG_TRANSFORM_* bits (this presently excludes dithering, filling, setting background, and doing gamma adjustment), 
							// then you can read the entire image (including pixels) into the info structure with this call
							//
							// PNG_TRANSFORM_STRIP_16
							// PNG_TRANSFORM_PACKING forces 8 bit
							// PNG_TRANSFORM_EXPAND forces to expand a palette into RGB
							png_read_png( png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL );

							int bit_depth;
							int color_type, interlace_type;
							png_get_IHDR( png_ptr, info_ptr, &m_Width, &m_Height, &bit_depth, &color_type, &interlace_type, NULL, NULL );

							// A simple test for alpha.  This only works for images with a seperated alpha channel 
							m_Alpha = ( color_type == PNG_COLOR_TYPE_RGBA || color_type == PNG_COLOR_TYPE_GA );

							unsigned int row_bytes = (unsigned int)png_get_rowbytes( png_ptr, info_ptr );

							m_Size = row_bytes * m_Height;

							m_Data = new unsigned char[ m_Size ];

							if( m_Data )
							{
								png_bytepp row_pointers = png_get_rows( png_ptr, info_ptr );

								for( unsigned int i = 0; i < m_Height; i++ )
								{
									// Note that png is ordered top to bottom, but OpenGL expect it bottom to top so the order or swapped
									memcpy( m_Data + ( row_bytes * ( m_Height - 1 - i ) ), row_pointers[ i ], row_bytes );
								}

								results = true;
							}
						}
					}

					// Free all of the memory associated with the png_ptr and info_ptr 
					png_destroy_read_struct( &png_ptr, info_ptr ? &info_ptr : NULL, NULL );
				}
			}

			if( filePointer )
			{
				fclose( filePointer );
			}
		}

		return results;
	}
}