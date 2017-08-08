#ifndef GENERALOBJ_H__
#define GENERALOBJ_H__

/// @file GeneralObj.h
/// @brief a class for setting up objects and bounding boxes

#include "BBExtent.h"
#include <ngl/Transformation.h>
#include <ngl/Obj.h>
#include <ngl/ShaderLib.h>

//----------------------------------------------------------------------------------------------------------------------
///  @class GeneralObj "include/GeneralObj.h"
///  @brief Simple object and bounding box loading class used to set up properties and create bounding boxes
///  @author Ruth Hutton
///  @version 1.0
///  @date Last Revision 08/05/17 A function to delete the bounding boxes added \n
//----------------------------------------------------------------------------------------------------------------------

class GeneralObj
{
public:

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Default General Object ctor
    //----------------------------------------------------------------------------------------------------------------------
    GeneralObj();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief General Object ctor given transformations, object name, path and texture path. Uses calcDimensions method to create
    /// a bounding box when an object is created using the min and max values of the object as extents.
    ///  @param[in]  _objectPos the position of the object.
    ///  @param[in]  _objectRot the rotation of the object.
    ///  @param[in]  _objectScale the scale of the object.
    ///  @param[in]  objName the given name of the object.
    ///  @param[in]  objPath the path to the object file.
    ///  @param[in]  textPath the path to the texture for the object.
    //----------------------------------------------------------------------------------------------------------------------
    GeneralObj(ngl::Vec3 _objectPos, ngl::Vec3 _objectRot, ngl::Vec3 _objectScale, std::string objName, std::string objPath, std::string textPath);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Get Position method returns the position of the object
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 getPos() {return m_objectPos;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Get Rotated Position method returns the position of the object after taking into account the rotation of the world
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 getRotatedPos() {return m_rotatedObjectPos;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Get Rotation method returns the rotation of the object
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 getRot() {return m_objectRot;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @author Caterina Malfe'
    /// @brief stores the initial scale of the object for the replace() method
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 initialScale;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Get Scale method returns the scale of the object
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 getScale() {return m_objectScale;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Set Position method sets _pos param to equal object position
    /// @param[in] _pos the object position
    //----------------------------------------------------------------------------------------------------------------------
    inline void setPos(ngl::Vec3 _pos) { m_objectPos = _pos; }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Set Rotated Position method sets _pos param to equal object position after taking into account the rotation of the world
    /// @param[in] _pos the object position
    //----------------------------------------------------------------------------------------------------------------------
    inline void setRotatedPos(ngl::Vec3 _pos) { m_rotatedObjectPos = _pos; }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Set Rotated Scale method sets _objectScale param to equal object scale after taking into account the rotation of the world
    //----------------------------------------------------------------------------------------------------------------------
    inline void setRotatedScale(ngl::Vec3 _objectScale) { m_rotatedObjectScale = _objectScale; }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief pointer to Obj in NGL
    //----------------------------------------------------------------------------------------------------------------------
    std::unique_ptr<ngl::Obj> objPtr;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Get Extent method returns the extents
    /// used when creating bounding boxes for each object
    //----------------------------------------------------------------------------------------------------------------------
    inline BBExtent getExtents() {return m_bbExtents;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Draw Obj method draws the object to the scene
    //----------------------------------------------------------------------------------------------------------------------
    void drawObj();
    //----------------------------------------------------------------------------------------------------------------------
    /// @author Caterina Malfe
    /// @brief Pickup method sets the scale of the items the player interacts with to 0, making them disappear
    //----------------------------------------------------------------------------------------------------------------------
    void pickup();
    //----------------------------------------------------------------------------------------------------------------------
    /// @author Caterina Malfe'
    /// @brief Replace method sets the scale of all the items the player may have picked up back to their original scale and puts thei bounding boxes back in
    /// used for the game reset()
    //----------------------------------------------------------------------------------------------------------------------
    void replace();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Delete Bounding Box method sets the extents of the bounding box to 0
    //----------------------------------------------------------------------------------------------------------------------
    void deleteBBox();

private:

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief reset Transformation method sets object position and rotation to 0 and scale to 1
    //----------------------------------------------------------------------------------------------------------------------
    void resetTransformations();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Loads matrices to shader
    //----------------------------------------------------------------------------------------------------------------------
    void loadMatricesToShader();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Sets the extents of the bounding box to equal the min and max values of the object in the x, y and z axes.
    /// Calculates the min and max values of the bounding box by dividing the width, height and depth by 2 and adding/subtracting
    /// the value from the object position.
    //----------------------------------------------------------------------------------------------------------------------
    void setExtents();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The object position
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_objectPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The object position after taking into account the rotation of the world.
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_rotatedObjectPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The object scale after taking into account the rotation of the world.
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_rotatedObjectScale;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The extents of the bounding box after taking into account the new scale of the objects
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_scaledBBoxDimensions;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The object rotation
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_objectRot;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The object scale
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_objectScale;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The path to the obj file of the objects
    //----------------------------------------------------------------------------------------------------------------------
    std::string m_path;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The path to the texture file for the objects
    //----------------------------------------------------------------------------------------------------------------------
    std::string m_tex;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The name of the object
    //----------------------------------------------------------------------------------------------------------------------
    std::string m_name;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief An NGL transformation attribute
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Transformation m_transform;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief A Bounding Box Extents object to store the new extents of the bounding boxes
    //----------------------------------------------------------------------------------------------------------------------
    BBExtent m_bbExtents;
};

#endif
