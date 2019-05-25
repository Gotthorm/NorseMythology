
// First you must create the names of the buffers
// The variable(s) to hold the names
GLuint bufferName;
// Creatre a single buffer name
glCreateBuffers( 1, &bufferName );

// Allocate the data stores
// Creating a buffer of 1024 bytes
// Buffer has no initial data values
// We allow map for writing
glNamedBufferStorage( bufferName, 1024, nullptr, GL_MAP_WRITE_BIT );

// Do we need this?
// GL_DYNAMIC_STORAGE_BIT : We will be updating the buffer contants directly

// Once storage has been allocated it is considered immutable

// Bind buffer objects to the current GL context
glBindBuffer( GL_ARRAY_BUFFER, bufferName );


// Here is a way to update the buffer data

// This is the data that we will place into the buffer object
static const float data[] =
{
	1.0f, 0.0f, 0.0f, 1.0f,
};

// Get a pointer to the buffer's data store
// TODO: Its typically better to use the advanced version "glMapNamedBufferRange"
void * ptr = glMapNamedBuffer( buffer, GL_WRITE_ONLY );

// Copy our data into it...
memcpy( ptr, data, sizeof( data ) );

// Tell OPenGL that we are done with the pointer
glUnmapNamedBuffer( GL_ARRAY_BUFFER );
