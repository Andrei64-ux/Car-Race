#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/lab_camera.h"

#define Z_FAR		(200.f)
#define Z_NEAR		(.01f)

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Tema2();
        ~Tema2();

        void Init() override;

     private:
        //Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        int Orient(glm::vec3 A, glm::vec3 B, glm::vec3 C);
        bool check_IN(glm::vec3 P, glm::vec3 A, glm::vec3 B, glm::vec3 C);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void DrawTrees();
        void RenderScene(float deltaTimeSeconds);
        bool check_car_collision(glm::vec3 my_car, glm::vec3 obstacle);
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        bool check_grass_collision();

     protected:
        glm::mat4 modelMatrix;
        float translateX, translateY, translateZ;
        GLenum polygonMode;
        ViewportArea miniViewportArea;
        glm::vec3 a;
        glm::vec3 UP;
        float transateSpeed;
        float sizeSpeed;
        float angularSpeed;
        float rotate_angle;
        float moveX, moveY;
        float centerX, centerY, centerZ;
        float zoom;
        float obj1X, obj1Y, obj1Z;
        float obj2X, obj2Y, obj2Z;
        float obj3X, obj3Y, obj3Z;
        int count_tree;
        float car_center;
        implemented::Camera* camera;

        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        bool projectionType;
        int change_direction1, change_direction2, change_direction3, change_direction4;
        int change_direction11, change_direction21, change_direction31, change_direction41;
        int change_direction12, change_direction22, change_direction32, change_direction42;
        float rotate_car1;
        float rotate_car2;
        float rotate_car3;
        int index, index1, index2;
        GLfloat right;
        GLfloat left;
        GLfloat bottom;
        GLfloat top;
        GLfloat fov;
        float car_radius;
        glm::vec3 car_pos;
    };
}   // namespace m1

