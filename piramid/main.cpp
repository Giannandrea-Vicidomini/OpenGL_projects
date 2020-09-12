#include <OpenGlUtils.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main(void){

    Window w(800,600,"APP");
    glewInit();
    glewExperimental = true;
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //glEnable(GL_CULL_FACE);

    float vertices[] = {
        0,0.9,0,1,
        0.5,0,0.5,1,
        -0.5,0,0.5,1,
        -0.5,0,-0.5,1,
        0.5,0,-0.5,1,
    };

    GLushort indices[]={
        0,2,1,
        0,1,4,
        0,4,3,
        0,3,2,
        2,1,4,
        2,4,3,
    };

    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);

    GLuint ibo;
    glGenBuffers(1,&ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    GLuint program = createProgram("./shaders/vs.glsl","./shaders/fs.glsl");
    glUseProgram(program);

    GLuint loc = glGetUniformLocation(program,"projMatrix");
    GLuint loc2 = glGetUniformLocation(program,"viewMatrix");

    glm::mat4 pMat;
    glm::mat4 vMat;
    glm::mat4 mMat;
    glm::mat4 rMat;
    glm::mat4 mv;
    glm::mat4 cameraRotMat;
    
    float wave;
    float rotation;
    while(!w.closed()){

        w.refresh(0,0,0,1);
        wave = sin(glfwGetTime()*3)*1.5;
        rotation = (float)glfwGetTime()*2.0f;
        //rotation = sin(glfwGetTime())*0.5;

        pMat = glm::perspective(1.0472f,w.screenRatio(),0.100f,1000.0f);
        vMat = glm::translate(glm::mat4(1.0),glm::vec3(-0,-0,-4));
        mMat = glm::translate(glm::mat4(1.0),glm::vec3(0,wave,0));
        rMat = glm::rotate(glm::mat4(1.0),rotation,glm::vec3(0,1,0));
        //cameraRotMat = glm::rotate(glm::mat4(1.0),(float)sin(glfwGetTime())*0.4f,glm::vec3(1,0,0));
        //vMat = vMat*cameraRotMat;
        mv = vMat*rMat*mMat;

        glUniformMatrix4fv(loc,1,GL_FALSE,value_ptr(pMat));
        glUniformMatrix4fv(loc2,1,GL_FALSE,value_ptr(mv));

    
        
        glDrawElements(GL_TRIANGLES,3*6,GL_UNSIGNED_SHORT,nullptr);

        w.update();
    }

    std::cout<<glGetString(GL_VERSION)<<std::endl;

    return 0;
}