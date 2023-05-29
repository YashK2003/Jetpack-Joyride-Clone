#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <bits/stdc++.h>
#include "./textrender/text.h"
#include "../include/shader.h"

// colors
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
// my files

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// OUR ZAPPERS
float zapp[4000];
unsigned int zappindices[4000];

map<int, float> movingzappers;
set<int> stset;

void myfavzappies()
{
    float st = 0.0f;
    vector<pair<float, float>> vec1, vec2, vec3, vec4;
    for (int i = 0; i < 50; i++)
    {
        if (st >= 30)
        {
            break;
        }

        if (i % 2 == 0)
        {
            // means down zappers
            vec1.push_back({st + 0.15, 0.0f});
            vec2.push_back({st + 0.15, -1.0f});
            vec3.push_back({st - 0.15, -1.0f});
            vec4.push_back({st - 0.15, 0.0f});
            st += 1.0f;
        }
        else
        {
            // means up zappers
            vec1.push_back({st + 0.15, 1.0f});
            vec2.push_back({st + 0.15, 0.0f});
            vec3.push_back({st - 0.15, 0.0f});
            vec4.push_back({st - 0.15, 1.0f});
            st += 1.0f;
        }
        // if(i % 5 == 0)st -=0.5f;
    }

    float zappies[40000];
    int x = 0;
    for (int i = 0; i < vec1.size(); ++i)
    {
        zappies[x] = vec1[i].first;
        x++;
        zappies[x] = vec1[i].second;
        x++;
        zappies[x] = 0.0f;
        x++;
        // color coords
        zappies[x] = 1.0f;
        x++;
        zappies[x] = 0.0f;
        x++;
        zappies[x] = 0.0f;
        x++;
        // texture coodrs
        zappies[x] = 1.0f;
        x++;
        zappies[x] = 1.0f;
        x++;

        zappies[x] = vec2[i].first;
        x++;
        zappies[x] = vec2[i].second;
        x++;
        zappies[x] = 0.0f;
        x++;
        // color coords
        zappies[x] = 1.0f;
        x++;
        zappies[x] = 0.0f;
        x++;
        zappies[x] = 0.0f;
        x++;
        // texture coodrs
        zappies[x] = 1.0f;
        x++;
        zappies[x] = 0.0f;
        x++;

        zappies[x] = vec3[i].first;
        x++;
        zappies[x] = vec3[i].second;
        x++;
        zappies[x] = 0.0f;
        x++;
        // color coords
        zappies[x] = 1.0f;
        x++;
        zappies[x] = 0.0f;
        x++;
        zappies[x] = 0.0f;
        x++;
        // texture coodrs
        zappies[x] = 0.0f;
        x++;
        zappies[x] = 0.0f;
        x++;

        zappies[x] = vec4[i].first;
        x++;
        zappies[x] = vec4[i].second;
        x++;
        zappies[x] = 0.0f;
        x++;
        // color coords
        zappies[x] = 1.0f;
        x++;
        zappies[x] = 0.0f;
        x++;
        zappies[x] = 0.0f;
        x++;
        // texture coodrs
        zappies[x] = 0.0f;
        x++;
        zappies[x] = 1.0f;
        x++;
    }

    int pt = 0;
    for (int i = 0; i < 32 * vec1.size(); ++i)
    {
        zapp[pt] = zappies[i];
        pt++;
        // cout << zappies[i] << " ";
        // if(i % 8 == 7)cout << endl;
    }

    vector<int> indicestemp;
    int xt = 0;
    for (int i = 0; i < x / 4; ++i)
    {
        indicestemp.push_back(xt);
        indicestemp.push_back(xt + 1);
        indicestemp.push_back(xt + 2);
        indicestemp.push_back(xt + 2);
        indicestemp.push_back(xt + 3);
        indicestemp.push_back(xt);

        xt += 4;
    }

    pt = 0;
    for (int i = 0; i < indicestemp.size(); ++i)
    {
        zappindices[pt] = indicestemp[i];
        pt++;
        // cout << indices[i] << " ";
        // if(i % 3 == 2)cout << endl;
    }

    // NOW RANDOMLY CHOOSE SOME 10 ZAPPERS to move up and down
    // set contains the zappers index which has to move
    srand((unsigned)time(0));
    while (stset.size() < 10)
    {
        int random = rand();
        if ((random % 30) + 1 < 24)
        {
            stset.insert((random % 30) + 1);
        }
    }

    // cout << "HERE\n" << stset.size();
    for (auto it : stset)
    {
        // cout << it << " ";
        if (it % 2 == 0)
            movingzappers[it] = -0.5f;
        else
            movingzappers[it] = 0.5f;
    }
    // cout << endl;
}

