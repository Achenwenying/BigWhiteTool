//
// Created by Administrator on 2023-08-16.
//

#ifndef BIGWHITETOOL_DRAWPLAYER_H

#include "Android_Read/Android_Read.h"
#include "Android_Read/GetData.h"
#include <sstream>
#define BIGWHITETOOL_DRAWPLAYER_H

#endif //BIGWHITETOOL_DRAWPLAYER_H



void DrawPlayer(ImDrawList *Draw) {
    int py = displayInfo.height / 2;
    int px = displayInfo.width / 2;

    int ArrayaddrCount = GetDowrd( addr.Ulevel+ offsets.ArrayaddrCount);//数量
    float matrix[16];
    memset(matrix, 0, 16);
    ReadAddr(addr.Matrix, matrix, 16 * 4);

    for (int i = 0; i < ArrayaddrCount; i++) {

        uint64_t Objaddr = GetAddr(addr.Arrayaddr + 8 * i);  // 遍历数量次数
        if (Objaddr == 0)   continue;
        // 自身坐标
        Vector3A Z;
        ReadAddr(GetAddr(addr.AcknowledgedPawn + offsets.RootComponent) + offsets.XYZ_X, &Z, sizeof(Z)); // 自己坐标

        // 敌人和物资坐标
        Vector3A D;
        ReadAddr(GetAddr(Objaddr + offsets.RootComponent) + offsets.XYZ_X, &D, sizeof(D)); // 对象坐标
        if (D.X == 0 || D.Y == 0 || D.Z == 0)   continue;

        float camera,r_x,r_y,r_w;
        camera = matrix[3] * D.X + matrix[7] * D.Y + matrix[11] * D.Z + matrix[15];
        r_x = px + (matrix[0] * D.X + matrix[4] * D.Y + matrix[8] * D.Z + matrix[12]) / camera * px;
        r_y = py - (matrix[1] * D.X + matrix[5] * D.Y + matrix[9] * (D.Z - 5) + matrix[13]) / camera * py;
        r_w = py - (matrix[1] * D.X + matrix[5] * D.Y + matrix[9] * (D.Z + 205) + matrix[13]) / camera * py;

        float x = r_x - (r_y - r_w) / 4;
        float y = r_y;
        float w = (r_y - r_w) / 2;
        float xlx = r_x - (r_y - r_w) / 85;
        //float Distance = sqrt(pow(D.X - Z.X, 2) + pow(D.Y - Z.Y, 2) + pow(D.Z - Z.Z, 2)) * 0.01;
        float middle = x + w / 2;
        float bottom = y + w;
        float top = y - w;
        float ah =r_y - r_w;
        if (ah<=0)  continue;//过滤背部敌人
        std::string ClassName = GetName(Objaddr);//类名

        //类名绘制
        Draw->AddText(NULL, 24, {r_x , r_y-60}, ImColor(255,255,255,255) , ClassName.c_str());//类名绘制

    }

}

