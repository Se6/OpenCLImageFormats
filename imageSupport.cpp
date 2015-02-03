
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

string channelOrderName(cl_channel_order chan) {
    switch(chan) {
       case CL_R              : return "CL_R            ";
       case CL_A              : return "CL_A            ";
       case CL_RG             : return "CL_RG           ";
       case CL_RA             : return "CL_RA           ";
       case CL_RGB            : return "CL_RGB          ";
       case CL_RGBA           : return "CL_RGBA         ";
       case CL_BGRA           : return "CL_BGRA         ";
       case CL_ARGB           : return "CL_ARGB         ";
       case CL_INTENSITY      : return "CL_INTENSITY    ";
       case CL_LUMINANCE      : return "CL_LUMINANCE    ";
       case CL_Rx             : return "CL_Rx           ";
       case CL_RGx            : return "CL_RGx          ";
       case CL_RGBx           : return "CL_RGBx         ";
       case CL_DEPTH          : return "CL_DEPTH        ";
       case CL_DEPTH_STENCIL  : return "CL_DEPTH_STENCIL";
       default:
        std::ostringstream ss;
        ss << "Unknown order (" << chan << ")";
        return ss.str();
    }
}


string channelTypeName(cl_channel_type chan) {
    switch(chan) {
    case CL_SNORM_INT8        : return "CL_SNORM_INT8      ";
    case CL_SNORM_INT16       : return "CL_SNORM_INT16     ";
    case CL_UNORM_INT8        : return "CL_UNORM_INT8      ";
    case CL_UNORM_INT16       : return "CL_UNORM_INT16     ";
    case CL_UNORM_SHORT_565   : return "CL_UNORM_SHORT_565 ";
    case CL_UNORM_SHORT_555   : return "CL_UNORM_SHORT_555 ";
    case CL_UNORM_INT_101010  : return "CL_UNORM_INT_101010";
    case CL_SIGNED_INT8       : return "CL_SIGNED_INT8     ";
    case CL_SIGNED_INT16      : return "CL_SIGNED_INT16    ";
    case CL_SIGNED_INT32      : return "CL_SIGNED_INT32    ";
    case CL_UNSIGNED_INT8     : return "CL_UNSIGNED_INT8   ";
    case CL_UNSIGNED_INT16    : return "CL_UNSIGNED_INT16  ";
    case CL_UNSIGNED_INT32    : return "CL_UNSIGNED_INT32  ";
    case CL_HALF_FLOAT        : return "CL_HALF_FLOAT      ";
    case CL_FLOAT             : return "CL_FLOAT           ";
    case CL_UNORM_INT24       : return "CL_UNORM_INT24     ";
    default:
     std::ostringstream ss;
     ss << "Unknown type (" << chan << ")";
     return ss.str();
    }
}


int main(int argc, char** argv) {

    // Find the platforms
    //
    cl_uint numPlatforms;
    clGetPlatformIDs(0, NULL, &numPlatforms);
    if(numPlatforms==0) {
        cout << "No OpenCL platform found";
        return 1;
    }
    vector<cl_platform_id> platformIds(numPlatforms);
    clGetPlatformIDs(numPlatforms, &platformIds[0], NULL);

    // Iterate the platforms
    //
    for(auto &plat : platformIds){

        size_t size;
        clGetPlatformInfo(plat, CL_PLATFORM_NAME, 0, NULL, &size);
        vector<char> platformName(size);
        clGetPlatformInfo(plat, CL_PLATFORM_NAME, size, &platformName[0], NULL);
        cout << "Platform: " << string(platformName.begin(), platformName.end()-1) << "\n";

        cl_uint numDevAvail;
        clGetDeviceIDs(plat, CL_DEVICE_TYPE_ALL, 0, NULL, &numDevAvail);
        if(numDevAvail) {

            vector<cl_device_id> devAvail(numDevAvail);
            clGetDeviceIDs(plat, CL_DEVICE_TYPE_ALL, numDevAvail, &devAvail[0], NULL);

            // Iterate the devices on the platform
            //
            for(auto &dev : devAvail) {

                clGetDeviceInfo(dev, CL_DEVICE_NAME, 0, NULL, &size);
                vector<char> devName(size);
                clGetDeviceInfo(dev, CL_DEVICE_NAME, size, &devName[0], NULL);
                cout << "  Device: " << string(devName.begin(), devName.end()-1) << "\n";

                // check if it supports images
                cl_bool imageSupport = CL_FALSE;
                clGetDeviceInfo(dev, CL_DEVICE_IMAGE_SUPPORT, sizeof(cl_bool), &imageSupport, NULL);
                if ( imageSupport == CL_FALSE ) {
                    cout << "  Does not support images.\n";
                    continue;
                }

                // Create a context
                //
                cl_int err;
                cl_context_properties properties [] = {CL_CONTEXT_PLATFORM, (cl_context_properties)plat, 0};
                cl_context context = clCreateContext( properties, 1, &dev, NULL, NULL, &err);
                if (err != CL_SUCCESS) {
                    cout << "  Could not create context.\n";
                    continue;
                }

                // Enumerate the image formats
                //
                cl_uint nbFormats;
                clGetSupportedImageFormats( context, CL_MEM_READ_WRITE, CL_MEM_OBJECT_IMAGE2D, 0, NULL, &nbFormats );
                vector<cl_image_format> formats(nbFormats);
                clGetSupportedImageFormats( context, CL_MEM_READ_WRITE, CL_MEM_OBJECT_IMAGE2D, nbFormats, &formats[0], NULL );
                for(auto &form : formats)
                    cout << "  "  << channelOrderName(form.image_channel_order)
                         << " - " << channelTypeName (form.image_channel_data_type)
                         << "\n";
            }

        }
    }
}
