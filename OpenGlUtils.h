#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define dBug(x) x; printf("f[ %s ], ERROR: %i, LINE: %i\n",#x,glGetError(),__LINE__);



//#include <glm/glm.hpp>




/////////////////FUNCTION SIGNATURES
static GLuint createProgram(const std::string& vsPath, const std::string& fsPath);
static std::string getShaderSource(const std::string& path);
static GLuint compileShader(const std::string& source,GLuint type);

/////////////////

//////////CLASSESS
class Window{
private:
    static void handleKeys(GLFWwindow* _window,int key,int code,int action,int mode);
    static void handleMouse(GLFWwindow* _window,double posX,double posY);
    bool keys[1024] = {0};
    GLFWwindow* window;
    GLfloat lastX;
    GLfloat lastY;
    GLfloat changeX;
    GLfloat changeY;
    bool mouseFirstMoved = true;

    int height;
    int width;

public:
    Window(int width, int height, const std::string& title);
    ~Window();
    bool closed();
    void refresh(const float r,const float g, const float b,const float a);
    float screenRatio(){
        int width;
        int height;
        glfwGetFramebufferSize(window,&width,&height);
        return (float)width/(float)height;
    }

    int getWidth(){
        
        return width;
    }
    int getHeight(){
        
        return height;
    }

    void update();
    GLFWwindow* data(){ return window; }
    bool* getKeys(){ return keys; }
    GLfloat getChangeX(){ return changeX; }
    GLfloat getChangeY(){ return changeY; }


};

void Window::handleMouse(GLFWwindow* _window,double posX,double posY){
    Window* window =(Window*) glfwGetWindowUserPointer(_window);

    //std::cout<<"mouse position x: "+posX<<std::endl<<"mouse position y: "+posY<<std::endl;
    //printf("mouse position x:%f\nmouse position y:%f\n\n",posX,posY);

    if(window->mouseFirstMoved){
        window->lastX = posX;
        window->lastY = posY;
        window->mouseFirstMoved = false;
    }

    window->changeX = posX - window->lastX ;
    window->changeY = window->lastY - posY;

    window->lastX = posX;
    window->lastY = posY; 
}

void Window::handleKeys(GLFWwindow* _window,int key,int code,int action,int mode){
    Window* window =(Window*) glfwGetWindowUserPointer(_window);
    
    if(key==GLFW_KEY_ESCAPE && action==GLFW_PRESS){
        glfwSetWindowShouldClose(_window,GL_TRUE);
    }

    if(key >= 0 && key <= 1024){
        if(action == GLFW_PRESS){
            window->keys[key]=true;
            
        }
        else if(action == GLFW_RELEASE){
            (*window).keys[key]=false;
            
        }
    }
    
}

Window::Window(int width, int height, const std::string& title){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    this->height = height;
    this->width = width;

    this->window = glfwCreateWindow(width,height,title.c_str(),NULL,NULL);
    glfwMakeContextCurrent(this->window);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(this->window,this);

    glfwSetKeyCallback(this->window,handleKeys);
    glfwSetCursorPosCallback(this->window,handleMouse);
    

}

Window::~Window(){
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

bool Window::closed(){
    return glfwWindowShouldClose(this->window);
}

void Window::refresh(const float r,const float g, const float b,const float a){
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::update(){
    glfwSwapBuffers(this->window);
    glfwPollEvents();
}


class Axis{
private:
    GLuint vbo;
    GLuint vao;
    float points[4];
    GLuint program;
    
public:
   
    Axis(const float& x1,const float& x2,const float& y1,const float& y2){
        
        points[0]=x1;
        
        points[1]=x2;
        
        points[2]=y1;
        
        points[3]=y2;   
        
        
        glGenBuffers(1,&vbo);
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferData(GL_ARRAY_BUFFER,2*2*sizeof(float),points,GL_STATIC_DRAW);
        
        glGenVertexArrays(1,&vao);
        glBindVertexArray(vao);
        glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);
        glEnableVertexAttribArray(0);
        
        glBindBuffer(GL_ARRAY_BUFFER,0);
        
        glBindVertexArray(0);
        
        
    }   
    ~Axis(){

        glDeleteBuffers(1,&vbo); 
        glDeleteVertexArrays(1,&vao);
        
    }

    void draw(){
        
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);
        glUseProgram(program);
        glDrawArrays(GL_LINES,0,2);
        
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
        glDisableVertexAttribArray(0);
        
    }

    void setProgram(const std::string& vertexShader, const std::string& fragmentShader){
        this->program = createProgram(vertexShader,fragmentShader);        
    }

    void enableProgram(){
        glUseProgram(this->program);
    }
};

