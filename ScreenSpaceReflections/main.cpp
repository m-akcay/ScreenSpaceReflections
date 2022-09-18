#include "Util.h"

#include "Mesh.h"
#include "Skybox.h"
#include "Quad.h"

using namespace std;

int gWidth, gHeight;

GLint eyePosLoc;
GLint eyePosLoc1;
GLint eyePosLoc2;
GLint projMatLoc;

mat4 projectionMatrix;
mat4 viewingMatrix;
mat4 modelingMatrix;
vec3 eyePos(0, 0, 0);
vec3 gaze(0, 0, -1);
vec3 up(0, 1, 0);

bool firstMouse = true;
bool rightClicked = false;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 1200 / 2.0f;
float lastY = 1200 / 2.0f;
float fov = 45.0f;

std::shared_ptr<Quad> quad;
std::shared_ptr<Skybox> skybox;
std::vector<std::shared_ptr<Renderable>> renderableObjects;

GLuint fbo;
GLuint textureColorbuffer;
GLuint rbo;

GLuint dynamicCubemap;
GLuint dynamicCubemapLoc;
const vec3 fixedObjPos(0, -4, -8);

vec3 orbitingObjPos(0, 0, -1.5f);

void updateOrbitingObj()
{
    static float angle = 0;
    const vec3 baseVec(0, 0, 1);
    const vec3 baseUp(0, 1, 0);
    const float distance = 3;
    
    vec3 vec2Add = glm::rotate(mat4(1.0f), angle, baseUp) * vec4(baseVec, 1);
    orbitingObjPos = fixedObjPos + vec2Add * distance;
    //orbitingObjPos = fixedObjPos + vec3(3, 1, 1.5f);
    orbitingObjPos.y += 1;
    angle += 0.002f;
}

void renderScene();

void setUpDynamicCubemap()
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    dynamicCubemap = Util::emptyCubemap(1200);
    for (int i = 0; i < 6; ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, dynamicCubemap, 0);
    }


    //glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1200, 1200);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
  

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint cubemap;
GLuint cubemapLoc;

void init()
{
    auto faces = std::vector<string>{ "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg" };
    skybox = std::make_shared<Skybox>(faces, "skyVert.glsl", "skyFrag.glsl");
    
    quad = std::make_shared<Quad>("secondPassVert.glsl", "secondPassFrag.glsl", 1200, 1200, faces);


    auto teapot = std::make_shared<Mesh>("teapot.obj", "vert.glsl", "frag.glsl");
    teapot->setShaderFloat("reflectiveness", 0.5f);
    renderableObjects.push_back(teapot);

    auto teapot2 = std::make_shared<Mesh>("teapot.obj", "vert.glsl", "frag.glsl");
    teapot2->setShaderFloat("reflectiveness", 0.5f);
    renderableObjects.push_back(teapot2);

    auto teapot3 = std::make_shared<Mesh>("teapot.obj", "vert.glsl", "frag.glsl");
    teapot3->setShaderFloat("reflectiveness", 0.5f);
    renderableObjects.push_back(teapot3);

    auto cube = std::make_shared<Mesh>("cube.obj", "vert.glsl", "frag.glsl");
    renderableObjects.push_back(std::make_shared<Mesh>("cube.obj", "vert.glsl", "frag.glsl"));

    eyePosLoc = glGetUniformLocation(renderableObjects[0]->getProgram(), "inEyePos");
    glUniform3f(eyePosLoc, eyePos.x, eyePos.y, eyePos.z);
    eyePosLoc1 = glGetUniformLocation(renderableObjects[1]->getProgram(), "inEyePos");
    glUniform3f(eyePosLoc, eyePos.x, eyePos.y, eyePos.z);
    eyePosLoc2 = glGetUniformLocation(renderableObjects[2]->getProgram(), "inEyePos");
    glUniform3f(eyePosLoc, eyePos.x, eyePos.y, eyePos.z);

    quad->activateProgram();
    eyePosLoc = glGetUniformLocation(quad->getProgram(), "eyePos");
    glUniform3f(eyePosLoc, eyePos.x, eyePos.y, eyePos.z);

    float fovyRad = (float)(45.0 / 180.0) * M_PI;
    projectionMatrix = glm::perspective(fovyRad, 1.0f, 0.1f, 100.0f);
    projMatLoc = glGetUniformLocation(quad->getProgram(), "gProjectionMatrix");
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    
    cubemap = Util::loadCubemap(faces);
    cubemapLoc = glGetUniformLocation(quad->getProgram(), "skybox");
    //std::cout << "CUBEMAPLOCCCC_________" << cubemapLoc << std::endl;
    glEnable(GL_DEPTH_TEST);

}


