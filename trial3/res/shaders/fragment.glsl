#version 330 core

out vec4 col;
in vec4 outCol;

void main()
{
    col = outCol;
}