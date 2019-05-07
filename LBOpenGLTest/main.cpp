//
//  main.cpp
//  LBOpenGLTest
//
//  Created by 李斌 on 2019/5/2.
//  Copyright © 2019 com.njyl. All rights reserved.
//
#include <stdio.h>

#include "GLShaderManager.h"

#include "GLTools.h"

#include <glut/glut.h>

//简单的批次容器,是GLTools的一个简单的容器类
GLBatch triangleBatch;

//着色管理器
GLShaderManager shaderManager;

//blockSize : 边长的1/2
GLfloat blockSize = 0.1f;

GLfloat vVerts[] = {
    -blockSize,-blockSize,0.0f,
    blockSize,-blockSize,0.0f,
    blockSize,blockSize,0.0f,
    -blockSize,blockSize,0.0f,
};

GLfloat xPos = 0.0f;
GLfloat yPos = 0.0f;

//窗口大小改变时接受新的宽度和高度，其中0,0代表窗口中视口的左下角坐标，w，h代表像素

void ChangeSize(int w,int h)

{
    
    glViewport(0,0, w, h);
    
}

//为程序作一次性的设置

void SetupRC()

{
    
    //设置背影颜色
    
    glClearColor(0.0f,0.0f,1.0f,1.0f);
    
    //初始化着色管理器
    
    shaderManager.InitializeStockShaders();
    
    //设置三角形，其中数组vVert包含所有3个顶点的x,y,笛卡尔坐标对。
    
//    GLfloat vVerts[] = {
//
//        -0.5f,0.0f,0.0f,
//
//        0.5f,0.0f,0.0f,
//
//        0.0f,0.5f,0.0f,
//
//    };
    
    //批次处理
    // begin方法第一个参数是指用不同的模式来绘图,第二个参数是指有几个顶点
    triangleBatch.Begin(GL_TRIANGLE_FAN,4);
    
    triangleBatch.CopyVertexData3f(vVerts);
    
    triangleBatch.End();
    
}

//开始渲染

void RenderScene(void)

{
    
    //清除一个或一组特定的缓冲区
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    //设置一组浮点数来表示红色
    GLfloat vGreen[] = {0.0f,1.0f,0.0f,1.0f};
//    GLfloat vRed[] = {1.0f,0.0f,0.0f,1.0f};
    
    //传递到存储着色器，即GLT_SHADER_IDENTITY着色器，这个着色器只是使用指定颜色以默认笛卡尔坐标第在屏幕上渲染几何图形
    
//    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vGreen);
    
    // x,y,z,w(缩放因子默认1.0)
    M3DMatrix44f mTransformMatrix,mFinalTransform,mRotationMatix;
    
    m3dTranslationMatrix44(mTransformMatrix, xPos, yPos, 0.0f);
    
    static float ZRot = 0.0f;
    
    m3dRotationMatrix44(mRotationMatix, m3dDegToRad(ZRot), 0.0f, 0.0f, 1.0f);
    ZRot += 5.0f;
    
    //矩阵相乘
    m3dMatrixMultiply44(mFinalTransform, mTransformMatrix, mRotationMatix);
    
    //平面着色器
    shaderManager.UseStockShader(GLT_SHADER_FLAT,mFinalTransform,vGreen);
    
    //提交着色器
    
    triangleBatch.Draw();
    
    //将在后台缓冲区进行渲染，然后在结束时交换到前台
    
    glutSwapBuffers();
    
}


void SpecialKeys(int key,int x, int y)
{
    /* 键盘上下左右移动绘制的正方形  第一种方法
    GLfloat stepSize = 0.025f;
    GLfloat blockX = vVerts[0];
    GLfloat blockY = vVerts[10];
    
    if (key == GLUT_KEY_UP) {
        blockY += stepSize;
    }
    
    if (key == GLUT_KEY_DOWN) {
        blockY -= stepSize;
    }
    
    if (key == GLUT_KEY_LEFT) {
        blockX -= stepSize;
    }
    
    if (key == GLUT_KEY_RIGHT) {
        blockX += stepSize;
    }
    
    //更新顶点之前 检测是否到边界 到了则限制在显示范围内
    if (blockX < -1.0f) {
        blockX = -1.0f;
    }
    
    if (blockX > 1.0f - blockSize * 2) {
        blockX = 1.0f - blockSize * 2;
    }
    
    if (blockY < -1.0f + blockSize * 2) {
        blockY = -1.0f + blockSize * 2;
    }
    
    if (blockY > 1.0f) {
        blockY = 1.0f;
    }
    
    //更新其他顶点的位置
    //A点
    vVerts[0] = blockX;
    vVerts[1] = blockY - blockSize * 2;
    
    //B点
    vVerts[3] = blockX + blockSize * 2;
    vVerts[4] = blockY - blockSize * 2;
    
    //C点
    vVerts[6] = blockX + blockSize * 2;
    vVerts[7] = blockY;
    
    //D点
    vVerts[9] = blockX;
    vVerts[10] = blockY;
    
    triangleBatch.CopyVertexData3f(vVerts);
    
    glutPostRedisplay();
     */
    
    GLfloat stepSize = 0.025f;
    if (key == GLUT_KEY_UP) {
        yPos += stepSize;
    }
    
    if (key == GLUT_KEY_DOWN) {
        yPos -= stepSize;
    }
    
    if (key == GLUT_KEY_LEFT) {
        xPos -= stepSize;
    }
    
    if (key == GLUT_KEY_RIGHT) {
        xPos += stepSize;
    }
    
    //边界检测
    if (xPos < -1.0f + blockSize) {
        xPos = -1.0f + blockSize;
    }
    
    if (xPos > 1.0f - blockSize) {
        xPos = 1.0f - blockSize;
    }
    
    if (yPos < -1.0f + blockSize) {
        yPos = -1.0f + blockSize;
    }
    
    if (yPos > 1.0f - blockSize) {
        yPos = 1.0f - blockSize;
    }
    glutPostRedisplay();
}

int main(int argc,char* argv[])

{
    
    //设置当前工作目录，针对MAC OS X
    
    gltSetWorkingDirectory(argv[0]);
    
    //初始化GLUT库
    
    glutInit(&argc, argv);
    
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    
    //GLUT窗口大小，标题窗口
    
    glutInitWindowSize(800,600);
    
    glutCreateWindow("Triangle");
    
    //注册回调函数
    
    glutReshapeFunc(ChangeSize);
    
    glutDisplayFunc(RenderScene);
    
    glutSpecialFunc(SpecialKeys);
    
    //驱动程序的初始化中没有出现任何问题。
    
    GLenum err = glewInit();
    
    if(GLEW_OK != err) {
        
        fprintf(stderr,"glew error:%s\n",glewGetErrorString(err));
        
        return 1;
        
    }
    
    //调用SetupRC
    
    SetupRC();
    
    glutMainLoop();
    
    return 0;
    
}
