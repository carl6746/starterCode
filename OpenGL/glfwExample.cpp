#include <cstdlib>
#include <iostream>
#include <vector>
#include "components/scene.h"
#include "handleGraphicsArgs.h"
#include "components/shader.h"
#include "components/ISceneLoader.h"
#include "components/SceneLoader.h"
#include "components/SceneParser_JSON.cpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLSL.h"


static const float RADIUS = 1.0f;
static const int SUBDIV_LEVEL = 5;

class PerCamera {
public:
    glm::vec3 position;  
    glm::vec3 target;     
    glm::vec3 up;       

    float fov;           
    float aspect;         
    float near;     
    float far;       

    PerCamera(glm::vec3 pos, glm::vec3 tgt, glm::vec3 upVec,
           float fovDeg, float aspectRatio, float near, float far)
        : position(pos), target(tgt), up(upVec),
          fov(fovDeg), aspect(aspectRatio), near(near), far(far) {}

    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, target, up);
    }

    glm::mat4 getProjectionMatrix() {
        return glm::perspective(glm::radians(fov), aspect, near, far);
    }
};

int CheckGLErrors(const char *s)
{
    int errCount = 0;
    return errCount;
}

glm::vec3 normalizeToSphere(glm::vec3 v, float R) {
    return glm::normalize(v) * R;
}
    
std::vector<float> sphereVertices;

void emitTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
    glm::vec3 n = glm::normalize(glm::cross(b - a, c - a));

    auto push = [&](const glm::vec3& v){
        sphereVertices.push_back(v.x);
        sphereVertices.push_back(v.y);
        sphereVertices.push_back(v.z);

        sphereVertices.push_back(n.x);
        sphereVertices.push_back(n.y);
        sphereVertices.push_back(n.z);
    };

    push(a);
    push(b);
    push(c);
}

void subdivide(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, int depth) {
    if (depth == 0)
    {
        emitTriangle(v0, v1, v2);
        return;
    }

    glm::vec3 m0 = normalizeToSphere((v0 + v1) * 0.5f, RADIUS);
    glm::vec3 m1 = normalizeToSphere((v1 + v2) * 0.5f, RADIUS);
    glm::vec3 m2 = normalizeToSphere((v2 + v0) * 0.5f, RADIUS);

    subdivide(v0, m0, m2, depth - 1);
    subdivide(m0, v1, m1, depth - 1);
    subdivide(m2, m1, v2, depth - 1);
    subdivide(m0, m1, m2, depth - 1);
}