bool collision_detect(float x, float heigth)
{

    // return false;
    int temp = x;
    double temp2 = x - temp;

    if (temp2 > 0 || temp2 < 0)
    {
        ;
    }
    else
    {
        // cout << heigth << " " << movingzappers[temp] << endl;
        float ytmvz = movingzappers[temp];
        // cout << "-----------\n";
        // cout << "-> " <<  << " ";
        if (ytmvz != 0)
        {
            float ymx = ytmvz + 0.5;
            float ymn = ytmvz - 0.5;

            if (heigth - 0.80 >= ymn && heigth - 0.80 <= ymx)
            {
                cout << temp << endl;
                // cout << "GAME OVER" << endl;
                return true;
            }

            return false;
        }

        if (temp % 2 == 1)
        {
            // cout << "UP ZAPPER\n";
            float top = heigth + 0.3f;
            if (top > 0.9f)
            {
                // cout << " GAME OVER !!!!\n";
                return true;
            }
        }
        else
        {
            // cout << "DOWN ZAPPER\n";
            float bottom = heigth;
            if (bottom < 0.9f)
            {
                // cout << " GAME OVER !!!!\n";
                return true;
            }
        }
    }

    return false;
}

float coinvertices[56000];
map<pair<float, float>, int> mp;

void coingen(float cenxpara, float cenypara)
{
    // FOR RANDOM GENERATION
    srand((unsigned)time(0));
    vector<pair<float, float>> centts;
    for (int pos = 0; pos < 50; pos++)
    {
        int random = (rand() % 10) + 1;
        // draw coins from pos to pos + 1
        if (pos % 2 == 1)
        {
            // draw coins in upper area
            for (float i = 0.4; i < 2; i += 0.4)
            {

                float center_x = i + pos;
                float center_y = 0.5 + 0.15;
                if (center_x >= 30)
                {
                    break;
                }
                int utt = (random);
                if (utt % 10 == 1 || utt % 10 == 3)
                {
                    continue;
                }
                centts.push_back({center_x, center_y});
                mp[{center_x, center_y}] = 1;
            }

            for (float i = 0.4; i < 2; i += 0.4)
            {
                float center_x = i + pos;
                float center_y = 0.5 - 0.15;
                if (center_x >= 30)
                {
                    break;
                }
                int utt = (random);
                if (utt % 10 == 4 || utt % 10 == 8)
                {
                    continue;
                }
                centts.push_back({center_x, center_y});
            }
        }
        else
        {
            // draw coins in lower area
            for (float i = 0.4; i < 2; i += 0.4)
            {
                float center_x = i + pos;
                float center_y = -0.5 + 0.15;
                if (center_x >= 30)
                {
                    break;
                }
                int utt = (random);
                if (utt % 10 == 7 || utt % 10 == 9)
                {
                    continue;
                }
                centts.push_back({center_x, center_y});
            }

            for (float i = 0.4; i < 2; i += 0.4)
            {
                float center_x = i + pos;
                float center_y = -0.5 - 0.15;
                if (center_x >= 30)
                {
                    break;
                }
                int utt = (random);
                if (utt % 10 == 5 || utt % 10 == 6)
                {
                    continue;
                }
                centts.push_back({center_x, center_y});
            }
        }
    }

    // for (int i = 0; i < centts.size(); i += 1)
    // {
    //     cout << centts[i].first << " " << centts[i].second << endl;
    // }
    // cout << "\n==============\n";

    // cout << "size of centre vector is " << centts.size() << endl;
    vector<float> vtcoins;
    int num = 4;
    float radious = 0.15f;
    vector<pair<float, float>> vec;
    for (int i = 0; i < centts.size(); i += 1)
    {
        // cout << centts[i].first << " " << centts[i].second << endl;
        float ctx = centts[i].first;
        float cty = centts[i].second;
        // cout << "center is -> " << ctx << " " << cty << endl;
        vec.clear();
        for (int j = 0; j < num; j++)
        {
            float pi = 3.1415f;
            float x = radious * cos(2 * pi * ((float)(j / (float)num)));
            float y = radious * sin(2 * pi * ((float)(j / (float)num)));
            vec.push_back({(float)(ctx + x), (float)(cty + y)});
            // cout << " points are - > " << ctx + x << " " << cty + y << endl;
        }

        // now vec has num no. of vtcoins
        for (int j = 1; j < vec.size(); j++)
        {
            float ver1x = vec[j].first;
            float ver1y = vec[j].second;
            float ver1z = 0.0f;

            float ver2x = vec[j - 1].first;
            float ver2y = vec[j - 1].second;
            float ver2z = 0.0f;

            float ver3x = ctx;
            float ver3y = cty;
            float ver3z = 0.0f;

            vtcoins.push_back(ver1x);
            vtcoins.push_back(ver1y);
            vtcoins.push_back(ver1z);

            vtcoins.push_back(ver2x);
            vtcoins.push_back(ver2y);
            vtcoins.push_back(ver2z);

            vtcoins.push_back(ver3x);
            vtcoins.push_back(ver3y);
            vtcoins.push_back(ver3z);
        }

        // push the last vertex
        vtcoins.push_back(vec[0].first);
        vtcoins.push_back(vec[0].second);
        vtcoins.push_back(0.0f);

        vtcoins.push_back(vec[vec.size() - 1].first);
        vtcoins.push_back(vec[vec.size() - 1].second);
        vtcoins.push_back(0.0f);

        vtcoins.push_back(ctx);
        vtcoins.push_back(cty);
        vtcoins.push_back(0.0f);
    }

    // cout << vtcoins.size() << endl;
    int pt = 0;
    int cttexcord = 0;
    for (int j = 0; j < 20000; j++)
    {
        coinvertices[pt] = vtcoins[j];
        pt++;
        // cout << vtcoins[j] << " ";
        if (j % 3 == 2)
        {
            coinvertices[pt] = 1.0f;
            pt++;
            coinvertices[pt] = 0.0f;
            pt++;
            coinvertices[pt] = 0.0f;
            pt++;

            if (cttexcord % 12 == 0 || cttexcord % 12 == 4)
            {
                coinvertices[pt] = 0.0f;
                pt++;
                coinvertices[pt] = 1.0f;
                pt++;
                cttexcord++;
            }
            else if (cttexcord % 12 == 1 || cttexcord % 12 == 9)
            {
                coinvertices[pt] = 1.0f;
                pt++;
                coinvertices[pt] = 1.0f;
                pt++;
                cttexcord++;
            }
            else if (cttexcord % 12 == 10 || cttexcord % 12 == 6)
            {
                coinvertices[pt] = 1.0f;
                pt++;
                coinvertices[pt] = 0.0f;
                pt++;
                cttexcord++;
            }
            else if (cttexcord % 12 == 7 || cttexcord % 12 == 3)
            {
                coinvertices[pt] = 0.0f;
                pt++;
                coinvertices[pt] = 0.0f;
                pt++;
                cttexcord++;
            }
            else
            {
                coinvertices[pt] = 0.5f;
                pt++;
                coinvertices[pt] = 0.5f;
                pt++;
                cttexcord++;
            }
        }
    }
    // cout << pt << endl;
}

