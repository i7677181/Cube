#include "GeneralObj.h"

//----------------------------------------------------------------------------------------------------------------------

GeneralObj::GeneralObj()
{
    // set the object transformations to 0
    resetTransformations();
}

//----------------------------------------------------------------------------------------------------------------------

GeneralObj::GeneralObj(ngl::Vec3 _objectPos, ngl::Vec3 _objectRot, ngl::Vec3 _objectScale, std::string objName, std::string objPath, std::string textPath)
{
    resetTransformations();

    m_objectPos = _objectPos;
    m_objectRot = _objectRot;
    m_objectScale = _objectScale;

    m_path = objPath;
    m_name = objName;
    m_tex = textPath;

    // Create a new ngl object using the path of the Obj files and their textures
    objPtr.reset(new ngl::Obj(objPath,textPath));
    objPtr->createVAO();
    // Set up the bounding box and center
    objPtr->calcDimensions();

    // The object position and scale must take into account the rotation of the sphere
    m_rotatedObjectPos = m_objectPos;
    m_rotatedObjectScale = m_objectScale;

    // Store the scale of the objects in a different variable so it can be accessed later in the replace() function
    initialScale = m_objectScale;
}

//----------------------------------------------------------------------------------------------------------------------

void GeneralObj::drawObj()
{
    setExtents();
    // draw the objects to the scene
    objPtr->draw();
}

//----------------------------------------------------------------------------------------------------------------------

void GeneralObj::setExtents()
{
    // Create a temporary bounding box to access the properties of all bounding boxes in the scene
    ngl::BBox tmpBB(objPtr->getBBox());

    // Multiply the dimensions of the bounding boxes by the new scale values
    // to access the scaled height, width and depth values
    m_scaledBBoxDimensions.m_y = (tmpBB.height())*(m_rotatedObjectScale.m_y);
    m_scaledBBoxDimensions.m_x = (tmpBB.width())*(m_rotatedObjectScale.m_x);
    m_scaledBBoxDimensions.m_z = (tmpBB.depth())*(m_rotatedObjectScale.m_z);

    // Divide the dimensions by 2 to access the lengths in each axes from the center of the object
    m_bbExtents.BBheight = (m_scaledBBoxDimensions.m_y)/2.0f;
    m_bbExtents.BBwidth = (m_scaledBBoxDimensions.m_x)/2.0f;
    m_bbExtents.BBdepth = (m_scaledBBoxDimensions.m_z)/2.0f;

    // Use the calculated lengths from the object center to calculate the min and max values for the
    // bounding boxes
    m_bbExtents.minX = m_rotatedObjectPos.m_x - m_bbExtents.BBwidth;
    m_bbExtents.minY = m_rotatedObjectPos.m_y - m_bbExtents.BBheight;
    m_bbExtents.minZ = m_rotatedObjectPos.m_z - m_bbExtents.BBdepth;

    m_bbExtents.maxX = m_rotatedObjectPos.m_x + m_bbExtents.BBwidth;
    m_bbExtents.maxY = m_rotatedObjectPos.m_y + m_bbExtents.BBheight;
    m_bbExtents.maxZ = m_rotatedObjectPos.m_z + m_bbExtents.BBdepth;
}

//----------------------------------------------------------------------------------------------------------------------

void GeneralObj::resetTransformations()
{
    // set position and rotation of the objects to 0
    // set the object scale to 1
    m_objectPos.m_x = 0;
    m_objectPos.m_y = 0;
    m_objectPos.m_z = 0;

    m_objectRot.m_x = 0;
    m_objectRot.m_y = 0;
    m_objectRot.m_z = 0;

    m_objectScale.m_x = 1;
    m_objectScale.m_y = 1;
    m_objectScale.m_z = 1;

    m_rotatedObjectPos.m_x = 0;
    m_rotatedObjectPos.m_y = 0;
    m_rotatedObjectPos.m_z = 0;

    m_rotatedObjectScale.m_x = 1;
    m_rotatedObjectScale.m_y = 1;
    m_rotatedObjectScale.m_z = 1;

}

//----------------------------------------------------------------------------------------------------------------------

void GeneralObj::pickup()
{
    // set the scale of the object and its bounding box to zero
    m_objectScale = ngl::Vec3 (0,0,0);
    deleteBBox();
}

//----------------------------------------------------------------------------------------------------------------------

void GeneralObj::deleteBBox()
{
    // set the object scale to 0 causing the bounding box extents to also be set to 0
    m_rotatedObjectScale.m_x = 0;
    m_rotatedObjectScale.m_y = 0;
    m_rotatedObjectScale.m_z = 0;
}

//----------------------------------------------------------------------------------------------------------------------

void GeneralObj::replace()
{
    // rescales all the objects and their bounding boxes to their original scale
    m_objectScale = initialScale;
    m_rotatedObjectScale = initialScale;
}

//----------------------------------------------------------------------------------------------------------------------
