#include <OpenGlUtils.h>

using namespace std;

int main(void){

    Window w(800,600,"App");
    glewInit();

    float pos[]={
        0,1,0,
        -1.0,-1.0,0,
        1,-1,0
    };

    float colors[]={
        1,0,0,
        0,1,0,
        0,0,1
    };
    
    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(pos),pos,GL_STATIC_DRAW);

    GLuint col;
    glGenBuffers(1,&col);
    glBindBuffer(GL_ARRAY_BUFFER,col);
    glBufferData(GL_ARRAY_BUFFER,sizeof(colors),colors,GL_STATIC_DRAW);

    
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    glBindBuffer(GL_ARRAY_BUFFER,col);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLuint program = createProgram("./res/shaders/vertex.glsl","./res/shaders/fragment.glsl");
    glUseProgram(program);


    while(!w.closed()){
        w.refresh(0,0,0,1);

        glDrawArrays(GL_TRIANGLES,0,3);

        w.update();
         
    }

    cout<< glGetString(GL_VERSION)<<endl<<__FILE__<<endl;

    return 0;
}