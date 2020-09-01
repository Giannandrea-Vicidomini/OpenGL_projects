#include <OpenGlUtils.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::vec4 getInversedPoint(glm::vec4 point,glm::mat4& proj,glm::mat4 view);
using namespace std;
using namespace glm;

int main(void){
    Window w(800,600,"Dio Bestia");
    glewInit();
    glewExperimental = true;

    glLineWidth(40.0f);
    glPointSize(20.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    
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
    

    GLuint projLoc=glGetUniformLocation(program,"proj");
    GLuint viewLoc=glGetUniformLocation(program,"viewMatrix");

    glm::vec3 pos = glm::vec3(0,0,-5);

    float xNormalised = ((float)200/w.getWidth())*2 -1;
    float yNormalised = -(((float)300/w.getHeight())*2 -1);

    glm::vec4 nearP = glm::vec4(xNormalised,yNormalised,-1,1);
    glm::vec4 farP = glm::vec4(xNormalised,yNormalised,1,1);
    //glm::vec4 nearP = glm::vec4(-0.5,0,-1,1);
    //glm::vec4 farP = glm::vec4(-0.5,0,1,1);
    cout<<w.getWidth()<<endl;
    cout<<w.getHeight()<<endl;
    cout<<xNormalised<<endl;
    cout<<yNormalised<<endl;

    //glm::mat4 proj = glm::perspective(1.0437f,w.screenRatio(),0.10f,1000.0f);
    glm::mat4 proj = glm::perspective(1.0437f,(float)w.getWidth()/(float)w.getHeight(),0.10f,1000.0f);
    glm::mat4 view = glm::lookAt(pos,pos+glm::vec3(0.0,0,1),glm::vec3(0,1,1));
    glm::mat4 model;
    glm::mat4 mv;
    glm::mat4 rot;

    glm::vec4 worldNear,worldFar;

    worldNear = getInversedPoint(nearP,proj, view);
    worldFar = getInversedPoint(farP,proj, view);

    auto lol = normalize(worldFar - worldNear);
    cout<<lol.z<<endl;
    cout<<worldNear.z<<endl;
    cout<<worldFar.z<<endl;

    float arr[] = {
        //worldNear.x,worldNear.y,worldNear.z,1,//worldNear.w,
        //worldFar.x,worldFar.y,worldFar.z,1//worldFar.w
        xNormalised,yNormalised,-1,1,
        xNormalised,yNormalised,1,1
        //nearP,
        //farP
    };

    GLuint vbo2;
    glGenBuffers(1,&vbo2);
    glBindBuffer(GL_ARRAY_BUFFER,vbo2);
    glBufferData(GL_ARRAY_BUFFER,2*4*sizeof(float),arr,GL_STATIC_DRAW);

    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(1);

    GLuint program2 = createProgram("./shaders/vs.glsl","./shaders/fs.glsl");

    GLuint dioLoc=glGetUniformLocation(program2,"projectMat");
    GLuint caneLoc=glGetUniformLocation(program2,"viewMat");

    float wave;
  
    while (!w.closed())
    {
        
        w.refresh(0,0,0,0);
        //glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,nullptr);
        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        wave = abs(8*sin(glfwGetTime()*2));
        proj = glm::perspective(1.0437f,w.screenRatio(),0.10f,1000.0f);
        view = glm::lookAt(pos,pos+glm::vec3(0.0,0,1),glm::vec3(0,1,1));
        model = glm::translate(glm::mat4(1.0),glm::vec3(0.0,sin(glfwGetTime()*4)*0.5,-3.0));
        rot = glm::rotate(glm::mat4(1.0),(float)-glfwGetTime()*3,glm::vec3(0,1.0,0));
        mv=view*model*rot;
        
        glUseProgram(program);

        glUniformMatrix4fv(projLoc,1,GL_FALSE,value_ptr(proj));
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,value_ptr(mv));
        
        glDrawArrays(GL_TRIANGLES,0,3);
        glUseProgram(program2);
        glUniformMatrix4fv(dioLoc,1,GL_FALSE,value_ptr(proj));
        glUniformMatrix4fv(caneLoc,1,GL_FALSE,value_ptr(view));
        glDrawArrays(GL_POINTS,0,2);


        w.update();
        
    }

    

    return 0;
}

glm::vec4 getInversedPoint(glm::vec4 point,glm::mat4& proj,glm::mat4 view){

    //glm::mat4 inv = inverse(proj*view);
    point = point*inverse(proj);
    auto v = vec4(point.x/point.w,point.y/point.w,point.z/point.w,1);
    //auto v = vec4(point.x/point.w,point.y/point.w,point.z/point.w,point.w);
    point = v*inverse(view);
    return vec4(point.x,point.y,point.z,point.w);
}