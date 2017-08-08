#ifndef BBEXTENT_H
#define BBEXTENT_H

/// @file BBExtent.h
/// @brief a struct for storing dimension and min and max values for the bounding boxes

#include "ngl/Types.h"
#include <QDebug>
#include <QString>

//----------------------------------------------------------------------------------------------------------------------
///  @class BBExtent "include/BBExtent.h"
///  @brief Simple struct to store dimension and min and max attributes for the bounding boxes
///  @author Ruth Hutton
///  @version 1.0
///  @date Last Revision 20/04/17 \n
//----------------------------------------------------------------------------------------------------------------------

struct BBExtent
{
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Default Bounding Box Extents ctor
    //----------------------------------------------------------------------------------------------------------------------
    BBExtent();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the min x value of the bounding box
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Real minX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the max x value of the bounding box
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Real maxX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the min y value of the bounding box
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Real minY;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the max y value of the bounding box
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Real maxY;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the min z value of the bounding box
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Real minZ;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the max z value of the bounding box
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Real maxZ;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the height of the bounding box
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Real BBheight;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the width of the bounding box
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Real BBwidth;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the depth of the bounding box
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Real BBdepth;
};

#endif // BBEXTENT_H