int main(void)
{
    /* Initialize the library */
    if (!glfwInit()) {
        exit (-1);
    }
    // throw std::runtime_error("Error! initialization of glfw failed!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    int winWidth = 1000;
    float aspectRatio = 1.0; // 16.0 / 9.0; // winWidth / (float)winHeight;
    int winHeight = winWidth / aspectRatio;

    PerCamera camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),  45.0f,  winWidth / (float)winHeight, 0.1f, 100.0f);

    
    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "GLFW Example", NULL, NULL);
    if (!window) {
        std::cerr << "GLFW did not create a window!" << std::endl;
        
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err=glewInit();
    if(err != GLEW_OK) {
        std::cerr <<"GLEW Error! glewInit failed, exiting."<< std::endl;
        exit(EXIT_FAILURE);
    }
    
    const GLubyte* renderer = glGetString (GL_RENDERER);
    const GLubyte* version = glGetString (GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(1.0, 0.7, 1.0, 1.0);

    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);

    Scene sc;
    
    std::shared_ptr<ISceneLoader> loader = std::make_shared<SceneLoader>(sc);
    SceneParser_JSON parser(loader);

    // std::string filename= "C:/Users/ajcar/CS4212/starterCode/sceneData-main/scenes_A/lotsOTriangles.json";;
    // parser.parseFileData(filename);


    // std::vector<float> sphereVertices = sc.glPrepareToRasterize();
    // std::cout<<sphereVertices.size()<<std::endl;

    //----------------------------------------------------------------

    std::vector<glm::vec3> cube = {
        {-1,-1, 1}, { 1,-1, 1}, { 1, 1, 1},
        {-1,-1, 1}, { 1, 1, 1}, {-1, 1, 1},
        { 1,-1,-1}, {-1,-1,-1}, {-1, 1,-1},
        { 1,-1,-1}, {-1, 1,-1}, { 1, 1,-1},
        {-1,-1,-1}, {-1,-1, 1}, {-1, 1, 1},
        {-1,-1,-1}, {-1, 1, 1}, {-1, 1,-1},
        { 1,-1, 1}, { 1,-1,-1}, { 1, 1,-1},
        { 1,-1, 1}, { 1, 1,-1}, { 1, 1, 1},
        {-1, 1, 1}, { 1, 1, 1}, { 1, 1,-1},
        {-1, 1, 1}, { 1, 1,-1}, {-1, 1,-1},
        {-1,-1,-1}, { 1,-1,-1}, { 1,-1, 1},
        {-1,-1,-1}, { 1,-1, 1}, {-1,-1, 1}
    };

    for (auto &v : cube) {
        v = normalizeToSphere(v, RADIUS);
    }

    for (int i = 0; i < cube.size(); i += 3)
    {
        subdivide(cube[i],cube[i+1],cube[i+2],SUBDIV_LEVEL);
    }

    //----------------------------------------------------------------
        // Need to set a projection matrix that fits the aspect ratio set
        // by the window frame.

        // The ortho parameters, in order: left, right, bottom, top, zNear, zFar
        //         float halfWidth = 15.0 / 2.0;
        //         float halfHeight = halfWidth / aspectRatio;
        //         glm::mat4 projectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -10.0f, 10.0f);

    GLint major_version;
    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    std::cout << "GL_MAJOR_VERSION: " << major_version << std::endl;

    //Initialize all my data and get it on the gpu.
    //Load the scene file...
    //get my shapes in the scene...
    
    //load a triangle....
    //Create a vertex array buffer to hold our triangle data
    //clang formatter is cool

    // create a Vertex Array Buffer to hold our triangle data
    GLuint m_triangleVBO[1], m_VAO;   
    

    glGenBuffers(1, m_triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO[0]);

    // this is the actual triangle data that will be copied to                                              
    // the GPU memory                                                                                       
    // std::vector<float> host_VertexBuffer{
    
    // };                        

    
    int stride = 6;
    int numBytes = sphereVertices.size() * sizeof(float);
    int numVertices = sphereVertices.size() / stride;

    // copy the numBytes from host_VertexBuffer t the GPU and store in                                      
    // the currently bound VBO                                                                              
    glBufferData(GL_ARRAY_BUFFER, numBytes, sphereVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // once copied, we no longer need the data on the host                                                  
    sphereVertices.clear();

    // create a vertex array object that will map the attributes in                                         
    // our vertex buffer to different location attributes for our                                           
    // shaders                                                                                              
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // VAO details here - we only have 1 attribute or location                                              
    // (Position of the vertex)                                                                             
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)0);


    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,stride * sizeof(float),(void*)(3*sizeof(float)));
    
    // glEnableVertexAttribArray(2);
    // glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,stride*sizeof(float),(void*)(6*sizeof(float)));
    
    glBindVertexArray(0);
    // Create a shader using my GLSLObject class                                                            
    sivelab::GLSLObject shader;
    shader.addShader( "vertexShader_PrepForPerFragment.glsl", sivelab::GLSLObject::VERTEX_SHADER );
    shader.addShader( "blinnPhongFragmentShader.glsl", sivelab::GLSLObject::FRAGMENT_SHADER );
    shader.createProgram();

    GLuint projMatrixID, viewMatrixID,modelMatrixID, normalMatrixID;
    projMatrixID = shader.createUniform( "projMatrix" );
    viewMatrixID = shader.createUniform( "viewMatrix" );
    modelMatrixID = shader.createUniform("modelMatrix");
    normalMatrixID = shader.createUniform("normalMatrix");

    GLuint lightPostId = shader.createUniform("lightPosWorld");
    GLuint diffuseComponentID = shader.createUniform("diffuseComponent");
    GLuint cameraPosID = shader.createUniform("cameraPosWorld");

    glm::vec3 m_pos(0,0,0), m_viewDir(0,0,-1);
    glm::vec3 m_U(1,0,0), m_V(0,1,0), m_W(0,0,1);
    double timeDiff = 0.0, startFrameTime = 0.0, endFrameTime = 0.0;
     // The ortho parameters, in order: left, right, bottom, top, zNear, zFar
    float halfWidth = 15.0 / 2.0;
    float halfHeight = halfWidth;

    float left = -halfWidth;
    float right = halfWidth;

    float bottom = -halfHeight;
    float top = halfHeight;

    float near = 5.0f;
    float far = -5.0f;

    glm::mat4 M_ortho = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, near, far);
    float rotX = 0.0f;
    float rotY = 0.0f;
    float rotZ = 0.0f;
    float scale = 1.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        endFrameTime = glfwGetTime();
        timeDiff = endFrameTime - startFrameTime;
        startFrameTime = glfwGetTime();

        // Clear the window's buffer (or clear the screen to our
        // background color)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // create the view matrix from our camera data                                                                                                   
        glm::mat4 M_view = camera.getViewMatrix();
        glm::mat4 M_proj = camera.getProjectionMatrix();
        glm::mat4 M_model = glm::mat4(1.0f);

        M_model = glm::rotate(M_model, rotX, glm::vec3(1,0,0)); 
        M_model = glm::rotate(M_model, rotY, glm::vec3(0,1,0));
        M_model = glm::rotate(M_model, rotZ, glm::vec3(0,0,1)); 

        M_model = glm::scale(M_model, glm::vec3(scale, scale, scale));


        glm::mat4 M_normal = glm::transpose(glm::inverse(glm::mat4(M_model)));


        /* Render your objects here */
        shader.activate();
        glm::vec4 lightPos(0.0f, 0.0f, 2.0f, 1.0f);
        glm::vec3 diffuseComponent(0.0f,1.0f,1.0f);
        glm::vec4 cameraPosWorld(camera.position, 1.0f);
        glUniform4fv(cameraPosID, 1, glm::value_ptr(cameraPosWorld));
        glUniform4fv(lightPostId, 1, glm::value_ptr(lightPos));
        glUniform3fv(diffuseComponentID, 1, glm::value_ptr(diffuseComponent));
        // copy from the host to the device the view matrix and the projection matrix                                                                                       
        glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr( M_proj ));
        glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr( M_view ));
        glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(M_model));
        glUniformMatrix4fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(M_normal));


        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
        glBindVertexArray(0);
        shader.deactivate();
        // Swap the front and back buffers
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        if (glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }
        float moveRatePerFrame = 0.002;
        float rotSpeed = 0.002f;
        float scaleSpeed = 0.001f;


        glm::vec3 forward = glm::normalize(camera.target - camera.position);
        glm::vec3 right = glm::normalize(glm::cross(forward, camera.up));

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.position += forward * moveRatePerFrame;
            camera.target += forward * moveRatePerFrame;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.position -= forward * moveRatePerFrame;
            camera.target -= forward * moveRatePerFrame;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.position -= right * moveRatePerFrame;
            camera.target -= right * moveRatePerFrame;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.position += right * moveRatePerFrame;
            camera.target += right * moveRatePerFrame;
        }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            rotX += rotSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
            rotY += rotSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            rotZ += rotSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        std::cout << "fps: " << 1.0 / timeDiff << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            scale += scaleSpeed;  
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            scale -= scaleSpeed;   
        }
    }
  
    glfwTerminate();
    return 0;
}
