#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "stb_image.h"

#include <iostream>
#include <direct.h> 
#define MAX_PATH 1000

std::vector<float> sphereData;//顶点信息
std::vector<unsigned int> sphereIndices;//ebo索引

struct materialAttribute {//材质属性
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLfloat shininess;
}material[9] = { 
    {glm::vec3(0.65,0.65,0.67),glm::vec3(0.65,0.65,0.67),glm::vec3(0.1),32},
    {glm::vec3(0.92,0.59,0.2),glm::vec3(0.92,0.59,0.2),glm::vec3(0.1),32},
    {glm::vec3(0.18,0.3,0.55),glm::vec3(0.18,0.3,0.55),glm::vec3(0.1),32},
    {glm::vec3(0.74,0.74,0.79),glm::vec3(0.74,0.74,0.79),glm::vec3(0.1),32},
    {glm::vec3(0.91,0.63,0.42),glm::vec3(0.91,0.63,0.42),glm::vec3(0.1),32},
    {glm::vec3(0.56,0.47,0.33),glm::vec3(0.56,0.47,0.33),glm::vec3(0.1),32},
    {glm::vec3(0.82,0.62,0.41),glm::vec3(0.82,0.62,0.41),glm::vec3(0.1),32},
    {glm::vec3(0.76,0.89,0.92),glm::vec3(0.76,0.89,0.92),glm::vec3(0.1),32},
    {glm::vec3(0.035,0.55,0.85),glm::vec3(0.035,0.55,0.85),glm::vec3(0.1),32}
};

struct lightAttribute {//光照属性
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
}light[9] = {
    {glm::vec3(0.5),glm::vec3(0.5),glm::vec3(1)}, 
    {glm::vec3(0.5),glm::vec3(0.5),glm::vec3(1)},
    {glm::vec3(0.5),glm::vec3(0.5),glm::vec3(1)},
    {glm::vec3(0.5),glm::vec3(0.5),glm::vec3(1)},
    {glm::vec3(0.5),glm::vec3(0.5),glm::vec3(1)},
    {glm::vec3(0.5),glm::vec3(0.5),glm::vec3(1)},
    {glm::vec3(0.5),glm::vec3(0.5),glm::vec3(1)},
    {glm::vec3(0.5),glm::vec3(0.5),glm::vec3(1)},
    {glm::vec3(0.5),glm::vec3(0.5),glm::vec3(1)}
};

float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f,  

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f,  
};

float rectangleVertices[] =
{
   1.0f, 1.0f, 1.0f,1.0f,1.0f,   // 右上角
    1.0f, 0.0f, 1.0f,1.0f,0.0f,  // 右下角
    0.0f, 1.0f, 1.0f,0.0f,1.0f,  // 左上角
    // 第二个三角形
    1.0f, 0.0f, 1.0f,1.0f,0.0f,  // 右下角
    0.0f, 0.0f, 1.0f,0.0f,0.0f, // 左下角
    0.0f, 1.0f, 1.0f,0.0f,1.0f   // 左上角
};

struct cubeLoc {
    GLfloat radius;
    GLfloat theta;
};

unsigned int loadTexture(char const* path);
void renderCube(unsigned int cubeVAO);
void setCubeTransMov(Shader* shader, GLfloat scale, glm::vec3 revoPoint, GLfloat revoRadius, GLfloat theta, GLfloat revoSpeed);
void genSphereValue(std::vector<float> &data, std::vector<unsigned int> &indices);//生成球体顶点数据
void renderSphere(unsigned int sphereVAO);//绘制球体
glm::vec3 setSphereTransMov(Shader* shader, GLfloat scale, glm::vec3 revoPoint, GLfloat revoRadius, GLfloat revoSpeed, GLfloat rotaSpeed);//设置变换和运动
void setSphereMaterialColor(Shader* shader, unsigned int TVO, struct materialAttribute m, struct lightAttribute l);//设置颜色和材质

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 20.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 sunPos(0.0f, 0.0f, 0.0f);//太阳位置
float timeSpeed = 1.0f;//时间

