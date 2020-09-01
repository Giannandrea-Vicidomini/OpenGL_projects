#include <OpenGlUtils.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

int main(void)
{

    Window window(800, 600, "AXISES");
    glewInit();
    glewExperimental = true;


    float offset = 0.5;
    float positions[] = {
        -0.5,-0.5,0.0,
        0.5,-0.5,0.0,
        0.5,0.5,0.0,
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(0);

    /*
    SampleTriangle t(positions,3,3);
    t.setProgram("./shaders/vs.glsl","./shaders/fsGreen.glsl");
    SampleTriangle s(positions2,3,3);
    s.setProgram("./shaders/vs.glsl","./shaders/fsRed.glsl");

    
    Axis x(-1.0f,0.0f,1.0f,0.0f);
    x.setProgram("./shaders/vs.glsl","./shaders/fsRed.glsl");
    Axis y(0.0,1.0,0.0,-1.0);
    y.setProgram("./shaders/vs.glsl","./shaders/fsGreen.glsl");
   
    */
    GLuint program = createProgram("./shaders/vs.glsl", "./shaders/fsGreen.glsl");
    glUseProgram(program);

    GLuint loc = glGetUniformLocation(program, "pTransform");
    GLuint loc2 = glGetUniformLocation(program, "proj");

    GLuint program2 = createProgram("./shaders/vs.glsl", "./shaders/fsRed.glsl");
    vec3 pos = glm::vec3(-0.0, -0.0f, -8.0);
    mat4 mMat = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0f, 0));
    mat4 vMat = glm::lookAt(pos, pos+glm::vec3(0.0, -1.0f, 0.0),vec3(0,1,0));
    mat4 pMat = glm::perspective(1.0472f, window.screenRatio(), 0.1f, 1000.0f); // 1.0472 radians = 60 degrees
    mat4 mvMat;
    mat4 rMat;
    
    glUniformMatrix4fv(loc2, 1, GL_FALSE, value_ptr(pMat));

    float freq = 4;

    
    
    while (!window.closed())
    {
        window.refresh(0, 0, 0, 1);
        
        rMat= glm::rotate(glm::mat4(1.0f), freq*1.75f*(float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        mMat = glm::translate(glm::mat4(1.0), glm::vec3(glm::sin(glfwGetTime()*freq)*2, cos(glfwGetTime()*freq)*2,0.0));
        mvMat =  vMat* mMat * rMat;
       
        glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(mvMat));
       
        glDrawArrays(GL_TRIANGLES, 0,3);
        
        window.update();
    }
    std::cout << glGetString(GL_VERSION) << std::endl;
    return 0;
}