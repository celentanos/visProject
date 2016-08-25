#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/fields.hpp>
using namespace fantom;
namespace
{
class CommitTutorialAlgorithm : public DataAlgorithm
{
public:
    struct Options : public DataAlgorithm::Options {
        Options( fantom::Options::Control &control ) : DataAlgorithm::Options( control ) {
            add< long >( "nx", "", 10 );
            add< long >( "ny", "", 10 );
            add< long >( "nz", "", 10 );
            add< long >( "nt", "", 10 );
            addSeparator();
            add< double >( "dx", "", 1.0 );
            add< double >( "dy", "", 1.0 );
            add< double >( "dz", "", 1.0 );
            add< double >( "dt", "", 1.0 );
        }
    };
    struct DataOutputs : public DataAlgorithm::DataOutputs {
        DataOutputs( fantom::DataOutputs::Control &control ) : DataAlgorithm::DataOutputs( control ) {
            add< Grid< 3 > >( "grid" );
            add< TensorFieldBase >( "velocity" );
            addBundle< TensorFieldBase >( "velocity fields" ); // test: add "velocity" as a bundle
        }
    };

    CommitTutorialAlgorithm( InitData &data ) : DataAlgorithm( data ) {
    }

    virtual void execute( const Algorithm::Options &options, const volatile bool & /*abortFlag*/ ) override {
        size_t extent[] = { (size_t)options.get< long >( "nx" ),
                            (size_t)options.get< long >( "ny" ),
                            (size_t)options.get< long >( "nz" )
                          };
        double origin[] = { -0.5 * options.get< double >( "dx" ) *( options.get< long >( "nx" ) - 1 ),
                            -0.5 * options.get< double >( "dy" ) *( options.get< long >( "ny" ) - 1 ),
                            -0.5 * options.get< double >( "dz" ) *( options.get< long >( "nz" ) - 1 )
                          };
        double spacing[] = { options.get< double >( "dx" ), options.get< double >( "dy" ), options.get< double >( "dz" ) };
        std::vector< double > timeSteps( std::max( 1L, options.get< long >( "nt" ) ) );
        for( size_t i = 0; i != timeSteps.size(); ++i ) {
            timeSteps[i] = i * options.get< double >( "dt" );
        }
        std::shared_ptr< const DiscreteDomain< 3 > > mDomain = DomainFactory::makeDomainUniform( extent, origin, spacing );
        std::vector< Vector3 > values( mDomain->numPoints() * timeSteps.size() );
        const double M[] = { -0.5, -0.2, 0.1, 0.5, 1.1, 0.1, -0.3, 0.0, 0.0, 1.0, -1.1, -0.5 };
        Tensor< double, 3, 4 > m( M );
        size_t k = 0;
        for( size_t n = 0; n != timeSteps.size(); ++n ) {
            for( size_t i = 0; i != mDomain->numPoints(); ++i ) {
                double t = timeSteps[n];
                Point3 p = mDomain->points()[i];
                values[k++] = m * Vector4( p[0], p[1], p[2], t );
            }
        }
        std::shared_ptr< const Grid< 3 > > mGrid = DomainFactory::makeGridStructured( *mDomain );
        std::shared_ptr< const TensorFieldBase > mField = DomainFactory::makeTensorField(
                    *mGrid, std::move( values ), Precision::FLOAT64, DomainType::points, timeSteps );
        setResult( "grid", mGrid );
        setResult( "velocity", mField );
        std::shared_ptr< DataObjectBundle > bundle = std::make_shared< DataObjectBundle >();
        bundle->addContent( mField );
        bundle->addContent( mField );
        setResult( "velocity fields", bundle );
    }
};
AlgorithmRegister< CommitTutorialAlgorithm > dummy( "Tutorial/Commit", "Generate a simple vector field." );
}
