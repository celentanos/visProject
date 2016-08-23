#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/graphics.hpp>
#include <fantom/fields.hpp>


using namespace fantom;

namespace
{

class ShowCellGridAlgorithm : public VisAlgorithm
{
    std::unique_ptr<Primitive> mColorMap;

public:
    struct VisOutputs : public VisAlgorithm::VisOutputs {
        VisOutputs(fantom::VisOutputs::Control &control) : VisAlgorithm::VisOutputs(control) {
            addGraphics("colormap");
        }
    };

    struct Options : public DataAlgorithm::Options {
    public:
        Options(Algorithm::Options::Control &control) : DataAlgorithm::Options(control) {
            // data
            add<Grid<2>>("Domain", "");
            add<TensorFieldDiscrete<Scalar> >("Field", "");

            // settings
            add<double>("TempMin", "", -10.0);
            add<double>("TempMax", "", 30.0);
        }
    };


    ShowCellGridAlgorithm(InitData &data) : VisAlgorithm(data) {}

    virtual void execute(const Algorithm::Options &options, const volatile bool & /*abortFlag*/ ) override {
        auto domain = options.get<Grid<2> >("Domain");
        auto field = options.get<const TensorFieldDiscrete<Scalar> >("Field");
        auto tempMin = options.get<double>("TempMin");
        auto tempMax = options.get<double>("TempMax");

        mColorMap.reset();
        mColorMap = getGraphics("colormap").makePrimitive();

        if (!field) {
            debugLog() << "Field not connected." << std::endl;
            return;
        }

        if (!domain) {
            auto domain = std::dynamic_pointer_cast<const DiscreteDomain<2> >(field->domain());
        }

        if (!domain) {
            debugLog() << "No domain found!" << std::endl;
            return;
        }

        if (domain->numPoints() != field->values().size()) {
            debugLog() << "Temperature value count doesn't match points count!" << std::endl;
            return;
        }

        // check if the given grid is structured and rectinlinear or uniform; but other forms might also work
        if (domain->topologyType() != TopologyType::STRUCTURED) {
            debugLog() << "Grid is not structured!" << std::endl;
            return;
        }

        if (domain->structuringType() != StructuringType::RECTILINEAR &&
            domain->structuringType() != StructuringType::UNIFORM) {
            debugLog() << "Unsupported grid structure type!" << std::endl;
            return;
        }

        // use evaluator for time specific values
        auto eval = field->makeDiscreteEvaluator();

        // needed data fields
        std::vector<Vector3> vertices;
        std::vector<unsigned int> indices;
        std::vector<Color> colors;

        // fill data fields from field and domain
        for (size_t x = 0; x < domain->points().size(); ++x) {
            vertices.push_back(toPoint3(domain->points()[x]));
            colors.push_back(getRGBColorFromTemperature(tempMin, tempMax, eval->value(x)[0]));
        }


        // create cells
        debugLog() << "Draw " << domain->numCells() << " cells" << std::endl;

        // iterate through all cells
        for (size_t x = 0; x < domain->numCells(); ++x) {
            // get current cell
            auto currentCell = domain->cell(x);

            // get vertices of cell and draw polygon with y vertices
            for (size_t y = 0; y < currentCell.numVertices(); ++y) {
                // map cell vertex index to vertex index in domain
                auto domainIndexOfVertex = currentCell.index(y);
                // add index to ordered vector for the right access of vertex color and position
                indices.push_back(domainIndexOfVertex);
            }
        }

        // draw color map
        // use colors in the same order as the vertex positions
        // use the indices to reference the colors and use vertex positions
        mColorMap->add(Primitive::QUADS).setColors(colors).setVertices(vertices, indices);

    }

private:
    Color getRGBColorFromTemperature(double min, double max, double temperature) {
        if (temperature < min) temperature = min;
        else if (temperature > max) temperature = max;

        temperature = (temperature - min) / (max - min);
        double r = std::min(std::max(0.0, 1.5 - fabs(1.0 - 4.0 * (temperature - 0.5))), 1.0);
        double g = std::min(std::max(0.0, 1.5 - fabs(1.0 - 4.0 * (temperature - 0.25))), 1.0);
        double b = std::min(std::max(0.0, 1.5 - fabs(1.0 - 4.0 * (temperature))), 1.0);

        return Color(r, g, b);
    }

    double lerp(double normVal, float minValue, float maxValue) const {
        return minValue * (1 - normVal) + maxValue * (normVal);
    }

    int matrixToArrayIndize(int x, int y, int xSize, int ySize) {
        return (x + y * ySize);
    }
};

AlgorithmRegister<ShowCellGridAlgorithm> dummy("Vis Praktikum/Show Cell Grid", "Show cell grid.");
} // namespace