void drawModels(int face)
{
    static const int ignoreObj = 0;
    float fovyRad = (float)(90.0 / 180.0) * M_PI;
    static float angle = 0;
    mat4 viewMat;
    mat4 skyboxViewMat;
    vec3 fwdVec;
   
    viewMat = glm::lookAt(eyePos, eyePos + gaze, vec3(0, 1, 0));
    skyboxViewMat = glm::lookAt(vec3(0.0f), gaze, vec3(0, 1, 0));

    auto projMat = glm::perspective(fovyRad, 1.0f, 0.01f, 100.0f);
    {
        //eyePos = vec3(0, 0, 0);
        glDepthFunc(GL_LEQUAL);
        skybox->activateProgram();
        skybox->setViewMat(skyboxViewMat);
        skybox->setProjMat(projMat);
        glBindVertexArray(skybox->getVao());
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getCubemap());
        glDrawElements(GL_TRIANGLES, skybox->getIndexCount(), GL_UNSIGNED_INT, (void*)0);
        glDepthFunc(GL_LESS);

        for (int i = 0; i < renderableObjects.size(); ++i)
        {
            // avoid self render of the reflective obj
            if (i == ignoreObj && face != 6)
            {
                std::cout << "cont" << std::endl;
                continue;
            }
            
            auto obj = renderableObjects[i];

            obj->activateProgram();

            if (i == 0)
            {
                auto matT = glm::translate(mat4(1.0f), fixedObjPos);
                matT = glm::scale(matT, vec3(0.75f));
                obj->setModelMat(matT);
            }
            else if (i == 1)
            {
                auto matT = glm::translate(mat4(1.0f), fixedObjPos + vec3(-6, 0, 0));
                matT = glm::scale(matT, vec3(0.75f));
                obj->setModelMat(matT);
            }
            else if (i == 2)
            {
                auto matT = glm::translate(mat4(1.0f), fixedObjPos + vec3(6, 0, 0));
                matT = glm::scale(matT, vec3(0.75f));
                obj->setModelMat(matT);
            }
            else
            {
                auto matT = glm::translate(mat4(1.0f), orbitingObjPos);
                matT = glm::scale(matT, vec3(0.25f));
                obj->setModelMat(matT);
            }

            obj->setViewMat(viewMat);
            obj->setProjMat(projMat);

            glBindVertexArray(obj->getVao());
            glDrawElements(GL_TRIANGLES, obj->getIndexCount(), GL_UNSIGNED_INT, (void*)0);
        }
    }
    
}

void renderScene()
{
    glBindFramebuffer(GL_FRAMEBUFFER, quad->getFbo());
    quad->activateProgram();
    quad->bindColorTexture();
    quad->bindDepthTexture();
    quad->bindNormalTexture();
    quad->bindPosTexture();
    quad->bindReflectionTexture();
    quad->bindCubeTex();

    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5};
    glDrawBuffers(6, drawBuffers);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 1200, 1200);

    drawModels(6);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    quad->activateProgram();

    glBindVertexArray(quad->getVao());
    glDisable(GL_DEPTH_TEST);

    glDrawElements(GL_TRIANGLES, quad->getIndexCount(), GL_UNSIGNED_INT, (void*)0);


    updateOrbitingObj();
}

void display()
{
    // Draw the scene
    renderScene();

}

