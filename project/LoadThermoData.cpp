#include <vector>
#include <cstring>
#include <string>

#include <fantom/algorithm.hpp>
#include <fantom/fields.hpp>
#include <fantom/register.hpp>

#include "mmv/MultiModalVideo.hpp"

using namespace fantom;
using namespace std;
namespace
{

class LoadThermoDataAlgorithm : public DataAlgorithm
{
public:

    struct Options : public DataAlgorithm::Options {
    public:
        Options(Algorithm::Options::Control &control) : DataAlgorithm::Options(control) {
            add<InputLoadPath>("Input File", "The file to be read", "");
            add<int>("Start Frame", "The frame to start Loading", 0);
            add<int>("End Frame", "The frame to end Loading", 100);
        }
    };

    struct DataOutputs : public DataAlgorithm::DataOutputs {
    public:
        DataOutputs(Control &control)
            : DataAlgorithm::DataOutputs(control) {
            add<DomainBase>("Points");
            add < Grid < 2 > > ("Grid");
            add < TensorFieldInterpolated < 2, Scalar >> ("Temperatures");
        }
    };

    LoadThermoDataAlgorithm(InitData &data) : DataAlgorithm(data) {}

    void execute(const Algorithm::Options &parameters, const volatile bool &abortFlag) {
        // Locale setzen, damit . , ist
        std::setlocale(LC_ALL, "en_US.UTF-8");

        int startFrame = parameters.get<int>("Start Frame");
        int endFrame = parameters.get<int>("End Frame");
        char *path = new char[parameters.get<std::string>("Input File").length() + 1];
        std::strcpy(path, parameters.get<std::string>("Input File").c_str());
        debugLog() << endFrame << std::endl;
        // Test, ob die Datei gesetzt ist, sonst nichts machen
        if (parameters.get<std::string>("Input File") != "") {

            MultiModalVideo mmv(path, 382, 288, startFrame);

            //Zähler für die Frames
            int frameCounter = 0;
            const double timeStep = 1 / 27.0;

            std::vector<double> positions[2];
            std::vector <Scalar> temperatures;
            std::vector<double> timeSteps;

            for (int y = 0; y < 288; y++) {
                positions[1].push_back((double) y);
            }

            for (int x = 0; x < 382; x++) {
                positions[0].push_back((double) x);
            }

            // Schleife zum Einlesen der Daten
            while (!abortFlag) {

                unsigned short *data = mmv.getCurrentFrame()->getThermoDataFrame()->getThermoData();

                for (int y = 0; y < 288; y++) {

                    for (int x = 0; x < 382; x++) {

                        double temp = ((double) * (data + (y * 382 + x))) / 100;
                        temperatures.push_back(Scalar(temp));
                    }
                }

                frameCounter++;
                timeSteps.push_back(timeStep * frameCounter);

                if (!mmv.next() || (endFrame > 0 && frameCounter == endFrame)) break;
            }

            debugLog() << "generating domain..." << endl;
            const shared_ptr<const DiscreteDomain<2>> domain = DomainFactory::makeDomainRectilinear(positions);
            const shared_ptr<const Grid<2>> grid = DomainFactory::makeGridStructured(*domain);

            debugLog() << "generating field..." << endl;
            debugLog() << "tempsize: " << temperatures.size() << " positionssize: " << positions->size() <<
                       " pointsize: " << domain->numPoints()
                       << " grid points: " << grid->numPoints()
                       << " timesteps: " << timeSteps.size() << endl;

            shared_ptr<const TensorFieldInterpolated<2, Scalar>>
                    field = DomainFactory::makeTensorField(*grid, temperatures, Precision::FLOAT64,
                            DomainType::points, timeSteps);
            debugLog() << "setting result..." << endl;
            setResult("Points", domain);
            setResult("Grid", grid);
            setResult("Temperatures", field);

            delete path;
        }
    }

public:
    IMPLEMENT_CAN_HANDLE(".mmv")

    static bool loaderSetOptions(Algorithm::Options &options, std::vector <std::string> &filenames) {
        // find the name we will handle
        for (auto it = filenames.begin(); it != filenames.end(); ++it) {
            if (it->substr(it->size() - 4, 4) == ".mmv") {
                options.set<std::string>("Input File", *it);
                it = filenames.erase(it);
                return true;
            }
        }
        return false;
    }

    std::vector <std::string> &split(const std::string &s, char delim, std::vector <std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }
};

AlgorithmRegister <LoadThermoDataAlgorithm> dummy("Load/ThermoData", "Load ThermoData dataset.");
DataLoaderRegister <LoadThermoDataAlgorithm> dummy2("Load/ThermoData");

}
