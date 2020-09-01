CC = clang++
DEP = ./vendor/imgui_draw.cpp ./vendor/imgui_impl_glfw.cpp ./vendor/imgui_impl_opengl3.cpp ./vendor/imgui_widgets.cpp ./vendor/imgui.cpp
LIB = -lglew -lglfw
FWORK = -framework OpenGl
MAIN = main.cpp
ALIAS = -o dio -std=c++11

exe:
	${CC} ${DEP} ${MAIN} ${LIB} ${FWORK} ${ALIAS}