class SampleTriangle{
private:
    GLuint vbo;
    GLuint vao;
    float* points;
    int size;
    GLuint program;
public:
    SampleTriangle(float* vertices,int size,int dimension){
        this->size = size;
        
        points = (float*) malloc(size*dimension*sizeof(float));
        /*
        for(int i = 0 ;i<size*dimension;i++){
            points[i]=vertices[i];
        }
        */
        memcpy(points,vertices,size*dimension*sizeof(float));

        glGenBuffers(1,&vbo);
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferData(GL_ARRAY_BUFFER,size*dimension*sizeof(float),points,GL_STATIC_DRAW);

        glGenVertexArrays(1,&vao);
        glBindVertexArray(vao);

        glVertexAttribPointer(0,dimension,GL_FLOAT,GL_FALSE,0,0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
        
    }
    void setProgram(const std::string& vs,const std::string& fs){
        program = createProgram(vs,fs);
    }

    void performOperation(std::function<void(SampleTriangle*)> pointer){
        pointer(this);
    }

    GLuint getProgram(){
        return program;
    }
    
    void draw(){

        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);
        glUseProgram(program);

        glDrawArrays(GL_TRIANGLES,0,size);
        /*
        glBindVertexArray(0);
        glDisableVertexAttribArray(0);
        glUseProgram(0);
        */
    }

    ~SampleTriangle(){
        glDeleteBuffers(1,&vbo);
        glDeleteVertexArrays(1,&vao);
        free(points);
    }
};

class Texture{
private:
    std::string location;
    int height,width,depth;
    GLuint tId;


public:
    Texture(const std::string& location):location(location){
        loadTexture(location);

    }

    void loadTexture(const std::string& location){
        unsigned char* txt = stbi_load(location.c_str(),&width,&height,&depth,0);
        if(!txt){
            throw "no such texture was found.";
        }
        stbi_set_flip_vertically_on_load(1);
       
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1,&tId);
        glBindTexture(GL_TEXTURE_2D,tId);
        
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,txt);
        glGenerateTextureMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,0);

        stbi_image_free(txt);
        
    }
    void useTexture();
    void clearTexture();
    ~Texture(){
        glDeleteTextures(1,&tId);
    }
};

//////////SHADERS

static std::string getShaderSource(const std::string& path){
    std::ifstream file(path);
    if(file.is_open() == false){
        throw ("File not found exception: The path: "+path+" could not be found.").c_str();
    }

    std::string result;
    std::string line;

    while(getline(file,line)){
        line.append("\n");
        result.append(line);
    }

    return result;
}

static GLuint compileShader(const std::string& source,GLuint type)
{
    GLuint id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id,1,&src,nullptr);
    glCompileShader(id);

    int debug;
    glGetShaderiv(id,GL_COMPILE_STATUS,&debug);

    if(debug == GL_FALSE){

        int length;
        glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length);
        char message[length];
        glGetShaderInfoLog(id,length,&length,message);
        fprintf(stderr,"Something went wrong in the shader of type %i: %s",type,message);
        glDeleteShader(id);
    }

    return id;
}

static GLuint createProgram(const std::string& vsPath, const std::string& fsPath){
    //std::cout<<vsPath<<std::endl<<fsPath<<std::endl;;
    GLuint program = glCreateProgram();

    std::string vsS,fsS;
    try{
        vsS = getShaderSource(vsPath);
        fsS = getShaderSource(fsPath);
    }   
    catch(const char& e){
        std::cout<<e<<std::endl;
        exit(1);
    }

    GLuint vs = compileShader(vsS,GL_VERTEX_SHADER);
    //checkError(); 
    GLuint fs = compileShader(fsS,GL_FRAGMENT_SHADER);
    //checkError(); 

    glAttachShader(program,vs);
    glAttachShader(program,fs);
    glLinkProgram(program);
    glDetachShader(program,vs);
    glDetachShader(program,fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glValidateProgram(program);

    return program;
}
//////////
