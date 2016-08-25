#include <stdexcept>
#include <vector>
#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/graphics.hpp>
#include <fantom/fields.hpp>
using namespace fantom;
namespace
{
class FieldsTutorialAlgorithm : public VisAlgorithm
{
    std::unique_ptr< Primitive > mGlyphs;
public:
    struct Options : public VisAlgorithm::Options {
        Options( Options::Control &control ) : VisAlgorithm::Options( control ) {
            add< TensorFieldInterpolated< 3, Vector3 > >( "Field", "A 3D vector field" );
            add< double >( "Time", "The time when to sample the field.", 0.0 );
            add< double >( "Scale", "The size of the graphics.", 1.0 );
            add< Color >( "Color", "The color of the graphics.", Color( 0.75, 0.75, 0.0 ) );
        }
    };
    struct VisOutputs : public VisAlgorithm::VisOutputs {
        VisOutputs( VisOutputs::Control &control ) : VisAlgorithm::VisOutputs( control ) {
            addGraphics( "Glyphs" );
        }
    };

    FieldsTutorialAlgorithm( InitData &data ) : VisAlgorithm( data ) {}

    virtual void execute( const Algorithm::Options &options, const volatile bool &abortFlag ) override {
        mGlyphs = getGraphics( "Glyphs" ).makePrimitive();
        std::shared_ptr< const TensorFieldContinuous< 3, Vector3 > > field = options.get< TensorFieldContinuous< 3, Vector3 > >( "Field" );
        // if there is no input, do nothing
        if( !field ) {
            debugLog() << "Input Field not set." << std::endl;
            return;
        }
        // sanity check that interpolated fields really use the correct grid type. This should never fail
        std::shared_ptr< const Grid< 3 > > grid = std::dynamic_pointer_cast< const Grid< 3 > >( field->domain() );
        if( !grid ) {
            throw std::logic_error( "Wrong type of grid!" );
        }
        double scale = options.get< double >( "Scale" );
        double time = options.get< double >( "Time" );
        Color color = options.get< Color >( "Color" );
        const ValueArray< Point3 > &points = grid->points();
        std::vector< Vector3 > vertices( 2 * grid->numCells() );
        double maximum = 0.0;
        #pragma omp parallel
        {
            Progress progress( *this, "Processing cells", 0, false );
            size_t i;
            double locMaximum = 0.0;
            auto evaluator = field->makeEvaluator();
            #pragma omp for nowait
            for( i = 0; i < grid->numCells(); ++i ) {
                if( abortFlag ) {
                    continue;
                }
                Cell cell = grid->cell( i );
                Point3 point;
                for( size_t j = 0; j != cell.numVertices(); ++j ) {
                    point += points[cell.index( j )];
                }
                point /= cell.numVertices();
                if( evaluator->reset( point, time ) ) {
                    auto v = evaluator->value();
                    locMaximum = std::max( locMaximum, norm( v ) );
                    vertices[2 * i + 0] = point;
                    vertices[2 * i + 1] = v;
                }
            }
            #pragma omp critical
            maximum = std::max( maximum, locMaximum );
            #pragma omp barrier
            #pragma omp for nowait
            for( i = 0; i < grid->numCells(); ++i ) {
                if( abortFlag ) {
                    continue;
                }
                vertices[2 * i + 1] = vertices[2 * i + 0] + vertices[2 * i + 1] / maximum * scale;
            }
        }
        if( abortFlag ) {
            return;
        }
        mGlyphs->add( Primitive::LINES ).setColor( color ).setVertices( vertices );
    }
};
AlgorithmRegister< FieldsTutorialAlgorithm > dummy( "Tutorial/Fields", "Show vector field glyphs at cell centers." );
} // namespace
