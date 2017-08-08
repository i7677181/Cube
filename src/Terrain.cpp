#include "Terrain.h"

///----------------------------------------------------------------------------------------------------------------------
/// Default Terrain constructor
///----------------------------------------------------------------------------------------------------------------------
Terrain::Terrain()
{
}
///----------------------------------------------------------------------------------------------------------------------
/// Creates the height information for the terrain on each side of the world.
/// Imports height map images, rotates them in the appropriate angle, then converts the pixel information into integers between 1 and 8.
/// The integers are going to refer to the heights of the small cubes that are creating the terrain on each side.
///----------------------------------------------------------------------------------------------------------------------
std::vector<int> Terrain::terrainHeight(int _side, int _rotationAngle, bool _mirrored)
{
    /// checks whether or not the imported height map has to be mirrored before converting its data
    bool mirrored=_mirrored;
    /// tells the angle of rotation that the image has to be rotated with before converting its data
    int rotationAngle=_rotationAngle;
    /// the side of the world, varies from 1 to 6
    int side = _side;
    /// temporary qimage variable, it is used to substitute the height maps depending on the side of the world
    QImage img;
    /// qimage variables storing the height maps for each side of the world
    QImage img1( "textures/height_map1.png" );
    QImage img2( "textures/height_map2.png" );
    QImage img3( "textures/height_map4.png" );
    QImage img4( "textures/height_map3.png" );
    QImage img5( "textures/height_map5.png" );
    QImage img6( "textures/height_map6.png" );
    /// stores integers between 1-8
    /// the integers are going to refer to the heights of the small cubes that are creating the terrain on each side
    std::vector<int> terrainHeights(1600);
    /// height of the imported height map
    int height = img1.height();
    /// width of the imported height map
    int width = img1.width();
    /// when importing the height maps only img2, img3 and img5 has to be mirrored before converting their pixel data
    if (mirrored==true)
    {
        img2 = img2.mirrored(true,false);
        img3 = img3.mirrored(true,false);
        img5 = img5.mirrored(true,false);
    }
    /// tells which side of the world to calculate
    if (side==1) {img=img1;}
    else if (side==2) {img=img2;}
    else if (side==3) {img=img3;}
    else if (side==4) {img=img4;}
    else if (side==5) {img=img5;}
    else if (side==6) {img=img6;}
    /// checks the rotation angle and rotates the image depending on it
    if (rotationAngle==0)
    {
        QTransform myTransform;
        myTransform.rotate(0);
        img = img.transformed(myTransform);
    }
    else if (rotationAngle==90)
    {
        QTransform myTransform;
        myTransform.rotate(90);
        img = img.transformed(myTransform);
    }
    else if (rotationAngle==180)
    {
        QTransform myTransform;
        myTransform.rotate(180);
        img = img.transformed(myTransform);
    }
    else if (rotationAngle==270)
    {
        QTransform myTransform;
        myTransform.rotate(270);
        img = img.transformed(myTransform);
    }
    /// checks if the importing was successful
    if ( false == img.isNull() )
    {
        /// counter referring to the terrainHeights index to fill up each slot
        int counter=0;
        /// loops through each pixel on the image and converts the red intensity to an integer between 1-8
        /// 1 refers to the lowest red intensity, 8 refers to the highest red intensity
        for ( int row = 0; row < width; row++ )
        {
            for ( int col = 0; col < height; col++ )
            {
                QColor clrCurrent( img.pixel(row,col) );
                if (clrCurrent.red()>=0 && clrCurrent.red()<32) {terrainHeights[counter]=1;}
                else if (clrCurrent.red()>=32 && clrCurrent.red()<64) {terrainHeights[counter]=2;}
                else if (clrCurrent.red()>=64 && clrCurrent.red()<=96) {terrainHeights[counter]=3;}
                else if (clrCurrent.red()>=96 && clrCurrent.red()<128) {terrainHeights[counter]=4;}
                else if (clrCurrent.red()>=128 && clrCurrent.red()<=161) {terrainHeights[counter]=5;}
                else if (clrCurrent.red()>=161 && clrCurrent.red()<=193) {terrainHeights[counter]=6;}
                else if (clrCurrent.red()>=193 && clrCurrent.red()<225) {terrainHeights[counter]=7;}
                else if (clrCurrent.red()>=225 && clrCurrent.red()<=255) {terrainHeights[counter]=8;}
                counter++;
            }
        }
    }
    return terrainHeights;
}

///----------------------------------------------------------------------------------------------------------------------
/// Initializing terrain variables. Depending on the side and the converted data from the height maps
/// using the values from 1-8, it calculates the scale and the position for the small cubes on each side of the terrain
///----------------------------------------------------------------------------------------------------------------------