void coincollisiondetect(float x, float height)
{

    int yt = x;

    float xcor = x;

    float ycor = height - 0.85;
    // cout << height - 0.85 << endl;
    // cout << xcor << " " << ycor << endl;

    if (ycor < 0.4 && ycor > 0.3)
    {
        for (float i = 0.2; i < 1; i += 0.2)
        {
            float temp = yt + i;
            float l = temp - 0.08;
            float r = temp + 0.08;
            if (xcor < r && xcor > l)
            {
                // cout << "my centre here is : " << temp << " " << "0.35" << endl;
                if (mp[{temp, 0.35}] == 1 || mp[{temp, 0.35}] == 0)
                {
                    mp[{temp, 0.35}] = 2;
                    return;
                }
            }
        }
    }

    if (ycor < 0.7 && ycor > 0.6)
    {
        for (float i = 0.2; i < 1; i += 0.2)
        {
            float temp = yt + i;
            float l = temp - 0.08;
            float r = temp + 0.08;
            if (xcor < r && xcor > l)
            {
                // cout << "my centre here is : " << temp << " " << "0.65" << endl;
                if (mp[{temp, 0.65}] == 1 || mp[{temp, 0.65}] == 0)
                {
                    mp[{temp, 0.65}] = 2;
                    return;
                }
            }
        }
    }

    if (ycor < -0.3 && ycor > -0.4)
    {
        for (float i = 0.2; i < 1; i += 0.2)
        {
            float temp = yt + i;
            float l = temp - 0.08;
            float r = temp + 0.08;
            if (xcor < r && xcor > l)
            {
                // cout << "my centre here is : " << temp << " " << "-0.35" << endl;
                if (mp[{temp, -0.35}] == 1 || mp[{temp, -0.35}] == 0)
                {
                    mp[{temp, -0.35}] = 2;
                    return;
                }
            }
        }
    }

    if (ycor < -0.6 && ycor > -0.7)
    {
        for (float i = 0.2; i < 1; i += 0.2)
        {
            float temp = yt + i;
            float l = temp - 0.08;
            float r = temp + 0.08;
            if (xcor < r && xcor > l)
            {
                // cout << "my centre here is : " << temp << " " << "-0.65" << endl;
                if (mp[{temp, -0.65}] == 1 || mp[{temp, -0.65}] == 0)
                {
                    mp[{temp, -0.65}] = 2;
                    return;
                }
            }
        }
    }
}

