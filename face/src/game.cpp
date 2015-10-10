#include "game.h"
#include "glwrapper.h"
/*

    Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Tutorial 16 - Basic Texture Mapping
*/
/*
    zע��㣺��������������������ǽ��������ʷֵĺ���֢����ʵ����ʹ��һ��ָ�룬ָ�򶥵�����Ϊ3��λ�á�
    �������ʷֵõ���ͼ������0~1���䣬Ӧ��ӳ�䵽-1~1���䡣
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
//
#include "ogldev_util.h"
#include "ogldev_pipeline.h"
#include "ogldev_camera.h"
#include "ogldev_texture.h"

#include "mesh_matrix_producer.h"

#include "lodepng.h"

#include <android/log.h>

#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500

class Vertex
{
public:
    Vector3f m_pos;
    Vector2f m_tex;

    Vertex() {}

    Vertex(Vector3f pos, Vector2f tex)
    {
        m_pos = pos;
        m_tex = tex;
    }

    void setVertex(Vector3f pos, Vector2f tex){
        m_pos = pos;
        m_tex = tex;
    }
};


GLuint VBO;
GLuint IBO;
GLuint gWVPLocation;
GLuint gSampler;
GLuint gSampler2;
GLuint originMatrix;
GLuint targetMatrix;
GLuint originWeight;
Texture* pTexture = NULL;
Texture * pTexture2 = NULL;
Camera* pGameCamera = NULL;
PersProjInfo gPersProjInfo;

MESH * originMesh;
MESH * targetMesh;

MESH_MATRIX * mesh_matrix = NULL;


/*
 * Added for change resouces.
 * */
int resourcesUpdate;
char * originPath;
char * targetPath;
char * originPicPath;
char * targetPicPath;
float * origin_key_points;
float * target_key_points;
float origin_key_points_length;
float target_key_points_length;

const char* pVSFileName = "/sdcard/keyPoints/shader1.vs";
const char* pFSFileName = "/sdcard/keyPoints/shader1.fs";

float originWeightValue = 0.0;

int vertexNum;
int triangleNum;

void mesh_compute(MESH * originMesh, MESH * targetMesh, const float originWeight);
void resourcesUpdateAct();
void setUpdate(float * origin, float * target, int origin_length, int target_length, char * origin_pic_path, char * target_pic_path);

static void RenderSceneCB()
{
    if(resourcesUpdate == 1){
        resourcesUpdate = 0;

        resourcesUpdateAct();

    }


    originWeightValue += 0.01f;
    if(originWeightValue >= 1.0)
        originWeightValue = 0.0;
    mesh_compute(originMesh, targetMesh, originWeightValue);

    pGameCamera->OnRender();

    glClear(GL_COLOR_BUFFER_BIT);

    static float Scale = 0.0f;

    Scale += 0.1f;
    Scale += 0.1f;
    Scale += 0.1f;

    Pipeline p;
    p.Rotate(0.0f, Scale, 0.0f);
    p.WorldPos(0.0f, 0.0f, 3.0f);
    p.SetCamera(*pGameCamera);
    p.SetPerspectiveProj(gPersProjInfo);

    glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());
    glUniform1f(originWeight, originWeightValue);
//
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    pTexture->Bind(GL_TEXTURE0);
    pTexture2->Bind(GL_TEXTURE1);
    int indexfirst = 0;
    for(int i = 0; i < triangleNum; i++){
        glUniformMatrix3fv(originMatrix, 1, GL_FALSE, mesh_matrix->triangle_matrix[i].originTransMatrix.get());
        glUniformMatrix3fv(targetMatrix, 1, GL_FALSE, mesh_matrix->triangle_matrix[i].targetTransMatrix.get());
        indexfirst = i * 3;
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid *)(sizeof(unsigned int) * (indexfirst)));
    }



    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    delete mesh_matrix;
    mesh_matrix = NULL;
}

static void CreateVertexBuffer(Vertex * Vertices, int num)
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * num, Vertices, GL_STATIC_DRAW);
}


static void CreateIndexBuffer(unsigned int * Indices, int num)
{
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num, Indices, GL_STATIC_DRAW);
}


static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(1);
    }

    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0]= strlen(pShaderText);
    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        __android_log_print(ANDROID_LOG_INFO, "tag here", "%s ---Error shader program: %s\n", pShaderText, InfoLog);
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}


static void CompileShaders()
{
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    string vs, fs;

    if (!ReadFile(pVSFileName, vs)) {
        exit(1);
    };

    if (!ReadFile(pFSFileName, fs)) {
        exit(1);
    };

    AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
    AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
//        char * myErr = new char[1024];
//        for(int i = 0; i < 1024; i++){
//            myErr[i] = ErrorLog[i];
//        }
        __android_log_print(ANDROID_LOG_INFO, "tag here", "Error linking shader program: %s\n", ErrorLog);
        exit(1);
    }
