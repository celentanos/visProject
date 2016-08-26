// only include OpenGL if you use a custom drawer. Otherwise,
// there is no need to include any extra header
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/graphics.hpp>
using namespace fantom;
namespace
{
class GraphicsTutorialAlgorithm : public VisAlgorithm
{
//    std::unique_ptr< Primitive > mX;
//    std::unique_ptr< Primitive > mY;
//    std::unique_ptr< Primitive > mZ;
//    std::unique_ptr< Primitive > mText;
    std::unique_ptr< Primitive > mTriangle;
    std::unique_ptr< Primitive > mBlinnPhong;
    std::unique_ptr< Primitive > mGeometry;
//    std::unique_ptr< Primitive > mTexture;
//    std::unique_ptr< Primitive > mCustom;
public:
    struct VisOutputs : public VisAlgorithm::VisOutputs {
        VisOutputs( fantom::VisOutputs::Control &control ) : VisAlgorithm::VisOutputs( control ) {
//            addGraphics( "x" );
//            addGraphics( "y" );
//            addGraphics( "z" );
//            addGraphics( "text" );
            addGraphics( "triangle" );
            addGraphics( "Blinn-Phong" );
            addGraphics( "geometry" );
//            addGraphics( "texture" );
//            addGraphics( "custom" );
        }
    };

    GraphicsTutorialAlgorithm( InitData &data ) : VisAlgorithm( data ) {
    }