void reshape(GLFWwindow* window, int w, int h)
{
    w = w < 1 ? 1 : w;
    h = h < 1 ? 1 : h;

    gWidth = w;
    gHeight = h;

    glViewport(0, 0, w, h);

    // Use perspective projection

    float fovyRad = (float)(45.0 / 180.0) * M_PI;
    projectionMatrix = glm::perspective(fovyRad, 1.0f, 0.1f, 100.0f);


    viewingMatrix = glm::lookAt(eyePos, vec3(0, 0, -1), vec3(0, 1, 0));

}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        eyePos += up * 0.5f;
        glUniform3f(eyePosLoc, eyePos.x, eyePos.y, eyePos.z);
        glUniform3f(eyePosLoc1, eyePos.x, eyePos.y, eyePos.z);
        glUniform3f(eyePosLoc2, eyePos.x, eyePos.y, eyePos.z);
    }
    else if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        eyePos -= up * 0.5f;
        glUniform3f(eyePosLoc, eyePos.x, eyePos.y, eyePos.z);
        glUniform3f(eyePosLoc1, eyePos.x, eyePos.y, eyePos.z);
        glUniform3f(eyePosLoc2, eyePos.x, eyePos.y, eyePos.z);
    }
    else if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        eyePos += gaze * 0.5f;
        glUniform3f(eyePosLoc, eyePos.x, eyePos.y, eyePos.z);
        glUniform3f(eyePosLoc1, eyePos.x, eyePos.y, eyePos.z);
        glUniform3f(eyePosLoc2, eyePos.x, eyePos.y, eyePos.z);
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        eyePos -= gaze * 0.5f;
        glUniform3f(eyePosLoc, eyePos.x, eyePos.y, eyePos.z);
        glUniform3f(eyePosLoc1, eyePos.x, eyePos.y, eyePos.z);
        glUniform3f(eyePosLoc2, eyePos.x, eyePos.y, eyePos.z);
    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        auto right = glm::normalize(glm::cross(gaze, up));
        eyePos -= right * 0.5f;
        glUniform3f(eyePosLoc, eyePos.x, eyePos.y, eyePos.z);
        glUniform3f(eyePosLoc1, eyePos.x, eyePos.y, eyePos.z);
        glUniform3f(eyePosLoc2, eyePos.x, eyePos.y, eyePos.z);
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        auto right = glm::normalize(glm::cross(gaze, up));
        eyePos += right * 0.5f;
        glUniform3f(eyePosLoc, eyePos.x, eyePos.y, eyePos.z);
        glUniform3f(eyePosLoc1, eyePos.x, eyePos.y, eyePos.z);
        glUniform3f(eyePosLoc2, eyePos.x, eyePos.y, eyePos.z);
    }
    else if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
        //glShadeModel(GL_SMOOTH);
        //activeProgramIndex = 0;
        gaze = glm::normalize(glm::rotate(mat4(1.0f), 1.0f, vec3(0, 1, 0)) * vec4(gaze, 1));
        glUniform3f(eyePosLoc, eyePos.x, eyePos.y, eyePos.z);
        glUniform3f(eyePosLoc1, eyePos.x, eyePos.y, eyePos.z);
        glUniform3f(eyePosLoc2, eyePos.x, eyePos.y, eyePos.z);
        //eyePos.z -= 0.11f;
    }
    else if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        //glShadeModel(GL_SMOOTH);
    }
    else if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        //glShadeModel(GL_FLAT);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
            rightClicked = true;
        else if (action == GLFW_RELEASE)
        {
            rightClicked = false;
            firstMouse = true;
        }
    }

}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (!rightClicked)
        return;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    gaze = glm::normalize(front);
}

void mainLoop(GLFWwindow* window)
{
    double lastTime = glfwGetTime();
    unsigned long nbFrames = 0;
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
            // printf and reset timer
            double fps = 1 / (1000.0 / double(nbFrames)) * 100;
            printf("%f fps\n", fps);
            nbFrames = 0;
            lastTime += 1.0;
        }
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


int main(int argc, char** argv)   // Create Main Function For Bringing It All Together
{
    GLFWwindow* window;
    if (!glfwInit())
    {
        exit(-1);
    }

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    int width = 1200, height = 1200;
    window = glfwCreateWindow(width, height, "Simple Example", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    char rendererInfo[512] = { 0 };
    strcpy(rendererInfo, (const char*)glGetString(GL_RENDERER));
    strcat(rendererInfo, " - ");
    strcat(rendererInfo, (const char*)glGetString(GL_VERSION));
    glfwSetWindowTitle(window, rendererInfo);
    reshape(window, width, height); // need to call this once ourselves

    init();

    glfwSetKeyCallback(window, keyboard);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetWindowSizeCallback(window, reshape);

    mainLoop(window); // this does not return unless the window is closed

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
