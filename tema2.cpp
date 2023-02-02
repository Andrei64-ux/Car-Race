#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/tema2/transform3D.h"

using namespace std;
using namespace m1;

vector<VertexFormat> vertices_for_circuit_outer;
vector<VertexFormat> vertices_for_circuit;
vector<VertexFormat> vertices_for_car1;
vector<VertexFormat> vertices_for_car2;
vector<VertexFormat> vertices_for_car3;
vector<VertexFormat> vertices_for_tree;
vector<VertexFormat> vertices_for_tree_inside;
vector<VertexFormat> vertices_for_branch;
vector<VertexFormat> vertices_for_branch_inside;

glm::vec3 color;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
    color = glm::vec3(0, 0, 0);
    polygonMode = GL_FILL;
    camera = new implemented::Camera();
    camera->Set(glm::vec3(-20, 2, 11), glm::vec3(0, -20, 0), glm::vec3(0, 1, 0));

    // Initialize tx, ty and tz (the translation steps)
    translateX = camera->GetTargetPosition().x;
    translateY = camera->GetTargetPosition().y;
    translateZ = camera->GetTargetPosition().z;
    rotate_angle = 0;

    // Initialize sx, sy and sz (the scale factors)
    sizeSpeed = 0.5;
    centerX = 0;
    centerY = 0;
    centerZ = 0;

    UP = glm::vec3(0, 1, 0);

    // Sets the resolution of the small viewport
    moveX = 0; moveY = 0;
    zoom = 1;
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 6.f, resolution.y / 3.5f);

    right = 20.f;
    left = 10.f;
    bottom = .01f;
    top = 20.f;

    projectionMatrix = glm::perspective(RADIANS(90), window->props.aspectRatio, Z_NEAR, Z_FAR);

    change_direction1 = 0;
    change_direction2 = 0;
    change_direction3 = 0;
    change_direction4 = 0;

    change_direction11 = 0;
    change_direction21 = 0;
    change_direction31 = 0;
    change_direction41 = 0;

    change_direction12 = 0;
    change_direction22 = 0;
    change_direction32 = 0;
    change_direction42 = 0;

    rotate_car1 = 0;
    rotate_car2 = 0;
    rotate_car3 = 0;

    car_radius = 0.5;

    // generate tree
    {
        vector<VertexFormat> tree_vertices
        {
           VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.59,0.29,0.00)),
            VertexFormat(glm::vec3(0.5, 0, 0), glm::vec3(0.59,0.29,0.00)),
            VertexFormat(glm::vec3(0.5, 1, 0), glm::vec3(0.59,0.29,0.00)),
            VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0.59,0.29,0.00)),
            VertexFormat(glm::vec3(0, 0, 0.4), glm::vec3(0.59,0.29,0.00)),
            VertexFormat(glm::vec3(0.5, 0, 0.4), glm::vec3(0.59,0.29,0.00)),
            VertexFormat(glm::vec3(0.5, 1, 0.4), glm::vec3(0.59,0.29,0.00)),
            VertexFormat(glm::vec3(0, 1, 0.4), glm::vec3(0.59,0.29,0.00))
        };

        vector<unsigned int> indices_tree =
        {
            0, 5, 1,
            0, 4, 5,
            0, 7, 4,
            0, 3, 7,
            3, 6, 7,
            3, 2, 6,
            2, 5, 6,
            2, 1, 5,
            0, 1, 2,
            0, 2, 3,
            4, 6, 5,
            4, 7, 6
        };

        meshes["tree"] = new Mesh("generated tree");
        meshes["tree"]->InitFromData(tree_vertices, indices_tree);
    }

    // generate branch
    {
        vector<VertexFormat> branch_vertices
        {
           VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.05,0.30,0.00)),
            VertexFormat(glm::vec3(1.2, 0, 0), glm::vec3(0.05,0.30,0.00)),
            VertexFormat(glm::vec3(1.2, 1.2, 0), glm::vec3(0.05,0.30,0.00)),
            VertexFormat(glm::vec3(0, 1.2, 0), glm::vec3(0.05,0.30,0.00)),
            VertexFormat(glm::vec3(0, 0, 1.2), glm::vec3(0.05,0.30,0.00)),
            VertexFormat(glm::vec3(1.2, 0, 1.2), glm::vec3(0.05,0.30,0.00)),
            VertexFormat(glm::vec3(1.2, 1.2, 1.2), glm::vec3(0.05,0.30,0.00)),
            VertexFormat(glm::vec3(0, 1.2, 1.2), glm::vec3(0.05,0.30,0.00))
        };

        vector<unsigned int> indices_branch =
        {
            0, 5, 1,    3, 6, 7,    0, 1, 2,
            0, 4, 5,	3, 2, 6,    0, 2, 3,
            0, 7, 4,	2, 5, 6,    4, 6, 5,
            0, 3, 7,    2, 1, 5,    4, 7, 6
        };

        meshes["branch"] = new Mesh("generated branch");
        meshes["branch"]->InitFromData(branch_vertices, indices_branch);
    }

    // generate grass
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-200, 0, -200), glm::vec3(0.2, 1, 0.2)),
            VertexFormat(glm::vec3(200, 0, -200), glm::vec3(0.2, 1, 0.2)),
            VertexFormat(glm::vec3(-200, 0, 200), glm::vec3(0.2, 1, 0.2)),
            VertexFormat(glm::vec3(200, 0, 200), glm::vec3(0.2, 1, 0.2)),
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,
            1, 2, 3
        };

        meshes["grass"] = new Mesh("generated grass");
        meshes["grass"]->InitFromData(vertices, indices);
    }

    // generate circuit
    {
        vector<VertexFormat> vertices_for_circuit
        {
            VertexFormat(glm::vec3(48.6, 0, 2.9), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(42.1, 0, 14), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(27.9, 0, 21.4), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(15, 0, 19.5), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(5.9, 0, 12.5), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(-4.8, 0, 6), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(-17.6, 0, 11.5), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(-28.3, 0, 14.3), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(-37.9, 0, 8.1), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(-39.3, 0, -1.9), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(-34.5, 0, -10.5), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(-21.1, 0, -15.5), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(-6.8, 0, -16.2), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(4.4, 0, -9.8), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(13, 0, -2.5), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(25.8, 0, -7), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(34.6, 0, -14), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(46, 0, -12.3), glm::vec3(0, 0, 0))
        };

        for (int i = 0; i < vertices_for_circuit.size(); i++) {
            glm::vec3 D;
            if (i == vertices_for_circuit.size() - 1) {
                D = glm::vec3(vertices_for_circuit[vertices_for_circuit.size() - 1].position.x - vertices_for_circuit[0].position.x, 0,
                    vertices_for_circuit[vertices_for_circuit.size() - 1].position.z - vertices_for_circuit[0].position.z);
                glm::vec3 P;
                P = cross(D, UP);
                glm::vec3 R;

                // circut and car1
                R.x = vertices_for_circuit[i].position.x + 0.2 * P.x;
                R.y = 0.1;
                R.z = vertices_for_circuit[i].position.z + 0.2 * P.z;
                vertices_for_circuit_outer.push_back(R);

                R.x = vertices_for_circuit[i].position.x + 0.1 * P.x;
                R.y = 0.5;
                R.z = vertices_for_circuit[i].position.z + 0.1 * P.z;
                vertices_for_car1.push_back(R);

                // tree
                R.x = vertices_for_circuit[i].position.x + 0.35 * P.x;
                R.y = 0.5;
                R.z = vertices_for_circuit[i].position.z + 0.35 * P.z;
                vertices_for_tree.push_back(R);

                // branch
                R.x = vertices_for_circuit[i].position.x + 0.35 * P.x - 0.3;
                R.y = 0.5 + 1;
                R.z = vertices_for_circuit[i].position.z + 0.35 * P.z - 0.5;
                vertices_for_branch.push_back(R);

                // circuit inside and car2
                glm::vec3 A;
                A.x = vertices_for_circuit[i].position.x - 0.2 * P.x;
                A.y = 0.1;
                A.z = vertices_for_circuit[i].position.z - 0.2 * P.z;
                vertices_for_circuit_outer.push_back(A);

                A.x = vertices_for_circuit[i].position.x - 0.2 * P.x;
                A.y = 0.5;
                A.z = vertices_for_circuit[i].position.z - 0.2 * P.z;
                vertices_for_car2.push_back(A);

                A.x = vertices_for_circuit[i].position.x - 0.05 * P.x;
                A.y = 0.5;
                A.z = vertices_for_circuit[i].position.z - 0.05 * P.z;
                vertices_for_car3.push_back(A);

                // tree inside
                A.x = vertices_for_circuit[i].position.x - 0.2 * P.x;
                A.y = 0.5;
                A.z = vertices_for_circuit[i].position.z - 0.2 * P.z;
                vertices_for_tree_inside.push_back(A);

                // branch inside
                A.x = vertices_for_circuit[i].position.x - 0.2 * P.x - 0.3;
                A.y = 0.5 + 1;
                A.z = vertices_for_circuit[i].position.z - 0.2 * P.z - 0.5;
                vertices_for_branch_inside.push_back(A);
            }
            else {
                D = glm::vec3(vertices_for_circuit[i].position.x - vertices_for_circuit[i + 1].position.x, 0,
                    vertices_for_circuit[i].position.z - vertices_for_circuit[i + 1].position.z);
                glm::vec3 P;
                P = cross(D, UP);

                glm::vec3 R;

                // circuit and car1
                R.x = vertices_for_circuit[i].position.x + 0.2 * P.x;
                R.y = 0.1;
                R.z = vertices_for_circuit[i].position.z + 0.2 * P.z;
                vertices_for_circuit_outer.push_back(R);

                R.x = vertices_for_circuit[i].position.x + 0.05 * P.x;
                R.y = 0.5;
                R.z = vertices_for_circuit[i].position.z + 0.05 * P.z;
                vertices_for_car1.push_back(R);

                // tree
                R.x = vertices_for_circuit[i].position.x + 0.45 * P.x;
                R.y = 0.5;
                R.z = vertices_for_circuit[i].position.z + 0.45 * P.z;
                vertices_for_tree.push_back(R);

                // branch
                R.x = vertices_for_circuit[i].position.x + 0.45 * P.x - 0.3;
                R.y = 0.5 + 1;
                R.z = vertices_for_circuit[i].position.z + 0.45 * P.z - 0.5;
                vertices_for_branch.push_back(R);

                // circuit inside and car2
                glm::vec3 A;
                A.x = vertices_for_circuit[i].position.x - 0.2 * P.x;
                A.y = 0.1;
                A.z = vertices_for_circuit[i].position.z - 0.2 * P.z;
                vertices_for_circuit_outer.push_back(A);

                A.x = vertices_for_circuit[i].position.x - 0.07 * P.x;
                A.y = 0.5;
                A.z = vertices_for_circuit[i].position.z - 0.07 * P.z;
                vertices_for_car2.push_back(A);

                // car3
                A.x = vertices_for_circuit[i].position.x - 0.05 * P.x;
                A.y = 0.5;
                A.z = vertices_for_circuit[i].position.z - 0.05 * P.z;
                vertices_for_car3.push_back(A);

                // tree inside
                A.x = vertices_for_circuit[i].position.x - 0.4 * P.x;
                A.y = 0.5;
                A.z = vertices_for_circuit[i].position.z - 0.4 * P.z;
                vertices_for_tree_inside.push_back(A);

                // branch inside
                A.x = vertices_for_circuit[i].position.x - 0.4 * P.x - 0.3;
                A.y = 0.5 + 1;
                A.z = vertices_for_circuit[i].position.z - 0.4 * P.z - 0.5;
                vertices_for_branch_inside.push_back(A);
            }
        }

        for (int i = 0; i < vertices_for_circuit_outer.size(); i++) {
            vertices_for_circuit_outer[i].color = glm::vec3(0, 0, 0);
        }

        vector<unsigned int> indices_circuit =
        {
            0, 1, 2,    5, 6, 7,     10, 11, 12,    15, 16, 17,     20, 21, 22,     25, 26, 27,     30, 31, 32,
            1, 2, 3,    6, 7, 8,     11, 12, 13,    16, 17, 18,     21, 22, 23,     26, 27, 28,     31, 32, 33,
            2, 3, 4,    7, 8, 9,     12, 13, 14,    17, 18, 19,     22, 23, 24,     27, 28, 29,     32, 33, 34,
            3, 4, 5,    8, 9, 10,    13, 14, 15,    18, 19, 20,     23, 24, 25,     28, 29, 30,     33, 34, 35,
            4, 5, 6,    9, 10, 11,   14, 15, 16,    19, 20, 21,     24, 25, 26,     29, 30, 31,     34, 35, 0,
            35, 0, 1
        };

        meshes["circuit"] = new Mesh("generated circuit");
        meshes["circuit"]->InitFromData(vertices_for_circuit_outer, indices_circuit);
    }
    for (int i = 0; i < vertices_for_car1.size(); i++) {
        cout << vertices_for_car1[i].position << endl;
    }
    // generate my_car
    {
        vector<VertexFormat> vertices_car
        {
            VertexFormat(glm::vec3(0 , 0.11, 0), glm::vec3(0, 0, 1)),
            VertexFormat(glm::vec3(1.4, 0.11, 0), glm::vec3(0, 0, 1)),
            VertexFormat(glm::vec3(1.4, 0.9, 0), glm::vec3(0, 0, 1)),
            VertexFormat(glm::vec3(0, 0.9, 0), glm::vec3(0, 0, 1)),
            VertexFormat(glm::vec3(0, 0.11, 0.9), glm::vec3(0, 0, 1)),
            VertexFormat(glm::vec3(1.4, 0.11, 0.9), glm::vec3(0, 0, 1)),
            VertexFormat(glm::vec3(1.4, 0.9, 0.9), glm::vec3(0, 0, 1)),
            VertexFormat(glm::vec3(0, 0.9, 0.9), glm::vec3(0, 0, 1))
        };

        vector<unsigned int> indices_car =
        {
            0, 5, 1,	3, 6, 7,    0, 1, 2,
            0, 4, 5,	3, 2, 6,    0, 2, 3,
            0, 7, 4,	2, 5, 6,    4, 6, 5,
            0, 3, 7,    2, 1, 5,    4, 7, 6
        };

        meshes["car"] = new Mesh("generated car");
        meshes["car"]->InitFromData(vertices_car, indices_car);
    }

    // generate car object
    {
        vector<VertexFormat> vertices_car1
        {
            VertexFormat(glm::vec3(0 , 0.11, 0), glm::vec3(0.22, 0.32, 1)),
            VertexFormat(glm::vec3(1.4, 0.11, 0), glm::vec3(0.22, 0.32, 1)),
            VertexFormat(glm::vec3(1.4, 0.9, 0), glm::vec3(0.22, 0.32, 1)),
            VertexFormat(glm::vec3(0, 0.9, 0), glm::vec3(0.22, 0.32, 1)),
            VertexFormat(glm::vec3(0, 0.11, 0.9), glm::vec3(0.22, 0.32, 1)),
            VertexFormat(glm::vec3(1.4, 0.11, 0.9), glm::vec3(0.22, 0.32, 1)),
            VertexFormat(glm::vec3(1.4, 0.9, 0.9), glm::vec3(0.22, 0.32, 1)),
            VertexFormat(glm::vec3(0, 0.9, 0.9), glm::vec3(0.22, 0.32, 1))
        };

        vector<unsigned int> indices_car =
        {
            0, 5, 1,	3, 6, 7,    0, 1, 2,
            0, 4, 5,	3, 2, 6,    0, 2, 3,
            0, 7, 4,	2, 5, 6,    4, 6, 5,
            0, 3, 7,    2, 1, 5,    4, 7, 6
        };

        meshes["car1"] = new Mesh("generated car1");
        meshes["car1"]->InitFromData(vertices_car1, indices_car);
    }

    {
        vector<VertexFormat> vertices_car2
        {
            VertexFormat(glm::vec3(0 , 0.11, 0), glm::vec3(0.58, 0.94, 0.32)),
            VertexFormat(glm::vec3(1.4, 0.11, 0), glm::vec3(0.58, 0.94, 0.32)),
            VertexFormat(glm::vec3(1.4, 0.9, 0), glm::vec3(0.58, 0.94, 0.32)),
            VertexFormat(glm::vec3(0, 0.9, 0), glm::vec3(0.58, 0.94, 0.32)),
            VertexFormat(glm::vec3(0, 0.11, 0.9), glm::vec3(0.58, 0.94, 0.32)),
            VertexFormat(glm::vec3(1.4, 0.11, 0.9), glm::vec3(0.58, 0.94, 0.32)),
            VertexFormat(glm::vec3(1.4, 0.9, 0.9), glm::vec3(0.58, 0.94, 0.32)),
            VertexFormat(glm::vec3(0, 0.9, 0.9), glm::vec3(0.58, 0.94, 0.32))
        };

        vector<unsigned int> indices_car =
        {
            0, 5, 1,	3, 6, 7,    0, 1, 2,
            0, 4, 5,	3, 2, 6,    0, 2, 3,
            0, 7, 4,	2, 5, 6,    4, 6, 5,
            0, 3, 7,    2, 1, 5,    4, 7, 6
        };

        meshes["car2"] = new Mesh("generated car1");
        meshes["car2"]->InitFromData(vertices_car2, indices_car);
    }

    {
        vector<VertexFormat> vertices_car3
        {
            VertexFormat(glm::vec3(0 , 0.11, 0), glm::vec3(0.80, 0.20, 0.50)),
            VertexFormat(glm::vec3(1.4, 0.11, 0), glm::vec3(0.80, 0.20, 0.50)),
            VertexFormat(glm::vec3(1.4, 0.9, 0), glm::vec3(0.80, 0.20, 0.50)),
            VertexFormat(glm::vec3(0, 0.9, 0), glm::vec3(0.80, 0.20, 0.50)),
            VertexFormat(glm::vec3(0, 0.11, 0.9), glm::vec3(0.80, 0.20, 0.50)),
            VertexFormat(glm::vec3(1.4, 0.11, 0.9), glm::vec3(0.80, 0.20, 0.50)),
            VertexFormat(glm::vec3(1.4, 0.9, 0.9), glm::vec3(0.80, 0.20, 0.50)),
            VertexFormat(glm::vec3(0, 0.9, 0.9), glm::vec3(0.80, 0.20, 0.50))
        };

        vector<unsigned int> indices_car =
        {
            0, 5, 1,	3, 6, 7,    0, 1, 2,
            0, 4, 5,	3, 2, 6,    0, 2, 3,
            0, 7, 4,	2, 5, 6,    4, 6, 5,
            0, 3, 7,    2, 1, 5,    4, 7, 6
        };

        meshes["car3"] = new Mesh("generated car3");
        meshes["car3"]->InitFromData(vertices_car3, indices_car);
    }

    obj1X = 0;  obj1Y = 0;  obj1Z = 0;
    obj2X = 0;  obj2Y = 0;  obj2Z = 0;
    obj3X = 0;  obj3Y = 0;  obj3Z = 0;

    index = 0;
    index1 = 0;
    index2 = 0;
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::DrawTrees() {
    for (int i = 0; i < vertices_for_tree.size(); i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(vertices_for_tree[i].position.x, vertices_for_tree[i].position.y, vertices_for_tree[i].position.z);
        RenderMesh(meshes["tree"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(vertices_for_branch[i].position.x, vertices_for_branch[i].position.y, vertices_for_branch[i].position.z);
        RenderMesh(meshes["branch"], shaders["VertexColor"], modelMatrix);
    }

    for (int i = 0; i < vertices_for_tree_inside.size(); i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(vertices_for_tree_inside[i].position.x, vertices_for_tree_inside[i].position.y, vertices_for_tree_inside[i].position.z);
        RenderMesh(meshes["tree"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(vertices_for_branch_inside[i].position.x, vertices_for_branch_inside[i].position.y, vertices_for_branch_inside[i].position.z);
        RenderMesh(meshes["branch"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema2::RenderScene(float deltaTimeSeconds) {
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(1.5f, 0.5f, 0.f);
    RenderMesh(meshes["grass"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(1.5f, 0.5f, 0.f);
    RenderMesh(meshes["circuit"], shaders["VertexColor"],modelMatrix);

    DrawTrees();

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(obj1X, 0.5, obj1Z);
    modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    modelMatrix *= transform3D::Translate(0.7, 0.4, 0.45);
    modelMatrix *= transform3D::RotateOY(rotate_car1);
    modelMatrix *= transform3D::Translate(-0.7, -0.4, -0.45);
    RenderMesh(meshes["car1"], shaders["VertexColor"], modelMatrix);

    // move obstacle 1
    if (index == vertices_for_car1.size() - 1) {
        if (vertices_for_car1[index].position.x < vertices_for_car1[0].position.x && vertices_for_car1[index].position.z < vertices_for_car1[0].position.z) {
            obj1X += (vertices_for_car1[0].position.x - vertices_for_car1[index].position.x) * deltaTimeSeconds / 3;
            obj1Z += (vertices_for_car1[0].position.z - vertices_for_car1[index].position.z) * deltaTimeSeconds / 3;
            if (obj1X >= vertices_for_car1[0].position.x || (obj1Z >= vertices_for_car1[0].position.z)) {
                obj1X = vertices_for_car1[0].position.x;
                obj1Z = vertices_for_car1[0].position.z;
                index = 0;
            }
            change_direction1 = 1;
            change_direction2 = 0;
            change_direction3 = 0;
            change_direction4 = 0;
        }
        else if (vertices_for_car1[index].position.x > vertices_for_car1[0].position.x && vertices_for_car1[index].position.z < vertices_for_car1[0].position.z) {
            obj1X -= (vertices_for_car1[index].position.x - vertices_for_car1[0].position.x) * deltaTimeSeconds / 3;
            obj1Z += (vertices_for_car1[0].position.z - vertices_for_car1[index].position.z) * deltaTimeSeconds / 3;
            if (obj1X <= vertices_for_car1[0].position.x || (obj1Z >= vertices_for_car1[0].position.z)) {
                obj1X = vertices_for_car1[0].position.x;
                obj1Z = vertices_for_car1[0].position.z;
                index = 0;
            }
            change_direction1 = 0;
            change_direction2 = 1;
            change_direction3 = 0;
            change_direction4 = 0;
        }
        else if (vertices_for_car1[index].position.x < vertices_for_car1[0].position.x && vertices_for_car1[index].position.z > vertices_for_car1[0].position.z) {
            obj1X += (vertices_for_car1[0].position.x - vertices_for_car1[index].position.x) * deltaTimeSeconds / 3;
            obj1Z -= (vertices_for_car1[index].position.z - vertices_for_car1[0].position.z) * deltaTimeSeconds / 3;
            if (obj1X >= vertices_for_car1[0].position.x || (obj1Z <= vertices_for_car1[0].position.z)) {
                obj1X = vertices_for_car1[0].position.x;
                obj1Z = vertices_for_car1[0].position.z;
                index = 0;
            }
            change_direction1 = 0;
            change_direction2 = 0;
            change_direction3 = 1;
            change_direction4 = 0;
        }
        else {
            obj1X -= (vertices_for_car1[index].position.x - vertices_for_car1[0].position.x) * deltaTimeSeconds / 3;
            obj1Z -= (vertices_for_car1[index].position.z - vertices_for_car1[0].position.z) * deltaTimeSeconds / 3;
            if (obj1X <= vertices_for_car1[0].position.x || (obj1Z <= vertices_for_car1[0].position.z)) {
                obj1X = vertices_for_car1[0].position.x;
                obj1Z = vertices_for_car1[0].position.z;
                index = 0;
            }
            change_direction1 = 0;
            change_direction2 = 0;
            change_direction3 = 0;
            change_direction4 = 1;
        }
    }
    if (index != vertices_for_car1.size() - 1) {
        if (vertices_for_car1[index].position.x < vertices_for_car1[index + 1].position.x && vertices_for_car1[index].position.z < vertices_for_car1[index + 1].position.z) {
            obj1X += (vertices_for_car1[index + 1].position.x - vertices_for_car1[index].position.x) * deltaTimeSeconds / 3;
            obj1Z += (vertices_for_car1[index + 1].position.z - vertices_for_car1[index].position.z) * deltaTimeSeconds / 3;
            if (obj1X >= vertices_for_car1[index + 1].position.x || (obj1Z >= vertices_for_car1[index + 1].position.z)) {
                obj1X = vertices_for_car1[index + 1].position.x;
                obj1Z = vertices_for_car1[index + 1].position.z;
                index++;
            }
            change_direction1 = 1;
            change_direction2 = 0;
            change_direction3 = 0;
            change_direction4 = 0;
        }
        else if (vertices_for_car1[index].position.x > vertices_for_car1[index + 1].position.x && vertices_for_car1[index].position.z < vertices_for_car1[index + 1].position.z) {
            obj1X -= (vertices_for_car1[index].position.x - vertices_for_car1[index + 1].position.x) * deltaTimeSeconds / 3;
            obj1Z += (vertices_for_car1[index + 1].position.z - vertices_for_car1[index].position.z) * deltaTimeSeconds / 3;
            if (obj1X <= vertices_for_car1[index + 1].position.x || (obj1Z >= vertices_for_car1[index + 1].position.z)) {
                obj1X = vertices_for_car1[index + 1].position.x;
                obj1Z = vertices_for_car1[index + 1].position.z;
                index++;
            }
            change_direction1 = 0;
            change_direction2 = 1;
            change_direction3 = 0;
            change_direction4 = 0;
        }
        else if (vertices_for_car1[index].position.x < vertices_for_car1[index + 1].position.x && vertices_for_car1[index].position.z > vertices_for_car1[index + 1].position.z) {
            obj1X += (vertices_for_car1[index + 1].position.x - vertices_for_car1[index].position.x) * deltaTimeSeconds / 3;
            obj1Z -= (vertices_for_car1[index].position.z - vertices_for_car1[index + 1].position.z) * deltaTimeSeconds / 3;
            if (obj1X >= vertices_for_car1[index + 1].position.x || (obj1Z <= vertices_for_car1[index + 1].position.z)) {
                obj1X = vertices_for_car1[index + 1].position.x;
                obj1Z = vertices_for_car1[index + 1].position.z;
                index++;
            }
            change_direction1 = 0;
            change_direction2 = 0;
            change_direction3 = 1;
            change_direction4 = 0;
        }
        else {
            obj1X -= (vertices_for_car1[index].position.x - vertices_for_car1[index + 1].position.x) * deltaTimeSeconds / 3;
            obj1Z -= (vertices_for_car1[index].position.z - vertices_for_car1[index + 1].position.z) * deltaTimeSeconds / 3;
            if (obj1X <= vertices_for_car1[index + 1].position.x || (obj1Z <= vertices_for_car1[index + 1].position.z)) {
                obj1X = vertices_for_car1[index + 1].position.x;
                obj1Z = vertices_for_car1[index + 1].position.z;
                index++;
            }
            change_direction1 = 0;
            change_direction2 = 0;
            change_direction3 = 0;
            change_direction4 = 1;
        }
    }

    if (obj1Z >= vertices_for_car1[vertices_for_car1.size() - 1].position.z && obj1Z <= vertices_for_car1[0].position.z &&
        obj1X >= vertices_for_car1[vertices_for_car1.size() - 1].position.x && obj1X <= vertices_for_car1[0].position.x) {
        rotate_car1 = M_PI_2;
    }
    else if (obj1Z <= vertices_for_car1[8].position.z && obj1Z >= vertices_for_car1[9].position.z &&
        obj1X <= vertices_for_car1[8].position.x && obj1X >= vertices_for_car1[9].position.x) {
        rotate_car1 = -M_PI_2;
    }
    else {
        if (rotate_car1 >= 0.3) {
            rotate_car1 = 0.3;
        }
        if (rotate_car1 <= -0.3) {
            rotate_car1 = -0.3;
        }
        if (change_direction1 == 1) {
            rotate_car1 -= 0.05;
        }
        if (change_direction2 == 1) {
            rotate_car1 += 0.05;
        }
        if (change_direction3 == 1) {
            rotate_car1 += 0.05;
        }
        if (change_direction4 == 1) {
            rotate_car1 -= 0.05;
        }
    }

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(obj2X, 0.5, obj2Z);
    modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    modelMatrix *= transform3D::Translate(0.7, 0.4, 0.45);
    modelMatrix *= transform3D::RotateOY(rotate_car2);
    modelMatrix *= transform3D::Translate(-0.7, -0.4, -0.45);
    RenderMesh(meshes["car2"], shaders["VertexColor"], modelMatrix);

    // move obstacle 2
    if (index1 == vertices_for_car2.size() - 1) {
        if (vertices_for_car2[index1].position.x < vertices_for_car2[0].position.x && vertices_for_car2[index1].position.z < vertices_for_car2[0].position.z) {
            obj2X += (vertices_for_car2[0].position.x - vertices_for_car2[index1].position.x) * deltaTimeSeconds;
            obj2Z += (vertices_for_car2[0].position.z - vertices_for_car2[index1].position.z) * deltaTimeSeconds;
            if (obj2X >= vertices_for_car2[0].position.x || (obj2Z >= vertices_for_car2[0].position.z)) {
                obj2X = vertices_for_car2[0].position.x;
                obj2Z = vertices_for_car2[0].position.z;
                index1 = 0;
            }
            change_direction11 = 1;
            change_direction21 = 0;
            change_direction31 = 0;
            change_direction41 = 0;
        }
        else if (vertices_for_car2[index1].position.x > vertices_for_car2[0].position.x && vertices_for_car2[index1].position.z < vertices_for_car2[0].position.z) {
            obj2X -= (vertices_for_car2[index1].position.x - vertices_for_car2[0].position.x) * deltaTimeSeconds;
            obj2Z += (vertices_for_car2[0].position.z - vertices_for_car2[index1].position.z) * deltaTimeSeconds;
            if (obj2X <= vertices_for_car2[0].position.x || (obj2Z >= vertices_for_car2[0].position.z)) {
                obj2X = vertices_for_car2[0].position.x;
                obj2Z = vertices_for_car2[0].position.z;
                index1 = 0;
            }
            change_direction11 = 0;
            change_direction21 = 1;
            change_direction31 = 0;
            change_direction41 = 0;
        }
        else if (vertices_for_car2[index1].position.x < vertices_for_car2[0].position.x && vertices_for_car2[index1].position.z > vertices_for_car2[0].position.z) {
            obj2X += (vertices_for_car2[0].position.x - vertices_for_car2[index1].position.x) * deltaTimeSeconds;
            obj2Z -= (vertices_for_car2[index1].position.z - vertices_for_car2[0].position.z) * deltaTimeSeconds;
            if (obj2X >= vertices_for_car2[0].position.x || (obj2Z <= vertices_for_car2[0].position.z)) {
                obj2X = vertices_for_car2[0].position.x;
                obj2Z = vertices_for_car2[0].position.z;
                index1 = 0;
            }
            change_direction11 = 0;
            change_direction21 = 0;
            change_direction31 = 1;
            change_direction41 = 0;
        }
        else {
            obj2X -= (vertices_for_car2[index1].position.x - vertices_for_car2[0].position.x) * deltaTimeSeconds;
            obj2Z -= (vertices_for_car2[index1].position.z - vertices_for_car2[0].position.z) * deltaTimeSeconds;
            if (obj2X <= vertices_for_car2[0].position.x || (obj2Z <= vertices_for_car2[0].position.z)) {
                obj2X = vertices_for_car2[0].position.x;
                obj2Z = vertices_for_car2[0].position.z;
                index1 = 0;
            }
            change_direction11 = 0;
            change_direction21 = 0;
            change_direction31 = 0;
            change_direction41 = 1;
        }
    }
    if (index1 != vertices_for_car2.size() - 1) {
        if (vertices_for_car2[index1].position.x < vertices_for_car2[index1 + 1].position.x && vertices_for_car2[index1].position.z < vertices_for_car2[index1 + 1].position.z) {
            obj2X += (vertices_for_car2[index1 + 1].position.x - vertices_for_car2[index1].position.x) * deltaTimeSeconds;
            obj2Z += (vertices_for_car2[index1 + 1].position.z - vertices_for_car2[index1].position.z) * deltaTimeSeconds;
            if (obj2X >= vertices_for_car2[index1 + 1].position.x || (obj2Z >= vertices_for_car2[index1 + 1].position.z)) {
                obj2X = vertices_for_car2[index1 + 1].position.x;
                obj2Z = vertices_for_car2[index1 + 1].position.z;
                index1++;
            }
            change_direction11 = 1;
            change_direction21 = 0;
            change_direction31 = 0;
            change_direction41 = 0;
        }
        else if (vertices_for_car2[index1].position.x > vertices_for_car2[index1 + 1].position.x && vertices_for_car2[index1].position.z < vertices_for_car2[index1 + 1].position.z) {
            obj2X -= (vertices_for_car2[index1].position.x - vertices_for_car2[index1 + 1].position.x) * deltaTimeSeconds;
            obj2Z += (vertices_for_car2[index1 + 1].position.z - vertices_for_car2[index1].position.z) * deltaTimeSeconds;
            if (obj2X <= vertices_for_car2[index1 + 1].position.x || (obj2Z >= vertices_for_car2[index1 + 1].position.z)) {
                obj2X = vertices_for_car2[index1 + 1].position.x;
                obj2Z = vertices_for_car2[index1 + 1].position.z;
                index1++;
            }
            change_direction11 = 0;
            change_direction21 = 1;
            change_direction31 = 0;
            change_direction41 = 0;
        }
        else if (vertices_for_car2[index1].position.x < vertices_for_car2[index1 + 1].position.x && vertices_for_car2[index1].position.z > vertices_for_car2[index1 + 1].position.z) {
            obj2X += (vertices_for_car2[index1 + 1].position.x - vertices_for_car2[index1].position.x) * deltaTimeSeconds;
            obj2Z -= (vertices_for_car2[index1].position.z - vertices_for_car2[index1 + 1].position.z) * deltaTimeSeconds;
            if (obj2X >= vertices_for_car2[index1 + 1].position.x || (obj2Z <= vertices_for_car2[index1 + 1].position.z)) {
                obj2X = vertices_for_car2[index1 + 1].position.x;
                obj2Z = vertices_for_car2[index1 + 1].position.z;
                index1++;
            }
            change_direction11 = 0;
            change_direction21 = 0;
            change_direction31 = 1;
            change_direction41 = 0;
        }
        else {
            obj2X -= (vertices_for_car2[index1].position.x - vertices_for_car2[index1 + 1].position.x) * deltaTimeSeconds;
            obj2Z -= (vertices_for_car2[index1].position.z - vertices_for_car2[index1 + 1].position.z) * deltaTimeSeconds;
            if (obj2X <= vertices_for_car2[index1 + 1].position.x || (obj2Z <= vertices_for_car2[index1 + 1].position.z)) {
                obj2X = vertices_for_car2[index1 + 1].position.x;
                obj2Z = vertices_for_car2[index1 + 1].position.z;
                index1++;
            }
            change_direction11 = 0;
            change_direction21 = 0;
            change_direction31 = 0;
            change_direction41 = 1;
        }
    }
    if (obj2Z >= vertices_for_car2[vertices_for_car2.size() - 1].position.z && obj2Z <= vertices_for_car2[0].position.z &&
        obj2X >= vertices_for_car2[vertices_for_car2.size() - 1].position.x && obj2X <= vertices_for_car2[0].position.x) {
        rotate_car2 = M_PI_2;
    }
    else if (obj2Z <= vertices_for_car2[8].position.z && obj2Z >= vertices_for_car2[9].position.z &&
        obj2X <= vertices_for_car2[8].position.x && obj2X >= vertices_for_car2[9].position.x) {
        rotate_car2 = -M_PI_2;
    }
    else {
        if (rotate_car2 >= 0.3) {
            rotate_car2 = 0.3;
        }
        if (rotate_car2 <= -0.3) {
            rotate_car2 = -0.3;
        }
        if (change_direction11 == 1) {
            rotate_car2 -= 0.05;
        }
        if (change_direction21 == 1) {
            rotate_car2 += 0.05;
        }
        if (change_direction31 == 1) {
            rotate_car2 += 0.05;
        }
        if (change_direction41 == 1) {
            rotate_car2 -= 0.05;
        }
    }

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(obj3X, 0.5, obj3Z);
    modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    modelMatrix *= transform3D::Translate(0.7, 0.4, 0.45);
    modelMatrix *= transform3D::RotateOY(rotate_car3);
    modelMatrix *= transform3D::Translate(-0.7, -0.4, -0.45);
    RenderMesh(meshes["car3"], shaders["VertexColor"], modelMatrix);

    // move obstacle 3
    if (index2 == vertices_for_car3.size() - 1) {
        if (vertices_for_car3[index2].position.x < vertices_for_car3[0].position.x && vertices_for_car3[index2].position.z < vertices_for_car3[0].position.z) {
            obj3X += (vertices_for_car3[0].position.x - vertices_for_car3[index2].position.x) * deltaTimeSeconds / 5;
            obj3Z += (vertices_for_car3[0].position.z - vertices_for_car3[index2].position.z) * deltaTimeSeconds / 5;
            if (obj3X >= vertices_for_car3[0].position.x || (obj3Z >= vertices_for_car3[0].position.z)) {
                obj3X = vertices_for_car3[0].position.x;
                obj3Z = vertices_for_car3[0].position.z;
                index2 = 0;
            }
            change_direction12 = 1;
            change_direction22 = 0;
            change_direction32 = 0;
            change_direction42 = 0;
        }
        else if (vertices_for_car3[index2].position.x > vertices_for_car3[0].position.x && vertices_for_car3[index2].position.z < vertices_for_car3[0].position.z) {
            obj3X -= (vertices_for_car3[index2].position.x - vertices_for_car3[0].position.x) * deltaTimeSeconds / 5;
            obj3Z += (vertices_for_car3[0].position.z - vertices_for_car3[index2].position.z) * deltaTimeSeconds / 5;
            if (obj3X <= vertices_for_car3[0].position.x || (obj3Z >= vertices_for_car3[0].position.z)) {
                obj3X = vertices_for_car3[0].position.x;
                obj3Z = vertices_for_car3[0].position.z;
                index2 = 0;
            }
            change_direction12 = 0;
            change_direction22 = 1;
            change_direction32 = 0;
            change_direction42 = 0;
        }
        else if (vertices_for_car3[index2].position.x < vertices_for_car3[0].position.x && vertices_for_car3[index2].position.z > vertices_for_car3[0].position.z) {
            obj3X += (vertices_for_car3[0].position.x - vertices_for_car3[index2].position.x) * deltaTimeSeconds / 5;
            obj3Z -= (vertices_for_car3[index2].position.z - vertices_for_car3[0].position.z) * deltaTimeSeconds / 5;
            if (obj3X >= vertices_for_car3[0].position.x || (obj3Z <= vertices_for_car3[0].position.z)) {
                obj3X = vertices_for_car3[0].position.x;
                obj3Z = vertices_for_car3[0].position.z;
                index2 = 0;
            }
            change_direction12 = 0;
            change_direction22 = 0;
            change_direction32 = 1;
            change_direction42 = 0;
        }
        else {
            obj3X -= (vertices_for_car3[index2].position.x - vertices_for_car3[0].position.x) * deltaTimeSeconds / 5;
            obj3Z -= (vertices_for_car3[index2].position.z - vertices_for_car3[0].position.z) * deltaTimeSeconds / 5;
            if (obj3X <= vertices_for_car3[0].position.x || (obj3Z <= vertices_for_car3[0].position.z)) {
                obj3X = vertices_for_car3[0].position.x;
                obj3Z = vertices_for_car3[0].position.z;
                index2 = 0;
            }
            change_direction12 = 0;
            change_direction22 = 0;
            change_direction32 = 0;
            change_direction42 = 1;
        }
    }
    if (index2 != vertices_for_car3.size() - 1) {
        if (vertices_for_car3[index2].position.x < vertices_for_car3[index2 + 1].position.x && vertices_for_car3[index2].position.z < vertices_for_car3[index2 + 1].position.z) {
            obj3X += (vertices_for_car3[index2 + 1].position.x - vertices_for_car3[index2].position.x) * deltaTimeSeconds / 5;
            obj3Z += (vertices_for_car3[index2 + 1].position.z - vertices_for_car3[index2].position.z) * deltaTimeSeconds / 5;
            if (obj3X >= vertices_for_car3[index2 + 1].position.x || (obj3Z >= vertices_for_car3[index2 + 1].position.z)) {
                obj3X = vertices_for_car3[index2 + 1].position.x;
                obj3Z = vertices_for_car3[index2 + 1].position.z;
                index2++;
            }
            change_direction12 = 1;
            change_direction22 = 0;
            change_direction32 = 0;
            change_direction42 = 0;
        }
        else if (vertices_for_car3[index2].position.x > vertices_for_car3[index2 + 1].position.x && vertices_for_car3[index2].position.z < vertices_for_car3[index2 + 1].position.z) {
            obj3X -= (vertices_for_car3[index2].position.x - vertices_for_car3[index2 + 1].position.x) * deltaTimeSeconds / 5;
            obj3Z += (vertices_for_car3[index2 + 1].position.z - vertices_for_car3[index2].position.z) * deltaTimeSeconds / 5;
            if (obj3X <= vertices_for_car3[index2 + 1].position.x || (obj3Z >= vertices_for_car3[index2 + 1].position.z)) {
                obj3X = vertices_for_car3[index2 + 1].position.x;
                obj3Z = vertices_for_car3[index2 + 1].position.z;
                index2++;
            }
            change_direction12 = 0;
            change_direction22 = 1;
            change_direction32 = 0;
            change_direction42 = 0;
        }
        else if (vertices_for_car3[index2].position.x < vertices_for_car3[index2 + 1].position.x && vertices_for_car3[index2].position.z > vertices_for_car3[index2 + 1].position.z) {
            obj3X += (vertices_for_car3[index2 + 1].position.x - vertices_for_car3[index2].position.x) * deltaTimeSeconds / 5;
            obj3Z -= (vertices_for_car3[index2].position.z - vertices_for_car3[index2 + 1].position.z) * deltaTimeSeconds / 5;
            if (obj3X >= vertices_for_car3[index2 + 1].position.x || (obj3Z <= vertices_for_car3[index2 + 1].position.z)) {
                obj3X = vertices_for_car3[index2 + 1].position.x;
                obj3Z = vertices_for_car3[index2 + 1].position.z;
                index2++;
            }
            change_direction12 = 0;
            change_direction22 = 0;
            change_direction32 = 1;
            change_direction42 = 0;
        }
        else {
            obj3X -= (vertices_for_car3[index2].position.x - vertices_for_car3[index2 + 1].position.x) * deltaTimeSeconds / 5;
            obj3Z -= (vertices_for_car3[index2].position.z - vertices_for_car3[index2 + 1].position.z) * deltaTimeSeconds / 5;
            if (obj3X <= vertices_for_car3[index2 + 1].position.x || (obj3Z <= vertices_for_car3[index2 + 1].position.z)) {
                obj3X = vertices_for_car3[index2 + 1].position.x;
                obj3Z = vertices_for_car3[index2 + 1].position.z;
                index2++;
            }
            change_direction12 = 0;
            change_direction22 = 0;
            change_direction32 = 0;
            change_direction42 = 1;
        }
    }
    if (obj3Z >= vertices_for_car3[vertices_for_car3.size() - 1].position.z && obj3Z <= vertices_for_car3[0].position.z &&
        obj3X >= vertices_for_car3[vertices_for_car3.size() - 1].position.x && obj3X <= vertices_for_car3[0].position.x) {
        rotate_car3 = M_PI_2;
    }
    else if (obj3Z <= vertices_for_car3[8].position.z && obj3Z >= vertices_for_car3[9].position.z &&
        obj3X <= vertices_for_car3[8].position.x && obj3X >= vertices_for_car3[9].position.x) {
        rotate_car3 = -M_PI_2;
    }
    else {
        if (rotate_car3 >= 0.3) {
            rotate_car3 = 0.3;
        }
        if (rotate_car3 <= -0.3) {
            rotate_car3 = -0.3;
        }
        if (change_direction12 == 1) {
            rotate_car3 -= 0.05;
        }
        if (change_direction22 == 1) {
            rotate_car3 += 0.05;
        }
        if (change_direction32 == 1) {
            rotate_car3 += 0.05;
        }
        if (change_direction42 == 1) {
            rotate_car3 -= 0.05;
        }
    }

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    modelMatrix *= transform3D::Translate(0.7, 0.4, 0.45);
    modelMatrix *= transform3D::RotateOY(rotate_angle + 0.3);
    modelMatrix *= transform3D::Translate(-0.7, -0.4, -0.45);
    RenderMesh(meshes["car"], shaders["VertexColor"], modelMatrix);
}

int Tema2::Orient(glm::vec3 A, glm::vec3 B, glm::vec3 C) {
    glm::vec3 AB = glm::vec3(B.x - A.x - 0.5, B.y - A.y, B.z - A.z + 1);
    glm::vec3 AC = glm::vec3(C.x - A.x - 0.5, C.y - A.y, C.z - A.z + 1);
    float cross_prod = AB.y * AC.z - AB.z * AC.y - AB.x * AC.z + AB.z * AC.x + AB.x * AC.y - AB.y * AC.x;
    if (cross_prod > 0) {
        return 1;
    }
    return -1;
}

bool Tema2::check_IN(glm::vec3 P, glm::vec3 A, glm::vec3 B, glm::vec3 C) {
    int turns = Orient(A, B, P) + Orient(B, C, P) + Orient(C, A, P);
    if (abs(turns) == 3) {
        return true;
    }
    return false;
}

bool Tema2::check_car_collision(glm::vec3 my_car, glm::vec3 obstacle) {
    float dx = my_car.x - obstacle.x;
    float dy = my_car.y - obstacle.y;
    float dz = my_car.z - obstacle.z;

    float distance = sqrt(dx * dx + dy * dy + dz * dz);

    if (distance <= 2 * car_radius) {
        return true;
    }
    return false;
}

void Tema2::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
    translateX = camera->GetTargetPosition().x;
    translateY = camera->GetTargetPosition().y - 0.08;
    translateZ = camera->GetTargetPosition().z;

    //translateX += cos(rotate_angle + 1) * 0.2;
    //translateZ += sin(rotate_angle + 1) * 0.2;

    projectionMatrix = glm::perspective(RADIANS(90), window->props.aspectRatio, Z_NEAR , Z_FAR);
    RenderScene(deltaTimeSeconds);
    
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    float scale = 8.f;
    float aspect = static_cast<float>(top - bottom) / static_cast<float>(right - left);
    projectionMatrix = glm::ortho(-aspect * scale, aspect * scale, -scale, scale, Z_NEAR, Z_FAR);
    RenderScene(deltaTimeSeconds);
}

void Tema2::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

bool Tema2::check_grass_collision() {
    if (!check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[0].position, vertices_for_circuit_outer[1].position, vertices_for_circuit_outer[2].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[1].position, vertices_for_circuit_outer[2].position, vertices_for_circuit_outer[3].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[2].position, vertices_for_circuit_outer[3].position, vertices_for_circuit_outer[4].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[3].position, vertices_for_circuit_outer[4].position, vertices_for_circuit_outer[5].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[4].position, vertices_for_circuit_outer[5].position, vertices_for_circuit_outer[6].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[5].position, vertices_for_circuit_outer[6].position, vertices_for_circuit_outer[7].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[6].position, vertices_for_circuit_outer[7].position, vertices_for_circuit_outer[8].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[7].position, vertices_for_circuit_outer[8].position, vertices_for_circuit_outer[9].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[8].position, vertices_for_circuit_outer[9].position, vertices_for_circuit_outer[10].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[9].position, vertices_for_circuit_outer[10].position, vertices_for_circuit_outer[11].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[10].position, vertices_for_circuit_outer[11].position, vertices_for_circuit_outer[12].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[11].position, vertices_for_circuit_outer[12].position, vertices_for_circuit_outer[13].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[12].position, vertices_for_circuit_outer[13].position, vertices_for_circuit_outer[14].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[13].position, vertices_for_circuit_outer[14].position, vertices_for_circuit_outer[15].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[14].position, vertices_for_circuit_outer[15].position, vertices_for_circuit_outer[16].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[15].position, vertices_for_circuit_outer[16].position, vertices_for_circuit_outer[17].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[16].position, vertices_for_circuit_outer[17].position, vertices_for_circuit_outer[18].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[17].position, vertices_for_circuit_outer[18].position, vertices_for_circuit_outer[19].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[18].position, vertices_for_circuit_outer[19].position, vertices_for_circuit_outer[20].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[19].position, vertices_for_circuit_outer[20].position, vertices_for_circuit_outer[21].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[20].position, vertices_for_circuit_outer[21].position, vertices_for_circuit_outer[22].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[21].position, vertices_for_circuit_outer[22].position, vertices_for_circuit_outer[23].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[22].position, vertices_for_circuit_outer[23].position, vertices_for_circuit_outer[24].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[23].position, vertices_for_circuit_outer[24].position, vertices_for_circuit_outer[25].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[24].position, vertices_for_circuit_outer[25].position, vertices_for_circuit_outer[26].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[25].position, vertices_for_circuit_outer[26].position, vertices_for_circuit_outer[27].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[26].position, vertices_for_circuit_outer[27].position, vertices_for_circuit_outer[28].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[27].position, vertices_for_circuit_outer[28].position, vertices_for_circuit_outer[29].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[28].position, vertices_for_circuit_outer[29].position, vertices_for_circuit_outer[30].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[29].position, vertices_for_circuit_outer[30].position, vertices_for_circuit_outer[31].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[30].position, vertices_for_circuit_outer[31].position, vertices_for_circuit_outer[32].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[31].position, vertices_for_circuit_outer[32].position, vertices_for_circuit_outer[33].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[32].position, vertices_for_circuit_outer[33].position, vertices_for_circuit_outer[34].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[33].position, vertices_for_circuit_outer[34].position, vertices_for_circuit_outer[35].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[34].position, vertices_for_circuit_outer[35].position, vertices_for_circuit_outer[0].position) &&
        !check_IN(glm::vec3(translateX, translateY, translateZ), vertices_for_circuit_outer[35].position, vertices_for_circuit_outer[0].position, vertices_for_circuit_outer[1].position)) {
        return true;
    }
    return false;
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{   
    if (window->KeyHold(GLFW_KEY_W))
    {
        camera->MoveForward(deltaTime * 5);
        if ((check_car_collision(glm::vec3(translateX, translateY, translateZ) , glm::vec3(obj1X, obj1Y, obj1Z)) == true) ||
            (check_car_collision(glm::vec3(translateX, translateY, translateZ), glm::vec3(obj2X, obj2Y, obj2Z)) == true) ||
            (check_car_collision(glm::vec3(translateX, translateY, translateZ), glm::vec3(obj3X, obj3Y, obj3Z)) == true)) {
            camera->MoveForward(-deltaTime * 5);
        }
        if (check_grass_collision()) {
            camera->MoveForward(-deltaTime * 5);
        }
    }
    if (window->KeyHold(GLFW_KEY_S))
    {
        camera->MoveForward(-deltaTime * 5);
        if ((check_car_collision(glm::vec3(translateX, translateY, translateZ), glm::vec3(obj1X, obj1Y, obj1Z)) == true) ||
            (check_car_collision(glm::vec3(translateX, translateY, translateZ), glm::vec3(obj2X, obj2Y, obj2Z)) == true) ||
            (check_car_collision(glm::vec3(translateX, translateY, translateZ), glm::vec3(obj3X, obj3Y, obj3Z)) == true)) {
            camera->MoveForward(deltaTime * 5);
        }
    }
    if (window->KeyHold(GLFW_KEY_D))
    {
        camera->RotateThirdPerson_OY(-0.5 * deltaTime);
        rotate_angle -= 0.5 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_A))
    {
        camera->RotateThirdPerson_OY(0.5 * deltaTime);
        rotate_angle += 0.5 * deltaTime;
    }
}

void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
}

void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
