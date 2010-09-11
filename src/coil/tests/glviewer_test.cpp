#include <iostream>

#include <coil/clWindow.hpp>
#include <coil/RenderObj/TestWaves.hpp>
#include <coil/RenderObj/Spheres.hpp>

int main(int argc, char** argv)
{
  try {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    
    cl::Platform clplatform = platforms[0];
    
    CoilMaster::getInstance(argc, argv);
    
    CLGLWindow* CLWindow = new CLGLWindow(1024, 1024,//height, width
					  200, 400,//initPosition (x,y)
					  "GLCLWindow",//title
					  clplatform);
    
    //CLWindow.addRenderObj<RTTestWaves>((size_t)1000, 0.0f);

    std::vector<RTSpheres::SphereDetails> sphereDetailLevels;

    //Shadow testing
//    size_t N = 10 * 1024;
//    sphereDetailLevels.push_back(RTSpheres::SphereDetails(Sphere::icosahedron, 2, 1024));
//    sphereDetailLevels.push_back(RTSpheres::SphereDetails(Sphere::icosahedron, 1, 1024));
//    sphereDetailLevels.push_back(RTSpheres::SphereDetails(Sphere::icosahedron, 0, N-2048));

    //Work computer test render
    size_t N = 1024 * 1000;
    sphereDetailLevels.push_back(RTSpheres::SphereDetails(Sphere::icosahedron, 2, 10));
    sphereDetailLevels.push_back(RTSpheres::SphereDetails(Sphere::icosahedron, 1, 1000));
    sphereDetailLevels.push_back(RTSpheres::SphereDetails(Sphere::icosahedron, 0, 10000));
    sphereDetailLevels.push_back(RTSpheres::SphereDetails(Sphere::octahedron, 0, 200000));
    sphereDetailLevels.push_back(RTSpheres::SphereDetails(Sphere::tetrahedron, 0, N - 200000 - 10000 - 1000 -10));

    //Home laptop test render
//    size_t N = 10 * 1024;
//    sphereDetailLevels.push_back(RTSpheres::SphereDetails(Sphere::icosahedron, 2, 10));
//    sphereDetailLevels.push_back(RTSpheres::SphereDetails(Sphere::icosahedron, 1, 100));
//    sphereDetailLevels.push_back(RTSpheres::SphereDetails(Sphere::icosahedron, 0, 1000));
//    sphereDetailLevels.push_back(RTSpheres::SphereDetails(Sphere::octahedron, 0, N - 1000 - 100 - 10));

    CLWindow->addRenderObj<RTSpheres>((size_t)N, sphereDetailLevels);

    CoilMaster::getInstance().addWindow(CLWindow);

    CoilMaster::getInstance().bootRenderer();
    
    CoilMaster::getInstance().waitForRendererShutdown();
    
  } catch(cl::Error& err) {
    std::cerr << "OpenCL error: " << err.what() << "(" << err.err() 
	      << ")" << std::endl;
    
    return EXIT_FAILURE;
  }
  
  return 0;

  
}