int main()
{
    // ALL THE VARIABLES REQUIRED ARE HERE
    //***********************

    int level = 1;
    myfavzappies();
    coingen(-1.0, -1.0);

    // for(auto it : stset){
    //     cout << it << endl;
    // }

    // for(int i = 0 ; i<100; ++i){
    // cout << zapp[i] << " ";
    // if(i % 3 == 2)cout << endl;
    // if(i % 12 == 11)cout << endl;
    // }
    // cout << "HERE"<< endl;
    // for (int kk = 0; kk < 8; kk++)
    // {
    //     cout << coinvertices[kk] << " ";
    // }
    // cout << "ENDHERE"<< endl;

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("../src/shader.vs", "../src/shader.fs"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // FIRST BACKGROUND IMAGE COORDINATES
    float vertices[] = {
        // positions         // colors
        10.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom right
        10.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom right
        -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // bottom right
        -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f   // bottom right
    };

    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // SECOND BACKGROUND IMAGE COORDINATES
    float verticesbg2[] = {
        // positions         // colors
        20.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom right
        20.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom right
        10.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // bottom right
        10.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f   // bottom right
    };

    unsigned int indicesbg2[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // THIRD BACKGROUND IMAGE COORDINATES
    float verticesbg3[] = {
        // positions         // colors
        30.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom right
        30.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom right
        20.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // bottom right
        20.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f   // bottom right
    };

    unsigned int indicesbg3[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // our character array
    float avatarver[] = {
        // positions         // colors        // texture coordinates
        -0.6f, -0.7f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.6f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.8f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.8f, -0.7f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};

    unsigned int avatarindices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    float verfinishline[] = {
        // positions         // colors        // texture coordinates
        30.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        30.5f, -1.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        30.0f, -1.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        30.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};

    unsigned int indicesfinishline[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // For the final score display
    float finalscore[] = {
        // positions         // colors
        32.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom right
        32.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom right
        28.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // bottom right
        28.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f   // bottom right
    };

    unsigned int finalindices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // BACKGROUND - 1
    // *************************
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // // *************************

    // *************************
    // BACKGROUND - 2
    unsigned int VBObg2, VAObg2, EBObg2;
    glGenVertexArrays(1, &VAObg2);
    glGenBuffers(1, &VBObg2);
    glGenBuffers(1, &EBObg2);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAObg2);

    glBindBuffer(GL_ARRAY_BUFFER, VBObg2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesbg2), verticesbg2, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBObg2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesbg2), indicesbg2, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // // *************************

    // *************************
    // BACKGROUND - 3
    unsigned int VBObg3, VAObg3, EBObg3;
    glGenVertexArrays(1, &VAObg3);
    glGenBuffers(1, &VBObg3);
    glGenBuffers(1, &EBObg3);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAObg3);

    glBindBuffer(GL_ARRAY_BUFFER, VBObg3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesbg3), verticesbg3, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBObg3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesbg3), indicesbg3, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // // *************************

    // *************************
    // Finalscore for display
    unsigned int VBOfinscore, VAOfinscore, EBOfinscore;
    glGenVertexArrays(1, &VAOfinscore);
    glGenBuffers(1, &VBOfinscore);
    glGenBuffers(1, &EBOfinscore);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAOfinscore);
    glBindBuffer(GL_ARRAY_BUFFER, VBOfinscore);
    glBufferData(GL_ARRAY_BUFFER, sizeof(finalscore), finalscore, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOfinscore);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(finalindices), finalindices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // // *************************

    // *************************
    // FINISH LINE
    unsigned int VBOfnl, VAOfnl, EBOfnl;
    glGenVertexArrays(1, &VAOfnl);
    glGenBuffers(1, &VBOfnl);
    glGenBuffers(1, &EBOfnl);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAOfnl);

    glBindBuffer(GL_ARRAY_BUFFER, VBOfnl);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verfinishline), verfinishline, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOfnl);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesfinishline), indicesfinishline, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // // *************************

    // rendering the avatar
    // *************************
    unsigned int VBO1, VAO1, EBO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &EBO1);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(avatarver), avatarver, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(avatarindices), avatarindices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // *************************

    // rendering the zappies
    // *************************
    unsigned int VBO2, VAO2, EBO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO2);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(zapp), zapp, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(zappindices), zappindices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // *************************

    // rendering the coins
    // *************************
    unsigned int VBOcoin, VAOcoin, EBOcoin;
    glGenVertexArrays(1, &VAOcoin);
    glGenBuffers(1, &VBOcoin);
    glGenBuffers(1, &EBOcoin);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAOcoin);

    glBindBuffer(GL_ARRAY_BUFFER, VBOcoin);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coinvertices), coinvertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // *************************

    // load and create a texture
    GLuint texture[15];
    glGenTextures(15, texture);
    // FIRST TEXTURE BACKGROUND - 1
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char *data;
    data = stbi_load("../src/images/bg1.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // SECOND TEXTURE BACKGROUND - 2
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int widthbg2, heightbg2, nrChannelsbg2;
    unsigned char *databg2;
    databg2 = stbi_load("../src/images/bg2.jpg", &widthbg2, &heightbg2, &nrChannelsbg2, 0);
    if (databg2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthbg2, heightbg2, 0, GL_RGB, GL_UNSIGNED_BYTE, databg2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(databg2);

    // THIRD TEXTURE BACKGROUND - 3
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int widthbg3, heightbg3, nrChannelsbg3;
    unsigned char *databg3;
    databg3 = stbi_load("../src/images/bg3.jpg", &widthbg3, &heightbg3, &nrChannelsbg3, 0);
    if (databg3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthbg3, heightbg3, 0, GL_RGB, GL_UNSIGNED_BYTE, databg3);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(databg3);

    // FINISH LINE TEXTURE TEXTURE BACKGROUND - 4
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int widthfnl, heightfnl, nrChannelsfnl;
    unsigned char *datafnl;
    datafnl = stbi_load("../src/images/cross.jpg", &widthfnl, &heightfnl, &nrChannelsfnl, 0);
    if (datafnl)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthfnl, heightfnl, 0, GL_RGB, GL_UNSIGNED_BYTE, datafnl);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(datafnl);

    // SECOND TEXTURE
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    int w, h, n;
    unsigned char *data1 = stbi_load("../src/images/myp.png", &w, &h, &n, 0);
    if (n == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
    }
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    stbi_image_free(data1);

    // THIRD TEXTURE
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    int w1, h1, n1;
    unsigned char *data2 = stbi_load("../src/images/boyflyavt.png", &w1, &h1, &n1, 0);
    if (n1 == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w1, h1, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w1, h1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
    }
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    stbi_image_free(data2);

    // FLYING AVATAR TEXTURE
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    int w1flyavt, h1flyavt, n1flyavt;
    unsigned char *data2flyavt = stbi_load("../src/images/boyflyglow.png", &w1flyavt, &h1flyavt, &n1flyavt, 0);
    if (n1flyavt == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w1flyavt, h1flyavt, 0, GL_RGB, GL_UNSIGNED_BYTE, data2flyavt);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w1flyavt, h1flyavt, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2flyavt);
    }
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    stbi_image_free(data2flyavt);

    // FOURTH TEXTURE
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    int wcn, hcn, ncn;
    unsigned char *data3 = stbi_load("../src/images/transcoin.png", &wcn, &hcn, &ncn, 0);
    if (ncn == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wcn, hcn, 0, GL_RGB, GL_UNSIGNED_BYTE, data3);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wcn, hcn, 0, GL_RGBA, GL_UNSIGNED_BYTE, data3);
    }
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    stbi_image_free(data3);

    // MAKE THE THREE MOVEMENT AVATAR
    // LEFT ------------------------------------------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, texture[11]);
    int wcavleft, hcavleft, ncavleft;
    unsigned char *dataavleft = stbi_load("../src/images/leftavt.png", &wcavleft, &hcavleft, &ncavleft, 0);
    if (ncavleft == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wcavleft, hcavleft, 0, GL_RGB, GL_UNSIGNED_BYTE, dataavleft);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wcavleft, hcavleft, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataavleft);
    }
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    stbi_image_free(dataavleft);
    // ------------------------------------------------------------------------------------------
    // CENTER ------------------------------------------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    int wcavcenter, hcavcenter, ncavcenter;
    unsigned char *dataavcenter = stbi_load("../src/images/centeravt.png", &wcavcenter, &hcavcenter, &ncavcenter, 0);
    if (ncavcenter == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wcavcenter, hcavcenter, 0, GL_RGB, GL_UNSIGNED_BYTE, dataavcenter);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wcavcenter, hcavcenter, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataavcenter);
    }
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    stbi_image_free(dataavcenter);
    // ------------------------------------------------------------------------------------------
    // RIGHT ------------------------------------------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    int wcavright, hcavright, ncavright;
    unsigned char *dataavright = stbi_load("../src/images/rightavt.png", &wcavright, &hcavright, &ncavright, 0);
    if (ncavright == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wcavright, hcavright, 0, GL_RGB, GL_UNSIGNED_BYTE, dataavright);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wcavright, hcavright, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataavright);
    }
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    stbi_image_free(dataavright);
    // ------------------------------------------------------------------------------------------

    // calculate time
    double rotation = 0.0f;
    double prevtime = glfwGetTime();

    // VARIABLES TO MAKE GRAVITY USE
    float centre_x = -0.7f;
    float y = 0;
    float currentpos = (-0.85f);
    float xcor = 0.0f;
    int currlevel = 1;
    float fast = 0.003f;
    int incrementlevel2 = 0;
    int incrementlevel3 = 0;
    int finalwin = 0;
    int coinscore = 0;
    float translatezappers = 0.0f;
    float trzapspeed = 0.005f;
    int legsmove = 0;
    GLuint avatarchange;
    GLuint legstexture;
    int todisplayup = 0;
    int lostmessage = 0;
    int winflag=0;

    Text T;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        // SET THE up down key KEY
        avatarchange = texture[2];
        if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS))
        {
            y += 0.02f;
            currentpos += 0.02f;
            avatarchange = texture[7];
        }

        // SET THE CORRESPONDING LEGS MOVEMENT
        if (legsmove % 45 < 20)
        {
            legstexture = texture[11];
            legsmove++;
        }
        else if (legsmove % 45 >= 20 && legsmove % 15 < 25)
        {
            legstexture = texture[12];
            legsmove++;
        }
        else if (legsmove % 45 >= 25)
        {
            legstexture = texture[13];
            legsmove++;
        }

        // simulate the downfall based on height
        float temp = 0.01; // gravity(currentpos);
        y -= (temp);
        currentpos -= temp;

        if (y < 0)
            y = 0.0f;

        if (y > 1.7)
            y = 1.7f;

        // cout << y << endl;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        // glBindTexture(GL_TEXTURE_2D, texture);

        // get and set the time
        // unsigned int isZapperloc = glGetUniformLocation(ourShader.ID, "isZapper");
        // glUniform1i(isZapperloc, 0);
        ourShader.use();
        ourShader.setInt("isZapper", 0);

        double crtTime = glfwGetTime();
        if (crtTime - prevtime >= 1 / 60)
        {
            rotation += 0.007f;
            prevtime = crtTime;
        }

        // create transformations

        xcor += fast;
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform = glm::translate(transform, glm::vec3(-xcor, 0.0f, 0.0f));
        centre_x += fast;
        // cout << centre_x << " \n";
        float value = std::ceil(centre_x * 100.0) / 100.0;

        // set the level variable
        if (currlevel == 2 && incrementlevel2 == 0)
        {
            fast += 0.002;
            incrementlevel2 = 1;
        }

        if (currlevel == 3 && incrementlevel3 == 0)
        {
            fast += 0.002;
            incrementlevel3 = 1;
        }

        if (xcor >= 10.7)
        {
            currlevel = 2;
        }
        if (xcor >= 20.7)
        {
            currlevel = 3;
        }

        // // COLLISION DETECTION
        if (collision_detect(value, y) == true)
        {
            fast = 2000.0;
            lostmessage = 1;
            // cout << "OUT   ->  " << xcor << endl;
            // break;
            // T.render("GAME OVER : " , 205.0f , 360.0f);
        }

        coincollisiondetect(value, y);

        // get matrix's uniform location and set matrix
        ourShader.use();
        ourShader.setInt("isZapper", 0);
        // T.render("GAME OVER : " , 205.0f , 360.0f);
        // for(int i=
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // LOAD THE FIRST BACKGROUND IMAGE
        ourShader.use();
        ourShader.setInt("isZapper", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // LOAD THE SECOND BACKGROUND IMAGE
        ourShader.use();
        ourShader.setInt("isZapper", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glBindVertexArray(VAObg2);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // LOAD THE SECOND BACKGROUND IMAGE
        ourShader.use();
        ourShader.setInt("isZapper", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glBindVertexArray(VAObg3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // // LOAD THE FINISH LINE IMAGE
        ourShader.use();
        ourShader.setInt("isZapper", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[6]);
        glBindVertexArray(VAOfnl);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // render the coins
        glBindVertexArray(VAOcoin);
        // ourShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        // glDrawElements(GL_TRIANGLES, 3 , GL_UNSIGNED_INT, 0);
        int pointer = 0;
        int torender = 12;
        int till_now = 0;
        for (int i = 0; i < 4500; i += 12)
        {

            float yt1 = coinvertices[16 + 96 * pointer];
            float yt2 = coinvertices[17 + 96 * pointer];

            if (mp[{yt1, yt2}] == 2)
            {
                till_now++;
            }
            else
            {
                // cout << yt1 << " " << yt2 << endl;
                glDrawArrays(GL_TRIANGLES, i, torender);
            }

            pointer++;
        }
        // cout << till_now << endl;
        // isZapperloc = glGetUniformLocation(ourShader.ID, "isZapper");
        // glUniform1i(isZapperloc, 1);
        ourShader.use();
        ourShader.setInt("isZapper", 1);

        // VAO2 IS FOR THE ZAPPIES IMAGE
        ourShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        transform = glm::mat4(1.0f);
        translatezappers += trzapspeed;
        if (translatezappers >= 1)
        {
            trzapspeed = -trzapspeed;
        }
        if (translatezappers < 0)
        {
            trzapspeed = -trzapspeed;
        }
        // isZapperloc = glGetUniformLocation(ourShader.ID, "isZapper");
        // glUniform1i(isZapperloc, 0);

        // transform = glm::translate(transform, glm::vec3(-xcor, translatezappers, 0.0f));
        // transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(VAO2);
        // for (int i = 0; i < 200; i += 3){
        // glDrawArrays(GL_TRIANGLES, 0, 7);
        // }
        // cout << movingzappers[1] << endl;
        for (int i = 0; i < 30; i++)
        {
            if (stset.find(i) != stset.end())
            {
                // cout << i << endl;
                if (i % 2 == 0)
                {
                    // down zapper
                    transform = glm::translate(transform, glm::vec3(0.0f, translatezappers, 0.0f));
                    movingzappers[i] = -0.5 + translatezappers;
                    transformLoc = glGetUniformLocation(ourShader.ID, "transform");
                    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
                }
                else
                {
                    // up zapper
                    transform = glm::translate(transform, glm::vec3(0.0f, -translatezappers, 0.0f));
                    movingzappers[i] = 0.5 - translatezappers;
                    transformLoc = glGetUniformLocation(ourShader.ID, "transform");
                    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
                }

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)((6 * i) * sizeof(unsigned int)));
            }
            else
            {
                transform = glm::mat4(1.0f);
                transform = glm::translate(transform, glm::vec3(-xcor, 0.0f, 0.0f));
                transformLoc = glGetUniformLocation(ourShader.ID, "transform");
                glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)((6 * i) * sizeof(unsigned int)));
            }
        }

        ourShader.use();
        ourShader.setInt("isZapper", 0);

        // VAO1 IS FOR THE AVATAR IMAGE
        glActiveTexture(GL_TEXTURE0);
        if (y == 0)
        {
            glBindTexture(GL_TEXTURE_2D, legstexture);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, avatarchange);
        }
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.0f, y, 0.0f));
        transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // DISPLAY THE HUD
        coinscore = till_now;
        if (todisplayup == 0)
        {
            string scorestr = to_string(till_now);
            string levelstr = to_string(currlevel);
            string disstr1 = to_string(round(xcor * 10) / 10);
            string disstr2 = disstr1.substr(0, 4);
            T.render("LEVEL : ", 5.0f, 560.0f);
            T.render(levelstr, 210.0f, 560.0f);
            T.render("SCORE : ", 5.0f, 520.0f);
            T.render(scorestr, 210.0f, 520.0f);
            T.render("DISTANCE : ", 5.0f, 480.0f);
            T.render(disstr2, 300.0f, 480.0f);
        }
        if(winflag==1) lostmessage=0;
        if (xcor >= 30 && lostmessage == 0)
        {
            winflag = 1;
            string scorestr = to_string(till_now);
            todisplayup = 1;
            T.render("YOU WON THE GAME", 125.0f, 360.0f);
            T.render("SCORE : ", 155.0f, 310.0f);
            T.render(scorestr, 360.0f, 310.0f);
        }
        else if (winflag == 0 && xcor > 35)
        {
            fast = 0.003;
            string scorestr = to_string(till_now);
            todisplayup = 1;
            T.render("BETTER LUCK NEXT TIME", 125.0f, 360.0f);
            T.render("SCORE : ", 155.0f, 310.0f);
            T.render(scorestr, 360.0f, 310.0f);
        }

        // cout << lostmessage << endl;

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // NOW SHOW THE FINAL SCORE ON TERMINAL
    if (winflag == 1)
    {
        cout << "\n\n";
        cout << YELLOW << "                             -------------------------------------------\n\n"
             << RESET;
        cout << YELLOW << "                                            " << RESET;
        cout << "\033[;32mYOU WON THE GAME\033[0m";
        cout << endl;
        cout << YELLOW << "                                            " << RESET;
        cout << "\033[;32mYOUR SCORE IS: \033[0m";
        cout << GREEN << coinscore << endl;
        cout << "\n";
        cout << YELLOW << "                             -------------------------------------------\n"
             << RESET;
        cout << "\n\n";
    }
    else
    {
        cout << "\n\n";
        cout << YELLOW << "                             -------------------------------------------\n\n"
             << RESET;
        cout << RED << "                                            " << RESET;
        cout << RED << "YOU LOST THE GAME\033[0m";
        cout << endl;
        cout << RED << "                                            " << RESET;
        cout << RED << "YOUR SCORE IS: \033[0m";
        cout << RED << coinscore << endl;
        cout << "\n";
        cout << YELLOW << "                             -------------------------------------------\n"
             << RESET;
        cout << "\n\n";
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}