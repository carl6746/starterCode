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
#include "robot.h"
#include "camera.h"


static const float RADIUS = 1.0f;
static const int SUBDIV_LEVEL = 5;

int CheckGLErrors(const char *s)
{
    int errCount = 0;
    return errCount;
}

glm::vec3 normalizeToSphere(glm::vec3 v, float R) {
    return glm::normalize(v) * R;
}
    
std::vector<float> sphereVertices;
std::vector<float> cubeVertices;
std::vector<float> planeVertices;

struct MovingBall {
    glm::vec3 position;
    glm::vec3 target;
    float speed;
    bool goingPositiveX; // which side we're heading toward
};

float randRange(float min, float max) {
    return min + (max - min) * (float(rand()) / float(RAND_MAX));
}

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

float lastX = 500.0f;
float lastY = 500.0f;
bool firstMouse = true;
bool mouseMode = true;

float sensitivity = 0.1f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!mouseMode) return; 
    PerCamera* cam = (PerCamera*)glfwGetWindowUserPointer(window);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cam->yaw   += xoffset;
    cam->pitch += yoffset;

    if (cam->pitch > 89.0f) cam->pitch = 89.0f;
    if (cam->pitch < -89.0f) cam->pitch = -89.0f;

    cam->updateCameraVectors();
}

