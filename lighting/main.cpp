#include "./vendor/imgui.h"
#include "./vendor/imgui_impl_glfw.h"
#include "./vendor/imgui_impl_opengl3.h"
#include <OpenGlUtils.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ObjLoader.h>


#define DEBUG 0

using namespace std;
using namespace glm;
using namespace objl;

int main(void){

    Window w(1080,720,"Dio Cane");
    glewInit();
    
    //////////////////////////IMGUI INITIAIZATION
    const char* glsl_version = "#version 150";

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(w.data(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ///////////////////////////////////////////////

    
    Loader loader;

#if DEBUG
    bool loaded = loader.LoadFile("/Users/giannandreavicidomini/Downloads/sample.obj");
#else
    bool loaded = loader.LoadFile("/Users/giannandreavicidomini/Downloads/dragon.obj");
#endif

    if(!loaded){
        printf("DAAAAMN\n");
    }


    Mesh mesh = loader.LoadedMeshes[0];
    
    
    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,mesh.Vertices.size()*sizeof(Vertex),mesh.Vertices.data(),GL_STATIC_DRAW);

    GLuint ibo;
    glGenBuffers(1,&ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,mesh.Indices.size()*sizeof(unsigned int),mesh.Indices.data(),GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    GLuint program = createProgram("./shaders/vs.glsl","./shaders/fs.glsl");
    glUseProgram(program);

    mat4 proj;
    mat4 modView;
    mat4 model;
    vec4 lightPos;

    GLuint prLoc = glGetUniformLocation(program,"projMat");
    GLuint mvLoc = glGetUniformLocation(program,"viewMat");
    GLuint mLoc = glGetUniformLocation(program,"modMat");
    GLuint lLoc = glGetUniformLocation(program,"lightPosition");
   

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    float increment;
    float traslationX=0;
    float traslationY=-6;
    float traslationZ=-12;
    float frequency =2;
    float intensity = 0.0;

    float lTraslationX=0;
    float lTraslationY=0;
    float lTraslationZ=0;

    float cameraZ=0;

    bool isRotating;

    float rotation;
    float stationary;
    float rotationFactor=rotation;
    
    while(!w.closed()){
        ////////////IMGUI STUFF
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ////////////////////////

        w.refresh(0.135,0.15,0.1759,1);
        rotation = (float)(glfwGetTime()*2);
        increment = 1+pow(sin(glfwGetTime()*frequency)*intensity,2);
        proj = perspective(1.0432f,w.screenRatio(),0.100f,1000.0f);

        //view = translate(mat4(1.0),vec3(0,0,-4));
        model=translate(mat4(1.0),vec3(traslationX,traslationY,traslationZ))*rotate(mat4(1.0),rotationFactor,vec3(0,1,0))*scale(mat4(1.0),vec3(increment,increment,increment));
        //modView = translate(mat4(1.0),vec3(0,0,-6));
        modView = lookAt(vec3(0,0,cameraZ),vec3(0,0,-cameraZ)+vec3(0,0,1.0),vec3(0,1,0));

        //lightPos = translate(mat4(1.0),vec3(lTraslationX,lTraslationY,lTraslationZ))*vec4(0,0,0,1);
        

        glUniformMatrix4fv(prLoc,1,GL_FALSE,value_ptr(proj));
        glUniformMatrix4fv(mvLoc,1,GL_FALSE,value_ptr(modView));
        glUniformMatrix4fv(mLoc,1,GL_FALSE,value_ptr(model));
        //glUniform3f(lLoc,lightPos.x,lightPos.y,lightPos.z);
        glUniform3f(lLoc,lTraslationX,lTraslationY,lTraslationZ);
        glDrawElements(GL_TRIANGLES,mesh.Indices.size(),GL_UNSIGNED_INT,nullptr);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            //static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, god swine!");                          // Create a window called "Hello, world!" and append into it.

            //ImGui::Text("This is the vagina of disemboweled virgin mary.");               // Display some text (you can use a format strings too)
            //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            //ImGui::Checkbox("Another Window", &show_another_window);
            if(ImGui::Checkbox("isRotating",&isRotating)){
                if(!isRotating){
                    stationary = rotation;
                }
                else{                    
                    
                    rotation = stationary;
                    
                }
            }
            ImGui::SliderFloat("posX", &traslationX, -24.0f, 24.0f);
            ImGui::SliderFloat("posY", &traslationY, -24.0f, 24.0f);
            ImGui::SliderFloat("posZ", &traslationZ, -24.0f, 24.0f);
            ImGui::SliderFloat("scaleFreq", &frequency, 1.0f, 6.0f); 
            ImGui::SliderFloat("scaleIntens", &intensity, 0.0f, 3.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();

            ImGui::Begin("Light settings");
            ImGui::SliderFloat("posX", &lTraslationX, -24.0f, 24.0f);
            ImGui::SliderFloat("posY", &lTraslationY, -24.0f, 24.0f);
            ImGui::SliderFloat("posZ", &lTraslationZ, -24.0f, 24.0f);
            ImGui::SliderFloat("cameraZ", &cameraZ, -24.0f, 24.0f);
            ImGui::End();

        }
        
        
        if(isRotating) rotationFactor = rotation;
        else rotationFactor = stationary;

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        w.update();
    }

    auto mammt =vec3(1.0,1.0,1.0)*vec3(0.5,0.5,0.5);
    cout<<mammt.x<<mammt.y<<mammt.z<<endl;

    /////IMGUI Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}