void Terrain::initializeTerrain()
{
    /// creating vectors to store the values between 1-8 on each side
    std::vector<int> terrainHeights1;
    std::vector<int> terrainHeights2;
    std::vector<int> terrainHeights3;
    std::vector<int> terrainHeights4;
    std::vector<int> terrainHeights5;
    std::vector<int> terrainHeights6;
    /// temporary vector, used to store one of the terrainHeights vectors depedning on the side
    std::vector<int> terrainHeights;
    /// temporary transformation variable storing the scale and position for one small cube at a time
    ngl::Transformation T1;
    /// initializing the vectors
    for(int i=0;i<1600;i++)
    {
        terrainHeights1.push_back(0);
        terrainHeights2.push_back(0);
        terrainHeights3.push_back(0);
        terrainHeights4.push_back(0);
        terrainHeights5.push_back(0);
        terrainHeights6.push_back(0);
    }
    /// counter used to loop through all the small cubes
    int cubeCounter=0;
    /// filling up the terrain arrays for each side of the world, storing the integers from 1-8 for each small cube
    terrainHeights1 = terrainHeight(1,0,false);
    terrainHeights2 = terrainHeight(2,0,false);
    terrainHeights3 = terrainHeight(3,0,false);
    terrainHeights4 = terrainHeight(4,0,false);
    terrainHeights5 = terrainHeight(5,0,false);
    terrainHeights6 = terrainHeight(6,0,false);
    /// offsets to place the terrains to the appropriate place
    float offset1=0;
    float offset2=-0.375f;
    /// when scaling a small cube, the scale is staying constant for two axis
    float const scaleConst=0.25f;
    /// when scaling a small cube, the scale for on axis is depending on the received integer values between 1-8
    float scaleVar;
    /// the position of the small cube on one axis varies depending on the scale of them
    float posVar;
    /// used to define the position of one small cube with the offsets
    float const posConst=4.5f;
    /// loop going through each side and calculating the transformations for each small cube
    for (int sideCounter=1; sideCounter<7; sideCounter++)
    {
        if (sideCounter>1)
        {
            offset2=-0.375f;
            cubeCounter=0;
        }
        for (float j=0; j<10; j+=0.25f)
        {
            offset1=-0.375f;
            for (float i=0; i<10; i+=0.25f)
            {
                /// depending on the world side and the index of the cube in the array the loop calculates
                /// the position and the scale variable
                for (int inc=1; inc<9; inc++)
                {
                    if (sideCounter==1) {terrainHeights=terrainHeights1;}
                    else if (sideCounter==2) {terrainHeights=terrainHeights2;}
                    else if (sideCounter==3) {terrainHeights=terrainHeights3;}
                    else if (sideCounter==4) {terrainHeights=terrainHeights4;}
                    else if (sideCounter==5) {terrainHeights=terrainHeights5;}
                    else if (sideCounter==6) {terrainHeights=terrainHeights6;}
                    if (terrainHeights[cubeCounter]==inc)
                    {
                        posVar=5.1f+(0.1f*(inc-2));
                        scaleVar=(0.2f*(inc-1));
                    }
                }
                /// depending on the world side the calculated positions and scales are stored as transformations in new arrays
                if (sideCounter==1)
                {
                    T1.setPosition(offset1-posConst,posVar,offset2-posConst);
                    T1.setScale(scaleConst, scaleVar, scaleConst);
                    if (scaleVar==0){T1.setScale(0,0,0);}
                    m_CubePosScale1.push_back(T1);
                }
                else if (sideCounter==2)
                {
                    T1.setPosition(offset1-posConst,offset2-posConst,posVar);
                    T1.setScale(scaleConst, scaleConst, scaleVar);
                    if (scaleVar==0){T1.setScale(0,0,0);}
                    m_CubePosScale2.push_back(T1);
                }
                else if (sideCounter==3)
                {
                    T1.setPosition(posVar, offset1-posConst,offset2-posConst);
                    T1.setScale(scaleVar, scaleConst, scaleConst);
                    if (scaleVar==0){T1.setScale(0,0,0);}
                    m_CubePosScale3.push_back(T1);
                }
                else if (sideCounter==4)
                {
                    T1.setPosition(-posVar, offset1-posConst,offset2-posConst);
                    T1.setScale(-scaleVar, scaleConst, scaleConst);
                    if (scaleVar==0){T1.setScale(0,0,0);}
                    m_CubePosScale4.push_back(T1);
                }
                else if (sideCounter==5)
                {
                    T1.setPosition(offset1-posConst,-posVar,offset2-posConst);
                    T1.setScale(scaleConst, -scaleVar, scaleConst);
                    if (scaleVar==0){T1.setScale(0,0,0);}
                    m_CubePosScale5.push_back(T1);
                }
                else if (sideCounter==6)
                {
                    T1.setPosition(offset1-posConst,offset2-posConst,-posVar);
                    T1.setScale(scaleConst, scaleConst, -scaleVar);
                    if (scaleVar==0){T1.setScale(0,0,0);}
                    m_CubePosScale6.push_back(T1);
                }
            cubeCounter++;
            offset1+=0.25f;
            }
            offset2+=0.25f;
        }
    }
}