glm::vec3 getGoalDirection(float time) {
    float amplitude = 10.0f; 
    float speed = 1.5f;     

    float x = amplitude * (speed * time);
    float z = amplitude * (speed * time);

    float fixedY = 5.0f; 

    return glm::vec3(x, fixedY, z);
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

    PerCamera camera(glm::vec3(0.0f, 0.0f, 10.0f),glm::vec3(0.0f, 1.0f, 0.0f),-90.0f,0.0f,45.0f,winWidth / (float)winHeight,0.1f,100.0f);


    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "GLFW Example", NULL, NULL);
    if (!window) {
        std::cerr << "GLFW did not create a window!" << std::endl;
        
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    
    // std::shared_ptr<ISceneLoader> loader = std::make_shared<SceneLoader>(sc);
    // SceneParser_JSON parser(loader);

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

    float planeScale = 12.0f;
    std::vector<glm::vec3> plane = {
        {-planeScale, 0, -planeScale},
        { planeScale, 0, -planeScale},
        { planeScale, 0,  planeScale},

        {-planeScale, 0, -planeScale},
        { planeScale, 0,  planeScale},
        {-planeScale, 0,  planeScale}
    };

    for (auto &v : cube) {
        v = normalizeToSphere(v, RADIUS);
    }

    for (int i = 0; i < cube.size(); i += 3)
    {
        subdivide(cube[i],cube[i+1],cube[i+2],SUBDIV_LEVEL);
    }

    //Storing the cube into something we want to keep.
    
    for (int i = 0; i < cube.size(); i += 3) {
        glm::vec3 a = cube[i];
        glm::vec3 b = cube[i+1];
        glm::vec3 c = cube[i+2];

        glm::vec3 n = glm::normalize(glm::cross(b - a, c - a));

        auto push = [&](glm::vec3 v) {
            cubeVertices.push_back(v.x);
            cubeVertices.push_back(v.y);
            cubeVertices.push_back(v.z);
            cubeVertices.push_back(n.x);
            cubeVertices.push_back(n.y);
            cubeVertices.push_back(n.z);
        };

        push(a); push(b); push(c);
    }

    glm::vec3 n(0,1,0);

    for (auto &v : plane) {
        planeVertices.push_back(v.x);
        planeVertices.push_back(v.y);
        planeVertices.push_back(v.z);

        planeVertices.push_back(n.x);
        planeVertices.push_back(n.y);
        planeVertices.push_back(n.z);
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
    int numSphereBytes = sphereVertices.size() * sizeof(float);
    int numSphereVertices = sphereVertices.size() / stride;
    int numCubeBytes = cubeVertices.size() * sizeof(float);
    int numCubeVertices = cubeVertices.size() / stride;
    int numPlaneBytes = planeVertices.size() * sizeof(float);
    int numPlaneVertices = planeVertices.size() / stride;

    // copy the numBytes from host_VertexBuffer t the GPU and store in                                      
    // the currently bound VBO                                                                              
    glBufferData(GL_ARRAY_BUFFER, numSphereBytes, sphereVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // once copied, we no longer need the data on the host                                                  
    sphereVertices.clear();
    GLuint cubeVBO, cubeVAO;

    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, numCubeBytes, cubeVertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3*sizeof(float)));

    glBindVertexArray(0);

    GLuint planeVBO, planeVAO;

    glGenBuffers(1, &planeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, planeVertices.size() * sizeof(float), planeVertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &planeVAO);
    glBindVertexArray(planeVAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));

    glBindVertexArray(0);
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
    shader.addShader( "vertexShader_robotSphere.glsl", sivelab::GLSLObject::VERTEX_SHADER );
    shader.addShader( "robotSphereFragmentShader.glsl", sivelab::GLSLObject::FRAGMENT_SHADER );
    shader.createProgram();

    GLuint projMatrixID, viewMatrixID,modelMatrixID, normalMatrixID;
    projMatrixID = shader.createUniform( "projMatrix" );
    viewMatrixID = shader.createUniform( "viewMatrix" );
    modelMatrixID = shader.createUniform("modelMatrix");
    normalMatrixID = shader.createUniform("normalMatrix");

    GLuint lightPosID = shader.createUniform("lightPosWorld");
    GLuint numLightsID = shader.createUniform("numLights");
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
    float scale = 0.3f;

    Robot grant(glm::vec3(0,0,-10));
    Robot steve(glm::vec3(0,0,10));
    steve.initSimpleArm();
    grant.initSimpleArm();
    int selectedTargetPos = 0;


        MovingBall ball;
        ball.position = glm::vec3(-10.0f, 5.0f, 0.0f);
        ball.speed = 8.0f;
        ball.goingPositiveX = true;
        ball.target = glm::vec3(10.0f, 5.0f, randRange(-10.0f, 10.0f));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        endFrameTime = glfwGetTime();
        timeDiff = endFrameTime - startFrameTime;
        startFrameTime = glfwGetTime();

        // Clear the window's buffer (or clear the screen to our
        // background color)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        std::vector<glm::vec3> potentialTargets = {glm::vec3(12.0,12.0,0.0),glm::vec3(24.0,1.0,1.0), glm::vec3(1,24.0,1),glm::vec3(1,1,24.0)};
        
        glm::vec3 steveLocalTarget = ball.target - steve.getAnchor();
        steve.setTarget(ball.target);
        steve.solveIK();
        steve.forwardKinematics();

        glm::vec3 grantLocalTarget = ball.target - grant.getAnchor();
        grant.setTarget(ball.target);
        grant.solveIK();
        grant.forwardKinematics();

        
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
        std::vector<glm::vec4> lights = {glm::vec4(0.0f, 10.0f, 10.0f, 1.0f),glm::vec4(0.0f, -10.0f, 10.0f, 1.0f),glm::vec4(0.0f, -10.0f, -10.0f, 1.0f),glm::vec4(0.0f, 10.0f, -10.0f, 1.0f)};
        glm::vec3 diffuseComponent(0.0f,1.0f,1.0f);
        glm::vec4 cameraPosWorld(camera.position, 1.0f);
        glUniform4fv(cameraPosID, 1, glm::value_ptr(cameraPosWorld));
        glUniform1i(numLightsID, lights.size());
        glUniform4fv(lightPosID, lights.size(), glm::value_ptr(lights[0]));         
        glUniform3fv(diffuseComponentID, 1, glm::value_ptr(diffuseComponent));
        // copy from the host to the device the view matrix and the projection matrix                                                                                       
        glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr( M_proj ));
        glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr( M_view ));
        //glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(M_model));
        glUniformMatrix4fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(M_normal));

        glm::mat4 M_steve_parent = glm::translate(glm::mat4(1.0f), steve.getAnchor()) * glm::scale(glm::mat4(1.0f), glm::vec3(scale));
        glm::mat4 M_grant_parent = glm::translate(glm::mat4(1.0f), grant.getAnchor()) * glm::scale(glm::mat4(1.0f), glm::vec3(scale));

        glm::mat4 M_plane = glm::mat4(1.0f);

        glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(M_plane));

        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, numPlaneVertices);

        glm::vec3 goalPoint = ball.position;
        glm::vec3 dir = ball.target - ball.position;
        float dist = glm::length(dir);

        if (dist < 0.2f) {
            ball.goingPositiveX = !ball.goingPositiveX;

            float z = ball.goingPositiveX ? 10.0f : -10.0f;
            float x = randRange(-8.0f, 8.0f);

            ball.target = glm::vec3(x, 0.5f, z);
        } else {
            glm::vec3 velocity = glm::normalize(dir) * ball.speed * (float)timeDiff;
            ball.position += velocity;
        }
        glm::mat4 M_ball =
        glm::translate(glm::mat4(1.0f), ball.position) *
        glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

        glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(M_ball));
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, numSphereVertices);
        // for(int i = 0; i<potentialTargets.size(); ++i ) {
        //     glm::mat4 M_link = M_model * glm::translate(glm::mat4(1.0f), potentialTargets[i]) * glm::scale(glm::mat4(1.0f), glm::vec3(.2, .2, .2));
        //     glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(M_link));
        //     glBindVertexArray(m_VAO);
        //     glDrawArrays(GL_TRIANGLES, 0, numSphereVertices);
        // }

        for (int i = 0; i < steve.joints.size(); ++i) {
            glm::mat4 R = glm::mat4_cast(steve.joints[i].rotation);
            glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, steve.joints[i].length));

            glm::mat4 M_model = M_steve_parent * R;

            glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(M_model));
            glBindVertexArray(m_VAO);
            glDrawArrays(GL_TRIANGLES, 0, numSphereVertices);

            float thickness = scale;

            glm::mat4 M_link = M_model * glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, steve.joints[i].length * 0.5f)) * glm::scale(glm::mat4(1.0f), glm::vec3(thickness, thickness, steve.joints[i].length));

            glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(M_link));
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, numCubeVertices);

            M_steve_parent = M_model * T;
        }

        for (int i = 0; i < grant.joints.size(); ++i) {
            glm::mat4 R = glm::mat4_cast(grant.joints[i].rotation);
            glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, grant.joints[i].length));

            glm::mat4 M_model = M_grant_parent * R;

            glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(M_model));
            glBindVertexArray(m_VAO);
            glDrawArrays(GL_TRIANGLES, 0, numSphereVertices);

            float thickness = scale;

            glm::mat4 M_link = M_model * glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, grant.joints[i].length * 0.5f)) * glm::scale(glm::mat4(1.0f), glm::vec3(thickness, thickness, grant.joints[i].length));

            glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(M_link));
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, numCubeVertices);

            M_grant_parent = M_model * T;
        }

        glBindVertexArray(0);

        shader.deactivate();
        // Swap the front and back buffers
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        
        float dt = timeDiff;
        float speed = 1.5f;
        float moveRatePerFrame = 5.0f*timeDiff;
        float rotSpeed = 0.002f;
        float scaleSpeed = 0.001f;

        glm::vec3  forward = camera.front;
        glm::vec3 right = glm::normalize(glm::cross(forward, camera.up));
        static bool nWasDown = false;

        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
            if (!nWasDown) {

                selectedTargetPos += 1;

                if (selectedTargetPos >= potentialTargets.size()) {
                    selectedTargetPos = 0;
                }

                std::cout << "Target " << selectedTargetPos << std::endl;
            }
            nWasDown = true;
        } else {
            nWasDown = false;
        }

        // if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        //     if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        //         robot.joints[0].rotation *= glm::inverse(angleAxis(speed * dt,glm::vec3(1,0,0)));
        //     } else {
        //         robot.joints[0].rotation *= glm::angleAxis(speed * dt,glm::vec3(1,0,0));
        //     }
        // }
        // if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        //     if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        //         robot.joints[0].rotation *= glm::inverse(angleAxis(speed * dt,glm::vec3(0,1,0)));
        //     } else {
        //         robot.joints[0].rotation *= glm::angleAxis(speed * dt,glm::vec3(0,1,0));
        //     }
        // }
        // if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        //     if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        //         robot.joints[1].rotation *= glm::inverse(angleAxis(speed * dt,glm::vec3(1,0,0)));
        //     } else {
        //         robot.joints[1].rotation *= glm::angleAxis(speed * dt,glm::vec3(1,0,0));
        //     }
        // }
        // if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        //     if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        //         robot.joints[1].rotation *= glm::inverse(angleAxis(speed * dt,glm::vec3(0,1,0)));
        //     } else {
        //         robot.joints[1].rotation *= glm::angleAxis(speed * dt,glm::vec3(0,1,0));
        //     }
        // }
        // if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        //     if(glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        //         robot.joints[2].rotation *= glm::inverse(angleAxis(speed * dt,glm::vec3(0,1,0)));
        //     } else {
        //         robot.joints[2].rotation *= glm::angleAxis(speed * dt,glm::vec3(0,1,0));
        //     }
        // }
        if (glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS) {
                    glfwSetWindowShouldClose(window, 1);
                }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.position += forward * moveRatePerFrame;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.position -= forward * moveRatePerFrame;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.position -= right * moveRatePerFrame;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.position += right * moveRatePerFrame;
        }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                rotX -= rotSpeed;
            } else {
                rotX += rotSpeed;           
            }
        }
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
            if(glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                rotY -= rotSpeed;
            } else {
                rotY += rotSpeed;           
            }
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                rotZ -= rotSpeed;
            } else {
                rotZ += rotSpeed;           
            }
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
        static bool mWasDown = false;
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
            if (!mWasDown) {
                mouseMode = !mouseMode;

                if (mouseMode) {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    firstMouse = true; // prevents camera jump
                } else {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
            }
            mWasDown = true;
        } else {
            mWasDown = false;
        }
        
        }
  
    glfwTerminate();
    return 0;
}
