#include <fstream>
#include <sstream>
#include <vector>

#include <fantom/algorithm.hpp>
#include <fantom/fields.hpp>
#include <fantom/register.hpp>

using namespace fantom;
namespace
{

/**
* Interpolates scattered data values on another set of points
*/
class InterpolateGeoDataAlgorithm
    : public DataAlgorithm
{
public:

    struct Options : public DataAlgorithm::Options {
    public:
        Options( Algorithm::Options::Control &control ) : DataAlgorithm::Options( control ) {
            add < DomainBase >( "Points", "A point set or grid" );
            add < TensorFieldDiscrete<Scalar> >( "Field", "A scattered scalar field" );
        }
    };

    struct DataOutputs : public DataAlgorithm::DataOutputs {
    public:
        DataOutputs( Control &control ): DataAlgorithm::DataOutputs( control ) {
            add< TensorFieldBase >( "Interpolated" );
        }
    };

    InterpolateGeoDataAlgorithm( InitData &data ) : DataAlgorithm( data ) {
    }

    void execute( const Algorithm::Options &parameters, const volatile bool &abortFlag ) {
        // get the point set
//        auto domain = parameters.get< DomainBase >( "Points" );

        // get the tensor field
//        auto field = parameters.get< TensorFieldDiscrete<Scalar> >( "Field" );

        // iteriere durch alle Datenpunkte, Bestime die Werte mittels Interpolationsformel

        std::vector< Scalar > values;

        // ...

        // auto field  = DomainFactory::makeTensorField( *domainND, temperatures );

        // setResult( "Interpolated", field );
    }

public:
};

AlgorithmRegister<InterpolateGeoDataAlgorithm> dummy( "Interpolate", "Interpolate in a data set." );

} // namespace