//
    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glUseProgram(ShaderProgram);

    gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
    //assert(gWVPLocation != 0xFFFFFFFF);
    gSampler = glGetUniformLocation(ShaderProgram, "gSampler");
    gSampler2 = glGetUniformLocation(ShaderProgram, "gSampler2");
    originMatrix = glGetUniformLocation(ShaderProgram, "originMatrix");
    targetMatrix = glGetUniformLocation(ShaderProgram, "targetMatrix");
    originWeight = glGetUniformLocation(ShaderProgram, "originWeight");
}

void mesh_compute(MESH * originMesh, MESH * targetMesh, const float originWeight){
    MESH_MATRIX * mm = middle_mesh_matrix_producer(originMesh, targetMesh, originWeight);
    mesh_matrix = mm;
    vertexNum = mm->middleMesh->vertex_num + 3;
    triangleNum = mm->middleMesh->triangle_num;
    MESH * mesh = mm->middleMesh;
    Vertex * vertices = new Vertex[vertexNum];
    for(int i = 0; i < vertexNum; i++){
        Vector3f * point = new Vector3f((mesh->pVerArr[i].x - 0.5) * 2, (mesh->pVerArr[i].y - 0.5) * 2, 0.0);
        Vector2f * tex = new Vector2f(mesh->pVerArr[i].x, mesh->pVerArr[i].y);
        vertices[i].setVertex(*point, *tex);

        cout << "(" << point->x << ", " << point->y << ", " << point->z << ")" << ",(" << tex->x << ", " << tex->y << ")" << endl;

    }

    TRIANGLE * triangle_ptr = mesh->pTriArr;
    unsigned int * indices = new unsigned int[triangleNum * 3];

    int idx = 0;
    for(int i = 0; i < triangleNum; i++){
        indices[idx++] = triangle_ptr->i1;
        indices[idx++] = triangle_ptr->i2;
        indices[idx++] = triangle_ptr->i3;
        triangle_ptr = triangle_ptr->pNext;

        cout << indices[idx - 3] << ", " << indices[idx - 2] << ", " << indices[idx - 1] << endl;
    }

    CreateVertexBuffer(vertices, vertexNum);
    CreateIndexBuffer(indices, triangleNum * 3);

    delete [] vertices;
    delete [] indices;
}

void on_surface_created(){
//    /*ǰ��׼������������-----------------------------------------------------------------------*/
    resourcesUpdate = 1;

//    setUpdate(NULL, NULL, 9, 9, "/sdcard/keyPoints/image-fbb.png", "/sdcard/keyPoints/image-lh.png");

    /*ǰ��׼��������������-----------------------------------------------------------------------*/

    pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Must be done after glut is initialized!

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    CompileShaders();

    glUniform1i(gSampler, 0);
    glUniform1i(gSampler2, 1);



    gPersProjInfo.FOV = 60.0f;
    gPersProjInfo.Height = WINDOW_HEIGHT;
    gPersProjInfo.Width = WINDOW_WIDTH;
    gPersProjInfo.zNear = 1.0f;
    gPersProjInfo.zFar = 100.0f;
//    glClearColor(1.0, 0.0, 0.0, 1.0);
}

void on_surface_changed(){
}

void on_draw_frame(){
    //ˢ��һ��
    RenderSceneCB();
}

void resourcesUpdateAct(){
    originMesh = new MESH();
    targetMesh = new MESH();

//    mesh_init(originPath, targetPath, originMesh, targetMesh);
    mesh_init(origin_key_points, target_key_points, origin_key_points_length, target_key_points_length, originMesh, targetMesh);

    pTexture = new Texture(GL_TEXTURE_2D, originPicPath);
    pTexture2 = new Texture(GL_TEXTURE_2D, targetPicPath);

    if ((!pTexture->Load()) || (!pTexture2->Load())) {
        exit(1);
    }

    pTexture->Bind(GL_TEXTURE0);
    pTexture2->Bind(GL_TEXTURE1);
}

void setUpdate(float * origin, float * target, int origin_length, int target_length, char * origin_pic_path, char * target_pic_path){
//    __android_log_print(ANDROID_LOG_INFO, "JNITag","%s", "run in setUpdate");
    resourcesUpdate = 1;
    origin_key_points = origin;
    target_key_points = target;

    origin_key_points_length = origin_length;
    target_key_points_length = target_length;

    originPicPath = origin_pic_path;
    targetPicPath = target_pic_path;
//
//    __android_log_print(ANDROID_LOG_INFO, "JNITag","%d, %s, %s, %d, %d", update, origin_path, target_path, origin_length, target_length);
}