    virtual void execute( const Algorithm::Options & /*options*/, const volatile bool & /*abortFlag*/ ) override {
//        mX = getGraphics( "x" ).makePrimitive();
//        mY = getGraphics( "y" ).makePrimitive();
//        mZ = getGraphics( "z" ).makePrimitive();
//        mText = getGraphics( "text" ).makePrimitive();
        mTriangle = getGraphics( "triangle" ).makePrimitive();
        mBlinnPhong = getGraphics( "Blinn-Phong" ).makePrimitive();
        mGeometry = getGraphics( "geometry" ).makePrimitive();
//        mTexture = getGraphics( "texture" ).makePrimitive();
//        mCustom = getGraphics( "custom" ).makePrimitive();
        // example high-level primitive interface
//        mX->addArrow( Point3( 0.75, 0.0, 0.0 ), Vector3( 0.25, 0.0, 0.0 ), 0.02, Color( 1.0, 0.0, 0.0, 1.0 ) );
//        mY->addArrow( Point3( 0.0, 0.75, 0.0 ), Vector3( 0.0, 0.25, 0.0 ), 0.02, Color( 0.0, 1.0, 0.0, 1.0 ) );
//        mZ->addArrow( Point3( 0.0, 0.0, 0.75 ), Vector3( 0.0, 0.0, 0.25 ), 0.02, Color( 0.0, 0.0, 1.0, 1.0 ) );
//        mX->addSphere( Point3( -1.0, 0.0, 0.0 ), 0.25, Color( 1.0, 0.0, 0.0, 1.0 ) );
//        mY->addSphere( Point3( 0.0, -1.0, 0.0 ), 0.25, Color( 0.0, 1.0, 0.0, 1.0 ) );
//        mZ->addSphere( Point3( 0.0, 0.0, -1.0 ), 0.25, Color( 0.0, 0.0, 1.0, 1.0 ) );
        // example text label
//        mText->addTextLabel( Point3( 0.0, 0.0, 1.0 ), "Hello World !", 14 );
        // example low-level primitive interface
        /// Triangle ###########################################################
        std::vector< Point3 > tri( 3 );
        tri[0] = Point3( 0.0, 0.0, 0.0 );
        tri[1] = Point3( 1.0, 2.0, 1.0 );
        tri[2] = Point3( 0.0, -1.0, -1.0 );
        mTriangle->add( Primitive::LINE_LOOP )
        .setLineWidth( 5.0 )
        .setColor( Color( 0.6, 0.6, 0.6, 1.0 ) )
        .setVertices( tri );
        /// Blinn-Phong ########################################################
        // example fragment shader interface
        tri[1] *= -1.0;
        tri[2] *= -1.0;
        std::vector< Color > col( 3, Color( 0.0, 0.0, 0.0, 1.0 ) );
        col[0] = Color( 1.0, 0.0, 0.0, 0.5 );
        col[1] = Color( 0.0, 1.0, 0.0, 0.5 );
        col[2] = Color( 0.0, 0.0, 1.0, 0.5 );
        mBlinnPhong->setShaders( resourcePath() + "shaders/BlinnPhong-vertex.glsl",
                                 resourcePath() + "shaders/BlinnPhong-fragment.glsl" );
        mBlinnPhong->setBlending( true );
        mBlinnPhong->setUniform( "Transparency", 1.0f );
        mBlinnPhong->add( Primitive::TRIANGLES ).setColors( col ).setVertices( tri );
        // example geometry shader usage: each line is duplicated swapping x- and y-coordinates
        /// Line ###############################################################
        std::vector< Point3 > v( 2 );
        v[0] = Point3( -1.5, -1.0, 0.0 );
        v[1] = Point3( 1.5, 1.0, 0.0 );
//        mGeometry->setShaders( resourcePath() + "Tutorial/swizzle-vertex.glsl",
//                               resourcePath() + "Tutorial/swizzle-fragment.glsl",
//                               resourcePath() + "Tutorial/swizzle-geometry.glsl",
//                               Primitive::IN_LINES,
//                               Primitive::OUT_LINE_STRIP,
//                               4 );
        mGeometry->add( Primitive::LINES ).setVertices( v );
        // example texture mapped primitive drawing
//        std::unique_ptr< Texture > texture2D = makeTexture( resourcePath() + "images/FAnToM-logo.jpg" );
//        std::unique_ptr< Texture > texture3D = makeTexture( false, 256, 64, 32 );
//        for( size_t k = 0; k != texture3D->depth(); ++k ) {
//            for( size_t j = 0; j != texture3D->height(); ++j ) {
//                for( size_t i = 0; i != texture3D->width(); ++i ) {
//                    size_t column = ( i * texture2D->width() ) / texture3D->width();
//                    size_t row = ( j * texture2D->height() ) / texture3D->height();
//                    texture3D->set( texture2D->get( column, row ), i, j, k );
//                }
//            }
//        }
//        std::vector< Point3 > cube( 8 );
//        cube[0] = Point3( -0.5, -0.5, 0.5 );
//        cube[1] = Point3( 0.5, -0.5, 0.5 );
//        cube[2] = Point3( 0.5, 0.5, 0.5 );
//        cube[3] = Point3( -0.5, 0.5, 0.5 );
//        cube[4] = Point3( -0.5, -0.5, -0.5 );
//        cube[5] = Point3( 0.5, -0.5, -0.5 );
//        cube[6] = Point3( 0.5, 0.5, -0.5 );
//        cube[7] = Point3( -0.5, 0.5, -0.5 );
//        std::vector< Point3 > texCoords( 8 );
//        texCoords[0] = Point3( 0.0, 0.0, 1.0 );
//        texCoords[1] = Point3( 1.0, 0.0, 1.0 );
//        texCoords[2] = Point3( 1.0, 1.0, 1.0 );
//        texCoords[3] = Point3( 0.0, 1.0, 1.0 );
//        texCoords[4] = Point3( 0.0, 0.0, 0.0 );
//        texCoords[5] = Point3( 1.0, 0.0, 0.0 );
//        texCoords[6] = Point3( 1.0, 1.0, 0.0 );
//        texCoords[7] = Point3( 0.0, 1.0, 0.0 );
//        std::vector< unsigned int > sides( 6 * 4 );
//        sides[0] = 0;
//        sides[1] = 1;
//        sides[2] = 2;
//        sides[3] = 3;
//        sides[4] = 1;
//        sides[5] = 5;
//        sides[6] = 6;
//        sides[7] = 2;
//        sides[8] = 4;
//        sides[9] = 0;
//        sides[10] = 3;
//        sides[11] = 7;
//        sides[12] = 4;
//        sides[13] = 5;
//        sides[14] = 1;
//        sides[15] = 0;
//        sides[16] = 3;
//        sides[17] = 2;
//        sides[18] = 6;
//        sides[19] = 7;
//        sides[20] = 5;
//        sides[21] = 4;
//        sides[22] = 7;
//        sides[23] = 6;
//        mTexture->setTexture( 0, *texture3D );
//        mTexture->add( Primitive::QUADS ).setTexCoords( 0, texCoords ).setVertices( cube, sides );
//        mCustom->addCustom( makeTriangleDrawer );
    }
//    struct GlTriangleDrawer : public CustomDrawer {
//        GLuint list;
//        GlTriangleDrawer() : list( glGenLists( 1 ) ) {
//            glNewList( list, GL_COMPILE );
//            glColor3f( 1.0f, 0.0f, 0.0f );
//            glNormal3f( 0.0f, 0.0f, 1.0f );
//            glBegin( GL_TRIANGLES );
//            glVertex2f( 0.0f, 0.0f );
//            glVertex2f( 0.0f, 2.0f );
//            glVertex2f( 2.0f, 0.0f );
//            glEnd();
//            glEndList();
//        }
//        ~GlTriangleDrawer() {
//            glDeleteLists( list, 1 );
//        }
//        virtual void draw() const {
//            glPushAttrib( GL_ALL_ATTRIB_BITS );
//            glCallList( list );
//            glPopAttrib();
//        }
//    };
//    static std::unique_ptr< CustomDrawer > makeTriangleDrawer() {
//        return std::unique_ptr< CustomDrawer >( new GlTriangleDrawer() );
//    }
};
AlgorithmRegister< GraphicsTutorialAlgorithm > dummy( "Tutorial/Graphics", "Show some example graphics." );
} // namespace