int main()
{
    char buffer[MAX_PATH];
    _getcwd(buffer, MAX_PATH);
    std::cout << buffer;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Setup Dear ImGui context
    const char* glsl_version = "#version 330";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = false;
    bool show_another_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

//--------------------------------------------------------------------------------------
    genSphereValue(sphereData, sphereIndices);
    unsigned int sphereVBO, sphereEBO, cubeVBO, rectangleVBO;
    glGenBuffers(1, &sphereEBO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &rectangleVBO);
 
    Shader sphere("sphere.vs", "sphere.fs");
    Shader cube("cube.vs", "cube.fs");
    Shader rectangle("rectangle.vs", "rectangle.fs");

    unsigned int sunDiffuseMap = loadTexture("./planet_texture./sun.jpg");
    unsigned int earthDiffuseMap = loadTexture("./planet_texture/earth.jpg");
    unsigned int venusDiffuseMap = loadTexture("./planet_texture/venus.jpg");
    unsigned int mercuryDiffuseMap = loadTexture("./planet_texture/mercury.jpg");
    unsigned int marsDiffuseMap = loadTexture("./planet_texture/mars.jpg");
    unsigned int saturnDiffuseMap = loadTexture("./planet_texture/saturn.jpg");
    unsigned int jupiterDiffuseMap = loadTexture("./planet_texture/jupiter.jpg");
    unsigned int moonDiffuseMap = loadTexture("./planet_texture/moon.jpg");
    unsigned int uranusDiffuseMap = loadTexture("./planet_texture/uranus.jpg");
    unsigned int neptuneDiffuseMap = loadTexture("./planet_texture/neptune.jpg");
    unsigned int starsMap = loadTexture("./planet_texture/stars.jpg");
    
    unsigned int sphereVAO[10];
    for (int i = 0; i < 10; i++)
    {
        glGenVertexArrays(1, &sphereVAO[i]);

        glBindVertexArray(sphereVAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
        glBufferData(GL_ARRAY_BUFFER, sphereData.size() * sizeof(float), &sphereData[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), &sphereIndices[0], GL_STATIC_DRAW);

        float stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

    }

    unsigned int asteroidBeltVAO[5000];
    struct cubeLoc asteroidBeltLoc[5000];
    for (int i = 0; i < 5000; i++)
    {
        glGenVertexArrays(1, &asteroidBeltVAO[i]);
        glBindVertexArray(asteroidBeltVAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        asteroidBeltLoc[i].radius = (float(rand()) / float(RAND_MAX))*float(7) + float(25);
        asteroidBeltLoc[i].theta = rand();
    }

    unsigned int saturnRingVAO[2500];
    struct cubeLoc saturnRingLoc[2500];
    for (int i = 0; i < 2500; i++)
    {
        glGenVertexArrays(1, &saturnRingVAO[i]);
        glBindVertexArray(saturnRingVAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        saturnRingLoc[i].radius = (float(rand()) / float(RAND_MAX)) * float(2.2) + float(3.5);
        saturnRingLoc[i].theta = rand();
    }

    unsigned int rectangleVAO[6];
    for (int i = 0; i < 6; i++)
    {
        glGenVertexArrays(1, &rectangleVAO[i]);
        glBindVertexArray(rectangleVAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
    }

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//------------------------------------------------------------------
        //--------------------------------------------------------------------------gui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
        
        if (show_another_window)
        {
            ImGui::Begin("Time passing speed", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("-                +");
            ImGui::SliderFloat("Times", &timeSpeed, 0.1f, 10.0f);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //---------------------------------------------------------------------------gui
        glDisable(GL_DEPTH_TEST);

        


        glEnable(GL_DEPTH_TEST);

        rectangle.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        rectangle.setMat4("projection", projection);
        rectangle.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(50));
        rectangle.setMat4("model", model);

        glBindTexture(GL_TEXTURE_2D, starsMap);
        glBindVertexArray(rectangleVAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        sphere.use();//sun
        setSphereTransMov(&sphere, 5, glm::vec3(0, 0, 0), 0,0,0);
        setSphereMaterialColor(&sphere, sunDiffuseMap, material[0], lightAttribute{ glm::vec3(1), glm::vec3(1), glm::vec3(1) });
        renderSphere(sphereVAO[0]);

        sphere.use();//水
        setSphereTransMov(&sphere, 0.1, glm::vec3(0, 0, 0), 10, 4.2, 1);
        setSphereMaterialColor(&sphere, mercuryDiffuseMap, material[0], light[0]);
        renderSphere(sphereVAO[1]);

        sphere.use();//金
        setSphereTransMov(&sphere, 0.3, glm::vec3(0, 0, 0), 13, 1.63, 1);
        setSphereMaterialColor(&sphere, venusDiffuseMap, material[1], light[1]);
        renderSphere(sphereVAO[2]);

        sphere.use();
        glm::vec3 earthLoc = setSphereTransMov(&sphere, 0.35, glm::vec3(0, 0, 0), 17, 1,10);
        setSphereMaterialColor(&sphere, earthDiffuseMap, material[1], light[1]);
        renderSphere(sphereVAO[3]);


        sphere.use();//月
        setSphereTransMov(&sphere, 0.11, earthLoc, 1.2, 13.5,1);
        setSphereMaterialColor(&sphere, moonDiffuseMap, material[3], light[3]);
        renderSphere(sphereVAO[4]);

        sphere.use();//火
        setSphereTransMov(&sphere, 0.2, glm::vec3(0, 0, 0), 21, 0.53,1);
        setSphereMaterialColor(&sphere, marsDiffuseMap, material[4], light[4]);
        renderSphere(sphereVAO[5]);

        sphere.use();//土
        glm::vec3 saLoc=setSphereTransMov(&sphere, 1.8, glm::vec3(0, 0, 0), 40, 0.085,0.3);
        setSphereMaterialColor(&sphere, saturnDiffuseMap, material[5], light[5]);
        renderSphere(sphereVAO[6]);

        sphere.use();//木
        setSphereTransMov(&sphere, 2.5, glm::vec3(0, 0, 0), 60, 0.034,0.3);
        setSphereMaterialColor(&sphere, jupiterDiffuseMap, material[6], light[6]);
        renderSphere(sphereVAO[7]);

        sphere.use();//天
        setSphereTransMov(&sphere, 0.8, glm::vec3(0, 0, 0), 80, 0.012,1);
        setSphereMaterialColor(&sphere, uranusDiffuseMap,material[7], light[7]);
        renderSphere(sphereVAO[8]);

        sphere.use();//海
        setSphereTransMov(&sphere, 0.6, glm::vec3(0, 0, 0), 110, 0.006,1);
        setSphereMaterialColor(&sphere, neptuneDiffuseMap, material[8], light[8]);
        renderSphere(sphereVAO[9]);

        for (int i = 0; i < 5000; i++)
        {
            if (i < 2500)
            {
                cube.use();
                setCubeTransMov(&cube, 0.05, saLoc, saturnRingLoc[i].radius, saturnRingLoc[i].theta, 0.5);
                cube.setVec3("cubeColor", glm::vec3(0.56, 0.47, 0.33));
                renderCube(saturnRingVAO[i]);
            }
            cube.use();
            setCubeTransMov(&cube, 0.1, glm::vec3(0, 0, 0), asteroidBeltLoc[i].radius, asteroidBeltLoc[i].theta, 0.5);
            cube.setVec3("cubeColor", glm::vec3(0.5f));
            renderCube(asteroidBeltVAO[i]);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    for (int i = 0; i < 10; i++)
    {
        glDeleteVertexArrays(1, &sphereVAO[i]);
    }
    glDeleteBuffers(1, &sphereVBO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
        if (timeSpeed < 10)
            timeSpeed += 0.1;
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
        if (timeSpeed > 0.0)
            timeSpeed -= 0.1;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


void renderSphere(unsigned int sphereVAO)
{
    unsigned int indexCount = sphereIndices.size();   
    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
    
}

void genSphereValue(std::vector<float>& data, std::vector<unsigned int>& indices)
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals; 
    std::vector<glm::vec2> uv;
    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    const float PI = 3.14159265359;
    const float temp = 0.015625;
    for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
    {
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            positions.push_back(glm::vec3(xPos, yPos, zPos));
            uv.push_back(glm::vec2(xSegment, ySegment));
            normals.push_back(glm::vec3(xPos, yPos, zPos));
        }
    }

    bool oddRow = false;
    for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
    {
        if (!oddRow) // even rows: y == 0, y == 2; and so on
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                indices.push_back(y * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            }
        }
        else
        {
            for (int x = X_SEGMENTS; x >= 0; --x)
            {
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back(y * (X_SEGMENTS + 1) + x);
            }
        }
        oddRow = !oddRow;
    }
    unsigned int indexCount = indices.size();

    for (unsigned int i = 0; i < positions.size(); ++i)
    {
        data.push_back(positions[i].x);
        data.push_back(positions[i].y);
        data.push_back(positions[i].z);
        if (uv.size() > 0)
        {
            data.push_back(uv[i].x);
            data.push_back(uv[i].y);
        }
        if (normals.size() > 0)
        {
            data.push_back(normals[i].x);
            data.push_back(normals[i].y);
            data.push_back(normals[i].z);
        }
    }
}

glm::vec3 setSphereTransMov(Shader* shader, GLfloat scale, glm::vec3 revoPoint, GLfloat revoRadius, GLfloat revoSpeed, GLfloat rotaSpeed)
{
    revoSpeed /= 2.0f;
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);
    float locX = sin(glfwGetTime()*revoSpeed*timeSpeed) * revoRadius;
    float locZ = cos(glfwGetTime()*revoSpeed*timeSpeed) * revoRadius;
    model = glm::translate(model, glm::vec3(locX,0,locZ));
    model = glm::translate(model, revoPoint);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f)*rotaSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scale));
    shader->setMat4("model", model);

    return glm::vec3(locX, 0, locZ);
}

void setSphereMaterialColor(Shader* shader, unsigned int TVO, struct materialAttribute m, struct lightAttribute l)
{
    shader->setInt("material.diffuse", 0);
    shader->setVec3("light.position", sunPos);
    shader->setVec3("viewPos", camera.Position);

    // light properties
    shader->setVec3("light.ambient", l.ambient);
    shader->setVec3("light.diffuse", l.diffuse);
    shader->setVec3("light.specular", l.specular);

    // material properties
    shader->setVec3("material.specular", m.specular);
    shader->setFloat("material.shininess", m.shininess);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TVO);
}


void renderCube(unsigned int cubeVAO)
{
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void setCubeTransMov(Shader* shader, GLfloat scale, glm::vec3 revoPoint, GLfloat revoRadius, GLfloat theta, GLfloat revoSpeed)
{
    revoSpeed /= 2.0f;
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);
    float locX = sin(glfwGetTime() * revoSpeed * timeSpeed+theta) * revoRadius;
    float locZ = cos(glfwGetTime() * revoSpeed * timeSpeed+theta) * revoRadius;
    model = glm::translate(model, glm::vec3(locX, 0, locZ));
    model = glm::translate(model, revoPoint);
    model = glm::scale(model, glm::vec3(scale));
    shader->setMat4("model", model);
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        //std::cout << width << height;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    std::cout << textureID;
    return textureID;
}
