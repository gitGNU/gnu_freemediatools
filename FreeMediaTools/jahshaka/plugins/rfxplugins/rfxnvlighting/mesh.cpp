/*******************************************************************************
**
** A Jahshaka RasFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "rfxnvlighting.h"
#include <math.h>

////////////////////////////////////////////
//main imageprocessing loop

void MyPlugin::get_mesh_normal(float4& normal, int mesh_x_coordinate, int mesh_y_coordinate,
                                            int mesh_x_dimension, int mesh_y_dimension)
{
    bool						on_the_mesh_left_edge = FALSE;
    bool						on_the_mesh_bottom_edge = FALSE;
    bool						on_the_mesh_right_edge = FALSE;
    bool						on_the_mesh_top_edge = FALSE;
    bool						on_the_mesh_edge;
    float4						vector1;
    float4						vector2;
    float4						point1;
    float4						point2;
    float4						point3;
    float4						normal1;
    float4						normal2;
    float4						normal3;
    float4						normal4;

    point1.w = 1.0;
    point2.w = 1.0;
    point3.w = 1.0;

    if (mesh_x_coordinate == 0)
    {
        on_the_mesh_left_edge = TRUE;
    }

    if (mesh_y_coordinate == 0)
    {
        on_the_mesh_bottom_edge = TRUE;
    }

    if (mesh_x_coordinate == (mesh_x_dimension))
    {
        on_the_mesh_right_edge = TRUE;
    }

    if (mesh_y_coordinate == (mesh_y_dimension))
    {
        on_the_mesh_top_edge = TRUE;
    }

    on_the_mesh_edge = on_the_mesh_left_edge || on_the_mesh_bottom_edge || on_the_mesh_right_edge || on_the_mesh_top_edge;

    if (on_the_mesh_edge)
    {
        getMeshCoord(mesh_x_coordinate, mesh_y_coordinate,
                                                point1.x, point1.y, point1.z);

        if (on_the_mesh_right_edge)
        {
            getMeshCoord(mesh_x_coordinate - 1, mesh_y_coordinate,
                                                point2.x, point2.y, point2.z);
            get_vector_from_points(vector1, point2, point1);
        }
        else
        {
            getMeshCoord(mesh_x_coordinate + 1, mesh_y_coordinate,
                                                point2.x, point2.y, point2.z);
            get_vector_from_points(vector1, point1, point2);
        }

        if (on_the_mesh_top_edge)
        {
            getMeshCoord(mesh_x_coordinate, mesh_y_coordinate - 1,
                                                point3.x, point3.y, point3.z);
            get_vector_from_points(vector2, point3, point1);
        }
        else
        {
            getMeshCoord(mesh_x_coordinate, mesh_y_coordinate + 1,
                                                point3.x, point3.y, point3.z);
            get_vector_from_points(vector2, point1, point3);
        }


        get_cross_product_4(normal, vector1, vector2);
    }
    else
    {
        getMeshCoord(mesh_x_coordinate, mesh_y_coordinate,
                                                point1.x, point1.y, point1.z);

        // Upper right quad
        getMeshCoord(mesh_x_coordinate + 1, mesh_y_coordinate,
                                                point2.x, point2.y, point2.z);

        getMeshCoord(mesh_x_coordinate, mesh_y_coordinate + 1,
                                                point3.x, point3.y, point3.z);

        get_vector_from_points(vector1, point1, point2);
        get_vector_from_points(vector2, point1, point3);
        get_cross_product_4(normal1, vector1, vector2);

        // Upper left quad
        getMeshCoord(mesh_x_coordinate, mesh_y_coordinate + 1,
                                                point2.x, point2.y, point2.z);

        getMeshCoord(mesh_x_coordinate - 1, mesh_y_coordinate,
                                                point3.x, point3.y, point3.z);

        get_vector_from_points(vector1, point1, point2);
        get_vector_from_points(vector2, point1, point3);
        get_cross_product_4(normal2, vector1, vector2);

        // Lower left quad
        getMeshCoord(mesh_x_coordinate - 1, mesh_y_coordinate,
                                                point2.x, point2.y, point2.z);

        getMeshCoord(mesh_x_coordinate, mesh_y_coordinate - 1,
                                                point3.x, point3.y, point3.z);

        get_vector_from_points(vector1, point1, point2);
        get_vector_from_points(vector2, point1, point3);
        get_cross_product_4(normal3, vector1, vector2);

        // Lower right quad
        getMeshCoord(mesh_x_coordinate, mesh_y_coordinate - 1,
                                                point2.x, point2.y, point2.z);

        getMeshCoord(mesh_x_coordinate + 1, mesh_y_coordinate,
                                                point3.x, point3.y, point3.z);

        get_vector_from_points(vector1, point1, point2);
        get_vector_from_points(vector2, point1, point3);
        get_cross_product_4(normal4, vector1, vector2);

        normal.x = (normal1.x + normal2.x + normal3.x + normal4.x) / 4;
        normal.y = (normal1.y + normal2.y + normal3.y + normal4.y) / 4;
        normal.z = (normal1.z + normal2.z + normal3.z + normal4.z) / 4;
        normal.w = 1.0;
// 		normal.w = (normal1.w + normal2.w + normal3.w + normal4.w) / 4;
    }

    normalize_vector4(normal);
}


