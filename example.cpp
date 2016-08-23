#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>

using namespace fantom;

namespace
{
    class ExampleAlgorithm : public VisAlgorithm
    {

    public:
        struct Options : public VisAlgorithm::Options
        {
            Options( Control& control ) : VisAlgorithm::Options(control)
            {
                // add options here
                add<bool>("3D", "select 3D field", false);
            }
        };

        ExampleAlgorithm( InitData & data) : VisAlgorithm(data)
        {
            // initialize internal data members
        }

        void execute( const Algorithm::Options& options, const volatile bool& /* abortFlag */ ) override
        {
            //add your code here

            auto flag = options.get< bool >( "3D" );
            if( flag )
            {
                infoLog() << "Flag has been set..." << std::endl;
            }
        }
    };

    AlgorithmRegister< ExampleAlgorithm > reg("Vis Praktikum/Example Algorithm", "Meaningful description of what this does...");
}
