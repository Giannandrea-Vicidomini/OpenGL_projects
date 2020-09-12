#include <OpenGlUtils.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

int main(void){
    Window w(800,600,"App");
    glewInit();
    glewExperimental = true;

    
    float offsettington[]={
        0.0,0.5,0.0,
        0.5,-0.5,0.0,
        -0.5,-0.5,0.0,
    };
    
    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,3*3*sizeof(float),offsettington,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);

    GLuint program = createProgram("./shaders/vertex.glsl","./shaders/fragment.glsl");
    glUseProgram(program);

    GLuint projLoc=glGetUniformLocation(program,"proj");
    GLuint viewLoc=glGetUniformLocation(program,"viewMatrix");

    glm::mat4 proj;
    glm::mat4 view;
    glm::mat4 model;
    glm::mat4 mv;
    glm::mat4 rot;

    float wave;
    while (!w.closed())
    {
        
        w.refresh(0,0,0,0);
        //glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,nullptr);
        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        wave = abs(8*sin(glfwGetTime()*2));
        proj = glm::perspective(1.0437f,w.screenRatio(),0.10f,1000.0f);
        view = glm::translate(glm::mat4(1.0),glm::vec3(0.0,0.0,-4));
        model = glm::translate(glm::mat4(1.0),glm::vec3(0.0,sin(glfwGetTime()*4)*0.5,0.0));
        rot = glm::rotate(glm::mat4(1.0),(float)-glfwGetTime()*3,glm::vec3(0,1.0,0));
        mv=view*model*rot;
        

        glUniformMatrix4fv(projLoc,1,GL_FALSE,value_ptr(proj));
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,value_ptr(mv));

        glDrawArrays(GL_TRIANGLES,0,3);

        w.update();
        
    }

    return 0;
}